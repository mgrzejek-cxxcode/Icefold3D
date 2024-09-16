
#ifndef __IC3_CPPX_UTILITIES_H__
#define __IC3_CPPX_UTILITIES_H__

#include "Prerequisites.h"
#include <array>

namespace Ic3::Cppx
{

	template <typename TPFirst, typename TPSecond = TPFirst>
	inline constexpr typename std::common_type<TPFirst, TPSecond>::type getMaxOf( const TPFirst & pFirst, const TPSecond & pSecond )
	{
		return ( pFirst >= pSecond ) ? pFirst : pSecond;
	}

	template <typename TP0, typename TP1, typename ...TPRest>
	inline constexpr typename std::common_type<TP0, TP1, TPRest...>::type getMaxOf( const TP0 & p0, const TP1 & p1, TPRest && ...pRest )
	{
		return getMaxOf( getMaxOf( p0, p1 ), std::forward<TPRest>( pRest )... );
	}


	template <typename TPFirst, typename TPSecond = TPFirst>
	inline constexpr typename std::common_type<TPFirst, TPSecond>::type getMinOf( const TPFirst & pFirst, const TPSecond & pSecond )
	{
		return pFirst <= pSecond ? pFirst : pSecond;
	}

	template <typename TP0, typename TP1, typename ...TPRest>
	inline constexpr typename std::common_type<TP0, TP1, TPRest...>::type getMinOf( const TP0 & p0, const TP1 & p1, TPRest && ...pRest )
	{
		return getMinOf( getMinOf( p0, p1 ), std::forward<TPRest>( pRest )... );
	}


	template <typename TPClass, typename TPMember>
	inline uint32 memberOffset( TPMember TPClass::* pMPtr )
	{
		return static_cast<uint32>( reinterpret_cast<byte *>( &( static_cast<TPClass *>( nullptr )->*pMPtr ) ) - static_cast<byte *>( nullptr ) );
	}


	template <typename TPValue, size_t tpSize, typename TPIndex>
	inline constexpr TPValue & staticArrayElement( TPValue( &pArray )[tpSize], TPIndex pIndex )
	{
		return pArray[static_cast<size_t>( pIndex )];
	}

	template <typename TPValue, size_t tpSize, typename TPIndex>
	inline constexpr const TPValue & staticArrayElement( const TPValue( &pArray )[tpSize], TPIndex pIndex )
	{
		return pArray[static_cast<size_t>( pIndex )];
	}

	template <typename TPValue, size_t tpSize, typename TPIndex>
	inline constexpr TPValue & staticArrayElement( std::array<TPValue, tpSize> & pArray, TPIndex pIndex )
	{
		return pArray[static_cast<size_t>( pIndex )];
	}

	template <typename TPValue, size_t tpSize, typename TPIndex>
	inline constexpr const TPValue & staticArrayElement( const std::array<TPValue, tpSize> & pArray, TPIndex pIndex )
	{
		return pArray[static_cast<size_t>( pIndex )];
	}

	template <typename TPValue, size_t tpSize, typename TPIndex>
	inline constexpr const TPValue & staticArrayElementOrDefault( const TPValue( &pArray )[tpSize], TPIndex pIndex, const TPValue & pDefault )
	{
		return ( static_cast<size_t>( pIndex ) < tpSize ) ? pArray[static_cast<size_t>( pIndex )] : pDefault;
	}

	template <typename TPValue, size_t tpSize, typename TPIndex>
	inline constexpr const TPValue & staticArrayElementOrDefault( const std::array<TPValue, tpSize> & pArray, TPIndex pIndex, const TPValue & pDefault )
	{
		return ( static_cast<size_t>( pIndex ) < tpSize ) ? pArray[static_cast<size_t>( pIndex )] : pDefault;
	}

	template <typename TPValue, size_t tpSize>
	inline constexpr size_t staticArraySize( const TPValue( &pArray )[tpSize] )
	{
		return tpSize;
	}

	template <typename TPValue, size_t tpSize>
	inline constexpr size_t staticArraySize( const std::array<TPValue, tpSize> & pArray )
	{
		return tpSize;
	}


	template <typename TPValue, size_t tpSize, typename TPOther>
	inline constexpr size_t copyArrayValues( TPValue( &pTarget )[tpSize], const TPOther( &pSource )[tpSize], size_t pOffset = 0, size_t pCount = tpSize )
	{
		const auto offset = getMinOf( pOffset, tpSize );
		const auto count = getMinOf( pCount, tpSize - offset );

		for( size_t index = offset; index < offset + count; ++index )
		{
			pTarget[index] = pSource[index];
		}

		return count;
	}


	template <typename TPValue, size_t tpSize, typename TPOther>
	inline constexpr size_t setArrayValues( TPValue( &pArray )[tpSize], TPOther pValue, size_t pOffset = 0, size_t pCount = tpSize )
	{
		const auto offset = getMinOf( pOffset, tpSize );
		const auto count = getMinOf( pCount, tpSize - offset );

		for( size_t index = offset; index < offset + count; ++index )
		{
			pArray[index] = pValue;
		}

		return count;
	}

	template <typename TPValue, size_t tpSize, typename TPOther>
	inline constexpr size_t setArrayValues( std::array<TPValue, tpSize> & pArray, TPOther pValue, size_t pOffset = 0, size_t pCount = tpSize  )
	{
		const auto offset = getMinOf( pOffset, tpSize );
		const auto count = getMinOf( pCount, tpSize - offset );

		for( size_t index = offset; index < offset + count; ++index )
		{
			pArray[index] = pValue;
		}

		return count;
	}

	template <typename TPIterable, typename TPCallable>
	inline void foreachIn( TPIterable && pIterable, TPCallable pCallable )
	{
		for( auto && element : pIterable )
		{
			pCallable( element );
		}
	}

	template <typename TPIterable, typename TPValue, typename TPMemberFunc, typename... TPArgs>
	inline void foreachIn( TPIterable && pIterable, TPMemberFunc TPValue::* pMemberFunc, TPArgs && ...pArgs )
	{
		for( auto && element : pIterable )
		{
			( element.*pMemberFunc )( std::forward<TPArgs>( pArgs )... );
		}
	}

	template <typename TPIterator, typename TPValue, typename TPMemberFunc, typename... TPArgs>
	inline void foreachIn( TPIterator pBegin, TPIterator pEnd, TPMemberFunc TPValue::* pMemberFunc, TPArgs && ...pArgs )
	{
		for( auto elemIter = pBegin; elemIter != pEnd; ++elemIter )
		{
			( elemIter->*pMemberFunc )( std::forward<TPArgs>( pArgs )... );
		}
	}

	template <typename TPIterator>
	inline size_t countNonNullPointers( TPIterator pBegin, TPIterator pEnd )
	{
		const auto numElements = std::count_if( pBegin, pEnd, []( auto && pValue ) -> bool { return !!pValue; } );
        return numElements;
	}

	template <typename TPIterator>
	inline size_t countNullPointers( TPIterator pBegin, TPIterator pEnd )
	{
		const auto numElements = std::count_if( pBegin, pEnd, []( auto && pValue ) -> bool { return !pValue; } );
        return numElements;
	}

}

#endif // __IC3_CPPX_UTILITIES_H__
