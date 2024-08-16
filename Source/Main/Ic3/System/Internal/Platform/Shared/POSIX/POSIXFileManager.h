
#ifndef __IC3_SYSTEM_PLATFORM_OSAPI_POSIX_FILE_MANAGER_H__
#define __IC3_SYSTEM_PLATFORM_OSAPI_POSIX_FILE_MANAGER_H__

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
			FILE * filePtr = nullptr;
		};

	}

	class PosixFileManager : public FileManager
	{
	public:
		explicit PosixFileManager( SysContextHandle pSysContext );
		virtual ~PosixFileManager() noexcept;

	private:
		virtual FileHandle _nativeOpenFile( std::string pFilePath, EFileOpenMode pOpenMode ) override final;
		virtual FileHandle _nativeCreateFile( std::string pFilePath ) override final;
		virtual FileHandle _nativeCreateTemporaryFile() override final;
		virtual FileNameList _nativeEnumDirectoryFileNameList( const std::string & pDirectory ) override final;
		virtual std::string _nativeGenerateTemporaryFileName() override final;
		virtual bool _nativeCheckDirectoryExists( const std::string & pDirPath ) override final;
		virtual bool _nativeCheckFileExists( const std::string & pFilePath ) override final;
	};

	class PosixFile : public NativeObject<File, Platform::PosixFileNativeData>
	{
		friend class PosixFileManager;

	public:
		explicit PosixFile( FileManagerHandle pFileManager );
		virtual ~PosixFile() noexcept;

	friendapi:
		void setInternalFilePtr( FILE * pFilePtr );

	private:
		void _releasePosixFileHandle();

		virtual file_size_t _nativeReadData( void * pTargetBuffer, file_size_t pReadSize ) override final;
		virtual file_size_t _nativeWriteData( const void * pData, file_size_t pWriteSize ) override final;
		virtual file_offset_t _nativeSetFilePointer( file_offset_t pOffset, EFilePointerRefPos pRefPos ) override final;
		virtual file_offset_t _nativeGetFilePointer() const override final;
		virtual file_size_t _nativeGetSize() const override final;
		virtual file_size_t _nativeGetRemainingBytes() const override final;
		virtual bool _nativeCheckEOF() const override final;
		virtual bool _nativeIsGood() const override final;
	};

} // namespace Ic3::System

#endif // __IC3_SYSTEM_PLATFORM_OSAPI_POSIX_FILE_MANAGER_H__
