
set( IC3_GRAPHICS_HWAPI_GLES3_SRC_FILES
        ${IC3_GRAPHICS_HWAPI_GL_SRC_FILES}

        "GLES3Prerequisites.h"
        "GLES3UCmdCore.h"
        "GLES3UDevice.h"
        "GLES3UDevice.cpp"
        "GLES3UDriver.h"
        "GLES3UDriver.cpp"
        "GLES3UDriverAPI.h"
        "GLES3UDriverAPI.cpp"
        )

if( "${IC3_COMPONENTS_BUILD_MODE}" STREQUAL "STATIC" )
    add_library( Ic3.Graphics.HW3D.gles3 STATIC ${IC3_GRAPHICS_HWAPI_GLES3_SRC_FILES} )
    target_compile_definitions( Ic3.Graphics.HW3D.gles3 PRIVATE
            "IC3_BUILD_STATIC=1")
else()
    add_library( Ic3.Graphics.HW3D.gles3 SHARED ${IC3_GRAPHICS_HWAPI_GLES3_SRC_FILES} )
endif()

set_target_properties( Ic3.Graphics.HW3D.gles3 PROPERTIES
        OUTPUT_NAME "Ic3.Graphics.HW3D.gles3" )

target_compile_definitions( Ic3.Graphics.HW3D.gles3 PRIVATE
        #"GLEW_STATIC=1"
        "IC3_GRAPHICS_HWAPI_GL_BUILD=1"
        "IC3_GRAPHICS_HWAPI_GLES3_BUILD=1"
        "GL_PLATFORM_TYPE_CORE=0"
        "GL_PLATFORM_TYPE_ES=1" )

target_include_directories( Ic3.Graphics.HW3D.gles3 PUBLIC
        ${IC3_DRIVERS_INCLUDE_DIR_INTERNAL} )

target_link_libraries( Ic3.Graphics.HW3D.gles3 PUBLIC
        Ic3.Graphics.GCI
        Ic3.System )
