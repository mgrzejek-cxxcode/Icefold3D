
#include "FileSystem.h"

namespace Ic3::System
{

	File::File( FileManagerHandle pFileManager, EIOAccessMode pAccessMode )
	: IOReadWriteStream( pFileManager->mSysContext, { pAccessMode } )
	, mFileManager( std::move( pFileManager ) )
	{}

	File::~File() noexcept = default;

	bool File::IsSeekSupported() const noexcept
	{
		return true;
	}

	bool File::IsValid() const noexcept
	{
		return _NativeIsValid();
	}

	io_size_t File::GetAvailableDataSize() const
	{
		return _NativeGetAvailableDataSize();
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

	io_offset_t File::MoveFilePointer( io_offset_t pOffset )
	{
		return _NativeSetFilePointer( pOffset, EIOPointerRefPos::CurrentPos );
	}

	io_offset_t File::SetFilePointer( io_offset_t pOffset, EIOPointerRefPos pRefPos )
	{
		return _NativeSetFilePointer( pOffset, pRefPos );
	}

	io_size_t File::ReadImpl( void * pTargetBuffer, io_size_t pReadSize )
	{
		if( !pTargetBuffer || ( pReadSize == 0 ) )
		{
			return 0;
		}

		return _NativeReadData( pTargetBuffer, pReadSize );
	}

	io_size_t File::WriteImpl( const void * pData , io_size_t pWriteSize )
	{
		if( !pData || ( pWriteSize == 0 ) )
		{
			return 0;
		}

		return _NativeWriteData( pData, pWriteSize );
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
