#ifndef _EPG_CHANNEL_LIST_VIEW_H_
#define _EPG_CHANNEL_LIST_VIEW_H_

#include <view/YiListView.h>

class EPGChannelListView : public CYIListView
{
public:
    EPGChannelListView();
    virtual ~EPGChannelListView() = default;
    
    bool HasFocusedItem() const { return m_hasFocusedItem; }
    
    CYISignal<float> FocusedHorizontalPositionChanged;

    virtual bool RequestFocus(CYIFocus::Direction direction = CYIFocus::Direction::Forward,
                              CYIFocus::FocusRootRule focusRootRule = CYIFocus::FocusRootRule::DescendantsUpdateContext,
                              const CYIAABB &previousFocusRect = CYIAABB(),
                              const CYIFocusSearchOptions &options = CYIFocusSearchOptions()) override;

    bool FocusAssetWithId(const CYIString &assetId);

protected:
    void OnFocusGainedForItem(size_t index) override;
    void OnFocusLostForItem(size_t index) override;
    
private:
    void HandleHorizontalPositionChanged(float position);
    
    bool m_hasFocusedItem = false;

    YI_DISALLOW_COPY_AND_ASSIGN(EPGChannelListView)
    YI_TYPE_BASES(EPGChannelListView, CYIListView)
};

#endif
