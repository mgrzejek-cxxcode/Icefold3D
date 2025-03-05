
#ifndef __IC3_SYSTEM_PLATFORM_SHARED_POSIX_FILE_MANAGER_H__
#define __IC3_SYSTEM_PLATFORM_SHARED_POSIX_FILE_MANAGER_H__

#include "POSIXCommon.h"
#include <Ic3/System/FileManager.h>
#include <cstdio>
#include <cstring>

namespace Ic3::System
{

	namespace Platform
	{

		struct PosixFileNativeData
		{
			FILE * mFilePtr = nullptr;
		};

	}

	class IC3_SYSTEM_CLASS PosixFileManager : public FileManager
	{
	public:
		explicit PosixFileManager( SysContextHandle pSysContext );
		virtual ~PosixFileManager() noexcept;

	private:
		virtual FileHandle _NativeOpenFile( std::string pFilePath, EFileOpenMode pOpenMode ) override final;
		virtual FileHandle _NativeCreateFile( std::string pFilePath ) override final;
		virtual FileHandle _NativeCreateTemporaryFile() override final;
		virtual FileNameList _NativeEnumDirectoryFileNameList( const std::string & pDirectory ) override final;
		virtual std::string _NativeGenerateTemporaryFileName() override final;
		virtual bool _NativeCheckDirectoryExists( const std::string & pDirPath ) override final;
		virtual bool _NativeCheckFileExists( const std::string & pFilePath ) override final;
	};

	class IC3_SYSTEM_CLASS PosixFile : public NativeObject<File, Platform::PosixFileNativeData>
	{
		friend class PosixFileManager;

	public:
		explicit PosixFile( FileManagerHandle pFileManager );
		virtual ~PosixFile() noexcept;

	friendapi:
		void setInternalFilePtr( FILE * pFilePtr );

	private:
		void _ReleasePosixFileHandle();

		virtual file_size_t _NativeReadData( void * pTargetBuffer, file_size_t pReadSize ) override final;
		virtual file_size_t _NativeWriteData( const void * pData, file_size_t pWriteSize ) override final;
		virtual file_offset_t _NativeSetFilePointer( file_offset_t pOffset, EFilePointerRefPos pRefPos ) override final;
		virtual file_offset_t _NativeGetFilePointer() const override final;
		virtual file_size_t _NativeGetSize() const override final;
		virtual file_size_t _NativeGetRemainingBytes() const override final;
		virtual bool _NativeCheckEOF() const override final;
		virtual bool _NativeIsGood() const override final;
	};

} // namespace Ic3::System

#endif // __IC3_SYSTEM_PLATFORM_SHARED_POSIX_FILE_MANAGER_H__
