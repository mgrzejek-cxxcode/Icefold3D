
#pragma once

#ifndef __IC3_COMMON_MEMORY_DEFS_H__
#define __IC3_COMMON_MEMORY_DEFS_H__

#include "../Prerequisites.h"
#include <Ic3/Cppx/Memory.h>
#include <Ic3/Cppx/Range.h>

namespace Ic3
{

    using memory_align_t = uint32;
    using memory_diff_t = native_int;
    using memory_size_t = native_uint;

	/// @brief
    using MemoryRegion = TRegion<memory_size_t>;

	/// @brief
    using MemoryRange = MemoryRegion::RangeType;

	/// @brief Represents an invalid memory offset, expressed as the maximum value of the memory_size_t type.
	constexpr memory_size_t cxMemoryOffsetInvalid = Cppx::QLimits<memory_size_t>::sMaxValue;

	/// @brief Represents maximum possible size of single block/region of memory.
	constexpr memory_size_t cxMemorySizeMax = Cppx::QLimits<memory_size_t>::sMaxValue;

	/// @brief
	struct AllocNewSizeExplicitTag
	{};

	/// @brief
	inline constexpr AllocNewSizeExplicitTag cvAllocNewSizeExplicit{};

}

inline void * operator new( size_t pSize, const Ic3::AllocNewSizeExplicitTag &, size_t pExtraSize )
{
	const auto requestedObjectSize = pSize + pExtraSize;
	const auto allocationSize = Ic3::Cppx::memGetAlignedValue( requestedObjectSize, Ic3::cxMemoryCPUDefaultAlignment );
	return std::malloc( allocationSize );
}

#endif // __IC3_COMMON_MEMORY_DEFS_H__
