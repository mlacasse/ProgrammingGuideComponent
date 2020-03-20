set(YI_PROJECT_NAME "ProgrammingGuideComponent") # The name of the project. When building for Android, this is passed from Android Studio.
set(YI_PACKAGE_NAME "tv.youi.programmingguidecomponent") # The package name used for the project.")
set(YI_DISPLAY_NAME "ProgrammingGuideComponent") # The name of the application displayed on the device.

set(YI_VERSION_MAJOR 1)
set(YI_VERSION_MINOR 0)
set(YI_VERSION_PATCH 0)

#basic definitions
set(CACHE_FILE "BuildNumberCache.txt")
set(HEADER_FILE "${CMAKE_CURRENT_SOURCE_DIR}/src/Version.h")

#Reading data from file + incrementation
IF(EXISTS ${CACHE_FILE})
    file(READ ${CACHE_FILE} YI_BUILD_NUMBER)
    math(EXPR YI_BUILD_NUMBER "${YI_BUILD_NUMBER}+1")
ELSE()
    set(YI_BUILD_NUMBER "1")
ENDIF()

set(YI_VERSION_NUMBER "${YI_VERSION_MAJOR}.${YI_VERSION_MINOR}.${YI_VERSION_PATCH}.${YI_BUILD_NUMBER}" CACHE STRING "Build version of the application. This is the version that will be displayed to the end user in the store.")

#Update the cache
file(WRITE ${CACHE_FILE} "${YI_BUILD_NUMBER}")

#Create the header
file(WRITE ${HEADER_FILE} "#ifndef _VERSION_H_\n#define _VERSION_H_\n\n#define VERSION_NUMBER \"${YI_VERSION_NUMBER}\"\n#define VERSION_MAJOR ${YI_VERSION_MAJOR}\n#define VERSION_MINOR ${YI_VERSION_MINOR}\n#define VERSION_PATCH ${YI_VERSION_PATCH}\n#define BUILD_NUMBER ${YI_BUILD_NUMBER}\n\n#endif    // _VERSION_H_\n")
