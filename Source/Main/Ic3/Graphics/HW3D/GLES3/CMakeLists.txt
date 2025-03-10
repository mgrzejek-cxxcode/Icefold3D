
set( IC3_GRAPHICS_HW3D_GLES3_SRC_FILES
    ${IC3_GRAPHICS_HW3D_GL_SRC_FILES}

    "GLES3Prerequisites.h"
    "GLES3UCmdCore.h"
    "GLES3UDevice.h"
    "GLES3UDevice.cpp"
    "GLES3UDriver.h"
    "GLES3UDriver.cpp"
    "GLES3UDriverAPI.h"
    "GLES3UDriverAPI.cpp"

    PARENT_SCOPE
)

ic3AddCoreLib( Ic3.Graphics.HW3D.GLES3 ${IC3_GRAPHICS_HW3D_GLES3_SRC_FILES} )

target_compile_definitions( Ic3.Graphics.HW3D.gles3 PRIVATE
        "GL_PLATFORM_TYPE_CORE=0"
        "GL_PLATFORM_TYPE_ES=1" )

target_include_directories( Ic3.Graphics.HW3D.gles3 PUBLIC
        ${IC3_GRAPHICS_HW3D_INCLUDE_DIR_INTERNAL} )

target_link_libraries( Ic3.Graphics.HW3D.gles3 PUBLIC
        Ic3.Graphics.GCI
        Ic3.System )
