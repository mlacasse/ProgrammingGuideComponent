#include "EPGListView.h"

#define LOG_TAG "EPGListView"

YI_TYPE_DEF_INST(EPGListView, CYIListView)

EPGListView::EPGListView() : CYIListView()
{
}

void EPGListView::ApplyMeasurements()
{
    CYIListView::ApplyMeasurements();
    
    MeasurementsApplied.Emit();
}
