
#ifndef __IC3_SYSTEM_FILE_MANAGER_H__
#define __IC3_SYSTEM_FILE_MANAGER_H__

#include "FileCommon.h"
#include "SysObject.h"
#include <Ic3/Cppx/ByteArray.h>

namespace Ic3::System
{

	class FileManager : public SysObject
	{
	public:
		explicit FileManager( SysContextHandle pSysContext );
		virtual ~FileManager() noexcept;

		FileHandle openFile( std::string pFilePath, EFileOpenMode pOpenMode );

		FileHandle createFile( std::string pFilePath );

		FileHandle createTemporaryFile();

		FileList openDirectoryFiles( const std::string & pDirectory );

		FileNameList enumDirectoryFiles( const std::string & pDirectory );

		std::string generateTemporaryFileName();

		bool checkDirectoryExists( const std::string & pDirPath );

		bool checkFileExists( const std::string & pFilePath );

	private:
		virtual FileHandle _nativeOpenFile( std::string pFilePath, EFileOpenMode pOpenMode ) = 0;
		virtual FileHandle _nativeCreateFile( std::string pFilePath ) = 0;
		virtual FileHandle _nativeCreateTemporaryFile() = 0;
		virtual FileNameList _nativeEnumDirectoryFileNameList( const std::string & pDirectory ) = 0;
		virtual std::string _nativeGenerateTemporaryFileName() = 0;
		virtual bool _nativeCheckDirectoryExists( const std::string & pDirPath ) = 0;
		virtual bool _nativeCheckFileExists( const std::string & pFilePath ) = 0;
	};

	class File : public SysObject
	{
	public:
		FileManagerHandle const mFileManager;

	public:
		explicit File( FileManagerHandle pFileManager );
		virtual ~File() noexcept;

		file_size_t read( void * pTargetBuffer, file_size_t pTargetBufferSize, file_size_t pReadSize = CX_FILE_SIZE_MAX );
		file_size_t read( const Cppx::ReadWriteMemoryView & pTarget, file_size_t pReadSize = CX_FILE_SIZE_MAX );

		file_size_t readAuto( Cppx::DynamicMemoryBuffer & pTarget, file_size_t pReadSize = CX_FILE_SIZE_MAX );
		file_size_t readAuto( Cppx::DynamicByteArray & pTarget, file_size_t pReadSize = CX_FILE_SIZE_MAX );

		template <typename TResizableBuffer>//, std::enable_if_t<!std::is_pointer<TResizableBuffer>::value, int>>
		file_size_t readAuto( TResizableBuffer & pTarget, file_size_t pReadSize = CX_FILE_SIZE_MAX )
		{
			return _readAuto( pTarget, pReadSize );
		}

		file_size_t write( const void * pData, file_size_t pDataSize, file_size_t pWriteSize = CX_FILE_SIZE_MAX );
		file_size_t write( const Cppx::ReadOnlyMemoryView & pSource, file_size_t pWriteSize = CX_FILE_SIZE_MAX );
		
		file_size_t write( const Cppx::MemoryBuffer & pSource, file_size_t pWriteSize = CX_FILE_SIZE_MAX );
		file_size_t write( const Cppx::ByteArray & pSource, file_size_t pWriteSize = CX_FILE_SIZE_MAX );

		template <typename TBuffer, std::enable_if_t<!std::is_pointer<TBuffer>::value, int>>
		file_size_t write( const TBuffer & pSource, file_size_t pWriteSize )
		{
			return _write( pSource, pWriteSize );
		}

		file_offset_t moveFilePointer( file_offset_t pOffset );

		file_offset_t setFilePointer( file_offset_t pPosition, EFilePointerRefPos pRefPos = EFilePointerRefPos::FileBeg );

		file_offset_t getFilePointer() const;

		file_size_t getSize() const;

		file_size_t getRemainingBytes() const;

		bool checkEOF() const;
		bool isGood() const;

	private:
		template <typename TResizableBuffer>
		file_size_t _readAuto( TResizableBuffer & pTarget, file_size_t pReadSize )
		{
			if( pReadSize == 0 )
			{
				return 0;
			}

			const auto remainingFileDataSize = _nativeGetRemainingBytes();
			const auto readSize = Cppx::getMinOf( pReadSize, remainingFileDataSize );

			pTarget.resize( readSize );

			return _nativeReadData( pTarget.data(), readSize );
		}

		template <typename TContinuousBuffer>
		file_size_t _write( const TContinuousBuffer & pBuffer, file_size_t pWriteSize )
		{
			if( pWriteSize == 0 )
			{
				return 0;
			}

			const auto writeSize = Cppx::getMinOf( pBuffer.size(), pWriteSize );

			return _nativeWriteData( pBuffer.data(), writeSize );
		}

		virtual file_size_t _nativeReadData( void * pTargetBuffer, file_size_t pReadSize ) = 0;
		virtual file_size_t _nativeWriteData( const void * pData, file_size_t pWriteSize ) = 0;
		virtual file_offset_t _nativeSetFilePointer( file_offset_t pOffset, EFilePointerRefPos pRefPos ) = 0;
		virtual file_offset_t _nativeGetFilePointer() const = 0;
		virtual file_size_t _nativeGetSize() const = 0;
		virtual file_size_t _nativeGetRemainingBytes() const = 0;
		virtual bool _nativeCheckEOF() const = 0;
		virtual bool _nativeIsGood() const = 0;
	};

}

#endif //__IC3_SYSTEM_FILE_MANAGER_H__
