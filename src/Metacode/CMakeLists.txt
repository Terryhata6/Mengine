MENGINE_PROJECT(Metacode)

ADD_FILTER(
Metabuf
    Metacode.cpp
    Metacode.h
    Metatype.cpp
    Metatype.h
    Metacache.h
    protocol.xml
)
   
add_definitions(-DMETABUF_EXTERNAL_CONFIG)

INCLUDE_DIRECTORIES(${THIRDPARTY_DIR}/metabuf/include)

ADD_MENGINE_LIBRARY()

TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${THIRDPARTY_LIB_DIR}/${MENGINE_LIB_PREFIX}metabuf${MENGINE_LIB_SUFFIX})