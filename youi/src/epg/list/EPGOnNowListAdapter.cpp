#include "EPGOnNowListAdapter.h"

#include "epg/model/EPGAssetModel.h"
#include "epg/model/EPGChannelModel.h"
#include "epg/view/EPGOnNowView.h"

#include <utility/YiSceneNodeUtility.h>
#include <view/YiPooledViewRecycler.h>

#define ON_NOW_TEMPLATE_NAME "EPG_Lane-OnNow-root"

#define LOG_TAG "EPGOnNowListAdapter"

EPGOnNowListAdapter::EPGOnNowListAdapter() : CYIViewAdapter(std::make_shared<CYIPooledViewRecycler>())
{
}

std::shared_ptr<CYIAssetViewTemplate> EPGOnNowListAdapter::GetViewTemplate(size_t index) const
{
    return CYIViewTemplate::GetViewTemplate(ON_NOW_TEMPLATE_NAME);
}

size_t EPGOnNowListAdapter::GetItemsCount() const
{
    return m_channelModels.size();
}

void EPGOnNowListAdapter::PopulateView(size_t index, CYISceneView *pView)
{
    auto pOnNowView = YiDynamicCast<EPGOnNowView>(pView);

    const std::shared_ptr<EPGChannelModel> &pChannelModel = m_channelModels[index];
    const std::shared_ptr<EPGAssetModel> &pOnNowAssetModel = pChannelModel->GetModelOnNow();

    pOnNowView->Format(pOnNowAssetModel, pChannelModel, static_cast<int32_t>(index));
    pOnNowView->AssetSelected.Connect(AssetSelected, &CYISignal<const std::shared_ptr<EPGAssetModel> &, const std::shared_ptr<EPGChannelModel> &>::Emit);
    pOnNowView->ChannelButtonPressed.Connect(ChannelButtonPressed, &CYISignal<const std::shared_ptr<EPGChannelModel> &>::Emit);
    pOnNowView->ChannelFavoriteButtonToggled.Connect(ChannelFavoriteButtonToggled, &CYISignal<const std::shared_ptr<EPGChannelModel> &, bool>::Emit);

    m_streamedViews.push_front(pOnNowView);
}

void EPGOnNowListAdapter::OnReleaseView(size_t index, CYISceneView *pView)
{
    YI_UNUSED(index);

    m_streamedViews.remove(static_cast<EPGOnNowView*>(pView));
}

void EPGOnNowListAdapter::ConstructChannelList(const std::vector<std::shared_ptr<EPGChannelModel>> &models)
{
    Clear();

    m_channelModels = models;

    for (size_t i = 0; i < m_channelModels.size(); ++i)
    {
        ItemAddedAtIndex.Emit(i);
    }
}

void EPGOnNowListAdapter::Clear()
{
    if (m_channelModels.size() > 0)
    {
        for (int32_t i = static_cast<int32_t>(m_channelModels.size()) - 1; i >= 0; --i)
        {
            ItemRemovedAtIndex.Emit(i);
        }

        m_channelModels.clear();
    }
}

void EPGOnNowListAdapter::Tick()
{
    for (auto &onNowView : m_streamedViews)
    {
        onNowView->UpdateTimeRemaining();
    }
}

#undef ON_NOW_TEMPLATE_NAME
