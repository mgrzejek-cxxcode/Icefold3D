
#include "FileManager.h"

namespace Ic3::System
{

	File::File( FileManagerHandle pFileManager )
	: IODataStream( pFileManager->mSysContext )
	, mFileManager( std::move( pFileManager ) )
	{}

	File::~File() noexcept = default;

	io_size_t File::GetRemainingBytes() const noexcept
	{
		return _NativeGetRemainingBytes();
	}

	bool File::IsSeekSupported() const noexcept
	{
		return true;
	}

	io_offset_t File::MoveFilePointer( io_offset_t pOffset )
	{
		return _NativeSetFilePointer( pOffset, EIOPointerRefPos::CurrentPos );
	}

	io_offset_t File::SetFilePointer( io_offset_t pOffset, EIOPointerRefPos pRefPos )
	{
		return _NativeSetFilePointer( pOffset, pRefPos );
	}

	io_offset_t File::GetFilePointer() const
	{
		return _NativeGetFilePointer();
	}

	io_size_t File::GetSize() const
	{
		return _NativeGetSize();
	}

	bool File::CheckEOF() const
	{
		return _NativeCheckEOF();
	}

	bool File::IsGood() const
	{
		return _NativeIsGood();
	}

	io_size_t File::ReadImpl( void * pTargetBuffer, io_size_t pReadSize )
	{
		if( !pTargetBuffer || ( pTargetBufferSize == 0 ) || ( pReadSize == 0 ) )
		{
			return 0;
		}

		const auto readSize = cppx::get_min_of( pTargetBufferSize, pReadSize );

		return _NativeReadData( pTargetBuffer, readSize );
	}

	io_size_t File::WriteImpl( const void * pData , io_size_t pWriteSize )
	{
		if( !pData || ( pDataSize == 0 ) || ( pWriteSize == 0 ) )
		{
			return 0;
		}

		const auto writeSize = cppx::get_min_of( pDataSize, pWriteSize );

		return _NativeWriteData( pData, writeSize );
	}


	FileManager::FileManager( SysContextHandle pSysContext )
	: SysObject( std::move( pSysContext ) )
	{}

	FileManager::~FileManager() noexcept = default;

	FileHandle FileManager::OpenFile( std::string pFilePath, EIOAccessMode pAccessMode )
	{
		return _NativeOpenFile( std::move( pFilePath ), pAccessMode );
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
				auto fileHandle = OpenFile( std::move( fileName ), EIOAccessMode::ReadOnly );
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
