
#ifndef __IC3_CPPX_STRING_UTILS_H__
#define __IC3_CPPX_STRING_UTILS_H__

#include "StringExt.h"
#include <cctype>
#include <cwctype>
#include <functional>

namespace Ic3::Cppx
{

	/// @brief
	template <typename TChar>
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
	template <typename TChar>
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

		template <typename _CharOut, typename _CharIn>
		inline std::basic_string<_CharOut> convertStringRepresentation( const _CharIn * pInput, size_t pInputLength )
		{
			return StringConv<_CharOut>::convert( pInput, pInputLength );
		}

		template <typename _CharOut, typename _CharIn>
		inline std::basic_string<_CharOut> convertStringRepresentation( const _CharIn * pInput )
		{
			return StringConv<_CharOut>::convert( pInput );
		}

		template <typename _CharOut, typename _CharIn>
		inline std::basic_string<_CharOut> convertStringRepresentation( const std::basic_string<_CharIn> & pInput )
		{
			return StringConv<_CharOut>::convert( pInput );
		}

		template <typename TChar>
		inline void makeLowerTransform( std::basic_string<TChar> & pInput )
		{
			std::transform( pInput.begin(), pInput.end(), pInput.begin(), CharConv<TChar>::toLower );
		}

		template <typename TChar>
		inline void makeUpperTransform( std::basic_string<TChar> & pInput )
		{
			std::transform( pInput.begin(), pInput.end(), pInput.begin(), CharConv<TChar>::toUpper );
		}

		template <typename TChar>
		inline std::basic_string<TChar> makeLower( const std::basic_string<TChar> & pInput )
		{
			std::basic_string<TChar> result = pInput;
			makeLowerTransform( result );
			return result;
		}

		template <typename TChar>
		inline std::basic_string<TChar> makeUpper( const std::basic_string<TChar> & pInput )
		{
			std::basic_string<TChar> result = pInput;
			makeUpperTransform( result );
			return result;
		}

		template <typename TChar, class _Apred>
		inline size_t splitString( const TChar * pInputStr, size_t pInputStrLength, TChar pSeparator, _Apred pAppendPredicate )
		{
			size_t result = 0;

			for( const TChar * strEnd = pInputStr + pInputStrLength; pInputStr < strEnd; )
			{
				const TChar * tokenPtr = std::char_traits<TChar>::find( pInputStr, pInputStrLength, pSeparator );
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

		template <typename TChar, class _Apred>
		inline size_t splitString( const TChar * pInputStr, TChar pSeparator, _Apred pAppendPredicate )
		{
			size_t inputStrLength = std::char_traits<TChar>::length( pInputStr );
			return splitString( pInputStr, inputStrLength, pSeparator, pAppendPredicate );
		}

		template <typename TChar, class _Apred>
		inline size_t splitString( const std::basic_string<TChar> & pInputStr, TChar pSeparator, _Apred pAppendPredicate )
		{
			return splitString( pInputStr.data(), pInputStr.length(), pSeparator, pAppendPredicate );
		}

		template <typename _Res, typename TChar, class _Apred>
		inline _Res splitStringEx( const TChar * pInputStr, size_t pInputStrLength, TChar pSeparator, _Apred pAppendPredicate )
		{
			_Res result{};
			auto appendPredCb = std::bind( pAppendPredicate, std::ref( result ), std::placeholders::_1, std::placeholders::_2 );
			splitString( pInputStr, pInputStrLength, pSeparator, appendPredCb );
			return result;
		}

		template <typename _Res, typename TChar, class _Apred>
		inline _Res splitStringEx( const TChar * pInputStr, TChar pSeparator, _Apred pAppendPredicate )
		{
			size_t inputStrLength = std::char_traits<TChar>::length( pInputStr );
			return splitStringEx<_Res>( pInputStr, inputStrLength, pSeparator, pAppendPredicate );
		}

		template <typename _Res, typename TChar, class _Apred>
		inline _Res splitStringEx( const std::basic_string<TChar> & pInputStr, TChar pSeparator, _Apred pAppendPredicate )
		{
			return splitStringEx<_Res>( pInputStr.data(), pInputStr.length(), pSeparator, pAppendPredicate );
		}

		template <typename TChar>
		inline std::basic_string<TChar> extractShortFilePath( const std::basic_string<TChar> & pFilename, TChar pPathSeparator )
		{
			std::basic_string<TChar> shortFilename = pFilename;
			size_t filenameSeparator = shortFilename.find_last_of( pPathSeparator, 0 );

			if( filenameSeparator != std::basic_string<TChar>::npos )
			{
				size_t lastDirSep = shortFilename.find_last_of( pPathSeparator, filenameSeparator );
				if( lastDirSep != std::basic_string<TChar>::npos )
				{
					shortFilename.erase( 0, lastDirSep + 1 );
				}
			}

			return shortFilename;
		}

		template <typename TChar>
		inline std::basic_string<TChar> extractShortFilePath( const TChar * pFilename, TChar pPathSeparator )
		{
			return extractShortFilePath( std::basic_string<TChar>{pFilename}, pPathSeparator );
		}

	}

}

#endif /* __IC3_CPPX_STRING_UTILS_H__ */
