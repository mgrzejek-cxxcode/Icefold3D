
#ifndef __IC3_CPPX_STATIC_LIMITS_H__
#define __IC3_CPPX_STATIC_LIMITS_H__

#include "Prerequisites.h"

#include <cfloat>
#include <climits>
#include <numeric>
#include <type_traits>

namespace Ic3::Cppx
{

	template <typename TPType, size_t tpTypeSize, bool tpIsSigned>
	struct QIntegerLimitsProxy;

	template <typename TPType>
	struct QIntegerLimitsProxy<TPType, 1, true>
	{
		static constexpr uint32 sBitCount = CHAR_BIT;
		static constexpr TPType sMinValue = INT8_MIN;
		static constexpr TPType sMaxValue = INT8_MAX;
		static constexpr TPType sEpsilon = 0;
		static constexpr TPType sRoundError = 0;
	};

	template <typename TPType>
	struct QIntegerLimitsProxy<TPType, 1, false>
	{
		static constexpr uint32 sBitCount = CHAR_BIT;
		static constexpr TPType sMinValue = 0U;
		static constexpr TPType sMaxValue = UINT8_MAX;
		static constexpr TPType sEpsilon = 0U;
		static constexpr TPType sRoundError = 0U;
	};

	template <typename TPType>
	struct QIntegerLimitsProxy<TPType, 2, true>
	{
		static constexpr uint32 sBitCount = CHAR_BIT * 2;
		static constexpr TPType sMinValue = INT16_MIN;
		static constexpr TPType sMaxValue = INT16_MAX;
		static constexpr TPType sEpsilon = 0;
		static constexpr TPType sRoundError = 0;
	};

	template <typename TPType>
	struct QIntegerLimitsProxy<TPType, 2, false>
	{
		static constexpr uint32 sBitCount = CHAR_BIT * 2;
		static constexpr TPType sMinValue = 0U;
		static constexpr TPType sMaxValue = UINT16_MAX;
		static constexpr TPType sEpsilon = 0U;
		static constexpr TPType sRoundError = 0U;
	};

	template <typename TPType>
	struct QIntegerLimitsProxy<TPType, 4, true>
	{
		static constexpr uint32 sBitCount = CHAR_BIT * 4;
		static constexpr TPType sMinValue = INT32_MIN;
		static constexpr TPType sMaxValue = INT32_MAX;
		static constexpr TPType sEpsilon = 0;
		static constexpr TPType sRoundError = 0;
	};

	template <typename TPType>
	struct QIntegerLimitsProxy<TPType, 4, false>
	{
		static constexpr uint32 sBitCount = CHAR_BIT * 4;
		static constexpr TPType sMinValue = 0U;
		static constexpr TPType sMaxValue = UINT32_MAX;
		static constexpr TPType sEpsilon = 0U;
		static constexpr TPType sRoundError = 0U;
	};

	template <typename TPType>
	struct QIntegerLimitsProxy<TPType, 8, true>
	{
		static constexpr uint32 sBitCount = CHAR_BIT * 8;
		static constexpr TPType sMinValue = LLONG_MIN;
		static constexpr TPType sMaxValue = LLONG_MAX;
		static constexpr TPType sEpsilon = 0;
		static constexpr TPType sRoundError = 0;
	};

	template <typename TPType>
	struct QIntegerLimitsProxy<TPType, 8, false>
	{
		static constexpr uint32 sBitCount = CHAR_BIT * 8;
		static constexpr TPType sMinValue = 0U;
		static constexpr TPType sMaxValue = ULLONG_MAX;
		static constexpr TPType sEpsilon = 0U;
		static constexpr TPType sRoundError = 0U;
	};

	template <typename>
	struct QLimits;

	template <>
	struct QLimits<char> : public QIntegerLimitsProxy<char, sizeof( char ), std::is_signed<char>::value>
	{
		using ValueType = char;
	};

	template <>
	struct QLimits<signed char> : public QIntegerLimitsProxy<signed char, sizeof( signed char ), true>
	{
		using ValueType = signed char;
	};

	template <>
	struct QLimits<unsigned char> : public QIntegerLimitsProxy<unsigned char, sizeof( unsigned char ), false>
	{
		using ValueType = unsigned char;
	};

	template <>
	struct QLimits<signed short> : public QIntegerLimitsProxy<signed short, sizeof( signed short ), true>
	{
		using ValueType = signed short;
	};

	template <>
	struct QLimits<unsigned short> : public QIntegerLimitsProxy<unsigned short, sizeof( unsigned short ), false>
	{
		using ValueType = unsigned short;
	};

	template <>
	struct QLimits<signed int> : public QIntegerLimitsProxy<signed int, sizeof( signed int ), true>
	{
		using ValueType = signed int;
	};

	template <>
	struct QLimits<unsigned int> : public QIntegerLimitsProxy<unsigned int, sizeof( unsigned int ), false>
	{
		using ValueType = unsigned int;
	};

	template <>
	struct QLimits<signed long> : public QIntegerLimitsProxy<signed long, sizeof( signed long ), true>
	{
		using ValueType = signed long;
	};

	template <>
	struct QLimits<unsigned long> : public QIntegerLimitsProxy<unsigned long, sizeof( unsigned long ), false>
	{
		using ValueType = unsigned long;
	};

	template <>
	struct QLimits<signed long long> : public QIntegerLimitsProxy<signed long long, sizeof( signed long long ), true>
	{
		using ValueType = signed long long;
	};

	template <>
	struct QLimits<unsigned long long> : public QIntegerLimitsProxy<unsigned long long, sizeof( unsigned long long ), false>
	{
		using ValueType = unsigned long long;
	};

	template <>
	struct QLimits<float>
	{
		using ValueType = float;

		static constexpr ValueType sMinValue = FLT_MIN;
		static constexpr ValueType sMaxValue = FLT_MAX;
		static constexpr ValueType sEpsilon = 0;
		static constexpr ValueType sRoundError = 0.5f;
	};

	template <>
	struct QLimits<double>
	{
		using ValueType = double;

		static constexpr ValueType sMinValue = DBL_MIN;
		static constexpr ValueType sMaxValue = DBL_MAX;
		static constexpr ValueType sEpsilon = DBL_EPSILON;
		static constexpr ValueType sRoundError = 0.5;
	};

	template <>
	struct QLimits<long double>
	{
		using ValueType = long double;

		static constexpr ValueType sMinValue = LDBL_MIN;
		static constexpr ValueType sMaxValue = LDBL_MAX;
		static constexpr ValueType sEpsilon = LDBL_EPSILON;
		static constexpr ValueType sRoundError = 0.5L;
	};

	constexpr auto cxInt16Max = QLimits<int16>::sMaxValue;
	constexpr auto cxInt16Min = QLimits<int16>::sMinValue;
	constexpr auto cxUint16Max = QLimits<uint16>::sMaxValue;
	constexpr auto cxUint16Min = QLimits<uint16>::sMinValue;

	constexpr auto cxInt32Max = QLimits<int32>::sMaxValue;
	constexpr auto cxInt32Min = QLimits<int32>::sMinValue;
	constexpr auto cxUint32Max = QLimits<uint32>::sMaxValue;
	constexpr auto cxUint32Min = QLimits<uint32>::sMinValue;

	constexpr auto cxInt64Max = QLimits<int64>::sMaxValue;
	constexpr auto cxInt64Min = QLimits<int64>::sMinValue;
	constexpr auto cxUint64Max = QLimits<uint64>::sMaxValue;
	constexpr auto cxUint64Min = QLimits<uint64>::sMinValue;

}

#endif /* __IC3_CPPX_STATIC_LIMITS_H__ */
