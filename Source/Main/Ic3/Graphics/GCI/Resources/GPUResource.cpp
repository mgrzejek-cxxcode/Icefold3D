
#include "GPUResource.h"
#include "../GPUDevice.h"

namespace Ic3::Graphics::GCI
{

	GPUResource::GPUResource(
			GPUDevice & pGPUDevice,
			EGPUResourceBaseType pResourceBaseType,
			const ResourceMemoryInfo & pResourceMemory )
	: GPUDeviceChildObject( pGPUDevice )
	, mResourceBaseType( pResourceBaseType )
	, mResourceMemory( pResourceMemory )
	{}

	GPUResource::~GPUResource() = default;

	bool GPUResource::IsMapped() const
	{
		return ( _mappedMemory.pointer != nullptr ) && !_mappedMemory.mappedRegion.empty();
	}

	bool GPUResource::IsMapped( const GPUMemoryRegion & pRegion ) const
	{
		return cppx::range_is_sub_range_of( pRegion.as_range(), _mappedMemory.mappedRegion.as_range() );
	}

	gpu_resource_ref_counter_value_t GPUResource::AddActiveRef()
	{
		return _activeRefsCounter.increment();
	}

	gpu_resource_ref_counter_value_t GPUResource::ReleaseActiveRef()
	{
		const auto activeRefNum = _activeRefsCounter.decrement();

		if( mGPUDevice.IsResourceActiveRefsTrackingEnabled() && ( activeRefNum == 0 ) )
		{
			mGPUDevice.OnGPUResourceActiveRefsZero( *this );
		}

		return activeRefNum;
	}

	void GPUResource::SetMappedMemory( const ResourceMappedMemory & pMappedMemory )
	{
		_mappedMemory = pMappedMemory;
		_mappedMemory.sourceMemory = &mResourceMemory;
	}

	void GPUResource::ResetMappedMemory()
	{
		_mappedMemory.pointer = nullptr;
		_mappedMemory.memoryMapFlags = 0;
		_mappedMemory.mappedRegion.set_empty();
	}


	GPUResourceView::GPUResourceView(
			GPUDevice & pGPUDevice,
			EGPUResourceBaseType pAliasedResourceType,
			cppx::bitmask<resource_flags_value_t> pResourceFlags )
	: GPUDeviceChildObject( pGPUDevice )
	, mAliasedResourceType( pAliasedResourceType )
	, mResourceFlags( pResourceFlags )
	{}

	GPUResourceView::~GPUResourceView() = default;

} // namespace Ic3::Graphics::GCI
