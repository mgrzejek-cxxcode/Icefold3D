
#ifndef __IC3_SYSTEM_PLATFORM_COMMON_FILE_BASED_ASSET_SYSTEM__
#define __IC3_SYSTEM_PLATFORM_COMMON_FILE_BASED_ASSET_SYSTEM__

#include <Ic3/System/AssetSystem.h>
#include <Ic3/System/FileCommon.h>

#if( IC3_SYSTEM_USE_ASSET_SYSTEM_DEFAULT )
namespace Ic3::System
{

	ic3SysDeclareHandle( FileAssetLoader );

	namespace Platform
	{

		struct FileAssetNativeData
		{
			FileHandle mFileHandle = nullptr;
			std::string mFilePath;
		};

		struct FileAssetDirectoryNativeData
		{
			std::string mCombinedDirPath;
		};

		struct FileAssetLoaderNativeData
		{
			std::string mRootDir;
		};

		struct AssetLoaderCreateInfoNativeParams
		{
			FileManagerHandle mFileManager = nullptr;

			std::string mAbsoluteAssetRootDir;

			std::string mRelativeAssetRootDir;
		};

		IC3_SYSTEM_API_NODISCARD AssetLoaderHandle createFileAssetLoaderExplicit(
				SysContextHandle pSysContext,
				FileManagerHandle pFileManager,
				const std::string & pAbsoluteAssetDirectory );

		IC3_SYSTEM_API_NODISCARD AssetLoaderHandle createFileAssetLoaderResolve(
				SysContextHandle pSysContext,
				FileManagerHandle pFileManager,
				const std::string & pRelativeAssetDirectory );

		IC3_SYSTEM_API_NODISCARD AssetLoaderHandle createFileAssetLoader(
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

		void setRootDir( std::string pRootDir );

	private:
		/// @copybrief AssetLoader::_nativeOpenSubAsset
		virtual AssetHandle _nativeOpenSubAsset( Cppx::FilePathInfo pAssetPathInfo,
		                                         TBitmask<EAssetOpenFlags> pFlags ) override final;

		/// @copybrief AssetLoader::_nativeOpenDirectory
		virtual AssetDirectoryHandle _nativeOpenDirectory( std::string pDirectoryName ) override final;

		/// @copybrief AssetLoader::_nativeCheckDirectoryExists
		virtual bool _nativeCheckDirectoryExists( const std::string & pDirectoryName ) const override final;
	};
	
	class FileAssetDirectory : public NativeObject<AssetDirectory, Platform::FileAssetDirectoryNativeData>
	{
		friend class FileAssetLoader;

	public:
		FileManagerHandle const mFileManager;

	public:
		explicit FileAssetDirectory( FileAssetLoaderHandle pAssetLoader );
		virtual ~FileAssetDirectory() noexcept;

		using AssetDirectory::addAsset;
		using AssetDirectory::setDirName;
		
	private:
		/// @copybrief AssetDirectory::_nativeRefreshAssetList
		virtual void _nativeRefreshAssetList() override final;

		/// @copybrief AssetDirectory::_nativeOpenAsset
		virtual AssetHandle _nativeOpenAsset( std::string pAssetName, TBitmask<EAssetOpenFlags> pFlags ) override final;

		/// @copybrief AssetDirectory::_nativeCheckAssetExists
		virtual bool _nativeCheckAssetExists( const std::string & pAssetName ) const override final;
	};

	class FileAsset : public NativeObject<Asset, Platform::FileAssetNativeData>
	{
		friend class FileAssetLoader;
		
	public:
		explicit FileAsset( AssetLoaderHandle pAssetLoader );
		explicit FileAsset( AssetDirectoryHandle pAssetDirectory );
		virtual ~FileAsset() noexcept;

		using Asset::setName;

	private:
		/// @copybrief Asset::_nativeReadData
		virtual file_size_t _nativeReadData( void * pTargetBuffer, file_size_t pReadSize ) override final;

		/// @copybrief Asset::_nativeSetReadPointer
		virtual file_offset_t _nativeSetReadPointer( file_offset_t pOffset, EFilePointerRefPos pRefPos ) override final;

		/// @copybrief Asset::_nativeGetSize
		virtual file_size_t _nativeGetSize() const override final;
	};

} // namespace Ic3::System
#endif // IC3_SYSTEM_USE_ASSET_SYSTEM_DEFAULT

#endif // __IC3_SYSTEM_PLATFORM_COMMON_FILE_BASED_ASSET_SYSTEM__
