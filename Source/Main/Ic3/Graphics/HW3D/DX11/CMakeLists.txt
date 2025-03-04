
set( IC3_GRAPHICS_HWAPI_DX11_SRC_FILES
        "DX11Prerequisites.h"
        "DX11Prerequisites.cpp"
		
        "DX11_commandList.h"
        "DX11_commandList.cpp"
        "DX11_commandSystem.h"
        "DX11_commandSystem.cpp"
        "DX11_APITranslationLayer.h"
        "DX11APITranslationLayer.cpp"
        "DX11UDevice.h"
        "DX11UDevice.cpp"
        "DX11UDriver.h"
        "DX11UDriver.cpp"
        "DX11UDriverAPI.h"
        "DX11UDriverAPI.cpp"
        "DX11_presentationLayer.h"
        "DX11_presentationLayer.cpp"


        "resources/DX11UBuffer.h"
        "resources/DX11UBuffer.cpp"
        "resources/DX11_sampler.h"
        "resources/DX11_sampler.cpp"
        "resources/DX11_shader.h"
        "resources/DX11_shader.cpp"
        "resources/DX11_texture.h"
        "resources/DX11_texture.cpp"

        "state/DX11_pipelineStateController.h"
        "state/DX11_pipelineStateController.cpp"
        "state/DX11_pipelineStateObject.h"
        "state/DX11_pipelineStateObject.cpp"

        "state/DX11_commonGraphicsConfig.cpp"
        "state/DX11_commonGraphicsConfig.h"
        "state/DX11_graphicsShaderState.h"

        "state/DX11_inputAssembler.cpp"
		"state/DX11_inputAssembler.h"
		"state/DX11_pipelineCompiledDescriptorFactory.cpp"
		"state/DX11_pipelineCompiledDescriptorFactory.h"
		"state/DX11_renderTarget.cpp"
		"state/DX11_renderTarget.h"
		
		)


if( ${CM_EBS_SYSCONF_TARGET_FLAG_OSINFO_WFA} )

    if( "${IC3_COMPONENTS_BUILD_MODE}" STREQUAL "STATIC" )
        add_library( Ic3.Graphics.HW3D.DX11 STATIC ${IC3_GRAPHICS_HWAPI_DX11_SRC_FILES} )
        target_compile_definitions( Ic3.Graphics.HW3D.DX11 PRIVATE
                "IC3_BUILD_STATIC=1")
    else()
        add_library( Ic3.Graphics.HW3D.DX11 SHARED ${IC3_GRAPHICS_HWAPI_DX11_SRC_FILES} )
    endif()

    set_target_properties( Ic3.Graphics.HW3D.DX11 PROPERTIES
            OUTPUT_NAME "ts3dx-driver-gpuapi-dx11" )

    target_compile_definitions( Ic3.Graphics.HW3D.DX11 PRIVATE
            "IC3_GRAPHICS_HWAPI_DX_BUILD=1"
            "IC3_GRAPHICS_HWAPI_DX11_BUILD=1")

    target_include_directories( Ic3.Graphics.HW3D.DX11 PUBLIC
            ${IC3_DRIVERS_INCLUDE_DIR_INTERNAL} )

    target_link_libraries( Ic3.Graphics.HW3D.DX11 PUBLIC
            Ic3.Graphics.GCI
            D3D11.lib
            D3DCompiler.lib )

endif()
