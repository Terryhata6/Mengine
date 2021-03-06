MENGINE_PROJECT(SDLApplication)

SET(MENGINE_XCODE_DEPLOY_PATH "Desktop" CACHE STRING "Mengine Xcode deploy path")

MESSAGE("MENGINE_XCODE_DEPLOY_PATH: ${MENGINE_XCODE_DEPLOY_PATH}")

ADD_FILTER(
src
    main.cpp
    SDLApplication.cpp
    SDLApplication.h
    SDLMessageBoxLogger.cpp
    SDLMessageBoxLogger.h
    SDLStdioLogger.cpp
    SDLStdioLogger.h
)

if(MENGINE_TARGET_ANDROID)
    ADD_FILTER(
    android
        AndroidLogger.cpp
        AndroidLogger.h
    )
endif()

INCLUDE_DIRECTORIES(${THIRDPARTY_DIR}/SDL2/include)

LINK_DIRECTORIES(${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})

if(MENGINE_TARGET_WINDOWS)
    ADD_EXECUTABLE(${PROJECT_NAME} WIN32 ${SRC_FILES})
endif()

if(MENGINE_TARGET_UNIX)
    ADD_EXECUTABLE(${PROJECT_NAME} ${SRC_FILES})
endif()

if(MENGINE_TARGET_ANDROID)
    ADD_LIBRARY(${PROJECT_NAME} SHARED ${SRC_FILES})
endif()

if(MENGINE_TARGET_IOS AND NOT MENGINE_BUILD_ONLY_EXECUTABLE_MODULE)
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "$ENV{HOME}/${MENGINE_XCODE_DEPLOY_PATH}/deploy-ios-plist")

    set(myApp_MEDIA "$ENV{HOME}/${MENGINE_XCODE_DEPLOY_PATH}/deploy-ios-media.xcassets")
    set_source_files_properties(${myApp_MEDIA} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")

    set(DEPLOY_DATA "$ENV{HOME}/${MENGINE_XCODE_DEPLOY_PATH}/deploy-ios-data")
    file(GLOB_RECURSE myApp_DATAS "${DEPLOY_DATA}/*")
    list(FILTER myApp_DATAS EXCLUDE REGEX ".*.DS_Store$")
    foreach(myApp_DATA ${myApp_DATAS})
        file(RELATIVE_PATH RES ${DEPLOY_DATA} ${myApp_DATA})
        get_filename_component(RES_LOC ${RES} PATH)

        set_source_files_properties(${myApp_DATA} PROPERTIES MACOSX_PACKAGE_LOCATION "deploy-ios-data/${RES_LOC}" HEADER_FILE_ONLY 1)
        string(REPLACE "/" "\\" ide_group_name "Data/${RES_LOC}")
        source_group(${ide_group_name} FILES ${myApp_DATA})
    endforeach()

    string(TIMESTAMP MACOSX_FRAMEWORK_BUNDLE_VERSION "%Y.%m.%d.%H%M%S")
    string(TIMESTAMP MACOSX_FRAMEWORK_SHORT_VERSION_STRING "%Y.%m.%d")

    ADD_EXECUTABLE(${PROJECT_NAME} MACOSX_BUNDLE ${myApp_MEDIA} ${myApp_DATAS} ${SRC_FILES})

    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES MACOSX_BUNDLE_INFO_PLIST info.plist.in)
    
    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES XCODE_ATTRIBUTE_INSTALL_PATH "$(LOCAL_APPS_DIR)")
    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES XCODE_ATTRIBUTE_SKIP_INSTALL "No")
    
    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES XCODE_SCHEME_WORKING_DIRECTORY "${CMAKE_INSTALL_PREFIX}")
    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES XCODE_GENERATE_SCHEME TRUE)
    
    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES XCODE_ATTRIBUTE_OTHER_CODE_SIGN_FLAGS "--deep")

    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES XCODE_ATTRIBUTE_TARGETED_DEVICE_FAMILY "1,2")
    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES XCODE_ATTRIBUTE_COMBINE_HIDPI_IMAGES "NO")

    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES XCODE_ATTRIBUTE_ASSETCATALOG_COMPILER_APPICON_NAME AppIcon)
    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES XCODE_ATTRIBUTE_ASSETCATALOG_COMPILER_LAUNCHIMAGE_NAME LaunchImage)
    
    include("$ENV{HOME}/${MENGINE_XCODE_DEPLOY_PATH}/project_ios_deploy.cmake")
endif()

if(MENGINE_TARGET_IOS AND MENGINE_BUILD_ONLY_EXECUTABLE_MODULE)
    ADD_EXECUTABLE(${PROJECT_NAME} MACOSX_BUNDLE ${SRC_FILES})
endif()

if(MENGINE_TARGET_MACOS)
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "$ENV{HOME}/${MENGINE_XCODE_DEPLOY_PATH}/deploy-mac-plist")

    set(myApp_ICONS "$ENV{HOME}/${MENGINE_XCODE_DEPLOY_PATH}/deploy-mac-icons.icns")
    set_source_files_properties(${myApp_ICONS} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")

    if(MENGINE_PLUGIN_ASTRALAX)
        set(myApp_ASTRALAX "${THIRDPARTY_DIR}/astralax_mac/magic3d.dylib")
        set_source_files_properties(${myApp_ASTRALAX} PROPERTIES MACOSX_PACKAGE_LOCATION "MacOS")
        set_source_files_properties(${myApp_ASTRALAX} PROPERTIES XCODE_FILE_ATTRIBUTES "CodeSignOnCopy")
    else()
        set(myApp_ASTRALAX)
    endif()

    if(MENGINE_PLUGIN_SENTRY)
        set(myApp_SENTRY "$ENV{HOME}/${MENGINE_XCODE_DEPLOY_PATH}/crashpad_handler")
        set_source_files_properties(${myApp_ASTRALAX} PROPERTIES MACOSX_PACKAGE_LOCATION "MacOS")

        set_source_files_properties($ENV{HOME}/${MENGINE_XCODE_DEPLOY_PATH}/crashpad_handler PROPERTIES MACOSX_PACKAGE_LOCATION "Resources/" HEADER_FILE_ONLY 1)
    else()
        set(myApp_SENTRY)
    endif()

    if(MENGINE_PLUGIN_STEAM)
        set(myApp_STEAM "${THIRDPARTY_DIR}/steam_mac/libsteam_api.dylib")
        set_source_files_properties(${myApp_STEAM} PROPERTIES MACOSX_PACKAGE_LOCATION "MacOS")
    else()
        set(myApp_STEAM)
    endif()
    
    set(DEPLOY_DATA "$ENV{HOME}/${MENGINE_XCODE_DEPLOY_PATH}/deploy-mac-data")
    file(GLOB_RECURSE myApp_DATAS "${DEPLOY_DATA}/*")
    list(FILTER myApp_DATAS EXCLUDE REGEX ".*.DS_Store$")
    foreach(myApp_DATA ${myApp_DATAS})
        file(RELATIVE_PATH RES ${DEPLOY_DATA} ${myApp_DATA})
        get_filename_component(RES_LOC ${RES} PATH)

        set_source_files_properties(${myApp_DATA} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources/${RES_LOC}" HEADER_FILE_ONLY 1)
        string(REPLACE "/" "\\" ide_group_name "Resources/${RES_LOC}")
        source_group(${ide_group_name} FILES ${myApp_DATA})
    endforeach()
    
    string(TIMESTAMP MACOSX_FRAMEWORK_BUNDLE_VERSION "%Y.%m.%d.%H%M%S")
    string(TIMESTAMP MACOSX_FRAMEWORK_SHORT_VERSION_STRING "%Y.%m.%d")

    ADD_EXECUTABLE(${PROJECT_NAME} MACOSX_BUNDLE ${myApp_DATAS} ${myApp_ICONS} ${myApp_ASTRALAX} ${myApp_SENTRY} ${myApp_STEAM} ${SRC_FILES})

    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES XCODE_ATTRIBUTE_INSTALL_PATH "$(LOCAL_APPS_DIR)")
    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES XCODE_ATTRIBUTE_SKIP_INSTALL "No")
    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES XCODE_GENERATE_SCHEME TRUE)
    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES XCODE_ATTRIBUTE_OTHER_CODE_SIGN_FLAGS "--deep")

    include("$ENV{HOME}/${MENGINE_XCODE_DEPLOY_PATH}/project_mac_deploy.cmake")

    get_target_property(MACOSX_BUNDLE_GUI_IDENTIFIER ${PROJECT_NAME} XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER)

    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES MACOSX_BUNDLE_INFO_PLIST info.plist.in)
endif()

TARGET_LINK_LIBRARIES(${PROJECT_NAME} Kernel)

if(MENGINE_PLUGIN_MENGINE_STATIC)
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} Mengine)
else()
    ADD_DEPENDENCIES(${PROJECT_NAME} Mengine)
endif()

if(MENGINE_TARGET_WINDOWS)
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${THIRDPARTY_LIB_DIR}/${MENGINE_LIB_PREFIX}SDL2${MENGINE_DEPENDENCIES_DEBUG_SUFFIX}${MENGINE_LIB_SUFFIX})
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${THIRDPARTY_LIB_DIR}/${MENGINE_LIB_PREFIX}SDL2main${MENGINE_DEPENDENCIES_DEBUG_SUFFIX}${MENGINE_LIB_SUFFIX})
endif()

if(MENGINE_TARGET_MACOS)
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${THIRDPARTY_LIB_DIR}/${MENGINE_LIB_PREFIX}SDL2${MENGINE_DEPENDENCIES_DEBUG_SUFFIX}${MENGINE_LIB_SUFFIX})
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${THIRDPARTY_LIB_DIR}/${MENGINE_LIB_PREFIX}SDL2main${MENGINE_DEPENDENCIES_DEBUG_SUFFIX}${MENGINE_LIB_SUFFIX})    
endif()

if(MENGINE_TARGET_IOS)
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${THIRDPARTY_LIB_DIR}/${MENGINE_LIB_PREFIX}SDL2${MENGINE_DEPENDENCIES_DEBUG_SUFFIX}${MENGINE_LIB_SUFFIX})
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${THIRDPARTY_LIB_DIR}/${MENGINE_LIB_PREFIX}SDL2main${MENGINE_DEPENDENCIES_DEBUG_SUFFIX}${MENGINE_LIB_SUFFIX})
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} iconv)
endif()

if(MENGINE_TARGET_UNIX)
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${THIRDPARTY_LIB_DIR}/${MENGINE_LIB_PREFIX}SDL2${MENGINE_DEPENDENCIES_DEBUG_SUFFIX}${MENGINE_LIB_SUFFIX})
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${THIRDPARTY_LIB_DIR}/${MENGINE_LIB_PREFIX}SDL2main${MENGINE_DEPENDENCIES_DEBUG_SUFFIX}${MENGINE_LIB_SUFFIX})
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} pthread)
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} dl)
endif()

IF(MENGINE_TARGET_IOS)
    ADD_APPLE_FRAMEWORK(CoreData)
    ADD_APPLE_FRAMEWORK(CoreAudio)
    ADD_APPLE_FRAMEWORK(CoreVideo)
    ADD_APPLE_FRAMEWORK(CoreFoundation)
    ADD_APPLE_FRAMEWORK(CoreMotion)
    ADD_APPLE_FRAMEWORK(CoreBluetooth)
    ADD_APPLE_FRAMEWORK(CoreHaptics)
    ADD_APPLE_FRAMEWORK(AudioToolbox)
    ADD_APPLE_FRAMEWORK(AVFoundation)
    ADD_APPLE_FRAMEWORK(IOKit)
    ADD_APPLE_FRAMEWORK(UIKit)
    ADD_APPLE_FRAMEWORK(GameController)
    ADD_APPLE_FRAMEWORK(MediaPlayer)
    ADD_APPLE_FRAMEWORK(QuartzCore)
    ADD_APPLE_FRAMEWORK(Foundation)
    ADD_APPLE_FRAMEWORK(OpenGLES)
    ADD_APPLE_FRAMEWORK(CoreGraphics)
    ADD_APPLE_FRAMEWORK(OpenAL)
    ADD_APPLE_FRAMEWORK(Security)
    ADD_APPLE_FRAMEWORK(Metal)
ENDIF()

IF(MENGINE_TARGET_ANDROID)
    IF(UNIX)
        string(TOLOWER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE_CORRECT)
    ELSE()
        SET(CMAKE_BUILD_TYPE_CORRECT ${CMAKE_BUILD_TYPE})
    ENDIF()

    add_library(SDL2_IMPORTED SHARED IMPORTED)
            
    SET(SDL2_IMPORTED_LOCATION_PATH ${MENGINE_REPOSITORY}/gradle/libraries/SDL2/build/intermediates/cmake/${CMAKE_BUILD_TYPE_CORRECT}/obj/${ANDROID_ABI}/libSDL2.so)
             
    set_target_properties(SDL2_IMPORTED PROPERTIES IMPORTED_LOCATION ${SDL2_IMPORTED_LOCATION_PATH})
            
    MESSAGE("SDL2_IMPORTED_LOCATION_PATH: ${SDL2_IMPORTED_LOCATION_PATH}")

    TARGET_LINK_LIBRARIES(${PROJECT_NAME} SDL2_IMPORTED)
    
    add_library(hidapi_IMPORTED SHARED IMPORTED)
    
    SET(HIDAPI_IMPORTED_LOCATION_PATH ${MENGINE_REPOSITORY}/gradle/libraries/SDL2/build/intermediates/cmake/${CMAKE_BUILD_TYPE_CORRECT}/obj/${ANDROID_ABI}/libhidapi.so)
             
    set_target_properties(hidapi_IMPORTED PROPERTIES IMPORTED_LOCATION ${HIDAPI_IMPORTED_LOCATION_PATH})

    MESSAGE("HIDAPI_IMPORTED_LOCATION_PATH: ${HIDAPI_IMPORTED_LOCATION_PATH}")

    TARGET_LINK_LIBRARIES(${PROJECT_NAME} hidapi_IMPORTED)

    target_link_libraries(${PROJECT_NAME}
        stdc++
        c
        m
        dl
        log
        android
        OpenSLES
        GLESv1_CM
        GLESv2
    )
endif()

IF(MENGINE_TARGET_MACOS)
    ADD_APPLE_FRAMEWORK(Cocoa)
    ADD_APPLE_FRAMEWORK(Carbon)
    ADD_APPLE_FRAMEWORK(AppKit)
    ADD_APPLE_FRAMEWORK(CoreData)
    ADD_APPLE_FRAMEWORK(CoreAudio)
    ADD_APPLE_FRAMEWORK(CoreVideo)
    ADD_APPLE_FRAMEWORK(CoreFoundation)
    ADD_APPLE_FRAMEWORK(AudioUnit)
    ADD_APPLE_FRAMEWORK(AudioToolBox)
    ADD_APPLE_FRAMEWORK(IOKit)
    ADD_APPLE_FRAMEWORK(ForceFeedback)
    ADD_APPLE_FRAMEWORK(Foundation)
    ADD_APPLE_FRAMEWORK(OpenGL)
    ADD_APPLE_FRAMEWORK(OpenAL)
    ADD_APPLE_FRAMEWORK(Security)
    ADD_APPLE_FRAMEWORK(Metal)
    
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} iconv)
ENDIF()

IF(MENGINE_TARGET_WINDOWS)
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} Winmm Version Imm32 Setupapi)
ENDIF()

IF(MENGINE_TOOLCHAIN_MSVC)
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS /FORCE:MULTIPLE)
ENDIF()

IF(MENGINE_TARGET_IOS AND MENGINE_BUILD_ONLY_EXECUTABLE_MODULE)
    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "")
    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES XCODE_ATTRIBUTE_CODE_SIGNING_REQUIRED "NO")
    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES XCODE_ATTRIBUTE_CODE_SIGNING_ALLOWED "NO")
ENDIF()