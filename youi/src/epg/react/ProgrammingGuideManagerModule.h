#ifndef _PROGRAMMING_GUIDE_MANAGER_MODULE_H_
#define _PROGRAMMING_GUIDE_MANAGER_MODULE_H_

#include "ProgrammingGuideProps.h"

#include <youireact/modules/components/AbstractComponentManagerModule.h>
#include <youireact/modules/components/IViewManager.h>

namespace yi
{
namespace react
{
class YI_RN_MODULE(ProgrammingGuideManagerModule, AbstractComponentManagerModule), public IViewManager
{
public:
    YI_RN_EXPORT_NAME(ProgrammingGuideManager);

    YI_RN_EXPORT_COMMAND(Reset)
    (ReactComponent *pShadowView, folly::dynamic args);

    YI_RN_EXPORT_COMMAND(Prepend)
    (ReactComponent *pShadowView, std::vector<std::shared_ptr<EPGChannelModel>> channels);

    YI_RN_EXPORT_COMMAND(Append)
    (ReactComponent *pShadowView, std::vector<std::shared_ptr<EPGChannelModel>> channels);

    YI_RN_EXPORT_COMMAND(SetCurrentChannel)
    (ReactComponent *pShadowView, std::map<CYIString, CYIString> currentChannelIndex);

    YI_RN_DEFINE_COMPONENT_MODULE();

protected:
    virtual void SetupProperties() override;
    virtual ReactComponent *GetComponent(uint64_t tag) override;

private:
    void SetSchedulePriv();

};
} // namespace react
} // namespace yi

#endif  // _PROGRAMMING_GUIDE_MANAGER_MODULE_H_
