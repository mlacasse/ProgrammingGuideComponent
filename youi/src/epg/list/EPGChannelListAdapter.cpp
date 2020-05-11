#include "EPGChannelListAdapter.h"

#include "epg/list/EPGAssetListAdapter.h"
#include "epg/list/EPGListView.h"
#include "epg/list/EPGChannelListView.h"
#include "epg/model/EPGAssetModel.h"
#include "epg/model/EPGChannelModel.h"
#include "epg/model/EPGModel.h"
#include "epg/utility/NodeUtilities.h"
#include "epg/view/EPGDetailsView.h"
#include "epg/view/EPGLaneView.h"
#include "epg/view/EPGTimeMarkerView.h"

#include <animation/YiInterpolateQuadEaseOut.h>
#include <utility/YiSceneNodeUtility.h>
#include <view/YiImageView.h>
#include <view/YiListView.h>
#include <view/YiPooledViewRecycler.h>
#include <view/YiPushButtonView.h>
#include <view/YiToggleButtonView.h>


#define CHANNEL_LANE_TEMPLATE_NAME "EPG_Lane-root"
#define DETAILS_TEMPLATE_NAME "EPG_Details-root"

#define LOG_TAG "EPGChannelListAdapter"

EPGChannelListAdapter::EPGChannelListAdapter(EPGListView *pChannelsList)
    : m_pChannelsList(pChannelsList)
    , m_pInterpolator(new CYIInterpolateQuadEaseOut())
{
    m_pEPGViewRecycler = std::make_shared<CYIPooledViewRecycler>();
    
    pChannelsList->VerticalPositionChanged.Connect(*this, &EPGChannelListAdapter::OnVerticalPositionChanged);
    
    // Necessary for RevealChannel as the details view is not yet created or laid out
    pChannelsList->MeasurementsApplied.Connect(*this, &EPGChannelListAdapter::UpdateTimeMarkerGap);
}

std::shared_ptr<CYIAssetViewTemplate> EPGChannelListAdapter::GetViewTemplate(size_t index) const
{
    return CYIViewTemplate::GetViewTemplate((m_isDetailsItemVisible && index == m_detailsItemIndex) ? DETAILS_TEMPLATE_NAME : CHANNEL_LANE_TEMPLATE_NAME);
}

size_t EPGChannelListAdapter::GetItemsCount() const
{
    size_t detailsCount = m_isDetailsItemVisible ? 1 : 0;
    return m_channelIDs.size() + detailsCount;
}

void EPGChannelListAdapter::PopulateView(size_t index, CYISceneView *pView)
{
    if (m_isDetailsItemVisible)
    {
        if (index > m_detailsItemIndex)
        {
            --index;
        }
        else if (index == m_detailsItemIndex)
        {
            m_pDetailsItemView = YiDynamicCast<EPGDetailsView>(pView);
            m_pDetailsItemView->Format(m_pDetailsItemModel, m_pDetailsChannelModel);
            return;
        }
    }

    auto pLaneView = YiDynamicCast<EPGLaneView>(pView);
    auto pList = pLaneView->GetListView();
    
    std::shared_ptr<EPGChannelModel> pChannelModel = GetChannelData(index);

    pLaneView->SetChannel(pChannelModel);
    pLaneView->ChannelFocusGained.Connect(*this, &EPGChannelListAdapter::OnChannelGainedFocus);
    pLaneView->ChannelFocusLost.Connect(*this, &EPGChannelListAdapter::OnChannelLostFocus);

    auto pListAdapter = std::make_unique<EPGAssetListAdapter>(pChannelModel, pList, m_pEPGViewRecycler);
     
    pListAdapter->AssetSelected.Connect(AssetSelected, &CYISignal<const std::shared_ptr<EPGAssetModel> &, const std::shared_ptr<EPGChannelModel> & >::Emit);
    pListAdapter->AssetLostFocus.Connect(AssetLostFocus, &CYISignal<const std::shared_ptr<EPGAssetModel> &, const std::shared_ptr<EPGChannelModel> & >::Emit);

    pListAdapter->AssetGainedFocus.Connect(*this, &EPGChannelListAdapter::OnAssetGainedFocus);
    pListAdapter->AssetLostFocus.Connect(*this, &EPGChannelListAdapter::OnAssetLostFocus);

    pListAdapter->BuildElements(m_pEPGModel->GetStartTime(), m_pEPGModel->GetEndTime());

    pList->SetAdapter(std::move(pListAdapter));
    pList->SetID(static_cast<int32_t>(index));
    pList->ScrollToHorizontalPosition(m_horizontalPosition);
    pList->FocusedHorizontalPositionChanged.Connect(*this, &EPGChannelListAdapter::OnChannelHorizontalScroll);
    m_laneViews.push_front(pLaneView);
    
    auto pChannelButton = Utility::GetNode<CYIPushButtonView>(pView, "Channelslot");
    auto pFavoriteButton = Utility::GetNode<CYIToggleButtonView>(pView, "Favouriteslot");
    
    // Channel Btn
    pChannelButton->SetButtonID(static_cast<int32_t>(index));
    pChannelButton->GainedFocus.Connect(*this, &EPGChannelListAdapter::OnChannelButtonGainedFocus);
    pChannelButton->ButtonPressed.Connect(*this, &EPGChannelListAdapter::OnChannelButtonPressed);

    auto pImage = Utility::GetNode<CYIImageView>(pChannelButton, "ChannelImage");
    pImage->SetImage(pChannelModel->GetImageUrl());

    auto pFallbackText = Utility::GetNode<CYITextSceneNode>(pImage);
    pFallbackText->SetText(pChannelModel->GetCallSign());
    
    // Favourite Btn
    pFavoriteButton->SetButtonID(static_cast<int32_t>(index));
    pFavoriteButton->SetToggled(pChannelModel->IsFavorite());
    pFavoriteButton->ButtonToggled.Connect(*this, &EPGChannelListAdapter::OnChannelFavoriteButtonToggled);

    auto pChannelNumberText = Utility::GetNode<CYITextSceneNode>(pFavoriteButton);
    pChannelNumberText->SetText(pChannelModel->GetChannelNumberString());
    
    // Focus
    pChannelButton->SetNextFocus(pList, CYIFocus::Direction::Right);
    pList->SetNextFocus(pChannelButton, CYIFocus::Direction::Left);
    
    pFavoriteButton->SetNextFocus(pChannelButton, CYIFocus::Direction::Right);
    pChannelButton->SetNextFocus(pFavoriteButton, CYIFocus::Direction::Left);
}

void EPGChannelListAdapter::OnReleaseView(size_t index, CYISceneView *pView)
{
    YI_UNUSED(pView);

    if (m_isDetailsItemVisible)
    {
        if (index > m_detailsItemIndex)
        {
            --index;
        }
        else if (index == m_detailsItemIndex)
        {
            m_pDetailsItemView = nullptr;
            return;
        }
    }

    m_laneViews.remove(static_cast<EPGLaneView*>(pView));
}

void EPGChannelListAdapter::ConstructChannelList(std::shared_ptr<EPGModel> pModel)
{
    m_pEPGModel = pModel;

    ConstructChannelList(pModel->GetChannelModels());

    m_pEPGModel->ChannelAddedAtIndex.Connect(*this, &EPGChannelListAdapter::OnModelAddedChannelAtIndex, EYIConnectionType::Sync);
}

void EPGChannelListAdapter::ConstructChannelList(const std::vector<std::shared_ptr<EPGChannelModel>> &models)
{
    Clear();

    for (const auto &channelModel : models)
    {
        CYIString channelId = channelModel->GetChannelId();
        m_channelIDs.push_back(channelId);
        ItemAddedAtIndex(m_channelIDs.size() - 1);
    }
}

void EPGChannelListAdapter::Clear()
{
    if (m_channelIDs.size() > 0)
    {
        HideDetailsItem();

        for (int32_t i = static_cast<int32_t>(m_channelIDs.size()) - 1; i >= 0; --i)
        {
            ItemRemovedAtIndex(i);
        }

        m_channelIDs.clear();
    }
}

void EPGChannelListAdapter::ShowDetailsItem(size_t index)
{
    if (m_isDetailsItemVisible)
    {
        if (index != m_detailsItemIndex)
        {
            size_t prevIndex = m_detailsItemIndex;
            m_detailsItemIndex = index;
            ItemMoved.Emit(prevIndex, m_detailsItemIndex);
        }

        if (m_pDetailsItemView)
        {
            m_pDetailsItemView->Format(m_pDetailsItemModel, m_pDetailsChannelModel);
        }
    }
    else
    {
        m_isDetailsItemVisible = true;
        m_detailsItemIndex = index;
        ItemAddedAtIndex.Emit(m_detailsItemIndex);
    }
}

void EPGChannelListAdapter::HideDetailsItem()
{
    if (!m_isDetailsItemVisible)
    {
        return;
    }

    m_pDetailsItemModel = nullptr;
    m_pDetailsItemView = nullptr;
    m_isDetailsItemVisible = false;
    ItemRemovedAtIndex(m_detailsItemIndex);
}

void EPGChannelListAdapter::UpdateTimeMarkerGap()
{
    if (m_isDetailsItemVisible && m_pDetailsItemView)
    {
        if (auto pChannelNode = m_pChannelsList->GetStreamedNode(m_detailsItemIndex - 1))
        {
            glm::vec2 startPos = CYISceneNodeUtility::LocalToScreenSpace(pChannelNode, pChannelNode->GetLocalAABB().GetTopLeft());
            glm::vec2 endPos = CYISceneNodeUtility::LocalToScreenSpace(m_pDetailsItemView, m_pDetailsItemView->GetLocalAABB().GetBottomRight());
            m_pCurrentTimeMarker->UpdateMarkerGap(startPos, endPos);
        }
    }
    else
    {
        m_pCurrentTimeMarker->ResetMarkerGap();
    }
}

void EPGChannelListAdapter::OnAssetGainedFocus(const std::shared_ptr<EPGAssetModel>& pAssetModel, const std::shared_ptr<EPGChannelModel> &pChannelModel)
{
    m_pDetailsItemModel = pAssetModel;

    ShowDetailsItem(m_detailsItemIndex);

    AssetGainedFocus.Emit(pAssetModel, pChannelModel);
}

void EPGChannelListAdapter::OnAssetLostFocus(const std::shared_ptr<EPGAssetModel>& pChannel, const std::shared_ptr<EPGChannelModel> &pAsset)
{
    m_pDetailsItemModel = nullptr;

    ShowDetailsItem(m_detailsItemIndex);
}

void EPGChannelListAdapter::OnChannelGainedFocus(const std::shared_ptr<EPGChannelModel> &pChannel)
{
    m_pDetailsChannelModel = pChannel;

    auto findItr = std::find(m_channelIDs.begin(), m_channelIDs.end(), m_pDetailsChannelModel->GetChannelId());
    size_t index = std::distance(m_channelIDs.begin(), findItr);

    ShowDetailsItem(index + 1);
    
    UpdateTimeMarkerGap();
}

void EPGChannelListAdapter::OnChannelLostFocus(const std::shared_ptr<EPGChannelModel> &pChannel)
{
    m_pDetailsChannelModel = nullptr;
    
    UpdateTimeMarkerGap();
}

void EPGChannelListAdapter::OnModelAddedChannelAtIndex(size_t index)
{
    m_channelIDs.insert(m_channelIDs.begin() + index, m_pEPGModel->GetChannelModels()[index]->GetChannelId());

    if (m_isDetailsItemVisible && index <= m_detailsItemIndex)
    {
        m_detailsItemIndex++;
    }

    ItemAddedAtIndex.Emit(index);
}

std::shared_ptr<EPGChannelModel> EPGChannelListAdapter::GetChannelData(size_t index)
{
    CYIString id = m_channelIDs[index];
    
    return m_pEPGModel->GetChannelModel(id);
}

void EPGChannelListAdapter::OnVerticalPositionChanged()
{
    UpdateTimeMarkerGap();
}

void EPGChannelListAdapter::OnChannelHorizontalScroll(float position)
{
    ScrollToHorizontalPosition(position, true);
}

void EPGChannelListAdapter::ScrollToHorizontalPosition(float percent, bool emitScrolledSignal, float duration, CYITimeInterpolator *pInterpolator)
{
    m_horizontalPosition = percent;

    for (auto &view : m_laneViews)
    {
        if (view && view->GetListView() && !view->GetListView()->HasFocusedItem())
        {
            view->GetListView()->ScrollToHorizontalPosition(percent, duration, pInterpolator);
        }
    }
    
    if (emitScrolledSignal)
    {
        ChannelScrolled.Emit(percent);
    }
}

bool EPGChannelListAdapter::RevealAsset(const CYIString &channelId, const CYIString &assetId)
{
    bool bSuccess = false;

    if (channelId.IsEmpty())
    {
        YI_LOGD(LOG_TAG, "RevealAsset failed - no channel ID");
        m_pChannelsList->RevealItem(0, 0.f);
        m_pChannelsList->RequestFocusOnItem(0);
        return bSuccess;
    }

    auto itr = std::find(m_channelIDs.begin(), m_channelIDs.end(), channelId);

    if (itr != m_channelIDs.end())
    {
        size_t channelIndex = std::distance(m_channelIDs.begin(), itr);

        m_pChannelsList->RevealItem(channelIndex, 0.f);

        auto pListView = static_cast<EPGLaneView*>(m_pChannelsList->GetStreamedNode(channelIndex))->GetListView();
        bSuccess = pListView->FocusAssetWithId(assetId);
        if (!bSuccess)
        {
            bSuccess = m_pChannelsList->RequestFocusOnItem(channelIndex);
        }
    }
    else
    {
        YI_LOGD(LOG_TAG, "RevealAsset failed - no channel found with ID %s", channelId.GetData());
        m_pChannelsList->RevealItem(0, 0.f);
        m_pChannelsList->RequestFocusOnItem(0);
    }
    
    return bSuccess;
}

void EPGChannelListAdapter::OnChannelButtonGainedFocus()
{
    // When a channel list loses focus to the channel button
    // scrolling will no longer be reported, causing the rest of the
    // lists to go out of sync
    ScrollToHorizontalPosition(0.f, true, 200.f, m_pInterpolator.get());
}

void EPGChannelListAdapter::OnChannelButtonPressed(int32_t index)
{
    ChannelButtonPressed.Emit(GetChannelData(static_cast<size_t>(index)));
}

void EPGChannelListAdapter::OnChannelFavoriteButtonToggled(bool state, int32_t index)
{
    // Remove this one the issue with the time headers is resolved.
    auto channel = GetChannelData(static_cast<size_t>(index));
    channel->SetFavorite(state);

    ChannelFavoriteButtonToggled.Emit(GetChannelData(static_cast<size_t>(index)), state);
}

#undef CHANNEL_LANE_TEMPLATE_NAME
#undef DETAILS_TEMPLATE_NAME
