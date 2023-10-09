
#pragma once

#ifndef __IC3_NXMAIN_SCF_XML_PUBLIC_H__
#define __IC3_NXMAIN_SCF_XML_PUBLIC_H__

#include "scfXMLCommon.h"
#include <Ic3/System/FileCommon.h>

namespace Ic3
{

	class SCFXMLReader
	{
	public:
		explicit SCFXMLReader( System::FileManagerHandle pSysFileManager )
		: _sysFileManager( pSysFileManager )
		{}

		SCFXMLRootNode readFile( const std::string & pFilename );

	private:
		System::FileManagerHandle _sysFileManager;
	};

} // namespace Ic3

#endif // __IC3_NXMAIN_SCF_XML_PUBLIC_H__
