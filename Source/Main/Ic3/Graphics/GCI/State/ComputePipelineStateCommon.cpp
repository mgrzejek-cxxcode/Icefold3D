
#include "ComputePipelineStateCommon.h"

namespace Ic3::Graphics::GCI
{

	ComputePipelineStateObject::ComputePipelineStateObject( GPUDevice & pGPUDevice, pipeline_state_object_id_t pStateObjectID )
	: PipelineStateObject( pGPUDevice, pStateObjectID )
	{}

	ComputePipelineStateObject::~ComputePipelineStateObject() = default;

	EPipelineType ComputePipelineStateObject::GetPipelineType() const noexcept
	{
		return EPipelineType::Compute;
	}

} // namespace Ic3::Graphics::GCI
