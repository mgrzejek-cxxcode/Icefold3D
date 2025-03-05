
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
			HANDLE fileHandle = nullptr;
			cppx::bitmask<EWin32FileFlags> flags = 0;
		};

	}

	class Win32FileManager : public Win32NativeObject<FileManager, void>
	{
	public:
		explicit Win32FileManager( SysContextHandle pSysContext );
		virtual ~Win32FileManager() noexcept;

	private:
		virtual FileHandle _NativeOpenFile( std::string pFilePath, EFileOpenMode pOpenMode ) override final;
		virtual FileHandle _NativeCreateFile( std::string pFilePath ) override final;
		virtual FileHandle _NativeCreateTemporaryFile() override final;
		virtual FileNameList _NativeEnumDirectoryFileNameList( const std::string & pDirectory ) override final;
		virtual std::string _NativeGenerateTemporaryFileName() override final;
		virtual bool _NativeCheckDirectoryExists( const std::string & pDirPath ) override final;
		virtual bool _NativeCheckFileExists( const std::string & pFilePath ) override final;
	};

	class Win32File : public Win32NativeObject<File, Platform::Win32FileNativeData>
	{
		friend class Win32FileManager;

	public:
		explicit Win32File( FileManagerHandle pFileManager );
		virtual ~Win32File() noexcept;

	friendapi:
		void SetInternalWin32FileHandle( HANDLE pFileHandle );

	private:
		void _ReleaseWin32FileHandle();

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

#endif // __IC3_SYSTEM_PLATFORM_OSAPI_WIN32_FILE_MANAGER_H__
