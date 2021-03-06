MENGINE_PROJECT(ImageTrimmer)

ADD_FILTER(
src
    ImageTrimmer.cpp
)

add_definitions(-DUNICODE)

INCLUDE_DIRECTORIES(${THIRDPARTY_DIR}/tinyini/include) 
INCLUDE_DIRECTORIES(${THIRDPARTY_DIR}/libmath/include)

LINK_DIRECTORIES(${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
ADD_EXECUTABLE(${PROJECT_NAME} WIN32 ${SRC_FILES})

TARGET_LINK_LIBRARIES(${PROJECT_NAME} Kernel)

TARGET_LINK_LIBRARIES(${PROJECT_NAME} AllocatorService)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} DocumentService)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ProviderService)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} CodecService)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} FileService)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} LoggerService)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} StringizeService)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} MemoryService)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} DataService)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ThreadService)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ArchiveService)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ConfigService)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} PluginService)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} OptionsService)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} FactoryService)

TARGET_LINK_LIBRARIES(${PROJECT_NAME} ToolUtils)

TARGET_LINK_LIBRARIES(${PROJECT_NAME} Win32UnicodeSystem)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} Win32ThreadSystem)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} Win32FileGroupPlugin)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} Win32Platform)

TARGET_LINK_LIBRARIES(${PROJECT_NAME} ImageCodecPlugin)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ZipPlugin)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} LZ4Plugin)

TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${THIRDPARTY_LIB_DIR}/${MENGINE_LIB_PREFIX}stdex${MENGINE_LIB_SUFFIX})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${THIRDPARTY_LIB_DIR}/${MENGINE_LIB_PREFIX}zlib${MENGINE_LIB_SUFFIX})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${THIRDPARTY_LIB_DIR}/${MENGINE_LIB_PREFIX}stdex${MENGINE_LIB_SUFFIX})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${THIRDPARTY_LIB_DIR}/${MENGINE_LIB_PREFIX}lz4${MENGINE_LIB_SUFFIX})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${THIRDPARTY_LIB_DIR}/${MENGINE_LIB_PREFIX}tinyini${MENGINE_LIB_SUFFIX})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${THIRDPARTY_LIB_DIR}/${MENGINE_LIB_PREFIX}math${MENGINE_LIB_SUFFIX})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} Shlwapi.lib)

foreach(dependency ${APPLICATION_DEPENDENCIES})
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} debug ${dependency})
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} optimized ${dependency})
endforeach(dependency ${APPLICATION_DEPENDENCIES})

TARGET_LINK_LIBRARIES(${PROJECT_NAME} comctl32)