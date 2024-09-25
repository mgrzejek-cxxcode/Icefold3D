
#ifndef __IC3_CPPXBit_UTILS_H__
#define __IC3_CPPXBit_UTILS_H__

#include "StaticLimits.h"
#include "TypeTraits.h"
#include <climits>

namespace Ic3::Cppx
{

	template <typename TPUint, size_t tpShift, typename TPInput>
	inline constexpr TPUint bshLeft( TPInput pValue )
	{
		static_assert(
			sizeof( TPUint ) >= sizeof( TPInput ),
			"Target uint type has smaller size than the input, explicit cast required" );

		static_assert(
			tpShift <= sizeof( TPUint ) * CHAR_BIT,
			"Bit shift outside the integer range is an Undefined Behaviour" );

		return static_cast<TPUint>( pValue ) << tpShift;
	}

	template <typename TPUint, size_t tpShift, typename TPInput>
	inline constexpr TPUint bshRight( TPInput pValue )
	{
		static_assert(
			sizeof( TPUint ) >= sizeof( TPInput ),
			"Target uint type has smaller size than the input, explicit cast required" );

		static_assert(
			tpShift <= sizeof( TPUint ) * CHAR_BIT,
			"Bit shift outside the integer range is an Undefined Behaviour" );

		return static_cast<TPUint>( pValue ) >> tpShift;
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

	template <typename TPValue>
	inline constexpr TPValue makeLSFBitmask( size_t pBitCount )
	{
		return ( pBitCount < QLimits<TPValue>::sBitCount ) ? static_cast<TPValue>( ( 1u << pBitCount ) - 1 ) : QLimits<TPValue>::sMaxValue;
	}

	template <size_t tpBitCount>
	inline constexpr typename QUintTypeByBits<tpBitCount>::Type makeLSFBitmask()
	{
		return makeLSFBitmask<typename QUintTypeByBits<tpBitCount>::Type>( tpBitCount );
	}

}

#endif // __IC3_CPPXBit_UTILS_H__
