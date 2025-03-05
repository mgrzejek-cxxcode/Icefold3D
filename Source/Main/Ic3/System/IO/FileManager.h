
#ifndef __IC3_SYSTEM_FILE_MANAGER_H__
#define __IC3_SYSTEM_FILE_MANAGER_H__

#include "FileCommon.h"
#include "IODataStream.h"
#include "../SysObject.h"

namespace Ic3::System
{

	class IC3_SYSTEM_CLASS FileManager : public SysObject
	{
	public:
		explicit FileManager( SysContextHandle pSysContext );
		virtual ~FileManager() noexcept;

		FileHandle OpenFile( std::string pFilePath, EIOAccessMode pAccessMode );

		FileHandle CreateFile( std::string pFilePath );

		FileHandle CreateTemporaryFile();

		FileList OpenDirectoryFiles( const std::string & pDirectory );

		FileNameList EnumDirectoryFiles( const std::string & pDirectory );

		std::string GenerateTemporaryFileName();

		bool CheckDirectoryExists( const std::string & pDirPath );

		bool CheckFileExists( const std::string & pFilePath );

	private:
		virtual FileHandle _NativeOpenFile( std::string pFilePath, EIOAccessMode pAccessMode ) = 0;
		virtual FileHandle _NativeCreateFile( std::string pFilePath ) = 0;
		virtual FileHandle _NativeCreateTemporaryFile() = 0;
		virtual FileNameList _NativeEnumDirectoryFileNameList( const std::string & pDirectory ) = 0;
		virtual std::string _NativeGenerateTemporaryFileName() = 0;
		virtual bool _NativeCheckDirectoryExists( const std::string & pDirPath ) = 0;
		virtual bool _NativeCheckFileExists( const std::string & pFilePath ) = 0;
	};

	class IC3_SYSTEM_CLASS File : public IODataStream
	{
	public:
		FileManagerHandle const mFileManager;

	public:
		explicit File( FileManagerHandle pFileManager );
		virtual ~File() noexcept;

		virtual io_size_t GetRemainingBytes() const noexcept override final;

		virtual bool IsSeekSupported() const noexcept override final;

		io_offset_t MoveFilePointer( io_offset_t pOffset );

		io_offset_t SetFilePointer( io_offset_t pPosition, EIOPointerRefPos pRefPos = EIOPointerRefPos::StreamBase );

		io_offset_t GetFilePointer() const;

		io_size_t GetSize() const;

		bool CheckEOF() const;

		bool IsGood() const;

	protected:
		virtual io_size_t ReadImpl( void * pTargetBuffer, io_size_t pReadSize ) override;

		virtual io_size_t WriteImpl( const void * pData , io_size_t pWriteSize ) override;

	private:
		virtual io_size_t _NativeReadData( void * pTargetBuffer, io_size_t pReadSize ) = 0;
		virtual io_size_t _NativeWriteData( const void * pData, io_size_t pWriteSize ) = 0;
		virtual io_offset_t _NativeSetFilePointer( io_offset_t pOffset, EIOPointerRefPos pRefPos ) = 0;
		virtual io_offset_t _NativeGetFilePointer() const = 0;
		virtual io_size_t _NativeGetSize() const = 0;
		virtual io_size_t _NativeGetRemainingBytes() const = 0;
		virtual bool _NativeCheckEOF() const = 0;
		virtual bool _NativeIsGood() const = 0;
	};

}

#endif //__IC3_SYSTEM_FILE_MANAGER_H__
