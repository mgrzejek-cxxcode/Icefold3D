
#ifndef __IC3_CPPX_SORTED_ARRAY_H__
#define __IC3_CPPX_SORTED_ARRAY_H__

#include "StaticAlgo.h"
#include "StaticLimits.h"
#include <stdexcept>

namespace Ic3::Cppx
{

	template < typename TPValue,
	           typename TPComparator = TCmpLessAuto<TPValue>,
	           typename TPAllocator = std::allocator<TPValue> >
	class TSortedArray
	{
	public:
		using SelfType = TSortedArray<TPValue, TPComparator, TPAllocator>;

		using ValueType = TPValue;
		using CompareType = TPComparator;

		using UnderlyingContainerType = std::vector<TPValue, TPAllocator>;

		using Iterator = typename UnderlyingContainerType::iterator;
		using ConstIterator = typename UnderlyingContainerType::const_iterator;
		using ReverseIterator = typename UnderlyingContainerType::reverse_iterator;
		using ConstReverseIterator = typename UnderlyingContainerType::const_reverse_iterator;

	public:
		TSortedArray( TSortedArray && ) = default;
		TSortedArray& operator=( TSortedArray && ) = default;

		TSortedArray( const TSortedArray & ) = default;
		TSortedArray& operator=( const TSortedArray & ) = default;

		TSortedArray() = default;

		explicit TSortedArray( const TPAllocator & pAllocator )
		: _underlyingContainer( pAllocator )
		{}

		explicit TSortedArray( size_t pCapacity, const TPAllocator & pAllocator = TPAllocator() )
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

		IC3_ATTR_NO_DISCARD TPValue & at( size_t pIndex )
		{
			return _underlyingContainer.at( pIndex );
		}

		IC3_ATTR_NO_DISCARD const TPValue & at( size_t pIndex ) const
		{
			return _underlyingContainer.at( pIndex );
		}

		IC3_ATTR_NO_DISCARD TPValue * data() noexcept
		{
			return _underlyingContainer.data();
		}

		IC3_ATTR_NO_DISCARD const TPValue * data() const noexcept
		{
			return _underlyingContainer.data();
		}

		IC3_ATTR_NO_DISCARD Iterator begin()
		{
			return _underlyingContainer.begin();
		}

		IC3_ATTR_NO_DISCARD ConstIterator begin() const
		{
			return _underlyingContainer.begin();
		}

		IC3_ATTR_NO_DISCARD Iterator end()
		{
			return _underlyingContainer.end();
		}

		IC3_ATTR_NO_DISCARD ConstIterator end() const
		{
			return _underlyingContainer.end();
		}

		IC3_ATTR_NO_DISCARD TPValue & front()
		{
			return _underlyingContainer.front();
		}

		IC3_ATTR_NO_DISCARD const TPValue & front() const
		{
			return _underlyingContainer.front();
		}

		IC3_ATTR_NO_DISCARD TPValue & back()
		{
			return _underlyingContainer.back();
		}

		IC3_ATTR_NO_DISCARD const TPValue & back() const
		{
			return _underlyingContainer.back();
		}

		Iterator insert( TPValue && pValue )
		{
			auto insertPosition = _getInsertPosition( pValue, CompareType{} );
			auto insertIterator = _underlyingContainer.begin() + insertPosition;
			return _underlyingContainer.insert( insertIterator, std::move( pValue ) );
		}

		Iterator insert( const TPValue & pValue )
		{
			auto insertPosition = _getInsertPosition( pValue, CompareType{} );
			auto insertIterator = _underlyingContainer.begin() + insertPosition;
			return _underlyingContainer.insert( insertIterator, pValue );
		}

		IC3_ATTR_NO_DISCARD Iterator find( const TPValue & pValue )
		{
			auto elementPos = _findLower( pValue, CompareType{} );
			if( ( elementPos != cxInvalidPosition ) && ( _underlyingContainer[elementPos] == pValue ) )
			{
				return _underlyingContainer.begin() + elementPos;
			}
			return _underlyingContainer.end();
		}

		IC3_ATTR_NO_DISCARD ConstIterator find( const TPValue & pValue ) const
		{
			auto elementPos = _findLower( pValue, CompareType{} );
			if( ( elementPos != cxInvalidPosition ) && ( _underlyingContainer[elementPos] == pValue ) )
			{
				return _underlyingContainer.begin() + elementPos;
			}
			return _underlyingContainer.end();
		}

		template < typename TRef, typename TRefEqual = TCmpEqualAuto<TPValue> >
		IC3_ATTR_NO_DISCARD Iterator find( const TRef & pValue, TRefEqual pRefEqual = TRefEqual{} )
		{
			auto elementPos = _findLower( pValue, CompareType{} );
			if( ( elementPos != cxInvalidPosition ) && pRefEqual( _underlyingContainer[elementPos], pValue ) )
			{
				return _underlyingContainer.begin() + elementPos;
			}
			return _underlyingContainer.end();
		}

		template < typename TRef, typename TRefEqual = TCmpEqualAuto<TPValue> >
		IC3_ATTR_NO_DISCARD ConstIterator find( const TRef & pValue, TRefEqual pRefEqual = TRefEqual{} ) const
		{
			auto elementPos = _findLower( pValue, CompareType{} );
			if( ( elementPos != cxInvalidPosition ) && pRefEqual( _underlyingContainer[elementPos], pValue ) )
			{
				return _underlyingContainer.begin() + elementPos;
			}
			return _underlyingContainer.end();
		}

		template < typename TRef, typename TRefCompare = TCmpLessAuto<TPValue>, typename TRefEqual = TCmpEqualAuto<TPValue> >
		IC3_ATTR_NO_DISCARD Iterator find( const TRef & pValue, TRefCompare pRefCompare, TRefEqual pRefEqual )
		{
			auto elementPos = _findLower( pValue, pRefCompare );
			if( ( elementPos != cxInvalidPosition ) && pRefEqual( _underlyingContainer[elementPos], pValue ) )
			{
				return _underlyingContainer.begin() + elementPos;
			}
			return _underlyingContainer.end();
		}

		template < typename TRef, typename TRefCompare = TCmpLessAuto<TPValue>, typename TRefEqual = TCmpEqualAuto<TPValue> >
		IC3_ATTR_NO_DISCARD ConstIterator find( const TRef & pValue, TRefCompare pRefCompare, TRefEqual pRefEqual ) const
		{
			auto elementPos = _findLower( pValue, pRefCompare );
			if( ( elementPos != cxInvalidPosition ) && pRefEqual( _underlyingContainer[elementPos], pValue ) )
			{
				return _underlyingContainer.begin() + elementPos;
			}
			return _underlyingContainer.end();
		}

		Iterator erase( Iterator pIter )
		{
			return _underlyingContainer.erase( pIter );
		}

		Iterator erase( Iterator pStart, Iterator pEnd )
		{
			return _underlyingContainer.erase( pStart, pEnd );
		}

		Iterator remove( const TPValue & pValue )
		{
			auto rangeBeginPos = _findLower( pValue, CompareType{} );
			if( rangeBeginPos != cxInvalidPosition )
			{
				auto rangeEndPos = _findUpper( pValue, CompareType{} );
				const auto eraseFrom = _underlyingContainer.begin() + rangeBeginPos;
				const auto eraseTo = _underlyingContainer.begin() + rangeEndPos;
				return _underlyingContainer.erase( eraseFrom, eraseTo );
			}
			return _underlyingContainer.end();
		}

		Iterator remove( const TPValue & pLeft, const TPValue & pRight )
		{
			auto rangeBeginPos = _findLower( pLeft, CompareType{} );
			if( rangeBeginPos != cxInvalidPosition )
			{
				auto rangeEndPos = _findUpper( pRight, CompareType{} );
				if( rangeBeginPos <= rangeEndPos )
				{
					const auto eraseFrom = _underlyingContainer.begin() + rangeBeginPos;
					const auto eraseTo = _underlyingContainer.begin() + rangeEndPos;
					return _underlyingContainer.erase( eraseFrom, eraseTo );
				}
			}
			return _underlyingContainer.end();
		}

		void insertData( const std::vector<TPValue> & pValueArray )
		{
			_underlyingContainer.insert( _underlyingContainer.end(), pValueArray.begin(), pValueArray.end() );

			std::sort( _underlyingContainer.begin(),
			           _underlyingContainer.end(),
			           CompareType{} );
		}

		void setData( std::vector<TPValue> pValueArray )
		{
			_underlyingContainer = std::move( pValueArray );

			std::sort( _underlyingContainer.begin(),
			           _underlyingContainer.end(),
			           CompareType{} );
		}

		void clear()
		{
			_underlyingContainer.clear();
		}

		void reserve( size_t pCapacity )
		{
			_underlyingContainer.reserve( pCapacity );
		}

		IC3_ATTR_NO_DISCARD size_t size() const
		{
			return _underlyingContainer.size();
		}

		IC3_ATTR_NO_DISCARD bool empty() const
		{
			return _underlyingContainer.empty();
		}

		void swap( SelfType & pOther )
		{
			std::swap( _underlyingContainer, pOther._underlyingContainer );
		}

	private:
		template <typename TRef, typename... TPred>
		size_t _getInsertPosition( const TRef & pRefValue, TPred... pPredicate ) const
		{
			auto rangeBegin = _underlyingContainer.begin();
			auto rangeEnd = _underlyingContainer.end();
			auto resultIter = std::lower_bound( rangeBegin, rangeEnd, pRefValue, std::forward<TPred>( pPredicate )... );
			return ( resultIter != rangeEnd ) ? ( resultIter - rangeBegin ) : ( rangeEnd - rangeBegin );
		}

		template <typename TRef, typename... TPred>
		size_t _findLower( const TRef & pRefValue, TPred... pPredicate ) const
		{
			auto rangeBegin = _underlyingContainer.begin();
			auto rangeEnd = _underlyingContainer.end();
			auto resultIter = std::lower_bound( rangeBegin, rangeEnd, pRefValue, std::forward<TPred>( pPredicate )... );
			return ( resultIter != rangeEnd ) ? ( resultIter - rangeBegin ) : cxInvalidPosition;
		}

		template <typename TRef, typename... TPred>
		size_t _findUpper( const TRef & pRefValue, TPred... pPredicate ) const
		{
			auto rangeBegin = _underlyingContainer.begin();
			auto rangeEnd = _underlyingContainer.end();
			auto resultIter = std::upper_bound( rangeBegin, rangeEnd, pRefValue, std::forward<TPred>( pPredicate )... );
			return ( resultIter != rangeEnd ) ? ( resultIter - rangeBegin ) : cxInvalidPosition;
		}

	private:
		UnderlyingContainerType _underlyingContainer;
	};

	template <typename TPValue, typename TPComparator, typename TPAllocator>
	inline typename TSortedArray<TPValue, TPComparator, TPAllocator>::Iterator begin( TSortedArray<TPValue, TPComparator, TPAllocator> & pContainer )
	{
		return pContainer.begin();
	}

	template <typename TPValue, typename TPComparator, typename TPAllocator>
	inline typename TSortedArray<TPValue, TPComparator, TPAllocator>::ConstIterator begin( const TSortedArray<TPValue, TPComparator, TPAllocator> & pContainer )
	{
		return pContainer.begin();
	}

	template <typename TPValue, typename TPComparator, typename TPAllocator>
	inline typename TSortedArray<TPValue, TPComparator, TPAllocator>::Iterator end( TSortedArray<TPValue, TPComparator, TPAllocator> & pContainer )
	{
		return pContainer.end();
	}

	template <typename TPValue, typename TPComparator, typename TPAllocator>
	inline typename TSortedArray<TPValue, TPComparator, TPAllocator>::ConstIterator end( const TSortedArray<TPValue, TPComparator, TPAllocator> & pContainer )
	{
		return pContainer.end();
	}

}

#endif // __IC3_CPPX_SORTED_ARRAY_H__
