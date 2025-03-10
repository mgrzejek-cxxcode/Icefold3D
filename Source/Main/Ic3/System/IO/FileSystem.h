
#ifndef __IC3_SYSTEM_FILE_SYSTEM_H__
#define __IC3_SYSTEM_FILE_SYSTEM_H__

#include "IOStreamTypes.h"
#include "../SysObject.h"

namespace Ic3::System
{

	class IC3_SYSTEM_CLASS FileManager : public SysObject
	{
	public:
		explicit FileManager( SysContextHandle pSysContext );
		virtual ~FileManager() noexcept;

		CPPX_ATTR_NO_DISCARD FileHandle OpenFile( std::string pFilePath, EIOAccessMode pAccessMode );

		CPPX_ATTR_NO_DISCARD FileHandle CreateFile( std::string pFilePath );

		CPPX_ATTR_NO_DISCARD FileHandle CreateTemporaryFile();

		CPPX_ATTR_NO_DISCARD FileList OpenDirectoryFiles( const std::string & pDirectory );

		CPPX_ATTR_NO_DISCARD FileNameList EnumDirectoryFiles( const std::string & pDirectory );

		CPPX_ATTR_NO_DISCARD std::string GenerateTemporaryFileName();

		CPPX_ATTR_NO_DISCARD bool CheckDirectoryExists( const std::string & pDirPath );

		CPPX_ATTR_NO_DISCARD bool CheckFileExists( const std::string & pFilePath );

	private:
		virtual FileHandle _NativeOpenFile( std::string pFilePath, EIOAccessMode pAccessMode ) = 0;
		virtual FileHandle _NativeCreateFile( std::string pFilePath ) = 0;
		virtual FileHandle _NativeCreateTemporaryFile() = 0;
		virtual FileNameList _NativeEnumDirectoryFileNameList( const std::string & pDirectory ) = 0;
		virtual std::string _NativeGenerateTemporaryFileName() = 0;
		virtual bool _NativeCheckDirectoryExists( const std::string & pDirPath ) = 0;
		virtual bool _NativeCheckFileExists( const std::string & pFilePath ) = 0;
	};

	class IC3_SYSTEM_CLASS File : public IOReadWriteStream
	{
	public:
		FileManagerHandle const mFileManager;

	public:
		explicit File( FileManagerHandle pFileManager, EIOAccessMode pAccessMode );
		virtual ~File() noexcept;

		CPPX_ATTR_NO_DISCARD virtual bool IsSeekSupported() const noexcept override final;

		CPPX_ATTR_NO_DISCARD virtual bool IsValid() const noexcept override final;

		CPPX_ATTR_NO_DISCARD virtual io_size_t GetAvailableDataSize() const override final;

		CPPX_ATTR_NO_DISCARD io_offset_t GetFilePointer() const;

		CPPX_ATTR_NO_DISCARD io_size_t GetSize() const;

		CPPX_ATTR_NO_DISCARD bool CheckEOF() const;

		CPPX_ATTR_NO_DISCARD bool IsGood() const;

		io_offset_t MoveFilePointer( io_offset_t pOffset );

		io_offset_t SetFilePointer( io_offset_t pPosition, EIOPointerRefPos pRefPos = EIOPointerRefPos::StreamBase );

	protected:
		virtual io_size_t ReadImpl( void * pTargetBuffer, io_size_t pReadSize ) override;
		virtual io_size_t WriteImpl( const void * pData , io_size_t pWriteSize ) override;

	private:
		virtual bool _NativeIsValid() const noexcept = 0;
		virtual io_size_t _NativeGetAvailableDataSize() const = 0;
		virtual io_offset_t _NativeGetFilePointer() const = 0;
		virtual io_size_t _NativeGetSize() const = 0;
		virtual bool _NativeCheckEOF() const = 0;
		virtual bool _NativeIsGood() const = 0;
		virtual io_size_t _NativeReadData( void * pTargetBuffer, io_size_t pReadSize ) = 0;
		virtual io_size_t _NativeWriteData( const void * pData, io_size_t pWriteSize ) = 0;
		virtual io_offset_t _NativeSetFilePointer( io_offset_t pOffset, EIOPointerRefPos pRefPos ) = 0;
	};

}

#endif //__IC3_SYSTEM_FILE_SYSTEM_H__
