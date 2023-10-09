
#pragma once

#ifndef __IC3_GRAPHICS_GCI_GPU_MEMORY_POOL_H__
#define __IC3_GRAPHICS_GCI_GPU_MEMORY_POOL_H__

#include "CommonGPUMemoryDefs.h"
#include <Ic3/CoreLib/Sync/SpinLock.h>
#include <Ic3/CoreLib/Sync/SyncInterface.h>

namespace Ic3::Graphics::GCI
{

	class GPUMemoryRef
	{
	public:
		GPUMemoryPool * const mSourcePool;

		GPUMemoryRef( GPUMemoryPool & pSourcePool, const GPUMemoryRegion & pPoolSubRegion );
		~GPUMemoryRef();

		void lockMemory();

		bool tryLockMemory();

		void unlockMemory();

		IC3_ATTR_NO_DISCARD bool empty() const;

		IC3_ATTR_NO_DISCARD bool isMemoryLocked() const;

	private:
		GPUMemoryRegion _poolSubRegion;
		Sync::AutoSharedLock<Sync::SharedSpinLock> _poolMemoryLock;
		std::atomic<uint32_t> _poolMemoryLockStatus;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_GPU_MEMORY_POOL_H__

