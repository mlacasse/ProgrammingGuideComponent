#ifndef _EPG_ON_NOW_VIEW_H_
#define _EPG_ON_NOW_VIEW_H_

#include <animation/YiAnimationToggleController.h>
#include <signal/YiSignal.h>
#include <utility/YiDateTime.h>
#include <view/YiSceneView.h>

class EPGAssetModel;
class EPGChannelModel;

class CYIImageView;
class CYIPushButtonView;
class CYIToggleButtonView;
class CYITextSceneNode;

class EPGOnNowView : public CYISceneView
{
public:
    EPGOnNowView();
    virtual ~EPGOnNowView() = default;

    bool Init() override;
    void Reset() override;

    void Format(const std::shared_ptr<EPGAssetModel> &pAssetModel, const std::shared_ptr<EPGChannelModel> &pChannelModel, int32_t index);
    void UpdateTimeRemaining();

    virtual bool RequestFocus(CYIFocus::Direction direction = CYIFocus::Direction::Forward,
                              CYIFocus::FocusRootRule focusRootRule = CYIFocus::FocusRootRule::DescendantsUpdateContext,
                              const CYIAABB &previousFocusRect = CYIAABB(),
                              const CYIFocusSearchOptions &options = CYIFocusSearchOptions()) override;

    CYISignal<const std::shared_ptr<EPGChannelModel> &/*pChannelModel*/> ChannelButtonPressed;
    CYISignal<const std::shared_ptr<EPGChannelModel> &/*pChannelModel*/, bool /*state*/> ChannelFavoriteButtonToggled;
    CYISignal<const std::shared_ptr<EPGAssetModel> &/*pAssetModel*/, const std::shared_ptr<EPGChannelModel> &/*pChannelModel*/> AssetSelected;

protected:
    void OnFocusGainedInDescendants() override;
    void OnFocusLostInDescendants() override;

private:
    void SetTitle(const CYIString &title);
    void SetMetadata(const CYIString &metadata);

    void OnAssetSelected();
    void OnChannelButtonPressed();
    void OnChannelFavoriteButtonToggled(bool state);

    CYIString FormatTime(const CYIDateTime &time, bool appendMeridiem) const;

    CYIAnimationToggleController m_animationController;

    CYITextSceneNode *m_pPreviewTitleText = nullptr;
    CYITextSceneNode *m_pPreviewMetadataText = nullptr;

    CYITextSceneNode *m_pTitleText = nullptr;
    CYITextSceneNode *m_pMetadataText = nullptr;
    CYITextSceneNode *m_pDescriptionText = nullptr;
    CYITextSceneNode *m_pTimeText = nullptr;
    CYISceneNode *m_pTimeRemainingContainer = nullptr;
    CYITextSceneNode *m_pTimeRemainingText = nullptr;
    CYIImageView *m_pImageView = nullptr;

    CYIImageView *m_pChannelImageView = nullptr;
    CYITextSceneNode *m_pChannelImageFallbackText = nullptr;
    CYIPushButtonView *m_pAssetButton = nullptr;
    CYIPushButtonView *m_pChannelButton = nullptr;
    CYIToggleButtonView *m_pFavoriteButton = nullptr;
    CYITextSceneNode *m_pChannelNumberText = nullptr;

    std::shared_ptr<EPGChannelModel> m_pChannelModel;
    std::shared_ptr<EPGAssetModel> m_pAssetModel;

    YI_TYPE_BASES(EPGOnNowView, CYISceneView)
};

#endif
