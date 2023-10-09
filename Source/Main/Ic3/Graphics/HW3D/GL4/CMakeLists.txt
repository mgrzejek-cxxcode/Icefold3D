
set( IC3_GRAPHICS_HWAPI_GL4_SRC_FILES
        ${IC3_GRAPHICS_HWAPI_GL_SRC_FILES}
        "GL4Prerequisites.h"
        "GL4GPUCmdCore.h"
        "GL4GPUDevice.h"
        "GL4GPUDevice.cpp"
        "GL4GPUDriver.h"
        "GL4GPUDriver.cpp"
        "GL4GPUDriverAPI.h"
        "GL4GPUDriverAPI.cpp"
        )

message("${IC3_GRAPHICS_HWAPI_GL4_SRC_FILES}")

if( "${IC3_COMPONENTS_BUILD_MODE}" STREQUAL "STATIC" )
        add_library( Ic3.Graphics.HW3D.GL4 STATIC ${IC3_GRAPHICS_HWAPI_GL4_SRC_FILES} )
        target_compile_definitions( Ic3.Graphics.HW3D.GL4 PRIVATE
            "IC3_BUILD_STATIC=1")
else()
        add_library( Ic3.Graphics.HW3D.GL4 SHARED ${IC3_GRAPHICS_HWAPI_GL4_SRC_FILES} )
endif()

set_target_properties( Ic3.Graphics.HW3D.GL4 PROPERTIES
        OUTPUT_NAME "ts3dx-driver-gx-GL4" )

target_compile_definitions( Ic3.Graphics.HW3D.GL4 PRIVATE
        #"GLEW_STATIC=1"
        "IC3_GRAPHICS_HWAPI_GL_BUILD=1"
        "IC3_GRAPHICS_HWAPI_GL4_BUILD=1"
        "GL_PLATFORM_TYPE_CORE=1" )

target_include_directories( Ic3.Graphics.HW3D.GL4 PUBLIC
        ${IC3_DRIVERS_INCLUDE_DIR_INTERNAL} )

target_link_libraries( Ic3.Graphics.HW3D.GL4 PUBLIC
        Ic3.Graphics.GCI
        ts3-system )
