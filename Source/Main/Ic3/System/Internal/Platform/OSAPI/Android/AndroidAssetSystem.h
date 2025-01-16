
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
		void _InitializeAndroidAssetLoaderData();
		void _ReleaseAndroidAssetLoaderData();

		/// @copybrief AssetLoader::_NativeOpenSubAsset
		virtual AssetHandle _NativeOpenSubAsset( cppx::file_path_info pAssetPathInfo,
		                                         cppx::bitmask<EAssetOpenFlags> pFlags ) override final;

		/// @copybrief AssetLoader::_NativeOpenDirectory
		virtual AssetDirectoryHandle _NativeOpenDirectory( std::string pDirectoryName ) override final;

		/// @copybrief AssetLoader::_NativeCheckDirectoryExists
		virtual bool _NativeCheckDirectoryExists( const std::string & pDirectoryName ) const override final;
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
		using AssetDirectory::SetDirName;

	private:
		void _InitializeAndroidAssetDirectoryData( AAssetManager * pAssetManager );
		void _ReleaseAndroidAssetDirectoryData();

		/// @copybrief AssetDirectory::_NativeRefreshAssetList
		virtual void _NativeRefreshAssetList() override final;

		/// @copybrief AssetDirectory::_NativeOpenAsset
		virtual AssetHandle _NativeOpenAsset( std::string pAssetName, cppx::bitmask<EAssetOpenFlags> pFlags ) override final;

		/// @copybrief AssetDirectory::_NativeCheckAssetExists
		virtual bool _NativeCheckAssetExists( const std::string & pAssetName ) const override final;
	};

	class AndroidAsset : public AndroidNativeObject<Asset, Platform::AndroidAssetNativeData>
	{
		friend class AndroidAssetLoader;

	public:
		explicit AndroidAsset( AndroidAssetLoaderHandle pAssetLoader );
		explicit AndroidAsset( AndroidAssetDirectoryHandle pAssetDirectory );
		virtual ~AndroidAsset() noexcept;

		using Asset::SetName;

	private:
		void _ReleaseAndroidAssetData();

		/// @copybrief Asset::_NativeReadData
		virtual file_size_t _NativeReadData( void * pTargetBuffer, file_size_t pReadSize ) override final;

		/// @copybrief Asset::_NativeSetReadPointer
		virtual file_offset_t _NativeSetReadPointer( file_offset_t pOffset, EFilePointerRefPos pRefPos ) override final;

		/// @copybrief Asset::_NativeGetSize
		virtual file_size_t _NativeGetSize() const override final;
	};

} // namespace Ic3::System

#endif // __IC3_SYSTEM_PLATFORM_OSAPI_ANDROID_ASSET_SYSTEM_H__
