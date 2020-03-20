#ifndef _SHADOW_PROGRAMMING_GUIDE_VIEW_H_
#define _SHADOW_PROGRAMMING_GUIDE_VIEW_H_

#include "ProgrammingGuideManagerModule.h"
#include "ProgrammingGuideProps.h"

#include <youireact/NativeModule.h>
#include <youireact/nodes/ShadowView.h>

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

    virtual const CYISceneNode *GetCounterpart() const override;
    virtual CYISceneNode *GetCounterpart() override;

    virtual ProgrammingGuideProps *GetSavedProps() override;

private:
    virtual std::unique_ptr<CYISceneNode> CreateCounterpart(CYISceneManager *pSceneManager) override;

    virtual folly::dynamic GetDefaultProps() const final;

    virtual void OnLayoutApplied() final;
    virtual void ApplyProps(folly::dynamic properties) override;

    YI_TYPE_BASES(ShadowProgrammingGuideView, ShadowView);
};

} // namespace react
} // namespace yi

#endif // _SHADOW_PROGRAMMING_GUIDE_VIEW_H_
