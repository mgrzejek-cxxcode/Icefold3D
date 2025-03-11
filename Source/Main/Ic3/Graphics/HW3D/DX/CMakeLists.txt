
set( IC3_GRAPHICS_HW3D_DX_SRC_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/Prerequisites/DXCommonShaderDefs.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Prerequisites/DXErrorUtils.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Prerequisites/DXErrorUtils.cpp"

    "${CMAKE_CURRENT_SOURCE_DIR}/DXPrerequisites.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/DXPrerequisites.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/DXAPITranslationLayer.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/DXAPITranslationLayer.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/DXGPUDevice.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/DXGPUDevice.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/DXGPUDriver.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/DXGPUDriver.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/DXPresentationLayer.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/DXPresentationLayer.cpp"

    PARENT_SCOPE
)
