
#include <Ic3/System/AssetSystemNative.h>
#include <Ic3/System/FileCommon.h>

#if( IC3_PCL_TARGET_SYSAPI == IC3_PCL_TARGET_SYSAPI_ANDROID )
namespace Ic3::System
{

	namespace Platform
	{

		int _androidTranslateFilePointerRefPos( EFilePointerRefPos pFileRefPos );

		AAsset * _androidResolveAsset( AAssetManager * pAAssetManager,
		                               AAssetDir * pAAssetDir,
		                               Cppx::FilePathInfo & pAssetPathInfo,
		                               TBitmask<EAssetOpenFlags> pFlags );

	}


	AndroidAssetLoader::AndroidAssetLoader( SysContextHandle pSysContext )
	: AndroidNativeObject( std::move( pSysContext ) )
	{
		_initializeAndroidAssetLoaderData();
	}

	AndroidAssetLoader::~AndroidAssetLoader() noexcept
	{
		_releaseAndroidAssetLoaderData();
	}

	void AndroidAssetLoader::_initializeAndroidAssetLoaderData()
	{
		auto & aSessionData = Platform::androidGetASessionData( *this );
		mNativeData.mAndrAssetManager = aSessionData.aCommonAppState->activity->assetManager;
	}

	void AndroidAssetLoader::_releaseAndroidAssetLoaderData()
	{
		mNativeData.mAndrAssetManager = nullptr;
	}

	AssetHandle AndroidAssetLoader::_nativeOpenSubAsset( Cppx::FilePathInfo pAssetPathInfo, TBitmask<EAssetOpenFlags> pFlags )
	{
		TSysHandle<AndroidAsset> asset = nullptr;

		auto * aAssetManager = mNativeData.mAndrAssetManager;

		if( auto * aAsset = Platform::_androidResolveAsset( aAssetManager, nullptr, pAssetPathInfo, pFlags ) )
		{
			asset = createSysObject<AndroidAsset>( getHandle<AndroidAssetLoader>() );
			asset->setName( std::move( pAssetPathInfo.mFileName ) );
			asset->mNativeData.mAndrAsset = aAsset;
			asset->mNativeData.mAndrAssetManager = mNativeData.mAndrAssetManager;
		}

		return asset;
	}

	AssetDirectoryHandle AndroidAssetLoader::_nativeOpenDirectory( std::string pDirectoryName )
	{
		std::string assetDirRefName = "assets";
		assetDirRefName.append( 1, IC3_PCL_ENV_DEFAULT_PATH_DELIMITER );
		assetDirRefName.append( pDirectoryName );

		TSysHandle<AndroidAssetDirectory> assetDirectory = nullptr;

		if( auto * aAssetDir = AAssetManager_openDir( mNativeData.mAndrAssetManager, pDirectoryName.c_str() ) )
		{
			assetDirectory = createSysObject<AndroidAssetDirectory>( getHandle<AndroidAssetLoader>() );
			assetDirectory->setDirName( std::move( pDirectoryName ) );
			assetDirectory->mNativeData.mAndrAssetDir = aAssetDir;
		}

		return assetDirectory;
	}

	bool AndroidAssetLoader::_nativeCheckDirectoryExists( const std::string & pDirectoryName ) const
	{
		std::string assetDirRefName = "assets";
		assetDirRefName.append( 1, IC3_PCL_ENV_DEFAULT_PATH_DELIMITER );
		assetDirRefName.append( pDirectoryName );

		auto * aAssetDir = AAssetManager_openDir( mNativeData.mAndrAssetManager, assetDirRefName.c_str() );
		if( aAssetDir )
		{
			AAssetDir_close( aAssetDir );
			return true;
		}

		return false;
	}


	AndroidAssetDirectory::AndroidAssetDirectory( AndroidAssetLoaderHandle pAssetLoader )
	: AndroidNativeObject( pAssetLoader )
	{
		_initializeAndroidAssetDirectoryData( pAssetLoader->mNativeData.mAndrAssetManager );
	}

	AndroidAssetDirectory::~AndroidAssetDirectory() noexcept
	{
		_releaseAndroidAssetDirectoryData();
	}

	void AndroidAssetDirectory::_initializeAndroidAssetDirectoryData( AAssetManager * pAssetManager )
	{
		mNativeData.mAndrAssetManager = pAssetManager;
	}

	void AndroidAssetDirectory::_releaseAndroidAssetDirectoryData()
	{
		if( mNativeData.mAndrAssetDir )
		{
			AAssetDir_close( mNativeData.mAndrAssetDir );
			mNativeData.mAndrAssetDir = nullptr;
		}
	}

	void AndroidAssetDirectory::_nativeRefreshAssetList()
	{
		FileNameList assetNameList;
		while( auto * aAssetName = AAssetDir_getNextFileName( mNativeData.mAndrAssetDir ) )
		{
			assetNameList.push_back( std::string( aAssetName ) );
		}

		AAssetDir_rewind( mNativeData.mAndrAssetDir );

		setAssetList( std::move( assetNameList ) );
	}

	AssetHandle AndroidAssetDirectory::_nativeOpenAsset( std::string pAssetName, TBitmask<EAssetOpenFlags> pFlags )
	{
		TSysHandle<AndroidAsset> asset = nullptr;

		Cppx::FilePathInfo assetPathInfo;
		assetPathInfo.mDirectory = getDirName();
		assetPathInfo.mFileName = std::move( pAssetName );

		auto * aAssetManager = mNativeData.mAndrAssetManager;
		auto * aAssetDir = mNativeData.mAndrAssetDir;

		if( auto * aAsset = Platform::_androidResolveAsset( aAssetManager, aAssetDir, assetPathInfo, pFlags ) )
		{
			asset = createSysObject<AndroidAsset>( getHandle<AndroidAssetLoader>() );
			asset->setName( std::move( assetPathInfo.mFileName ) );
			asset->mNativeData.mAndrAsset = aAsset;
			asset->mNativeData.mAndrAssetManager = mNativeData.mAndrAssetManager;
		}

		return asset;
	}

	bool AndroidAssetDirectory::_nativeCheckAssetExists( const std::string & pAssetName ) const
	{
		if( auto * aAsset = AAssetManager_open( mNativeData.mAndrAssetManager, pAssetName.c_str(), AASSET_MODE_BUFFER ) )
		{
			AAsset_close( aAsset );
			return true;
		}
		return false;
	}


	AndroidAsset::AndroidAsset( AndroidAssetLoaderHandle pAssetLoader )
	: AndroidNativeObject( std::move( pAssetLoader ) )
	{}

	AndroidAsset::AndroidAsset( AndroidAssetDirectoryHandle pAssetDirectory )
	: AndroidNativeObject( std::move( pAssetDirectory ) )
	{}

	AndroidAsset::~AndroidAsset() noexcept
	{
		_releaseAndroidAssetData();
	}

	void AndroidAsset::_releaseAndroidAssetData()
	{
		if( mNativeData.mAndrAsset )
		{
			AAsset_close( mNativeData.mAndrAsset );
			mNativeData.mAndrAsset = nullptr;
		}
	}

	file_size_t AndroidAsset::_nativeReadData( void * pTargetBuffer, file_size_t pReadSize )
	{
		int readResult = AAsset_read( mNativeData.mAndrAsset, pTargetBuffer, pReadSize );
		return numeric_cast<file_size_t>( readResult );
	}

	file_offset_t AndroidAsset::_nativeSetReadPointer( file_offset_t pOffset, EFilePointerRefPos pRefPos )
	{
		auto seekOrigin = Platform::_androidTranslateFilePointerRefPos( pRefPos );
		auto seekResult = AAsset_seek64( mNativeData.mAndrAsset, pOffset, seekOrigin );
		return numeric_cast<file_offset_t>( seekResult );
	}

	file_size_t AndroidAsset::_nativeGetSize() const
	{
		auto assetSize = AAsset_getLength64( mNativeData.mAndrAsset );
		return numeric_cast<file_offset_t>( assetSize );
	}


	namespace Platform
	{

		int _androidTranslateFilePointerRefPos( EFilePointerRefPos pFileRefPos )
		{
			int seekOrigin = 0;

			switch( pFileRefPos )
			{
				case EFilePointerRefPos::FileBeg:
				{
					seekOrigin = SEEK_SET;
					break;
				}
				case EFilePointerRefPos::FileEnd:
				{
					seekOrigin = SEEK_END;
					break;
				}
				case EFilePointerRefPos::PtrCurrent:
				{
					seekOrigin = SEEK_CUR;
					break;
				}
			}

			return seekOrigin;
		}

		AAsset * _androidResolveAsset( AAssetManager * pAAssetManager,
		                               AAssetDir * pAAssetDir,
		                               Cppx::FilePathInfo & pAssetPathInfo,
		                               TBitmask<EAssetOpenFlags> pFlags )
		{
			AAsset * aAsset = nullptr;
			bool aAssetDirOwnershipFlag = false;

			if( !pAAssetDir )
			{
				pAAssetDir = AAssetManager_openDir( pAAssetManager, pAssetPathInfo.mDirectory.c_str() );
				aAssetDirOwnershipFlag = true;
			}

			if( pAAssetDir )
			{
				aAsset = AAssetManager_open( pAAssetManager, pAssetPathInfo.mFileName.c_str(), AASSET_MODE_RANDOM );

				if( !aAsset && pFlags.isSet( eAssetOpenFlagNoExtensionBit ) )
				{
					while( auto * assetFilename = AAssetDir_getNextFileName( pAAssetDir ) )
					{
						auto assetNameStr = std::string( assetFilename );
						auto assetNamePos = assetNameStr.find( pAssetPathInfo.mFileName );

						if( assetNamePos == 0 )
						{
							aAsset = AAssetManager_open( pAAssetManager, assetFilename, AASSET_MODE_RANDOM );
							pAssetPathInfo.mFileName = std::move( assetNameStr );
							break;
						}
					}

					AAssetDir_rewind( pAAssetDir );
				}

				if( aAssetDirOwnershipFlag )
				{
					AAssetDir_close( pAAssetDir );
					pAAssetDir = nullptr;
					aAssetDirOwnershipFlag = false;
				}
			}

			return aAsset;
		}

	}

} // namespace Ic3::System
#endif // IC3_PCL_TARGET_SYSAPI_ANDROID
