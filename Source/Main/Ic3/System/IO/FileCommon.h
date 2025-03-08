
#ifndef __IC3_SYSTEM_FILE_COMMON_H__
#define __IC3_SYSTEM_FILE_COMMON_H__

#include "IOCommonDefs.h"
#include <Ic3/CoreLib/Exception.h>

namespace Ic3::System
{

	Ic3SysDeclareHandle( FileManager );

	using FileHandle = std::shared_ptr<class File>;
	using FileList = std::vector<FileHandle>;
	using FileNameList = std::vector<std::string>;

	using file_char_t = char;

	/// @brief
	enum : exception_code_value_t
	{
		eEXCSystemFileOpenError = Ic3::CXU::DeclareExceptionCode( eExceptionCategorySystemFile, 1 )
	};

	/// @brief
	class FileException : public SystemException
	{
	public:
		std::string mFilePath;

	public:
		explicit FileException( ExceptionInfo pInfo, std::string pFilePath = {} )
		: SystemException( std::move( pInfo ) )
		, mFilePath( std::move( pFilePath ) )
		{}
	};

	Ic3SetExceptionCategoryType( eExceptionCategorySystemFile, FileException );

}

#endif //__IC3_SYSTEM_FILE_COMMON_H__
