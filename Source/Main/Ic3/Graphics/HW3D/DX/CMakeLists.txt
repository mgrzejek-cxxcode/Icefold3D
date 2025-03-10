
set( IC3_GRAPHICS_HW3D_DX_SRC_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/prerequisites/DX_commonShaderDefs.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/prerequisites/DX_errorUtils.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/prerequisites/DX_errorUtils.cpp"

    "${CMAKE_CURRENT_SOURCE_DIR}/DXPrerequisites.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/DXPrerequisites.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/DX_APITranslationLayer.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/DXAPITranslationLayer.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/DXUDevice.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/DXUDevice.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/DXUDriver.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/DXUDriver.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/DX_presentationLayer.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/DX_presentationLayer.cpp"

    PARENT_SCOPE
)
