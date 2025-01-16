
#pragma once

#include "prerequisites.h"
#include <chrono>

namespace cppx
{

	using duration_value_t = uint64;

	enum class duration_period : enum_default_value_t
	{
		nanosecond,
		microsecond,
		millisecond,
		second
	};

	template <duration_period>
	struct duration_type_wrapper;

	template <>
	struct duration_type_wrapper<duration_period::nanosecond>
	{
		using type = std::chrono::nanoseconds;
	};

	template <>
	struct duration_type_wrapper<duration_period::microsecond>
	{
		using type = std::chrono::microseconds;
	};

	template <>
	struct duration_type_wrapper<duration_period::millisecond>
	{
		using type = std::chrono::milliseconds;
	};

	template <>
	struct duration_type_wrapper<duration_period::second>
	{
		using type = std::chrono::seconds;
	};

	template <duration_period tpPeriod>
	using duration = typename duration_type_wrapper<tpPeriod>::type;

	template <duration_period tpPeriod>
	struct duration_traits
	{
		using ratio_type = typename duration_type_wrapper<tpPeriod>::type::period;

		static constexpr std::pair<native_int, native_int> unit_ratio {
			static_cast<native_int>( ratio_type::num ),
			static_cast<native_int>( ratio_type::den )
		};
	};

	using nanoseconds = duration<duration_period::nanosecond>;
	using microseconds = duration<duration_period::microsecond>;
	using milliseconds = duration<duration_period::millisecond>;
	using seconds = duration<duration_period::second>;

	/// @brief Infinite timeout expressed as Nanoseconds value.
	inline constexpr nanoseconds timeout_infinite_ns { cve::int64_max };

	/// @brief Infinite timeout expressed as Microseconds value.
	inline constexpr microseconds timeout_infinite_us { cve::int64_max };

	/// @brief Infinite timeout expressed as Milliseconds value.
	inline constexpr milliseconds timeout_infinite_ms { cve::int64_max };

	/// @brief Infinite timeout expressed as Seconds value.
	inline constexpr seconds timeout_infinite_sec { cve::int64_max };

}
