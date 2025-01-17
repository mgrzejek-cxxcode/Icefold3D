
#include "GpuResource.h"
#include "../GpuDevice.h"

namespace Ic3::Graphics::GCI
{

	GpuResource::GpuResource(
			GpuDevice & pGpuDevice,
			EGpuResourceBaseType pResourceBaseType,
			const ResourceMemoryInfo & pResourceMemory )
	: GpuDeviceChildObject( pGpuDevice )
	, mResourceBaseType( pResourceBaseType )
	, mResourceMemory( pResourceMemory )
	{}

	GpuResource::~GpuResource() = default;

	bool GpuResource::IsMapped() const
	{
		return ( _mappedMemory.pointer != nullptr ) && !_mappedMemory.mappedRegion.empty();
	}

	bool GpuResource::IsMapped( const GpuMemoryRegion & pRegion ) const
	{
		return cppx::range_is_sub_range_of( pRegion.as_range(), _mappedMemory.mappedRegion.as_range() );
	}

	gpu_resource_ref_counter_value_t GpuResource::AddActiveRef()
	{
		return _activeRefsCounter.increment();
	}

	gpu_resource_ref_counter_value_t GpuResource::ReleaseActiveRef()
	{
		const auto activeRefNum = _activeRefsCounter.decrement();

		if( mGpuDevice.IsResourceActiveRefsTrackingEnabled() && ( activeRefNum == 0 ) )
		{
			mGpuDevice.OnGpuResourceActiveRefsZero( *this );
		}

		return activeRefNum;
	}

	void GpuResource::SetMappedMemory( const ResourceMappedMemory & pMappedMemory )
	{
		_mappedMemory = pMappedMemory;
		_mappedMemory.sourceMemory = &mResourceMemory;
	}

	void GpuResource::ResetMappedMemory()
	{
		_mappedMemory.pointer = nullptr;
		_mappedMemory.memoryMapFlags = 0;
		_mappedMemory.mappedRegion.set_empty();
	}


	GpuResourceView::GpuResourceView(
			GpuDevice & pGpuDevice,
			EGpuResourceBaseType pAliasedResourceType,
			cppx::bitmask<resource_flags_value_t> pResourceFlags )
	: GpuDeviceChildObject( pGpuDevice )
	, mAliasedResourceType( pAliasedResourceType )
	, mResourceFlags( pResourceFlags )
	{}

	GpuResourceView::~GpuResourceView() = default;

} // namespace Ic3::Graphics::GCI
