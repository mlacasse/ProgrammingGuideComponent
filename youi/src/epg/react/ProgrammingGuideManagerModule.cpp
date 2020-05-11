#include "ProgrammingGuideManagerModule.h"

#include "ShadowProgrammingGuideView.h"

#include <youireact/NativeModuleRegistry.h>
#include <youireact/ReactBridge.h>
#include <youireact/ShadowTree.h>

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

folly::dynamic ProgrammingGuideManagerModule::GetNativeProps()
{
    folly::dynamic superProps = IViewManager::GetNativeProps();
    folly::dynamic props = folly::dynamic::object
        ("duration", "double")
        ("schedulesStartTime", "double")
        ("filters", "array")
        ("sortKeys", "array")
        ("dateSelector", "object")
        ("currentChannelIndex", "object");
    return folly::dynamic::merge(superProps, props);
}

void ProgrammingGuideManagerModule::SetupProperties()
{
    IViewManager::SetupProperties();

    YI_RN_DEFINE_PROPERTY("duration", [](ShadowProgrammingGuideView &self, double duration) {
        self.GetSavedProps()->duration = duration;
    });

    YI_RN_DEFINE_PROPERTY("schedulesStartTime", [](ShadowProgrammingGuideView &self, double schedulesStartTime) {
        self.GetSavedProps()->startTime = CYIDateTime(schedulesStartTime);
    });

    YI_RN_DEFINE_PROPERTY("currentChannelIndex", [](ShadowProgrammingGuideView &self, std::map<CYIString, CYIString> currentChannelIndex) {
        auto channel = currentChannelIndex.find("channelId");
        if (channel != currentChannelIndex.end())
        {
            self.GetSavedProps()->currentChannelId = channel->second;
        }

        auto asset = currentChannelIndex.find("resourceId");
        if (asset != currentChannelIndex.end())
        {
            self.GetSavedProps()->currentAssetId = asset->second;
        }
    });

    YI_RN_DEFINE_PROPERTY("filters", [](ShadowProgrammingGuideView &self, std::vector<EPGFilterModel> filters) {
        self.SetFilters(std::move(filters));
    });

    YI_RN_DEFINE_PROPERTY("sortKeys", [](ShadowProgrammingGuideView &self, std::vector<std::map<CYIString, CYIString>> sortKeys) {
        for (auto it = sortKeys.begin(); it != sortKeys.end(); ++it)
        {
            auto label = it->find("label");
            if (label != it->end())
            {
                YI_LOGD(LOG_TAG, "sortKey %s: %s", label->first.GetData(), label->second.GetData());
            }

            auto prop = it->find("prop");
            if (prop != it->end())
            {
                YI_LOGD(LOG_TAG, "sortKey %s: %s", prop->first.GetData(), prop->second.GetData());
            }

            // Assending uses 1 or 0 to represent true or false.
            auto ascending = it->find("ascending");
            if (ascending != it->end())
            {
                YI_LOGD(LOG_TAG, "sortKey %s: %s", ascending->first.GetData(), ascending->second.GetData());
            }
        }
    });

    YI_RN_DEFINE_PROPERTY("dateSelector", [](ShadowProgrammingGuideView &self, std::map<CYIString, CYIString> dateSelector) {
        auto label = dateSelector.find("label");
        if (label != dateSelector.end())
        {
            YI_LOGD(LOG_TAG, "dateSelector %s: %s", label->first.GetData(), label->second.GetData());
        }

        auto days = dateSelector.find("days");
        if (days != dateSelector.end())
        {
            YI_LOGD(LOG_TAG, "dateSelector %s: %s", days->first.GetData(), days->second.GetData());
        }
    });
}

ReactComponent *ProgrammingGuideManagerModule::GetComponent(uint64_t tag)
{
    return GetBridge().GetShadowTree().GetShadowRegistry().Get(tag);
}

YI_RN_DEFINE_EXPORT_COMMAND(ProgrammingGuideManagerModule, Reset)
(ReactComponent *pShadowView, folly::dynamic args)
{
    auto pView = static_cast<ShadowProgrammingGuideView *>(pShadowView);

    if (args.isArray() && args[0].isDouble())
    {
        time_t schedulesStartTime = static_cast<time_t>(args[0].asDouble());
        pView->GetSavedProps()->startTime = CYIDateTime(schedulesStartTime);
    }

    pView->Reset();
}

YI_RN_DEFINE_EXPORT_COMMAND(ProgrammingGuideManagerModule, Prepend)
(ReactComponent *pShadowView, std::vector<std::shared_ptr<EPGChannelModel>> channels)
{
    auto pView = static_cast<ShadowProgrammingGuideView *>(pShadowView);
    pView->PrependChannels(channels);
}

YI_RN_DEFINE_EXPORT_COMMAND(ProgrammingGuideManagerModule, Append)
(ReactComponent *pShadowView, std::vector<std::shared_ptr<EPGChannelModel>> channels)
{
    auto pView = static_cast<ShadowProgrammingGuideView *>(pShadowView);
    pView->AppendChannels(channels);
}

YI_RN_DEFINE_EXPORT_COMMAND(ProgrammingGuideManagerModule, SetCurrentChannel)
(ReactComponent *pShadowView, std::map<CYIString, CYIString> currentChannelIndex)
{
    auto pView = static_cast<ShadowProgrammingGuideView *>(pShadowView);

    auto channel = currentChannelIndex.find("channelId");
    if (channel != currentChannelIndex.end())
    {
        pView->GetSavedProps()->currentChannelId = channel->second;
    }

    auto asset = currentChannelIndex.find("resourceId");
    if (asset != currentChannelIndex.end())
    {
        pView->GetSavedProps()->currentAssetId = asset->second;
    }

    pView->ResetChannelFocus();
}

