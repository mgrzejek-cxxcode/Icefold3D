
#include "../prerequisites.h"

namespace cppx
{

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

	#if defined( PCL_PROJECT_ROOT_PATH )
		auto replacePos = result.find( PCL_PROJECT_ROOT_PATH );
		auto replaceLength = std::strlen( PCL_PROJECT_ROOT_PATH );
		if( ( replacePos != std::string::npos ) && ( replaceLength > 0 ) )
		{
			result.erase( replacePos, replaceLength );
		}
	#endif

		result.append( ", line " );
		result.append( std::to_string( line_number ) );

		return result;

	}

}
