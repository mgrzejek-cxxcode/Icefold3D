
#ifndef __IC3_CPPX_STATIC_ALGO_H__
#define __IC3_CPPX_STATIC_ALGO_H__

#include "Prerequisites.h"
#include <numeric>

namespace Ic3::Cppx
{

	template <typename TP1, typename TP2 = TP1>
	struct TCmpEqual
	{
		constexpr bool operator()( const TP1 & pLhs, const TP2 & pRhs ) const
		{
			return pLhs == pRhs;
		}
	};

	template <typename TP1>
	struct TCmpEqualAuto
	{
		template <typename TP2>
		constexpr bool operator()( const TP1 & pLhs, const TP2 & pRhs ) const
		{
			return pLhs == pRhs;
		}
	};

	template <typename TP1, typename TP2 = TP1>
	struct TCmpNotEqual
	{
		constexpr bool operator()( const TP1 & pLhs, const TP2 & pRhs ) const
		{
			return pLhs != pRhs;
		}
	};

	template <typename TP1>
	struct TCmpNotEqualAuto
	{
		template <typename TP2>
		constexpr bool operator()( const TP1 & pLhs, const TP2 & pRhs ) const
		{
			return pLhs != pRhs;
		}
	};

	template <typename TP1, typename TP2 = TP1>
	struct TCmpLess
	{
		constexpr bool operator()( const TP1 & pLhs, const TP2 & pRhs ) const
		{
			return pLhs < pRhs;
		}
	};

	template <typename TP1>
	struct TCmpLessAuto
	{
		template <typename TP2>
		constexpr bool operator()( const TP1 & pLhs, const TP2 & pRhs ) const
		{
			return pLhs < pRhs;
		}
	};

	template <typename TP1, typename TP2 = TP1>
	struct TCmpLessEqual
	{
		constexpr bool operator()( const TP1 & pLhs, const TP2 & pRhs ) const
		{
			return pLhs <= pRhs;
		}
	};

	template <typename TP1>
	struct TCmpLessEqualAuto
	{
		template <typename TP2>
		constexpr bool operator()( const TP1 & pLhs, const TP2 & pRhs ) const
		{
			return pLhs <= pRhs;
		}
	};

	template <typename TP1, typename TP2 = TP1>
	struct TCmpGreater
	{
		constexpr bool operator()( const TP1 & pLhs, const TP2 & pRhs ) const
		{
			return pLhs > pRhs;
		}
	};

	template <typename TP1>
	struct TCmpGreaterAuto
	{
		template <typename TP2>
		constexpr bool operator()( const TP1 & pLhs, const TP2 & pRhs ) const
		{
			return pLhs > pRhs;
		}
	};

	template <typename TP1, typename TP2 = TP1>
	struct TCmpGreaterEqual
	{
		constexpr bool operator()( const TP1 & pLhs, const TP2 & pRhs ) const
		{
			return pLhs >= pRhs;
		}
	};

	template <typename TP1>
	struct TCmpGreaterEqualAuto
	{
		template <typename TP2>
		constexpr bool operator()( const TP1 & pLhs, const TP2 & pRhs ) const
		{
			return pLhs >= pRhs;
		}
	};

	template <intmax_t tpValue>
	struct StaticAbs
	{
		static constexpr intmax_t value = ( tpValue < 0 ) ? -tpValue : tpValue;
	};

	template <intmax_t tpValue>
	struct StaticSignOf
	{
		static constexpr int32_t value = ( tpValue < 0 ) ? -1 : 1;
	};

	template <intmax_t tpValue1, intmax_t tpValue2, intmax_t... tpValues>
	struct StaticMax
	{
		static constexpr intmax_t value = StaticMax<tpValue1, StaticMax<tpValue2, tpValues...>::value>::value;
	};

	template <intmax_t tpValue1, intmax_t tpValue2>
	struct StaticMax<tpValue1, tpValue2>
	{
		static constexpr intmax_t value = tpValue1 >= tpValue2 ? tpValue1 : tpValue2;
	};

	template <intmax_t tpValue1, intmax_t tpValue2, intmax_t... tpValues>
	struct StaticMin
	{
		static constexpr intmax_t value = StaticMin<StaticMin<tpValue1, tpValue2>::value, tpValues...>::value;
	};

	template <intmax_t tpValue1, intmax_t tpValue2>
	struct StaticMin<tpValue1, tpValue2>
	{
		static constexpr intmax_t value = tpValue1 <= tpValue2 ? tpValue1 : tpValue2;
	};

	template <typename... TPTypes>
	struct StaticMaxSizeofT
	{
		static constexpr size_t value = static_cast< size_t >( StaticMax<sizeof( TPTypes )...>::value );
	};

	template <intmax_t... tpValues>
	struct StaticMaxSizeofVal
	{
		static constexpr size_t value = StaticMaxSizeofT<typename QIntTypeByValue<tpValues>::Type...>::value;
	};

	template <typename... TPTypes>
	struct StaticMaxAlignOfT
	{
		static constexpr size_t value = static_cast< size_t >( StaticMax<alignof( TPTypes )...>::value );
	};

	template <intmax_t... tpValues>
	struct StaticMaxAlignOfVal
	{
		static constexpr size_t value = StaticMaxAlignOfT<typename QIntTypeByValue<tpValues>::Type...>::value;
	};

}

#endif /* __IC3_CPPX_STATIC_ALGO_H__ */
