#ifndef _EPG_CHANNEL_LIST_ADAPTER_H_
#define _EPG_CHANNEL_LIST_ADAPTER_H_

#include <import/YiViewTemplate.h>
#include <scenetree/YiTextSceneNode.h>
#include <streamer/YiViewAdapter.h>
#include <utility/YiDateTime.h>

#include <forward_list>

class CYIBundle;
class CYIButton;
class CYIPooledViewRecycler;
class CYITimeInterpolator;

class EPGAssetListAdapter;
class EPGAssetModel;
class EPGChannelModel;
class EPGDetailsView;
class EPGModel;
class EPGLaneView;
class EPGListView;
class EPGTimeMarkerView;

class EPGChannelListAdapter : public CYIViewAdapter, public CYISignalHandler
{
public:
    EPGChannelListAdapter(EPGListView *pChannelList);
    virtual ~EPGChannelListAdapter() = default;

    std::shared_ptr<CYIAssetViewTemplate> GetViewTemplate(size_t index) const override;
    size_t GetItemsCount() const override;
    
    
    void SetCurrentTimeMarker(EPGTimeMarkerView *pTimeMarkerView) { m_pCurrentTimeMarker = pTimeMarkerView; };
    void ConstructChannelList(std::shared_ptr<EPGModel> pModel);
    void ConstructChannelList(const std::vector<std::shared_ptr<EPGChannelModel>> &models);
    void Clear();

    void ScrollToHorizontalPosition(float percent, bool emitScrolledSignal, float duration = 0.0f, CYITimeInterpolator *pInterpolator = nullptr);
    
    float GetHorizontalPosition() { return m_horizontalPosition; }
    bool RevealAsset(const CYIString &channelId, const CYIString &assetId);
    
    CYISignal<const std::shared_ptr<EPGAssetModel> &, const std::shared_ptr<EPGChannelModel> &> AssetSelected;
    CYISignal<const std::shared_ptr<EPGAssetModel> &, const std::shared_ptr<EPGChannelModel> &> AssetGainedFocus;
    CYISignal<const std::shared_ptr<EPGAssetModel> &, const std::shared_ptr<EPGChannelModel> &> AssetLostFocus;
    CYISignal<const std::shared_ptr<EPGChannelModel> &> ChannelButtonPressed;
    CYISignal<const std::shared_ptr<EPGChannelModel> &, bool> ChannelFavoriteButtonToggled;

    /*! \brief this signal is called whenever one of the channel list is scrolled.
     * if there are any others things you want to bind to EPG scrolling, they can be done with this signal.
     */
    CYISignal<float> ChannelScrolled;

protected:
    void PopulateView(size_t uIndex, CYISceneView *pView) override;
    void OnReleaseView(size_t index, CYISceneView *pView) override;

private:
    void OnModelAddedChannelAtIndex(size_t index);
    std::shared_ptr<EPGChannelModel> GetChannelData(size_t index);

    void ShowDetailsItem(size_t index);
    void HideDetailsItem();
    void UpdateTimeMarkerGap();
    
    void OnAssetGainedFocus(const std::shared_ptr<EPGAssetModel>& pAsset, const std::shared_ptr<EPGChannelModel> &pChannel);
    void OnAssetLostFocus(const std::shared_ptr<EPGAssetModel>& pAsset, const std::shared_ptr<EPGChannelModel> &pChannel);
    void OnChannelGainedFocus(const std::shared_ptr<EPGChannelModel> &pChannel);
    void OnChannelLostFocus(const std::shared_ptr<EPGChannelModel> &pChannel);

    void OnVerticalPositionChanged();
    void OnChannelHorizontalScroll(float position);
    void OnChannelButtonGainedFocus();
    void OnChannelFavoriteButtonToggled(bool state, int32_t buttonId);
    void OnChannelButtonPressed(int32_t buttonId);

    std::shared_ptr<CYIPooledViewRecycler> m_pEPGViewRecycler;
    std::forward_list<EPGLaneView*> m_laneViews;
    std::vector<CYIString> m_channelIDs;
    
    std::shared_ptr<EPGModel> m_pEPGModel;
    EPGListView *m_pChannelsList;
    EPGTimeMarkerView *m_pCurrentTimeMarker;

    size_t m_detailsItemIndex = 0;
    bool m_isDetailsItemVisible = false;
    std::shared_ptr<EPGAssetModel> m_pDetailsItemModel;
    std::shared_ptr<EPGChannelModel> m_pDetailsChannelModel;
    EPGDetailsView *m_pDetailsItemView = nullptr;

    float m_horizontalPosition;
    std::unique_ptr<CYITimeInterpolator> m_pInterpolator;
};

#endif
