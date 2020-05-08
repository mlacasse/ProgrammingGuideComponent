#include "EPGTimeListAdapter.h"

#include "epg/utility/NodeUtilities.h"

#include <asset/YiAssetViewTemplate.h>
#include <scenetree/YiTextSceneNode.h>
#include <view/YiPooledViewRecycler.h>

#define TIME_ASSET_TEMPLATE "EPG_Time-List-Item"
static float CELL_WIDTH_TO_MINUTES = 60.0f;

#define LOG_TAG "EPGTimeListAdapter"

EPGTimeListAdapter::EPGTimeListAdapter() : CYIViewAdapter(std::make_shared<CYIPooledViewRecycler>())
{
}

std::shared_ptr<CYIAssetViewTemplate> EPGTimeListAdapter::GetViewTemplate(size_t /*index*/) const
{
    return CYIViewTemplate::GetViewTemplate(TIME_ASSET_TEMPLATE);
}

size_t EPGTimeListAdapter::GetItemsCount() const
{
    return m_listElements.size();
}

glm::vec3 EPGTimeListAdapter::GetInitialNodeSize(size_t index) const
{
    return m_listElements[index].Size;
}

void EPGTimeListAdapter::BuildElements(const CYIDateTime &startTime, const CYIDateTime &endTime)
{
    Clear();

    CYIDateTime hours = startTime;
    
    if (hours.GetMinute() != 0)
    {
        float scale = CalculateScaleForMinutes(60 - hours.GetMinute());
        InsertElement(FormatTime(hours, true), scale);
        
        hours.SetMinute(0);
        hours.AddHours(1);
    }
    
    size_t numHours = (endTime - startTime) / (60 * 60);
    
    for (size_t i = 0; i < numHours; ++i)
    {
        CYIString timeString = FormatTime(hours, true);
        
        hours.AddHours(1);
        float scale;
        
        // cap the size of the last element if it extends beyond the list
        auto deltaBeyondEnd = hours - endTime;
        if (deltaBeyondEnd > 0)
        {
            scale = CalculateScaleForMinutes(60 - (deltaBeyondEnd / 60));
        }
        else
        {
            scale = CalculateScaleForMinutes(60);
        }
        
        InsertElement(timeString, scale);
    }
}

void EPGTimeListAdapter::Clear()
{
    if (m_listElements.size() > 0)
    {
        for (int32_t i = static_cast<int32_t>(m_listElements.size()) - 1; i >= 0; --i)
        {
            ItemRemovedAtIndex.Emit(i);
        }

        m_listElements.clear();
    }
}

void EPGTimeListAdapter::InsertElement(const CYIString &text, const float horizontalScaleFactor)
{
    auto pTemplate = GetViewTemplate(0)->GetTemplate();
    float width = pTemplate->GetWidth() * horizontalScaleFactor;
    float height = pTemplate->GetHeight();
    glm::vec3 size = glm::vec3(width, height, 0.1f);
    
    m_listElements.emplace_back(text, size);
    
    ItemAddedAtIndex.Emit(m_listElements.size() - 1);
}

void EPGTimeListAdapter::PopulateView(size_t index, CYISceneView *pView)
{
    const auto &item = m_listElements[index];
    CYITextSceneNode *pTimeText = Utility::GetNode<CYITextSceneNode>(pView, "Time-Text");
    pTimeText->SetText(item.Text);
    pView->SetSize(item.Size);
}

float EPGTimeListAdapter::CalculateScaleForMinutes(uint64_t minutes) const
{
    return minutes / CELL_WIDTH_TO_MINUTES;
}

CYIString EPGTimeListAdapter::FormatTime(const CYIDateTime &time, bool appendMeridiem) const
{
    CYIString timeString = time.FormatString("%I:%M");
    if (timeString.StartsWith("0"))
    {
        timeString.RemovePrefix(1);
    }

    if (appendMeridiem)
    {
        timeString.Append(time.GetHour() >= 12 ? "p" : "a");
    }

    return timeString;;
}

#undef TIME_ASSET_TEMPLATE
