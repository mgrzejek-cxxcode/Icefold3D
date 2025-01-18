
#ifndef __CPPX_UTILITIES_H__
#define __CPPX_UTILITIES_H__

#include "prerequisites.h"
#include <array>

namespace cppx
{

	template <typename TPFirst, typename TPSecond = TPFirst>
	inline constexpr typename std::common_type<TPFirst, TPSecond>::type get_max_of( const TPFirst & pFirst, const TPSecond & pSecond )
	{
		return ( pFirst >= pSecond ) ? pFirst : pSecond;
	}

	template <typename TP0, typename TP1, typename ...TPRest>
	inline constexpr typename std::common_type<TP0, TP1, TPRest...>::type get_max_of( const TP0 & p0, const TP1 & p1, TPRest && ...pRest )
	{
		return get_max_of( get_max_of( p0, p1 ), std::forward<TPRest>( pRest )... );
	}


	template <typename TPFirst, typename TPSecond = TPFirst>
	inline constexpr typename std::common_type<TPFirst, TPSecond>::type get_min_of( const TPFirst & pFirst, const TPSecond & pSecond )
	{
		return pFirst <= pSecond ? pFirst : pSecond;
	}

	template <typename TP0, typename TP1, typename ...TPRest>
	inline constexpr typename std::common_type<TP0, TP1, TPRest...>::type get_min_of( const TP0 & p0, const TP1 & p1, TPRest && ...pRest )
	{
		return get_min_of( get_min_of( p0, p1 ), std::forward<TPRest>( pRest )... );
	}


	template <typename TPClass, typename TPMember>
	inline uint32 member_offset( TPMember TPClass::* pMPtr )
	{
		return static_cast<uint32>( reinterpret_cast<byte *>( &( static_cast<TPClass *>( nullptr )->*pMPtr ) ) - static_cast<byte *>( nullptr ) );
	}


	template <typename TPValue, size_t tpSize, typename TPIndex>
	inline constexpr TPValue & static_array_element( TPValue( &pArray )[tpSize], TPIndex pIndex )
	{
		return pArray[static_cast<size_t>( pIndex )];
	}

	template <typename TPValue, size_t tpSize, typename TPIndex>
	inline constexpr const TPValue & static_array_element( const TPValue( &pArray )[tpSize], TPIndex pIndex )
	{
		return pArray[static_cast<size_t>( pIndex )];
	}

	template <typename TPValue, size_t tpSize, typename TPIndex>
	inline constexpr TPValue & static_array_element( std::array<TPValue, tpSize> & pArray, TPIndex pIndex )
	{
		return pArray[static_cast<size_t>( pIndex )];
	}

	template <typename TPValue, size_t tpSize, typename TPIndex>
	inline constexpr const TPValue & static_array_element( const std::array<TPValue, tpSize> & pArray, TPIndex pIndex )
	{
		return pArray[static_cast<size_t>( pIndex )];
	}

	template <typename TPValue, size_t tpSize, typename TPIndex>
	inline constexpr const TPValue & static_array_element_or_default( const TPValue( &pArray )[tpSize], TPIndex pIndex, const TPValue & pDefault )
	{
		return ( static_cast<size_t>( pIndex ) < tpSize ) ? pArray[static_cast<size_t>( pIndex )] : pDefault;
	}

	template <typename TPValue, size_t tpSize, typename TPIndex>
	inline constexpr const TPValue & static_array_element_or_default( const std::array<TPValue, tpSize> & pArray, TPIndex pIndex, const TPValue & pDefault )
	{
		return ( static_cast<size_t>( pIndex ) < tpSize ) ? pArray[static_cast<size_t>( pIndex )] : pDefault;
	}

	template <typename TPValue, size_t tpSize>
	inline constexpr size_t static_array_size( const TPValue( &pArray )[tpSize] )
	{
		return tpSize;
	}

	template <typename TPValue, size_t tpSize>
	inline constexpr size_t static_array_size( const std::array<TPValue, tpSize> & pArray )
	{
		return tpSize;
	}


	template <typename TPValue, size_t tpSize, typename TPOther>
	inline constexpr size_t copy_array_values( TPValue( &pTarget )[tpSize], const TPOther( &pSource )[tpSize], size_t pOffset = 0, size_t pCount = tpSize )
	{
		const auto offset = get_min_of( pOffset, tpSize );
		const auto count = get_min_of( pCount, tpSize - offset );

		for( size_t index = offset; index < offset + count; ++index )
		{
			pTarget[index] = pSource[index];
		}

		return count;
	}


	template <typename TPValue, size_t tpSize, typename TPOther>
	inline constexpr size_t set_array_values( TPValue( &pArray )[tpSize], TPOther pValue, size_t pOffset = 0, size_t pCount = tpSize )
	{
		const auto offset = get_min_of( pOffset, tpSize );
		const auto count = get_min_of( pCount, tpSize - offset );

		for( size_t index = offset; index < offset + count; ++index )
		{
			pArray[index] = pValue;
		}

		return count;
	}

	template <typename TPValue, size_t tpSize, typename TPOther>
	inline constexpr size_t set_array_values( std::array<TPValue, tpSize> & pArray, TPOther pValue, size_t pOffset = 0, size_t pCount = tpSize  )
	{
		const auto offset = get_min_of( pOffset, tpSize );
		const auto count = get_min_of( pCount, tpSize - offset );

		for( size_t index = offset; index < offset + count; ++index )
		{
			pArray[index] = pValue;
		}

		return count;
	}

	template <typename TPIterable, typename TPCallable>
	inline void foreach_in( TPIterable && pIterable, TPCallable pCallable )
	{
		for( auto && element : pIterable )
		{
			pCallable( element );
		}
	}

	template <typename TPIterable, typename TPValue, typename TPMemberFunc, typename... TPArgs>
	inline void foreach_in( TPIterable && pIterable, TPMemberFunc TPValue::* pMemberFunc, TPArgs && ...pArgs )
	{
		for( auto && element : pIterable )
		{
			( element.*pMemberFunc )( std::forward<TPArgs>( pArgs )... );
		}
	}

	template <typename TPIterator, typename TPValue, typename TPMemberFunc, typename... TPArgs>
	inline void foreach_in( TPIterator pBegin, TPIterator pEnd, TPMemberFunc TPValue::* pMemberFunc, TPArgs && ...pArgs )
	{
		for( auto elemIter = pBegin; elemIter != pEnd; ++elemIter )
		{
			( elemIter->*pMemberFunc )( std::forward<TPArgs>( pArgs )... );
		}
	}

	template <typename TPIterator>
	inline size_t count_non_null_pointers( TPIterator pBegin, TPIterator pEnd )
	{
		const auto numElements = std::count_if( pBegin, pEnd, []( auto && pValue ) -> bool { return !!pValue; } );
        return numElements;
	}

	template <typename TPIterator>
	inline size_t count_null_pointers( TPIterator pBegin, TPIterator pEnd )
	{
		const auto numElements = std::count_if( pBegin, pEnd, []( auto && pValue ) -> bool { return !pValue; } );
        return numElements;
	}

}

#endif // __CPPX_UTILITIES_H__
