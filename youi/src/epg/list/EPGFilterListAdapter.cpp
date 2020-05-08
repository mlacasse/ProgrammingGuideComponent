#include "EPGFilterListAdapter.h"

#include "epg/utility/NodeUtilities.h"

#include <asset/YiAssetViewTemplate.h>
#include <scenetree/YiTextSceneNode.h>
#include <view/YiListView.h>
#include <view/YiToggleButtonView.h>

#define FILTER_ASSET_TEMPLATE_NAME "EPG_SideMenu-ListItem"

#define LOG_TAG "EPGFilterListAdapter"

EPGFilterListAdapter::EPGFilterListAdapter(CYIListView *pListView)
    : m_pListView(pListView)
{
}

std::shared_ptr<CYIAssetViewTemplate> EPGFilterListAdapter::GetViewTemplate(size_t index) const
{
    return CYIViewTemplate::GetViewTemplate(FILTER_ASSET_TEMPLATE_NAME);
}

size_t EPGFilterListAdapter::GetItemsCount() const
{
    return m_listElements.size();
}

void EPGFilterListAdapter::BuildElements()
{
    Clear();
    
    // TEMP DATA
    m_listElements.emplace_back("All Channels", true);
    m_listElements.emplace_back("Favorite", false);
    m_listElements.emplace_back("Movies", false);
    m_listElements.emplace_back("TV Shows", false);
    m_listElements.emplace_back("Sports", false);
    m_listElements.emplace_back("Kids", false);
    
    m_selectedFilterIndex = 0;
    
    for (size_t i = 0; i < 6; ++i)
    {
        ItemAddedAtIndex(i);
    }
    //END TEMP DATA
}

void EPGFilterListAdapter::Clear()
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

void EPGFilterListAdapter::PopulateView(size_t index, CYISceneView *pView)
{
    const auto &item = m_listElements[index];
    
    auto pText = Utility::GetNode<CYITextSceneNode>(pView, "Text");
    pText->SetText(item.Text);
    
    auto pActiveText = Utility::GetNode<CYITextSceneNode>(pView, "Text-Active");
    pActiveText->SetText(item.Text);
    
    auto pToggleButton = Utility::GetNode<CYIToggleButtonView>(pView);
    pToggleButton->SetButtonID(static_cast<int32_t>(index));
    pToggleButton->SetToggled(item.ToggleState);
    pToggleButton->ButtonToggled.Connect(*this, &EPGFilterListAdapter::OnFilterButtonPressed);
}

void EPGFilterListAdapter::OnFilterButtonPressed(bool state, int32_t id)
{
    if (id == m_selectedFilterIndex)
    {
        return;
    }
    
    SetButtonStateForIndex(m_selectedFilterIndex, false);
    
    m_selectedFilterIndex = id;
    
    FilterEnabled.Emit(m_listElements[id].Text);
}

void EPGFilterListAdapter::SetButtonStateForIndex(int32_t index, bool state)
{
    m_listElements[index].ToggleState = state;
    
    CYISceneNode *pView = m_pListView->GetStreamedNode(index);
    if (pView)
    {
        auto pToggleButton = Utility::GetNode<CYIToggleButtonView>(pView);
        pToggleButton->SetToggled(state);
    }
}

#undef FILTER_ASSET_TEMPLATE
