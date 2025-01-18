
#ifndef __CPPX_VALUE_LIMITS_H__
#define __CPPX_VALUE_LIMITS_H__

#include "typeTraits.h"

namespace cppx
{

	namespace meta
	{

		template <size_t tpBitCount, bool tpIsSigned>
		struct value_bit_limits_base;

		template <bool tpIsSigned>
		struct value_bit_limits_base<0, tpIsSigned>;

		template <size_t tpBitCount>
		struct value_bit_limits_base<tpBitCount, true>
		{
			using value_type = typename int_type_by_bits<tpBitCount>::type;
			static constexpr value_type min_value = -( 2 << ( tpBitCount - 1 ) );
			static constexpr value_type max_value = ( 2 << ( tpBitCount - 1 ) ) - 1;
		};

		template <size_t tpBitCount>
		struct value_bit_limits_base<tpBitCount, false>
		{
			using value_type = typename uint_type_by_bits<tpBitCount>::type;
			static constexpr value_type min_value = 0;
			static constexpr value_type max_value = ( 2 << tpBitCount ) - 1;
		};

		template <size_t tpBitCount>
		using value_bit_limits_signed = value_bit_limits_base<tpBitCount, true>;

		template <size_t tpBitCount>
		using value_bit_limits_unsigned = value_bit_limits_base<tpBitCount, false>;

	}

}

#endif /* __CPPX_VALUE_LIMITS_H__ */
