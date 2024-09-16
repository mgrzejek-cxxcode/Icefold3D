
#include "FileBasedAssetSystem.h"
#include <Ic3/System/FileManager.h>
#include <Ic3/System/SysContext.h>
#include <Ic3/System/AssetSystem.h>

namespace Ic3::System
{

	namespace Platform
	{

		std::string _commonResolveFileName( FileManager & pFileManager,
		                                    Cppx::FilePathInfo & pAssetPathInfo,
		                                    TBitmask<EAssetOpenFlags> pFlags );

	}


	FileAssetLoader::FileAssetLoader( FileManagerHandle pFileManager )
	: NativeObject( pFileManager->mSysContext )
	, mFileManager( std::move( pFileManager ) ) 
	{}

	FileAssetLoader::~FileAssetLoader() noexcept = default;

	void FileAssetLoader::setRootDir( std::string pRootDir )
	{
		mNativeData.mRootDir = std::move( pRootDir );
	}

	AssetHandle FileAssetLoader::_nativeOpenSubAsset( Cppx::FilePathInfo pAssetPathInfo, TBitmask<EAssetOpenFlags> pFlags )
	{
		TSysHandle<FileAsset> asset = nullptr;

		auto fileDirectoryPath = mNativeData.mRootDir;
		fileDirectoryPath.append( 1, IC3_PCL_ENV_DEFAULT_PATH_DELIMITER );
		fileDirectoryPath.append( pAssetPathInfo.mDirectory );

		pAssetPathInfo.mDirectory = std::move( fileDirectoryPath );

		if( mFileManager->checkDirectoryExists( pAssetPathInfo.mDirectory ) )
		{
			auto combinedFilePath = Platform::_commonResolveFileName( *mFileManager, pAssetPathInfo, pFlags );

			if( mFileManager->checkFileExists( combinedFilePath ) )
			{
				auto fileHandle = mFileManager->openFile( combinedFilePath, EFileOpenMode::ReadOnly );

				asset = createSysObject<FileAsset>( getHandle<FileAssetLoader>() );
				asset->setName( std::move( pAssetPathInfo.mFileName ) );
				asset->mNativeData.mFileHandle = fileHandle;
				asset->mNativeData.mFilePath = std::move( combinedFilePath );
			}
		}

		return asset;
	}

	AssetDirectoryHandle FileAssetLoader::_nativeOpenDirectory( std::string pDirectoryName )
	{
		auto combinedDirPath = mNativeData.mRootDir;
		combinedDirPath.append( 1, IC3_PCL_ENV_DEFAULT_PATH_DELIMITER );
		combinedDirPath.append( pDirectoryName );

		if( !mFileManager->checkDirectoryExists( combinedDirPath ) )
		{
			return nullptr;
		}

		auto assetDirectory = createSysObject<FileAssetDirectory>( getHandle<FileAssetLoader>() );
		assetDirectory->setDirName( std::move( pDirectoryName ) );
		assetDirectory->mNativeData.mCombinedDirPath = std::move( combinedDirPath );

		return assetDirectory;
	}

	bool FileAssetLoader::_nativeCheckDirectoryExists( const std::string & pDirectoryName ) const
	{
		auto combinedDirPath = mNativeData.mRootDir;
		combinedDirPath.append( 1, IC3_PCL_ENV_DEFAULT_PATH_DELIMITER );
		combinedDirPath.append( pDirectoryName );

		return mFileManager->checkDirectoryExists( combinedDirPath );
	}


	FileAssetDirectory::FileAssetDirectory( FileAssetLoaderHandle pAssetLoader )
	: NativeObject( pAssetLoader )
	, mFileManager( pAssetLoader->mFileManager )
	{}

	FileAssetDirectory::~FileAssetDirectory() noexcept = default;

	void FileAssetDirectory::_nativeRefreshAssetList()
	{
		auto assetNameList = mFileManager->enumDirectoryFiles( mNativeData.mCombinedDirPath );
		setAssetList( std::move( assetNameList ) );
	}

	AssetHandle FileAssetDirectory::_nativeOpenAsset( std::string pAssetName, TBitmask<EAssetOpenFlags> pFlags )
	{
		TSysHandle<FileAsset> asset = nullptr;

		Cppx::FilePathInfo assetPathInfo;
		assetPathInfo.mDirectory = mNativeData.mCombinedDirPath;
		assetPathInfo.mFileName = std::move( pAssetName );

		if( mFileManager->checkDirectoryExists( mNativeData.mCombinedDirPath ) )
		{
			auto combinedFilePath = Platform::_commonResolveFileName( *mFileManager, assetPathInfo, pFlags );

			if( mFileManager->checkFileExists( combinedFilePath ) )
			{
				auto fileHandle = mFileManager->openFile( combinedFilePath, EFileOpenMode::ReadOnly );

				asset = createSysObject<FileAsset>( getHandle<FileAssetDirectory>() );
				asset->setName( std::move( assetPathInfo.mFileName ) );
				asset->mNativeData.mFileHandle = fileHandle;
				asset->mNativeData.mFilePath = std::move( combinedFilePath );
			}
		}

		return asset;
	}

	bool FileAssetDirectory::_nativeCheckAssetExists( const std::string & pAssetName ) const
	{
		auto combinedFilePath = mNativeData.mCombinedDirPath;
		combinedFilePath.append( pAssetName );
		return mFileManager->checkFileExists( combinedFilePath );
	}


	FileAsset::FileAsset( AssetLoaderHandle pAssetLoader )
	: NativeObject( std::move( pAssetLoader ) )
	{}

	FileAsset::FileAsset( AssetDirectoryHandle pAssetDirectory )
	: NativeObject( std::move( pAssetDirectory ) )
	{}

	FileAsset::~FileAsset() noexcept = default;

	file_size_t FileAsset::_nativeReadData( void * pTargetBuffer, file_size_t pReadSize )
	{
		return mNativeData.mFileHandle->read( pTargetBuffer, pReadSize );
	}

	file_offset_t FileAsset::_nativeSetReadPointer( file_offset_t pOffset, EFilePointerRefPos pRefPos )
	{
		return mNativeData.mFileHandle->setFilePointer( pOffset, pRefPos );
	}

	file_size_t FileAsset::_nativeGetSize() const
	{
		return mNativeData.mFileHandle->getSize();
	}


	namespace Platform
	{

		AssetLoaderHandle createFileAssetLoaderExplicit(
				SysContextHandle pSysContext,
				FileManagerHandle pFileManager,
				const std::string & pAbsoluteAssetDirectory )
		{
			if( !pFileManager )
			{
				pFileManager = pSysContext->createFileManager();
			}

			auto assetDirectory = Cppx::fsNormalizePath( pAbsoluteAssetDirectory );
			auto assetLoader = createSysObject<FileAssetLoader>( pFileManager );
			assetLoader->setRootDir( std::move( assetDirectory ) );

			return assetLoader;
		}

		AssetLoaderHandle createFileAssetLoaderResolve(
				SysContextHandle pSysContext,
				FileManagerHandle pFileManager,
				const std::string & pRelativeAssetDirectory )
		{
			if( !pFileManager )
			{
				pFileManager = pSysContext->createFileManager();
			}

			auto assetDirectory = pSysContext->queryCurrentProcessWorkingDirectory();
			// auto assetDirectory = pSysContext->queryCurrentProcessExecutableDirectory();
			assetDirectory.append( 1, IC3_PCL_ENV_DEFAULT_PATH_DELIMITER );
			assetDirectory.append( pRelativeAssetDirectory );
			assetDirectory = Cppx::fsNormalizePath( assetDirectory );

			auto assetLoader = createSysObject<FileAssetLoader>( pFileManager );
			assetLoader->setRootDir( std::move( assetDirectory ) );

			return assetLoader;
		}

		AssetLoaderHandle createFileAssetLoader( SysContextHandle pSysContext, const AssetLoaderCreateInfoNativeParams & pCreateParams )
		{
			const auto & fileManager = pCreateParams.mFileManager;

			if( !pCreateParams.mAbsoluteAssetRootDir.empty() )
			{
				return createFileAssetLoaderExplicit( std::move( pSysContext ), fileManager, pCreateParams.mAbsoluteAssetRootDir );
			}
			else
			{
				return createFileAssetLoaderResolve( std::move( pSysContext ), fileManager, pCreateParams.mRelativeAssetRootDir );
			}
		}


		std::string _commonResolveFileName( FileManager & pFileManager,
		                                    Cppx::FilePathInfo & pAssetPathInfo,
		                                    TBitmask<EAssetOpenFlags> pFlags )
		{
			auto combinedFilePath = pAssetPathInfo.mDirectory;
			combinedFilePath.append( 1, IC3_PCL_ENV_DEFAULT_PATH_DELIMITER );
			combinedFilePath.append( pAssetPathInfo.mFileName );

			if( !pFileManager.checkFileExists( combinedFilePath ) )
			{
				combinedFilePath.clear();

				if( pFlags.isSet( eAssetOpenFlagNoExtensionBit ) )
				{
					auto fileNameList = pFileManager.enumDirectoryFiles( pAssetPathInfo.mDirectory );
					if( !fileNameList.empty() )
					{
						for( auto & fileName : fileNameList )
						{
							auto assetNamePos = fileName.find( pAssetPathInfo.mFileName );
							if( assetNamePos == 0 )
							{
								pAssetPathInfo.mFileName = std::move( fileName );
								combinedFilePath = pAssetPathInfo.mDirectory;
								combinedFilePath.append( 1, IC3_PCL_ENV_DEFAULT_PATH_DELIMITER );
								combinedFilePath.append( pAssetPathInfo.mFileName );

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
