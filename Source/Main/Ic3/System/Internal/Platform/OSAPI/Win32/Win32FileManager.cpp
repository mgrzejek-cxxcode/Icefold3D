
#include <Ic3/System/FileManagerNative.h>

#if( PCL_TARGET_SYSAPI == PCL_TARGET_SYSAPI_WIN32 )
namespace Ic3::System
{

	namespace Platform
	{

		HANDLE _win32OpenFileGeneric( const char * pFilePath, DWORD pFileAccess, DWORD pOpenMode, DWORD pFileFlags );

		void _win32CloseFile( HANDLE pFileHandle );

		DWORD _Win32TranslateFileOpenModeToWin32Access( EFileOpenMode pOpenMode );

		DWORD _Win32TranslateFileOpenModeToWin32CreationDisposition( EFileOpenMode pOpenMode );

		DWORD _Win32TranslateFilePointerRefPos( EFilePointerRefPos pFileRefPos );

		std::string _win32GenerateTempFileName();

	}
	
	
	Win32FileManager::Win32FileManager( SysContextHandle pSysContext )
	: Win32NativeObject( std::move( pSysContext ) )
	{}

	Win32FileManager::~Win32FileManager() noexcept = default;

	FileHandle Win32FileManager::_NativeOpenFile( std::string pFilePath, EFileOpenMode pOpenMode )
	{
		const auto fileAccess = Platform::_Win32TranslateFileOpenModeToWin32Access( pOpenMode );
		const auto creationDisposition = Platform::_Win32TranslateFileOpenModeToWin32CreationDisposition( pOpenMode );

		auto fileHandle = Platform::_win32OpenFileGeneric( pFilePath.c_str(), fileAccess, creationDisposition, FILE_ATTRIBUTE_NORMAL );
		auto fileObject = CreateSysObject<Win32File>( GetHandle<Win32FileManager>() );
		fileObject->SetInternalWin32FileHandle( fileHandle );

		return fileObject;
	}

	FileHandle Win32FileManager::_NativeCreateFile( std::string pFilePath )
	{
		const auto fileAccess = GENERIC_READ | GENERIC_WRITE;
		const auto creationDisposition = CREATE_ALWAYS;

		auto fileHandle = Platform::_win32OpenFileGeneric( pFilePath.c_str(), fileAccess, creationDisposition, FILE_ATTRIBUTE_NORMAL );
		auto fileObject = CreateSysObject<Win32File>( GetHandle<Win32FileManager>() );
		fileObject->SetInternalWin32FileHandle( fileHandle );

		return fileObject;
	}

	FileHandle Win32FileManager::_NativeCreateTemporaryFile()
	{
		const auto fileAccess = GENERIC_READ | GENERIC_WRITE;
		const auto creationDisposition = CREATE_ALWAYS;

		auto tempFilePath = Platform::_win32GenerateTempFileName();

		auto fileHandle = Platform::_win32OpenFileGeneric( tempFilePath.c_str(), fileAccess, creationDisposition, FILE_ATTRIBUTE_TEMPORARY );
		auto fileObject = CreateSysObject<Win32File>( GetHandle<Win32FileManager>() );
		fileObject->SetInternalWin32FileHandle( fileHandle );

		return fileObject;
	}

	FileNameList Win32FileManager::_NativeEnumDirectoryFileNameList( const std::string & pDirectory )
	{
		FileNameList resultList;

		auto targetDirectory = pDirectory;
		targetDirectory.append( 1, PCL_ENV_DEFAULT_PATH_DELIMITER );
		targetDirectory.append( 1, '*' );

		WIN32_FIND_DATAA win32FindFileData;
		auto win32FindFileHandle = ::FindFirstFileA( targetDirectory.c_str(), &win32FindFileData );

		if( win32FindFileHandle == INVALID_HANDLE_VALUE )
		{
			auto lastError = ::GetLastError();
			auto lastErrorMessage = Platform::WFAQuerySystemErrorMessage( lastError );
			Ic3ThrowDesc( eExcCodeDebugPlaceholder, std::move( lastErrorMessage ) );
		}

		while( win32FindFileHandle )
		{
			auto fileName = std::string( win32FindFileData.cFileName );
			auto fileAttributes = cppx::make_bitmask( win32FindFileData.dwFileAttributes );

			if( !fileAttributes.is_set( FILE_ATTRIBUTE_DIRECTORY ) && ( fileName != "." ) && ( fileName != ".." ) )
			{
				resultList.push_back( std::string( win32FindFileData.cFileName ) );
			}

			auto FindNextResult = ::FindNextFileA( win32FindFileHandle, &win32FindFileData );

			if( !FindNextResult )
			{
				auto lastError = ::GetLastError();
				if( lastError == ERROR_NO_MORE_FILES )
				{
					::FindClose( win32FindFileHandle );
					win32FindFileHandle = nullptr;
				}
				else
				{
					auto lastErrorMessage = Platform::WFAQuerySystemErrorMessage( lastError );
					Ic3ThrowDesc( eExcCodeDebugPlaceholder, std::move( lastErrorMessage ) );
				}
			}
		}

		return resultList;
	}

	std::string Win32FileManager::_NativeGenerateTemporaryFileName()
	{
		return Platform::_win32GenerateTempFileName();
	}

	bool Win32FileManager::_NativeCheckDirectoryExists( const std::string & pDirPath )
	{
		cppx::bitmask<DWORD> targetAttributes = ::GetFileAttributesA( pDirPath.c_str() );
		return ( targetAttributes != INVALID_FILE_ATTRIBUTES ) && targetAttributes.is_set( FILE_ATTRIBUTE_DIRECTORY );
	}

	bool Win32FileManager::_NativeCheckFileExists( const std::string & pFilePath )
	{
		cppx::bitmask<DWORD> targetAttributes = ::GetFileAttributesA( pFilePath.c_str() );
		return ( targetAttributes != INVALID_FILE_ATTRIBUTES ) && !targetAttributes.is_set( FILE_ATTRIBUTE_DIRECTORY );
	}
	
	
	Win32File::Win32File( FileManagerHandle pFileManager )
	: Win32NativeObject( std::move( pFileManager ) )
	{}

	Win32File::~Win32File() noexcept
	{
		_ReleaseWin32FileHandle();
	}

	void Win32File::SetInternalWin32FileHandle( HANDLE pFileHandle )
	{
		Ic3DebugAssert( !mNativeData.fileHandle );
		mNativeData.fileHandle = pFileHandle;
	}

	void Win32File::_ReleaseWin32FileHandle()
	{
		if( mNativeData.fileHandle )
		{
			Platform::_win32CloseFile( mNativeData.fileHandle );
			mNativeData.fileHandle = nullptr;
		}
	}

	file_size_t Win32File::_NativeReadData( void * pTargetBuffer, file_size_t pReadSize )
	{
		DWORD readBytesNum = 0u;
		auto readResult = ::ReadFile( mNativeData.fileHandle, pTargetBuffer, cppx::numeric_cast<DWORD>( pReadSize ), &readBytesNum, nullptr);

		if( readResult && ( readBytesNum == 0 ) )
		{
			mNativeData.flags.set( Platform::eWin32FileFlagEOFBit );
		}

		if( !readResult )
		{
			auto errorCode = ::GetLastError();
			if( errorCode == ERROR_HANDLE_EOF )
			{
				mNativeData.flags.set( Platform::eWin32FileFlagEOFBit );
			}
			else
			{
				auto errorMessage = Platform::WFAQuerySystemErrorMessage( errorCode );
				Ic3ThrowDesc( eExcCodeDebugPlaceholder, std::move( errorMessage ) );
			}
		}

		return readBytesNum;
	}

	file_size_t Win32File::_NativeWriteData( const void * pData, file_size_t pWriteSize )
	{
		DWORD writtenBytesNum = 0u;
		auto writeResult = ::WriteFile(  mNativeData.fileHandle, pData, cppx::numeric_cast< DWORD >( pWriteSize ), &writtenBytesNum, nullptr );

		if( !writeResult )
		{
			auto errorCode = ::GetLastError();
			auto errorMessage = Platform::WFAQuerySystemErrorMessage( errorCode );
			Ic3ThrowDesc( eExcCodeDebugPlaceholder, std::move( errorMessage ) );
		}

		return writtenBytesNum;
	}

	file_offset_t Win32File::_NativeSetFilePointer( file_offset_t pOffset, EFilePointerRefPos pRefPos )
	{
		LARGE_INTEGER u64FileOffset;
		u64FileOffset.QuadPart = static_cast<decltype( u64FileOffset.QuadPart )>( pOffset );

		auto win32FPMoveMode = Platform::_Win32TranslateFilePointerRefPos( pRefPos );

		u64FileOffset.LowPart = ::SetFilePointer( mNativeData.fileHandle,
		                                          u64FileOffset.LowPart,
		                                          &( u64FileOffset.HighPart ),
		                                          win32FPMoveMode );

		if( u64FileOffset.LowPart == INVALID_SET_FILE_POINTER )
		{
			auto lastError = ::GetLastError();
			if( lastError != NO_ERROR )
			{
				auto errorMessage = Platform::WFAQuerySystemErrorMessage( lastError );
				Ic3ThrowDesc( eExcCodeDebugPlaceholder, std::move( errorMessage ) );
			}
		}

		return cppx::numeric_cast<file_offset_t>( u64FileOffset.QuadPart );
	}

	file_offset_t Win32File::_NativeGetFilePointer() const
	{
		LARGE_INTEGER u64SetFileOffset;
		u64SetFileOffset.QuadPart = 0u;

		LARGE_INTEGER u64GetFilePosition;
		u64GetFilePosition.QuadPart = 0u;

		::SetFilePointerEx( mNativeData.fileHandle, u64SetFileOffset, &u64GetFilePosition, FILE_CURRENT );

		return cppx::numeric_cast<file_offset_t>( u64GetFilePosition.QuadPart );
	}

	file_size_t Win32File::_NativeGetSize() const
	{
		LARGE_INTEGER u64FileSize;
		u64FileSize.QuadPart = 0L;

		::GetFileSizeEx( mNativeData.fileHandle, &u64FileSize );

		return cppx::numeric_cast<file_size_t>( u64FileSize.QuadPart );
	}

	file_size_t Win32File::_NativeGetRemainingBytes() const
	{
		LARGE_INTEGER u64SetFileOffset;
		u64SetFileOffset.QuadPart = 0u;

		LARGE_INTEGER u64GetFilePosition;
		u64GetFilePosition.QuadPart = 0u;

		::SetFilePointerEx( mNativeData.fileHandle, u64SetFileOffset, &u64GetFilePosition, FILE_CURRENT );
		const auto previousFilePointer = u64GetFilePosition.QuadPart;

		::SetFilePointerEx( mNativeData.fileHandle, u64SetFileOffset, &u64GetFilePosition, FILE_END );
		const auto endFilePosition = u64GetFilePosition.QuadPart;

		u64SetFileOffset.QuadPart = previousFilePointer;

		::SetFilePointerEx( mNativeData.fileHandle, u64SetFileOffset, nullptr, FILE_BEGIN );

		return cppx::numeric_cast<file_offset_t>( endFilePosition - previousFilePointer );
	}

	bool Win32File::_NativeCheckEOF() const
	{
		return mNativeData.flags.is_set( Platform::eWin32FileFlagEOFBit );
	}

	bool Win32File::_NativeIsGood() const
	{
		return mNativeData.flags == 0;
	}


	namespace Platform
	{

		HANDLE _win32OpenFileGeneric( const char * pFilePath, DWORD pFileAccess, DWORD pOpenMode, DWORD pFileFlags )
		{
			auto fileHandle = ::CreateFileA( pFilePath, pFileAccess, 0u, nullptr, pOpenMode, pFileFlags, nullptr );

			if( fileHandle == INVALID_HANDLE_VALUE )
			{
				auto lastErrorCode = ::GetLastError();
				auto errorMessage = Platform::WFAQuerySystemErrorMessage( lastErrorCode );
				Ic3ThrowDesc( eEXCSystemFileOpenError, std::move( errorMessage ) );
			}

			return fileHandle;
		}

		void _win32CloseFile( HANDLE pFileHandle )
		{
			auto closeResult = ::CloseHandle( pFileHandle );

			if( closeResult == FALSE )
			{
				auto lastErrorCode = ::GetLastError();
				auto errorMessage = Platform::WFAQuerySystemErrorMessage( lastErrorCode );
				Ic3DebugInterrupt();
			}
		}

		DWORD _Win32TranslateFileOpenModeToWin32Access( EFileOpenMode pOpenMode )
		{
			switch( pOpenMode )
			{
			case EFileOpenMode::ReadOnly:
				return GENERIC_READ;

			case EFileOpenMode::ReadWrite:
				return GENERIC_READ | GENERIC_WRITE;

			case EFileOpenMode::WriteAppend:
				return GENERIC_WRITE;

			case EFileOpenMode::WriteOverwrite:
				return GENERIC_READ | GENERIC_WRITE;

			default:
				break;
			}

			return GENERIC_READ | GENERIC_WRITE;
		}

		DWORD _Win32TranslateFileOpenModeToWin32CreationDisposition( EFileOpenMode pOpenMode )
		{
			switch( pOpenMode )
			{
			case EFileOpenMode::ReadOnly:
				return OPEN_EXISTING;

			case EFileOpenMode::ReadWrite:
				return OPEN_ALWAYS;

			case EFileOpenMode::WriteAppend:
				return OPEN_ALWAYS;

			case EFileOpenMode::WriteOverwrite:
				return CREATE_ALWAYS;

			default:
				break;
			}

			return OPEN_ALWAYS;
		}

		DWORD _Win32TranslateFilePointerRefPos( EFilePointerRefPos pFileRefPos )
		{
			DWORD win32FPMoveMode = 0;

			switch( pFileRefPos )
			{
				case EFilePointerRefPos::FileBeg:
				{
					win32FPMoveMode = FILE_BEGIN;
					break;
				}
				case EFilePointerRefPos::FileEnd:
				{
					win32FPMoveMode = FILE_END;
					break;
				}
				case EFilePointerRefPos::CurrentPos:
				{
					win32FPMoveMode = FILE_CURRENT;
					break;
				}
			}

			return win32FPMoveMode;
		}

		std::string _win32GenerateTempFileName()
		{
			file_char_t tempFileDir[MAX_PATH];
			::GetTempPathA( MAX_PATH, tempFileDir );

			file_char_t tempFileName[MAX_PATH];
			::GetTempFileNameA( tempFileDir, "TS3", 0, tempFileName );

			return std::string( tempFileName );
		}

	}

}
#endif // PCL_TARGET_SYSAPI_WIN32
