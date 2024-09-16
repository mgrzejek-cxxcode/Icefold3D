
#ifndef __IC3_CPPX_STRING_EXT_H__
#define __IC3_CPPX_STRING_EXT_H__

#include "ArrayView.h"
#include <string>

namespace Ic3::Cppx
{

	enum class EIntegerBase : uint32
	{
		Auto = 0,
		Decimal = 10,
		Octal = 8,
		Hexadecimal = 16
	};

	/// @brief Creates ArrayView that wraps an std::basic_string object
	/// @tparam TPChar Type of the string's internal characters.
	/// @param pString A string object to be mapped to a view.
	/// @return ArrayView referencing specified string object.
	template <typename TPChar>
	inline TArrayView<TPChar> bindArrayView( std::basic_string<TPChar> & pString )
	{
		return TArrayView<TPChar>( &( pString[0] ), pString.length() );
	}

	/// @brief Creates ArrayView that wraps an std::basic_string object
	/// @tparam TPChar Type of the string's internal characters.
	/// @param pString A string object to be mapped to a view.
	/// @return ArrayView referencing specified string object.
	template <typename TPChar>
	inline TArrayView<const TPChar> bindArrayView( const std::basic_string<TPChar> & pString )
	{
		return TArrayView<const TPChar>( &( pString[0] ), pString.length() );
	}


	/// @brief
	template <typename TPChar>
	struct ToStringProxy;

	template <>
	struct ToStringProxy<char>
	{
		template <class TPValue>
		static std::basic_string<char> toString( const TPValue & pValue )
		{
			return std::to_string( pValue );
		}
	};

	template <>
	struct ToStringProxy<wchar_t>
	{
		template <class TPValue>
		static std::basic_string<wchar_t> toString( const TPValue & pValue )
		{
			return std::to_wstring( pValue );
		}
	};


	/// @brief
	template <typename TPRet>
	struct FromStringProxy;

	template <>
	struct FromStringProxy<int32_t>
	{
		static std::pair<int32_t, char *> fromString( const char * pInputStr, EIntegerBase pBase = EIntegerBase::Decimal )
		{
			char * endPtr = nullptr;
			int32_t value = std::strtol( pInputStr, &endPtr, static_cast< int >( pBase ) );
			return { value, endPtr };
		}

		static std::pair<int32_t, wchar_t *> fromString( const wchar_t * pInputStr, EIntegerBase pBase = EIntegerBase::Decimal )
		{
			wchar_t * endPtr = nullptr;
			int32_t value = std::wcstol( pInputStr, &endPtr, static_cast< int >( pBase ) );
			return { value, endPtr };
		}

		template <typename TPChar>
		static std::pair<int32_t, size_t> fromString( const std::basic_string<TPChar> & pInputStr, EIntegerBase pBase = EIntegerBase::Decimal )
		{
			size_t processedNum = 0;
			int32_t value = std::stol( pInputStr, &processedNum, static_cast< int >( pBase ) );
			return { value, processedNum };
		}
	};

	template <>
	struct FromStringProxy<uint32_t>
	{
		static std::pair<uint32_t, char *> fromString( const char * pInputStr, EIntegerBase pBase = EIntegerBase::Decimal )
		{
			char * endPtr = nullptr;
			uint32_t value = std::strtoul( pInputStr, &endPtr, static_cast< int >( pBase ) );
			return { value, endPtr };
		}

		static std::pair<uint32_t, wchar_t *> fromString( const wchar_t * pInputStr, EIntegerBase pBase = EIntegerBase::Decimal )
		{
			wchar_t * endPtr = nullptr;
			uint32_t value = std::wcstoul( pInputStr, &endPtr, static_cast< int >( pBase ) );
			return { value, endPtr };
		}

		template <typename TPChar>
		static std::pair<uint32_t, size_t> fromString( const std::basic_string<TPChar> & pInputStr, EIntegerBase pBase = EIntegerBase::Decimal )
		{
			size_t processedNum = 0;
			uint32_t value = std::stoul( pInputStr, &processedNum, static_cast< int >( pBase ) );
			return { value, processedNum };
		}
	};

	template <>
	struct FromStringProxy<int64_t>
	{
		static std::pair<int64_t, char *> fromString( const char * pInputStr, EIntegerBase pBase = EIntegerBase::Decimal )
		{
			char * endPtr = nullptr;
			int64_t value = std::strtoll( pInputStr, &endPtr, static_cast< int >( pBase ) );
			return { value, endPtr };
		}

		static std::pair<int64_t, wchar_t *> fromString( const wchar_t * pInputStr, EIntegerBase pBase = EIntegerBase::Decimal )
		{
			wchar_t * endPtr = nullptr;
			int64_t value = std::wcstoll( pInputStr, &endPtr, static_cast< int >( pBase ) );
			return { value, endPtr };
		}

		template <typename TPChar>
		static std::pair<int64_t, size_t> fromString( const std::basic_string<TPChar> & pInputStr, EIntegerBase pBase = EIntegerBase::Decimal )
		{
			size_t processedNum = 0;
			int64_t value = std::stoll( pInputStr, &processedNum, static_cast< int >( pBase ) );
			return { value, processedNum };
		}
	};

	template <>
	struct FromStringProxy<uint64_t>
	{
		static std::pair<uint64_t, char *> fromString( const char * pInputStr, EIntegerBase pBase = EIntegerBase::Decimal )
		{
			char * endPtr = nullptr;
			uint64_t value = std::strtoull( pInputStr, &endPtr, static_cast< int >( pBase ) );
			return { value, endPtr };
		}

		static std::pair<uint64_t, wchar_t *> fromString( const wchar_t * pInputStr, EIntegerBase pBase = EIntegerBase::Decimal )
		{
			wchar_t * endPtr = nullptr;
			uint64_t value = std::wcstoull( pInputStr, &endPtr, static_cast< int >( pBase ) );
			return { value, endPtr };
		}

		template <typename TPChar>
		static std::pair<uint64_t, size_t> fromString( const std::basic_string<TPChar> & pInputStr, EIntegerBase pBase = EIntegerBase::Decimal )
		{
			size_t processedNum = 0;
			uint64_t value = std::stoull( pInputStr, &processedNum, static_cast< int >( pBase ) );
			return { value, processedNum };
		}
	};

	template <>
	struct FromStringProxy<float>
	{
		static std::pair<float, char *> fromString( const char * pInputStr )
		{
			char * endPtr = nullptr;
			float value = std::strtof( pInputStr, &endPtr );
			return { value, endPtr };
		}

		static std::pair<float, wchar_t *> fromString( const wchar_t * pInputStr )
		{
			wchar_t * endPtr = nullptr;
			float value = std::wcstof( pInputStr, &endPtr );
			return { value, endPtr };
		}

		template <typename TPChar>
		static std::pair<float, size_t> fromString( const std::basic_string<TPChar> & pInputStr )
		{
			size_t processedNum = 0;
			float value = std::stof( pInputStr, &processedNum );
			return { value, processedNum };
		}
	};

	template <>
	struct FromStringProxy<double>
	{
		static std::pair<double, char *> fromString( const char * pInputStr )
		{
			char * endPtr = nullptr;
			double value = std::strtod( pInputStr, &endPtr );
			return { value, endPtr };
		}

		static std::pair<double, wchar_t *> fromString( const wchar_t * pInputStr )
		{
			wchar_t * endPtr = nullptr;
			double value = std::wcstod( pInputStr, &endPtr );
			return { value, endPtr };
		}

		template <typename TPChar>
		static std::pair<double, size_t> fromString( const std::basic_string<TPChar> & pInputStr )
		{
			size_t processedNum = 0;
			double value = std::stod( pInputStr, &processedNum );
			return { value, processedNum };
		}
	};

	template <>
	struct FromStringProxy<long double>
	{
		static std::pair<long double, char *> fromString( const char * pInputStr )
		{
			char * endPtr = nullptr;
			long double value = std::strtold( pInputStr, &endPtr );
			return { value, endPtr };
		}

		static std::pair<long double, wchar_t *> fromString( const wchar_t * pInputStr )
		{
			wchar_t * endPtr = nullptr;
			long double value = std::wcstold( pInputStr, &endPtr );
			return { value, endPtr };
		}

		template <typename TPChar>
		static std::pair<long double, size_t> fromString( const std::basic_string<TPChar> & pInputStr )
		{
			size_t processedNum = 0;
			long double value = std::stold( pInputStr, &processedNum );
			return { value, processedNum };
		}
	};


	/// @brief
	template <typename TPChar, typename TPValue>
	std::basic_string<TPChar> toString( const TPValue & pValue )
	{
		return ToStringProxy<TPChar>::toString( pValue );
	}

	/// @brief
	template <typename TPValue, typename TPChar>
	std::pair<TPValue, TPChar *> fromString( const TPChar * pInputStr )
	{
		return FromStringProxy<TPValue>::fromString( pInputStr );
	}

	/// @brief
	template <typename TPValue, typename TPChar>
	std::pair<TPValue, TPChar *> fromString( const TPChar * pInputStr, EIntegerBase pBase )
	{
		return FromStringProxy<TPValue>::fromString( pInputStr, pBase );
	}

	/// @brief
	template <typename TPValue, typename TPChar>
	std::pair<TPValue, size_t> fromString( const std::basic_string<TPChar> & pInputStr )
	{
		return FromStringProxy<TPValue>::fromString( pInputStr );
	}

	/// @brief
	template <typename TPValue, typename TPChar>
	std::pair<TPValue, size_t> fromString( const std::basic_string<TPChar> & pInputStr, EIntegerBase pBase )
	{
		return FromStringProxy<TPValue>::fromString( pInputStr, pBase );
	}

}

#endif /* __IC3_CPPX_STRING_EXT_H__ */
