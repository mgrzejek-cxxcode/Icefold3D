
#include "RXMLParser.h"

namespace Ic3
{

	RxmlData RxmlParser::parseXMLContent( std::string pXMLContent )
	{
		RxmlData rxmlData;

		try
		{
			auto rxmlDocument = std::make_unique<RxmlDocument>();
			rxmlDocument->parse<0>( pXMLContent.data() );

			if( !rxmlDocument->first_node() )
			{
				return nullptr;
			}

			rxmlData.document = std::move( rxmlDocument );
			rxmlData.rootNode = rxmlData.document->first_node();
			rxmlData.textContent = std::move( pXMLContent );

			return rxmlData;
		}
		catch( const rapidxml::parse_error & pException )
		{
			( pException );
			Ic3DebugInterrupt();
		}
		catch( ... )
		{
			Ic3DebugInterrupt();
		}

		return nullptr;
	}

}
