
#pragma once

#ifndef __IC3_GRAPHICS_GCI_GPU_MEMORY_HEAP_H__
#define __IC3_GRAPHICS_GCI_GPU_MEMORY_HEAP_H__

#include "CommonGPUMemoryDefs.h"

namespace Ic3::Graphics::GCI
{

	struct GPUMemoryPoolMetrics
	{
		memory_align_t mBaseAlignment;
		gpu_memory_size_t mTotalSizeBase;
		gpu_memory_size_t mTotalSizeMB;
	};

	struct GPUMemoryHeapProperties
	{
		gpu_memory_heap_id_t mHeapID;
		TBitmask<EGPUMemoryFlags> mMemoryFlags;
		GPUMemoryPoolMetrics mHeapMetrics;
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
