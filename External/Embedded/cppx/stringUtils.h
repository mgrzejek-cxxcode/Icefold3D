
#ifndef __CPPX_STRING_UTILS_H__
#define __CPPX_STRING_UTILS_H__

#include "stringExt.h"
#include <cctype>
#include <cwctype>
#include <functional>

namespace cppx
{

	template <const std::string_view & ...tpStrArray>
	struct str_static_join
	{
		static constexpr auto join_impl() noexcept
		{
			constexpr size_t joinedLength = ( tpStrArray.size() + ... + 0 );
			std::array<char, joinedLength + 1> joinedData{};

			auto staticAppend = [joinedPos = 0, &joinedData]( const auto & pStrView ) mutable {
				for( auto ch : pStrView ) joinedData[joinedPos++] = ch;
			};

			( staticAppend( tpStrArray ), ... );

			joinedData[joinedLength] = 0;

			return joinedData;
		}

		static constexpr auto _joinedStrData = join_impl();

		static constexpr auto get() noexcept
		{
			return std::string_view{ _joinedStrData.data(), _joinedStrData.size() };
		}
	};

	/// @brief
	template <typename TPChar>
	struct char_conv;

	template <>
	struct char_conv<char>
	{
		static inline char toLower( char pChar )
		{
			return static_cast<char>( std::tolower( pChar ) );
		}

		static inline char toUpper( char pChar )
		{
			return static_cast<char>( std::toupper( pChar ) );
		}
	};

	template <>
	struct char_conv<wchar_t>
	{
		static inline wchar_t toLower( wchar_t pChar )
		{
			return static_cast<char>( std::towlower( pChar ) );
		}

		static inline wchar_t toUpper( wchar_t pChar )
		{
			return static_cast<char>( std::towupper( pChar ) );
		}
	};


	/// @brief
	template <typename TPChar>
	struct string_conv;

	template <>
	struct string_conv<char>
	{
		static inline std::basic_string<char> convert( const wchar_t * pInput, size_t pInputLength )
		{
			const auto convBufferLength = pInputLength * 4;

			std::basic_string<char> convBuffer;
			convBuffer.reserve( convBufferLength );

			const auto outputLength = std::wcstombs( convBuffer.data(), pInput, convBufferLength );

			return std::basic_string<char>( convBuffer.data(), outputLength );
		}

		static inline std::basic_string<char> convert( const wchar_t * pInput )
		{
			const auto inputLength = wcslen( pInput );
			return convert( pInput, inputLength );
		}

		static inline std::basic_string<char> convert( const std::basic_string<wchar_t> & pInput )
		{
			return convert( pInput.data(), pInput.length() );
		}

		static inline std::basic_string<char> convert( std::basic_string<char> pInput )
		{
			return std::basic_string<char>( std::move( pInput ) );
		}
	};

	template <>
	struct string_conv<wchar_t>
	{
		static inline std::basic_string<wchar_t> convert( const char * pInput, size_t pInputLength )
		{
			const auto convBufferLength = pInputLength;

			std::basic_string<wchar_t> convBuffer;
			convBuffer.reserve( convBufferLength );

			const auto outputLength = std::mbstowcs( convBuffer.data(), pInput, convBufferLength );

			return std::basic_string<wchar_t>( convBuffer.data(), outputLength );
		}

		static inline std::basic_string<wchar_t> convert( const char * pInput )
		{
			const auto inputLength = strlen( pInput );
			return convert( pInput, inputLength );
		}

		static inline std::basic_string<wchar_t> convert( const std::basic_string<char> & pInput )
		{
			return convert( pInput.data(), pInput.length() );
		}

		static inline std::basic_string<wchar_t> convert( std::basic_string<wchar_t> pInput )
		{
			return std::basic_string<wchar_t>( std::move( pInput ) );
		}
	};

	namespace strutil
	{
		
		enum EStrSplitFlags
		{
			eStrSplitFlagIncludeEmpty = 0x01
		};

		template <typename TPCharOut, typename TPCharIn>
		inline std::basic_string<TPCharOut> convert_string_representation( const TPCharIn * pInput, size_t pInputLength )
		{
			return string_conv<TPCharOut>::convert( pInput, pInputLength );
		}

		template <typename TPCharOut, typename TPCharIn>
		inline std::basic_string<TPCharOut> convert_string_representation( const TPCharIn * pInput )
		{
			return string_conv<TPCharOut>::convert( pInput );
		}

		template <typename TPCharOut, typename TPCharIn>
		inline std::basic_string<TPCharOut> convert_string_representation( const std::basic_string<TPCharIn> & pInput )
		{
			return string_conv<TPCharOut>::convert( pInput );
		}

		template <typename TPChar>
		inline void make_lower_transform( std::basic_string<TPChar> & pInput )
		{
			std::transform( pInput.begin(), pInput.end(), pInput.begin(), char_conv<TPChar>::toLower );
		}

		template <typename TPChar>
		inline void make_upper_transform( std::basic_string<TPChar> & pInput )
		{
			std::transform( pInput.begin(), pInput.end(), pInput.begin(), char_conv<TPChar>::toUpper );
		}

		template <typename TPChar>
		inline std::basic_string<TPChar> make_lower( const std::basic_string<TPChar> & pInput )
		{
			std::basic_string<TPChar> result = pInput;
			make_lower_transform( result );
			return result;
		}

		template <typename TPChar>
		inline std::basic_string<TPChar> make_upper( const std::basic_string<TPChar> & pInput )
		{
			std::basic_string<TPChar> result = pInput;
			make_upper_transform( result );
			return result;
		}

		template <typename TPChar, class TPAppendPred>
		inline size_t split_string( const TPChar * pInputStr, size_t pInputStrLength, TPChar pSeparator, TPAppendPred pAppendPredicate, bitmask<EStrSplitFlags> pSplitFlags = 0 )
		{
			size_t result = 0;

			for( const TPChar * strEnd = pInputStr + pInputStrLength; pInputStr < strEnd; )
			{
				const TPChar * tokenPtr = std::char_traits<TPChar>::find( pInputStr, pInputStrLength, pSeparator );

				if( tokenPtr == nullptr )
				{
					const auto substrLength = strEnd - pInputStr;
					if( ( substrLength > 0 ) || pSplitFlags.is_set( eStrSplitFlagIncludeEmpty ) )
					{
						pAppendPredicate( pInputStr, strEnd - pInputStr );
						++result;
					}
					break;
				}

				const auto substrLength = tokenPtr - pInputStr;
				if( ( substrLength > 0 ) || pSplitFlags.is_set( eStrSplitFlagIncludeEmpty ) )
				{
					pAppendPredicate( pInputStr, substrLength );
					++result;
				}

				pInputStr = tokenPtr + 1;
				pInputStrLength -= ( substrLength + 1 );
			}

			return result;
		}

		template <typename TPChar, class TPAppendPred>
		inline size_t split_string( const TPChar * pInputStr, TPChar pSeparator, TPAppendPred pAppendPredicate, bitmask<EStrSplitFlags> pSplitFlags = 0 )
		{
			size_t inputStrLength = std::char_traits<TPChar>::length( pInputStr );
			return split_string( pInputStr, inputStrLength, pSeparator, pAppendPredicate, pSplitFlags );
		}

		template <typename TPChar, class TPAppendPred>
		inline size_t split_string( const std::basic_string<TPChar> & pInputStr, TPChar pSeparator, TPAppendPred pAppendPredicate, bitmask<EStrSplitFlags> pSplitFlags = 0 )
		{
			return split_string( pInputStr.data(), pInputStr.length(), pSeparator, pAppendPredicate, pSplitFlags );
		}

		template <typename TPChar, class TPAppendPred>
		inline size_t split_string( const std::basic_string_view<TPChar> & pInputStr, TPChar pSeparator, TPAppendPred pAppendPredicate, bitmask<EStrSplitFlags> pSplitFlags = 0 )
		{
			return split_string( pInputStr.data(), pInputStr.length(), pSeparator, pAppendPredicate, pSplitFlags );
		}

		template <typename TPResult, typename TPChar, class TPAppendPred>
		inline TPResult split_string_ex( const TPChar * pInputStr, size_t pInputStrLength, TPChar pSeparator, TPAppendPred pAppendPredicate, bitmask<EStrSplitFlags> pSplitFlags = 0 )
		{
			TPResult result{};
			auto appendPredCb = std::bind( pAppendPredicate, std::ref( result ), std::placeholders::_1, std::placeholders::_2 );
			split_string( pInputStr, pInputStrLength, pSeparator, appendPredCb, pSplitFlags );
			return result;
		}

		template <typename TPResult, typename TPChar, class TPAppendPred>
		inline TPResult split_string_ex( const TPChar * pInputStr, TPChar pSeparator, TPAppendPred pAppendPredicate, bitmask<EStrSplitFlags> pSplitFlags = 0 )
		{
			size_t inputStrLength = std::char_traits<TPChar>::length( pInputStr );
			return split_string_ex<TPResult>( pInputStr, inputStrLength, pSeparator, pAppendPredicate, pSplitFlags );
		}

		template <typename TPResult, typename TPChar, class TPAppendPred>
		inline TPResult split_string_ex( const std::basic_string<TPChar> & pInputStr, TPChar pSeparator, TPAppendPred pAppendPredicate, bitmask<EStrSplitFlags> pSplitFlags = 0 )
		{
			return split_string_ex<TPResult>( pInputStr.data(), pInputStr.length(), pSeparator, pAppendPredicate, pSplitFlags );
		}

		template <typename TPResult, typename TPChar, class TPAppendPred>
		inline TPResult split_string_ex( const std::basic_string_view<TPChar> & pInputStr, TPChar pSeparator, TPAppendPred pAppendPredicate, bitmask<EStrSplitFlags> pSplitFlags = 0 )
		{
			return split_string_ex<TPResult>( pInputStr.data(), pInputStr.length(), pSeparator, pAppendPredicate, pSplitFlags );
		}

		template <typename TPChar>
		inline std::basic_string<TPChar> extract_short_file_path( const std::basic_string<TPChar> & pFilename, TPChar pPathSeparator )
		{
			std::basic_string<TPChar> shortFilename = pFilename;
			size_t filenameSeparator = shortFilename.find_last_of( pPathSeparator, 0 );

			if( filenameSeparator != std::basic_string<TPChar>::npos )
			{
				size_t lastDirSep = shortFilename.find_last_of( pPathSeparator, filenameSeparator );
				if( lastDirSep != std::basic_string<TPChar>::npos )
				{
					shortFilename.erase( 0, lastDirSep + 1 );
				}
			}

			return shortFilename;
		}

		template <typename TPChar>
		inline std::basic_string<TPChar> extract_short_file_path( const TPChar * pFilename, TPChar pPathSeparator )
		{
			return extract_short_file_path( std::basic_string<TPChar>{pFilename}, pPathSeparator );
		}

	}

}

#endif /* __CPPX_STRING_UTILS_H__ */
