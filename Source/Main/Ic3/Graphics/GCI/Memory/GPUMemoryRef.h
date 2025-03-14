
#pragma once

#ifndef __IC3_GRAPHICS_GCI_GPU_MEMORY_POOL_H__
#define __IC3_GRAPHICS_GCI_GPU_MEMORY_POOL_H__

#include "CommonGPUMemoryDefs.h"

namespace Ic3::Graphics::GCI
{

	class GPUMemoryRef
	{
	public:
		GPUMemoryPool * const mSourcePool;

		GPUMemoryRef( GPUMemoryPool & pSourcePool, const GPUMemoryRegion & pPoolSubRegion );
		~GPUMemoryRef();

		CPPX_ATTR_NO_DISCARD bool empty() const;

		CPPX_ATTR_NO_DISCARD bool IsMemoryLocked() const;

		void lockMemory();

		bool tryLockMemory();

		void unlockMemory();

	private:
		GPUMemoryRegion _poolSubRegion;
		//Sync::TAutoSharedLock<Sync::SharedSpinLock> _poolMemoryLock;
		std::atomic<uint32_t> _poolMemoryLockStatus;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_GPU_MEMORY_POOL_H__

