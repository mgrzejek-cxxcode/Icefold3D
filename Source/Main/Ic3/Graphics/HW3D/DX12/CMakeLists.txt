
set( IC3_GRAPHICS_HWAPI_DX12_SRC_FILES
        ${IC3_GRAPHICS_HWAPI_DX_SRC_FILES}

        "Prerequisites/DX12_coreDefs.h"

        "DX12Prerequisites.h"
        "DX12Prerequisites.cpp"
		
        "DX12_coreAPIProxy.h"
        "DX12_coreAPIProxy.cpp"
        "DX12UCmdContext.h"
        "DX12UCmdContext.cpp"
        "DX12UCmdManager.h"
        "DX12UCmdManager.cpp"
        "DX12UDevice.h"
        "DX12UDevice.cpp"
        "DX12UDriver.h"
        "DX12UDriver.cpp"
        "DX12UDriverAPI.h"
        "DX12UDriverAPI.cpp"
        "DX12_presentationLayer.h"
        "DX12_presentationLayer.cpp"

        )


if( ${CM_EBS_SYSCONF_TARGET_FLAG_OSINFO_WFA} )

    if( "${IC3_COMPONENTS_BUILD_MODE}" STREQUAL "STATIC" )
        add_library( Ic3.Graphics.HW3D.DX12 STATIC ${IC3_GRAPHICS_HWAPI_DX12_SRC_FILES} )
        target_compile_definitions( Ic3.Graphics.HW3D.DX12 PRIVATE
                "IC3_BUILD_STATIC=1")
    else()
        add_library( Ic3.Graphics.HW3D.DX12 SHARED ${IC3_GRAPHICS_HWAPI_DX12_SRC_FILES} )
    endif()

    set_target_properties( Ic3.Graphics.HW3D.DX12 PROPERTIES
            OUTPUT_NAME "ts3dx-driver-gpuapi-dx12" )

    target_compile_definitions( Ic3.Graphics.HW3D.DX12 PRIVATE
            "IC3_GRAPHICS_HWAPI_DX_BUILD=1"
            "IC3_GRAPHICS_HWAPI_DX12_BUILD=1")

    target_include_directories( Ic3.Graphics.HW3D.DX12 PUBLIC
            ${IC3_GRAPHICS_HW3D_INCLUDE_DIR_INTERNAL} )

    target_link_libraries( Ic3.Graphics.HW3D.DX12 PUBLIC
            Ic3.Graphics.GCI
            D3D12.lib )

endif()
