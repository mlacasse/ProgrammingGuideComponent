#ifndef _EPG_SIDE_MENU_VIEW_CONTROLLER_H_
#define _EPG_SIDE_MENU_VIEW_CONTROLLER_H_

#include <animation/YiAnimationToggleController.h>
#include <signal/YiSignal.h>
#include <signal/YiSignalHandler.h>

class EPGFilterListAdapter;

class CYIListView;
class CYISceneView;

class EPGSideMenuViewController : public CYISignalHandler
{
public:
    EPGSideMenuViewController();
    virtual ~EPGSideMenuViewController() = default;
  
    void Init(CYISceneView *pView);
    void Populate(bool forceReset = false);

    CYISignal<const CYIString &> FilterSelected;

private:
    void OnDescendantGainedFocus();
    void OnDescendantLostFocus();

    void OnSortButtonPressed();
    void OnDaySelectorButtonPressed();

    CYISceneView *m_pView = nullptr;
    CYIListView *m_pFilterListView = nullptr;
    EPGFilterListAdapter *m_pFilterListAdapter;
    
    CYIAnimationToggleController m_showHideToggleController;
    CYIAnimationToggleController m_jumpMenuToggleController;
};

#endif
