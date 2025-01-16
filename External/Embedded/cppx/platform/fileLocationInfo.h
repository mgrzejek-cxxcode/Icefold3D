
#pragma once

#ifndef __CPPX_PLATFORM_FILE_LOCATION_INFO_H__
#define __CPPX_PLATFORM_FILE_LOCATION_INFO_H__

#include <cstring>
#include <string>

namespace cppx
{

	namespace cve
	{

		/// @brief Represents empty char* literal.
		constexpr const char * str_char_empty = "";

		/// @brief Represents empty wchar_t* literal.
		constexpr const wchar_t * str_wchar_empty = L"";

	}
	
	/// @brief
	struct file_location_info
	{
	public:
		const char * function;

		const char * filename;

		int32 line_number;

	public:
		file_location_info( const file_location_info & ) = default;
		file_location_info & operator=( const file_location_info & ) = default;

		file_location_info()
		: function( cve::str_char_empty )
		, filename( cve::str_char_empty )
		, line_number( 0 )
		{}

		file_location_info( const char * pFunction, const char * pFilename, int32 pLineNumber )
		: function( pFunction )
		, filename( pFilename )
		, line_number( pLineNumber )
		{}

		CPPX_ATTR_NO_DISCARD std::string to_string() const;
	};

	inline std::string to_string( const file_location_info & pLocationInfo )
	{
		return pLocationInfo.to_string();
	}

	/// @brief Macro for shorter replacement of location info for a current source line.
	#define cppx_current_file_location_info() ::cppx::file_location_info{ __FUNCTION__, __FILE__, __LINE__ }

}

#endif // __CPPX_PLATFORM_FILE_LOCATION_INFO_H__
