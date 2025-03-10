
#pragma once

#include "typeTraits.h"
#include <chrono>

namespace cppx
{

	using duration_value_t = native_uint;

	enum class duration_unit : enum_default_value_t
	{
		nanosecond,
		microsecond,
		millisecond,
		second
	};

	template <duration_unit tpUnit, typename TPValue>
	class duration;

	template <duration_unit tpUnit, typename TPValue>
	struct duration_underlying_type_proxy;

	template <typename TPValue>
	struct duration_underlying_type_proxy<duration_unit::nanosecond, TPValue>
	{
		using type = std::chrono::duration<TPValue, std::nano>;
	};

	template <typename TPValue>
	struct duration_underlying_type_proxy<duration_unit::microsecond, TPValue>
	{
		using type = std::chrono::duration<TPValue, std::micro>;
	};

	template <typename TPValue>
	struct duration_underlying_type_proxy<duration_unit::millisecond, TPValue>
	{
		using type = std::chrono::duration<TPValue, std::milli>;
	};

	template <typename TPValue>
	struct duration_underlying_type_proxy<duration_unit::second, TPValue>
	{
		using type = std::chrono::duration<TPValue>;
	};

	template <duration_unit tpUnit, typename TPValue>
	struct duration_traits
	{
		using underlying_type = typename duration_underlying_type_proxy<tpUnit, TPValue>::type;
		using ratio_type = typename underlying_type::period;
		using value_type = typename underlying_type::rep;

		static constexpr std::pair<native_int, native_int> unit_ratio {
			static_cast<native_int>( ratio_type::num ),
			static_cast<native_int>( ratio_type::den )
		};

		template <duration_unit tpOtherPeriod, typename TPOtherValue>
		static constexpr typename duration_traits<tpOtherPeriod, TPOtherValue>::value_type convert_to( value_type pValue ) noexcept
		{
			using RR /* Result Ratio */ = typename duration_traits<tpOtherPeriod, TPOtherValue>::ratio_type;
			using RV /* Result Value */ = typename duration_traits<tpOtherPeriod, TPOtherValue>::value_type;
			using DR /* Divide Ratio */ = std::ratio_divide<ratio_type, RR>;
			using CVT /* Common Value Type */ = std::common_type_t<RV, value_type, intmax_t>;

			constexpr bool divide_ratio_num_is_1 = DR::num == 1;
			constexpr bool divide_ratio_den_is_1 = DR::den == 1;

			if constexpr( divide_ratio_den_is_1 )
			{
				if constexpr ( divide_ratio_num_is_1 )
				{
					return static_cast<RV>( pValue );
				}
				else
				{
					return static_cast<RV>( static_cast<CVT>( pValue ) * static_cast<CVT>( DR::num ) );
				}
			}
			else
			{
				if constexpr( divide_ratio_num_is_1 )
				{
					return static_cast<RV>( static_cast<CVT>( pValue ) / static_cast<CVT>( DR::den ) );
				}
				else
				{
					return static_cast<RV>( static_cast<CVT>( pValue ) * static_cast<CVT>( DR::num ) / static_cast<CVT>( DR::den ) );
				}
			}
		}
	};

	/**
	 * Provides a static duration_unit value which represents a unit that should be used when working
	 * with durations with different base units. This ensures no precision lost in computations or comparisons.
	 * @tparam tpUnit1
	 * @tparam tpUnit2
	 */
	template <duration_unit tpUnit1, duration_unit tpUnit2>
	struct duration_common_unit_proxy
	{
		static constexpr auto value = static_cast<duration_unit>( get_min_of( static_cast<uint32>( tpUnit1 ), static_cast<uint32>( tpUnit2 ) ) );
	};

	template <duration_unit tpUnit>
	struct duration_common_unit_proxy<tpUnit, tpUnit>
	{
		static constexpr auto value = tpUnit;
	};

	template <duration_unit tpUnit1, duration_unit tpUnit2>
	inline constexpr auto duration_common_unit = duration_common_unit_proxy<tpUnit1, tpUnit2>::value;

	/**
	 * 
	 * @tparam TPValue1 
	 * @tparam TPValue2 
	 */
	template <typename TPValue1, typename TPValue2>
	struct duration_common_value_type_proxy
	{
		using type = typename std::common_type<TPValue1, TPValue2, intmax_t>::type;
	};

	template <typename TPValue>
	struct duration_common_value_type_proxy<TPValue, TPValue>
	{
		using type = TPValue;
	};

	template <typename TPValue1, typename TPValue2>
	using duration_common_value_type = typename duration_common_value_type_proxy<TPValue1, TPValue2>::type;

	template <typename TPValue1, typename TPValue2>
	using duration_common_value_type_signed = typename meta::signed_type_equivalent<duration_common_value_type<TPValue1, TPValue2>>::type;

	/**
	 * 
	 * @tparam tpUnit1 
	 * @tparam TPValue1 
	 * @tparam tpUnit2 
	 * @tparam TPValue2 
	 */
	template <duration_unit tpUnit1, typename TPValue1, duration_unit tpUnit2, typename TPValue2>
	struct duration_common_type_proxy
	{
		using type = duration<duration_common_unit<tpUnit1, tpUnit2>, duration_common_value_type<TPValue1, TPValue2>>;
	};

	/**
	 * 
	 * @tparam tpUnit1 
	 * @tparam TPValue1 
	 * @tparam tpUnit2 
	 * @tparam TPValue2 
	 */
	template <duration_unit tpUnit1, typename TPValue1, duration_unit tpUnit2, typename TPValue2>
	struct duration_common_type_signed_proxy
	{
		using type = duration<duration_common_unit<tpUnit1, tpUnit2>, duration_common_value_type_signed<TPValue1, TPValue2>>;
	};

	template <duration_unit tpUnit1, typename TPValue1, duration_unit tpUnit2, typename TPValue2>
	using duration_common_type = typename duration_common_type_proxy<tpUnit1, TPValue1, tpUnit2, TPValue2>::type;

	template <typename TPDuration1, typename TPDuration2>
	using duration_common_type_auto = duration_common_type<TPDuration1::unit, typename TPDuration1::value_type, TPDuration2::unit, typename TPDuration2::value_type>;

	template <duration_unit tpUnit1, typename TPValue1, duration_unit tpUnit2, typename TPValue2>
	using duration_common_type_signed = typename duration_common_type_signed_proxy<tpUnit1, TPValue1, tpUnit2, TPValue2>::type;

	template <typename TPDuration1, typename TPDuration2>
	using duration_common_type_signed_auto = duration_common_type<TPDuration1::unit, typename TPDuration1::value_type, TPDuration2::unit, typename TPDuration2::value_type>;

	template <typename TPRatio>
	struct duration_unit_std_ratio_proxy;

	template <>
	struct duration_unit_std_ratio_proxy<std::nano>
	{
		static constexpr auto unit = duration_unit::nanosecond;
	};

	template <>
	struct duration_unit_std_ratio_proxy<std::micro>
	{
		static constexpr auto unit = duration_unit::microsecond;
	};

	template <>
	struct duration_unit_std_ratio_proxy<std::milli>
	{
		static constexpr auto unit = duration_unit::millisecond;
	};

	template <>
	struct duration_unit_std_ratio_proxy<std::ratio<1>>
	{
		static constexpr auto unit = duration_unit::second;
	};

	template <duration_unit tpUnit, typename TPValue = duration_value_t>
	class duration
	{
	public:
		using traits_type = duration_traits<tpUnit, TPValue>;
		using underlying_type = typename traits_type::underlying_type;
		using ratio_type = typename traits_type::ratio_type;
		using value_type = typename underlying_type::rep;

		static constexpr auto unit = tpUnit;

		constexpr duration()
		: _underlyingDuration( 0 )
		{}

		constexpr explicit duration( TPValue pValue )
		: _underlyingDuration( pValue )
		{}

		template <typename TPRep, typename TPRatio>
		explicit duration( const std::chrono::duration<TPRep, TPRatio> & pStdDuration )
		: _underlyingDuration( std::chrono::duration_cast<underlying_type>( pStdDuration ) )
		{}

		CPPX_ATTR_NO_DISCARD operator const underlying_type & () const noexcept
		{
			return _underlyingDuration;
		}

		CPPX_ATTR_NO_DISCARD TPValue get_count() const noexcept
		{
			return _underlyingDuration.count();
		}

		CPPX_ATTR_NO_DISCARD const underlying_type & get_std() const noexcept
		{
			return _underlyingDuration;
		}

		template <duration_unit tpResultUnit, typename TPResultValue = TPValue>
		CPPX_ATTR_NO_DISCARD duration<tpResultUnit, TPResultValue> get_as() const noexcept
		{
			return duration<tpResultUnit, TPResultValue>{ traits_type::template convert_to<tpResultUnit, TPResultValue>( get_count() ) };
		}

		template <typename TPDuration>
		duration<TPDuration::unit, typename TPDuration::value_type> get_as() const noexcept
		{
			return get_as<TPDuration::unit, typename TPDuration::value_type>();
		}

		template <typename TPResultValue = TPValue>
		duration<duration_unit::nanosecond, TPResultValue> get_as_nano() const noexcept
		{
			return get_as<duration_unit::nanosecond, TPResultValue>();
		}

		template <typename TPResultValue = TPValue>
		duration<duration_unit::microsecond, TPResultValue> get_as_micro() const noexcept
		{
			return get_as<duration_unit::microsecond, TPResultValue>();
		}

		template <typename TPResultValue = TPValue>
		duration<duration_unit::millisecond, TPResultValue> get_as_milli() const noexcept
		{
			return get_as<duration_unit::millisecond, TPResultValue>();
		}

		template <typename TPResultValue = TPValue>
		duration<duration_unit::second, TPResultValue> get_as_seconds() const noexcept
		{
			return get_as<duration_unit::second, TPResultValue>();
		}

		template <duration_unit tpResultUnit, typename TPResultValue = TPValue>
		TPResultValue get_count_as() const noexcept
		{
			return get_as<tpResultUnit, TPResultValue>().get_count();
		}

		template <typename TPDuration>
		typename TPDuration::value_type get_count_as() const noexcept
		{
			return get_as<TPDuration>().get_count();
		}

		template <typename TPResultValue = TPValue>
		TPResultValue get_count_as_nano() const noexcept
		{
			return get_count_as<duration_unit::nanosecond, TPResultValue>();
		}

		template <typename TPResultValue = TPValue>
		TPResultValue get_count_as_micro() const noexcept
		{
			return get_count_as<duration_unit::microsecond, TPResultValue>();
		}

		template <typename TPResultValue = TPValue>
		TPResultValue get_count_as_milli() const noexcept
		{
			return get_count_as<duration_unit::millisecond, TPResultValue>();
		}

		template <typename TPResultValue = TPValue>
		TPResultValue get_count_as_seconds() const noexcept
		{
			return get_count_as<duration_unit::second, TPResultValue>();
		}

	private:
		underlying_type _underlyingDuration;
	};

	
	template <duration_unit tpUnit, typename TPValue>
	inline duration<tpUnit, TPValue> operator+( const duration<tpUnit, TPValue> & pLhs, const duration<tpUnit, TPValue> & pRhs ) noexcept
	{
		return duration<tpUnit, TPValue>{ pLhs.get_count() + pRhs.get_count() };
	}

	template <duration_unit tpUnit, typename TPValue1, typename TPValue2>
	inline duration_common_type<tpUnit, TPValue1, tpUnit, TPValue2> operator+(
		const duration<tpUnit, TPValue1> & pLhs,
		const duration<tpUnit, TPValue1> & pRhs ) noexcept
	{
		using result_type = duration_common_type<tpUnit, TPValue1, tpUnit, TPValue2>;
		using value_type = typename result_type::value_type;
		return result_type{ static_cast<value_type>( pLhs.get_count() ) + static_cast<value_type>( pRhs.get_count() ) };
	}

	template <duration_unit tpUnit1, typename TPValue1, duration_unit tpUnit2, typename TPValue2>
	inline duration_common_type<tpUnit1, TPValue1, tpUnit2, TPValue2> operator+(
		const duration<tpUnit1, TPValue1> & pLhs,
		const duration<tpUnit2, TPValue1> & pRhs ) noexcept
	{
		using result_type = duration_common_type<tpUnit1, TPValue1, tpUnit2, TPValue2>;
		using value_type = typename result_type::value_type;
		return result_type{ static_cast<value_type>( pLhs.get_count() ) + static_cast<value_type>( pRhs.get_count() ) };
	}

	template <duration_unit tpUnit, typename TPValue>
	inline duration<tpUnit, typename meta::signed_type_equivalent<TPValue>::type> operator-(
		const duration<tpUnit, TPValue> & pLhs,
		const duration<tpUnit, TPValue> & pRhs ) noexcept
	{
		using result_type = duration<tpUnit, typename meta::signed_type_equivalent<TPValue>::type>;
		using value_type = typename result_type::value_type;
		return duration<tpUnit, value_type>{ static_cast<value_type>( pLhs.get_count() ) + static_cast<value_type>( pRhs.get_count() ) };
	}

	template <duration_unit tpUnit, typename TPValue1, typename TPValue2>
	inline duration<tpUnit, duration_common_value_type_signed<TPValue1, TPValue2>> operator-(
		const duration<tpUnit, TPValue1> & pLhs,
		const duration<tpUnit, TPValue1> & pRhs ) noexcept
	{
		using result_type = duration<tpUnit, duration_common_value_type_signed<TPValue1, TPValue2>>;
		using value_type = typename result_type::value_type;
		return result_type{ static_cast<value_type>( pLhs.get_count() ) - static_cast<value_type>( pRhs.get_count() ) };
	}

	template <duration_unit tpUnit1, typename TPValue1, duration_unit tpUnit2, typename TPValue2>
	inline duration_common_type<tpUnit1, TPValue1, tpUnit2, TPValue2> operator-(
		const duration<tpUnit1, TPValue1> & pLhs,
		const duration<tpUnit2, TPValue1> & pRhs ) noexcept
	{
		using result_type = duration_common_type_signed<tpUnit1, TPValue1, tpUnit2, TPValue2>;
		using value_type = typename result_type::value_type;
		return result_type{ static_cast<value_type>( pLhs.get_count() ) - static_cast<value_type>( pRhs.get_count() ) };
	}


	template <duration_unit tpUnit, typename TPValue1, typename TPValue2>
	inline bool operator==( const duration<tpUnit, TPValue1> & pLhs, const duration<tpUnit, TPValue2> & pRhs ) noexcept
	{
		using common_value_type = duration_common_value_type<TPValue1, TPValue2>;
		return static_cast<common_value_type>( pLhs.get_count() ) == static_cast<common_value_type>( pRhs.get_count() );
	}

	template <duration_unit tpUnit1, typename TPValue1, duration_unit tpUnit2, typename TPValue2>
	inline bool operator==( const duration<tpUnit1, TPValue1> & pLhs, const duration<tpUnit2, TPValue2> & pRhs ) noexcept
	{
		using common_type = duration_common_type<tpUnit1, TPValue1, tpUnit2, TPValue2>;
		return pLhs.template get_as<common_type>() == pRhs.template get_as<common_type>();
	}

	template <duration_unit tpUnit, typename TPValue1, typename TPValue2>
	inline bool operator==( const duration<tpUnit, TPValue1> & pLhs, const TPValue2 pRhs ) noexcept
	{
		using common_value_type = typename std::common_type<TPValue1, TPValue2>::type;
		return static_cast<common_value_type>( pLhs.get_count() ) == static_cast<common_value_type>( pRhs );
	}

	template <duration_unit tpUnit, typename TPValue1, typename TPValue2>
	inline bool operator==( const TPValue1 pLhs, const duration<tpUnit, TPValue2> & pRhs ) noexcept
	{
		using common_value_type = typename std::common_type<TPValue1, TPValue2>::type;
		return static_cast<common_value_type>( pLhs ) == static_cast<common_value_type>( pRhs.get_count() );
	}


	template <duration_unit tpUnit, typename TPValue1, typename TPValue2>
	inline bool operator!=( const duration<tpUnit, TPValue1> & pLhs, const duration<tpUnit, TPValue2> & pRhs ) noexcept
	{
		using common_value_type = typename std::common_type<TPValue1, TPValue2>::type;
		return static_cast<common_value_type>( pLhs.get_count() ) != static_cast<common_value_type>( pRhs.get_count() );
	}

	template <duration_unit tpUnit1, typename TPValue1, duration_unit tpUnit2, typename TPValue2>
	inline bool operator!=( const duration<tpUnit1, TPValue1> & pLhs, const duration<tpUnit2, TPValue2> & pRhs ) noexcept
	{
		using common_type = duration_common_type<tpUnit1, TPValue1, tpUnit2, TPValue2>;
		return pLhs.template get_as<common_type>() != pRhs.template get_as<common_type>();
	}

	template <duration_unit tpUnit, typename TPValue1, typename TPValue2>
	inline bool operator!=( const duration<tpUnit, TPValue1> & pLhs, const TPValue2 pRhs ) noexcept
	{
		using common_value_type = typename std::common_type<TPValue1, TPValue2>::type;
		return static_cast<common_value_type>( pLhs.get_count() ) != static_cast<common_value_type>( pRhs );
	}

	template <duration_unit tpUnit, typename TPValue1, typename TPValue2>
	inline bool operator!=( const TPValue1 pLhs, const duration<tpUnit, TPValue2> & pRhs ) noexcept
	{
		using common_value_type = typename std::common_type<TPValue1, TPValue2>::type;
		return static_cast<common_value_type>( pLhs ) != static_cast<common_value_type>( pRhs.get_count() );
	}


	template <duration_unit tpUnit, typename TPValue1, typename TPValue2>
	inline bool operator<( const duration<tpUnit, TPValue1> & pLhs, const duration<tpUnit, TPValue2> & pRhs ) noexcept
	{
		using common_value_type = typename std::common_type<TPValue1, TPValue2>::type;
		return static_cast<common_value_type>( pLhs.get_count() ) < static_cast<common_value_type>( pRhs.get_count() );
	}

	template <duration_unit tpUnit1, typename TPValue1, duration_unit tpUnit2, typename TPValue2>
	inline bool operator<( const duration<tpUnit1, TPValue1> & pLhs, const duration<tpUnit2, TPValue2> & pRhs ) noexcept
	{
		using common_type = duration_common_type<tpUnit1, TPValue1, tpUnit2, TPValue2>;
		return pLhs.template get_as<common_type>() < pRhs.template get_as<common_type>();
	}

	template <duration_unit tpUnit, typename TPValue1, typename TPValue2>
	inline bool operator<( const duration<tpUnit, TPValue1> & pLhs, const TPValue2 pRhs ) noexcept
	{
		using common_value_type = typename std::common_type<TPValue1, TPValue2>::type;
		return static_cast<common_value_type>( pLhs.get_count() ) < static_cast<common_value_type>( pRhs );
	}

	template <duration_unit tpUnit, typename TPValue1, typename TPValue2>
	inline bool operator<( const TPValue1 pLhs, const duration<tpUnit, TPValue2> & pRhs ) noexcept
	{
		using common_value_type = typename std::common_type<TPValue1, TPValue2>::type;
		return static_cast<common_value_type>( pLhs ) < static_cast<common_value_type>( pRhs.get_count() );
	}


	template <duration_unit tpUnit, typename TPValue1, typename TPValue2>
	inline bool operator<=( const duration<tpUnit, TPValue1> & pLhs, const duration<tpUnit, TPValue2> & pRhs ) noexcept
	{
		using common_value_type = typename std::common_type<TPValue1, TPValue2>::type;
		return static_cast<common_value_type>( pLhs.get_count() ) <= static_cast<common_value_type>( pRhs.get_count() );
	}

	template <duration_unit tpUnit1, typename TPValue1, duration_unit tpUnit2, typename TPValue2>
	inline bool operator<=( const duration<tpUnit1, TPValue1> & pLhs, const duration<tpUnit2, TPValue2> & pRhs ) noexcept
	{
		using common_type = duration_common_type<tpUnit1, TPValue1, tpUnit2, TPValue2>;
		return pLhs.template get_as<common_type>() <= pRhs.template get_as<common_type>();
	}

	template <duration_unit tpUnit, typename TPValue1, typename TPValue2>
	inline bool operator<=( const duration<tpUnit, TPValue1> & pLhs, const TPValue2 pRhs ) noexcept
	{
		using common_value_type = typename std::common_type<TPValue1, TPValue2>::type;
		return static_cast<common_value_type>( pLhs.get_count() ) <= static_cast<common_value_type>( pRhs );
	}

	template <duration_unit tpUnit, typename TPValue1, typename TPValue2>
	inline bool operator<=( const TPValue1 pLhs, const duration<tpUnit, TPValue2> & pRhs ) noexcept
	{
		using common_value_type = typename std::common_type<TPValue1, TPValue2>::type;
		return static_cast<common_value_type>( pLhs ) <= static_cast<common_value_type>( pRhs.get_count() );
	}


	template <duration_unit tpUnit, typename TPValue1, typename TPValue2>
	inline bool operator>( const duration<tpUnit, TPValue1> & pLhs, const duration<tpUnit, TPValue2> & pRhs ) noexcept
	{
		using common_value_type = typename std::common_type<TPValue1, TPValue2>::type;
		return static_cast<common_value_type>( pLhs.get_count() ) > static_cast<common_value_type>( pRhs.get_count() );
	}

	template <duration_unit tpUnit1, typename TPValue1, duration_unit tpUnit2, typename TPValue2>
	inline bool operator>( const duration<tpUnit1, TPValue1> & pLhs, const duration<tpUnit2, TPValue2> & pRhs ) noexcept
	{
		using common_type = duration_common_type<tpUnit1, TPValue1, tpUnit2, TPValue2>;
		return pLhs.template get_as<common_type>() > pRhs.template get_as<common_type>();
	}

	template <duration_unit tpUnit, typename TPValue1, typename TPValue2>
	inline bool operator>( const duration<tpUnit, TPValue1> & pLhs, const TPValue2 pRhs ) noexcept
	{
		using common_value_type = typename std::common_type<TPValue1, TPValue2>::type;
		return static_cast<common_value_type>( pLhs.get_count() ) > static_cast<common_value_type>( pRhs );
	}

	template <duration_unit tpUnit, typename TPValue1, typename TPValue2>
	inline bool operator>( const TPValue1 pLhs, const duration<tpUnit, TPValue2> & pRhs ) noexcept
	{
		using common_value_type = typename std::common_type<TPValue1, TPValue2>::type;
		return static_cast<common_value_type>( pLhs ) > static_cast<common_value_type>( pRhs.get_count() );
	}


	template <duration_unit tpUnit, typename TPValue1, typename TPValue2>
	inline bool operator>=( const duration<tpUnit, TPValue1> & pLhs, const duration<tpUnit, TPValue2> & pRhs ) noexcept
	{
		using common_value_type = typename std::common_type<TPValue1, TPValue2>::type;
		return static_cast<common_value_type>( pLhs.get_count() ) >= static_cast<common_value_type>( pRhs.get_count() );
	}

	template <duration_unit tpUnit1, typename TPValue1, duration_unit tpUnit2, typename TPValue2>
	inline bool operator>=( const duration<tpUnit1, TPValue1> & pLhs, const duration<tpUnit2, TPValue2> & pRhs ) noexcept
	{
		using common_type = duration_common_type<tpUnit1, TPValue1, tpUnit2, TPValue2>;
		return pLhs.template get_as<common_type>() >= pRhs.template get_as<common_type>();
	}

	template <duration_unit tpUnit, typename TPValue1, typename TPValue2>
	inline bool operator>=( const duration<tpUnit, TPValue1> & pLhs, const TPValue2 pRhs ) noexcept
	{
		using common_value_type = typename std::common_type<TPValue1, TPValue2>::type;
		return static_cast<common_value_type>( pLhs.get_count() ) >= static_cast<common_value_type>( pRhs );
	}

	template <duration_unit tpUnit, typename TPValue1, typename TPValue2>
	inline bool operator>=( const TPValue1 pLhs, const duration<tpUnit, TPValue2> & pRhs ) noexcept
	{
		using common_value_type = typename std::common_type<TPValue1, TPValue2>::type;
		return static_cast<common_value_type>( pLhs ) >= static_cast<common_value_type>( pRhs.get_count() );
	}

	/**
	 * 
	 * @tparam TPRep
	 * @tparam TPRatio 
	 * @param pStdDuration 
	 * @return 
	 */
	template <typename TPRep, typename TPRatio>
	CPPX_ATTR_NO_DISCARD inline constexpr duration<duration_unit_std_ratio_proxy<TPRatio>::unit, TPRep> duration_from_std( const std::chrono::duration<TPRep, TPRatio> & pStdDuration )
	{
		return duration<duration_unit_std_ratio_proxy<TPRatio>::unit, TPRep>{ pStdDuration.count() };
	}

	using nanoseconds = duration<duration_unit::nanosecond>;
	using microseconds = duration<duration_unit::microsecond>;
	using milliseconds = duration<duration_unit::millisecond>;
	using seconds = duration<duration_unit::second>;

	/// @brief Infinite timeout expressed as Nanoseconds value.
	inline constexpr nanoseconds timeout_infinite_ns { meta::limits<duration_value_t>::max_value };

	/// @brief Infinite timeout expressed as Microseconds value.
	inline constexpr microseconds timeout_infinite_us { meta::limits<duration_value_t>::max_value };

	/// @brief Infinite timeout expressed as Milliseconds value.
	inline constexpr milliseconds timeout_infinite_ms { meta::limits<duration_value_t>::max_value };

	/// @brief Infinite timeout expressed as Seconds value.
	inline constexpr seconds timeout_infinite_sec { meta::limits<duration_value_t>::max_value };

}
