#include "EPGDetailsView.h"

#include "epg/model/EPGAssetModel.h"
#include "epg/model/EPGChannelModel.h"
#include "epg/utility/NodeUtilities.h"

#include <view/YiImageView.h>
#include <view/YiPushButtonView.h>

YI_TYPE_DEF_INST(EPGDetailsView, CYISceneView);

#define LOG_TAG "EPGDetailsView"

EPGDetailsView::EPGDetailsView()
{}

bool EPGDetailsView::Init()
{
    bool bOK = CYISceneView::Init();

    if (bOK)
    {
        m_pTitleText = Utility::GetNode<CYITextSceneNode>(this, "Text-Title");
        m_pMetadataText = Utility::GetNode<CYITextSceneNode>(this, "Text-Metadata");
        m_pDescriptionText = Utility::GetNode<CYITextSceneNode>(this, "Text-Description");
        m_pImageView = Utility::GetNode<CYIImageView>(this);
    }

    return bOK;
}

void EPGDetailsView::Reset()
{
    m_pTitleText->SetText(CYIString::EmptyString());
    m_pMetadataText->SetText(CYIString::EmptyString());
    m_pDescriptionText->SetText(CYIString::EmptyString());
    m_pImageView->Reset();
}

void EPGDetailsView::Format(const std::shared_ptr<EPGAssetModel> &pAssetModel, const std::shared_ptr<EPGChannelModel> &pChannelModel)
{
    m_pImageView->Reset();

    m_pChannelModel = pChannelModel;

    if (pAssetModel)
    {
        m_pTitleText->SetText(pAssetModel->GetTitle());

        CYIString metadataStr = CYIString("%1-%2").Arg(FormatTime(pAssetModel->GetStartTime(), false)).Arg(FormatTime(pAssetModel->GetEndTime(), true));
        m_pMetadataText->SetText(metadataStr);

        m_pDescriptionText->SetText(pAssetModel->GetDescription());

        m_pImageView->SetImage(pAssetModel->GetImageUrls()[0]);
    }
    else if (m_pChannelModel)
    {
        m_pTitleText->SetText(m_pChannelModel->GetName());
        m_pMetadataText->SetText(CYIString::EmptyString());
        m_pDescriptionText->SetText(CYIString::EmptyString());
        m_pImageView->SetImage(m_pChannelModel->GetImageUrl());
    }
}

CYIString EPGDetailsView::FormatTime(const CYIDateTime &time, bool appendMeridiem) const
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
