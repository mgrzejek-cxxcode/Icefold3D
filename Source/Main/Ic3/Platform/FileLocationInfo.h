
#pragma once

#ifndef __IC3_PLATFORM_FILE_LOCATION_INFO_H__
#define __IC3_PLATFORM_FILE_LOCATION_INFO_H__

#include "Platform.h"
#include <cstring>
#include <string>

namespace Ic3
{

	namespace CxDef
	{

		/// @brief Represents empty char* literal.
		inline constexpr const char * STR_CHAR_EMPTY = "";

		/// @brief Represents empty wchar_t* literal.
		inline constexpr const wchar_t * STR_WCHAR_EMPTY = L"";

	}

	struct FileLocationInfo
	{
	public:
		const char * function;
		const char * filename;
		int32 lineNumber;

	public:
		FileLocationInfo( const FileLocationInfo & ) = default;
		FileLocationInfo & operator=( const FileLocationInfo & ) = default;

		FileLocationInfo()
		: function( CxDef::STR_CHAR_EMPTY )
		, filename( CxDef::STR_CHAR_EMPTY )
		, lineNumber( 0 )
		{}

		FileLocationInfo( const char * pFunction, const char * pFilename, int32 pLineNumber )
		: function( pFunction )
		, filename( pFilename )
		, lineNumber( pLineNumber )
		{}

		IC3_ATTR_NO_DISCARD std::string toString() const;
	};

	inline std::string toString( const FileLocationInfo & pLocationInfo )
	{
		return pLocationInfo.toString();
	}

	/// @brief Macro for shorter replacement of location info for a current source line.
	#define ic3CurrentFileLocationInfo() ::Ic3::FileLocationInfo{ __FUNCTION__, __FILE__, __LINE__ }

}

#endif // __IC3_PLATFORM_FILE_LOCATION_INFO_H__
