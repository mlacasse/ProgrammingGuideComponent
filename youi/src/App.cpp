// © You i Labs Inc. 2000-2020. All rights reserved.
#include "App.h"

#include <JSBundlingStrings.h>
#include <automation/YiWebDriverLocator.h>
#include <cxxreact/JSBigString.h>
#include <glog/logging.h>
#include <logging/YiLogger.h>
#include <logging/YiLoggerHelper.h>


App::App() = default;

App::~App() = default;

using namespace yi::react;

bool App::UserInit()
{
    // Start the web driver for allowing the use of Appium.
    CYIWebDriver *pWebDriver = CYIWebDriverLocator::GetWebDriver();
    if (pWebDriver)
    {
        pWebDriver->Start();
    }

#if !defined(YI_MINI_GLOG)
    // miniglog defines this using a non-const char * causing a compile error and it has no implementation anyway.
    static bool isGoogleLoggingInitialized = false;
    if (!isGoogleLoggingInitialized)
    {
        google::InitGoogleLogging("--logtostderr=1");
        isGoogleLoggingInitialized = true;
    }
#endif

    // App wide Log preferences
    CYILogger::SetLevel(EYILogLevel::debug);

    // Now create and install our logging filter.
    CYILogger::PushFilter(CYILogger::CreateFilter({
        { "CYIAssetDownloadHelper", EYILogLevel::err },
        { "CYIExoPlayer", EYILogLevel::err },
        { "CYIFocusState", EYILogLevel::err },
        { "CYIHTTPService", EYILogLevel::err },
        { "CYIHTTPServiceStats", EYILogLevel::err },
        { "CYIImageView", EYILogLevel::err },
        { "CYILRUCache", EYILogLevel::err },
        { "CYIPersistentStorePriv_Default", EYILogLevel::err },
        { "CYIPersistentCache", EYILogLevel::err },
        { "CYISceneManager", EYILogLevel::err },
        { "CYIScreenTransitionManager", EYILogLevel::err },
        { "CYISecureStorageBridgeDefault", EYILogLevel::err },
        { "CYITCPSocketPriv_Base", EYILogLevel::err },
        { "CYITransferHandle", EYILogLevel::err },
        { "DecoratorMap", EYILogLevel::err },
        { "EventDispatcherModule", EYILogLevel::err },
        { "MakeMethod_18ImageUtilityModule", EYILogLevel::err },
        { "NativeAnimatedNodesManager", EYILogLevel::err },
        { "NativeModuleBase", EYILogLevel::err },
        { "TimingModule", EYILogLevel::err },
        { "Transfer", EYILogLevel::err },
        { "ShadowTree", EYILogLevel::err },
        { "UIManagerModule", EYILogLevel::err },

        // Info messages
        { "JavaScript", EYILogLevel::info },

        // Suppressed messages
        { "CYISceneNode", EYILogLevel::off },
        { "CYIAssetManager", EYILogLevel::off },
        { "CYITextRendererFT", EYILogLevel::off },
        { "CYITextRendererFT::AddFont", EYILogLevel::off },
        { "IComponentManager", EYILogLevel::off },
        { "ReactComponent", EYILogLevel::off },
        { "CYITextEngine", EYILogLevel::off },
        { "TextUtilities", EYILogLevel::off },
        { "TimingModule", EYILogLevel::off },

    // by default, only warnings and errors are logged
    }, EYILogLevel::debug));

    std::unique_ptr<JsBundleLoader> pBundleLoader(GetBundler());

    PlatformApp::SetJsBundleLoader(std::move(pBundleLoader));
    return PlatformApp::UserInit();
}

bool App::UserStart()
{
    return PlatformApp::UserStart();
}

void App::UserUpdate()
{
    PlatformApp::UserUpdate();
}
