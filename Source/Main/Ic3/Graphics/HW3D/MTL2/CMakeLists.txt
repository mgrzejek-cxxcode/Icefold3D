
set( IC3_GRAPHICS_HWAPI_MTL2_SRC_FILES
        ${IC3_GRAPHICS_HWAPI_MTLCOMMON_SRC_FILES}

        "MTL2Prerequisites.h"
        "MTL2GPUDevice.h"
        "MTL2GPUDevice.mm"
        "MTL2GPUDriver.h"
        "MTL2GPUDriver.mm"
        "MTL2GPUDriverAPI.h"
        "MTL2GPUDriverAPI.mm"
        )

if( "${IC3_COMPONENTS_BUILD_MODE}" STREQUAL "STATIC" )
    add_library( Ic3.Graphics.HW3D.MTL2 STATIC ${IC3_GRAPHICS_HWAPI_MTL2_SRC_FILES} )
    target_compile_definitions( Ic3.Graphics.HW3D.MTL2 PRIVATE
            "IC3_BUILD_STATIC=1")
else()
    add_library( Ic3.Graphics.HW3D.MTL2 SHARED ${IC3_GRAPHICS_HWAPI_MTL2_SRC_FILES} )
endif()

set_target_properties( Ic3.Graphics.HW3D.MTL2 PROPERTIES
        OUTPUT_NAME "Ic3.Graphics.HW3D.MTL2" )

target_compile_definitions( Ic3.Graphics.HW3D.MTL2 PRIVATE
        "IC3_GRAPHICS_HWAPI_MTL_BUILD=1"
        "IC3_GRAPHICS_HWAPI_MTL2_BUILD=1" )

target_include_directories( Ic3.Graphics.HW3D.MTL2 PUBLIC
        ${IC3_DRIVERS_INCLUDE_DIR_INTERNAL} )

target_link_libraries( Ic3.Graphics.HW3D.MTL2 PUBLIC
        Ic3.Graphics.GCI
        Ic3.System )
