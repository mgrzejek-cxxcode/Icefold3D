
set( IC3_GRAPHICS_HWAPI_VK10_SRC_FILES
        ${IC3_GRAPHICS_HWAPI_VKCOMMON_SRC_FILES}

        "VK10Prerequisites.h"
        "VK10Prerequisites.cpp"

        "core/VK10UDriverAPI.h"
        "core/VK10UDriverAPI.cpp"
        )

if( "${IC3_COMPONENTS_BUILD_MODE}" STREQUAL "STATIC" )
    add_library( Ic3.Graphics.HW3D.VK13 STATIC ${IC3_GRAPHICS_HWAPI_VK10_SRC_FILES} )
    target_compile_definitions( Ic3.Graphics.HW3D.VK13 PRIVATE
            "IC3_BUILD_STATIC=1")
else()
    add_library( Ic3.Graphics.HW3D.VK13 SHARED ${IC3_GRAPHICS_HWAPI_VK10_SRC_FILES} )
endif()

set_target_properties( Ic3.Graphics.HW3D.VK13 PROPERTIES
        OUTPUT_NAME "ts3dx-driver-gpuapi-VK13" )

target_compile_definitions( Ic3.Graphics.HW3D.VK13 PRIVATE
        "IC3_GRAPHICS_HWAPI_VKCOMMON_BUILD=1"
        "IC3_GRAPHICS_HWAPI_VK10_BUILD=1")

target_include_directories( Ic3.Graphics.HW3D.VK13 PUBLIC
        ${IC3_GRAPHICS_HW3D_INCLUDE_DIR_INTERNAL} )

target_link_libraries( Ic3.Graphics.HW3D.VK13 PUBLIC
        Ic3.Graphics.GCI )
