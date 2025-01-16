
#ifndef __IC3_SYSTEM_FILE_COMMON_H__
#define __IC3_SYSTEM_FILE_COMMON_H__

#include "Prerequisites.h"
#include <Ic3/CoreLib/Exception.h>
#include <cppx/fsUtils.h>

namespace Ic3::System
{

	ic3SysDeclareHandle( FileManager );

	using FileHandle = std::shared_ptr<class File>;
	using FileList = std::vector<FileHandle>;
	using FileNameList = std::vector<std::string>;

	using file_char_t = char;
	using file_str_t = std::basic_string<file_char_t>;
	using file_offset_t = native_int;
	using file_size_t = native_uint;

	inline constexpr auto cxFileSizeMax = cppx::meta::limits<file_size_t>::max_value;

	/// @brief
	enum : exception_code_value_t
	{
		eEXCSystemFileOpenError = Ic3::CxDef::declareExceptionCode( eExceptionCategorySystemFile, 1 )
	};

	/// @brief
	enum class EFilePointerRefPos : enum_default_value_t
	{
		CurrentPos,
		FileBeg,
		FileEnd,
	};

	/// @brief
	enum class EFileOpenMode : enum_default_value_t
	{
		// Open file for reading. If the specified file does not exist, an error is reported.
		// - File pointer is set to a BEGINNING.
		// - Existing content is PRESERVED.
		ReadOnly,

		// Open file for reading and writing. If the specified file does not exist, a new one is created.
		// - File pointer is set to a BEGINNING.
		// - Existing content is PRESERVED.
		ReadWrite,

		// Open file for writing. If the specified file does not exist, a new one is created.
		// - File pointer is set to an END.
		// - Existing content is PRESERVED.
		WriteAppend,

		// Open file for writing. If the specified file does not exist, a new one is created.
		// - File pointer is set to a BEGINNING.
		// - Existing content is ERASED.
		WriteOverwrite
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

	ic3SetExceptionCategoryType( eExceptionCategorySystemFile, FileException );

}

#endif //__IC3_SYSTEM_FILE_COMMON_H__
