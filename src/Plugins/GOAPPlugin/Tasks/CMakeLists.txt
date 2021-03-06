MENGINE_PROJECT(GOAPCook)

ADD_FILTER(
src   
    GOAPCook.h
    GOAPCook.cpp
)

ADD_FILTER(
Tasks
    TaskEnum.h
    TaskNodeEnable.h
    TaskNodeEnable.cpp
    TaskNodeDisable.h
    TaskNodeDisable.cpp
    TaskNodeDestroy.h
    TaskNodeDestroy.cpp
    TaskDelay.cpp
    TaskDelay.h
    TaskLocalDelay.h
    TaskLocalDelay.cpp
    TaskGlobalDelay.h
    TaskGlobalDelay.cpp
    TaskPrint.cpp
    TaskPrint.h
    TaskReleasePrint.cpp
    TaskReleasePrint.h
    TaskPickerableMouseButton.cpp
    TaskPickerableMouseButton.h
    TaskPickerableMouseEnter.cpp
    TaskPickerableMouseEnter.h
    TaskPickerableMouseLeave.cpp
    TaskPickerableMouseLeave.h
    TaskGlobalMouseButton.cpp
    TaskGlobalMouseButton.h
    TaskGlobalMouseMove.cpp
    TaskGlobalMouseMove.h
    TaskGlobalMouseWheel.cpp
    TaskGlobalMouseWheel.h
    TaskGlobalKeyPress.h
    TaskGlobalKeyPress.cpp
    TransformationRotateMode.h
    AffectorTransformationRotate.h
    AffectorTransformationRotate.cpp
    AffectorTransformationRotateTime.h
    AffectorTransformationRotateTime.cpp
    TaskTransformationRotateX.cpp
    TaskTransformationRotateX.h
    TaskTransformationRotateY.cpp
    TaskTransformationRotateY.h
    TaskTransformationRotateZ.cpp
    TaskTransformationRotateZ.h
    TaskTransformationRotateXTime.cpp
    TaskTransformationRotateXTime.h
    TaskTransformationRotateYTime.cpp
    TaskTransformationRotateYTime.h
    TaskTransformationRotateZTime.cpp
    TaskTransformationRotateZTime.h
    TaskTransformationScaleTime.cpp
    TaskTransformationScaleTime.h
    TaskTransformationTranslate.cpp
    TaskTransformationTranslate.h
    TaskTransformationTranslateTime.h
    TaskTransformationTranslateTime.cpp
    TaskTransformationTranslateTimeWithSkip.h
    TaskTransformationTranslateTimeWithSkip.cpp
    TaskColorableAlphaTime.h
    TaskColorableAlphaTime.cpp
    TaskAnimatablePlay.cpp
    TaskAnimatablePlay.h
    TaskAnimatablePlayWait.cpp
    TaskAnimatablePlayWait.h
    TaskAnimatableStop.cpp
    TaskAnimatableStop.h
    TaskAnimatablePause.cpp
    TaskAnimatablePause.h
    TaskAnimatableResume.cpp
    TaskAnimatableResume.h
    TaskAnimatablePlayReceiver.h
    TaskAnimatablePlayReceiver.cpp
    TaskAnimatableRewind.h
    TaskAnimatableRewind.cpp
    TaskNotify.h
    TaskEventable.h
    TaskEventable.cpp
)

INCLUDE_DIRECTORIES(${MENGINE_CMAKE_DEPENDENCIES_DIR}/GOAP/Config)
INCLUDE_DIRECTORIES(${THIRDPARTY_DIR}/GOAP/include)

ADD_MENGINE_LIBRARY(Plugins)

TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${THIRDPARTY_LIB_DIR}/${MENGINE_LIB_PREFIX}stdex${MENGINE_LIB_SUFFIX})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${THIRDPARTY_LIB_DIR}/${MENGINE_LIB_PREFIX}GOAP${MENGINE_LIB_SUFFIX})

TARGET_LINK_LIBRARIES(${PROJECT_NAME} Kernel)