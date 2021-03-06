CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

SET(MENGINE_BUILD_TYPE_DEBUG OFF)
SET(MENGINE_BUILD_TYPE_RELWITHDEBINFO OFF)
SET(MENGINE_BUILD_TYPE_RELEASE OFF)

IF(CMAKE_BUILD_TYPE MATCHES "Debug")
    SET(MENGINE_BUILD_TYPE_DEBUG ON)
ELSEIF(CMAKE_BUILD_TYPE MATCHES "RelWithDebInfo")
    SET(MENGINE_BUILD_TYPE_RELWITHDEBINFO ON)
ELSEIF(CMAKE_BUILD_TYPE MATCHES "Release")
    SET(MENGINE_BUILD_TYPE_RELEASE ON)
ELSE()
    MESSAGE(FATAL_ERROR "Non support build type: ${CMAKE_BUILD_TYPE}")
ENDIF()

IF(MENGINE_BUILD_TYPE_DEBUG)
    SET(MENGINE_DEPENDENCIES_DEBUG_SUFFIX "d")
ELSE()
    SET(MENGINE_DEPENDENCIES_DEBUG_SUFFIX "")
ENDIF()

MACRO(SET_MENGINE_ENVIRONMENT MENGINE_TARGET MENGINE_RENDER MENGINE_PLATFORM MENGINE_TOOLCHAIN)
    SET(MENGINE_TARGET_WINDOWS OFF CACHE BOOL "MENGINE_TARGET_WINDOWS" FORCE)
    SET(MENGINE_TARGET_ANDROID OFF CACHE BOOL "MENGINE_TARGET_ANDROID" FORCE)
    SET(MENGINE_TARGET_MACOS OFF CACHE BOOL "MENGINE_TARGET_MACOS" FORCE)
    SET(MENGINE_TARGET_IOS OFF CACHE BOOL "MENGINE_TARGET_IOS" FORCE)
    SET(MENGINE_TARGET_UNIX OFF CACHE BOOL "MENGINE_TARGET_UNIX" FORCE)

    SET(MENGINE_RENDER_OPENGL OFF CACHE BOOL "MENGINE_RENDER_OPENGL" FORCE)
    SET(MENGINE_RENDER_DIRECTX OFF CACHE BOOL "MENGINE_RENDER_DIRECTX" FORCE)

    SET(MENGINE_PLATFORM_SDL OFF CACHE BOOL "MENGINE_PLATFORM_SDL" FORCE)
    SET(MENGINE_PLATFORM_WIN32 OFF CACHE BOOL "MENGINE_PLATFORM_WIN32" FORCE)

    SET(MENGINE_TOOLCHAIN_MSVC OFF CACHE BOOL "MENGINE_TOOLCHAIN_MSVC" FORCE)
    SET(MENGINE_TOOLCHAIN_MINGW OFF CACHE BOOL "MENGINE_TOOLCHAIN_MINGW" FORCE)
    SET(MENGINE_TOOLCHAIN_GRADLE OFF CACHE BOOL "MENGINE_TOOLCHAIN_GRADLE" FORCE)
    SET(MENGINE_TOOLCHAIN_XCODE OFF CACHE BOOL "MENGINE_TOOLCHAIN_XCODE" FORCE)
    SET(MENGINE_TOOLCHAIN_GCC OFF CACHE BOOL "MENGINE_TOOLCHAIN_GCC" FORCE)
    
    SET(MENGINE_TARGET_${MENGINE_TARGET} ON CACHE BOOL "MENGINE_TARGET" FORCE)
    SET(MENGINE_RENDER_${MENGINE_RENDER} ON CACHE BOOL "MENGINE_RENDER" FORCE)
    SET(MENGINE_PLATFORM_${MENGINE_PLATFORM} ON CACHE BOOL "MENGINE_PLATFORM" FORCE)
    SET(MENGINE_TOOLCHAIN_${MENGINE_TOOLCHAIN} ON CACHE BOOL "MENGINE_TOOLCHAIN" FORCE)
    
    ADD_DEFINITIONS(-DMENGINE_ENVIRONMENT_TARGET_${MENGINE_TARGET})
    ADD_DEFINITIONS(-DMENGINE_ENVIRONMENT_RENDER_${MENGINE_RENDER})
    ADD_DEFINITIONS(-DMENGINE_ENVIRONMENT_PLATFORM_${MENGINE_PLATFORM})
    ADD_DEFINITIONS(-DMENGINE_ENVIRONMENT_TOOLCHAIN_${MENGINE_TOOLCHAIN})
    
    if(MENGINE_ARCHITECTURE_X86)
        ADD_DEFINITIONS(-DMENGINE_ENVIRONMENT_ARCHITECTURE_X86)
    elseif(MENGINE_ARCHITECTURE_X64)
        ADD_DEFINITIONS(-DMENGINE_ENVIRONMENT_ARCHITECTURE_X64)
    else()
        message(FATAL_ERROR "Invalid select ARCHITECTURE")
    endif()    
        
    MESSAGE("*********************************************")
    MESSAGE("MENGINE_TARGET_WINDOWS: ${MENGINE_TARGET_WINDOWS}")
    MESSAGE("MENGINE_TARGET_ANDROID: ${MENGINE_TARGET_ANDROID}")
    MESSAGE("MENGINE_TARGET_MACOS: ${MENGINE_TARGET_MACOS}")
    MESSAGE("MENGINE_TARGET_IOS: ${MENGINE_TARGET_IOS}")
    MESSAGE("MENGINE_TARGET_UNIX: ${MENGINE_TARGET_UNIX}")
    MESSAGE("*********************************************")
    MESSAGE("MENGINE_RENDER_OPENGL: ${MENGINE_RENDER_OPENGL}")
    MESSAGE("MENGINE_RENDER_DIRECTX: ${MENGINE_RENDER_DIRECTX}")
    MESSAGE("*********************************************")
    MESSAGE("MENGINE_PLATFORM_SDL: ${MENGINE_PLATFORM_SDL}")
    MESSAGE("MENGINE_PLATFORM_WIN32: ${MENGINE_PLATFORM_WIN32}")
    MESSAGE("*********************************************")
    MESSAGE("MENGINE_TOOLCHAIN_MSVC: ${MENGINE_TOOLCHAIN_MSVC}")
    MESSAGE("MENGINE_TOOLCHAIN_MINGW: ${MENGINE_TOOLCHAIN_MINGW}")
    MESSAGE("MENGINE_TOOLCHAIN_GRADLE: ${MENGINE_TOOLCHAIN_GRADLE}")
    MESSAGE("MENGINE_TOOLCHAIN_XCODE: ${MENGINE_TOOLCHAIN_XCODE}")
    MESSAGE("MENGINE_TOOLCHAIN_GCC: ${MENGINE_TOOLCHAIN_GCC}")
    MESSAGE("*********************************************")
    MESSAGE("MENGINE_PROJECT_NAME: ${MENGINE_PROJECT_NAME}")
    MESSAGE("MENGINE_BUILD_ONLY_EXECUTABLE_MODULE: ${MENGINE_BUILD_ONLY_EXECUTABLE_MODULE}")
    MESSAGE("MENGINE_BUILD_MENGINE_MASTER_RELEASE: ${MENGINE_BUILD_MENGINE_MASTER_RELEASE}")
    MESSAGE("MENGINE_BUILD_MENGINE_BUILD_PUBLISH: ${MENGINE_BUILD_MENGINE_BUILD_PUBLISH}")
    MESSAGE("MENGINE_BUILD_MENGINE_DEVELOPMENT: ${MENGINE_BUILD_MENGINE_DEVELOPMENT}")
    MESSAGE("MENGINE_USE_PRECOMPILED_HEADER: ${MENGINE_USE_PRECOMPILED_HEADER}")
    MESSAGE("MENGINE_USE_SCRIPT_SERVICE: ${MENGINE_USE_SCRIPT_SERVICE}")
    MESSAGE("MENGINE_LOGGER_DEBUG: ${MENGINE_LOGGER_DEBUG}")
    MESSAGE("MENGINE_SECURE_VALUE: ${MENGINE_SECURE_VALUE}")
    MESSAGE("MENGINE_EXTERNAL_PDB: ${MENGINE_EXTERNAL_PDB}")
    MESSAGE("MENGINE_EXTERNAL_PDB_PATH: ${MENGINE_EXTERNAL_PDB_PATH}")
    MESSAGE("MENGINE_EXTERNAL_SOURCE: ${MENGINE_EXTERNAL_SOURCE}")
    MESSAGE("MENGINE_EXTERNAL_SOURCE_PATH: ${MENGINE_EXTERNAL_SOURCE_PATH}")
    MESSAGE("MENGINE_EXTERNAL_TEMP_PATH: ${MENGINE_EXTERNAL_TEMP_PATH}")
    MESSAGE("MENGINE_USE_SUBFOLDER: ${MENGINE_USE_SUBFOLDER}")
    MESSAGE("MENGINE_SUBFOLDER_NAME: ${MENGINE_SUBFOLDER_NAME}")
    MESSAGE("MENGINE_INSTALL_PATH: ${MENGINE_INSTALL_PATH}")
    MESSAGE("*********************************************")
ENDMACRO()

MACRO(SET_MENGINE_OUTPUT_DIRECTORY)
    get_directory_property( DirDefs COMPILE_DEFINITIONS )
    foreach( d ${DirDefs} )
        LIST(APPEND MENGINE_DEFINITIONS_SHA1_LIST "${d}: [${${d}}]")
    endforeach()
    
    STRING(SHA1 MENGINE_DEFINITIONS_SHA1 "${MENGINE_DEFINITIONS_SHA1_LIST}")

    if(UNIX AND NOT ANDROID AND NOT APPLE AND NOT IOS)
        # output paths
        SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${MENGINE_REPOSITORY}/lib/${PROJECT_NAME}/${CMAKE_GENERATOR}/${MENGINE_DEFINITIONS_SHA1}/${CMAKE_BUILD_TYPE})
        SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${MENGINE_INSTALL_PATH}/${PROJECT_NAME}/${CMAKE_GENERATOR}/${CMAKE_BUILD_TYPE})
    elseif(ANDROID)
        # output paths
        SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${MENGINE_REPOSITORY}/lib/${PROJECT_NAME}/${CMAKE_GENERATOR}/${MENGINE_DEFINITIONS_SHA1}/${ANDROID_ABI}/${CMAKE_BUILD_TYPE})
        SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${MENGINE_INSTALL_PATH}/${PROJECT_NAME}/${CMAKE_GENERATOR}/${ANDROID_ABI}/${CMAKE_BUILD_TYPE})
    elseif(MINGW)
        # output paths
        SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${MENGINE_REPOSITORY}/lib/${PROJECT_NAME}/${CMAKE_GENERATOR}/${MENGINE_DEFINITIONS_SHA1}/${CMAKE_BUILD_TYPE})
        SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${MENGINE_INSTALL_PATH}/${PROJECT_NAME}/${CMAKE_GENERATOR}/${CMAKE_BUILD_TYPE})
    elseif(APPLE)
        # output paths
        if(MENGINE_TARGET_MACOS)
            SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${MENGINE_REPOSITORY}/lib/${PROJECT_NAME}/${CMAKE_GENERATOR}/${MENGINE_DEFINITIONS_SHA1}/${CMAKE_BUILD_TYPE})
            SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${MENGINE_INSTALL_PATH}/${PROJECT_NAME}/${CMAKE_GENERATOR})
        elseif(MENGINE_TARGET_IOS)
            SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${MENGINE_REPOSITORY}/lib/${PROJECT_NAME}/${CMAKE_GENERATOR}/${MENGINE_DEFINITIONS_SHA1})
            SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${MENGINE_INSTALL_PATH}/${PROJECT_NAME}/${CMAKE_GENERATOR})
        else()
            MESSAGE(FATAL_ERROR "unsupported target") 
        endif()
    else()
        # output paths
        if(CMAKE_GENERATOR_PLATFORM)
            SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${MENGINE_REPOSITORY}/lib/${PROJECT_NAME}/${CMAKE_GENERATOR} ${CMAKE_GENERATOR_PLATFORM}/${MENGINE_DEFINITIONS_SHA1}")
            SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${MENGINE_INSTALL_PATH}/${PROJECT_NAME}/${CMAKE_GENERATOR} ${CMAKE_GENERATOR_PLATFORM}")
        else()
            SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${MENGINE_REPOSITORY}/lib/${PROJECT_NAME}/${CMAKE_GENERATOR}/${MENGINE_DEFINITIONS_SHA1}")
            SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${MENGINE_INSTALL_PATH}/${PROJECT_NAME}/${CMAKE_GENERATOR}")
        endif()
    endif()

    MESSAGE("CMAKE_ARCHIVE_OUTPUT_DIRECTORY: ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")
    MESSAGE("CMAKE_RUNTIME_OUTPUT_DIRECTORY: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
    
    if(MENGINE_EXTERNAL_PDB)
        MESSAGE("MENGINE_SHARED_PDB_OUTPUT_DIRECTORY: ${MENGINE_SHARED_PDB_OUTPUT_DIRECTORY}")
    endif()
ENDMACRO()

MACRO(MENGINE_PROJECT name)
    PROJECT(${name})
    SET(SRC_FILES)
ENDMACRO()

MACRO(ADD_FILTER group_name)
    SOURCE_GROUP(${group_name} FILES ${ARGN})
    SET(SRC_FILES ${SRC_FILES} ${ARGN})
ENDMACRO()

# solution
MACRO(ADD_MENGINE_FRAMEWORK)
    IF(MENGINE_EXTERNAL_SOURCE)
        ADD_PLUGIN(MENGINE_PLUGIN_EXTERNALBOOTSTRAPPER ON OFF "MENGINE_PLUGIN_EXTERNALBOOTSTRAPPER")
        
        add_definitions(-DMENGINE_EXTERNAL_SOURCE)
    ENDIF()
ENDMACRO()

MACRO(ADD_PLUGIN Plugin Toggle DLL MSG)
    OPTION(${Plugin}_FORCE_ENABLE ${MSG} OFF)
    OPTION(${Plugin}_FORCE_DISABLE ${MSG} OFF)
    
    IF(${Plugin}_FORCE_ENABLE)
        SET(${Plugin} ON CACHE BOOL ${MSG} FORCE)
    ELSEIF(${Plugin}_FORCE_DISABLE)
        SET(${Plugin} OFF CACHE BOOL ${MSG} FORCE)
    ELSE()
        SET(${Plugin} ${Toggle} CACHE BOOL ${MSG} FORCE)
    ENDIF()
    
    IF(${Plugin})
        SET(${Plugin}_DLL ${DLL} CACHE BOOL ${MSG} FORCE)
        
        IF(${Plugin}_DLL)
            SET(${Plugin}_STATIC OFF CACHE BOOL ${MSG} FORCE)
        ELSE()
            SET(${Plugin}_STATIC ON CACHE BOOL ${MSG} FORCE)
        ENDIF()
        
        add_definitions(-D${Plugin})
    ELSE()
        SET(${Plugin}_DLL OFF CACHE BOOL ${MSG} FORCE)
        SET(${Plugin}_STATIC OFF CACHE BOOL ${MSG} FORCE)
    ENDIF()
    
    IF(${Plugin})
        IF(${Plugin}_DLL)
            add_definitions(-D${Plugin}_DLL)
        ENDIF()
        
        IF(${Plugin}_STATIC)
            add_definitions(-D${Plugin}_STATIC)
        ENDIF()
    ENDIF()
    
    MESSAGE("PLUGIN: ${Plugin} = ${${Plugin}} [${${Plugin}_DLL}]")
ENDMACRO()

MACRO(ADD_PLUGIN_IF Plugin Toggle DLL MSG DIRECTORY)
    OPTION(${Plugin}_FORCE_ENABLE ${MSG} OFF)
    OPTION(${Plugin}_FORCE_DISABLE ${MSG} OFF)
    
    IF(${Plugin}_FORCE_ENABLE)
        SET(${Plugin} ON CACHE BOOL ${MSG} FORCE)
    ELSEIF(${Plugin}_FORCE_DISABLE)
        SET(${Plugin} OFF CACHE BOOL ${MSG} FORCE)
    ELSE()
        IF(EXISTS ${DIRECTORY})
            SET(${Plugin} ${Toggle} CACHE BOOL ${MSG} FORCE)
        ELSE()
            SET(${Plugin} OFF CACHE BOOL ${MSG} FORCE)
        ENDIF()
    ENDIF()
    
    IF(${Plugin})
        SET(${Plugin}_DLL ${DLL} CACHE BOOL ${MSG} FORCE)
        
        IF(${Plugin}_DLL)
            SET(${Plugin}_STATIC OFF CACHE BOOL ${MSG} FORCE)
        ELSE()
            SET(${Plugin}_STATIC ON CACHE BOOL ${MSG} FORCE)
        ENDIF()
        
        add_definitions(-D${Plugin})
    ENDIF()
    
    IF(${Plugin})
        IF(${Plugin}_DLL)
            add_definitions(-D${Plugin}_DLL)
        ELSE()
            add_definitions(-D${Plugin}_STATIC)
        ENDIF()
    ENDIF()
    
    MESSAGE("PLUGIN: ${Plugin} = ${${Plugin}} [${${Plugin}_DLL}]")
ENDMACRO()

MACRO(CREATE_PRECOMPILED_HEADER)
  IF(MSVC AND MENGINE_USE_PRECOMPILED_HEADER)
	SET(PrecompiledHeader "${MENGINE_SOURCE_DIR}/PrecompiledHeader/PrecompiledHeader.h")
	SET(PrecompiledSource "${MENGINE_SOURCE_DIR}/PrecompiledHeader/PrecompiledHeader.cpp")
    SET(PrecompiledBinary "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE}/PrecompiledHeader.pch")

    SET_SOURCE_FILES_PROPERTIES(${PrecompiledSource}
                                PROPERTIES COMPILE_FLAGS "/Yc\"${PrecompiledHeader}\" /FI\"${PrecompiledHeader}\" /Fp\"${PrecompiledBinary}\""
                                           OBJECT_OUTPUTS "${PrecompiledBinary}")
  ENDIF()
ENDMACRO()

MACRO(ADD_PRECOMPILED_HEADER)
  IF(MSVC AND MENGINE_USE_PRECOMPILED_HEADER)
	SET(PrecompiledHeader "${MENGINE_SOURCE_DIR}/PrecompiledHeader/PrecompiledHeader.h")
    SET(PrecompiledBinary "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE}/PrecompiledHeader.pch")

    SET_SOURCE_FILES_PROPERTIES(${SRC_FILES}
                                PROPERTIES COMPILE_FLAGS "/Yu\"${PrecompiledHeader}\" /FI\"${PrecompiledHeader}\" /Fp\"${PrecompiledBinary}\""
                                           )
  ENDIF()
ENDMACRO()

MACRO(ADD_MENGINE_INTERFACE)
    ADD_LIBRARY(${PROJECT_NAME} INTERFACE)
    
    add_custom_target(${PROJECT_NAME}.headers SOURCES ${SRC_FILES})
    
    set(extra_macro_args ${ARGN})
    list(LENGTH extra_macro_args num_extra_args)
    
    set(USE_SUBFOLDER ON)
    if(${num_extra_args} EQUAL 1)
        list(GET extra_macro_args 0 FILTER_FOLDER)
    elseif(${num_extra_args} EQUAL 2)
        list(GET extra_macro_args 0 FILTER_FOLDER)
        list(GET extra_macro_args 1 USE_SUBFOLDER)
    endif()

    if(MENGINE_USE_SUBFOLDER AND USE_SUBFOLDER)
        if(${num_extra_args} GREATER 0)
            set_target_properties (${PROJECT_NAME}.headers PROPERTIES
                FOLDER ${MENGINE_SUBFOLDER_NAME}/${FILTER_FOLDER}
            )
        else()
            set_target_properties (${PROJECT_NAME}.headers PROPERTIES
                FOLDER ${MENGINE_SUBFOLDER_NAME}
            )
        endif()
    else()
        if(${num_extra_args} GREATER 0)
            set_target_properties (${PROJECT_NAME}.headers PROPERTIES
                FOLDER ${FILTER_FOLDER}
            )
        endif()
    endif()
ENDMACRO()

MACRO(ADD_MENGINE_LIBRARY)
	ADD_PRECOMPILED_HEADER()
    
	ADD_LIBRARY(${PROJECT_NAME} STATIC ${SRC_FILES})
    
    if(MSVC)
        target_sources(${PROJECT_NAME} INTERFACE ${MENGINE_REPOSITORY}/contributing/codestyle/mengine.natvis)
    endif()
    
    IF(MSVC AND MENGINE_USE_PRECOMPILED_HEADER)
        ADD_DEPENDENCIES(${PROJECT_NAME} PrecompiledHeader)
        TARGET_LINK_LIBRARIES(${PROJECT_NAME} PrecompiledHeader)
    ENDIF()
    
    set(extra_macro_args ${ARGN})
    list(LENGTH extra_macro_args num_extra_args)
    
    set(USE_SUBFOLDER ON)
    if(${num_extra_args} EQUAL 1)
        list(GET extra_macro_args 0 FILTER_FOLDER)
    elseif(${num_extra_args} EQUAL 2)
        list(GET extra_macro_args 0 FILTER_FOLDER)
        list(GET extra_macro_args 1 USE_SUBFOLDER)
    endif()

    if(MENGINE_USE_SUBFOLDER AND USE_SUBFOLDER)
        if(${num_extra_args} GREATER 0)
            set_target_properties (${PROJECT_NAME} PROPERTIES
                FOLDER ${MENGINE_SUBFOLDER_NAME}/${FILTER_FOLDER}
            )
        else()
            set_target_properties (${PROJECT_NAME} PROPERTIES
                FOLDER ${MENGINE_SUBFOLDER_NAME}
            )
        endif()
    else()
        if(${num_extra_args} GREATER 0)
            set_target_properties (${PROJECT_NAME} PROPERTIES
                FOLDER ${FILTER_FOLDER}
            )
        endif()
    endif()
    
    SET(APPLICATION_LINK_LIBRARIES ${APPLICATION_LINK_LIBRARIES} ${PROJECT_NAME} PARENT_SCOPE)
ENDMACRO()

MACRO(ADD_MENGINE_SHARED)
	ADD_PRECOMPILED_HEADER()
    
	ADD_LIBRARY(${PROJECT_NAME} SHARED ${SRC_FILES})
    
    if(MSVC)
        target_sources(${PROJECT_NAME} INTERFACE ${MENGINE_REPOSITORY}/contributing/codestyle/mengine.natvis)
    endif()
    
    if(MENGINE_EXTERNAL_PDB)
        if(MSVC)
            SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES 
                COMPILE_PDB_NAME ${PROJECT_NAME} 
                PDB_OUTPUT_DIRECTORY ${MENGINE_EXTERNAL_PDB_PATH}
            )
        elseif(MINGW)
            target_compile_options(${PROJECT_NAME} PRIVATE -gcodeview)
            target_link_options(${PROJECT_NAME} PRIVATE -Wl,-pdb=${MENGINE_EXTERNAL_PDB_PATH}/${PROJECT_NAME}.pdb)
        endif()
    else()
        if(MENGINE_BUILD_TYPE_DEBUG)
            if(MINGW)
                target_compile_options(${PROJECT_NAME} PRIVATE -gcodeview)
                target_link_options(${PROJECT_NAME} PRIVATE -Wl,-pdb=)
            endif()
        endif()
    endif()
    
	
	SET_TARGET_PROPERTIES( ${PROJECT_NAME} 
		PROPERTIES PREFIX ""
		)
	
    IF(MSVC AND MENGINE_USE_PRECOMPILED_HEADER)
        ADD_DEPENDENCIES(${PROJECT_NAME} PrecompiledHeader)
        TARGET_LINK_LIBRARIES(${PROJECT_NAME} PrecompiledHeader)
    ENDIF()
    
    set(extra_macro_args ${ARGN})
    list(LENGTH extra_macro_args num_extra_args)
    
    set(USE_SUBFOLDER ON)
    if(${num_extra_args} EQUAL 1)
        list(GET extra_macro_args 0 FILTER_FOLDER)
    elseif(${num_extra_args} EQUAL 2)
        list(GET extra_macro_args 0 FILTER_FOLDER)
        list(GET extra_macro_args 1 USE_SUBFOLDER)
    endif()
    
    if(MENGINE_USE_SUBFOLDER AND USE_SUBFOLDER)
        if(${num_extra_args} GREATER 0)
            set_target_properties (${PROJECT_NAME} PROPERTIES
                FOLDER ${MENGINE_SUBFOLDER_NAME}/${FILTER_FOLDER}
            )
        else()
            set_target_properties (${PROJECT_NAME} PROPERTIES
                    FOLDER ${MENGINE_SUBFOLDER_NAME}
                )
        endif()
    else()
        if(${num_extra_args} GREATER 0)
            set_target_properties (${PROJECT_NAME} PROPERTIES
                FOLDER ${FILTER_FOLDER}
            )
        endif()
    endif()
    
    SET(APPLICATION_DEPENDENCIES ${APPLICATION_DEPENDENCIES} ${PROJECT_NAME} PARENT_SCOPE)
ENDMACRO()

MACRO(ADD_MENGINE_EXECUTABLE)
    ADD_EXECUTABLE(${PROJECT_NAME} WIN32 ${SRC_FILES})
    
    if(MSVC)
        target_sources(${PROJECT_NAME} INTERFACE ${MENGINE_REPOSITORY}/contributing/codestyle/mengine.natvis)
    endif()
    
    if(MENGINE_EXTERNAL_PDB)
        if(MSVC)
            set_target_properties(${PROJECT_NAME} PROPERTIES
                COMPILE_PDB_NAME ${PROJECT_NAME}
                PDB_OUTPUT_DIRECTORY ${MENGINE_EXTERNAL_PDB_PATH}
            )
        elseif(MINGW)
            target_compile_options(${PROJECT_NAME} PRIVATE -gcodeview)
            target_link_options(${PROJECT_NAME} PRIVATE -Wl,-pdb=${MENGINE_EXTERNAL_PDB_PATH}/${PROJECT_NAME}.pdb)
        endif()
    else()
        if(MENGINE_BUILD_TYPE_DEBUG)
            if(MINGW)
                target_compile_options(${PROJECT_NAME} PRIVATE -gcodeview)
                target_link_options(${PROJECT_NAME} PRIVATE -Wl,-pdb=)
            endif()
        endif()
    endif()
    
    IF(MSVC AND MENGINE_USE_PRECOMPILED_HEADER)
        ADD_DEPENDENCIES(${PROJECT_NAME} PrecompiledHeader)
        TARGET_LINK_LIBRARIES(${PROJECT_NAME} PrecompiledHeader)
    ENDIF()
ENDMACRO()

MACRO(ADD_MENGINE_PLUGIN PLUGIN_NAME)
    IF(${PLUGIN_NAME}_DLL)
        add_definitions(-DMENGINE_PLUGIN_DLL)
        ADD_MENGINE_SHARED()
    ELSE()
        ADD_MENGINE_LIBRARY()
    ENDIF()
    
    set(extra_macro_args ${ARGN})
    list(LENGTH extra_macro_args num_extra_args)
    
    set(USE_SUBFOLDER ON)
    if(${num_extra_args} EQUAL 1)
        list(GET extra_macro_args 0 FILTER_FOLDER)
    elseif(${num_extra_args} EQUAL 2)
        list(GET extra_macro_args 0 FILTER_FOLDER)
        list(GET extra_macro_args 1 USE_SUBFOLDER)
    endif()
    
    if(MENGINE_USE_SUBFOLDER AND ${USE_SUBFOLDER})
        if(${num_extra_args} GREATER 0)
            set_target_properties (${PROJECT_NAME} PROPERTIES
                FOLDER ${MENGINE_SUBFOLDER_NAME}/${FILTER_FOLDER}
            )
        else()
            set_target_properties (${PROJECT_NAME} PROPERTIES
                FOLDER ${MENGINE_SUBFOLDER_NAME}/Plugins
            )
        endif()
    else()
        if(${num_extra_args} GREATER 0)
            set_target_properties (${PROJECT_NAME} PROPERTIES
                FOLDER ${FILTER_FOLDER}
            )
        else()
            set_target_properties (${PROJECT_NAME} PROPERTIES
                FOLDER Plugins
            )
        endif()
    endif()
ENDMACRO()

if(APPLE)
    MESSAGE("CMAKE_OSX_SYSROOT: ${CMAKE_OSX_SYSROOT}")
    MESSAGE("CMAKE_OSX_DEPLOYMENT_TARGET: ${CMAKE_OSX_DEPLOYMENT_TARGET}")

    macro(set_xcode_property TARGET XCODE_PROPERTY XCODE_VALUE)
        set_property(TARGET ${TARGET} PROPERTY XCODE_ATTRIBUTE_${XCODE_PROPERTY} ${XCODE_VALUE})
    endmacro()

    macro(ADD_APPLE_FRAMEWORK fwname)
        find_library(FRAMEWORK_${fwname} ${fwname})
    
        if( ${FRAMEWORK_${fwname}} STREQUAL FRAMEWORK_${fwname}-NOTFOUND)
            MESSAGE(ERROR ": Framework ${fwname} not found")
        else()
            target_link_libraries(${PROJECT_NAME} "-framework ${fwname}")
            MESSAGE(STATUS "Framework ${fwname} found at ${FRAMEWORK_${fwname}}")
        endif()
    endmacro()
endif()