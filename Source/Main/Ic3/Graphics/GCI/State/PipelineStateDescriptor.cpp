
#include "PipelineStateDescriptor.h"
#include "../GPUDevice.h"

namespace Ic3::Graphics::GCI
{

	/// #PipelineStateDescriptor

	PipelineStateDescriptor::PipelineStateDescriptor( GPUDevice & pGPUDevice )
	: GPUDeviceChildObject( pGPUDevice )
	, _descriptorID( kPipelineStateDescriptorIDInvalid )
	{}

	PipelineStateDescriptor::~PipelineStateDescriptor() = default;

	bool PipelineStateDescriptor::IsDynamicDescriptor() const noexcept
	{
		return false;
	}

	void PipelineStateDescriptor::SetInternalFlags( cppx::bitmask<descriptor_internal_flags_value_t> pInternalFlags ) noexcept
	{
		_internalFlags.set( pInternalFlags );
	}

	void PipelineStateDescriptor::ResetInternalFlags( cppx::bitmask<descriptor_internal_flags_value_t> pInternalFlags ) noexcept
	{
		_internalFlags.unset( pInternalFlags );
	}

	bool PipelineStateDescriptor::CheckInternalFlags( cppx::bitmask<descriptor_internal_flags_value_t> pInternalFlags ) const noexcept
	{
		return _internalFlags.is_set( pInternalFlags );
	}

	cppx::bitmask<PipelineStateDescriptor::descriptor_internal_flags_value_t> PipelineStateDescriptor::GetInternalFlags() const noexcept
	{
		return _internalFlags;
	}

} // namespace Ic3::Graphics::GCI
