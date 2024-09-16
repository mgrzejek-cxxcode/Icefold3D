
#pragma once

#ifndef __IC3_GRAPHICS_GCI_GPU_RESOURCE_H__
#define __IC3_GRAPHICS_GCI_GPU_RESOURCE_H__

#include "CommonGPUResourceDefs.h"
#include <Ic3/Cppx/RefCounter.h>

namespace Ic3::Graphics::GCI
{

	using gpu_resource_ref_counter_value_t = Cppx::ref_counter_value_t;

	struct GPUResourceProperties
	{
		TBitmask<resource_flags_value_t> mResourceFlags;
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

		IC3_ATTR_NO_DISCARD gpu_resource_ref_counter_value_t getActiveRefsNum() const noexcept;

		IC3_ATTR_NO_DISCARD const ResourceMappedMemory & getMappedMemory() const noexcept;

		IC3_ATTR_NO_DISCARD bool isMapped() const;

		IC3_ATTR_NO_DISCARD bool isMapped( const GPUMemoryRegion & pRegion ) const;

		IC3_ATTR_NO_DISCARD virtual const GPUResourceProperties & getProperties() const = 0;

	protected:
		gpu_resource_ref_counter_value_t addActiveRef();

		gpu_resource_ref_counter_value_t releaseActiveRef();

		void setMappedMemory( const ResourceMappedMemory & pMappedMemory );

		void resetMappedMemory();

	private:
		Cppx::AtomicRefCounter _activeRefsCounter;
		ResourceMappedMemory _mappedMemory;
	};

	inline gpu_resource_ref_counter_value_t GPUResource::getActiveRefsNum() const noexcept
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
		TBitmask<resource_flags_value_t> const mResourceFlags;

		GPUResourceView(
			GPUDevice & pGPUDevice,
			EGPUResourceBaseType pAliasedResourceType,
			TBitmask<resource_flags_value_t> pResourceFlags );

		virtual ~GPUResourceView();
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_GPU_RESOURCE_H__
