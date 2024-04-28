
#ifndef __IC3_CPPX_FLAT_MAP_H__
#define __IC3_CPPX_FLAT_MAP_H__

#include "SortedArray.h"

namespace Ic3::Cppx
{

	template < typename TKey,
	           typename TValue,
	           typename TCompare = CmpLessT<TValue>,
	           typename TAlloc = std::allocator<TValue> >
	class FlatMap
	{
	public:
		using SelfType = FlatMap<TValue, TCompare, TAlloc>;

		using KeyType = TKey;
		using ValueType = TValue;
		using CompareType = TCompare;

		struct SElement
		{
			TKey key;
			TValue value;
		};

		using UnderlyingContainerType = SortedArray<SElement, TCompare, TAlloc>;

		using Iterator = typename UnderlyingContainerType::iterator;
		using ConstIterator = typename UnderlyingContainerType::const_iterator;
		using ReverseIterator = typename UnderlyingContainerType::reverse_iterator;
		using ConstReverseIterator = typename UnderlyingContainerType::const_reverse_iterator;

	public:
		FlatMap( FlatMap && ) = default;
		FlatMap& operator=( FlatMap && ) = default;

		FlatMap( const FlatMap & ) = default;
		FlatMap& operator=( const FlatMap & ) = default;

		FlatMap()
		{}

		explicit FlatMap( const TAlloc & pAllocator )
		: _underlyingContainer( pAllocator )
		{}

		explicit FlatMap( size_t pCapacity, const TAlloc & pAllocator = TAlloc() )
		: _underlyingContainer( pCapacity, pAllocator )
		{}

		TValue & operator[]( size_t pIndex )
		{
			return _underlyingContainer[pIndex];
		}

		IC3_ATTR_NO_DISCARD const TValue & operator[]( size_t pIndex ) const
		{
			return _underlyingContainer[pIndex];
		}

	private:
		UnderlyingContainerType _underlyingContainer;
	};

}

#endif // __IC3_CPPX_FLAT_MAP_H__
