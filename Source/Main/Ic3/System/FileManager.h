
#ifndef __IC3_SYSTEM_FILE_MANAGER_H__
#define __IC3_SYSTEM_FILE_MANAGER_H__

#include "FileCommon.h"
#include "SysObject.h"
#include <cppx/byteArray.h>

namespace Ic3::System
{

	class IC3_SYSTEM_CLASS FileManager : public SysObject
	{
	public:
		explicit FileManager( SysContextHandle pSysContext );
		virtual ~FileManager() noexcept;

		FileHandle OpenFile( std::string pFilePath, EFileOpenMode pOpenMode );

		FileHandle CreateFile( std::string pFilePath );

		FileHandle CreateTemporaryFile();

		FileList OpenDirectoryFiles( const std::string & pDirectory );

		FileNameList EnumDirectoryFiles( const std::string & pDirectory );

		std::string GenerateTemporaryFileName();

		bool CheckDirectoryExists( const std::string & pDirPath );

		bool CheckFileExists( const std::string & pFilePath );

	private:
		virtual FileHandle _NativeOpenFile( std::string pFilePath, EFileOpenMode pOpenMode ) = 0;
		virtual FileHandle _NativeCreateFile( std::string pFilePath ) = 0;
		virtual FileHandle _NativeCreateTemporaryFile() = 0;
		virtual FileNameList _NativeEnumDirectoryFileNameList( const std::string & pDirectory ) = 0;
		virtual std::string _NativeGenerateTemporaryFileName() = 0;
		virtual bool _NativeCheckDirectoryExists( const std::string & pDirPath ) = 0;
		virtual bool _NativeCheckFileExists( const std::string & pFilePath ) = 0;
	};

	class IC3_SYSTEM_CLASS File : public SysObject
	{
	public:
		FileManagerHandle const mFileManager;

	public:
		explicit File( FileManagerHandle pFileManager );
		virtual ~File() noexcept;

		file_size_t Read( void * pTargetBuffer, file_size_t pTargetBufferSize, file_size_t pReadSize = cxFileSizeMax );
		file_size_t Read( const cppx::read_write_memory_view & pTarget, file_size_t pReadSize = cxFileSizeMax );

		file_size_t ReadAuto( cppx::dynamic_memory_buffer & pTarget, file_size_t pReadSize = cxFileSizeMax );
		file_size_t ReadAuto( cppx::dynamic_byte_array & pTarget, file_size_t pReadSize = cxFileSizeMax );

		template <typename TResizableBuffer>//, std::enable_if_t<!std::is_pointer<TResizableBuffer>::value, int>>
		file_size_t ReadAuto( TResizableBuffer & pTarget, file_size_t pReadSize = cxFileSizeMax )
		{
			return _ReadAuto( pTarget, pReadSize );
		}

		file_size_t Write( const void * pData, file_size_t pDataSize, file_size_t pWriteSize = cxFileSizeMax );
		file_size_t Write( const cppx::read_only_memory_view & pSource, file_size_t pWriteSize = cxFileSizeMax );
		
		file_size_t Write( const cppx::memory_buffer & pSource, file_size_t pWriteSize = cxFileSizeMax );
		file_size_t Write( const cppx::byte_array & pSource, file_size_t pWriteSize = cxFileSizeMax );

		template <typename TBuffer, std::enable_if_t<!std::is_pointer<TBuffer>::value, int>>
		file_size_t Write( const TBuffer & pSource, file_size_t pWriteSize )
		{
			return _Write( pSource, pWriteSize );
		}

		file_offset_t MoveFilePointer( file_offset_t pOffset );

		file_offset_t SetFilePointer( file_offset_t pPosition, EFilePointerRefPos pRefPos = EFilePointerRefPos::FileBeg );

		file_offset_t GetFilePointer() const;

		file_size_t GetSize() const;

		file_size_t GetRemainingBytes() const;

		bool CheckEOF() const;
		bool IsGood() const;

	private:
		template <typename TResizableBuffer>
		file_size_t _ReadAuto( TResizableBuffer & pTarget, file_size_t pReadSize )
		{
			if( pReadSize == 0 )
			{
				return 0;
			}

			const auto remainingFileDataSize = _NativeGetRemainingBytes();
			const auto readSize = cppx::get_min_of( pReadSize, remainingFileDataSize );

			pTarget.resize( readSize );

			return _NativeReadData( pTarget.data(), readSize );
		}

		template <typename TContinuousBuffer>
		file_size_t _Write( const TContinuousBuffer & pBuffer, file_size_t pWriteSize )
		{
			if( pWriteSize == 0 )
			{
				return 0;
			}

			const auto writeSize = cppx::get_min_of( pBuffer.size(), pWriteSize );

			return _NativeWriteData( pBuffer.data(), writeSize );
		}

		virtual file_size_t _NativeReadData( void * pTargetBuffer, file_size_t pReadSize ) = 0;
		virtual file_size_t _NativeWriteData( const void * pData, file_size_t pWriteSize ) = 0;
		virtual file_offset_t _NativeSetFilePointer( file_offset_t pOffset, EFilePointerRefPos pRefPos ) = 0;
		virtual file_offset_t _NativeGetFilePointer() const = 0;
		virtual file_size_t _NativeGetSize() const = 0;
		virtual file_size_t _NativeGetRemainingBytes() const = 0;
		virtual bool _NativeCheckEOF() const = 0;
		virtual bool _NativeIsGood() const = 0;
	};

}

#endif //__IC3_SYSTEM_FILE_MANAGER_H__
