
#include "GraphicsPipelineStateCommon.h"
#include <Ic3/Graphics/GCI/State/RenderTargetCommon.h>

namespace Ic3::Graphics::GCI
{

	GraphicsPipelineStateObject::GraphicsPipelineStateObject(
			GPUDevice & pGPUDevice,
			pipeline_state_object_id_t pStateObjectID,
			const RenderTargetLayout & pRenderTargetLayout,
			const GraphicsPipelineStateObjectDescriptorIDSet & pDescriptorsIDSet )
	: PipelineStateObject( pGPUDevice, pStateObjectID )
	, mRenderTargetLayout( pRenderTargetLayout )
	, mDescriptorsIDSet( pDescriptorsIDSet )

	{}

	GraphicsPipelineStateObject::~GraphicsPipelineStateObject() = default;

	EPipelineType GraphicsPipelineStateObject::GetPipelineType() const noexcept
	{
		return EPipelineType::Compute;
	}

} // namespace Ic3::Graphics::GCI
