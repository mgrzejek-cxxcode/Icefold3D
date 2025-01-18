
#pragma once

#ifndef __IC3_GRAPHICS_GCI_GPU_RESOURCE_H__
#define __IC3_GRAPHICS_GCI_GPU_RESOURCE_H__

#include "CommonGPUResourceDefs.h"
#include <cppx/refCounter.h>

namespace Ic3::Graphics::GCI
{

	using gpu_resource_ref_counter_value_t = cppx::ref_counter_default_value_t;

	struct GPUResourceProperties
	{
		cppx::bitmask<resource_flags_value_t> resourceFlags;
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

		CPPX_ATTR_NO_DISCARD gpu_resource_ref_counter_value_t GetActiveRefsNum() const noexcept;

		CPPX_ATTR_NO_DISCARD const ResourceMappedMemory & GetMappedMemory() const noexcept;

		CPPX_ATTR_NO_DISCARD bool IsMapped() const;

		CPPX_ATTR_NO_DISCARD bool IsMapped( const GPUMemoryRegion & pRegion ) const;

		CPPX_ATTR_NO_DISCARD virtual const GPUResourceProperties & GetProperties() const = 0;

	protected:
		gpu_resource_ref_counter_value_t AddActiveRef();

		gpu_resource_ref_counter_value_t ReleaseActiveRef();

		void SetMappedMemory( const ResourceMappedMemory & pMappedMemory );

		void ResetMappedMemory();

	private:
		cppx::atomic_ref_counter<> _activeRefsCounter;
		ResourceMappedMemory _mappedMemory;
	};

	inline gpu_resource_ref_counter_value_t GPUResource::GetActiveRefsNum() const noexcept
	{
		return _activeRefsCounter.get_value();
	}

	inline const ResourceMappedMemory & GPUResource::GetMappedMemory() const noexcept
	{
		return _mappedMemory;
	}

	class GPUResourceView : public GPUDeviceChildObject
	{
	public:
		EGPUResourceBaseType const mAliasedResourceType;
		cppx::bitmask<resource_flags_value_t> const mResourceFlags;

		GPUResourceView(
			GPUDevice & pGPUDevice,
			EGPUResourceBaseType pAliasedResourceType,
			cppx::bitmask<resource_flags_value_t> pResourceFlags );

		virtual ~GPUResourceView();
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_GPU_RESOURCE_H__
