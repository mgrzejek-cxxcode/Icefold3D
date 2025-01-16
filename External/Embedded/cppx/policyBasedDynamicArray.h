
#ifndef __CPPX_POLICY_BASED_DYNAMIC_ARRAY_H__
#define __CPPX_POLICY_BASED_DYNAMIC_ARRAY_H__

#include "memoryBuffer.h"
#include "staticLimits.h"

namespace cppx
{

	template <size_t tpFixedCapacity, size_t tpResizeFactor, size_t tpMaximumCapacity = meta::limits<size_t>::max_value>
	struct dynamic_array_policy
	{
		static constexpr size_t fixed_capacity = tpFixedCapacity;
		static constexpr size_t resize_factor = tpResizeFactor;
		static constexpr size_t maximum_capacity = tpMaximumCapacity;
	};

	template <typename TPValue, typename TMemoryPolicy = dynamic_array_policy<0, 2>, typename TPAllocator = std::allocator<TPValue> >
	class dynamic_array_policy_based
	{
	private:
	};

}

#endif // __CPPX_POLICY_BASED_DYNAMIC_ARRAY_H__
