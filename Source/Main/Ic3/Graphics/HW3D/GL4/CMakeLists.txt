
set( IC3_GRAPHICS_HW3D_GL4_SRC_FILES
    ${IC3_GRAPHICS_HW3D_GL_SRC_FILES}
    "GL4Prerequisites.h"
    "GL4GPUCmdCore.h"
    "GL4GPUDevice.h"
    "GL4GPUDevice.cpp"
    "GL4GPUDriver.h"
    "GL4GPUDriver.cpp"
    "GL4GPUDriverAPI.h"
    "GL4GPUDriverAPI.cpp"
)

ic3AddCoreLib( Ic3.Graphics.HW3D.GL4 ${IC3_GRAPHICS_HW3D_GL4_SRC_FILES} )

target_compile_definitions( Ic3.Graphics.HW3D.GL4 PRIVATE
        "GL_PLATFORM_TYPE_CORE=1")

target_include_directories( Ic3.Graphics.HW3D.GL4 PUBLIC
        ${IC3_GRAPHICS_HW3D_INCLUDE_DIR_INTERNAL} )

target_link_libraries( Ic3.Graphics.HW3D.GL4 PUBLIC
        Ic3.Graphics.GCI
        Ic3.System )
