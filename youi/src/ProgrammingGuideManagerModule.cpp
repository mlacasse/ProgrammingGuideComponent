#include "ProgrammingGuideManagerModule.h"

#include "ProgrammingGuideProps.h"
#include "ShadowProgrammingGuideView.h"

#include <youireact/NativeModuleRegistry.h>
#include <youireact/nodes/ReactComponent.h>

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

//std::vector<CYIString, ProgrammingGuideChannel> ValidateChannels(std::vector<ProgrammingGuideChannel> channels)
//{
//    
//}

}   // namespace

folly::dynamic ProgrammingGuideManagerModule::GetNativeProps()
{
    folly::dynamic superProps = IComponentManager::GetNativeProps();
    folly::dynamic props = folly::dynamic::object("channels", "array")("schedules", "array");
    return folly::dynamic::merge(superProps, props);
}

void ProgrammingGuideManagerModule::SetupProperties()
{
    IComponentManager::SetupProperties();
    
//    YI_RN_DEFINE_PROPERTY("channels", [](ShadowProgrammingGuideView &self, std::vector<ProgrammingGuideChannel> channels) {
//        self.GetSavedProps()->channels = ValidateChannels(channels);
//    });
}
