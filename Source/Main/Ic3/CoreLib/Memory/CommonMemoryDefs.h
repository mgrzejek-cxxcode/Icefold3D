
#pragma once

#ifndef __IC3_COMMON_MEMORY_DEFS_H__
#define __IC3_COMMON_MEMORY_DEFS_H__

#include "../Prerequisites.h"
#include <cppx/memory.h>
#include <cppx/range.h>

namespace Ic3
{

    using memory_align_t = uint32;
    using memory_diff_t = native_int;
    using memory_size_t = native_uint;

	/// @brief
    using MemoryRegion = cppx::region<memory_size_t>;

	/// @brief
    using MemoryRange = MemoryRegion::range_type;

	/// @brief Represents an invalid memory offset, expressed as the maximum value of the memory_size_t type.
	constexpr memory_size_t cxMemoryOffsetInvalid = cppx::meta::limits<memory_size_t>::max_value;

	/// @brief Represents maximum possible size of single block/region of memory.
	constexpr memory_size_t cxMemorySizeMax = cppx::meta::limits<memory_size_t>::max_value;

	/// @brief Default alignment value for CPU-side (RAM) memory allocation.
	inline constexpr uint32 cxMemoryCpuDefaultAlignment = PCL_MEMORY_BASE_ALIGNMENT;

	/// @brief Default alignment value for GPU-side (VideoRAM) memory allocation.
	inline constexpr uint32 cxMemoryGpuDefaultAlignment = 64;

	/// @brief
	struct AllocNewSizeExplicitTag
	{};

	/// @brief
	inline constexpr AllocNewSizeExplicitTag cvAllocNewSizeExplicit{};

}

inline void * operator new( size_t pSize, const Ic3::AllocNewSizeExplicitTag &, size_t pExtraSize )
{
	const auto requestedObjectSize = pSize + pExtraSize;
	const auto allocationSize = cppx::mem_get_aligned_value( requestedObjectSize, Ic3::cxMemoryCpuDefaultAlignment );
	return std::malloc( allocationSize );
}

#endif // __IC3_COMMON_MEMORY_DEFS_H__
