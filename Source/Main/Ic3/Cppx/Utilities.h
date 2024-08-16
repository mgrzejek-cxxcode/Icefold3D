
#ifndef __IC3_CPPX_UTILITIES_H__
#define __IC3_CPPX_UTILITIES_H__

#include "Prerequisites.h"
#include <array>

namespace Ic3::Cppx
{

	template <typename TFirst, typename TSecond = TFirst>
	inline constexpr typename std::common_type<TFirst, TSecond>::type getMaxOf( const TFirst & pFirst, const TSecond & pSecond )
	{
		return ( pFirst >= pSecond ) ? pFirst : pSecond;
	}

	template <typename T0, typename T1, typename ...TRest>
	inline constexpr typename std::common_type<T0, T1, TRest...>::type getMaxOf( const T0 & p0, const T1 & p1, TRest && ...pRest )
	{
		return getMaxOf( getMaxOf( p0, p1 ), std::forward<TRest>( pRest )... );
	}


	template <typename TFirst, typename TSecond = TFirst>
	inline constexpr typename std::common_type<TFirst, TSecond>::type getMinOf( const TFirst & pFirst, const TSecond & pSecond )
	{
		return pFirst <= pSecond ? pFirst : pSecond;
	}

	template <typename T0, typename T1, typename ...TRest>
	inline constexpr typename std::common_type<T0, T1, TRest...>::type getMinOf( const T0 & p0, const T1 & p1, TRest && ...pRest )
	{
		return getMinOf( getMinOf( p0, p1 ), std::forward<TRest>( pRest )... );
	}


	template <typename TClass, typename TMember>
	inline uint32 memberOffset( TMember TClass::* pMptr )
	{
		return static_cast<uint32>( reinterpret_cast<byte *>( &( static_cast<TClass *>( nullptr )->*pMptr ) ) - static_cast<byte *>( nullptr ) );
	}


	template <typename TVal, size_t tSize, typename TIndex>
	inline constexpr TVal & staticArrayElement( TVal( &pArray )[tSize], TIndex pIndex )
	{
		return pArray[static_cast<size_t>( pIndex )];
	}

	template <typename TVal, size_t tSize, typename TIndex>
	inline constexpr const TVal & staticArrayElement( const TVal( &pArray )[tSize], TIndex pIndex )
	{
		return pArray[static_cast<size_t>( pIndex )];
	}

	template <typename TVal, size_t tSize, typename TIndex>
	inline constexpr TVal & staticArrayElement( std::array<TVal, tSize> & pArray, TIndex pIndex )
	{
		return pArray[static_cast<size_t>( pIndex )];
	}

	template <typename TVal, size_t tSize, typename TIndex>
	inline constexpr const TVal & staticArrayElement( const std::array<TVal, tSize> & pArray, TIndex pIndex )
	{
		return pArray[static_cast<size_t>( pIndex )];
	}

	template <typename TVal, size_t tSize, typename TIndex>
	inline constexpr const TVal & staticArrayElementOrDefault( const TVal( &pArray )[tSize], TIndex pIndex, const TVal & pDefault )
	{
		return ( static_cast<size_t>( pIndex ) < tSize ) ? pArray[static_cast<size_t>( pIndex )] : pDefault;
	}

	template <typename TVal, size_t tSize, typename TIndex>
	inline constexpr const TVal & staticArrayElementOrDefault( const std::array<TVal, tSize> & pArray, TIndex pIndex, const TVal & pDefault )
	{
		return ( static_cast<size_t>( pIndex ) < tSize ) ? pArray[static_cast<size_t>( pIndex )] : pDefault;
	}

	template <typename TVal, size_t tSize>
	inline constexpr size_t staticArraySize( const TVal( &pArray )[tSize] )
	{
		return tSize;
	}

	template <typename TVal, size_t tSize>
	inline constexpr size_t staticArraySize( const std::array<TVal, tSize> & pArray )
	{
		return tSize;
	}


	template <typename TVal, size_t tSize, typename TOther>
	inline constexpr size_t copyArrayValues( TVal( &pTarget )[tSize], const TOther( &pSource )[tSize], size_t pOffset = 0, size_t pCount = tSize )
	{
		const auto offset = getMinOf( pOffset, tSize );
		const auto count = getMinOf( pCount, tSize - offset );

		for( size_t index = offset; index < offset + count; ++index )
		{
			pTarget[index] = pSource[index];
		}

		return count;
	}


	template <typename TVal, size_t tSize, typename TOther>
	inline constexpr size_t setArrayValues( TVal( &pArray )[tSize], TOther pValue, size_t pOffset = 0, size_t pCount = tSize )
	{
		const auto offset = getMinOf( pOffset, tSize );
		const auto count = getMinOf( pCount, tSize - offset );

		for( size_t index = offset; index < offset + count; ++index )
		{
			pArray[index] = pValue;
		}

		return count;
	}

	template <typename TVal, size_t tSize, typename TOther>
	inline constexpr size_t setArrayValues( std::array<TVal, tSize> & pArray, TOther pValue, size_t pOffset = 0, size_t pCount = tSize  )
	{
		const auto offset = getMinOf( pOffset, tSize );
		const auto count = getMinOf( pCount, tSize - offset );

		for( size_t index = offset; index < offset + count; ++index )
		{
			pArray[index] = pValue;
		}

		return count;
	}

	template <typename TIterable, typename TCallable>
	inline void foreachIn( TIterable && pIterable, TCallable pCallable )
	{
		for( auto && element : pIterable )
		{
			pCallable( element );
		}
	}

	template <typename TIterable, typename TValue, typename TMemberFunc, typename... TArgs>
	inline void foreachIn( TIterable && pIterable, TMemberFunc TValue::* pMemberFunc, TArgs && ...pArgs )
	{
		for( auto && element : pIterable )
		{
			( element.*pMemberFunc )( std::forward<TArgs>( pArgs )... );
		}
	}

	template <typename TIter, typename TValue, typename TMemberFunc, typename... TArgs>
	inline void foreachIn( TIter pBegin, TIter pEnd, TMemberFunc TValue::* pMemberFunc, TArgs && ...pArgs )
	{
		for( auto elemIter = pBegin; elemIter != pEnd; ++elemIter )
		{
			( elemIter->*pMemberFunc )( std::forward<TArgs>( pArgs )... );
		}
	}

	template <typename TIter>
	inline size_t countNonNullPointers( TIter pBegin, TIter pEnd )
	{
		const auto numElements = std::count_if( pBegin, pEnd, []( auto && pValue ) -> bool { return !!pValue; } );
        return numElements;
	}

	template <typename TIter>
	inline size_t countNullPointers( TIter pBegin, TIter pEnd )
	{
		const auto numElements = std::count_if( pBegin, pEnd, []( auto && pValue ) -> bool { return !pValue; } );
        return numElements;
	}

}

#endif // __IC3_CPPX_UTILITIES_H__
