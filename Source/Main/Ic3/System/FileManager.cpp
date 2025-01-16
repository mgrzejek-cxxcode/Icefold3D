
#include "FileManager.h"

namespace Ic3::System
{

	File::File( FileManagerHandle pFileManager )
	: SysObject( pFileManager->mSysContext )
	, mFileManager( std::move( pFileManager ) )
	{}

	File::~File() noexcept = default;

	file_size_t File::Read( void * pTargetBuffer, file_size_t pTargetBufferSize, file_size_t pReadSize )
	{
		if( !pTargetBuffer || ( pTargetBufferSize == 0 ) || ( pReadSize == 0 ) )
		{
			return 0;
		}

		const auto readSize = cppx::get_min_of( pTargetBufferSize, pReadSize );

		return _NativeReadData( pTargetBuffer, readSize );
	}

	file_size_t File::Read( const cppx::read_write_memory_view & pTarget, file_size_t pReadSize )
	{
		return Read( pTarget.data(), pTarget.size(), pReadSize );
	}

	file_size_t File::ReadAuto( cppx::dynamic_memory_buffer & pTarget, file_size_t pReadSize )
	{
		return _ReadAuto( pTarget, pReadSize );
	}

	file_size_t File::ReadAuto( cppx::dynamic_byte_array & pTarget, file_size_t pReadSize )
	{
		return _ReadAuto( pTarget, pReadSize );
	}

	file_size_t File::Write( const void * pData, file_size_t pDataSize, file_size_t pWriteSize )
	{
		if( !pData || ( pDataSize == 0 ) || ( pWriteSize == 0 ) )
		{
			return 0;
		}

		const auto writeSize = cppx::get_min_of( pDataSize, pWriteSize );

		return _NativeWriteData( pData, writeSize );
	}
	
	file_size_t File::Write( const cppx::memory_buffer & pSource, file_size_t pWriteSize )
	{
		return _Write( pSource, pWriteSize );
	}

	file_size_t File::Write( const cppx::byte_array & pSource, file_size_t pWriteSize )
	{
		return _Write( pSource, pWriteSize );
	}

	file_offset_t File::SetFilePointer( file_offset_t pOffset, EFilePointerRefPos pRefPos )
	{
		return _NativeSetFilePointer( pOffset, pRefPos );
	}

	file_offset_t File::GetFilePointer() const
	{
		return _NativeGetFilePointer();
	}

	file_size_t File::GetSize() const
	{
		return _NativeGetSize();
	}

	file_size_t File::GetRemainingBytes() const
	{
		return _NativeGetRemainingBytes();
	}

	bool File::CheckEOF() const
	{
		return _NativeCheckEOF();
	}

	bool File::IsGood() const
	{
		return _NativeIsGood();
	}


	FileManager::FileManager( SysContextHandle pSysContext )
	: SysObject( std::move( pSysContext ) )
	{}

	FileManager::~FileManager() noexcept = default;

	FileHandle FileManager::OpenFile( std::string pFilePath, EFileOpenMode pOpenMode )
	{
		return _NativeOpenFile( std::move( pFilePath ), pOpenMode );
	}

	FileHandle FileManager::CreateFile( std::string pFilePath )
	{
		return _NativeCreateFile( std::move( pFilePath ) );
	}

	FileHandle FileManager::CreateTemporaryFile()
	{
		return _NativeCreateTemporaryFile();
	}

	FileList FileManager::OpenDirectoryFiles( const std::string & pDirectory )
	{
		FileList resultFileList;

		auto fileNameList = EnumDirectoryFiles( pDirectory );

		if( !fileNameList.empty() )
		{
			resultFileList.reserve( fileNameList.size() );

			for( auto & fileName : fileNameList )
			{
				auto fileHandle = OpenFile( std::move( fileName ), EFileOpenMode::ReadOnly );
				resultFileList.push_back( std::move( fileHandle ) );
			}
		}

		return resultFileList;
	}

	FileNameList FileManager::EnumDirectoryFiles( const std::string & pDirectory )
	{
		if( pDirectory.empty() )
		{
			return {};
		}
		return _NativeEnumDirectoryFileNameList( pDirectory );
	}

	std::string FileManager::GenerateTemporaryFileName()
	{
		return _NativeGenerateTemporaryFileName();
	}

	bool FileManager::CheckDirectoryExists( const std::string & pDirPath )
	{
		if( pDirPath.empty() )
		{
			return false;
		}
		return _NativeCheckDirectoryExists( pDirPath );
	}

	bool FileManager::CheckFileExists( const std::string & pFilePath )
	{
		if( pFilePath.empty() )
		{
			return false;
		}
		return _NativeCheckFileExists( pFilePath );
	}

}
