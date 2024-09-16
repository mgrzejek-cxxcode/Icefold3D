
#ifndef __IC3_SYSTEM_PLATFORM_OSAPI_ANDROID_ASSET_SYSTEM_H__
#define __IC3_SYSTEM_PLATFORM_OSAPI_ANDROID_ASSET_SYSTEM_H__

#include "AndroidCommon.h"
#include <Ic3/System/AssetSystem.h>
#include <android/asset_manager.h>

namespace Ic3::System
{

	ic3DeclareClassHandle( AndroidAssetLoader );
	ic3DeclareClassHandle( AndroidAssetDirectory );
	ic3DeclareClassHandle( AndroidAsset );

	namespace Platform
	{

		struct AndroidAssetNativeData : public AndroidNativeDataCommon
		{
			AAssetManager * mAndrAssetManager = nullptr;
			AAsset * mAndrAsset = nullptr;
		};

		struct AndroidAssetDirectoryNativeData : public AndroidNativeDataCommon
		{
			AAssetManager * mAndrAssetManager = nullptr;
			AAssetDir * mAndrAssetDir = nullptr;
		};

		struct AndroidAssetLoaderNativeData : public AndroidNativeDataCommon
		{
			AAssetManager * mAndrAssetManager = nullptr;
		};

		struct AssetLoaderCreateInfoNativeParams
		{
		};

	}

	class AndroidAssetLoader : public AndroidNativeObject<AssetLoader, Platform::AndroidAssetLoaderNativeData>
	{
	public:
		explicit AndroidAssetLoader( SysContextHandle pSysContext );
		virtual ~AndroidAssetLoader() noexcept;

	private:
		void _initializeAndroidAssetLoaderData();
		void _releaseAndroidAssetLoaderData();

		/// @copybrief AssetLoader::_nativeOpenSubAsset
		virtual AssetHandle _nativeOpenSubAsset( Cppx::FilePathInfo pAssetPathInfo,
		                                         TBitmask<EAssetOpenFlags> pFlags ) override final;

		/// @copybrief AssetLoader::_nativeOpenDirectory
		virtual AssetDirectoryHandle _nativeOpenDirectory( std::string pDirectoryName ) override final;

		/// @copybrief AssetLoader::_nativeCheckDirectoryExists
		virtual bool _nativeCheckDirectoryExists( const std::string & pDirectoryName ) const override final;
	};

	class AndroidAssetDirectory : public AndroidNativeObject<AssetDirectory, Platform::AndroidAssetDirectoryNativeData>
	{
		friend class AndroidAssetLoader;

	public:
		FileManagerHandle const mFileManager;

	public:
		explicit AndroidAssetDirectory( AndroidAssetLoaderHandle pAssetLoader );
		virtual ~AndroidAssetDirectory() noexcept;

		using AssetDirectory::addAsset;
		using AssetDirectory::setDirName;

	private:
		void _initializeAndroidAssetDirectoryData( AAssetManager * pAssetManager );
		void _releaseAndroidAssetDirectoryData();

		/// @copybrief AssetDirectory::_nativeRefreshAssetList
		virtual void _nativeRefreshAssetList() override final;

		/// @copybrief AssetDirectory::_nativeOpenAsset
		virtual AssetHandle _nativeOpenAsset( std::string pAssetName, TBitmask<EAssetOpenFlags> pFlags ) override final;

		/// @copybrief AssetDirectory::_nativeCheckAssetExists
		virtual bool _nativeCheckAssetExists( const std::string & pAssetName ) const override final;
	};

	class AndroidAsset : public AndroidNativeObject<Asset, Platform::AndroidAssetNativeData>
	{
		friend class AndroidAssetLoader;

	public:
		explicit AndroidAsset( AndroidAssetLoaderHandle pAssetLoader );
		explicit AndroidAsset( AndroidAssetDirectoryHandle pAssetDirectory );
		virtual ~AndroidAsset() noexcept;

		using Asset::setName;

	private:
		void _releaseAndroidAssetData();

		/// @copybrief Asset::_nativeReadData
		virtual file_size_t _nativeReadData( void * pTargetBuffer, file_size_t pReadSize ) override final;

		/// @copybrief Asset::_nativeSetReadPointer
		virtual file_offset_t _nativeSetReadPointer( file_offset_t pOffset, EFilePointerRefPos pRefPos ) override final;

		/// @copybrief Asset::_nativeGetSize
		virtual file_size_t _nativeGetSize() const override final;
	};

} // namespace Ic3::System

#endif // __IC3_SYSTEM_PLATFORM_OSAPI_ANDROID_ASSET_SYSTEM_H__
