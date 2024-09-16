
#ifndef __IC3_SYSTEM_PLATFORM_OSAPI_WIN32_FILE_MANAGER_H__
#define __IC3_SYSTEM_PLATFORM_OSAPI_WIN32_FILE_MANAGER_H__

#include <Ic3/System/FileManager.h>
#include "Win32FileAPI.h"

namespace Ic3::System
{

	namespace Platform
	{
		enum EWin32FileFlags : uint32
		{
			eWin32FileFlagEOFBit = 0xF0
		};

		struct Win32FileNativeData
		{
			HANDLE mFileHandle = nullptr;
			TBitmask<EWin32FileFlags> mFlags = 0;
		};

	}

	class Win32FileManager : public Win32NativeObject<FileManager, void>
	{
	public:
		explicit Win32FileManager( SysContextHandle pSysContext );
		virtual ~Win32FileManager() noexcept;

	private:
		virtual FileHandle _nativeOpenFile( std::string pFilePath, EFileOpenMode pOpenMode ) override final;
		virtual FileHandle _nativeCreateFile( std::string pFilePath ) override final;
		virtual FileHandle _nativeCreateTemporaryFile() override final;
		virtual FileNameList _nativeEnumDirectoryFileNameList( const std::string & pDirectory ) override final;
		virtual std::string _nativeGenerateTemporaryFileName() override final;
		virtual bool _nativeCheckDirectoryExists( const std::string & pDirPath ) override final;
		virtual bool _nativeCheckFileExists( const std::string & pFilePath ) override final;
	};

	class Win32File : public Win32NativeObject<File, Platform::Win32FileNativeData>
	{
		friend class Win32FileManager;

	public:
		explicit Win32File( FileManagerHandle pFileManager );
		virtual ~Win32File() noexcept;

	friendapi:
		void setInternalWin32FileHandle( HANDLE pFileHandle );

	private:
		void _releaseWin32FileHandle();

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

#endif // __IC3_SYSTEM_PLATFORM_OSAPI_WIN32_FILE_MANAGER_H__
