
#ifndef __IC3_SYSTEM_ASSET_SYSTEM_H__
#define __IC3_SYSTEM_ASSET_SYSTEM_H__

#include "AssetCommon.h"
#include "FileCommon.h"
#include "SysObject.h"
#include <Ic3/Cppx/MemoryBuffer.h>

namespace Ic3::System
{

	using AssetNameList = std::vector<std::string>;

	enum EAssetOpenFlags : uint32
	{
		eAssetOpenFlagNoExtensionBit = 0x0001
	};

	class AssetLoader : public SysObject
	{
	public:
		explicit AssetLoader( SysContextHandle pSysContext );
		virtual ~AssetLoader() noexcept;

		AssetHandle openSubAsset( const std::string & pAssetRefName, TBitmask<EAssetOpenFlags> pFlags = 0u );

		AssetDirectoryHandle openDirectory( std::string pDirectoryName );

		bool checkDirectoryExists( const std::string & pDirectoryName ) const;

	private:
		virtual AssetHandle _nativeOpenSubAsset( Cppx::FilePathInfo pAssetPathInfo, TBitmask<EAssetOpenFlags> pFlags ) = 0;

		virtual AssetDirectoryHandle _nativeOpenDirectory( std::string pDirectoryName ) = 0;

		virtual bool _nativeCheckDirectoryExists( const std::string & pDirectoryName ) const = 0;
	};

	class AssetDirectory : public SysObject
	{
	public:
		AssetLoaderHandle const mAssetLoader;

	public:
		explicit AssetDirectory( AssetLoaderHandle pAssetLoader );
		virtual ~AssetDirectory() noexcept;

		void refreshAssetList();

		AssetHandle openAsset( std::string pAssetName, TBitmask<EAssetOpenFlags> pFlags = 0u );

		const AssetNameList & getAssetList() const;

		bool checkAssetExists( const std::string & pAssetName ) const;

		const std::string & getDirName() const;

	protected:
		void addAsset( std::string pAssetName );

		void setAssetList( AssetNameList pAssetList );

		void setDirName( std::string pDirName );

	private:
		virtual void _nativeRefreshAssetList() = 0;

		virtual AssetHandle _nativeOpenAsset( std::string pAssetName, TBitmask<EAssetOpenFlags> pFlags ) = 0;

		virtual bool _nativeCheckAssetExists( const std::string & pAssetName ) const = 0;

	private:
		std::string _dirName;
		AssetNameList _assetNameList;
	};

	class Asset : public SysObject
	{
	public:
		AssetDirectoryHandle const mAssetDirectory;

	public:
		explicit Asset( AssetLoaderHandle pAssetLoader );
		explicit Asset( AssetDirectoryHandle pAssetDirectory );
		virtual ~Asset() noexcept;

		file_size_t readData( void * pTargetBuffer, file_size_t pTargetBufferSize, file_size_t pReadSize = cxFileSizeMax );
		file_size_t readData( Cppx::MemoryBuffer & pBuffer, file_size_t pReadSize = cxFileSizeMax );

		template <typename TChar>
		file_size_t readData( std::basic_string<TChar> & pString, file_size_t pReadSize = cxFileSizeMax )
		{
			return readData( pString.data(), pString.length() * sizeof( TChar ), pReadSize );
		}

		template <typename TPValue>
		file_size_t readData( std::vector<TPValue> & pVector, file_size_t pReadSize = cxFileSizeMax )
		{
			return readData( pVector.data(), pVector.size() * sizeof( TPValue ), pReadSize );
		}

		file_size_t readAll( Cppx::DynamicMemoryBuffer & pBuffer, size_t pExtraAllocSize = 0 )
		{
			const auto assetSize = _nativeGetSize();
			pBuffer.resize( assetSize + pExtraAllocSize );
			return readData( pBuffer.data(), assetSize, assetSize );
		}

		template <typename TChar>
		file_size_t readAll( std::basic_string<TChar> & pString )
		{
			const auto assetSize = _nativeGetSize();
			const auto strLength = assetSize / sizeof( TChar );
			pString.resize( strLength + 1 );
			return readData( pString.data(), pString.length() * sizeof( TChar ), assetSize );
		}

		template <typename TPValue>
		file_size_t readAll( std::vector<TPValue> & pVector )
		{
			const auto assetSize = _nativeGetSize();
			const auto vectorSize = assetSize / sizeof( TPValue );
			pVector.resize( vectorSize );
			return readData( pVector.data(), pVector.size() * sizeof( TPValue ), vectorSize );
		}

		file_offset_t setReadPointer( file_offset_t pOffset, EFilePointerRefPos pRefPos = EFilePointerRefPos::FileBeg );

		void resetReadPointer();

		const std::string & getName() const;

	protected:
		void setName( std::string pAssetName );

	private:
		virtual file_size_t _nativeReadData( void * pTargetBuffer, file_size_t pReadSize ) = 0;

		virtual file_offset_t _nativeSetReadPointer( file_offset_t pOffset, EFilePointerRefPos pRefPos ) = 0;

		virtual file_size_t _nativeGetSize() const = 0;

	private:
		std::string _name;
	};

} // namespace Ic3::System

#endif // __IC3_SYSTEM_ASSET_SYSTEM_H__
