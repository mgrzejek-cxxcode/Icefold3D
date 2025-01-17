
set( IC3_GRAPHICS_GCI_SRC_FILES
        "../Common/GraphicsCoreMetrics.h"

        "Prerequisites/CommonTypes.h"
        "Prerequisites/CoreInterfaceDefs.h"
        "Prerequisites/CoreInterfaceDefs.cpp"
        "Prerequisites/GpuDataFormats.h"
        "Prerequisites/GpuDataFormats.cpp"
        "Prerequisites/VertexAttribFormatUtils.h"

        "Prerequisites.h"
        "Prerequisites.cpp"

        "CommonCommandDefs.h"
        "CommandContext.h"
        "CommandContext.cpp"
        "CommandList.h"
        "CommandList.cpp"
        "CommandSystem.h"
        "CommandSystem.cpp"
        "DisplayCommon.h"
        "DisplayManager.h"
        "DisplayManager.cpp"
        "GpuDevice.h"
        "GpuDevice.cpp"
        "GpuDeviceNull.h"
        "GpuDriver.h"
        "GpuDriver.cpp"
        "GpuDriverAPI.h"
        "GpuDriverNull.h"
        "GpuDriverPlugin.h"
        "GpuUtils.h"
        "GpuUtils.cpp"
        "GpuCapabilities.h"
        "PresentationLayer.h"
        "PresentationLayer.cpp"

        "Memory/CommonGpuMemoryDefs.h"
        "Memory/CommonGpuMemoryDefs.cpp"
        "Memory/GpuMemoryAllocator.h"
        "Memory/GpuMemoryAllocator.cpp"
        "Memory/GpuMemoryHeap.h"
        "Memory/GpuMemoryHeap.cpp"
        "Memory/GpuMemoryPool.h"
        "Memory/GpuMemoryPool.cpp"
        "Memory/GpuMemoryRef.h"
        "Memory/GpuMemoryRef.cpp"

        "Resources/CommonGpuResourceDefs.h"
        "Resources/GpuBuffer.h"
        "Resources/GpuBuffer.cpp"
        "Resources/GpuBufferReference.h"
        "Resources/GpuBufferReference.cpp"
        "Resources/GpuBufferCommon.h"
        "Resources/GpuBufferCommon.cpp"
        "Resources/GpuResource.h"
        "Resources/GpuResource.cpp"
        "Resources/RenderTargetTexture.h"
        "Resources/RenderTargetTexture.cpp"
        "Resources/Shader.h"
        "Resources/Shader.cpp"
        "Resources/ShaderCommon.h"
        "Resources/Texture.h"
        "Resources/Texture.cpp"
        "Resources/TextureCommon.h"
        "Resources/TextureCommon.cpp"
        "Resources/TextureDimensions.h"
        "Resources/TextureDimensions.cpp"
        "Resources/TextureReference.h"
        "Resources/TextureReference.cpp"

        "State/CommonGpuStateDefs.h"
        "State/CommonGpuStateDefs.cpp"
        "State/CommonGraphicsConfig.h"
        "State/CommonGraphicsConfig.cpp"
        "State/CommonGraphicsConfigImmutableStates.h"
        "State/CommonGraphicsConfigImmutableStates.cpp"

        "State/GraphicsPipelineImmutableState.h"
        "State/GraphicsPipelineImmutableState.cpp"
        "State/GraphicsShaderLinkageImmutableState.h"
        "State/GraphicsShaderLinkageImmutableState.cpp"

        "State/InputAssemblerCommon.h"
        "State/InputAssemblerCommon.cpp"
        "State/InputAssemblerDynamicStates.h"
        "State/InputAssemblerDynamicStates.cpp"
        "State/InputAssemblerImmutableStates.h"
        "State/InputAssemblerImmutableStates.cpp"

        "State/GraphicsPipelineStateController.h"
        "State/GraphicsPipelineStateController.cpp"
        "State/GraphicsShaderState.h"
        "State/GraphicsShaderState.cpp"
        "State/PipelineImmutableStateCache.h"
        "State/PipelineImmutableStateCache.cpp"
        "State/PipelineStateObject.h"
        "State/PipelineStateObject.cpp"
        "State/SeparablePipelineState.h"
        "State/SeparablePipelineState.cpp"
        "State/ShaderInputSignature.h"
        "State/ShaderInputSignature.cpp"

        "State/RenderPassCommon.h"
        "State/RenderPassCommon.cpp"
        "State/RenderTargetCommon.h"
        "State/RenderTargetCommon.cpp"
        "State/RenderTargetDynamicStates.h"
        "State/RenderTargetDynamicStates.cpp"
        "State/RenderTargetImmutableStates.h"
        "State/RenderTargetImmutableStates.cpp"

        "State/Sampler.h"
        "State/Sampler.cpp"
        "State/SamplerCommon.h"
        "State/SamplerCommon.cpp"
        "resources/ShaderInputTexture.cpp"
        "resources/ShaderInputTexture.h"
        )

if( "${IC3_COMPONENTS_BUILD_MODE}" STREQUAL "STATIC" )
    add_library( Ic3.Graphics.GCI STATIC ${IC3_GRAPHICS_GCI_SRC_FILES} )
    target_compile_definitions( Ic3.Graphics.GCI PRIVATE
            "IC3_BUILD_STATIC=1" )
else()
    add_library( Ic3.Graphics.GCI SHARED ${IC3_GRAPHICS_GCI_SRC_FILES} )
endif()

set_target_properties( Ic3.Graphics.GCI PROPERTIES
        OUTPUT_NAME "Ic3.Graphics.GCI"
        )

target_compile_definitions( Ic3.Graphics.GCI PRIVATE
        "IC3_GRAPHICS_GCI_BUILD=1"
        )

target_include_directories( Ic3.Graphics.GCI PUBLIC
        "${IC3_INCLUDE_DIR}"
        )

target_link_libraries( Ic3.Graphics.GCI PUBLIC
        Ic3.CoreLib
        Ic3.Math
        Ic3.Cppx
        )
