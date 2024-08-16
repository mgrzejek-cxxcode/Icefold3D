
#ifndef __IC3_CPPX_STATIC_LIMITS_H__
#define __IC3_CPPX_STATIC_LIMITS_H__

#include "Prerequisites.h"

#include <cfloat>
#include <climits>
#include <numeric>
#include <type_traits>

namespace Ic3::Cppx
{

	template <typename TType, size_t tTypeSize, bool tSigned>
	struct IntegerLimitsProxy;

	template <typename TType>
	struct IntegerLimitsProxy<TType, 1, true>
	{
		static constexpr uint32 bitCount = CHAR_BIT;
		static constexpr TType minValue = INT8_MIN;
		static constexpr TType maxValue = INT8_MAX;
		static constexpr TType epsilon = 0;
		static constexpr TType roundError = 0;
	};

	template <typename TType>
	struct IntegerLimitsProxy<TType, 1, false>
	{
		static constexpr uint32 bitCount = CHAR_BIT;
		static constexpr TType minValue = 0U;
		static constexpr TType maxValue = UINT8_MAX;
		static constexpr TType epsilon = 0U;
		static constexpr TType roundError = 0U;
	};

	template <typename TType>
	struct IntegerLimitsProxy<TType, 2, true>
	{
		static constexpr uint32 bitCount = CHAR_BIT * 2;
		static constexpr TType minValue = INT16_MIN;
		static constexpr TType maxValue = INT16_MAX;
		static constexpr TType epsilon = 0;
		static constexpr TType roundError = 0;
	};

	template <typename TType>
	struct IntegerLimitsProxy<TType, 2, false>
	{
		static constexpr uint32 bitCount = CHAR_BIT * 2;
		static constexpr TType minValue = 0U;
		static constexpr TType maxValue = UINT16_MAX;
		static constexpr TType epsilon = 0U;
		static constexpr TType roundError = 0U;
	};

	template <typename TType>
	struct IntegerLimitsProxy<TType, 4, true>
	{
		static constexpr uint32 bitCount = CHAR_BIT * 4;
		static constexpr TType minValue = INT32_MIN;
		static constexpr TType maxValue = INT32_MAX;
		static constexpr TType epsilon = 0;
		static constexpr TType roundError = 0;
	};

	template <typename TType>
	struct IntegerLimitsProxy<TType, 4, false>
	{
		static constexpr uint32 bitCount = CHAR_BIT * 4;
		static constexpr TType minValue = 0U;
		static constexpr TType maxValue = UINT32_MAX;
		static constexpr TType epsilon = 0U;
		static constexpr TType roundError = 0U;
	};

	template <typename TType>
	struct IntegerLimitsProxy<TType, 8, true>
	{
		static constexpr uint32 bitCount = CHAR_BIT * 8;
		static constexpr TType minValue = LLONG_MIN;
		static constexpr TType maxValue = LLONG_MAX;
		static constexpr TType epsilon = 0;
		static constexpr TType roundError = 0;
	};

	template <typename TType>
	struct IntegerLimitsProxy<TType, 8, false>
	{
		static constexpr uint32 bitCount = CHAR_BIT * 8;
		static constexpr TType minValue = 0U;
		static constexpr TType maxValue = ULLONG_MAX;
		static constexpr TType epsilon = 0U;
		static constexpr TType roundError = 0U;
	};

	template <typename>
	struct QLimits;

	template <>
	struct QLimits<char> : public IntegerLimitsProxy<char, sizeof( char ), std::is_signed<char>::value>
	{
		using ValueType = char;
	};

	template <>
	struct QLimits<signed char> : public IntegerLimitsProxy<signed char, sizeof( signed char ), true>
	{
		using ValueType = signed char;
	};

	template <>
	struct QLimits<unsigned char> : public IntegerLimitsProxy<unsigned char, sizeof( unsigned char ), false>
	{
		using ValueType = unsigned char;
	};

	template <>
	struct QLimits<signed short> : public IntegerLimitsProxy<signed short, sizeof( signed short ), true>
	{
		using ValueType = signed short;
	};

	template <>
	struct QLimits<unsigned short> : public IntegerLimitsProxy<unsigned short, sizeof( unsigned short ), false>
	{
		using ValueType = unsigned short;
	};

	template <>
	struct QLimits<signed int> : public IntegerLimitsProxy<signed int, sizeof( signed int ), true>
	{
		using ValueType = signed int;
	};

	template <>
	struct QLimits<unsigned int> : public IntegerLimitsProxy<unsigned int, sizeof( unsigned int ), false>
	{
		using ValueType = unsigned int;
	};

	template <>
	struct QLimits<signed long> : public IntegerLimitsProxy<signed long, sizeof( signed long ), true>
	{
		using ValueType = signed long;
	};

	template <>
	struct QLimits<unsigned long> : public IntegerLimitsProxy<unsigned long, sizeof( unsigned long ), false>
	{
		using ValueType = unsigned long;
	};

	template <>
	struct QLimits<signed long long> : public IntegerLimitsProxy<signed long long, sizeof( signed long long ), true>
	{
		using ValueType = signed long long;
	};

	template <>
	struct QLimits<unsigned long long> : public IntegerLimitsProxy<unsigned long long, sizeof( unsigned long long ), false>
	{
		using ValueType = unsigned long long;
	};

	template <>
	struct QLimits<float>
	{
		using ValueType = float;

		static constexpr ValueType minValue = FLT_MIN;
		static constexpr ValueType maxValue = FLT_MAX;
		static constexpr ValueType epsilon = 0;
		static constexpr ValueType roundError = 0.5f;
	};

	template <>
	struct QLimits<double>
	{
		using ValueType = double;

		static constexpr ValueType minValue = DBL_MIN;
		static constexpr ValueType maxValue = DBL_MAX;
		static constexpr ValueType epsilon = DBL_EPSILON;
		static constexpr ValueType roundError = 0.5;
	};

	template <>
	struct QLimits<long double>
	{
		using ValueType = long double;

		static constexpr ValueType minValue = LDBL_MIN;
		static constexpr ValueType maxValue = LDBL_MAX;
		static constexpr ValueType epsilon = LDBL_EPSILON;
		static constexpr ValueType roundError = 0.5L;
	};

	constexpr auto CX_INT16_MAX = QLimits<int16>::maxValue;
	constexpr auto CX_INT16_MIN = QLimits<int16>::minValue;
	constexpr auto CX_UINT16_MAX = QLimits<uint16>::maxValue;
	constexpr auto CX_UINT16_MIN = QLimits<uint16>::minValue;

	constexpr auto CX_INT32_MAX = QLimits<int32>::maxValue;
	constexpr auto CX_INT32_MIN = QLimits<int32>::minValue;
	constexpr auto CX_UINT32_MAX = QLimits<uint32>::maxValue;
	constexpr auto CX_UINT32_MIN = QLimits<uint32>::minValue;

	constexpr auto CX_INT64_MAX = QLimits<int64>::maxValue;
	constexpr auto CX_INT64_MIN = QLimits<int64>::minValue;
	constexpr auto CX_UINT64_MAX = QLimits<uint64>::maxValue;
	constexpr auto CX_UINT64_MIN = QLimits<uint64>::minValue;

}

#endif /* __IC3_CPPX_STATIC_LIMITS_H__ */
