
#pragma once

#ifndef __IC3_GRAPHICS_GCI_GPU_MEMORY_POOL_H__
#define __IC3_GRAPHICS_GCI_GPU_MEMORY_POOL_H__

#include "CommonGpuMemoryDefs.h"
#include <cppx/sync/spinLock.h>

namespace Ic3::Graphics::GCI
{

	class GpuMemoryAllocator;
	class GpuMemoryHeap;
	class GpuMemoryRef;

	class GpuMemoryPool
	{
		friend class GpuMemoryAllocator;
		friend class GpuMemoryRef;

	public:
		using MemoryLock = cppx::sync::shared_spin_lock;

		gpu_memory_pool_id_t const mPoolID;
		gpu_memory_size_t const mPoolSize;
		GpuMemoryHeap * const mSourceHeap;
		gpu_memory_heap_id_t const mSourceHeapID;

	public:
		GpuMemoryPool();
		virtual ~GpuMemoryPool();

		CPPX_ATTR_NO_DISCARD gpu_memory_size_t GetCurrentUsage() const;

	protected:
		void SetCurrentUsage( gpu_memory_size_t pUsageInBytes );

		MemoryLock & GetMemoryLock();

	private:
		GpuMemoryRegion _sourceHeapRegion;
		std::atomic<gpu_memory_size_t> _currentUsage;
		MemoryLock _memoryLock;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_GPU_MEMORY_POOL_H__

