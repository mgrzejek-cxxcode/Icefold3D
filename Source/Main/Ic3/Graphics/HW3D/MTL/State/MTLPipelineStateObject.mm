
#include "MTLpipelineStateObject.h"
#include "../MTLUDevice.h"

#include <Metal/MTLRenderPipeline.h>

namespace Ic3::Graphics::GCI
{

	MetalGraphicsPipelineStateObject::MetalGraphicsPipelineStateObject(
			MetalGpuDevice & pGpuDevice,
			RenderTargetLayout pRenderTargetLayout,
			ShaderInputSignature pShaderInputSignature,
			id<MTLRenderPipelineState> pMTLPipelineState )
	: GraphicsPipelineStateObject(
		pGpuDevice,
		std::move( pRenderTargetLayout ),
		std::move( pShaderInputSignature ) )
	, mMTLPipelineState( pMTLPipelineState )
	{}

	MetalGraphicsPipelineStateObject::~MetalGraphicsPipelineStateObject() = default;

	GpaHandle<MetalGraphicsPipelineStateObject> MetalGraphicsPipelineStateObject::Create(
			MetalGpuDevice & pGpuDevice,
			const GraphicsPipelineStateObjectCreateInfo & pCreateInfo )
	{
	@autoreleasepool
	{
		auto & mtlDevice = pGpuDevice.mMTLDevice;

		auto * pipelineStateDescriptor = [[MTLRenderPipelineDescriptor alloc] init];
	}
	}

}
