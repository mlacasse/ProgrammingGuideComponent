#include "EPGChannelListView.h"

#include "EPGAssetListAdapter.h"

#define LOG_TAG "EPGChannelListView"

YI_TYPE_DEF_INST(EPGChannelListView, CYIListView)

EPGChannelListView::EPGChannelListView()
{
    HorizontalPositionChanged.Connect(*this, &EPGChannelListView::HandleHorizontalPositionChanged);
}

bool EPGChannelListView::RequestFocus(CYIFocus::Direction direction, CYIFocus::FocusRootRule focusRootRule, const CYIAABB &previousFocusRect, const CYIFocusSearchOptions &options)
{
    CYIAABB prevRect = previousFocusRect;

    // Stops focus from jumping us up the list when leaving a very large item
    if (direction == CYIFocus::Direction::Down || direction == CYIFocus::Direction::Up)
    {
        glm::vec3 topLeft = prevRect.GetTopLeft();
        glm::vec3 bottomRight = prevRect.GetBottomRight();
        bottomRight.x = topLeft.x + 300.f;
        prevRect.SetCorners(topLeft, bottomRight);
    }
    else if (direction == CYIFocus::Direction::Forward)
    {
        return RequestFocusOnItem(0);
    }

    return CYIScrollingView::RequestFocus(direction, focusRootRule, prevRect, options);
}

bool EPGChannelListView::FocusAssetWithId(const CYIString &assetId)
{
    if (assetId.IsEmpty())
    {
        return false;
    }

    int32_t index = static_cast<EPGAssetListAdapter*>(GetAdapter())->GetIndexForAsset(assetId);
    if (index > 0)
    {
        RevealItem(index, 0.f);
        return RequestFocusOnItem(index);
    }
    else
    {
        YI_LOGD(LOG_TAG, "FocusAssetWithId failed - no asset found with ID %s", assetId.GetData());
        return false;
    }
}

void EPGChannelListView::OnFocusGainedForItem(size_t index)
{
    YI_UNUSED(index);
    m_hasFocusedItem = true;
}

void EPGChannelListView::OnFocusLostForItem(size_t index)
{
    YI_UNUSED(index);
    m_hasFocusedItem = false;
}

void EPGChannelListView::HandleHorizontalPositionChanged(float position)
{
    if (HasFocusedItem())
    {
        FocusedHorizontalPositionChanged.Emit(position);
    }
}
