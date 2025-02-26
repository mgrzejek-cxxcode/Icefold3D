
set( IC3_GRAPHICS_GCI_SRC_FILES
        "../Common/GraphicsCoreMetrics.h"

        "Prerequisites/CommonTypes.h"
        "Prerequisites/CoreInterfaceDefs.h"
        "Prerequisites/CoreInterfaceDefs.cpp"
        "Prerequisites/GPUDataFormats.h"
        "Prerequisites/GPUDataFormats.cpp"
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
        "GPUDevice.h"
        "GPUDevice.cpp"
        "GPUDeviceNull.h"
        "GPUDriver.h"
        "GPUDriver.cpp"
        "GPUDriverAPI.h"
        "GPUDriverNull.h"
        "GPUDriverPlugin.h"
        "GPUUtils.h"
        "GPUUtils.cpp"
        "GPUCapabilities.h"
        "PresentationLayer.h"
        "PresentationLayer.cpp"

        "Memory/CommonGPUMemoryDefs.h"
        "Memory/CommonGPUMemoryDefs.cpp"
        "Memory/GPUMemoryAllocator.h"
        "Memory/GPUMemoryAllocator.cpp"
        "Memory/GPUMemoryHeap.h"
        "Memory/GPUMemoryHeap.cpp"
        "Memory/GPUMemoryPool.h"
        "Memory/GPUMemoryPool.cpp"
        "Memory/GPUMemoryRef.h"
        "Memory/GPUMemoryRef.cpp"

        "Resources/CommonGPUResourceDefs.h"
        "Resources/GPUBuffer.h"
        "Resources/GPUBuffer.cpp"
        "Resources/GPUBufferReference.h"
        "Resources/GPUBufferReference.cpp"
        "Resources/GPUBufferCommon.h"
        "Resources/GPUBufferCommon.cpp"
        "Resources/GPUResource.h"
        "Resources/GPUResource.cpp"
        "Resources/RenderTargetTexture.h"
        "Resources/RenderTargetTexture.cpp"
        "Resources/Shader.h"
        "Resources/Shader.cpp"
        "Resources/ShaderCommon.h"
        "Resources/ShaderInputTexture.cpp"
        "Resources/ShaderInputTexture.h"
        "Resources/Texture.h"
        "Resources/Texture.cpp"
        "Resources/TextureCommon.h"
        "Resources/TextureCommon.cpp"
        "Resources/TextureDimensions.h"
        "Resources/TextureDimensions.cpp"
        "Resources/TextureReference.h"
        "Resources/TextureReference.cpp"

        "State/CommonGPUStateDefs.h"
        "State/CommonGPUStateDefs.cpp"
        "State/CommonGraphicsConfig.h"
        "State/CommonGraphicsConfig.cpp"

        "State/ComputePipelineStateCommon.h"
        "State/ComputePipelineStateCommon.cpp"

        "State/GraphicsPipelineStateCommon.h"
        "State/GraphicsPipelineStateCommon.cpp"
        "State/GraphicsPipelineStateController.h"
        "State/GraphicsPipelineStateController.cpp"
        "State/PipelineStateDescriptorCache.h"
        "State/PipelineStateDescriptorCache.cpp"
        "State/GraphicsPipelineStateDescriptorCommon.h"
        "State/GraphicsPipelineStateDescriptorCommon.cpp"
        "State/GraphicsPipelineStateDescriptorIA.h"
        "State/GraphicsPipelineStateDescriptorIA.cpp"
        "State/GraphicsPipelineStateDescriptorRTArrayCommon.h"
        "State/GraphicsPipelineStateDescriptorRTO.h"
        "State/GraphicsPipelineStateDescriptorRTO.cpp"
        "State/GraphicsPipelineStateDescriptorRTODynamic.h"
        "State/GraphicsPipelineStateDescriptorRTODynamic.cpp"
        "State/GraphicsPipelineStateDescriptorShader.h"
        "State/GraphicsPipelineStateDescriptorShader.cpp"
        "State/GraphicsShaderDefs.h"
        "State/GraphicsShaderDefs.cpp"

        "State/InputAssemblerCommon.h"
        "State/InputAssemblerCommon.cpp"
        "State/RenderPassCommon.h"
        "State/RenderPassCommon.cpp"
        "State/RenderTargetArrayUtils.h"
        "State/RenderTargetCommon.h"
        "State/RenderTargetCommon.cpp"
        "State/Sampler.h"
        "State/Sampler.cpp"
        "State/SamplerCommon.h"
        "State/SamplerCommon.cpp"
        "State/RootSignature.h"
        "State/RootSignature.cpp"
        "State/PipelineStateDescriptorRootSignature.h"
        "State/PipelineStateDescriptorRootSignature.cpp"

        "State/PipelineStateCommon.h"
        "State/PipelineStateCommon.cpp"
        "State/PipelineStateDescriptorCacheUnit.h"
        "State/PipelineStateDescriptorCacheUnit.cpp"
        "State/PipelineStateDescriptorFactory.h"
        "State/PipelineStateDescriptorFactory.cpp"
        "State/PipelineStateDescriptorManager.h"
        "State/PipelineStateDescriptorManager.cpp"
        "State/GraphicsPipelineStateCommon.h"
        "State/GraphicsPipelineStateCommon.cpp"
        "State/GraphicsPipelineStateImplSeparableState.h"
        "State/GraphicsPipelineStateImplSeparableState.cpp"

        State/PipelineStateDescriptor.cpp
        State/PipelineStateDescriptor.h
        State/PipelineStateController.cpp
        State/PipelineStateController.h
        State/PipelineStateDescriptorDynamic.h
        State/GraphicsPipelineStateDescriptorIADynamic.cpp
        State/GraphicsPipelineStateDescriptorIADynamic.h
        GPUDeviceFeatureQuery.h
        GPUDeviceFeatureQuery.cpp
        State/GraphicsPipelineStateController.inl
        State/GraphicsPipelineStateImplGenericRenderPass.h
        State/GraphicsPipelineStateImplGenericRenderPass.cpp
        State/GraphicsPipelineStateImplSeparableState.inl
        CommandListImplGenericRenderPass.h

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
        )
