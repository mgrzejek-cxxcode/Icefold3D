
#ifndef __CPPX_FLAT_MAP_H__
#define __CPPX_FLAT_MAP_H__

#include "SortedArray.h"

namespace cppx
{

	template < typename TPKey,
	           typename TPValue,
	           typename TPComparator = cmp_less_auto<TPValue>,
	           typename TPAllocator = std::allocator<TPValue> >
	class flat_map
	{
	public:
		using self_type = flat_map<TPValue, TPComparator, TPAllocator>;
		using key_type = TPKey;
		using value_type = TPValue;
		using compare_type = TPComparator;

		struct element
		{
			TPKey key;
			TPValue value;
		};

		using underlying_container_type = sorted_array<element, TPComparator, TPAllocator>;

		using iterator = typename underlying_container_type::iterator;
		using const_iterator = typename underlying_container_type::const_iterator;
		using reverse_iterator = typename underlying_container_type::reverse_iterator;
		using const_reverse_iterator = typename underlying_container_type::const_reverse_iterator;

	public:
		flat_map( flat_map && ) = default;
		flat_map& operator=( flat_map && ) = default;

		flat_map( const flat_map & ) = default;
		flat_map& operator=( const flat_map & ) = default;

		flat_map() = default;

		explicit flat_map( const TPAllocator & pAllocator )
		: _underlyingContainer( pAllocator )
		{}

		explicit flat_map( size_t pCapacity, const TPAllocator & pAllocator = TPAllocator() )
		: _underlyingContainer( pCapacity, pAllocator )
		{}

		TPValue & operator[]( size_t pIndex )
		{
			return _underlyingContainer[pIndex];
		}

		CPPX_ATTR_NO_DISCARD const TPValue & operator[]( size_t pIndex ) const
		{
			return _underlyingContainer[pIndex];
		}

	private:
		underlying_container_type _underlyingContainer;
	};

}

#endif // __CPPX_FLAT_MAP_H__
