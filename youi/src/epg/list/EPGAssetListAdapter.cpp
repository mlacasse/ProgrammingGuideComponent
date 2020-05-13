#include "EPGAssetListAdapter.h"

#include "epg/list/EPGChannelListView.h"
#include "epg/model/EPGAssetModel.h"
#include "epg/model/EPGChannelModel.h"
#include "epg/view/EPGAssetView.h"

#include <asset/YiAssetViewTemplate.h>
#include <layout/YiLinearLayout.h>
#include <utility/YiDateTime.h>

#include <glm/glm.hpp>

#define LOG_TAG "EPGAssetListAdapter"

#define EPG_CELL_TEMPLATE_NAME "EPG_Timeslot-root"
#define EPG_CELL_BLANK_TEMPLATE_NAME "EPG_Timeslot-Filler-root"

static float CELL_WIDTH_TO_SECONDS = 3600.0f;

EPGAssetListAdapter::EPGAssetListAdapter(const std::shared_ptr<EPGChannelModel> &pChannelModel, EPGChannelListView *pList, std::shared_ptr<IYIViewRecycler> pViewRecycler)
    : CYIViewAdapter(pViewRecycler)
    , m_pChannelModel(pChannelModel)
    , m_pListView(pList)
{}

void EPGAssetListAdapter::BuildElements(const CYIDateTime &startTime, const CYIDateTime &endTime)
{
    m_listWidth = CalculateScaleForSeconds(endTime - startTime) * CYIViewTemplate::GetViewTemplate(EPG_CELL_TEMPLATE_NAME)->GetTemplate()->GetWidth();
    
    m_listElements.clear();
    
    CYILinearLayout *pLayout = YiDynamicCast<CYILinearLayout>(m_pListView->GetLayout());
    float listSpacing = 0;
    if (pLayout)
    {
        listSpacing = pLayout->GetSpacing();
    }
    
    auto models = m_pChannelModel->GetAssetModels();
    for (size_t i = 0; i < models.size(); ++i)
    {
        std::shared_ptr<EPGAssetModel> pModel = m_pChannelModel->GetModelAtIndex(i);
        
        // We're out of the time range of the EPG
        if (startTime - pModel->GetEndTime() > 0 || endTime - pModel->GetStartTime() < 1)
        {
            continue;
        }
        
        //check for gaps at the beginning
        if (i == 0 && pModel->GetStartTime() - startTime > 0)
        {
            InsertEmptyElement(pModel->GetStartTime() - startTime, listSpacing);
        }
        
        // cap the size of the element if it extends beyond the list
        auto deltaBeyondStart = glm::max((startTime - pModel->GetStartTime()), CYIDateTime::TimeDelta(0));
        auto deltaBeyondEnd = glm::max((pModel->GetEndTime() - endTime), CYIDateTime::TimeDelta(0));
        float scale = CalculateScaleForSeconds(pModel->GetDuration() - deltaBeyondEnd - deltaBeyondStart);
        
        InsertElementForTemplate(pModel, EPG_CELL_TEMPLATE_NAME, scale, listSpacing);
        
        //check for gaps at the end
        CYIDateTime nextStartTime;
        if (i + 1 >= m_pChannelModel->GetAssetCount() || m_pChannelModel->GetModelAtIndex(i + 1)->GetStartTime() - endTime > 0)
        {
            nextStartTime = endTime;
        }
        else
        {
            nextStartTime = m_pChannelModel->GetModelAtIndex(i + 1)->GetStartTime();
        }
        
        auto endDelta = nextStartTime - pModel->GetEndTime();
        if (endDelta > 0)
        {
            InsertEmptyElement(endDelta, listSpacing);
        }
    }
}

float EPGAssetListAdapter::CalculateScaleForSeconds(int64_t seconds)
{
    return seconds / CELL_WIDTH_TO_SECONDS;
}

void EPGAssetListAdapter::InsertEmptyElement(int64_t duration, const float listSpacing)
{
    float scaleFactor = CalculateScaleForSeconds(duration);
    InsertElementForTemplate(nullptr,
                             EPG_CELL_BLANK_TEMPLATE_NAME,
                             scaleFactor,
                             listSpacing);
}

void EPGAssetListAdapter::InsertElementForTemplate(const std::shared_ptr<EPGAssetModel> &pModel,
                                                   const CYIString &templateName,
                                                   const float horizontalScaleFactor,
                                                   const float listSpacing)
{
    auto pTemplate = CYIViewTemplate::GetViewTemplate(templateName)->GetTemplate();
    float width = pTemplate->GetWidth();
    float height = pTemplate->GetHeight();
    glm::vec3 size = glm::vec3(width * horizontalScaleFactor, height, 0.1f);
    size.x -= listSpacing;
    
    m_listElements.emplace_back(pModel, size);
}

std::shared_ptr<CYIAssetViewTemplate> EPGAssetListAdapter::GetViewTemplate(size_t index) const
{
    const CYIString templateName = m_listElements[index].AssetModel == nullptr ? EPG_CELL_BLANK_TEMPLATE_NAME : EPG_CELL_TEMPLATE_NAME;
    return CYIViewTemplate::GetViewTemplate(templateName);
}

glm::vec3 EPGAssetListAdapter::GetInitialNodeSize(size_t index) const
{
    return m_listElements[index].CardSize;
}

size_t EPGAssetListAdapter::GetItemsCount() const
{
    return m_listElements.size();
}

void EPGAssetListAdapter::PopulateView(size_t index, CYISceneView *pView)
{
    std::shared_ptr<EPGAssetModel> pModel = m_listElements[index].AssetModel;

    auto pEPGAssetView = YiDynamicCast<EPGAssetView>(pView);

    pEPGAssetView->SetScrollPosition(m_pListView->GetHorizontalPosition());
    pEPGAssetView->SetCardSize(m_listElements[index].CardSize);
    pEPGAssetView->Format(pModel, static_cast<int32_t>(index));

    pEPGAssetView->SetListView(m_pListView);
    pEPGAssetView->SetChannelWidth(m_listWidth);
    m_pListView->HorizontalPositionChanged.Connect(*pEPGAssetView, &EPGAssetView::UpdateContentAlignment);

    pEPGAssetView->AssetSelected.Connect(*this, &EPGAssetListAdapter::OnAssetSelected);
    pEPGAssetView->AssetGainedFocus.Connect(*this, &EPGAssetListAdapter::OnAssetGainedFocus);
    pEPGAssetView->AssetLostFocus.Connect(*this, &EPGAssetListAdapter::OnAssetLostFocus);
}

void EPGAssetListAdapter::OnReleaseView(size_t index, CYISceneView *pView)
{
    auto pEPGAssetView = dynamic_cast<EPGAssetView*>(pView);

    m_pListView->HorizontalPositionChanged.Disconnect(*pEPGAssetView, &EPGAssetView::UpdateContentAlignment);

    pEPGAssetView->AssetSelected.Disconnect(*this, &EPGAssetListAdapter::OnAssetSelected);
    pEPGAssetView->AssetGainedFocus.Disconnect(*this, &EPGAssetListAdapter::OnAssetGainedFocus);
    pEPGAssetView->AssetLostFocus.Disconnect(*this, &EPGAssetListAdapter::OnAssetLostFocus);
}

int32_t EPGAssetListAdapter::GetIndexForAsset(const CYIString &assetId)
{
    if (assetId.IsNotEmpty())
    {
        for (size_t i = 0; i < m_listElements.size(); ++i)
        {
            if (m_listElements[i].AssetModel && m_listElements[i].AssetModel->GetResourceId() == assetId)
            {
                return static_cast<int32_t>(i);
            }
        }
    }

    return -1;
}

void EPGAssetListAdapter::OnAssetSelected(int32_t index)
{
    AssetSelected.Emit(m_listElements[index].AssetModel, m_pChannelModel);
}

void EPGAssetListAdapter::OnAssetGainedFocus(int32_t index)
{
    AssetGainedFocus.Emit(m_listElements[index].AssetModel, m_pChannelModel);
}
 
void EPGAssetListAdapter::OnAssetLostFocus(int32_t index)
{
    AssetLostFocus.Emit(m_listElements[index].AssetModel, m_pChannelModel);
}

#undef EPG_CELL_TEMPLATE_NAME
#undef EPG_CELL_BLANK_TEMPLATE_NAME
