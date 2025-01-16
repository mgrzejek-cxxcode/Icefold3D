
#ifndef __CPPX_FS_UTILS_H__
#define __CPPX_FS_UTILS_H__

#include "bitmask.h"
#include <string>

namespace cppx
{

	enum fs_api_flags : uint32
	{
		fs_api_flag_split_assume_directory_bit = 0x0001,

		fs_api_flag_split_assume_file_bit      = 0x0002,

		fs_api_flags_split_default             = fs_api_flag_split_assume_directory_bit,
	};

	struct file_path_info
	{
		std::string directory;
		std::string file_name;
	};

	std::string fs_normalize_path( const std::string & pPath );

	file_path_info fs_split_path( std::string pFilePath, bitmask<fs_api_flags> pFlags = fs_api_flags_split_default );

	bool fs_is_filename_with_extension( const std::string & pFilename );

}

#endif // __CPPX_FS_UTILS_H__
