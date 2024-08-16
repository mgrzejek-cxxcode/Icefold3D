
#ifndef __IC3_CPPX_BIT_UTILS_H__
#define __IC3_CPPX_BIT_UTILS_H__

#include "StaticLimits.h"
#include "TypeTraits.h"
#include <climits>

namespace Ic3::Cppx
{

	template <typename TUint, size_t tShift, typename TInput>
	inline constexpr TUint bshLeft( TInput pValue )
	{
		static_assert( sizeof( TUint ) >= sizeof( TInput ), "Target uint type has smaller size than the input, explicit cast required" );
		static_assert( tShift <= sizeof( TUint ) * CHAR_BIT, "Bit shift outside the integer range is an Undefined Behaviour" );
		return static_cast<TUint>( pValue ) << tShift;
	}

	template <typename TUint, size_t tShift, typename TInput>
	inline constexpr TUint bshRight( TInput pValue )
	{
		static_assert( sizeof( TUint ) >= sizeof( TInput ), "Target uint type has smaller size than the input, explicit cast required" );
		static_assert( tShift <= sizeof( TUint ) * CHAR_BIT, "Bit shift outside the integer range is an Undefined Behaviour" );
		return static_cast<TUint>( pValue ) >> tShift;
	}

	inline uint16 popCount( uint8 pValue )
	{
		return static_cast<uint16>( IC3_PCL_POPCNT16( static_cast<uint16>( pValue ) ) );
	}

	inline uint16 popCount( uint16 pValue )
	{
		return static_cast<uint16>( IC3_PCL_POPCNT16( pValue ) );
	}

	inline uint16 popCount( uint32 pValue )
	{
		return static_cast<uint16>( IC3_PCL_POPCNT32( pValue ) );
	}

	inline uint16 popCount( uint64 pValue )
	{
		return static_cast<uint16>( IC3_PCL_POPCNT64( pValue ) );
	}

	template <typename TVal>
	inline constexpr TVal makeLSFBitmask( size_t pBitCount )
	{
		return ( pBitCount < QLimits<TVal>::bitCount ) ? static_cast<TVal>( ( 1u << pBitCount ) - 1 ) : QLimits<TVal>::maxValue;
	}

	template <size_t tBitCount>
	inline constexpr typename QUintTypeByBits<tBitCount>::Type makeLSFBitmask()
	{
		using TVal = typename QUintTypeByBits<tBitCount>::Type;
		return ( tBitCount < QLimits<TVal>::bitCount ) ? static_cast<TVal>( ( 1u << tBitCount ) - 1 ) : QLimits<TVal>::maxValue;
	}

}

#endif // __IC3_CPPX_BIT_UTILS_H__
