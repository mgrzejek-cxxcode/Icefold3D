
#ifndef __CPPX_BIT_UTILS_H__
#define __CPPX_BIT_UTILS_H__

#include "typeLimits.h"
#include "typeTraits.h"
#include <climits>

namespace cppx
{

	template <typename TPUint, size_t tpShift, typename TPInput>
	inline constexpr TPUint bsh_left( TPInput pValue )
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
	inline constexpr TPUint bsh_right( TPInput pValue )
	{
		static_assert(
			sizeof( TPUint ) >= sizeof( TPInput ),
			"Target uint type has smaller size than the input, explicit cast required" );

		static_assert(
			tpShift <= sizeof( TPUint ) * CHAR_BIT,
			"Bit shift outside the integer range is an Undefined Behaviour" );

		return static_cast<TPUint>( pValue ) >> tpShift;
	}

	inline uint16 pop_count( uint8 pValue )
	{
		return static_cast<uint16>( PCL_POPCNT16( static_cast<uint16>( pValue ) ) );
	}

	inline uint16 pop_count( uint16 pValue )
	{
		return static_cast<uint16>( PCL_POPCNT16( pValue ) );
	}

	inline uint16 pop_count( uint32 pValue )
	{
		return static_cast<uint16>( PCL_POPCNT32( pValue ) );
	}

	inline uint16 pop_count( uint64 pValue )
	{
		return static_cast<uint16>( PCL_POPCNT64( pValue ) );
	}

	template <typename TPValue>
	inline constexpr TPValue make_lsfb_bitmask( size_t pBitCount )
	{
		return ( pBitCount < meta::limits<TPValue>::bit_count ) ? static_cast<TPValue>( ( 1u << pBitCount ) - 1 ) : meta::limits<TPValue>::max_value;
	}

	template <size_t tpBitCount>
	inline constexpr typename meta::uint_type_by_bits<tpBitCount>::Type make_lsfb_bitmask()
	{
		return make_lsfb_bitmask<typename meta::uint_type_by_bits<tpBitCount>::Type>( tpBitCount );
	}

}

#endif // __CPPX_BIT_UTILS_H__
