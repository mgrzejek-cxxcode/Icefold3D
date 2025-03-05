
#pragma once

#ifndef __IC3_GRAPHICS_GCI_COMMON_MEMORY_DEFS_H__
#define __IC3_GRAPHICS_GCI_COMMON_MEMORY_DEFS_H__

#include "../Prerequisites.h"
#include <Ic3/CoreLib/Memory/CommonMemoryDefs.h>
#include <cppx/byteArray.h>

namespace Ic3::Graphics::GCI
{

    class GPUMemoryHeap;
    class GPUMemoryPool;

    using gpu_memory_diff_t = int64;
	using gpu_memory_size_t = uint64;
	using gpu_memory_flags_value_t = uint32;

    using gpu_memory_heap_id_t = uint64;
    using gpu_memory_pool_id_t = uint64;

    using GPUMemoryRegion = cppx::region<gpu_memory_size_t>;
    using GPUMemoryRange = GPUMemoryRegion::range_type;

	/// @brief
	inline constexpr gpu_memory_size_t cxGPUMemoryOffsetInvalid = cppx::meta::limits<gpu_memory_size_t>::max_value;

	/// @brief
	inline constexpr gpu_memory_size_t cxGPUMemorySizeMax = cppx::meta::limits<gpu_memory_size_t>::max_value;

	/// @brief Flags representing various properties of host/device memory pools like access and heap properties.
	enum EGPUMemoryFlags : gpu_memory_flags_value_t
	{
		// Memory has a READ access granted to the CPU.
		eGPUMemoryAccessFlagCPUReadBit = 0x0001,

		// Memory has a WRITE access granted to the CPU.
		// Note: if a region of device-local memory has a WRITE access, but the READ access is (or may be) missing,
		// the app must ensure it does not read any content of the memory (including potential compiler-issued code).
		// This can cause a significant performance penalty.
		eGPUMemoryAccessFlagCPUWriteBit = 0x0002,

		// Memory has a READ access granted to the GPU.
		eGPUMemoryAccessFlagGPUReadBit = 0x0004,

		// Memory has a WRITE access granted to the GPU.
		eGPUMemoryAccessFlagGPUWriteBit = 0x0008,

		// A helper constant yielding both READ and WRITE CPU access.
		eGPUMemoryAccessMaskCPUReadWrite = eGPUMemoryAccessFlagCPUReadBit | eGPUMemoryAccessFlagCPUWriteBit,

		// Mask with all valid ACCESS_FLAG_CPU bits set.
		eGPUMemoryAccessMaskCPUAll = eGPUMemoryAccessMaskCPUReadWrite,

		// A helper constant yielding both READ and WRITE GPU access.
		eGPUMemoryAccessMaskGPUReadWrite = eGPUMemoryAccessFlagGPUReadBit | eGPUMemoryAccessFlagGPUWriteBit,

		// Mask with all valid ACCESS_FLAG_GPU bits set.
		eGPUMemoryAccessMaskGPUAll = eGPUMemoryAccessMaskGPUReadWrite,

		// Mask with all valid ACCESS_FLAG bits set.
		eGPUMemoryAccessMaskAll = eGPUMemoryAccessMaskCPUAll | eGPUMemoryAccessMaskGPUAll,

		// Memory heap is CPU-cached, which means all host-side writes are not visible to the device until
		// an explicit flush is performed on the modified memory range.
		eGPUMemoryHeapPropertyFlagCPUCachedBit = 0x0010,

		// Memory heap is CPU-coherent. All host accesses to this memory is automatically made visible to the device.
		// Mapped memory without this flag will require an explicit Flush() to make CPU writes visible to the GPU.
		eGPUMemoryHeapPropertyFlagCPUCoherentBit = 0x0020,

		// Memory heap is GPU-coherent. All device accesses to this memory is automatically made visible to the host.
		// Mapped memory without this flag will require an explicit Invalidate() to make GPU writes visible to the CPU.
		eGPUMemoryHeapPropertyFlagGPUCoherentBit = 0x0040,

		// Memory heap supports persistent mapping. It can be mapped once and such map pointer can be used as long as
		// the app needs to. Persistent mapping may require additional visibility control depending on caching/coherency.
		eGPUMemoryHeapPropertyFlagPersistentMapBit = 0x0080,

		// Mask with all valid HEAP_PROPERTY_FLAG bits set.
		eGPUMemoryHeapPropertyMaskAll = 0x00F0,
	};

	/// @brief
	enum EGPUMemoryMapFlags : gpu_memory_flags_value_t
	{
		// Memory is mapped with read-only permission.
		eGPUMemoryMapFlagAccessReadBit = eGPUMemoryAccessFlagCPUReadBit,

		// Memory is mapped with both read and write permissions.
		eGPUMemoryMapFlagAccessReadWriteBit = eGPUMemoryAccessMaskCPUReadWrite,

		// Resource is mapped with write-only permission.
		eGPUMemoryMapFlagAccessWriteBit = eGPUMemoryAccessFlagCPUWriteBit,

		// Resource is mapped with write-only permission. However, application must ensure, that sections of the
		// resource, that could be potentially in use, will not be affected. Write operations should be performed
		// only on uninitialized sections of the resource. Since driver assumes that guarantee, this access mode
		// is more efficient than E_GPU_MEMORY_MAP_FLAG_ACCESS_WRITE_BIT, because it does not involve a CPU<>GPU syncs.
		// Modifying sections which are in use is an undefined behaviour.
		eGPUMemoryMapFlagWriteAppendBit = 0x1000 | eGPUMemoryMapFlagAccessWriteBit,

		// Resource is mapped with write-only permission. After resource is mapped, all its data is discarded and
		// becomes undefined. This access can only be used in case of resources with dynamic usage.
		eGPUMemoryMapFlagWriteInvalidateBit = 0x2000 | eGPUMemoryMapFlagAccessWriteBit,
	};

	/// @brief
	enum class EGPUMemoryMapMode : uint32
	{
	    /// @see EGPUMemoryMapFlags::E_GPU_MEMORY_MAP_FLAG_ACCESS_READ_BIT
		ReadOnly = eGPUMemoryMapFlagAccessReadBit,

		/// @see EGPUMemoryMapFlags::E_GPU_MEMORY_MAP_FLAG_ACCESS_READ_WRITE_BIT
		ReadWrite = eGPUMemoryMapFlagAccessReadWriteBit,

		/// @see EGPUMemoryMapFlags::E_GPU_MEMORY_MAP_FLAG_ACCESS_WRITE_BIT
		WriteDefault = eGPUMemoryMapFlagAccessWriteBit,

		/// @see EGPUMemoryMapFlags::E_GPU_MEMORY_MAP_FLAG_WRITE_APPEND_BIT
		WriteAppend = eGPUMemoryMapFlagWriteAppendBit,

		/// @see EGPUMemoryMapFlags::E_GPU_MEMORY_MAP_FLAG_WRITE_INVALIDATE_BIT
		WriteInvalidate = eGPUMemoryMapFlagWriteInvalidateBit,
	};

	struct StructuredResourceAlignedMemoryMetrics
	{
		memory_size_t elementSize;
		memory_size_t elementSizeAligned;
		gpu_memory_size_t elementsNum;
		gpu_memory_size_t storageSize;
		gpu_memory_size_t storageSizeAligned;
	};

	struct StructuredResourceAlignedMemory
	{
		cppx::dynamic_byte_array alignedBuffer;
		StructuredResourceAlignedMemoryMetrics metrics;
	};

	struct ResourceMemoryInfo
	{
        memory_align_t baseAlignment;
		cppx::bitmask<EGPUMemoryFlags> memoryFlags;
		gpu_memory_heap_id_t sourceHeapID;
		GPUMemoryRegion sourceHeapRegion;
	};

	struct ResourceMappedMemory
	{
		const ResourceMemoryInfo * sourceMemory = nullptr;
		GPUMemoryRegion mappedRegion;
		void * pointer;
		cppx::bitmask<EGPUMemoryMapFlags> memoryMapFlags;

		constexpr explicit operator bool () const
		{
			return sourceMemory != nullptr;
		}
	};

	namespace GCU
	{

		/// @brief Returns true if the requested memory Map mode is valid for the memory with given properties (flags).
		IC3_GRAPHICS_GCI_API_NO_DISCARD bool MemCheckMapAccess( EGPUMemoryMapMode pRequestedMapMode, cppx::bitmask<EGPUMemoryFlags> pMemoryFlags );

		IC3_GRAPHICS_GCI_API_NO_DISCARD StructuredResourceAlignedMemory MemAlignStructuredResourceDataAuto(
				const void * pData,
				native_uint pElementSize,
				native_uint pElementsNum );

		IC3_GRAPHICS_GCI_API_NO_DISCARD cppx::dynamic_byte_array MemAlignStructuredResourceData(
				const void * pData,
				native_uint pElementSize,
				native_uint pElementsNum,
				memory_align_t pAlignedStride );

		IC3_GRAPHICS_GCI_API_NO_DISCARD cppx::dynamic_byte_array MemAlignStructuredResourceData(
				const void * pData,
				const StructuredResourceAlignedMemoryMetrics & pMetrics );

		/// @brief
		IC3_GRAPHICS_GCI_API_NO_DISCARD StructuredResourceAlignedMemoryMetrics MemComputeStructuredResourceAlignedMemoryMetrics(
				gpu_memory_size_t pElementSize,
				gpu_memory_size_t pElementsNum );

		/// @brief
		IC3_GRAPHICS_GCI_API_NO_DISCARD gpu_memory_size_t MemComputeStructuredResourceAlignedStride( gpu_memory_size_t pElementSize );

	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_COMMON_MEMORY_DEFS_H__
