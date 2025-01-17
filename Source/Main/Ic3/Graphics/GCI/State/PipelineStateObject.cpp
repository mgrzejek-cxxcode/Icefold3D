
#include "PipelineStateObject.h"
#include <Ic3/Graphics/GCI/State/RenderTargetCommon.h>

namespace Ic3::Graphics::GCI
{

	PipelineStateObject::PipelineStateObject( GpuDevice & pGpuDevice )
	: GpuDeviceChildObject( pGpuDevice )
	{}

	PipelineStateObject::~PipelineStateObject() = default;


	ComputePipelineStateObject::ComputePipelineStateObject( GpuDevice & pGpuDevice )
	: PipelineStateObject( pGpuDevice )
	{}

	ComputePipelineStateObject::~ComputePipelineStateObject() = default;


	GraphicsPipelineStateObject::GraphicsPipelineStateObject(
			GpuDevice & pGpuDevice,
			RenderTargetLayout pRenderTargetLayout,
			ShaderInputSignature pShaderInputSignature )
	: PipelineStateObject( pGpuDevice )
	, mRenderTargetLayout( std::move( pRenderTargetLayout ) )
	, mShaderInputSignature( std::move( pShaderInputSignature ) )
	{}

	GraphicsPipelineStateObject::~GraphicsPipelineStateObject() = default;

} // namespace Ic3::Graphics::GCI
