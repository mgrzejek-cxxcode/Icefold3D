
#include "PipelineStateDescriptor.h"
#include "../GPUDevice.h"

namespace Ic3::Graphics::GCI
{

	/// #PipelineStateDescriptor

	PipelineStateDescriptor::PipelineStateDescriptor( GPUDevice & pGPUDevice, pipeline_state_descriptor_id_t pDescriptorID )
	: GPUDeviceChildObject( pGPUDevice )
	, mDescriptorID( pDescriptorID )
	{}

	PipelineStateDescriptor::~PipelineStateDescriptor() = default;

	bool PipelineStateDescriptor::IsDynamicDescriptor() const noexcept
	{
		return false;
	}

	bool PipelineStateDescriptor::IsDynamicDriverStateInitialized() const noexcept
	{
		return false;
	}

	DynamicDescriptorDriverState * PipelineStateDescriptor::GetDynamicDriverState() const
	{
		if( !IsDynamicDescriptor() )
		{
			Ic3Throw( 0 );
		}

		return nullptr;
	}

	void PipelineStateDescriptor::SetDynamicDriverState( DynamicDescriptorDriverState * pDynamicDriverState ) noexcept
	{
		Ic3UnusedParam( pDynamicDriverState );

		if( !IsDynamicDescriptor() )
		{
			Ic3Throw( 0 );
		}
	}


	/// #PipelineStateObject

	PipelineStateObject::PipelineStateObject( GPUDevice & pGPUDevice, pipeline_state_object_id_t pStateObjectID )
	: GPUDeviceChildObject( pGPUDevice )
	, mStateObjectID( pStateObjectID )
	{}

	PipelineStateObject::~PipelineStateObject() = default;

} // namespace Ic3::Graphics::GCI
