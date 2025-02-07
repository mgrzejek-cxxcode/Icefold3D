
#ifndef __CPPX_TYPE_LIMITS_H__
#define __CPPX_TYPE_LIMITS_H__

#include "prerequisites.h"

#include <cfloat>
#include <climits>
#include <numeric>
#include <type_traits>

namespace cppx
{

	namespace meta
	{

		template <typename TPType, size_t tpTypeSize, bool tpIsSigned>
		struct integer_limits_proxy;

		template <typename TPType>
		struct integer_limits_proxy<TPType, 1, true>
		{
			static constexpr uint32 bit_count = CHAR_BIT;
			static constexpr TPType min_value = INT8_MIN;
			static constexpr TPType max_value = INT8_MAX;
			static constexpr TPType epsilon = 0;
			static constexpr TPType round_error = 0;
		};

		template <typename TPType>
		struct integer_limits_proxy<TPType, 1, false>
		{
			static constexpr uint32 bit_count = CHAR_BIT;
			static constexpr TPType min_value = 0U;
			static constexpr TPType max_value = UINT8_MAX;
			static constexpr TPType epsilon = 0U;
			static constexpr TPType round_error = 0U;
		};

		template <typename TPType>
		struct integer_limits_proxy<TPType, 2, true>
		{
			static constexpr uint32 bit_count = CHAR_BIT * 2;
			static constexpr TPType min_value = INT16_MIN;
			static constexpr TPType max_value = INT16_MAX;
			static constexpr TPType epsilon = 0;
			static constexpr TPType round_error = 0;
		};

		template <typename TPType>
		struct integer_limits_proxy<TPType, 2, false>
		{
			static constexpr uint32 bit_count = CHAR_BIT * 2;
			static constexpr TPType min_value = 0U;
			static constexpr TPType max_value = UINT16_MAX;
			static constexpr TPType epsilon = 0U;
			static constexpr TPType round_error = 0U;
		};

		template <typename TPType>
		struct integer_limits_proxy<TPType, 4, true>
		{
			static constexpr uint32 bit_count = CHAR_BIT * 4;
			static constexpr TPType min_value = INT32_MIN;
			static constexpr TPType max_value = INT32_MAX;
			static constexpr TPType epsilon = 0;
			static constexpr TPType round_error = 0;
		};

		template <typename TPType>
		struct integer_limits_proxy<TPType, 4, false>
		{
			static constexpr uint32 bit_count = CHAR_BIT * 4;
			static constexpr TPType min_value = 0U;
			static constexpr TPType max_value = UINT32_MAX;
			static constexpr TPType epsilon = 0U;
			static constexpr TPType round_error = 0U;
		};

		template <typename TPType>
		struct integer_limits_proxy<TPType, 8, true>
		{
			static constexpr uint32 bit_count = CHAR_BIT * 8;
			static constexpr TPType min_value = LLONG_MIN;
			static constexpr TPType max_value = LLONG_MAX;
			static constexpr TPType epsilon = 0;
			static constexpr TPType round_error = 0;
		};

		template <typename TPType>
		struct integer_limits_proxy<TPType, 8, false>
		{
			static constexpr uint32 bit_count = CHAR_BIT * 8;
			static constexpr TPType min_value = 0U;
			static constexpr TPType max_value = ULLONG_MAX;
			static constexpr TPType epsilon = 0U;
			static constexpr TPType round_error = 0U;
		};

		template <typename>
		struct limits;

		template <>
		struct limits<char> : public integer_limits_proxy<char, sizeof( char ), std::is_signed<char>::value>
		{
			using value_type = char;
		};

		template <>
		struct limits<signed char> : public integer_limits_proxy<signed char, sizeof( signed char ), true>
		{
			using value_type = signed char;
		};

		template <>
		struct limits<unsigned char> : public integer_limits_proxy<unsigned char, sizeof( unsigned char ), false>
		{
			using value_type = unsigned char;
		};

		template <>
		struct limits<signed short> : public integer_limits_proxy<signed short, sizeof( signed short ), true>
		{
			using value_type = signed short;
		};

		template <>
		struct limits<unsigned short> : public integer_limits_proxy<unsigned short, sizeof( unsigned short ), false>
		{
			using value_type = unsigned short;
		};

		template <>
		struct limits<signed int> : public integer_limits_proxy<signed int, sizeof( signed int ), true>
		{
			using value_type = signed int;
		};

		template <>
		struct limits<unsigned int> : public integer_limits_proxy<unsigned int, sizeof( unsigned int ), false>
		{
			using value_type = unsigned int;
		};

		template <>
		struct limits<signed long> : public integer_limits_proxy<signed long, sizeof( signed long ), true>
		{
			using value_type = signed long;
		};

		template <>
		struct limits<unsigned long> : public integer_limits_proxy<unsigned long, sizeof( unsigned long ), false>
		{
			using value_type = unsigned long;
		};

		template <>
		struct limits<signed long long> : public integer_limits_proxy<signed long long, sizeof( signed long long ), true>
		{
			using value_type = signed long long;
		};

		template <>
		struct limits<unsigned long long> : public integer_limits_proxy<unsigned long long, sizeof( unsigned long long ), false>
		{
			using value_type = unsigned long long;
		};

		template <>
		struct limits<float>
		{
			using value_type = float;

			static constexpr value_type min_value = FLT_MIN;
			static constexpr value_type max_value = FLT_MAX;
			static constexpr value_type epsilon = 0;
			static constexpr value_type round_error = 0.5f;
		};

		template <>
		struct limits<double>
		{
			using value_type = double;

			static constexpr value_type min_value = DBL_MIN;
			static constexpr value_type max_value = DBL_MAX;
			static constexpr value_type epsilon = DBL_EPSILON;
			static constexpr value_type round_error = 0.5;
		};

		template <>
		struct limits<long double>
		{
			using value_type = long double;

			static constexpr value_type min_value = LDBL_MIN;
			static constexpr value_type max_value = LDBL_MAX;
			static constexpr value_type epsilon = LDBL_EPSILON;
			static constexpr value_type round_error = 0.5L;
		};

	}

	namespace cve
	{

		/// @brief Represents max value which can be stored in the native_int type.
		constexpr native_int native_int_max = meta::limits<native_int>::max_value;

		/// @brief Represents max value which can be stored in the native_uint type.
		constexpr native_uint native_uint_max = meta::limits<native_uint>::max_value;

		constexpr auto int16_max = meta::limits<int16>::max_value;
		constexpr auto int16_min = meta::limits<int16>::min_value;
		constexpr auto uint16_max = meta::limits<uint16>::max_value;
		constexpr auto uint16_min = meta::limits<uint16>::min_value;

		constexpr auto int32_max = meta::limits<int32>::max_value;
		constexpr auto int32_min = meta::limits<int32>::min_value;
		constexpr auto uint32_max = meta::limits<uint32>::max_value;
		constexpr auto uint32_min = meta::limits<uint32>::min_value;

		constexpr auto int64_max = meta::limits<int64>::max_value;
		constexpr auto int64_min = meta::limits<int64>::min_value;
		constexpr auto uint64_max = meta::limits<uint64>::max_value;
		constexpr auto uint64_min = meta::limits<uint64>::min_value;

	}

}

#endif /* __CPPX_TYPE_LIMITS_H__ */
