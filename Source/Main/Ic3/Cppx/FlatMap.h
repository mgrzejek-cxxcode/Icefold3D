
#ifndef __IC3_CPPX_FLAT_MAP_H__
#define __IC3_CPPX_FLAT_MAP_H__

#include "SortedArray.h"

namespace Ic3::Cppx
{

	template < typename TPKey,
	           typename TPValue,
	           typename TPComparator = TCmpLessAuto<TPValue>,
	           typename TPAllocator = std::allocator<TPValue> >
	class FlatMap
	{
	public:
		using SelfType = FlatMap<TPValue, TPComparator, TPAllocator>;

		using KeyType = TPKey;
		using ValueType = TPValue;
		using CompareType = TPComparator;

		struct Element
		{
			TPKey mKey;
			TPValue mValue;
		};

		using UnderlyingContainerType = TSortedArray<Element, TPComparator, TPAllocator>;

		using Iterator = typename UnderlyingContainerType::iterator;
		using ConstIterator = typename UnderlyingContainerType::const_iterator;
		using ReverseIterator = typename UnderlyingContainerType::reverse_iterator;
		using ConstReverseIterator = typename UnderlyingContainerType::const_reverse_iterator;

	public:
		FlatMap( FlatMap && ) = default;
		FlatMap& operator=( FlatMap && ) = default;

		FlatMap( const FlatMap & ) = default;
		FlatMap& operator=( const FlatMap & ) = default;

		FlatMap() = default;

		explicit FlatMap( const TPAllocator & pAllocator )
		: _underlyingContainer( pAllocator )
		{}

		explicit FlatMap( size_t pCapacity, const TPAllocator & pAllocator = TPAllocator() )
		: _underlyingContainer( pCapacity, pAllocator )
		{}

		TPValue & operator[]( size_t pIndex )
		{
			return _underlyingContainer[pIndex];
		}

		IC3_ATTR_NO_DISCARD const TPValue & operator[]( size_t pIndex ) const
		{
			return _underlyingContainer[pIndex];
		}

	private:
		UnderlyingContainerType _underlyingContainer;
	};

}

#endif // __IC3_CPPX_FLAT_MAP_H__
