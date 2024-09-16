
#ifndef __IC3_CPPX_STRING_UTILS_H__
#define __IC3_CPPX_STRING_UTILS_H__

#include "StringExt.h"
#include <cctype>
#include <cwctype>
#include <functional>

namespace Ic3::Cppx
{

	template <const std::string_view & ...tpStrArray>
	struct StrStaticJoin
	{
		static constexpr auto joinImpl() noexcept
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

		static constexpr auto _joinedStrData = joinImpl();

		static constexpr auto get() noexcept
		{
			return std::string_view{ _joinedStrData.data(), _joinedStrData.size() };
		}
	};

	/// @brief
	template <typename TPChar>
	struct CharConv;

	template <>
	struct CharConv<char>
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
	struct CharConv<wchar_t>
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
	struct StringConv;

	template <>
	struct StringConv<char>
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
	struct StringConv<wchar_t>
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

	namespace strUtils
	{

		template <typename TPCharOut, typename TPCharIn>
		inline std::basic_string<TPCharOut> convertStringRepresentation( const TPCharIn * pInput, size_t pInputLength )
		{
			return StringConv<TPCharOut>::convert( pInput, pInputLength );
		}

		template <typename TPCharOut, typename TPCharIn>
		inline std::basic_string<TPCharOut> convertStringRepresentation( const TPCharIn * pInput )
		{
			return StringConv<TPCharOut>::convert( pInput );
		}

		template <typename TPCharOut, typename TPCharIn>
		inline std::basic_string<TPCharOut> convertStringRepresentation( const std::basic_string<TPCharIn> & pInput )
		{
			return StringConv<TPCharOut>::convert( pInput );
		}

		template <typename TPChar>
		inline void makeLowerTransform( std::basic_string<TPChar> & pInput )
		{
			std::transform( pInput.begin(), pInput.end(), pInput.begin(), CharConv<TPChar>::toLower );
		}

		template <typename TPChar>
		inline void makeUpperTransform( std::basic_string<TPChar> & pInput )
		{
			std::transform( pInput.begin(), pInput.end(), pInput.begin(), CharConv<TPChar>::toUpper );
		}

		template <typename TPChar>
		inline std::basic_string<TPChar> makeLower( const std::basic_string<TPChar> & pInput )
		{
			std::basic_string<TPChar> result = pInput;
			makeLowerTransform( result );
			return result;
		}

		template <typename TPChar>
		inline std::basic_string<TPChar> makeUpper( const std::basic_string<TPChar> & pInput )
		{
			std::basic_string<TPChar> result = pInput;
			makeUpperTransform( result );
			return result;
		}

		template <typename TPChar, class TPAppendPred>
		inline size_t splitString( const TPChar * pInputStr, size_t pInputStrLength, TPChar pSeparator, TPAppendPred pAppendPredicate )
		{
			size_t result = 0;

			for( const TPChar * strEnd = pInputStr + pInputStrLength; pInputStr < strEnd; )
			{
				const TPChar * tokenPtr = std::char_traits<TPChar>::find( pInputStr, pInputStrLength, pSeparator );
				if( tokenPtr == nullptr )
				{
					pAppendPredicate( pInputStr, strEnd - pInputStr );
					break;
				}

				size_t substrLen = tokenPtr - pInputStr;

				pAppendPredicate( pInputStr, substrLen );
				++result;

				pInputStr = tokenPtr + 1;
				pInputStrLength -= ( substrLen + 1 );
			}

			return result;
		}

		template <typename TPChar, class TPAppendPred>
		inline size_t splitString( const TPChar * pInputStr, TPChar pSeparator, TPAppendPred pAppendPredicate )
		{
			size_t inputStrLength = std::char_traits<TPChar>::length( pInputStr );
			return splitString( pInputStr, inputStrLength, pSeparator, pAppendPredicate );
		}

		template <typename TPChar, class TPAppendPred>
		inline size_t splitString( const std::basic_string<TPChar> & pInputStr, TPChar pSeparator, TPAppendPred pAppendPredicate )
		{
			return splitString( pInputStr.data(), pInputStr.length(), pSeparator, pAppendPredicate );
		}

		template <typename TPResult, typename TPChar, class TPAppendPred>
		inline TPResult splitStringEx( const TPChar * pInputStr, size_t pInputStrLength, TPChar pSeparator, TPAppendPred pAppendPredicate )
		{
			TPResult result{};
			auto appendPredCb = std::bind( pAppendPredicate, std::ref( result ), std::placeholders::_1, std::placeholders::_2 );
			splitString( pInputStr, pInputStrLength, pSeparator, appendPredCb );
			return result;
		}

		template <typename TPResult, typename TPChar, class TPAppendPred>
		inline TPResult splitStringEx( const TPChar * pInputStr, TPChar pSeparator, TPAppendPred pAppendPredicate )
		{
			size_t inputStrLength = std::char_traits<TPChar>::length( pInputStr );
			return splitStringEx<TPResult>( pInputStr, inputStrLength, pSeparator, pAppendPredicate );
		}

		template <typename TPResult, typename TPChar, class TPAppendPred>
		inline TPResult splitStringEx( const std::basic_string<TPChar> & pInputStr, TPChar pSeparator, TPAppendPred pAppendPredicate )
		{
			return splitStringEx<TPResult>( pInputStr.data(), pInputStr.length(), pSeparator, pAppendPredicate );
		}

		template <typename TPChar>
		inline std::basic_string<TPChar> extractShortFilePath( const std::basic_string<TPChar> & pFilename, TPChar pPathSeparator )
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
		inline std::basic_string<TPChar> extractShortFilePath( const TPChar * pFilename, TPChar pPathSeparator )
		{
			return extractShortFilePath( std::basic_string<TPChar>{pFilename}, pPathSeparator );
		}

	}

}

#endif /* __IC3_CPPX_STRING_UTILS_H__ */
