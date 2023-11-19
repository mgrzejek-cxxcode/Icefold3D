
#ifndef __IC3_CPPX_POLICY_BASED_DYNAMIC_ARRAY_H__
#define __IC3_CPPX_POLICY_BASED_DYNAMIC_ARRAY_H__

#include "StaticLimits.h"
#include "MemoryBuffer.h"
#include <variant>

namespace Ic3
{

	template <size_t tFixedCapacity, size_t tResizeFactor, size_t tMaximumCapacity = QLimits<size_t>::maxValue>
	struct DynamicArrayPolicy
	{
		static constexpr size_t sFixedCapacity = tFixedCapacity;
		static constexpr size_t sResizeFactor = tResizeFactor;
		static constexpr size_t sMaximumCapacity = tMaximumCapacity;
	};

	template <typename TValue, typename TMemoryPolicy = DynamicArrayPolicy<0, 2>, typename TAlloc = std::allocator<TValue> >
	class PolicyBasedDynamicArray
	{
	private:

	};

}

#endif // __IC3_CPPX_POLICY_BASED_DYNAMIC_ARRAY_H__
