// © You i Labs Inc. 2000-2020. All rights reserved.
#include "AppFactory.h"
#include "App.h"

#define APP_NAME "ProgrammingGuideComponent"

#define APP_WIDTH (1920)
#define APP_HEIGHT (1080)

std::unique_ptr<CYIApp> AppFactory::Create()
{
    return std::make_unique<App>();
}

int AppFactory::GetWindowWidth()
{
    return APP_WIDTH;
}

int AppFactory::GetWindowHeight()
{
    return APP_HEIGHT;
}

const char *AppFactory::GetWindowName()
{
    return APP_NAME;
}
