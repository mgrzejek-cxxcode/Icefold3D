
#pragma once

#ifndef __IC3_GRAPHICS_GCI_GPU_MEMORY_HEAP_H__
#define __IC3_GRAPHICS_GCI_GPU_MEMORY_HEAP_H__

#include "CommonGPUMemoryDefs.h"

namespace Ic3::Graphics::GCI
{

	struct GPUMemoryPoolMetrics
	{
		memory_align_t baseAlignment;
		gpu_memory_size_t totalSizeBase;
		gpu_memory_size_t totalSizeMB;
	};

	struct GPUMemoryHeapProperties
	{
		gpu_memory_heap_id_t heapID;
		Bitmask<EGPUMemoryFlags> memoryFlags;
		GPUMemoryPoolMetrics heapMetrics;
	};

	class GPUMemoryHeap
	{
	public:
		GPUMemoryHeapProperties const mHeapProperties;

	public:
		GPUMemoryHeap();
		virtual ~GPUMemoryHeap();
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_GPU_MEMORY_HEAP_H__
