
#pragma once

#ifndef __IC3_GRAPHICS_GCI_GPU_RESOURCE_H__
#define __IC3_GRAPHICS_GCI_GPU_RESOURCE_H__

#include "CommonGPUResourceDefs.h"
#include <Ic3/Cppx/RefCounter.h>

namespace Ic3::Graphics::GCI
{

	struct GPUResourceProperties
	{
		Bitmask<resource_flags_value_t> resourceFlags;
	};

	class GPUResource : public GPUDeviceChildObject
	{
	public:
		EGPUResourceBaseType const mResourceBaseType;
		ResourceMemoryInfo const mResourceMemory;

		GPUResource(
			GPUDevice & pGPUDevice,
			EGPUResourceBaseType pResourceBaseType,
			const ResourceMemoryInfo & pResourceMemory );

		virtual ~GPUResource();

		IC3_ATTR_NO_DISCARD ref_counter_value_t getActiveRefsNum() const noexcept;

		IC3_ATTR_NO_DISCARD const ResourceMappedMemory & getMappedMemory() const noexcept;

		IC3_ATTR_NO_DISCARD bool isMapped() const;

		IC3_ATTR_NO_DISCARD bool isMapped( const GPUMemoryRegion & pRegion ) const;

		IC3_ATTR_NO_DISCARD virtual const GPUResourceProperties & getProperties() const = 0;

	protected:
		ref_counter_value_t addActiveRef();

		ref_counter_value_t releaseActiveRef();

		void setMappedMemory( const ResourceMappedMemory & pMappedMemory );

		void resetMappedMemory();

	private:
		AtomicRefCounter _activeRefsCounter;
		ResourceMappedMemory _mappedMemory;
	};

	inline ref_counter_value_t GPUResource::getActiveRefsNum() const noexcept
	{
		return _activeRefsCounter.getValue();
	}

	inline const ResourceMappedMemory & GPUResource::getMappedMemory() const noexcept
	{
		return _mappedMemory;
	}

	class GPUResourceView : public GPUDeviceChildObject
	{
	public:
		EGPUResourceBaseType const mAliasedResourceType;
		Bitmask<resource_flags_value_t> const mResourceFlags;

		GPUResourceView(
			GPUDevice & pGPUDevice,
			EGPUResourceBaseType pAliasedResourceType,
			Bitmask<resource_flags_value_t> pResourceFlags );

		virtual ~GPUResourceView();
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_GPU_RESOURCE_H__
