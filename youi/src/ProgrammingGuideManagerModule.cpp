#include "ProgrammingGuideManagerModule.h"

#include "ProgrammingGuideProps.h"
#include "ShadowProgrammingGuideView.h"

#include <youireact/NativeModuleRegistry.h>
#include <youireact/nodes/ReactComponent.h>
#include <youireact/props/DirectEventTypes.h>

#define LOG_TAG "ProgrammingGuideManagerModule"

using namespace yi::react;

YI_RN_INSTANTIATE_MODULE(ProgrammingGuideManagerModule, AbstractComponentManagerModule);
YI_RN_REGISTER_MODULE(ProgrammingGuideManagerModule);

YI_RN_OVERRIDE_OnInit(ProgrammingGuideManagerModule);
YI_RN_OVERRIDE_OnLayoutApplied(ProgrammingGuideManagerModule);
YI_RN_OVERRIDE_ApplyProps(ProgrammingGuideManagerModule);
YI_RN_OVERRIDE_DismantleCounterpart(ProgrammingGuideManagerModule);
YI_RN_OVERRIDE_ConfigureCounterpart(ProgrammingGuideManagerModule);

namespace
{
}   // namespace

folly::dynamic ProgrammingGuideManagerModule::GetNativeProps()
{
    folly::dynamic superProps = IViewManager::GetNativeProps();
    folly::dynamic props = folly::dynamic::object("schedules", "array");
    return folly::dynamic::merge(superProps, props);
}

void ProgrammingGuideManagerModule::SetupProperties()
{
    IViewManager::SetupProperties();

    YI_RN_DEFINE_PROPERTY("schedules", [](ShadowProgrammingGuideView &self, std::vector<ChannelProps> channels) {
        
        
        
        self.GetSavedProps()->channels = channels;
        
        
    });
}
