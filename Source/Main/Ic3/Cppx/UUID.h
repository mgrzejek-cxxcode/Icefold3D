
#include "Prerequisites.h"

namespace Ic3::Cppx
{

	inline std::string getUUIDString( const std::string & pInput, char pLeftDelim = '{' )
	{
		std::string result;

		char rightDelim = 0;

		switch( pLeftDelim )
		{
			case '{': rightDelim = '}'; break;
			case '[': rightDelim = ']'; break;
			case '<': rightDelim = '>'; break;
			case '$': rightDelim = '$'; break;
			default : break;
		}

		if( rightDelim != 0 )
		{
			auto lbPos = pInput.find_first_of( pLeftDelim );
			auto rbPos = pInput.find_first_of( rightDelim, lbPos );

			if( ( lbPos != std::string::npos ) && ( rbPos != std::string::npos ) )
			{
				auto uuidStr = pInput.substr( lbPos, rbPos - lbPos + 1 );
				// #xxxxxxxx-xxxx-Mxxx-Nxxx-xxxxxxxxxxxx#
				if( uuidStr.length() == 38 )
				{
					result = std::move( uuidStr );
				}
			}
		}

		return result;
	}

}
