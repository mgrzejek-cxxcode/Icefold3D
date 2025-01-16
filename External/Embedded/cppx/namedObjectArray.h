
#ifndef __CPPX_NAMED_OBJECT_ARRAY_H__
#define __CPPX_NAMED_OBJECT_ARRAY_H__

#include "Hash.h"
#include "SortedArray.h"
#include <unordered_map>

namespace cppx
{

	template < typename TPValue,
	           typename TPChar = char,
	           typename TPName = std::basic_string_view<TPChar>,
	           typename TPComparator = cmp_less_auto<TPValue>,
	           typename TPAllocator = std::allocator<TPValue> >
	class NamedObjectArray
	{
	public:

	private:
		struct ValueType
		{
		};

		using IndexedArray = sorted_array<TPValue, TPComparator, TPAllocator>;
		using NameMap = std::unordered_map<TPName, uint32, hash_std_proxy<TPChar>>;

		IndexedArray _indexedArray;
		NameMap _nameMap;
	};

}

#endif // __CPPX_NAMED_OBJECT_ARRAY_H__
