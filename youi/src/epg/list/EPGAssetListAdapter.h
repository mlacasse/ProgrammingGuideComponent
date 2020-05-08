#ifndef _EPG_ASSET_LIST_ADAPTER_H_
#define _EPG_ASSET_LIST_ADAPTER_H_

#include <import/YiViewTemplate.h>
#include <scenetree/YiTextSceneNode.h>
#include <signal/YiSignal.h>
#include <streamer/YiViewAdapter.h>
#include <view/YiListView.h>

class EPGAssetModel;
class EPGChannelListView;
class EPGChannelModel;
class EPGAssetView;

class CYIBundle;
class CYIButton;
class CYIDateTime;

struct EPGAssetListElement
{
    std::shared_ptr<EPGAssetModel> AssetModel;
    glm::vec3 CardSize;
    
    EPGAssetListElement(std::shared_ptr<EPGAssetModel> pModel, glm::vec3 cardSize)
        : AssetModel(pModel)
        , CardSize(cardSize)
    {
    }
};

class EPGAssetListAdapter : public CYIViewAdapter, public CYISignalHandler
{
public:
    EPGAssetListAdapter(const std::shared_ptr<EPGChannelModel> &pChannelModel,
                        EPGChannelListView *pListView,
                        std::shared_ptr<IYIViewRecycler> pViewRecycler);
    virtual ~EPGAssetListAdapter() = default;
    
    std::shared_ptr<CYIAssetViewTemplate> GetViewTemplate(size_t index) const override;
    glm::vec3 GetInitialNodeSize(size_t index) const override;
    size_t GetItemsCount() const override;
    
    void BuildElements(const CYIDateTime &startTime, const CYIDateTime &endTime);
    int32_t GetIndexForAsset(const CYIString &assetId);

    CYISignal<const std::shared_ptr<EPGAssetModel> &/*assetModel*/, const std::shared_ptr<EPGChannelModel> &/*channelModel*/> AssetSelected;
    CYISignal<const std::shared_ptr<EPGAssetModel> &/*assetModel*/, const std::shared_ptr<EPGChannelModel> &/*channelModel*/> AssetGainedFocus;
    CYISignal<const std::shared_ptr<EPGAssetModel> &/*assetModel*/, const std::shared_ptr<EPGChannelModel> &/*channelModel*/> AssetLostFocus;

protected:
    void PopulateView(size_t index, CYISceneView *pView) override;
    void OnReleaseView(size_t index, CYISceneView *pView) override;
    
private:
    float CalculateScaleForSeconds(int64_t seconds);
    void InsertEmptyElement(int64_t seconds, const float listSpacing);
    void InsertElementForTemplate(const std::shared_ptr<EPGAssetModel> &pModel,
                                  const CYIString &templateName,
                                  const float horizontalScaleFactor,
                                  const float listSpacing);
    
    void OnAssetSelected(int32_t index);
    void OnAssetGainedFocus(int32_t index);
    void OnAssetLostFocus(int32_t index);
 
    std::shared_ptr<EPGChannelModel> m_pChannelModel;
    EPGChannelListView *m_pListView;
    std::vector<EPGAssetListElement> m_listElements;
    float m_listWidth = 0.0f;

    CYISignal<> OnTick;
};

#endif
