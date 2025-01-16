
#ifndef __CPPX_SORTED_ARRAY_H__
#define __CPPX_SORTED_ARRAY_H__

#include "arrayView.h"
#include "staticAlgo.h"
#include "staticLimits.h"
#include <stdexcept>

namespace cppx
{

	template <
        typename TPValue,
		typename TPComparator = cmp_less_auto<TPValue>,
		typename TPAllocator = std::allocator<TPValue> >
	class sorted_array
	{
	public:
		using self_type = sorted_array<TPValue, TPComparator, TPAllocator>;

		using value_type = TPValue;
		using compare_type = TPComparator;

		using UnderlyingContainerType = std::vector<TPValue, TPAllocator>;

		using iterator = typename UnderlyingContainerType::iterator;
		using const_iterator = typename UnderlyingContainerType::const_iterator;
		using reverse_iterator = typename UnderlyingContainerType::reverse_iterator;
		using const_reverse_iterator = typename UnderlyingContainerType::const_reverse_iterator;

	public:
		sorted_array( sorted_array && ) = default;
		sorted_array& operator=( sorted_array && ) = default;

		sorted_array( const sorted_array & ) = default;
		sorted_array& operator=( const sorted_array & ) = default;

		sorted_array() = default;

		explicit sorted_array( const TPAllocator & pAllocator )
		: _underlyingContainer( pAllocator )
		{}

		explicit sorted_array( size_t pCapacity, const TPAllocator & pAllocator = TPAllocator() )
		: _underlyingContainer( pAllocator )
		{
			_underlyingContainer.reserve( pCapacity );
		}

		TPValue & operator[]( size_t pIndex )
		{
			return _underlyingContainer[pIndex];
		}

		CPPX_ATTR_NO_DISCARD const TPValue & operator[]( size_t pIndex ) const
		{
			return _underlyingContainer[pIndex];
		}

		CPPX_ATTR_NO_DISCARD TPValue & at( size_t pIndex )
		{
			return _underlyingContainer.at( pIndex );
		}

		CPPX_ATTR_NO_DISCARD const TPValue & at( size_t pIndex ) const
		{
			return _underlyingContainer.at( pIndex );
		}

		CPPX_ATTR_NO_DISCARD TPValue * data() noexcept
		{
			return _underlyingContainer.data();
		}

		CPPX_ATTR_NO_DISCARD const TPValue * data() const noexcept
		{
			return _underlyingContainer.data();
		}

		CPPX_ATTR_NO_DISCARD iterator begin()
		{
			return _underlyingContainer.begin();
		}

		CPPX_ATTR_NO_DISCARD const_iterator begin() const
		{
			return _underlyingContainer.begin();
		}

		CPPX_ATTR_NO_DISCARD iterator end()
		{
			return _underlyingContainer.end();
		}

		CPPX_ATTR_NO_DISCARD const_iterator end() const
		{
			return _underlyingContainer.end();
		}

		CPPX_ATTR_NO_DISCARD TPValue & front()
		{
			return _underlyingContainer.front();
		}

		CPPX_ATTR_NO_DISCARD const TPValue & front() const
		{
			return _underlyingContainer.front();
		}

		CPPX_ATTR_NO_DISCARD TPValue & back()
		{
			return _underlyingContainer.back();
		}

		CPPX_ATTR_NO_DISCARD const TPValue & back() const
		{
			return _underlyingContainer.back();
		}

		iterator insert( TPValue && pValue )
		{
			auto insertPosition = _get_insert_position( pValue, compare_type{} );
			auto insertIterator = _underlyingContainer.begin() + insertPosition;
			return _underlyingContainer.insert( insertIterator, std::move( pValue ) );
		}

		iterator insert( const TPValue & pValue )
		{
			auto insertPosition = _get_insert_position( pValue, compare_type{} );
			auto insertIterator = _underlyingContainer.begin() + insertPosition;
			return _underlyingContainer.insert( insertIterator, pValue );
		}

		CPPX_ATTR_NO_DISCARD iterator find( const TPValue & pValue )
		{
			auto elementPos = _find_lower( pValue, compare_type{} );
			if(( elementPos != cve::invalid_position ) && ( _underlyingContainer[elementPos] == pValue ) )
			{
				return _underlyingContainer.begin() + elementPos;
			}
			return _underlyingContainer.end();
		}

		CPPX_ATTR_NO_DISCARD const_iterator find( const TPValue & pValue ) const
		{
			auto elementPos = _find_lower( pValue, compare_type{} );
			if(( elementPos != cve::invalid_position ) && ( _underlyingContainer[elementPos] == pValue ) )
			{
				return _underlyingContainer.begin() + elementPos;
			}
			return _underlyingContainer.end();
		}

		template < typename TRef, typename TRefEqual = cmp_equal_auto<TPValue> >
		CPPX_ATTR_NO_DISCARD iterator find( const TRef & pValue, TRefEqual pRefEqual = TRefEqual{} )
		{
			auto elementPos = _find_lower( pValue, compare_type{} );
			if(( elementPos != cve::invalid_position ) && pRefEqual( _underlyingContainer[elementPos], pValue ) )
			{
				return _underlyingContainer.begin() + elementPos;
			}
			return _underlyingContainer.end();
		}

		template < typename TRef, typename TRefEqual = cmp_equal_auto<TPValue> >
		CPPX_ATTR_NO_DISCARD const_iterator find( const TRef & pValue, TRefEqual pRefEqual = TRefEqual{} ) const
		{
			auto elementPos = _find_lower( pValue, compare_type{} );
			if(( elementPos != cve::invalid_position ) && pRefEqual( _underlyingContainer[elementPos], pValue ) )
			{
				return _underlyingContainer.begin() + elementPos;
			}
			return _underlyingContainer.end();
		}

		template < typename TRef, typename TRefCompare = cmp_less_auto<TPValue>, typename TRefEqual = cmp_equal_auto<TPValue> >
		CPPX_ATTR_NO_DISCARD iterator find( const TRef & pValue, TRefCompare pRefCompare, TRefEqual pRefEqual )
		{
			auto elementPos = _find_lower( pValue, pRefCompare );
			if(( elementPos != cve::invalid_position ) && pRefEqual( _underlyingContainer[elementPos], pValue ) )
			{
				return _underlyingContainer.begin() + elementPos;
			}
			return _underlyingContainer.end();
		}

		template < typename TRef, typename TRefCompare = cmp_less_auto<TPValue>, typename TRefEqual = cmp_equal_auto<TPValue> >
		CPPX_ATTR_NO_DISCARD const_iterator find( const TRef & pValue, TRefCompare pRefCompare, TRefEqual pRefEqual ) const
		{
			auto elementPos = _find_lower( pValue, pRefCompare );
			if(( elementPos != cve::invalid_position ) && pRefEqual( _underlyingContainer[elementPos], pValue ) )
			{
				return _underlyingContainer.begin() + elementPos;
			}
			return _underlyingContainer.end();
		}

		CPPX_ATTR_NO_DISCARD bool hasValue( const TPValue & pValue ) const noexcept
		{
			auto elementPos = _find_lower( pValue, compare_type{} );
			return ( elementPos != cve::invalid_position ) && ( _underlyingContainer[elementPos] == pValue );
		}

		CPPX_ATTR_NO_DISCARD uint32 count( const TPValue & pValue ) const noexcept
		{
			auto firstValuePos = _find_lower( pValue, compare_type{} );
			if( firstValuePos != cve::invalid_position )
			{
				const auto occurrencesNum = std::count( _underlyingContainer.begin() + firstValuePos, _underlyingContainer.end(), pValue );
				return numeric_cast<uint32>( occurrencesNum );
			}
			return 0u;
		}

		iterator erase( iterator pIter )
		{
			return _underlyingContainer.erase( pIter );
		}

		iterator erase( iterator pStart, iterator pEnd )
		{
			return _underlyingContainer.erase( pStart, pEnd );
		}

		iterator remove( const TPValue & pValue )
		{
			auto rangeBeginPos = _find_lower( pValue, compare_type{} );
			if( rangeBeginPos != cve::invalid_position )
			{
				auto rangeEndPos = _find_upper( pValue, compare_type{} );
				const auto eraseFrom = _underlyingContainer.begin() + rangeBeginPos;
				const auto eraseTo = _underlyingContainer.begin() + rangeEndPos;
				return _underlyingContainer.erase( eraseFrom, eraseTo );
			}
			return _underlyingContainer.end();
		}

		iterator remove( const TPValue & pLeft, const TPValue & pRight )
		{
			auto rangeBeginPos = _find_lower( pLeft, compare_type{} );
			if( rangeBeginPos != cve::invalid_position )
			{
				auto rangeEndPos = _find_upper( pRight, compare_type{} );
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

			std::sort(
				_underlyingContainer.begin(),
				_underlyingContainer.end(),
				compare_type{} );
		}

		void setData( std::vector<TPValue> pValueArray )
		{
			_underlyingContainer = std::move( pValueArray );

			std::sort(
				_underlyingContainer.begin(),
				_underlyingContainer.end(),
				compare_type{} );
		}

		void clear()
		{
			_underlyingContainer.clear();
		}

		void reserve( size_t pCapacity )
		{
			_underlyingContainer.reserve( pCapacity );
		}

		CPPX_ATTR_NO_DISCARD size_t size() const
		{
			return _underlyingContainer.size();
		}

		CPPX_ATTR_NO_DISCARD bool empty() const
		{
			return _underlyingContainer.empty();
		}

		void swap( self_type & pOther )
		{
			std::swap( _underlyingContainer, pOther._underlyingContainer );
		}

	private:
		template <typename TRef, typename... TPred>
		size_t _get_insert_position( const TRef & pRefValue, TPred... pPredicate ) const
		{
			auto rangeBegin = _underlyingContainer.begin();
			auto rangeEnd = _underlyingContainer.end();
			auto resultIter = std::lower_bound( rangeBegin, rangeEnd, pRefValue, std::forward<TPred>( pPredicate )... );
			return ( resultIter != rangeEnd ) ? ( resultIter - rangeBegin ) : ( rangeEnd - rangeBegin );
		}

		template <typename TRef, typename... TPred>
		size_t _find_lower( const TRef & pRefValue, TPred... pPredicate ) const
		{
			auto rangeBegin = _underlyingContainer.begin();
			auto rangeEnd = _underlyingContainer.end();
			auto resultIter = std::lower_bound( rangeBegin, rangeEnd, pRefValue, std::forward<TPred>( pPredicate )... );
			return ( resultIter != rangeEnd ) ? ( resultIter - rangeBegin ) : cve::invalid_position;
		}

		template <typename TRef, typename... TPred>
		size_t _find_upper( const TRef & pRefValue, TPred... pPredicate ) const
		{
			auto rangeBegin = _underlyingContainer.begin();
			auto rangeEnd = _underlyingContainer.end();
			auto resultIter = std::upper_bound( rangeBegin, rangeEnd, pRefValue, std::forward<TPred>( pPredicate )... );
			return ( resultIter != rangeEnd ) ? ( resultIter - rangeBegin ) : cve::invalid_position;
		}

	private:
		UnderlyingContainerType _underlyingContainer;
	};

	template <typename TPValue, typename TPComparator, typename TPAllocator>
	inline typename sorted_array<TPValue, TPComparator, TPAllocator>::iterator begin( sorted_array<TPValue, TPComparator, TPAllocator> & pContainer )
	{
		return pContainer.begin();
	}

	template <typename TPValue, typename TPComparator, typename TPAllocator>
	inline typename sorted_array<TPValue, TPComparator, TPAllocator>::Constiterator begin( const sorted_array<TPValue, TPComparator, TPAllocator> & pContainer )
	{
		return pContainer.begin();
	}

	template <typename TPValue, typename TPComparator, typename TPAllocator>
	inline typename sorted_array<TPValue, TPComparator, TPAllocator>::iterator end( sorted_array<TPValue, TPComparator, TPAllocator> & pContainer )
	{
		return pContainer.end();
	}

	template <typename TPValue, typename TPComparator, typename TPAllocator>
	inline typename sorted_array<TPValue, TPComparator, TPAllocator>::Constiterator end( const sorted_array<TPValue, TPComparator, TPAllocator> & pContainer )
	{
		return pContainer.end();
	}

	template <typename TPValue, typename TPComparator, typename TPAllocator>
	CPPX_ATTR_NO_DISCARD inline array_view<TPValue> bind_array_view( sorted_array<TPValue, TPComparator, TPAllocator> & pContainer )
	{
		return array_view<TPValue>( pContainer.data(), pContainer.size() );
	}

	template <typename TPValue, typename TPComparator, typename TPAllocator>
	CPPX_ATTR_NO_DISCARD inline array_view<const TPValue> bind_array_view( const sorted_array<TPValue, TPComparator, TPAllocator> & pContainer )
	{
		return array_view<const TPValue>( pContainer.data(), pContainer.size() );
	}

}

#endif // __CPPX_SORTED_ARRAY_H__
