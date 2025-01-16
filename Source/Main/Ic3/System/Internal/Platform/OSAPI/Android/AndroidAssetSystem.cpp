
#include <Ic3/System/AssetSystemNative.h>
#include <Ic3/System/FileCommon.h>

#if( PCL_TARGET_SYSAPI == PCL_TARGET_SYSAPI_ANDROID )
namespace Ic3::System
{

	namespace Platform
	{

		int _androidTranslateFilePointerRefPos( EFilePointerRefPos pFileRefPos );

		AAsset * _androidResolveAsset(
				AAssetManager * pAAssetManager,
				AAssetDir * pAAssetDir,
				cppx::file_path_info & pAssetPathInfo,
				cppx::bitmask<EAssetOpenFlags> pFlags );

	}


	AndroidAssetLoader::AndroidAssetLoader( SysContextHandle pSysContext )
	: AndroidNativeObject( std::move( pSysContext ) )
	{
		_InitializeAndroidAssetLoaderData();
	}

	AndroidAssetLoader::~AndroidAssetLoader() noexcept
	{
		_ReleaseAndroidAssetLoaderData();
	}

	void AndroidAssetLoader::_InitializeAndroidAssetLoaderData()
	{
		auto & aSessionData = Platform::AndroidGetASessionData( *this );
		mNativeData.mAndrAssetManager = aSessionData.aCommonAppState->activity->assetManager;
	}

	void AndroidAssetLoader::_ReleaseAndroidAssetLoaderData()
	{
		mNativeData.mAndrAssetManager = nullptr;
	}

	AssetHandle AndroidAssetLoader::_NativeOpenSubAsset( cppx::file_path_info pAssetPathInfo, cppx::bitmask<EAssetOpenFlags> pFlags )
	{
		TSysHandle<AndroidAsset> asset = nullptr;

		auto * aAssetManager = mNativeData.mAndrAssetManager;

		if( auto * aAsset = Platform::_androidResolveAsset( aAssetManager, nullptr, pAssetPathInfo, pFlags ) )
		{
			asset = CreateSysObject<AndroidAsset>( GetHandle<AndroidAssetLoader>() );
			asset->SetName( std::move( pAssetPathInfo.mFileName ) );
			asset->mNativeData.mAndrAsset = aAsset;
			asset->mNativeData.mAndrAssetManager = mNativeData.mAndrAssetManager;
		}

		return asset;
	}

	AssetDirectoryHandle AndroidAssetLoader::_NativeOpenDirectory( std::string pDirectoryName )
	{
		std::string assetDirRefName = "assets";
		assetDirRefName.append( 1, PCL_ENV_DEFAULT_PATH_DELIMITER );
		assetDirRefName.append( pDirectoryName );

		TSysHandle<AndroidAssetDirectory> assetDirectory = nullptr;

		if( auto * aAssetDir = AAssetManager_openDir( mNativeData.mAndrAssetManager, pDirectoryName.c_str() ) )
		{
			assetDirectory = CreateSysObject<AndroidAssetDirectory>( GetHandle<AndroidAssetLoader>() );
			assetDirectory->SetDirName( std::move( pDirectoryName ) );
			assetDirectory->mNativeData.mAndrAssetDir = aAssetDir;
		}

		return assetDirectory;
	}

	bool AndroidAssetLoader::_NativeCheckDirectoryExists( const std::string & pDirectoryName ) const
	{
		std::string assetDirRefName = "assets";
		assetDirRefName.append( 1, PCL_ENV_DEFAULT_PATH_DELIMITER );
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
		_InitializeAndroidAssetDirectoryData( pAssetLoader->mNativeData.mAndrAssetManager );
	}

	AndroidAssetDirectory::~AndroidAssetDirectory() noexcept
	{
		_ReleaseAndroidAssetDirectoryData();
	}

	void AndroidAssetDirectory::_InitializeAndroidAssetDirectoryData( AAssetManager * pAssetManager )
	{
		mNativeData.mAndrAssetManager = pAssetManager;
	}

	void AndroidAssetDirectory::_ReleaseAndroidAssetDirectoryData()
	{
		if( mNativeData.mAndrAssetDir )
		{
			AAssetDir_close( mNativeData.mAndrAssetDir );
			mNativeData.mAndrAssetDir = nullptr;
		}
	}

	void AndroidAssetDirectory::_NativeRefreshAssetList()
	{
		FileNameList assetNameList;
		while( auto * aAssetName = AAssetDir_getNextFileName( mNativeData.mAndrAssetDir ) )
		{
			assetNameList.push_back( std::string( aAssetName ) );
		}

		AAssetDir_rewind( mNativeData.mAndrAssetDir );

		SetAssetList( std::move( assetNameList ) );
	}

	AssetHandle AndroidAssetDirectory::_NativeOpenAsset( std::string pAssetName, cppx::bitmask<EAssetOpenFlags> pFlags )
	{
		TSysHandle<AndroidAsset> asset = nullptr;

		cppx::file_path_info assetPathInfo;
		assetPathInfo.mDirectory = GetDirName();
		assetPathInfo.mFileName = std::move( pAssetName );

		auto * aAssetManager = mNativeData.mAndrAssetManager;
		auto * aAssetDir = mNativeData.mAndrAssetDir;

		if( auto * aAsset = Platform::_androidResolveAsset( aAssetManager, aAssetDir, assetPathInfo, pFlags ) )
		{
			asset = CreateSysObject<AndroidAsset>( GetHandle<AndroidAssetLoader>() );
			asset->SetName( std::move( assetPathInfo.mFileName ) );
			asset->mNativeData.mAndrAsset = aAsset;
			asset->mNativeData.mAndrAssetManager = mNativeData.mAndrAssetManager;
		}

		return asset;
	}

	bool AndroidAssetDirectory::_NativeCheckAssetExists( const std::string & pAssetName ) const
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
		_ReleaseAndroidAssetData();
	}

	void AndroidAsset::_ReleaseAndroidAssetData()
	{
		if( mNativeData.mAndrAsset )
		{
			AAsset_close( mNativeData.mAndrAsset );
			mNativeData.mAndrAsset = nullptr;
		}
	}

	file_size_t AndroidAsset::_NativeReadData( void * pTargetBuffer, file_size_t pReadSize )
	{
		int readResult = AAsset_read( mNativeData.mAndrAsset, pTargetBuffer, pReadSize );
		return numeric_cast<file_size_t>( readResult );
	}

	file_offset_t AndroidAsset::_NativeSetReadPointer( file_offset_t pOffset, EFilePointerRefPos pRefPos )
	{
		auto seekOrigin = Platform::_androidTranslateFilePointerRefPos( pRefPos );
		auto seekResult = AAsset_seek64( mNativeData.mAndrAsset, pOffset, seekOrigin );
		return numeric_cast<file_offset_t>( seekResult );
	}

	file_size_t AndroidAsset::_NativeGetSize() const
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

		AAsset * _androidResolveAsset(
				AAssetManager * pAAssetManager,
				AAssetDir * pAAssetDir,
				cppx::file_path_info & pAssetPathInfo,
				cppx::bitmask<EAssetOpenFlags> pFlags )
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

				if( !aAsset && pFlags.is_set( eAssetOpenFlagNoExtensionBit ) )
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
#endif // PCL_TARGET_SYSAPI_ANDROID
