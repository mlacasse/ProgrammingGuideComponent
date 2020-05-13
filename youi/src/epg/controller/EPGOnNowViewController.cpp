#include "EPGOnNowViewController.h"

#include "epg/list/EPGOnNowListAdapter.h"
#include "epg/model/EPGAssetModel.h"
#include "epg/model/EPGChannelModel.h"
#include "epg/utility/NodeUtilities.h"

#include <scenetree/YiTextSceneNode.h>
#include <view/YiListView.h>

static constexpr uint32_t TICK_UPDATE_INTERVAL_MS = 10 * 1000;

#define LOG_TAG "EPGOnNowViewController"

EPGOnNowViewController::EPGOnNowViewController()
{
    m_tickTimer.SetSingleShot(false);
    m_tickTimer.SetInterval(TICK_UPDATE_INTERVAL_MS);
    m_tickTimer.TimedOut.Connect(*this, &EPGOnNowViewController::OnTick);
}

EPGOnNowViewController::~EPGOnNowViewController()
{
    AssetSelected.DisconnectFromAllSignals();
    ChannelFavoriteButtonToggled.DisconnectFromAllSignals();
    ChannelButtonPressed.DisconnectFromAllSignals();
}

void EPGOnNowViewController::Init(CYISceneView *pView)
{
    m_pView = Utility::GetNode<CYISceneView>(pView, "Main-OnNowListings");

    m_showHideToggleController.SetTimelines(pView->GetTimeline("ShowOnNow"));
    m_showHideToggleController.AnimationEnded.Connect([=] (CYIAnimationToggleController::State state) {
        if (state == CYIAnimationToggleController::State::Off) {
            m_pView->Hide();
        }
    });
    m_showHideToggleController.ResetToState(CYIAnimationToggleController::State::Off);

    auto pAdapter = std::make_unique<EPGOnNowListAdapter>();
    m_pListAdapter = pAdapter.get();

    pAdapter->AssetSelected.Connect(AssetSelected, &CYISignal<const std::shared_ptr<EPGAssetModel> &, const std::shared_ptr<EPGChannelModel> & >::Emit);
    pAdapter->ChannelButtonPressed.Connect(ChannelButtonPressed, &CYISignal<const std::shared_ptr<EPGChannelModel> &>::Emit);
    pAdapter->ChannelFavoriteButtonToggled.Connect(ChannelFavoriteButtonToggled, &CYISignal<const std::shared_ptr<EPGChannelModel> &, bool>::Emit);

    m_pListView = Utility::GetNode<CYIListView>(m_pView);
    m_pListView->SetAdapter(std::move(pAdapter));

    m_pHeaderText = Utility::GetNode<CYITextSceneNode>(m_pView, "Text-OnNow");
    m_pDateText = Utility::GetNode<CYITextSceneNode>(m_pView, "Text-Date");
}

void EPGOnNowViewController::Populate(const CYIString &label, const std::vector<std::shared_ptr<EPGChannelModel>> &data)
{
    CYIString headerText = CYIString("On Now - %1").Arg(label);
    m_pHeaderText->SetText(headerText);

    const auto &currentTime = CYIDateTime::GetCurrentDateTime();
    CYIString dateText = CYIString("%1 %2/%3")
        .Arg(currentTime.GetDayOfWeekShortName())
        .Arg(CYIString::FromValue(currentTime.GetMonth()))
        .Arg(CYIString::FromValue(currentTime.GetDay()));
    m_pDateText->SetText(dateText);

    m_pListAdapter->ConstructChannelList(data);
    m_tickTimer.Start();
}

void EPGOnNowViewController::Reset()
{
    m_pListAdapter->Clear();
}

void EPGOnNowViewController::SetVisibility(bool isVisible)
{
    if (isVisible)
    {
        m_pView->Show();
        m_showHideToggleController.ToggleOn();
        RestoreFocus();
    }
    else
    {
        m_showHideToggleController.ToggleOff();
    }
}

void EPGOnNowViewController::RestoreFocus()
{
    m_pListView->RequestFocusOnItem(0);
}

void EPGOnNowViewController::OnTick()
{
    m_pListAdapter->Tick();
}
