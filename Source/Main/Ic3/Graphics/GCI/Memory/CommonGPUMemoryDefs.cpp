
#include "CommonGPUMemoryDefs.h"

namespace Ic3::Graphics::GCI
{

	namespace Utilities
	{

		bool MemCheckMapAccess( EGPUMemoryMapMode pRequestedMapMode, cppx::bitmask<EGPUMemoryFlags> pMemoryFlags )
		{
			auto mapRequestedAccessFlags = static_cast<uint32>( pRequestedMapMode ) & eGPUMemoryMapFlagAccessReadWriteBit;
			return pMemoryFlags.is_set( mapRequestedAccessFlags );
		}

		StructuredResourceAlignedMemory MemAlignStructuredResourceDataAuto(
				const void * pData,
				native_uint pElementSize,
				native_uint pElementsNum )
		{
			StructuredResourceAlignedMemory resourceMemory;
			resourceMemory.metrics = MemComputeStructuredResourceAlignedMemoryMetrics( pElementSize, pElementsNum );
			resourceMemory.alignedBuffer = MemAlignStructuredResourceData( pData, resourceMemory.metrics );

			return resourceMemory;
		}

		cppx::dynamic_byte_array MemAlignStructuredResourceData(
				const void * pData,
				native_uint pElementSize,
				native_uint pElementsNum,
				memory_align_t pAlignedStride )
		{
			StructuredResourceAlignedMemoryMetrics memoryMetrics;
			memoryMetrics.elementSize = pElementSize;
			memoryMetrics.elementSizeAligned = pAlignedStride;
			memoryMetrics.elementsNum = pElementsNum;
			memoryMetrics.storageSize = pElementsNum * pElementSize;
			memoryMetrics.storageSizeAligned = pElementsNum * pAlignedStride;

			return MemAlignStructuredResourceData( pData, memoryMetrics );
		}

		cppx::dynamic_byte_array MemAlignStructuredResourceData(
				const void * pData,
				const StructuredResourceAlignedMemoryMetrics & pMetrics )
		{
			Ic3DebugAssert( pMetrics.elementSizeAligned >= pMetrics.elementSize );

			const auto paddingSize = pMetrics.elementSizeAligned - pMetrics.elementSize;

			cppx::dynamic_byte_array alignedData;
			alignedData.resize( pMetrics.storageSizeAligned );

			auto byteView = cppx::bind_memory_view( pData, pMetrics.storageSize );
			const auto * srcPtr = byteView.data();
			auto * targetPtr = alignedData.data();

			for( native_uint elementIndex = 0; elementIndex < pMetrics.elementsNum; ++elementIndex )
			{
				cppx::mem_copy_unchecked( targetPtr, pMetrics.elementSizeAligned, srcPtr, pMetrics.elementSize );
				cppx::mem_set_fill_unchecked( targetPtr + pMetrics.elementSize, pMetrics.elementSizeAligned, 0, paddingSize );

				srcPtr += pMetrics.elementSize;
				targetPtr += pMetrics.storageSizeAligned;
			}

			return alignedData;
		}

		StructuredResourceAlignedMemoryMetrics MemComputeStructuredResourceAlignedMemoryMetrics(
				gpu_memory_size_t pElementSize,
				gpu_memory_size_t pElementsNum )
		{
			StructuredResourceAlignedMemoryMetrics memoryMetrics;
			memoryMetrics.elementSize = pElementSize;
			memoryMetrics.elementsNum = pElementsNum;
			memoryMetrics.elementSizeAligned = MemComputeStructuredResourceAlignedStride( pElementSize );
			memoryMetrics.storageSize = pElementSize * pElementsNum;
			memoryMetrics.storageSizeAligned = memoryMetrics.elementSizeAligned * pElementsNum;

			return memoryMetrics;
		}

		gpu_memory_size_t MemComputeStructuredResourceAlignedStride( gpu_memory_size_t pElementSize )
		{
			return cppx::mem_get_aligned_value( pElementSize, 16 );
		}

	}

}
