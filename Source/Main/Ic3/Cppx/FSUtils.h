
#ifndef __IC3_CPPX_FS_UTILS_H__
#define __IC3_CPPX_FS_UTILS_H__

#include "Bitmask.h"
#include <string>

namespace Ic3::Cppx
{

	enum EFSAPIFlags : uint32
	{
		eFSAPISplitPathFlagAssumeDirectoryBit = 0x0001,

		eFSAPISplitPathFlagAssumeFileBit      = 0x0002,

		eFSAPISplitPathFlagsDefault           = eFSAPISplitPathFlagAssumeDirectoryBit,

		eFSAPISplitPathMaskAll                = eFSAPISplitPathFlagAssumeDirectoryBit |
		                                        eFSAPISplitPathFlagAssumeFileBit
	};

	struct FilePathInfo
	{
		std::string mDirectory;
		std::string mFileName;
	};

	std::string fsNormalizePath( const std::string & pPath );

	FilePathInfo fsSplitPath( std::string pFilePath, TBitmask<EFSAPIFlags> pFlags = eFSAPISplitPathFlagsDefault );

	bool fsIsFilenameWithExtension( const std::string & pFilename );

}

#endif // __IC3_CPPX_FS_UTILS_H__
