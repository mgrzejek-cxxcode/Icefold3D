
#include "PipelineStateObject.h"
#include <Ic3/Graphics/GCI/State/RenderTargetCommon.h>

namespace Ic3::Graphics::GCI
{

	PipelineStateObject::PipelineStateObject( GPUDevice & pGPUDevice )
	: GPUDeviceChildObject( pGPUDevice )
	{}

	PipelineStateObject::~PipelineStateObject() = default;


	ComputePipelineStateObject::ComputePipelineStateObject( GPUDevice & pGPUDevice )
	: PipelineStateObject( pGPUDevice )
	{}

	ComputePipelineStateObject::~ComputePipelineStateObject() = default;


	GraphicsPipelineStateObject::GraphicsPipelineStateObject(
			GPUDevice & pGPUDevice,
			RenderTargetLayout pRenderTargetLayout,
			ShaderInputSignature pShaderInputSignature )
	: PipelineStateObject( pGPUDevice )
	, mRenderTargetLayout( std::move( pRenderTargetLayout ) )
	, mShaderInputSignature( std::move( pShaderInputSignature ) )
	{}

	GraphicsPipelineStateObject::~GraphicsPipelineStateObject() = default;

} // namespace Ic3::Graphics::GCI
