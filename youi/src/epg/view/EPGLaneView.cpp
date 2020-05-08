#include "EPGLaneView.h"

#include "epg/list/EPGChannelListView.h"
#include "epg/utility/NodeUtilities.h"

YI_TYPE_DEF_INST(EPGLaneView, CYISceneView);

#define LOG_TAG "EPGLaneView"

EPGLaneView::EPGLaneView()
{}

bool EPGLaneView::Init()
{
    bool bOK = CYISceneView::Init();

    if (bOK)
    {
        m_pListView = Utility::GetNode<EPGChannelListView>(this);
        m_animationController.SetTimelines(GetTimeline("Row-Focus"));
    }

    return bOK;
}

bool EPGLaneView::RequestFocus(CYIFocus::Direction direction, CYIFocus::FocusRootRule focusRootRule, const CYIAABB &previousFocusRect, const CYIFocusSearchOptions &options)
{
    if (m_pListView && direction == CYIFocus::Direction::Forward)
    {
        return m_pListView->RequestFocus(direction, focusRootRule, previousFocusRect, options);
    }

    return CYISceneView::RequestFocus(direction, focusRootRule, previousFocusRect, options);
}

void EPGLaneView::SetChannel(const std::shared_ptr<EPGChannelModel>& pModel)
{
    m_pModel = pModel;
}

void EPGLaneView::OnFocusGainedInDescendants()
{
    ChannelFocusGained.Emit(m_pModel);
    m_animationController.ToggleOn();
}

void EPGLaneView::OnFocusLostInDescendants()
{
    ChannelFocusLost.Emit(m_pModel);
    m_animationController.ToggleOff();
}
