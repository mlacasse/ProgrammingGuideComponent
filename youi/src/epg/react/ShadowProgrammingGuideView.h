#ifndef _SHADOW_PROGRAMMING_GUIDE_VIEW_H_
#define _SHADOW_PROGRAMMING_GUIDE_VIEW_H_

#include "ProgrammingGuideManagerModule.h"
#include "ProgrammingGuideProps.h"

#include "../controller/EPGViewController.h"

#include <youireact/NativeModule.h>
#include <youireact/nodes/ShadowView.h>

#include <youireact/props/DirectEventTypes.h>

class EPGModel;

namespace yi
{
namespace react
{
class ShadowProgrammingGuideView : public ShadowView
{
public:
    ShadowProgrammingGuideView();
    virtual ~ShadowProgrammingGuideView() final;

    YI_RN_EXPORT_NAME(ShadowProgrammingGuideView);
    YI_RN_DECLARE_MANAGER(ProgrammingGuideManagerModule);

    virtual void ApplyProps(folly::dynamic properties) override;

    virtual const CYISceneView *GetCounterpart() const override;
    virtual CYISceneView *GetCounterpart() override;

    virtual ProgrammingGuideProps *GetSavedProps() override;

    YGSize YogaMeasure(YGNodeRef node, float width, YGMeasureMode widthMode, float height, YGMeasureMode heightMode) const;

    void PrependChannels(std::vector<std::shared_ptr<EPGChannelModel>> models);
    void AppendChannels(std::vector<std::shared_ptr<EPGChannelModel>> models);
    void ResetChannelFocus();
    void Reset();

private:
    virtual std::unique_ptr<CYISceneNode> CreateCounterpart(CYISceneManager *pSceneManager) override;

    virtual folly::dynamic GetDefaultProps() const final;

    virtual void OnLayoutApplied() final;

    glm::vec3 GetScaleFactor() const;

    inline void SetSize(CYISceneNode *pNode, float x, float y, float z);
    inline void SetSize(CYISceneNode *pNode, const glm::vec3 &size);

    void OnAssetSelected(const std::shared_ptr<EPGAssetModel> &asset, const std::shared_ptr<EPGChannelModel> &channel);
    void OnAssetFocused(const std::shared_ptr<EPGAssetModel> &asset, const std::shared_ptr<EPGChannelModel> &channel);
    void OnAssetUnfocused(const std::shared_ptr<EPGAssetModel> &asset, const std::shared_ptr<EPGChannelModel> &channel);

    void OnChannelButtonPressed(const std::shared_ptr<EPGChannelModel> &channel);
    void OnChannelFavoriteButtonToggled(const std::shared_ptr<EPGChannelModel> &channel, bool);

    EPGViewController m_controller;
    std::shared_ptr<EPGModel> m_pModel;

    YI_RN_EXPORT_DIRECT_EVENT(onLayout);
    YI_RN_EXPORT_DIRECT_EVENT(onFocus);
    YI_RN_EXPORT_DIRECT_EVENT(onBlur);

    YI_RN_EXPORT_DIRECT_EVENT(onHandleGuideChannelPress);
    YI_RN_EXPORT_DIRECT_EVENT_NAME(onHandleGuideChannelPressName);

    YI_RN_EXPORT_DIRECT_EVENT(onHandleFavoriteIconPress);
    YI_RN_EXPORT_DIRECT_EVENT_NAME(onHandleFavoriteIconPressName);

    YI_RN_EXPORT_DIRECT_EVENT(onHandleGuideContentPress);
    YI_RN_EXPORT_DIRECT_EVENT_NAME(onHandleGuideContentPressName);

    YI_RN_EXPORT_DIRECT_EVENT(onHandleGuideContentFocus);
    YI_RN_EXPORT_DIRECT_EVENT_NAME(onHandleGuideContentFocusName);

    YI_RN_EXPORT_DIRECT_EVENT(onHandleGuideContentBlur);
    YI_RN_EXPORT_DIRECT_EVENT_NAME(onHandleGuideContentBlurName);

    YI_TYPE_BASES(ShadowProgrammingGuideView, ShadowView);
};

} // namespace react
} // namespace yi

#endif // _SHADOW_PROGRAMMING_GUIDE_VIEW_H_
