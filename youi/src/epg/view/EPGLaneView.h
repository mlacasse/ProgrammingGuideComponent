#ifndef _EPG_LANE_VIEW_H_
#define _EPG_LANE_VIEW_H_

#include "epg/model/EPGChannelModel.h"

#include <animation/YiAnimationToggleController.h>
#include <view/YiSceneView.h>

class EPGChannelListView;
class EPGChannelModel;

class EPGLaneView : public CYISceneView
{
public:
    EPGLaneView();
    virtual ~EPGLaneView() = default;

    bool Init() override;

    virtual bool RequestFocus(CYIFocus::Direction direction = CYIFocus::Direction::Forward,
                              CYIFocus::FocusRootRule focusRootRule = CYIFocus::FocusRootRule::DescendantsUpdateContext,
                              const CYIAABB &previousFocusRect = CYIAABB(),
                              const CYIFocusSearchOptions &options = CYIFocusSearchOptions()) override;

    void SetChannel(const std::shared_ptr<EPGChannelModel>& pModel);

    const CYIString &GetChannelId() { return m_pModel->GetChannelId(); }

    EPGChannelListView *GetListView() { return m_pListView; }

    CYISignal<const std::shared_ptr<EPGChannelModel>& /*pModel*/> ChannelFocusGained;
    CYISignal<const std::shared_ptr<EPGChannelModel>& /*pModel*/> ChannelFocusLost;

protected:

    void OnFocusGainedInDescendants() override;
    void OnFocusLostInDescendants() override;

private:
    std::shared_ptr<EPGChannelModel> m_pModel;
    CYIAnimationToggleController m_animationController;

    EPGChannelListView *m_pListView = nullptr;

    YI_TYPE_BASES(EPGLaneView, CYISceneView)
};

#endif
