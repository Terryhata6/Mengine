MENGINE_PROJECT(PNGPlugin)

ADD_FILTER(
src
    PNGPlugin.cpp
    PNGPlugin.def
    PNGPlugin.h

    ImageDecoderPNG.cpp
    ImageDecoderPNG.h
    ImageEncoderPNG.cpp
    ImageEncoderPNG.h
)

INCLUDE_DIRECTORIES(${THIRDPARTY_DIR}/zlib)
INCLUDE_DIRECTORIES(${THIRDPARTY_CONFIG_DIR}/zlib)

INCLUDE_DIRECTORIES(${THIRDPARTY_DIR}/libpng)

ADD_MENGINE_PLUGIN(MENGINE_PLUGIN_PNG)

if(MENGINE_TOOLCHAIN_MSVC)
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${THIRDPARTY_LIB_DIR}/${MENGINE_LIB_PREFIX}libpng16_static${MENGINE_DEPENDENCIES_DEBUG_SUFFIX}${MENGINE_LIB_SUFFIX})
else()
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${THIRDPARTY_LIB_DIR}/${MENGINE_LIB_PREFIX}png16${MENGINE_DEPENDENCIES_DEBUG_SUFFIX}${MENGINE_LIB_SUFFIX})
endif()

TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${THIRDPARTY_LIB_DIR}/${MENGINE_LIB_PREFIX}zlib${MENGINE_LIB_SUFFIX})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${THIRDPARTY_LIB_DIR}/${MENGINE_LIB_PREFIX}stdex${MENGINE_LIB_SUFFIX})

TARGET_LINK_LIBRARIES(${PROJECT_NAME} Kernel)