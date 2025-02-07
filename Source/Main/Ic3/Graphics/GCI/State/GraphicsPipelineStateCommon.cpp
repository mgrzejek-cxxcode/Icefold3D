
#include "GraphicsPipelineStateCommon.h"
#include <Ic3/Graphics/GCI/State/RenderTargetCommon.h>

namespace Ic3::Graphics::GCI
{

	GraphicsPipelineStateObject::GraphicsPipelineStateObject( GPUDevice & pGPUDevice )
	: PipelineStateObject( pGPUDevice )
	{}

	GraphicsPipelineStateObject::~GraphicsPipelineStateObject() = default;

	EPipelineType GraphicsPipelineStateObject::GetPipelineType() const noexcept
	{
		return EPipelineType::Compute;
	}

} // namespace Ic3::Graphics::GCI
