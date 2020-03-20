#ifndef _PROGRAMMING_GUIDE_MANAGER_MODULE_H_
#define _PROGRAMMING_GUIDE_MANAGER_MODULE_H_

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

    YI_RN_DEFINE_COMPONENT_MODULE();
    
protected:
    virtual void SetupProperties() override;
};
} // namespace react
} // namespace yi

#endif  // _PROGRAMMING_GUIDE_MANAGER_MODULE_H_
