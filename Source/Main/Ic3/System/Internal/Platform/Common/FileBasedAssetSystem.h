
#ifndef __IC3_SYSTEM_PLATFORM_COMMON_FILE_BASED_ASSET_SYSTEM__
#define __IC3_SYSTEM_PLATFORM_COMMON_FILE_BASED_ASSET_SYSTEM__

#include <Ic3/System/AssetSystem.h>
#include <Ic3/System/FileCommon.h>

#if( IC3_SYSTEM_USE_ASSET_SYSTEM_DEFAULT )
namespace Ic3::System
{

	Ic3SysDeclareHandle( FileAssetLoader );

	namespace Platform
	{

		struct FileAssetNativeData
		{
			FileHandle fileHandle = nullptr;
			std::string filePath;
		};

		struct FileAssetDirectoryNativeData
		{
			std::string combinedDirPath;
		};

		struct FileAssetLoaderNativeData
		{
			std::string rootDir;
		};

		struct AssetLoaderCreateInfoNativeParams
		{
			FileManagerHandle fileManager = nullptr;
			std::string absoluteAssetRootDir;
			std::string relativeAssetRootDir;
		};

		IC3_SYSTEM_API_NODISCARD AssetLoaderHandle CreateFileAssetLoaderExplicit(
				SysContextHandle pSysContext,
				FileManagerHandle pFileManager,
				const std::string & pAbsoluteAssetDirectory );

		IC3_SYSTEM_API_NODISCARD AssetLoaderHandle CreateFileAssetLoaderResolve(
				SysContextHandle pSysContext,
				FileManagerHandle pFileManager,
				const std::string & pRelativeAssetDirectory );

		IC3_SYSTEM_API_NODISCARD AssetLoaderHandle CreateFileAssetLoader(
				SysContextHandle pSysContext,
				const AssetLoaderCreateInfoNativeParams & pCreateParams );

	}

	class FileAssetLoader : public NativeObject<AssetLoader, Platform::FileAssetLoaderNativeData>
	{
	public:
		FileManagerHandle const mFileManager;

	public:
		explicit FileAssetLoader( FileManagerHandle pFileManager );
		virtual ~FileAssetLoader() noexcept;

		void SetRootDir( std::string pRootDir );

	private:
		/// @copybrief AssetLoader::_NativeOpenSubAsset
		virtual AssetHandle _NativeOpenSubAsset( cppx::file_path_info pAssetPathInfo,
		                                         cppx::bitmask<EAssetOpenFlags> pFlags ) override final;

		/// @copybrief AssetLoader::_NativeOpenDirectory
		virtual AssetDirectoryHandle _NativeOpenDirectory( std::string pDirectoryName ) override final;

		/// @copybrief AssetLoader::_NativeCheckDirectoryExists
		virtual bool _NativeCheckDirectoryExists( const std::string & pDirectoryName ) const override final;
	};
	
	class FileAssetDirectory : public NativeObject<AssetDirectory, Platform::FileAssetDirectoryNativeData>
	{
		friend class FileAssetLoader;

	public:
		FileManagerHandle const mFileManager;

	public:
		explicit FileAssetDirectory( FileAssetLoaderHandle pAssetLoader );
		virtual ~FileAssetDirectory() noexcept;

		using AssetDirectory::AddAsset;
		using AssetDirectory::SetDirName;
		
	private:
		/// @copybrief AssetDirectory::_NativeRefreshAssetList
		virtual void _NativeRefreshAssetList() override final;

		/// @copybrief AssetDirectory::_NativeOpenAsset
		virtual AssetHandle _NativeOpenAsset( std::string pAssetName, cppx::bitmask<EAssetOpenFlags> pFlags ) override final;

		/// @copybrief AssetDirectory::_NativeCheckAssetExists
		virtual bool _NativeCheckAssetExists( const std::string & pAssetName ) const override final;
	};

	class FileAsset : public NativeObject<Asset, Platform::FileAssetNativeData>
	{
		friend class FileAssetLoader;
		
	public:
		explicit FileAsset( AssetLoaderHandle pAssetLoader );
		explicit FileAsset( AssetDirectoryHandle pAssetDirectory );
		virtual ~FileAsset() noexcept;

		using Asset::SetName;

	private:
		/// @copybrief Asset::_NativeReadData
		virtual file_size_t _NativeReadData( void * pTargetBuffer, file_size_t pReadSize ) override final;

		/// @copybrief Asset::_NativeSetReadPointer
		virtual file_offset_t _NativeSetReadPointer( file_offset_t pOffset, EFilePointerRefPos pRefPos ) override final;

		/// @copybrief Asset::_NativeGetSize
		virtual file_size_t _NativeGetSize() const override final;
	};

} // namespace Ic3::System
#endif // IC3_SYSTEM_USE_ASSET_SYSTEM_DEFAULT

#endif // __IC3_SYSTEM_PLATFORM_COMMON_FILE_BASED_ASSET_SYSTEM__
