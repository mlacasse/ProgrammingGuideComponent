#include "EPGViewController.h"

#include "epg/list/EPGListView.h"
#include "epg/list/EPGChannelListAdapter.h"
#include "epg/list/EPGTimeListAdapter.h"
#include "epg/model/EPGModel.h"
#include "epg/model/EPGAssetModel.h"
#include "epg/model/EPGChannelModel.h"
#include "epg/model/EPGFilterModel.h"
#include "epg/view/EPGTimeMarkerView.h"
#include "epg/utility/NodeUtilities.h"

#include <animation/YiTimeInterpolator.h>
#include <view/YiActivityIndicatorView.h>
#include <view/YiListView.h>

const uint32_t TICK_UPDATE_INTERVAL_MS = 5000;
const uint32_t ACTIVITY_INDICATOR_DELAY_MS = 300;

#define LOG_TAG "EPGViewController"

EPGViewController::EPGViewController()
{
}

EPGViewController::~EPGViewController()
{
    AssetUnfocused.DisconnectFromAllSignals();
    AssetSelected.DisconnectFromAllSignals();
    AssetFocused.DisconnectFromAllSignals();
    
    ChannelFavoriteButtonToggled.DisconnectFromAllSignals();
    ChannelButtonPressed.DisconnectFromAllSignals();
}

void EPGViewController::Init(CYISceneView *pView)
{
    m_pView = pView;
    
    m_pActivityIndicatorView = Utility::GetNode<CYIActivityIndicatorView>(pView);
    m_pActivityIndicatorView->SetStartDelay(ACTIVITY_INDICATOR_DELAY_MS);

    m_pEPGView = Utility::GetNode<CYISceneView>(pView, "Main-Epg");

    m_pEPGListView = Utility::GetNode<EPGListView>(m_pEPGView);
    auto pChannelListAdapter = std::make_unique<EPGChannelListAdapter>(m_pEPGListView);
    m_pEPGListAdapter = pChannelListAdapter.get();
    
    pChannelListAdapter->ChannelScrolled.Connect(*this, &EPGViewController::OnChannelListScrolled);
    
    pChannelListAdapter->AssetSelected.Connect(AssetSelected, &CYISignal<const std::shared_ptr<EPGAssetModel> &, const std::shared_ptr<EPGChannelModel> & >::Emit);
    pChannelListAdapter->AssetGainedFocus.Connect(*this, &EPGViewController::OnAssetGainedFocus);
    pChannelListAdapter->AssetLostFocus.Connect(*this, &EPGViewController::OnAssetLostFocus);
    pChannelListAdapter->ChannelButtonPressed.Connect(ChannelButtonPressed, &CYISignal<const std::shared_ptr<EPGChannelModel> &>::Emit);
    pChannelListAdapter->ChannelFavoriteButtonToggled.Connect(ChannelFavoriteButtonToggled, &CYISignal<const std::shared_ptr<EPGChannelModel> &, bool>::Emit);

    m_pEPGListView->SetAdapter(std::move(pChannelListAdapter));

    InitTimeList(m_pView);
    InitCurrentTimeMarker(m_pView);
    
    m_sideMenuViewController.Init(pView);
    m_sideMenuViewController.FilterEnabled.Connect(*this, &EPGViewController::OnFilterEnabled);
 
    m_onNowViewController.Init(pView);
    m_onNowViewController.AssetSelected.Connect(AssetSelected, &CYISignal<const std::shared_ptr<EPGAssetModel> &, const std::shared_ptr<EPGChannelModel> & >::Emit);
    m_onNowViewController.ChannelButtonPressed.Connect(ChannelButtonPressed, &CYISignal<const std::shared_ptr<EPGChannelModel> &>::Emit);
    m_onNowViewController.ChannelFavoriteButtonToggled.Connect(ChannelFavoriteButtonToggled, &CYISignal<const std::shared_ptr<EPGChannelModel> &, bool>::Emit);
    
    m_tickTimer.SetSingleShot(false);
    m_tickTimer.SetInterval(TICK_UPDATE_INTERVAL_MS);
    m_tickTimer.TimedOut.Connect(*this, &EPGViewController::OnTick);
}

void EPGViewController::InitTimeList(CYISceneView *pView)
{
    m_pTimeListView = Utility::GetNode<CYIListView>(m_pView, "Time-List");
    auto pTimeListAdapter = std::make_unique<EPGTimeListAdapter>();
    m_pTimeListAdapter = pTimeListAdapter.get();

    m_pTimeListView->SetAdapter(std::move(pTimeListAdapter));
}

void EPGViewController::InitCurrentTimeMarker(CYISceneView *pView)
{
    m_pCurrentTimeMarker = Utility::GetNode<EPGTimeMarkerView>(pView, "Time-Marker");

    float padding = 0;
    Utility::GetProperty<float>(m_pTimeListView, "padding-left", padding);
    m_pCurrentTimeMarker->SetPadding(padding);

    m_pCurrentTimeMarker->SetListElementWidth(m_pTimeListAdapter->GetViewTemplate(0)->GetTemplate()->GetWidth());
    m_pCurrentTimeMarker->SetListWidth(m_pEPGListAdapter->GetInitialNodeSize(0).x - padding);
    
    m_pEPGListAdapter->SetCurrentTimeMarker(m_pCurrentTimeMarker);
}

void EPGViewController::SetFilters(std::vector<EPGFilterModel> &&filters)
{
    m_sideMenuViewController.SetFilters(std::move(filters));
}

void EPGViewController::Populate(std::shared_ptr<EPGModel> data)
{
    m_pModel = data;

    m_pEPGListAdapter->ConstructChannelList(data);
    m_pTimeListAdapter->BuildElements(data->GetStartTime(), data->GetEndTime());

    m_pCurrentTimeMarker->SetDates(data->GetStartTime(), data->GetEndTime());
    m_pCurrentTimeMarker->UpdateMarkerPosition(CYIDateTime::GetCurrentDateTime(), 0.f);

    m_pCurrentTimeMarker->Show();

    m_tickTimer.Start();
}

void EPGViewController::Reset()
{
    m_pEPGListAdapter->Clear();
    m_pTimeListAdapter->Clear();
    m_pCurrentTimeMarker->Hide();
    m_didRestoreFocus = false;
}

void EPGViewController::SetFocusedAssetIds(const CYIString &channelId, const CYIString &assetId)
{
    m_lastFocusedChannelId = channelId;
    m_lastFocusedAssetId = assetId;
}
void EPGViewController::SetFocus()
{
    m_didRestoreFocus = m_pEPGListAdapter->RevealAsset(m_lastFocusedChannelId, m_lastFocusedAssetId);
}

void EPGViewController::RestoreFocus()
{
    if (!m_didRestoreFocus)
    {
        m_didRestoreFocus = m_pEPGListAdapter->RevealAsset(m_lastFocusedChannelId, m_lastFocusedAssetId);
    }
}

void EPGViewController::OnFilterEnabled(const EPGFilterModel &filter)
{
    if (filter.GetFilterType() == EPGFilterModel::FilterType::Favorite)
    {
        m_pEPGView->Show();
        m_onNowViewController.SetVisibility(false);

        if (filter.GetValue() == "0")
        {
            m_pEPGListAdapter->ConstructChannelList(m_pModel->GetChannelModels());
        }
        else
        {
            m_pEPGListAdapter->ConstructChannelList(m_pModel->GetFavoriteChannelModels());
        }
    }
    else if (filter.GetFilterType() == EPGFilterModel::FilterType::Category)
    {
        auto models = m_pModel->GetChannelModelsForCategory(filter.GetValue());
        m_onNowViewController.Populate(filter.GetLabel(), models);
        m_pEPGView->Hide();
        m_onNowViewController.SetVisibility(true);
    }

    m_pEPGListView->RequestFocusOnItem(0);
}

void EPGViewController::OnAssetGainedFocus(const std::shared_ptr<EPGAssetModel>& pAsset, const std::shared_ptr<EPGChannelModel> &pChannel)
{
    m_lastFocusedAssetId = pAsset ? pAsset->GetResourceId() : CYIString::EmptyString();
    m_lastFocusedChannelId = pChannel->GetChannelId();

    AssetFocused.Emit(pAsset, pChannel);
}

void EPGViewController::OnAssetLostFocus(const std::shared_ptr<EPGAssetModel>& pAsset, const std::shared_ptr<EPGChannelModel> &pChannel)
{
    YI_UNUSED(pChannel);

    m_lastFocusedAssetId = CYIString::EmptyString();

    AssetUnfocused.Emit(pAsset, pChannel);
}

void EPGViewController::ShowActivityIndicator()
{
    m_pActivityIndicatorView->Start();
}

void EPGViewController::HideActivityIndicator()
{
    m_pActivityIndicatorView->Stop();
}

void EPGViewController::OnChannelListScrolled(float scrolledPosition)
{
    m_pTimeListView->ScrollToHorizontalPosition(scrolledPosition);
    m_pCurrentTimeMarker->UpdateMarkerPosition(CYIDateTime::GetCurrentDateTime(), scrolledPosition);
}

void EPGViewController::OnTick()
{
    float horizontalPosition = m_pEPGListAdapter->GetHorizontalPosition();
    m_pCurrentTimeMarker->UpdateMarkerPosition(CYIDateTime::GetCurrentDateTime(), horizontalPosition);
}
