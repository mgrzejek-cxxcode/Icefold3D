
#include "MTLpipelineStateObject.h"
#include "../MTLUDevice.h"

#include <Metal/MTLRenderPipeline.h>

namespace Ic3::Graphics::GCI
{

	MetalGraphicsPipelineStateObject::MetalGraphicsPipelineStateObject(
			MetalGPUDevice & pGPUDevice,
			RenderTargetLayout pRenderTargetLayout,
			RootSignature pRootSignature,
			id<MTLRenderPipelineState> pMTLPipelineState )
	: GraphicsPipelineStateObject(
		pGPUDevice,
		std::move( pRenderTargetLayout ),
		std::move( pRootSignature ) )
	, mMTLPipelineState( pMTLPipelineState )
	{}

	MetalGraphicsPipelineStateObject::~MetalGraphicsPipelineStateObject() = default;

	GpaHandle<MetalGraphicsPipelineStateObject> MetalGraphicsPipelineStateObject::Create(
			MetalGPUDevice & pGPUDevice,
			const GraphicsPipelineStateObjectCreateInfo & pCreateInfo )
	{
	@autoreleasepool
	{
		auto & mtlDevice = pGPUDevice.mMTLDevice;

		auto * pipelineStateDescriptor = [[MTLRenderPipelineDescriptor alloc] init];
	}
	}

}
