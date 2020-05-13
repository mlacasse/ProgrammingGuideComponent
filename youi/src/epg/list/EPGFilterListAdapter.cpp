#include "EPGFilterListAdapter.h"

#include "epg/model/EPGFilterModel.h"
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
    return m_filters.size();
}

void EPGFilterListAdapter::SetFilters(std::vector<EPGFilterModel> &&filters)
{
    Clear();
    
    m_filters = std::move(filters);
    
    auto itr = std::find_if(m_filters.begin(), m_filters.end(), [](const EPGFilterModel &filter) { return filter.IsEnabled(); });
    if (itr != m_filters.end())
    {
        m_selectedFilterIndex = static_cast<int32_t>(std::distance(m_filters.begin(), itr));
    }
    else
    {
        m_selectedFilterIndex = 0;
        m_filters[0].SetIsEnabled(true);
    }

    for (size_t i = 0; i < m_filters.size(); ++i)
    {
        ItemAddedAtIndex(i);
    }
}

void EPGFilterListAdapter::Clear()
{
    for (int32_t i = static_cast<int32_t>(m_filters.size()) - 1; i >= 0; --i)
    {
        ItemRemovedAtIndex.Emit(i);
    }

    m_selectedFilterIndex = 0;
}

void EPGFilterListAdapter::PopulateView(size_t index, CYISceneView *pView)
{
    const auto &item = m_filters[index];
    
    auto pText = Utility::GetNode<CYITextSceneNode>(pView, "Text");
    pText->SetText(item.GetLabel());
    
    auto pActiveText = Utility::GetNode<CYITextSceneNode>(pView, "Text-Active");
    pActiveText->SetText(item.GetLabel());
    
    auto pToggleButton = Utility::GetNode<CYIToggleButtonView>(pView);
    pToggleButton->SetButtonID(static_cast<int32_t>(index));
    pToggleButton->SetToggled(item.IsEnabled());
    pToggleButton->SetEnabled(!item.IsEnabled());
    pToggleButton->ButtonToggled.Connect(*this, &EPGFilterListAdapter::OnFilterButtonPressed);
}

void EPGFilterListAdapter::OnFilterButtonPressed(bool state, int32_t id)
{
    SetButtonStateForIndex(m_selectedFilterIndex, false);
    
    m_selectedFilterIndex = id;
    
    SetButtonStateForIndex(m_selectedFilterIndex, true);

    FilterEnabled.Emit(m_filters[id]);
}

void EPGFilterListAdapter::SetButtonStateForIndex(int32_t index, bool state)
{
    m_filters[index].SetIsEnabled(state);
    
    CYISceneNode *pView = m_pListView->GetStreamedNode(index);
    if (pView)
    {
        auto pToggleButton = Utility::GetNode<CYIToggleButtonView>(pView);
        pToggleButton->SetEnabled(!state);
        pToggleButton->SetToggled(state);
    }
}

#undef FILTER_ASSET_TEMPLATE
