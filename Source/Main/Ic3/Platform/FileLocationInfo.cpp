
#include "FileLocationInfo.h"

namespace Ic3
{

	std::string FileLocationInfo::toString() const
	{
		auto funcStrLength = std::strlen( mFunction );
		auto fileStrLength = std::strlen( mFilename );

		std::string result{};
		result.reserve( funcStrLength + fileStrLength + 128 );

		result.append( "Function " );
		result.append( mFunction, funcStrLength );
		result.append( " in file " );
		result.append( mFilename, fileStrLength );

	#if defined( IC3_PCL_PROJECT_ROOT_PATH )
		auto replacePos = result.find( IC3_PCL_PROJECT_ROOT_PATH );
		auto replaceLength = std::strlen( IC3_PCL_PROJECT_ROOT_PATH );
		if( ( replacePos != std::string::npos ) && ( replaceLength > 0 ) )
		{
			result.erase( replacePos, replaceLength );
		}
	#endif

		result.append( ", line " );
		result.append( std::to_string( mLineNumber ) );

		return result;

	}

}
