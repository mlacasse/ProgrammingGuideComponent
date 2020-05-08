#ifndef _EPG_ASSET_VIEW_H_
#define _EPG_ASSET_VIEW_H_

#include <layout/YiLayout.h>
#include <signal/YiSignal.h>
#include <utility/YiDateTime.h>
#include <view/YiSceneView.h>

class EPGChannelModel;
class EPGChannelListView;
class EPGAssetModel;

class CYITextSceneNode;
class CYIPushButtonView;

class EPGAssetView : public CYISceneView
{
public:
    EPGAssetView();
    virtual ~EPGAssetView() = default;

    bool Init() override;
    void Reset() override;
    
    void Format(const std::shared_ptr<EPGAssetModel> &pModel, int32_t id);
    
    void SetListView(EPGChannelListView *pListView);
    void SetChannelWidth(float channelWidth) { m_channelWidth = channelWidth; }
    void SetScrollPosition(float scrollPosition) { m_scrollPosition = scrollPosition; }
    void SetCardSize(const glm::vec3 &size);
    
    void UpdateContentAlignment(float scrollPercent);
    
    CYISignal<int32_t> AssetSelected;
    CYISignal<int32_t> AssetGainedFocus;
    CYISignal<int32_t> AssetLostFocus;

protected:
    void ApplyMeasurements() override;

private:
    void OnButtonPressed(int32_t id);
    void OnButtonGainedFocus();
    void OnButtonLostFocus();
    
    std::shared_ptr<EPGAssetModel> m_pAssetModel;
    CYIPushButtonView *m_pSelectButton;
    CYITextSceneNode *m_pTitleText;
    
    EPGChannelListView *m_pListView = nullptr;
    float m_channelWidth = 0.0f;
    bool m_isAlignmentModified = false;
    float m_initialTextOffset = 0.0f;
    bool m_isFirstUpdate = true;
    CYILayout::Padding m_listPadding;
    float m_scrollPosition = 0.0f;

    YI_TYPE_BASES(EPGAssetView, CYISceneView)
};

#endif
