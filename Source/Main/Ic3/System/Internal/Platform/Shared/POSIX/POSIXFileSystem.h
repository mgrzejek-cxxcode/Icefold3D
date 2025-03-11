
#ifndef __IC3_SYSTEM_PLATFORM_SHARED_POSIX_FILE_SYSTEM_H__
#define __IC3_SYSTEM_PLATFORM_SHARED_POSIX_FILE_SYSTEM_H__

#include "POSIXCommon.h"
#include <Ic3/System/IO/FileSystem.h>
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
		virtual FileHandle _NativeOpenFile( std::string pFilePath, EIOAccessMode pAccessMode ) override final;
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

	friendapi( private ):
		void SetInternalFilePtr( FILE * pFilePtr );

	private:
		void _ReleasePosixFileHandle();

		virtual io_size_t _NativeReadData( void * pTargetBuffer, io_size_t pReadSize ) override final;
		virtual io_size_t _NativeWriteData( const void * pData, io_size_t pWriteSize ) override final;
		virtual io_offset_t _NativeSetFilePointer( io_offset_t pOffset, EIOPointerRefPos pRefPos ) override final;
		virtual io_offset_t _NativeGetFilePointer() const override final;
		virtual io_size_t _NativeGetSize() const override final;
		virtual io_size_t _NativeGetAvailableDataSize() const override final;
		virtual bool _NativeCheckEOF() const override final;
		virtual bool _NativeIsGood() const override final;
	};

} // namespace Ic3::System

#endif // __IC3_SYSTEM_PLATFORM_SHARED_POSIX_FILE_SYSTEM_H__
