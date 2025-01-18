
#include "FileBasedAssetSystem.h"
#include <Ic3/System/FileManager.h>
#include <Ic3/System/SysContext.h>
#include <Ic3/System/AssetSystem.h>

namespace Ic3::System
{

	namespace Platform
	{

		std::string _commonResolveFileName( FileManager & pFileManager,
		                                    cppx::file_path_info & pAssetPathInfo,
		                                    cppx::bitmask<EAssetOpenFlags> pFlags );

	}


	FileAssetLoader::FileAssetLoader( FileManagerHandle pFileManager )
	: NativeObject( pFileManager->mSysContext )
	, mFileManager( std::move( pFileManager ) ) 
	{}

	FileAssetLoader::~FileAssetLoader() noexcept = default;

	void FileAssetLoader::SetRootDir( std::string pRootDir )
	{
		mNativeData.rootDir = std::move( pRootDir );
	}

	AssetHandle FileAssetLoader::_NativeOpenSubAsset( cppx::file_path_info pAssetPathInfo, cppx::bitmask<EAssetOpenFlags> pFlags )
	{
		TSysHandle<FileAsset> asset = nullptr;

		auto fileDirectoryPath = mNativeData.rootDir;
		fileDirectoryPath.append( 1, PCL_ENV_DEFAULT_PATH_DELIMITER );
		fileDirectoryPath.append( pAssetPathInfo.directory );

		pAssetPathInfo.directory = std::move( fileDirectoryPath );

		if( mFileManager->CheckDirectoryExists( pAssetPathInfo.directory ) )
		{
			auto combinedFilePath = Platform::_commonResolveFileName( *mFileManager, pAssetPathInfo, pFlags );

			if( mFileManager->CheckFileExists( combinedFilePath ) )
			{
				auto fileHandle = mFileManager->OpenFile( combinedFilePath, EFileOpenMode::ReadOnly );

				asset = CreateSysObject<FileAsset>( GetHandle<FileAssetLoader>() );
				asset->SetName( std::move( pAssetPathInfo.file_name ) );
				asset->mNativeData.fileHandle = fileHandle;
				asset->mNativeData.filePath = std::move( combinedFilePath );
			}
		}

		return asset;
	}

	AssetDirectoryHandle FileAssetLoader::_NativeOpenDirectory( std::string pDirectoryName )
	{
		auto combinedDirPath = mNativeData.rootDir;
		combinedDirPath.append( 1, PCL_ENV_DEFAULT_PATH_DELIMITER );
		combinedDirPath.append( pDirectoryName );

		if( !mFileManager->CheckDirectoryExists( combinedDirPath ) )
		{
			return nullptr;
		}

		auto assetDirectory = CreateSysObject<FileAssetDirectory>( GetHandle<FileAssetLoader>() );
		assetDirectory->SetDirName( std::move( pDirectoryName ) );
		assetDirectory->mNativeData.combinedDirPath = std::move( combinedDirPath );

		return assetDirectory;
	}

	bool FileAssetLoader::_NativeCheckDirectoryExists( const std::string & pDirectoryName ) const
	{
		auto combinedDirPath = mNativeData.rootDir;
		combinedDirPath.append( 1, PCL_ENV_DEFAULT_PATH_DELIMITER );
		combinedDirPath.append( pDirectoryName );

		return mFileManager->CheckDirectoryExists( combinedDirPath );
	}


	FileAssetDirectory::FileAssetDirectory( FileAssetLoaderHandle pAssetLoader )
	: NativeObject( pAssetLoader )
	, mFileManager( pAssetLoader->mFileManager )
	{}

	FileAssetDirectory::~FileAssetDirectory() noexcept = default;

	void FileAssetDirectory::_NativeRefreshAssetList()
	{
		auto assetNameList = mFileManager->EnumDirectoryFiles( mNativeData.combinedDirPath );
		SetAssetList( std::move( assetNameList ) );
	}

	AssetHandle FileAssetDirectory::_NativeOpenAsset( std::string pAssetName, cppx::bitmask<EAssetOpenFlags> pFlags )
	{
		TSysHandle<FileAsset> asset = nullptr;

		cppx::file_path_info assetPathInfo;
		assetPathInfo.directory = mNativeData.combinedDirPath;
		assetPathInfo.file_name = std::move( pAssetName );

		if( mFileManager->CheckDirectoryExists( mNativeData.combinedDirPath ) )
		{
			auto combinedFilePath = Platform::_commonResolveFileName( *mFileManager, assetPathInfo, pFlags );

			if( mFileManager->CheckFileExists( combinedFilePath ) )
			{
				auto fileHandle = mFileManager->OpenFile( combinedFilePath, EFileOpenMode::ReadOnly );

				asset = CreateSysObject<FileAsset>( GetHandle<FileAssetDirectory>() );
				asset->SetName( std::move( assetPathInfo.file_name ) );
				asset->mNativeData.fileHandle = fileHandle;
				asset->mNativeData.filePath = std::move( combinedFilePath );
			}
		}

		return asset;
	}

	bool FileAssetDirectory::_NativeCheckAssetExists( const std::string & pAssetName ) const
	{
		auto combinedFilePath = mNativeData.combinedDirPath;
		combinedFilePath.append( pAssetName );
		return mFileManager->CheckFileExists( combinedFilePath );
	}


	FileAsset::FileAsset( AssetLoaderHandle pAssetLoader )
	: NativeObject( std::move( pAssetLoader ) )
	{}

	FileAsset::FileAsset( AssetDirectoryHandle pAssetDirectory )
	: NativeObject( std::move( pAssetDirectory ) )
	{}

	FileAsset::~FileAsset() noexcept = default;

	file_size_t FileAsset::_NativeReadData( void * pTargetBuffer, file_size_t pReadSize )
	{
		return mNativeData.fileHandle->Read( pTargetBuffer, pReadSize );
	}

	file_offset_t FileAsset::_NativeSetReadPointer( file_offset_t pOffset, EFilePointerRefPos pRefPos )
	{
		return mNativeData.fileHandle->SetFilePointer( pOffset, pRefPos );
	}

	file_size_t FileAsset::_NativeGetSize() const
	{
		return mNativeData.fileHandle->GetSize();
	}


	namespace Platform
	{

		AssetLoaderHandle CreateFileAssetLoaderExplicit(
				SysContextHandle pSysContext,
				FileManagerHandle pFileManager,
				const std::string & pAbsoluteAssetDirectory )
		{
			if( !pFileManager )
			{
				pFileManager = pSysContext->CreateFileManager();
			}

			auto assetDirectory = cppx::fs_normalize_path( pAbsoluteAssetDirectory );
			auto assetLoader = CreateSysObject<FileAssetLoader>( pFileManager );
			assetLoader->SetRootDir( std::move( assetDirectory ) );

			return assetLoader;
		}

		AssetLoaderHandle CreateFileAssetLoaderResolve(
				SysContextHandle pSysContext,
				FileManagerHandle pFileManager,
				const std::string & pRelativeAssetDirectory )
		{
			if( !pFileManager )
			{
				pFileManager = pSysContext->CreateFileManager();
			}

			auto assetDirectory = pSysContext->QueryCurrentProcessWorkingDirectory();
			// auto assetDirectory = pSysContext->queryCurrentProcessExecutableDirectory();
			assetDirectory.append( 1, PCL_ENV_DEFAULT_PATH_DELIMITER );
			assetDirectory.append( pRelativeAssetDirectory );
			assetDirectory = cppx::fs_normalize_path( assetDirectory );

			auto assetLoader = CreateSysObject<FileAssetLoader>( pFileManager );
			assetLoader->SetRootDir( std::move( assetDirectory ) );

			return assetLoader;
		}

		AssetLoaderHandle CreateFileAssetLoader( SysContextHandle pSysContext, const AssetLoaderCreateInfoNativeParams & pCreateParams )
		{
			const auto & fileManager = pCreateParams.fileManager;

			if( !pCreateParams.absoluteAssetRootDir.empty() )
			{
				return CreateFileAssetLoaderExplicit( std::move( pSysContext ), fileManager,
				                                      pCreateParams.absoluteAssetRootDir );
			}
			else
			{
				return CreateFileAssetLoaderResolve( std::move( pSysContext ), fileManager, pCreateParams.relativeAssetRootDir );
			}
		}


		std::string _commonResolveFileName( FileManager & pFileManager,
		                                    cppx::file_path_info & pAssetPathInfo,
		                                    cppx::bitmask<EAssetOpenFlags> pFlags )
		{
			auto combinedFilePath = pAssetPathInfo.directory;
			combinedFilePath.append( 1, PCL_ENV_DEFAULT_PATH_DELIMITER );
			combinedFilePath.append( pAssetPathInfo.file_name );

			if( !pFileManager.CheckFileExists( combinedFilePath ) )
			{
				combinedFilePath.clear();

				if( pFlags.is_set( eAssetOpenFlagNoExtensionBit ) )
				{
					auto fileNameList = pFileManager.EnumDirectoryFiles( pAssetPathInfo.directory );
					if( !fileNameList.empty() )
					{
						for( auto & fileName : fileNameList )
						{
							auto assetNamePos = fileName.find( pAssetPathInfo.file_name );
							if( assetNamePos == 0 )
							{
								pAssetPathInfo.file_name = std::move( fileName );
								combinedFilePath = pAssetPathInfo.directory;
								combinedFilePath.append( 1, PCL_ENV_DEFAULT_PATH_DELIMITER );
								combinedFilePath.append( pAssetPathInfo.file_name );

								break;
							}
						}
					}
				}
			}

			return combinedFilePath;
		}

	}

}
