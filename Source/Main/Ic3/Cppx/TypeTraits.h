
#ifndef __IC3_CPPX_TYPE_TRAITS_H__
#define __IC3_CPPX_TYPE_TRAITS_H__

#include "StaticLimits.h"

namespace Ic3::Cppx
{

	/// @brief Contains a static `size_t` member variable `value` which yields the length of `TPTypes...`.
	template <typename... TPTypes>
	struct QTypeCounter
	{
		static constexpr size_t value = sizeof...( TPTypes );
	};

	/// @brief 
	template <bool tpBoolean, typename TPTrueType, typename TPFalseType>
	struct QConditionalType
	{
		using Type = typename std::conditional<tpBoolean, TPTrueType, TPFalseType>::type;
	};

	/// @brief
	template <typename TPValue, bool tpBoolean, TPValue tpTrueVal, TPValue tpFalseVal>
	struct QConditionalValue;
	
	template <typename TPValue, TPValue tpTrueVal, TPValue tpFalseVal>
	struct QConditionalValue<TPValue, true, tpTrueVal, tpFalseVal>
	{
		static constexpr TPValue value = tpTrueVal;
	};
	
	template <typename TPValue, TPValue tpTrueVal, TPValue tpFalseVal>
	struct QConditionalValue<TPValue, false, tpTrueVal, tpFalseVal>
	{
		static constexpr TPValue value = tpFalseVal;
	};


	/// @brief
	template <int... tpSequence>
	struct QIntegerSequence
	{};

	template <int tpN, int... tpSequence>
	struct QIntegerSequenceGenerator : QIntegerSequenceGenerator<tpN - 1, tpN - 1, tpSequence...>
	{};

	template <int... tpSequence>
	struct QIntegerSequenceGenerator<0, tpSequence...>
	{
		using Type = QIntegerSequence<tpSequence...>;
	};


	/// @brief Contains a member typedef `type`, which is the smallest signed integral type of at least specified size (in bytes)
	template <size_t tpSize>
	struct QIntTypeByBits
	{
		using Type = typename QConditionalType<
	        tpSize <= ( sizeof( int8 ) * CHAR_BIT ), int8, typename QConditionalType<
                tpSize <= ( sizeof( int16 ) * CHAR_BIT  ), int16, typename QConditionalType<
                    tpSize <= ( sizeof( int32 ) * CHAR_BIT ), int32, typename QConditionalType<
						tpSize <= ( sizeof( int64 ) * CHAR_BIT ), int64, void>::Type>::Type>::Type>::Type;
	};

	/// @brief Contains a member typedef `type`, which is the smallest unsigned integral type of at least specified size (in bytes)
	template <size_t tpSize>
	struct QUintTypeByBits
	{
		using Type = typename QConditionalType<
	        tpSize <= ( sizeof( uint8 ) * CHAR_BIT ), uint8, typename QConditionalType<
                tpSize <= ( sizeof( uint16 ) * CHAR_BIT ), uint16, typename QConditionalType<
                    tpSize <= ( sizeof( uint32 ) * CHAR_BIT ), uint32, typename QConditionalType<
						tpSize <= ( sizeof( uint64 ) * CHAR_BIT ), uint64, void>::Type>::Type>::Type>::Type;
	};


	/// @brief Contains a member typedef `type`, which is the smallest signed integral type of at least specified size (in bytes)
	template <size_t tpSize>
	struct QIntTypeBySize
	{
		using Type = typename QConditionalType<
	        tpSize <= sizeof( int8 ), int8, typename QConditionalType<
                tpSize <= sizeof( int16 ), int16, typename QConditionalType<
                    tpSize <= sizeof( int32 ), int32, typename QConditionalType<
						tpSize <= sizeof( int64 ), int64, void>::Type>::Type>::Type>::Type;
	};

	/// @brief Contains a member typedef `type`, which is the smallest unsigned integral type of at least specified size (in bytes)
	template <size_t tpSize>
	struct QUintTypeBySize
	{
		using Type = typename QConditionalType<
	        tpSize <= sizeof( uint8 ), uint8, typename QConditionalType<
                tpSize <= sizeof( uint16 ), uint16, typename QConditionalType<
                    tpSize <= sizeof( uint32 ), uint32, typename QConditionalType<
						tpSize <= sizeof( uint64 ), uint64, void>::Type>::Type>::Type>::Type;
	};

	/// @brief Contains a member typedef `type`, which is the smallest signed integral type required to hold the specified value.
	template <intmax_t tpValue>
	struct QIntTypeByValue
	{
		using Type = typename QConditionalType<
	        ( tpValue >= QLimits<int8>::sMinValue ) && ( tpValue <= QLimits<int8>::sMaxValue ), int8, typename QConditionalType<
                ( tpValue >= QLimits<int16>::sMinValue ) && ( tpValue <= QLimits<int16>::sMaxValue ), int16, typename QConditionalType<
                    ( tpValue >= QLimits<int32>::sMinValue ) && ( tpValue <= QLimits<int32>::sMaxValue ), int32, int64>::Type>::Type>::Type;
	};

	/// @brief Contains a member typedef `type`, which is the smallest unsigned integral type required to hold the specified value
	template <uintmax_t tpValue>
	struct QUintTypeByValue
	{
		using Type = typename QConditionalType<
	        tpValue <= QLimits<uint8>::sMaxValue, uint8, typename QConditionalType<
                tpValue <= QLimits<uint16>::sMaxValue, uint16, typename QConditionalType<
                    tpValue <= QLimits<uint32>::sMaxValue, uint32, uint64>::Type>::Type>::Type;
	};


	template <size_t tpTypeSize, bool tpIsInteger>
	struct QSignedUnsignedTypesProxy;

	template <>
	struct QSignedUnsignedTypesProxy<1, true>
	{
		using SignedType = signed char;
		using UnsignedType = unsigned char;
	};

	template <>
	struct QSignedUnsignedTypesProxy<2, true>
	{
		using SignedType = int16_t;
		using UnsignedType = uint16_t;
	};

	template <>
	struct QSignedUnsignedTypesProxy<4, true>
	{
		using SignedType = int32_t;
		using UnsignedType = uint32_t;
	};

	template <>
	struct QSignedUnsignedTypesProxy<8, true>
	{
		using SignedType = int64_t;
		using UnsignedType = uint64_t;
	};

	template <>
	struct QSignedUnsignedTypesProxy<sizeof( float ), false>
	{
		using SignedType = float;
		using UnsignedType = float;
	};

	template <>
	struct QSignedUnsignedTypesProxy<sizeof( double ), false>
	{
		using SignedType = double;
		using UnsignedType = double;
	};


	template <typename TPNumeric>
	struct QSignedTypeEquivalent
	{
		using Type = typename QSignedUnsignedTypesProxy<sizeof( TPNumeric ), std::is_integral<TPNumeric>::value>::SignedType;
	};

	template <typename TPNumeric>
	struct QUnsignedTypeEquivalent
	{
		using Type = typename QSignedUnsignedTypesProxy<sizeof( TPNumeric ), std::is_integral<TPNumeric>::value>::UnsignedType;
	};


	/// @brief Contains a static boolean variable `value`, which yields true if specified type list `TPTypes` contains `void` or false otherwise.
	template <typename... TPTypes>
	struct QIsVoidOnTypeList;

	template <typename TPValue>
	struct QIsVoidOnTypeList<TPValue>
	{
		static constexpr bool value = std::is_void<TPValue>::value;
	};

	template <typename TPValue, typename... TPRest>
	struct QIsVoidOnTypeList<TPValue, TPRest...>
	{
		static constexpr bool value = std::is_void<TPValue>::value || QIsVoidOnTypeList<TPRest...>::value;
	};


	template <typename TPType, typename... TList>
	struct QIsTypeOnTypeList;

	template <typename TPType, typename TCheck>
	struct QIsTypeOnTypeList<TPType, TCheck>
	{
		static constexpr bool value = std::is_same<TPType, TCheck>::value;
	};

	template <typename TPType, typename TCheck, typename... TList>
	struct QIsTypeOnTypeList<TPType, TCheck, TList...>
	{
		static constexpr bool value = std::is_same<TPType, TCheck>::value || QIsTypeOnTypeList<TPType, TList...>::value;
	};


	/// @brief Contains a member typedef `type`, which is the first type from `TPTypes...` which matches `TPPredicate`
	template <template<typename> class TPPredicate, typename... TPTypes>
	struct QFirstMatchingType;

	template <template<typename> class TPPredicate, typename TPValue>
	struct QFirstMatchingType<TPPredicate, TPValue>
	{
		using Type = typename QConditionalType<TPPredicate<TPValue>::value, TPValue, void>::Type;
	};

	template <template<typename> class TPPredicate, typename TPValue, typename... TPRest>
	struct QFirstMatchingType<TPPredicate, TPValue, TPRest...>
	{
		using Type = typename QConditionalType<TPPredicate<TPValue>::value, TPValue, typename QFirstMatchingType<TPPredicate, TPRest...>::Type>::Type;
	};

}

#endif /* __IC3_CPPX_TYPE_TRAITS_H__ */
