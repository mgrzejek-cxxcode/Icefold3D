
#ifndef __CPPX_TYPE_TRAITS_H__
#define __CPPX_TYPE_TRAITS_H__

#include "typeLimits.h"

namespace cppx::meta
{

	/// @brief Contains a static `size_t` member variable `value` which yields the length of `TPTypes...`.
	template <typename... TPTypes>
	struct type_counter
	{
		static constexpr size_t value = sizeof...( TPTypes );
	};

	/// @brief 
	template <bool tpBoolean, typename TPTrueType, typename TPFalseType>
	struct conditional_type
	{
		using type = typename std::conditional<tpBoolean, TPTrueType, TPFalseType>::type;
	};

	/// @brief
	template <typename TPValue, bool tpBoolean, TPValue tpTrueVal, TPValue tpFalseVal>
	struct conditional_value;
	
	template <typename TPValue, TPValue tpTrueVal, TPValue tpFalseVal>
	struct conditional_value<TPValue, true, tpTrueVal, tpFalseVal>
	{
		static constexpr TPValue value = tpTrueVal;
	};
	
	template <typename TPValue, TPValue tpTrueVal, TPValue tpFalseVal>
	struct conditional_value<TPValue, false, tpTrueVal, tpFalseVal>
	{
		static constexpr TPValue value = tpFalseVal;
	};


	/// @brief
	template <int... tpSequence>
	struct integer_sequence
	{};

	template <int tpN, int... tpSequence>
	struct integer_sequence_generator : integer_sequence_generator<tpN - 1, tpN - 1, tpSequence...>
	{};

	template <int... tpSequence>
	struct integer_sequence_generator<0, tpSequence...>
	{
		using type = integer_sequence<tpSequence...>;
	};


	/// @brief Contains a member typedef `type`, which is the smallest signed integral type of at least specified size (in bytes)
	template <size_t tpSize>
	struct int_type_by_bits
	{
		using type = typename conditional_type<
	        tpSize <= ( sizeof( int8 ) * CHAR_BIT ), int8, typename conditional_type<
                tpSize <= ( sizeof( int16 ) * CHAR_BIT  ), int16, typename conditional_type<
                    tpSize <= ( sizeof( int32 ) * CHAR_BIT ), int32, typename conditional_type<
						tpSize <= ( sizeof( int64 ) * CHAR_BIT ), int64, void>::type>::type>::type>::type;
	};

	/// @brief Contains a member typedef `type`, which is the smallest unsigned integral type of at least specified size (in bytes)
	template <size_t tpSize>
	struct uint_type_by_bits
	{
		using type = typename conditional_type<
	        tpSize <= ( sizeof( uint8 ) * CHAR_BIT ), uint8, typename conditional_type<
                tpSize <= ( sizeof( uint16 ) * CHAR_BIT ), uint16, typename conditional_type<
                    tpSize <= ( sizeof( uint32 ) * CHAR_BIT ), uint32, typename conditional_type<
						tpSize <= ( sizeof( uint64 ) * CHAR_BIT ), uint64, void>::type>::type>::type>::type;
	};


	/// @brief Contains a member typedef `type`, which is the smallest signed integral type of at least specified size (in bytes)
	template <size_t tpSize>
	struct int_type_by_size
	{
		using type = typename conditional_type<
	        tpSize <= sizeof( int8 ), int8, typename conditional_type<
                tpSize <= sizeof( int16 ), int16, typename conditional_type<
                    tpSize <= sizeof( int32 ), int32, typename conditional_type<
						tpSize <= sizeof( int64 ), int64, void>::type>::type>::type>::type;
	};

	/// @brief Contains a member typedef `type`, which is the smallest unsigned integral type of at least specified size (in bytes)
	template <size_t tpSize>
	struct uint_type_by_size
	{
		using type = typename conditional_type<
	        tpSize <= sizeof( uint8 ), uint8, typename conditional_type<
                tpSize <= sizeof( uint16 ), uint16, typename conditional_type<
                    tpSize <= sizeof( uint32 ), uint32, typename conditional_type<
						tpSize <= sizeof( uint64 ), uint64, void>::type>::type>::type>::type;
	};

	/// @brief Contains a member typedef `type`, which is the smallest signed integral type required to hold the specified value.
	template <intmax_t tpValue>
	struct int_type_by_value
	{
		using type = typename conditional_type<
	        ( tpValue >= limits<int8>::min_value ) && ( tpValue <= limits<int8>::max_value ), int8, typename conditional_type<
                ( tpValue >= limits<int16>::min_value ) && ( tpValue <= limits<int16>::max_value ), int16, typename conditional_type<
                    ( tpValue >= limits<int32>::min_value ) && ( tpValue <= limits<int32>::max_value ), int32, int64>::type>::type>::type;
	};

	/// @brief Contains a member typedef `type`, which is the smallest unsigned integral type required to hold the specified value
	template <uintmax_t tpValue>
	struct uint_type_by_value
	{
		using type = typename conditional_type<
	        tpValue <= limits<uint8>::max_value, uint8, typename conditional_type<
                tpValue <= limits<uint16>::max_value, uint16, typename conditional_type<
                    tpValue <= limits<uint32>::max_value, uint32, uint64>::type>::type>::type;
	};


	template <size_t tpTypeSize, bool tpIsInteger>
	struct signed_unsigned_types_proxy;

	template <>
	struct signed_unsigned_types_proxy<1, true>
	{
		using signed_type = signed char;
		using unsigned_type = unsigned char;
	};

	template <>
	struct signed_unsigned_types_proxy<2, true>
	{
		using signed_type = int16_t;
		using unsigned_type = uint16_t;
	};

	template <>
	struct signed_unsigned_types_proxy<4, true>
	{
		using signed_type = int32_t;
		using unsigned_type = uint32_t;
	};

	template <>
	struct signed_unsigned_types_proxy<8, true>
	{
		using signed_type = int64_t;
		using unsigned_type = uint64_t;
	};

	template <>
	struct signed_unsigned_types_proxy<sizeof( float ), false>
	{
		using signed_type = float;
		using unsigned_type = float;
	};

	template <>
	struct signed_unsigned_types_proxy<sizeof( double ), false>
	{
		using signed_type = double;
		using unsigned_type = double;
	};


	template <typename TPNumeric>
	struct signed_type_equivalent
	{
		using type = typename signed_unsigned_types_proxy<sizeof( TPNumeric ), std::is_integral<TPNumeric>::value>::signed_type;
	};

	template <typename TPNumeric>
	struct unsigned_type_equivalent
	{
		using type = typename signed_unsigned_types_proxy<sizeof( TPNumeric ), std::is_integral<TPNumeric>::value>::unsigned_type;
	};


	template <typename TPFirst, typename TPSecond>
	struct is_same_decay
	{
		static constexpr auto value = std::is_same<
		        typename std::remove_cv<typename std::decay<TPFirst>::type>::type,
				typename std::remove_cv<typename std::decay<TPSecond>::type>::type>::value;
	};


	template <typename TPType, typename... TList>
	struct is_type_on_type_list;

	template <typename TPType, typename TCheck>
	struct is_type_on_type_list<TPType, TCheck>
	{
		static constexpr bool value = std::is_same<TPType, TCheck>::value;
	};

	template <typename TPType, typename TCheck, typename... TList>
	struct is_type_on_type_list<TPType, TCheck, TList...>
	{
		static constexpr bool value = std::is_same<TPType, TCheck>::value || is_type_on_type_list<TPType, TList...>::value;
	};


	/// @brief Contains a static boolean variable `value`, which yields true if specified type list `TPTypes` contains `void` or false otherwise.
	template <typename... TPTypes>
	struct is_void_on_type_list;

	template <typename TPValue>
	struct is_void_on_type_list<TPValue>
	{
		static constexpr bool value = std::is_void<TPValue>::value;
	};

	template <typename TPValue, typename... TPRest>
	struct is_void_on_type_list<TPValue, TPRest...>
	{
		static constexpr bool value = std::is_void<TPValue>::value || is_void_on_type_list<TPRest...>::value;
	};


	/// @brief Contains a member typedef `type`, which is the first type from `TPTypes...` which matches `TPPredicate`
	template <template<typename> class TPPredicate, typename... TPTypes>
	struct first_matching_type;

	template <template<typename> class TPPredicate, typename TPValue>
	struct first_matching_type<TPPredicate, TPValue>
	{
		using type = typename conditional_type<TPPredicate<TPValue>::value, TPValue, void>::type;
	};

	template <template<typename> class TPPredicate, typename TPValue, typename... TPRest>
	struct first_matching_type<TPPredicate, TPValue, TPRest...>
	{
		using type = typename conditional_type<TPPredicate<TPValue>::value, TPValue, typename first_matching_type<TPPredicate, TPRest...>::type>::type;
	};

}

#endif /* __CPPX_TYPE_TRAITS_H__ */
