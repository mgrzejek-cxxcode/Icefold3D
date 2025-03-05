
#ifndef __CPPX_STRING_EXT_H__
#define __CPPX_STRING_EXT_H__

#include "arrayView.h"
#include <string>

namespace cppx
{

	enum class integer_base : uint32
	{
		auto_detect = 0,
		decimal = 10,
		octal = 8,
		hexadecimal = 16
	};

	/// @brief Creates ArrayView that wraps an std::basic_string object
	/// @tparam TPChar Type of the string's internal characters.
	/// @param pString A string object to be mapped to a view.
	/// @return ArrayView referencing specified string object.
	template <typename TPChar>
	inline array_view<TPChar> bind_array_view( std::basic_string<TPChar> & pString )
	{
		return array_view<TPChar>( &( pString[0] ), pString.length() );
	}

	/// @brief Creates ArrayView that wraps an std::basic_string object
	/// @tparam TPChar Type of the string's internal characters.
	/// @param pString A string object to be mapped to a view.
	/// @return ArrayView referencing specified string object.
	template <typename TPChar>
	inline array_view<const TPChar> bind_array_view( const std::basic_string<TPChar> & pString )
	{
		return array_view<const TPChar>( &( pString[0] ), pString.length() );
	}


	/// @brief
	template <typename TPChar>
	struct to_string_proxy;

	template <>
	struct to_string_proxy<char>
	{
		template <class TPValue>
		static std::basic_string<char> to_string( const TPValue & pValue )
		{
			return std::to_string( pValue );
		}
	};

	template <>
	struct to_string_proxy<wchar_t>
	{
		template <class TPValue>
		static std::basic_string<wchar_t> to_string( const TPValue & pValue )
		{
			return std::to_wstring( pValue );
		}
	};


	template <typename TPValue, typename TPChar>
	struct from_string_conv_result
	{
		TPValue value{};

		uint32 processed_chars_num = 0;

		const TPChar * next_char_ptr = nullptr;

		explicit operator bool() const noexcept
		{
			return processed_chars_num > 0;
		}

		explicit operator TPValue() const noexcept
		{
			return value;
		}

		template <typename TPResult>
		from_string_conv_result<TPResult, TPChar> get_as() const noexcept
		{
			const auto resultValue = numeric_cast<TPResult>( value );
			return { resultValue, processed_chars_num, next_char_ptr };
		}
	};

	/// @brief
	template <typename TPRet>
	struct from_string_proxy;

	template <>
	struct from_string_proxy<int32_t>
	{
		static from_string_conv_result<int32_t, char> from_string( const char * pInputStr, integer_base pBase = integer_base::decimal )
		{
			char * endPtr = nullptr;
			int32_t value = std::strtol( pInputStr, &endPtr, static_cast<int>( pBase ) );
			return { value, numeric_cast<uint32>( endPtr - pInputStr ), ( endPtr != pInputStr ) ? endPtr : nullptr };
		}

		static from_string_conv_result<int32_t, wchar_t> from_string( const wchar_t * pInputStr, integer_base pBase = integer_base::decimal )
		{
			wchar_t * endPtr = nullptr;
			int32_t value = std::wcstol( pInputStr, &endPtr, static_cast<int>( pBase ) );
			return { value, numeric_cast<uint32>( endPtr - pInputStr ), ( endPtr != pInputStr ) ? endPtr : nullptr };
		}

		template <typename TPChar>
		static from_string_conv_result<int32_t, TPChar> from_string( const std::basic_string_view<TPChar> & pInputStr, uintptr_t pOffset = 0u, integer_base pBase = integer_base::decimal )
		{
			return from_string( pInputStr.data() + get_min_of( pOffset, pInputStr.size() ), pBase );
		}

		template <typename TPChar>
		static from_string_conv_result<int32_t, TPChar> from_string( const std::basic_string<TPChar> & pInputStr, uintptr_t pOffset = 0u, integer_base pBase = integer_base::decimal )
		{
			return from_string( pInputStr.data() + get_min_of( pOffset, pInputStr.size() ), pBase );
		}
	};

	template <>
	struct from_string_proxy<uint32_t>
	{
		static from_string_conv_result<uint32_t, char> from_string( const char * pInputStr, integer_base pBase = integer_base::decimal )
		{
			char * endPtr = nullptr;
			uint32_t value = std::strtoul( pInputStr, &endPtr, static_cast<int>( pBase ) );
			return { value, numeric_cast<uint32>( endPtr - pInputStr ), ( endPtr != pInputStr ) ? endPtr : nullptr };
		}

		static from_string_conv_result<uint32_t, wchar_t> from_string( const wchar_t * pInputStr, integer_base pBase = integer_base::decimal )
		{
			wchar_t * endPtr = nullptr;
			uint32_t value = std::wcstoul( pInputStr, &endPtr, static_cast<int>( pBase ) );
			return { value, numeric_cast<uint32>( endPtr - pInputStr ), ( endPtr != pInputStr ) ? endPtr : nullptr };
		}

		template <typename TPChar>
		static from_string_conv_result<uint32_t, TPChar> from_string( const std::basic_string_view<TPChar> & pInputStr, uintptr_t pOffset = 0u, integer_base pBase = integer_base::decimal )
		{
			return from_string( pInputStr.data() + get_min_of( pOffset, pInputStr.size() ), pBase );
		}

		template <typename TPChar>
		static from_string_conv_result<uint32_t, TPChar> from_string( const std::basic_string<TPChar> & pInputStr, uintptr_t pOffset = 0u, integer_base pBase = integer_base::decimal )
		{
			return from_string( pInputStr.data() + get_min_of( pOffset, pInputStr.size() ), pBase );
		}
	};

	template <>
	struct from_string_proxy<int64_t>
	{
		static from_string_conv_result<int64_t, char> from_string( const char * pInputStr, integer_base pBase = integer_base::decimal )
		{
			char * endPtr = nullptr;
			int64_t value = std::strtoll( pInputStr, &endPtr, static_cast<int>( pBase ) );
			return { value, numeric_cast<uint32>( endPtr - pInputStr ), ( endPtr != pInputStr ) ? endPtr : nullptr };
		}

		static from_string_conv_result<int64_t, wchar_t> from_string( const wchar_t * pInputStr, integer_base pBase = integer_base::decimal )
		{
			wchar_t * endPtr = nullptr;
			int64_t value = std::wcstoll( pInputStr, &endPtr, static_cast<int>( pBase ) );
			return { value, numeric_cast<uint32>( endPtr - pInputStr ), ( endPtr != pInputStr ) ? endPtr : nullptr };
		}

		template <typename TPChar>
		static from_string_conv_result<int64_t, TPChar> from_string( const std::basic_string_view<TPChar> & pInputStr, uintptr_t pOffset = 0u, integer_base pBase = integer_base::decimal )
		{
			return from_string( pInputStr.data() + get_min_of( pOffset, pInputStr.size() ), pBase );
		}

		template <typename TPChar>
		static from_string_conv_result<int64_t, TPChar> from_string( const std::basic_string<TPChar> & pInputStr, uintptr_t pOffset = 0u, integer_base pBase = integer_base::decimal )
		{
			return from_string( pInputStr.data() + get_min_of( pOffset, pInputStr.size() ), pBase );
		}
	};

	template <>
	struct from_string_proxy<uint64_t>
	{
		static from_string_conv_result<uint64_t, char> from_string( const char * pInputStr, integer_base pBase = integer_base::decimal )
		{
			char * endPtr = nullptr;
			uint64_t value = std::strtoull( pInputStr, &endPtr, static_cast<int>( pBase ) );
			return { value, numeric_cast<uint32>( endPtr - pInputStr ), ( endPtr != pInputStr ) ? endPtr : nullptr };
		}

		static from_string_conv_result<uint64_t, wchar_t> from_string( const wchar_t * pInputStr, integer_base pBase = integer_base::decimal )
		{
			wchar_t * endPtr = nullptr;
			uint64_t value = std::wcstoull( pInputStr, &endPtr, static_cast<int>( pBase ) );
			return { value, numeric_cast<uint32>( endPtr - pInputStr ), ( endPtr != pInputStr ) ? endPtr : nullptr };
		}

		template <typename TPChar>
		static from_string_conv_result<uint64_t, TPChar> from_string( const std::basic_string_view<TPChar> & pInputStr, uintptr_t pOffset = 0u, integer_base pBase = integer_base::decimal )
		{
			return from_string( pInputStr.data() + get_min_of( pOffset, pInputStr.size() ), pBase );
		}

		template <typename TPChar>
		static from_string_conv_result<uint64_t, TPChar> from_string( const std::basic_string<TPChar> & pInputStr, uintptr_t pOffset = 0u, integer_base pBase = integer_base::decimal )
		{
			return from_string( pInputStr.data() + get_min_of( pOffset, pInputStr.size() ), pBase );
		}
	};

	template <>
	struct from_string_proxy<float>
	{
		static from_string_conv_result<float, char> from_string( const char * pInputStr )
		{
			char * endPtr = nullptr;
			float value = std::strtof( pInputStr, &endPtr );
			return { value, numeric_cast<uint32>( endPtr - pInputStr ), ( endPtr != pInputStr ) ? endPtr : nullptr };
		}

		static from_string_conv_result<float, wchar_t> from_string( const wchar_t * pInputStr )
		{
			wchar_t * endPtr = nullptr;
			float value = std::wcstof( pInputStr, &endPtr );
			return { value, numeric_cast<uint32>( endPtr - pInputStr ), ( endPtr != pInputStr ) ? endPtr : nullptr };
		}

		template <typename TPChar>
		static from_string_conv_result<float, TPChar> from_string( const std::basic_string_view<TPChar> & pInputStr, uintptr_t pOffset = 0u )
		{
			return from_string( pInputStr.data() + get_min_of( pOffset, pInputStr.size() ) );
		}

		template <typename TPChar>
		static from_string_conv_result<float, TPChar> from_string( const std::basic_string<TPChar> & pInputStr, uintptr_t pOffset = 0u )
		{
			return from_string( pInputStr.data() + get_min_of( pOffset, pInputStr.size() ) );
		}
	};

	template <>
	struct from_string_proxy<double>
	{
		static from_string_conv_result<double, char> from_string( const char * pInputStr )
		{
			char * endPtr = nullptr;
			double value = std::strtod( pInputStr, &endPtr );
			return { value, numeric_cast<uint32>( endPtr - pInputStr ), ( endPtr != pInputStr ) ? endPtr : nullptr };
		}

		static from_string_conv_result<double, wchar_t> from_string( const wchar_t * pInputStr )
		{
			wchar_t * endPtr = nullptr;
			double value = std::wcstod( pInputStr, &endPtr );
			return { value, numeric_cast<uint32>( endPtr - pInputStr ), ( endPtr != pInputStr ) ? endPtr : nullptr };
		}

		template <typename TPChar>
		static from_string_conv_result<double, TPChar> from_string( const std::basic_string_view<TPChar> & pInputStr, uintptr_t pOffset = 0u )
		{
			return from_string( pInputStr.data() + get_min_of( pOffset, pInputStr.size() ) );
		}

		template <typename TPChar>
		static from_string_conv_result<double, TPChar> from_string( const std::basic_string<TPChar> & pInputStr, uintptr_t pOffset = 0u )
		{
			return from_string( pInputStr.data() + get_min_of( pOffset, pInputStr.size() ) );
		}
	};

	template <>
	struct from_string_proxy<long double>
	{
		static from_string_conv_result<long double, char> from_string( const char * pInputStr )
		{
			char * endPtr = nullptr;
			long double value = std::strtold( pInputStr, &endPtr );
			return { value, numeric_cast<uint32>( endPtr - pInputStr ), ( endPtr != pInputStr ) ? endPtr : nullptr };
		}

		static from_string_conv_result<long double, wchar_t> from_string( const wchar_t * pInputStr )
		{
			wchar_t * endPtr = nullptr;
			long double value = std::wcstold( pInputStr, &endPtr );
			return { value, numeric_cast<uint32>( endPtr - pInputStr ), ( endPtr != pInputStr ) ? endPtr : nullptr };
		}

		template <typename TPChar>
		static from_string_conv_result<long double, TPChar> from_string( const std::basic_string_view<TPChar> & pInputStr, uintptr_t pOffset = 0u )
		{
			return from_string( pInputStr.data() + get_min_of( pOffset, pInputStr.size() ) );
		}

		template <typename TPChar>
		static from_string_conv_result<long double, TPChar> from_string( const std::basic_string<TPChar> & pInputStr, uintptr_t pOffset = 0u )
		{
			return from_string( pInputStr.data() + get_min_of( pOffset, pInputStr.size() ) );
		}
	};

	template <>
	struct from_string_proxy<int16_t>
	{
		static from_string_conv_result<int16_t, char> from_string( const char * pInputStr, integer_base pBase = integer_base::decimal )
		{
			return from_string_proxy<int32_t>::from_string( pInputStr, pBase ).get_as<int16_t>();
		}

		static from_string_conv_result<int16_t, wchar_t> from_string( const wchar_t * pInputStr, integer_base pBase = integer_base::decimal )
		{
			return from_string_proxy<int32_t>::from_string( pInputStr, pBase ).get_as<int16_t>();
		}

		template <typename TPChar>
		static from_string_conv_result<int16_t, TPChar> from_string( const std::basic_string_view<TPChar> & pInputStr, uintptr_t pOffset = 0u, integer_base pBase = integer_base::decimal )
		{
			return from_string_proxy<int32_t>::from_string( pInputStr, pOffset, pBase ).template get_as<int16_t>();
		}

		template <typename TPChar>
		static from_string_conv_result<int16_t, TPChar> from_string( const std::basic_string<TPChar> & pInputStr, uintptr_t pOffset = 0u, integer_base pBase = integer_base::decimal )
		{
			return from_string_proxy<int32_t>::from_string( pInputStr, pOffset, pBase ).template get_as<int16_t>();
		}
	};

	template <>
	struct from_string_proxy<uint16_t>
	{
		static from_string_conv_result<uint16_t, char> from_string( const char * pInputStr, integer_base pBase = integer_base::decimal )
		{
			return from_string_proxy<uint32_t>::from_string( pInputStr, pBase ).get_as<uint16_t>();
		}

		static from_string_conv_result<uint16_t, wchar_t> from_string( const wchar_t * pInputStr, integer_base pBase = integer_base::decimal )
		{
			return from_string_proxy<uint32_t>::from_string( pInputStr, pBase ).get_as<uint16_t>();
		}

		template <typename TPChar>
		static from_string_conv_result<uint16_t, TPChar> from_string( const std::basic_string_view<TPChar> & pInputStr, uintptr_t pOffset = 0u, integer_base pBase = integer_base::decimal )
		{
			return from_string_proxy<uint32_t>::from_string( pInputStr, pOffset, pBase ).template get_as<uint16_t>();
		}

		template <typename TPChar>
		static from_string_conv_result<uint16_t, TPChar> from_string( const std::basic_string<TPChar> & pInputStr, uintptr_t pOffset = 0u, integer_base pBase = integer_base::decimal )
		{
			return from_string_proxy<uint32_t>::from_string( pInputStr, pOffset, pBase ).template get_as<uint16_t>();
		}
	};

	template <>
	struct from_string_proxy<int8_t>
	{
		static from_string_conv_result<int8_t, char> from_string( const char * pInputStr, integer_base pBase = integer_base::decimal )
		{
			return from_string_proxy<int32_t>::from_string( pInputStr, pBase ).get_as<int8_t>();
		}

		static from_string_conv_result<int8_t, wchar_t> from_string( const wchar_t * pInputStr, integer_base pBase = integer_base::decimal )
		{
			return from_string_proxy<int32_t>::from_string( pInputStr, pBase ).get_as<int8_t>();
		}

		template <typename TPChar>
		static from_string_conv_result<int8_t, TPChar> from_string( const std::basic_string_view<TPChar> & pInputStr, uintptr_t pOffset = 0u, integer_base pBase = integer_base::decimal )
		{
			return from_string_proxy<int32_t>::from_string( pInputStr, pOffset, pBase ).template get_as<int8_t>();
		}

		template <typename TPChar>
		static from_string_conv_result<int8_t, TPChar> from_string( const std::basic_string<TPChar> & pInputStr, uintptr_t pOffset = 0u, integer_base pBase = integer_base::decimal )
		{
			return from_string_proxy<int32_t>::from_string( pInputStr, pOffset, pBase ).template get_as<int8_t>();
		}
	};

	template <>
	struct from_string_proxy<uint8_t>
	{
		static from_string_conv_result<uint8_t, char> from_string( const char * pInputStr, integer_base pBase = integer_base::decimal )
		{
			return from_string_proxy<uint32_t>::from_string( pInputStr, pBase ).get_as<uint8_t>();
		}

		static from_string_conv_result<uint8_t, wchar_t> from_string( const wchar_t * pInputStr, integer_base pBase = integer_base::decimal )
		{
			return from_string_proxy<uint32_t>::from_string( pInputStr, pBase ).get_as<uint8_t>();
		}

		template <typename TPChar>
		static from_string_conv_result<uint8_t, TPChar> from_string( const std::basic_string_view<TPChar> & pInputStr, uintptr_t pOffset = 0u, integer_base pBase = integer_base::decimal )
		{
			return from_string_proxy<uint32_t>::from_string( pInputStr, pOffset, pBase ).template get_as<uint8_t>();
		}

		template <typename TPChar>
		static from_string_conv_result<uint8_t, TPChar> from_string( const std::basic_string<TPChar> & pInputStr, uintptr_t pOffset = 0u, integer_base pBase = integer_base::decimal )
		{
			return from_string_proxy<uint32_t>::from_string( pInputStr, pOffset, pBase ).template get_as<uint8_t>();
		}
	};


	/// @brief
	template <typename TPChar, typename TPValue>
	std::basic_string<TPChar> to_string( const TPValue & pValue )
	{
		return to_string_proxy<TPChar>::to_string( pValue );
	}

	/// @brief
	template <typename TPValue, typename TPChar>
	TPValue from_string( const TPChar * pInputStr )
	{
		const auto convResult = from_string_proxy<TPValue>::from_string( pInputStr );
		return static_cast<TPValue>( convResult );
	}

	/// @brief
	template <typename TPValue, typename TPChar>
	TPValue from_string( const TPChar * pInputStr, integer_base pBase )
	{
		const auto convResult = from_string_proxy<TPValue>::from_string( pInputStr, pBase );
		return static_cast<TPValue>( convResult );
	}

	/// @brief
	template <typename TPValue, typename TPChar>
	TPValue from_string( const std::basic_string<TPChar> & pInputStr, uintptr_t pOffset = 0u )
	{
		const auto convResult = from_string_proxy<TPValue>::from_string( pInputStr, pOffset );
		return static_cast<TPValue>( convResult );
	}

	/// @brief
	template <typename TPValue, typename TPChar>
	TPValue from_string( const std::basic_string<TPChar> & pInputStr, integer_base pBase, uintptr_t pOffset = 0u )
	{
		const auto convResult = from_string_proxy<TPValue>::from_string( pInputStr, pOffset, pBase );
		return static_cast<TPValue>( convResult );
	}

	/// @brief
	template <typename TPValue, typename TPChar>
	TPValue from_string_or_default( const TPChar * pInputStr, const TPValue pDefault = static_cast<TPValue>( 0 ) )
	{
		const auto convResult = from_string_proxy<TPValue>::from_string( pInputStr );
		return convResult ? static_cast<TPValue>( convResult ) : pDefault;
	}

	/// @brief
	template <typename TPValue, typename TPChar>
	TPValue from_string_or_default( const TPChar * pInputStr, integer_base pBase, const TPValue pDefault = static_cast<TPValue>( 0 ) )
	{
		const auto convResult = from_string_proxy<TPValue>::from_string( pInputStr, pBase );
		return convResult ? static_cast<TPValue>( convResult ) : pDefault;
	}

	/// @brief
	template <typename TPValue, typename TPChar>
	TPValue from_string_or_default( const std::basic_string<TPChar> & pInputStr, const TPValue pDefault = static_cast<TPValue>( 0 ), uintptr_t pOffset = 0u )
	{
		const auto convResult = from_string_proxy<TPValue>::from_string( pInputStr, pOffset );
		return convResult ? static_cast<TPValue>( convResult ) : pDefault;
	}

	/// @brief
	template <typename TPValue, typename TPChar>
	TPValue from_string_or_default( const std::basic_string<TPChar> & pInputStr, integer_base pBase, const TPValue pDefault = static_cast<TPValue>( 0 ), uintptr_t pOffset = 0u )
	{
		const auto convResult = from_string_proxy<TPValue>::from_string( pInputStr, pBase, pOffset );
		return convResult ? static_cast<TPValue>( convResult ) : pDefault;
	}

	/// @brief
	template <typename TPValue, typename TPChar>
	from_string_conv_result<TPValue, TPChar> from_string_ex( const TPChar * pInputStr )
	{
		return from_string_proxy<TPValue>::from_string( pInputStr );
	}

	/// @brief
	template <typename TPValue, typename TPChar>
	from_string_conv_result<TPValue, TPChar> from_string_ex( const TPChar * pInputStr, integer_base pBase )
	{
		return from_string_proxy<TPValue>::from_string( pInputStr, pBase );
	}

	/// @brief
	template <typename TPValue, typename TPChar>
	from_string_conv_result<TPValue, TPChar> from_string_ex( const std::basic_string<TPChar> & pInputStr, uintptr_t pOffset = 0u )
	{
		return from_string_proxy<TPValue>::from_string( pInputStr, pOffset );
	}

	/// @brief
	template <typename TPValue, typename TPChar>
	from_string_conv_result<TPValue, TPChar> from_string_ex( const std::basic_string<TPChar> & pInputStr, integer_base pBase, uintptr_t pOffset = 0u )
	{
		return from_string_proxy<TPValue>::from_string( pInputStr, pBase, pOffset );
	}

}

#endif /* __CPPX_STRING_EXT_H__ */
