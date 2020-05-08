#ifndef _EPG_SIDE_MENU_VIEW_H_
#define _EPG_SIDE_MENU_VIEW_H_

#include "view/YiListView.h"

class EPGSideMenuView : public CYIListView
{
public:
    EPGSideMenuView();
    virtual ~EPGSideMenuView() = default;

    YI_DISALLOW_COPY_AND_ASSIGN(EPGSideMenuView)

    YI_TYPE_BASES(EPGSideMenuView, CYIListView)
};

#endif
