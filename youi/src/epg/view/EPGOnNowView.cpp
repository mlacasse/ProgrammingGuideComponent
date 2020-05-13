#include "EPGOnNowView.h"

#include "epg/model/EPGAssetModel.h"
#include "epg/model/EPGChannelModel.h"
#include "epg/utility/NodeUtilities.h"

#include <view/YiImageView.h>
#include <view/YiPushButtonView.h>
#include <view/YiToggleButtonView.h>

#include <cmath>

YI_TYPE_DEF_INST(EPGOnNowView, CYISceneView);

#define LOG_TAG "EPGOnNowView"

EPGOnNowView::EPGOnNowView()
{}

bool EPGOnNowView::Init()
{
    bool bOK = CYISceneView::Init();

    if (bOK)
    {
        m_animationController.SetTimelines(GetTimeline("Row-Expand", TimelineGetMode::GetTimelineGroup),
                                           GetTimeline("Row-Collapse", TimelineGetMode::GetTimelineGroup));

        auto pPreviewView = Utility::GetNode<CYISceneNode>(this, "Preview-Contents");
        m_pPreviewTitleText = Utility::GetNode<CYITextSceneNode>(pPreviewView, "Text-Title");
        m_pPreviewMetadataText = Utility::GetNode<CYITextSceneNode>(pPreviewView, "Text-Metadata");

        auto pExpandedView = Utility::GetNode<CYISceneNode>(this, "OnNow-Details-Text-Container");
        m_pTitleText = Utility::GetNode<CYITextSceneNode>(pExpandedView, "Text-Title");
        m_pMetadataText = Utility::GetNode<CYITextSceneNode>(pExpandedView, "Text-Metadata");
        m_pDescriptionText = Utility::GetNode<CYITextSceneNode>(pExpandedView, "Text-Description");
        m_pTimeRemainingContainer = Utility::GetNode<CYISceneNode>(pExpandedView, "RemainingTime-Container");
        m_pTimeText = Utility::GetNode<CYITextSceneNode>(m_pTimeRemainingContainer);
        m_pTimeRemainingText = Utility::GetNode<CYITextSceneNode>(pExpandedView, "Text-RemainingTime");

        m_pImageView = Utility::GetNode<CYIImageView>(this, "OnNow-Details-Image");

        m_pAssetButton = Utility::GetNode<CYIPushButtonView>(this, "OnNow-Details-Slot");
        m_pAssetButton->ButtonPressed.Connect(*this, &EPGOnNowView::OnAssetSelected);

        m_pChannelButton = Utility::GetNode<CYIPushButtonView>(this, "Channelslot-OnNow");
        m_pChannelButton->ButtonPressed.Connect(*this, &EPGOnNowView::OnChannelButtonPressed);

        m_pChannelImageView = Utility::GetNode<CYIImageView>(m_pChannelButton, "ChannelImage");
        m_pChannelImageFallbackText = Utility::GetNode<CYITextSceneNode>(m_pChannelImageView);

        m_pFavoriteButton = Utility::GetNode<CYIToggleButtonView>(this, "Favouriteslot-OnNow");
        m_pFavoriteButton->ButtonToggled.Connect(*this, &EPGOnNowView::OnChannelFavoriteButtonToggled);
        m_pChannelNumberText = Utility::GetNode<CYITextSceneNode>(m_pFavoriteButton);

        // Focus
        m_pFavoriteButton->SetNextFocus(m_pChannelButton, CYIFocus::Direction::Right);
        m_pChannelButton->SetNextFocus(m_pFavoriteButton, CYIFocus::Direction::Left);

        m_pChannelButton->SetNextFocus(m_pAssetButton, CYIFocus::Direction::Right);
        m_pAssetButton->SetNextFocus(m_pChannelButton, CYIFocus::Direction::Left);
    }

    return bOK;
}

void EPGOnNowView::Reset()
{
    m_pAssetModel = nullptr;
    m_pChannelModel = nullptr;

    SetTitle(CYIString::EmptyString());
    SetMetadata(CYIString::EmptyString());
    m_pDescriptionText->SetText(CYIString::EmptyString());
    m_pImageView->Reset();

    m_pTimeText->SetText(CYIString::EmptyString());
    m_pTimeRemainingContainer->Hide();

    m_pChannelImageView->Reset();
    m_pChannelImageFallbackText->SetText(CYIString::EmptyString());
    m_pChannelNumberText->SetText(CYIString::EmptyString());
}

void EPGOnNowView::Format(const std::shared_ptr<EPGAssetModel> &pAssetModel, const std::shared_ptr<EPGChannelModel> &pChannelModel, int32_t index)
{
    m_pImageView->Reset();

    m_pAssetModel = pAssetModel;
    m_pChannelModel = pChannelModel;

    SetTitle(pAssetModel->GetTitle());
    CYIString metadataStr = CYIString("%1-%2").Arg(FormatTime(pAssetModel->GetStartTime(), false)).Arg(FormatTime(pAssetModel->GetEndTime(), true));
    SetMetadata(metadataStr);
    m_pDescriptionText->SetText(pAssetModel->GetDescription());
    m_pImageView->SetImage(pAssetModel->GetImageUrls()[0]);

    CYIString timeText = CYIString("%1-%2").Arg(FormatTime(pAssetModel->GetStartTime(), false)).Arg(FormatTime(pAssetModel->GetEndTime(), true));
    m_pTimeText->SetText(timeText);

    UpdateTimeRemaining();

    // Channel Btn
    m_pChannelButton->SetButtonID(static_cast<int32_t>(index));
    m_pChannelImageView->SetImage(pChannelModel->GetImageUrl());
    m_pChannelImageFallbackText->SetText(pChannelModel->GetCallSign());

    // Favourite Btn
    m_pFavoriteButton->SetButtonID(static_cast<int32_t>(index));
    m_pFavoriteButton->SetToggled(pChannelModel->IsFavorite());
    m_pChannelNumberText->SetText(pChannelModel->GetChannelNumberString());
}

void EPGOnNowView::OnFocusGainedInDescendants()
{
    m_animationController.ToggleOn();
}

void EPGOnNowView::OnFocusLostInDescendants()
{
    m_animationController.ToggleOff();
}

void EPGOnNowView::SetTitle(const CYIString &title)
{
    m_pTitleText->SetText(title);
    m_pPreviewTitleText->SetText(title);
}

void EPGOnNowView::SetMetadata(const CYIString &metadata)
{
    m_pMetadataText->SetText(metadata);
    m_pPreviewMetadataText->SetText(metadata);
}

void EPGOnNowView::UpdateTimeRemaining()
{
    static const double SECONDS_IN_A_MINUTE = 60.f;

    if (m_pAssetModel && m_pAssetModel->IsOnNow())
    {
        auto minutesRemaining = std::ceil((m_pAssetModel->GetEndTime() - CYIDateTime::GetCurrentDateTime()) / SECONDS_IN_A_MINUTE);
        CYIString timeRemaining = CYIString("%1m left").Arg(CYIString::FromValue(static_cast<int32_t>(minutesRemaining)));
        m_pTimeRemainingText->SetText(timeRemaining);
        m_pTimeRemainingContainer->Show();
    }
    else
    {
        m_pTimeRemainingContainer->Hide();
    }
}

bool EPGOnNowView::RequestFocus(CYIFocus::Direction direction, CYIFocus::FocusRootRule focusRootRule, const CYIAABB &previousFocusRect, const CYIFocusSearchOptions &options)
{
    if (m_pAssetButton && direction == CYIFocus::Direction::Forward)
    {
        return m_pAssetButton->RequestFocus(direction, focusRootRule, previousFocusRect, options);
    }

    return CYISceneView::RequestFocus(direction, focusRootRule, previousFocusRect, options);
}

void EPGOnNowView::OnAssetSelected()
{
    AssetSelected.Emit(m_pAssetModel, m_pChannelModel);
}

void EPGOnNowView::OnChannelButtonPressed()
{
    ChannelButtonPressed.Emit(m_pChannelModel);
}

void EPGOnNowView::OnChannelFavoriteButtonToggled(bool state)
{
    ChannelFavoriteButtonToggled.Emit(m_pChannelModel, state);
}

CYIString EPGOnNowView::FormatTime(const CYIDateTime &time, bool appendMeridiem) const
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

    return timeString;
}
