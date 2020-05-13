#ifndef _EPG_ON_NOW_LIST_ADAPTER_H_
#define _EPG_ON_NOW_LIST_ADAPTER_H_

#include <import/YiViewTemplate.h>
#include <scenetree/YiTextSceneNode.h>
#include <streamer/YiViewAdapter.h>
#include <utility/YiDateTime.h>

#include <forward_list>

class EPGAssetModel;
class EPGChannelModel;
class EPGOnNowView;

class EPGOnNowListAdapter : public CYIViewAdapter, public CYISignalHandler
{
public:
    EPGOnNowListAdapter();
    virtual ~EPGOnNowListAdapter() = default;

    std::shared_ptr<CYIAssetViewTemplate> GetViewTemplate(size_t index) const override;
    size_t GetItemsCount() const override;

    void ConstructChannelList(const std::vector<std::shared_ptr<EPGChannelModel>> &models);
    void Clear();
    void Tick();

    CYISignal<const std::shared_ptr<EPGAssetModel> &, const std::shared_ptr<EPGChannelModel> &> AssetSelected;
    CYISignal<const std::shared_ptr<EPGChannelModel> &> ChannelButtonPressed;
    CYISignal<const std::shared_ptr<EPGChannelModel> &, bool> ChannelFavoriteButtonToggled;

protected:
    void PopulateView(size_t uIndex, CYISceneView *pView) override;
    void OnReleaseView(size_t index, CYISceneView *pView) override;

private:
    std::vector<std::shared_ptr<EPGChannelModel>> m_channelModels;
    std::forward_list<EPGOnNowView*> m_streamedViews;
};

#endif
