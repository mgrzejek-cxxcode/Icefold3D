
#ifndef __IC3_CPPX_FS_UTILS_H__
#define __IC3_CPPX_FS_UTILS_H__

#include "Bitmask.h"
#include <string>

namespace Ic3::Cppx
{

	enum EFSAPIFlags : uint32
	{
		E_FSAPI_SPLIT_PATH_FLAG_ASSUME_DIRECTORY_BIT = 0x0001,

		E_FSAPI_SPLIT_PATH_FLAG_ASSUME_FILE_BIT      = 0x0002,

		E_FSAPI_SPLIT_PATH_FLAGS_DEFAULT             = E_FSAPI_SPLIT_PATH_FLAG_ASSUME_DIRECTORY_BIT,

		E_FSAPI_SPLIT_PATH_MASK_ALL                  = E_FSAPI_SPLIT_PATH_FLAG_ASSUME_DIRECTORY_BIT |
		                                               E_FSAPI_SPLIT_PATH_FLAG_ASSUME_FILE_BIT
	};

	struct FilePathInfo
	{
		std::string directory;
		std::string fileName;
	};

	std::string fsNormalizePath( const std::string & pPath );

	FilePathInfo fsSplitPath( std::string pFilePath, Bitmask<EFSAPIFlags> pFlags = E_FSAPI_SPLIT_PATH_FLAGS_DEFAULT );

	bool fsIsFilenameWithExtension( const std::string & pFilename );

}

#endif // __IC3_CPPX_FS_UTILS_H__
