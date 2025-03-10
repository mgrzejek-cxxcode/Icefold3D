
#include "scfXMLReader.h"
#include <Ic3/System/IO/FileSystem.h>

namespace Ic3
{

	SCFXMLRootNode SCFXMLReader::readFile( const std::string & pFilename )
	{
		auto file = _sysFileManager->openFile( pFilename, System::EIOAccessMode::ReadOnly );
		if( !file )
		{
			return nullptr;
		}

		std::string xmlContent;
		file->readAuto( xmlContent );

		if( xmlContent.empty() )
		{
			return nullptr;
		}

		try
		{
			auto xmlTree = XMLReader::readXMLContent( std::move ( xmlContent ) );
			if( !xmlTree.valid() || xmlTree.empty() )
			{
				return nullptr;
			}

			return SCFXMLRootNode::initFromXMLTree( std::move( xmlTree ) );
		}
		catch( const rapidxml::parse_error & pException )
		{
			Ic3DebugInterrupt();
		}
		catch( ... )
		{
			Ic3DebugInterrupt();
		}

		return nullptr;
	}

} // namespace Ic3
