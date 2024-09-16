
#pragma once

#ifndef __IC3_GRAPHICS_GCI_GPU_MEMORY_POOL_H__
#define __IC3_GRAPHICS_GCI_GPU_MEMORY_POOL_H__

#include "CommonGPUMemoryDefs.h"
#include <Ic3/CoreLib/Sync/SpinLock.h>

namespace Ic3::Graphics::GCI
{

	class GPUMemoryAllocator;
	class GPUMemoryHeap;
	class GPUMemoryRef;

	class GPUMemoryPool
	{
		friend class GPUMemoryAllocator;
		friend class GPUMemoryRef;

	public:
		using MemoryLock = Sync::SharedSpinLock;

		gpu_memory_pool_id_t const mPoolID;
		gpu_memory_size_t const mPoolSize;
		GPUMemoryHeap * const mSourceHeap;
		gpu_memory_heap_id_t const mSourceHeapID;

	public:
		GPUMemoryPool();
		virtual ~GPUMemoryPool();

		IC3_ATTR_NO_DISCARD gpu_memory_size_t getCurrentUsage() const;

	protected:
		void setCurrentUsage( gpu_memory_size_t pUsageInBytes );

		MemoryLock & getMemoryLock();

	private:
		GPUMemoryRegion _sourceHeapRegion;
		std::atomic<gpu_memory_size_t> _currentUsage;
		MemoryLock _memoryLock;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_GPU_MEMORY_POOL_H__

