#ifndef _EPG_LIST_VIEW_H_
#define _EPG_LIST_VIEW_H_

#include "view/YiListView.h"

class EPGListView : public CYIListView
{
public:
    EPGListView();
    virtual ~EPGListView() = default;
    
    CYISignal<> MeasurementsApplied;
    
protected:
    void ApplyMeasurements() override;

    YI_DISALLOW_COPY_AND_ASSIGN(EPGListView)

    YI_TYPE_BASES(EPGListView, CYIListView)
};

#endif
