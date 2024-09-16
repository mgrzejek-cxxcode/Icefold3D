
#ifndef __IC3_CPPX_POLICY_BASED_DYNAMIC_ARRAY_H__
#define __IC3_CPPX_POLICY_BASED_DYNAMIC_ARRAY_H__

#include "StaticLimits.h"
#include "MemoryBuffer.h"
#include <variant>

namespace Ic3::Cppx
{

	template <size_t tpFixedCapacity, size_t tpResizeFactor, size_t tpMaximumCapacity = QLimits<size_t>::sMaxValue>
	struct DynamicArrayPolicy
	{
		static constexpr size_t sFixedCapacity = tpFixedCapacity;
		static constexpr size_t sResizeFactor = tpResizeFactor;
		static constexpr size_t sMaximumCapacity = tpMaximumCapacity;
	};

	template <typename TPValue, typename TMemoryPolicy = DynamicArrayPolicy<0, 2>, typename TPAllocator = std::allocator<TPValue> >
	class PolicyBasedDynamicArray
	{
	private:
	};

}

#endif // __IC3_CPPX_POLICY_BASED_DYNAMIC_ARRAY_H__
