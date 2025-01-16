
#ifndef __CPPX_STATIC_ALGO_H__
#define __CPPX_STATIC_ALGO_H__

#include "typeTraits.h"
#include <numeric>

namespace cppx
{

	template <typename TP1, typename TP2 = TP1>
	struct cmp_equal
	{
		constexpr bool operator()( const TP1 & pLhs, const TP2 & pRhs ) const
		{
			return pLhs == pRhs;
		}
	};

	template <typename TP1>
	struct cmp_equal_auto
	{
		template <typename TP2>
		constexpr bool operator()( const TP1 & pLhs, const TP2 & pRhs ) const
		{
			return pLhs == pRhs;
		}
	};

	template <typename TP1, typename TP2 = TP1>
	struct cmp_not_equal
	{
		constexpr bool operator()( const TP1 & pLhs, const TP2 & pRhs ) const
		{
			return pLhs != pRhs;
		}
	};

	template <typename TP1>
	struct cmp_not_equal_auto
	{
		template <typename TP2>
		constexpr bool operator()( const TP1 & pLhs, const TP2 & pRhs ) const
		{
			return pLhs != pRhs;
		}
	};

	template <typename TP1, typename TP2 = TP1>
	struct cmp_less
	{
		constexpr bool operator()( const TP1 & pLhs, const TP2 & pRhs ) const
		{
			return pLhs < pRhs;
		}
	};

	template <typename TP1>
	struct cmp_less_auto
	{
		template <typename TP2>
		constexpr bool operator()( const TP1 & pLhs, const TP2 & pRhs ) const
		{
			return pLhs < pRhs;
		}
	};

	template <typename TP1, typename TP2 = TP1>
	struct cmp_less_equal
	{
		constexpr bool operator()( const TP1 & pLhs, const TP2 & pRhs ) const
		{
			return pLhs <= pRhs;
		}
	};

	template <typename TP1>
	struct cmp_less_equal_auto
	{
		template <typename TP2>
		constexpr bool operator()( const TP1 & pLhs, const TP2 & pRhs ) const
		{
			return pLhs <= pRhs;
		}
	};

	template <typename TP1, typename TP2 = TP1>
	struct cmp_greater
	{
		constexpr bool operator()( const TP1 & pLhs, const TP2 & pRhs ) const
		{
			return pLhs > pRhs;
		}
	};

	template <typename TP1>
	struct cmp_greater_auto
	{
		template <typename TP2>
		constexpr bool operator()( const TP1 & pLhs, const TP2 & pRhs ) const
		{
			return pLhs > pRhs;
		}
	};

	template <typename TP1, typename TP2 = TP1>
	struct cmp_greater_equal
	{
		constexpr bool operator()( const TP1 & pLhs, const TP2 & pRhs ) const
		{
			return pLhs >= pRhs;
		}
	};

	template <typename TP1>
	struct cmp_greater_equal_auto
	{
		template <typename TP2>
		constexpr bool operator()( const TP1 & pLhs, const TP2 & pRhs ) const
		{
			return pLhs >= pRhs;
		}
	};

	template <intmax_t tpValue>
	struct static_abs
	{
		static constexpr intmax_t value = ( tpValue < 0 ) ? -tpValue : tpValue;
	};

	template <intmax_t tpValue>
	struct static_sign_of
	{
		static constexpr int32_t value = ( tpValue < 0 ) ? -1 : 1;
	};

	template <intmax_t tpValue1, intmax_t tpValue2, intmax_t... tpValues>
	struct static_max
	{
		static constexpr intmax_t value = static_max<tpValue1, static_max<tpValue2, tpValues...>::value>::value;
	};

	template <intmax_t tpValue1, intmax_t tpValue2>
	struct static_max<tpValue1, tpValue2>
	{
		static constexpr intmax_t value = tpValue1 >= tpValue2 ? tpValue1 : tpValue2;
	};

	template <intmax_t tpValue1, intmax_t tpValue2, intmax_t... tpValues>
	struct static_min
	{
		static constexpr intmax_t value = static_min<static_min<tpValue1, tpValue2>::value, tpValues...>::value;
	};

	template <intmax_t tpValue1, intmax_t tpValue2>
	struct static_min<tpValue1, tpValue2>
	{
		static constexpr intmax_t value = tpValue1 <= tpValue2 ? tpValue1 : tpValue2;
	};

	template <typename... TPTypes>
	struct static_sizeof_type
	{
		static constexpr size_t value = static_cast< size_t >( static_max<sizeof( TPTypes )...>::value );
	};

	template <intmax_t... tpValues>
	struct static_sizeof_value
	{
		static constexpr size_t value = static_sizeof_type<typename meta::int_type_by_value<tpValues>::Type...>::value;
	};

	template <typename... TPTypes>
	struct static_alignof_type
	{
		static constexpr size_t value = static_cast< size_t >( static_max<alignof( TPTypes )...>::value );
	};

	template <intmax_t... tpValues>
	struct static_alignof_value
	{
		static constexpr size_t value = static_alignof_type<typename meta::int_type_by_value<tpValues>::Type...>::value;
	};

}

#endif /* __CPPX_STATIC_ALGO_H__ */
