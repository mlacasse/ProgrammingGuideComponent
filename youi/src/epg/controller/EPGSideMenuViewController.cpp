#include "EPGSideMenuViewController.h"

#include "epg/list/EPGFilterListAdapter.h"
#include "epg/utility/NodeUtilities.h"

#include <view/YiListView.h>
#include <view/YiToggleButtonView.h>

#define LOG_TAG "EPGFilterViewController"

EPGSideMenuViewController::EPGSideMenuViewController()
{
}

void EPGSideMenuViewController::Init(CYISceneView *pView)
{
    CYIAbstractTimeline *pShowTimeline = pView->GetTimeline("ShowSideMenu");
    CYIAbstractTimeline *pHideTimeline = pView->GetTimeline("HideSideMenu");
    m_showHideToggleController.SetTimelines(pShowTimeline, pHideTimeline);
    
    m_pView = Utility::GetNode<CYISceneView>(pView, "SideMenu");
    
    auto pMenuHeader = Utility::GetNode<CYISceneNode>(m_pView, "SideMenu-SectionHeader");
    Utility::GetNode<CYITextSceneNode>(pMenuHeader)->SetText("Filter");

    CYIAbstractTimeline *pShowJumpMenuTimeline = m_pView->GetTimeline("ShowJumpMenu");
    CYIAbstractTimeline *pHideJumpMenuTimeline = m_pView->GetTimeline("ShowDefault");
    m_jumpMenuToggleController.SetTimelines(pShowJumpMenuTimeline, pHideJumpMenuTimeline);
    m_jumpMenuToggleController.ResetToState(CYIAnimationToggleController::State::Off);
    
    m_pFilterListView = Utility::GetNode<CYIListView>(m_pView, "SideMenu-FilterList");
    auto pAdapter = std::make_unique<EPGFilterListAdapter>(m_pFilterListView);
    m_pFilterListAdapter = pAdapter.get();

    m_pFilterListView->SetAdapter(std::move(pAdapter));
    
    m_pView->DescendantGainedFocus.Connect(*this, &EPGSideMenuViewController::OnDescendantGainedFocus);
    m_pView->DescendantLostFocus.Connect(*this, &EPGSideMenuViewController::OnDescendantLostFocus);

    auto pSortButton = Utility::GetNode<CYIToggleButtonView>(m_pView, "SideMenu-SortButton");
    pSortButton->SetText("All");

    auto pDateSelectorButton = Utility::GetNode<CYIToggleButtonView>(m_pView, "SideMenu-JumpButton");
    pDateSelectorButton->SetText("Jump To Day");
}

void EPGSideMenuViewController::Populate(bool forceReset)
{
    if (!forceReset && m_pFilterListAdapter->GetItemsCount() > 0)
    {
        return;
    }

    m_pFilterListAdapter->BuildElements();
}

void EPGSideMenuViewController::OnSortButtonPressed()
{}

void EPGSideMenuViewController::OnDaySelectorButtonPressed()
{}

void EPGSideMenuViewController::OnDescendantGainedFocus()
{
    m_showHideToggleController.ToggleOn();
}

void EPGSideMenuViewController::OnDescendantLostFocus()
{
    m_showHideToggleController.ToggleOff();
}
