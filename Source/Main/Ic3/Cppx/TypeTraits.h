
#ifndef __IC3_CPPX_TYPE_TRAITS_H__
#define __IC3_CPPX_TYPE_TRAITS_H__

#include "StaticLimits.h"

namespace Ic3::Cppx
{

	/// @brief Contains a static `size_t` member variable `value` which yields the length of `TTypes...`.
	template <typename... TTypes>
	struct QTypeCounter
	{
		static constexpr size_t value = sizeof...( TTypes );
	};

	/// @brief 
	template <bool tBoolean, typename TTrueType, typename TFalseType>
	struct QConditionalType
	{
		using Type = typename std::conditional<tBoolean, TTrueType, TFalseType>::type;
	};

	/// @brief
	template <typename TValue, bool tBoolean, TValue tTrueval, TValue tFalseval>
	struct QConditionalValue;
	
	template <typename TValue, TValue tTrueval, TValue tFalseval>
	struct QConditionalValue<TValue, true, tTrueval, tFalseval>
	{
		static constexpr TValue value = tTrueval;
	};
	
	template <typename TValue, TValue tTrueval, TValue tFalseval>
	struct QConditionalValue<TValue, false, tTrueval, tFalseval>
	{
		static constexpr TValue value = tFalseval;
	};


	/// @brief
	template <int... tSeq>
	struct QIntegerSequence
	{};

	template <int tN, int... tSeq>
	struct QIntegerSequenceGenerator : QIntegerSequenceGenerator<tN - 1, tN - 1, tSeq...>
	{};

	template <int... tSeq>
	struct QIntegerSequenceGenerator<0, tSeq...>
	{
		using Type = QIntegerSequence<tSeq...>;
	};


	/// @brief Contains a member typedef `type`, which is the smallest signed integral type of at least specified size (in bytes)
	template <size_t tSize>
	struct QIntTypeByBits
	{
		using Type = typename QConditionalType<
	        tSize <= ( sizeof( int8 ) * CHAR_BIT ), int8, typename QConditionalType<
                tSize <= ( sizeof( int16 ) * CHAR_BIT  ), int16, typename QConditionalType<
                    tSize <= ( sizeof( int32 ) * CHAR_BIT ), int32, typename QConditionalType<
						tSize <= ( sizeof( int64 ) * CHAR_BIT ), int64, void>::Type>::Type>::Type>::Type;
	};

	/// @brief Contains a member typedef `type`, which is the smallest unsigned integral type of at least specified size (in bytes)
	template <size_t tSize>
	struct QUintTypeByBits
	{
		using Type = typename QConditionalType<
	        tSize <= ( sizeof( uint8 ) * CHAR_BIT ), uint8, typename QConditionalType<
                tSize <= ( sizeof( uint16 ) * CHAR_BIT ), uint16, typename QConditionalType<
                    tSize <= ( sizeof( uint32 ) * CHAR_BIT ), uint32, typename QConditionalType<
						tSize <= ( sizeof( uint64 ) * CHAR_BIT ), uint64, void>::Type>::Type>::Type>::Type;
	};


	/// @brief Contains a member typedef `type`, which is the smallest signed integral type of at least specified size (in bytes)
	template <size_t tSize>
	struct QIntTypeBySize
	{
		using Type = typename QConditionalType<
	        tSize <= sizeof( int8 ), int8, typename QConditionalType<
                tSize <= sizeof( int16 ), int16, typename QConditionalType<
                    tSize <= sizeof( int32 ), int32, typename QConditionalType<
						tSize <= sizeof( int64 ), int64, void>::Type>::Type>::Type>::Type;
	};

	/// @brief Contains a member typedef `type`, which is the smallest unsigned integral type of at least specified size (in bytes)
	template <size_t tSize>
	struct QUintTypeBySize
	{
		using Type = typename QConditionalType<
	        tSize <= sizeof( uint8 ), uint8, typename QConditionalType<
                tSize <= sizeof( uint16 ), uint16, typename QConditionalType<
                    tSize <= sizeof( uint32 ), uint32, typename QConditionalType<
						tSize <= sizeof( uint64 ), uint64, void>::Type>::Type>::Type>::Type;
	};

	/// @brief Contains a member typedef `type`, which is the smallest signed integral type required to hold the specified value.
	template <intmax_t tValue>
	struct QIntTypeByValue
	{
		using Type = typename QConditionalType<
	        ( tValue >= QLimits<int8>::minValue ) && ( tValue <= QLimits<int8>::maxValue ), int8, typename QConditionalType<
                ( tValue >= QLimits<int16>::minValue ) && ( tValue <= QLimits<int16>::maxValue ), int16, typename QConditionalType<
                    ( tValue >= QLimits<int32>::minValue ) && ( tValue <= QLimits<int32>::maxValue ), int32, int64>::Type>::Type>::Type;
	};

	/// @brief Contains a member typedef `type`, which is the smallest unsigned integral type required to hold the specified value
	template <uintmax_t tValue>
	struct QUintTypeByValue
	{
		using Type = typename QConditionalType<
	        tValue <= QLimits<uint8>::maxValue, uint8, typename QConditionalType<
                tValue <= QLimits<uint16>::maxValue, uint16, typename QConditionalType<
                    tValue <= QLimits<uint32>::maxValue, uint32, uint64>::Type>::Type>::Type;
	};


	template <size_t tTypeSize, bool tIsInteger>
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


	template <typename TNumeric>
	struct QSignedTypeEquivalent
	{
		using Type = typename QSignedUnsignedTypesProxy<sizeof( TNumeric ), std::is_integral<TNumeric>::value>::SignedType;
	};

	template <typename TNumeric>
	struct QUnsignedTypeEquivalent
	{
		using Type = typename QSignedUnsignedTypesProxy<sizeof( TNumeric ), std::is_integral<TNumeric>::value>::UnsignedType;
	};


	/// @brief Contains a static boolean variable `value`, which yields true if specified type list `TTypes` contains `void` or false otherwise.
	template <typename... TTypes>
	struct QIsVoidOnTypeList;

	template <typename TVal>
	struct QIsVoidOnTypeList<TVal>
	{
		static constexpr bool value = std::is_void<TVal>::value;
	};

	template <typename TVal, typename... TRest>
	struct QIsVoidOnTypeList<TVal, TRest...>
	{
		static constexpr bool value = std::is_void<TVal>::value || QIsVoidOnTypeList<TRest...>::value;
	};


	template <typename TType, typename... TList>
	struct QIsTypeOnTypeList;

	template <typename TType, typename TCheck>
	struct QIsTypeOnTypeList<TType, TCheck>
	{
		static constexpr bool value = std::is_same<TType, TCheck>::value;
	};

	template <typename TType, typename TCheck, typename... TList>
	struct QIsTypeOnTypeList<TType, TCheck, TList...>
	{
		static constexpr bool value = std::is_same<TType, TCheck>::value || QIsTypeOnTypeList<TType, TList...>::value;
	};


	/// @brief Contains a member typedef `type`, which is the first type from `TTypes...` which matches `TPred`
	template <template<typename> class TPred, typename... TTypes>
	struct QFirstMatchingType;

	template <template<typename> class TPred, typename TVal>
	struct QFirstMatchingType<TPred, TVal>
	{
		using Type = typename QConditionalType<TPred<TVal>::value, TVal, void>::Type;
	};

	template <template<typename> class TPred, typename TVal, typename... TRest>
	struct QFirstMatchingType<TPred, TVal, TRest...>
	{
		using Type = typename QConditionalType<TPred<TVal>::value, TVal, typename QFirstMatchingType<TPred, TRest...>::Type>::Type;
	};

}

#endif /* __IC3_CPPX_TYPE_TRAITS_H__ */
