
#include "POSIXFileSystem.h"
#include <dirent.h>
#include <sys/stat.h>

namespace Ic3::System
{

	namespace Platform
	{

		FILE * _PAOpenFileGeneric( const char * pFilePath, const char * pAccessMode );

		const char * _PATranslatEIOAccessMode( EIOAccessMode pAccessMode );

		bool _PACheckIsFile( const char * pFilePath );

		int _PATranslatEIOPointerRefPos( EIOPointerRefPos pFileRefPos );

		std::string _PAGenerateTempFileName();

	}
	
	PosixFileManager::PosixFileManager( SysContextHandle pSysContext )
	: FileManager( std::move( pSysContext ) )
	{}

	PosixFileManager::~PosixFileManager() noexcept = default;

	FileHandle PosixFileManager::_NativeOpenFile( std::string pFilePath, EIOAccessMode pAccessMode )
	{
		auto openMode = Platform::_PATranslatEIOAccessMode( pAccessMode );
		auto filePtr = Platform::_PAOpenFileGeneric( pFilePath.c_str(), openMode );
		auto fileObject = CreateSysObject<PosixFile>( GetHandle<PosixFileManager>() );
		fileObject->SetInternalFilePtr( filePtr );

		return fileObject;
	}

	FileHandle PosixFileManager::_NativeCreateFile( std::string pFilePath )
	{
		auto filePtr = Platform::_PAOpenFileGeneric( pFilePath.c_str(), "w+" );
		auto fileObject = CreateSysObject<PosixFile>( GetHandle<PosixFileManager>() );
		fileObject->SetInternalFilePtr( filePtr );

		return fileObject;
	}

	FileHandle PosixFileManager::_NativeCreateTemporaryFile()
	{
		auto tempFilePath = Platform::_PAGenerateTempFileName();

		auto filePtr = Platform::_PAOpenFileGeneric( tempFilePath.c_str(), "w+" );
		auto fileObject = CreateSysObject<PosixFile>( GetHandle<PosixFileManager>() );
		fileObject->SetInternalFilePtr( filePtr );

		return fileObject;
	}

	FileNameList PosixFileManager::_NativeEnumDirectoryFileNameList( const std::string & pDirectory )
	{
		FileNameList resultList;

		if( auto * dirPtr = ::opendir( pDirectory.c_str() ) )
		{
			for( auto * dirEntry = ::readdir( dirPtr ); dirEntry; )
			{
				if( ( strcmp( dirEntry->d_name, "." ) != 0 ) && ( strcmp( dirEntry->d_name, ".." ) != 0 ) )
				{
					if( dirEntry->d_type != DT_UNKNOWN )
					{
						if( ( dirEntry->d_type == DT_REG ) || ( dirEntry->d_type == DT_LNK ) )
						{
							resultList.push_back( std::string( dirEntry->d_name ) );
						}
					}
					else
					{
						const std::string filePath = pDirectory + PCL_ENV_DEFAULT_PATH_DELIMITER + dirEntry->d_name;

						struct stat statInfo;
						auto statResult = ::stat( filePath.c_str(), &statInfo );

						if( statResult == 0 )
						{
							auto fileMode = ( statInfo.st_mode & S_IFMT );
							if( ( fileMode == S_IFREG ) || ( fileMode == S_IFLNK ) )
							{
								resultList.push_back( std::string( dirEntry->d_name ) );
							}
						}
					}
				}
				dirEntry = ::readdir( dirPtr );
			}
			::closedir( dirPtr );
		}

		return resultList;
	}

	std::string PosixFileManager::_NativeGenerateTemporaryFileName()
	{
		return Platform::_PAGenerateTempFileName();
	}

	bool PosixFileManager::_NativeCheckDirectoryExists( const std::string & pDirPath )
	{
		struct stat statInfo;
		auto statResult = ::stat( pDirPath.c_str(), &statInfo );
		return ( statResult == 0 ) && ( ( statInfo.st_mode & S_IFMT ) == S_IFDIR );
	}

	bool PosixFileManager::_NativeCheckFileExists( const std::string & pFilePath )
	{
		struct stat statInfo;
		auto statResult = ::stat( pFilePath.c_str(), &statInfo );
		return ( statResult == 0 ) && ( ( statInfo.st_mode & S_IFMT ) != S_IFDIR );
	}


	PosixFile::PosixFile( FileManagerHandle pFileManager )
	: NativeObject( std::move( pFileManager ) )
	{}

	PosixFile::~PosixFile() noexcept
	{
		_ReleasePosixFileHandle();
	}

	void PosixFile::SetInternalFilePtr( FILE * pFilePtr )
	{
		Ic3DebugAssert( !mNativeData.mFilePtr );
		mNativeData.mFilePtr = pFilePtr;
	}

	void PosixFile::_ReleasePosixFileHandle()
	{
		if( mNativeData.mFilePtr )
		{
			::fclose( mNativeData.mFilePtr );
			mNativeData.mFilePtr = nullptr;
		}
	}

	io_size_t PosixFile::_NativeReadData( void * pTargetBuffer, io_size_t pReadSize )
	{
		auto readBytesNum = ::fread( pTargetBuffer, 1, pReadSize, mNativeData.mFilePtr );
		return cppx::numeric_cast<io_size_t>( readBytesNum );
	}

	io_size_t PosixFile::_NativeWriteData( const void * pData, io_size_t pWriteSize )
	{
		auto writtenBytesNum = ::fwrite( pData, 1, pWriteSize, mNativeData.mFilePtr );
		return cppx::numeric_cast<io_size_t>( writtenBytesNum );
	}

	io_offset_t PosixFile::_NativeSetFilePointer( io_offset_t pOffset, EIOPointerRefPos pRefPos )
	{
		auto fileSeekPos = Platform::_PATranslatEIOPointerRefPos( pRefPos );
		auto seekResult = ::fseek( mNativeData.mFilePtr, static_cast<long>( pOffset ), fileSeekPos );

		if( seekResult != 0 )
		{
			auto errnoString = Platform::PXAQueryErrnoStringByCode( errno );
			Ic3ThrowDesc( eExcCodeDebugPlaceholder, std::move( errnoString ) );
		}

		auto currentFilePointer = ::ftell( mNativeData.mFilePtr );
		return static_cast<io_offset_t>( currentFilePointer );
	}

	io_offset_t PosixFile::_NativeGetFilePointer() const
	{
		auto currentFilePointer = ::ftell( mNativeData.mFilePtr );
		return static_cast<io_offset_t>( currentFilePointer );
	}

	io_size_t PosixFile::_NativeGetSize() const
	{
		auto savedFilePointer = ::ftell( mNativeData.mFilePtr );
		::fseek( mNativeData.mFilePtr, 0u, SEEK_END );

		auto fileSize = ::ftell( mNativeData.mFilePtr );
		::fseek( mNativeData.mFilePtr, savedFilePointer, SEEK_SET );

		return static_cast<io_size_t>( fileSize );
	}

	io_size_t PosixFile::_NativeGetAvailableDataSize() const
	{
		auto currentFilePointer = ::ftell( mNativeData.mFilePtr );
		::fseek( mNativeData.mFilePtr, 0u, SEEK_END );

		auto fileSize = ::ftell( mNativeData.mFilePtr );
		::fseek( mNativeData.mFilePtr, currentFilePointer, SEEK_SET );

		return static_cast<io_size_t>( fileSize - currentFilePointer );
	}

	bool PosixFile::_NativeCheckEOF() const
	{
		return ::feof( mNativeData.mFilePtr ) != 0;
	}

	bool PosixFile::_NativeIsGood() const
	{
		return !::feof( mNativeData.mFilePtr ) && !::ferror( mNativeData.mFilePtr );
	}


	namespace Platform
	{

		FILE * _PAOpenFileGeneric( const char * pFilePath, const char * pAccessMode )
		{
			FILE * filePtr = fopen( pFilePath, pAccessMode );

			if( !filePtr )
			{
				auto errnoString = Platform::PXAQueryErrnoStringByCode( errno );
				Ic3ThrowDesc( eExcCodeSystemIOError, std::move( errnoString ) );
			}

			return filePtr;
		}

		const char * _PATranslatEIOAccessMode( EIOAccessMode pAccessMode )
		{
			switch( pAccessMode )
			{
				case EIOAccessMode::ReadOnly: return "r";
				case EIOAccessMode::ReadWrite: return "r+";
				case EIOAccessMode::WriteAppend: return "a";
				case EIOAccessMode::WriteOverwrite: return "w+";
			}
			return "r";
		}

		bool _PACheckIsFile( const char * pFilePath )
		{
			struct stat statInfo;
			auto statResult = ::stat( pFilePath, &statInfo );
			return ( statResult == 0 ) && ( ( statInfo.st_mode & S_IFMT ) == S_IFREG ) && ( ( statInfo.st_mode & S_IFMT ) == S_IFLNK );
		}

		int _PATranslatEIOPointerRefPos( EIOPointerRefPos pFileRefPos )
		{
			int fileSeekPos = 0;

			switch( pFileRefPos )
			{
				case EIOPointerRefPos::StreamBase:
				{
					fileSeekPos = SEEK_SET;
					break;
				}
				case EIOPointerRefPos::StreamEnd:
				{
					fileSeekPos = SEEK_END;
					break;
				}
				case EIOPointerRefPos::CurrentPos:
				{
					fileSeekPos = SEEK_CUR;
					break;
				}
			}

			return fileSeekPos;
		}

		std::string _PAGenerateTempFileName()
		{
			return tmpnam( nullptr );
		}

	}

}
