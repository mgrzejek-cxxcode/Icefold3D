
#include "../prerequisites.h"
#include "../stringUtils.h"

namespace cppx
{

	std::string file_location_info::to_short_string() const
	{
		auto funcStrLength = std::strlen( function );
		auto fileStrLength = std::strlen( filename );

		std::string result{};
		result.reserve( funcStrLength + fileStrLength + 128 );

		const auto shortFile = strutil::extract_short_file_path( filename );
		const auto shortFunction = strutil::extract_short_function_name( function );

		result.append( shortFunction.c_str(), shortFunction.length() );
		result.append( "() in " );
		result.append( shortFile.c_str(), shortFile.length() );

		result.append( ":" );
		result.append( std::to_string( line_number ) );

		return result;
	}

	std::string file_location_info::to_string() const
	{
		auto funcStrLength = std::strlen( function );
		auto fileStrLength = std::strlen( filename );

		std::string result{};
		result.reserve( funcStrLength + fileStrLength + 128 );

		result.append( "Function " );
		result.append( function, funcStrLength );
		result.append( " in file " );
		result.append( filename, fileStrLength );

		result.append( ", line " );
		result.append( std::to_string( line_number ) );

		return result;

	}

}
