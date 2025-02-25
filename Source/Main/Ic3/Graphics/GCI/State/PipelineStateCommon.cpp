
#include "PipelineStateCommon.h"

namespace Ic3::Graphics::GCI
{

	PipelineStateObject::PipelineStateObject( GPUDevice & pGPUDevice, pipeline_state_object_id_t pStateObjectID )
	: GPUDeviceChildObject( pGPUDevice )
	, mStateObjectID( pStateObjectID )
	{}

	PipelineStateObject::~PipelineStateObject() = default;

} // namespace Ic3::Graphics::GCI
