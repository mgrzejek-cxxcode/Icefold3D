
#pragma once

#ifndef __IC3_GRAPHICS_GCI_GPU_MEMORY_HEAP_H__
#define __IC3_GRAPHICS_GCI_GPU_MEMORY_HEAP_H__

#include "CommonGpuMemoryDefs.h"

namespace Ic3::Graphics::GCI
{

	struct GpuMemoryPoolMetrics
	{
		memory_align_t baseAlignment;
		gpu_memory_size_t totalSizeBase;
		gpu_memory_size_t totalSizeMB;
	};

	struct GpuMemoryHeapProperties
	{
		gpu_memory_heap_id_t heapID;
		cppx::bitmask<EGpuMemoryFlags> memoryFlags;
		GpuMemoryPoolMetrics mHeapMetrics;
	};

	class GpuMemoryHeap
	{
	public:
		GpuMemoryHeapProperties const mHeapProperties;

	public:
		GpuMemoryHeap();
		virtual ~GpuMemoryHeap();
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_GPU_MEMORY_HEAP_H__
