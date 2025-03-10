
#ifndef __IC3_SYSTEM_ASSET_SYSTEM_H__
#define __IC3_SYSTEM_ASSET_SYSTEM_H__

#include "AssetCommon.h"
#include "IOCommonDefs.h"
#include "../SysObject.h"
#include <cppx/fsUtils.h>
#include <cppx/memoryBuffer.h>

namespace Ic3::System
{

	using AssetNameList = std::vector<std::string>;

	enum EAssetOpenFlags : uint32
	{
		eAssetOpenFlagNoExtensionBit = 0x0001
	};

	class IC3_SYSTEM_CLASS AssetLoader : public SysObject
	{
	public:
		explicit AssetLoader( SysContextHandle pSysContext );
		virtual ~AssetLoader() noexcept;

		AssetHandle OpenSubAsset( const std::string & pAssetRefName, cppx::bitmask<EAssetOpenFlags> pFlags = 0u );

		AssetDirectoryHandle OpenDirectory( std::string pDirectoryName );

		bool CheckDirectoryExists( const std::string & pDirectoryName ) const;

		CPPX_ATTR_NO_DISCARD static cppx::dynamic_memory_buffer LoadAsset(
				System::AssetLoader & pAssetLoader,
				const std::string & pAssetPath,
				bool pAppendNullTerm = false );

	private:
		virtual AssetHandle _NativeOpenSubAsset( cppx::file_path_info pAssetPathInfo, cppx::bitmask<EAssetOpenFlags> pFlags ) = 0;

		virtual AssetDirectoryHandle _NativeOpenDirectory( std::string pDirectoryName ) = 0;

		virtual bool _NativeCheckDirectoryExists( const std::string & pDirectoryName ) const = 0;
	};

	class IC3_SYSTEM_CLASS AssetDirectory : public SysObject
	{
	public:
		AssetLoaderHandle const mAssetLoader;

	public:
		explicit AssetDirectory( AssetLoaderHandle pAssetLoader );
		virtual ~AssetDirectory() noexcept;

		void RefreshAssetList();

		AssetHandle OpenAsset( std::string pAssetName, cppx::bitmask<EAssetOpenFlags> pFlags = 0u );

		const AssetNameList & GetAssetList() const;

		bool CheckAssetExists( const std::string & pAssetName ) const;

		const std::string & GetDirName() const;

	protected:
		void AddAsset( std::string pAssetName );

		void SetAssetList( AssetNameList pAssetList );

		void SetDirName( std::string pDirName );

	private:
		virtual void _NativeRefreshAssetList() = 0;

		virtual AssetHandle _NativeOpenAsset( std::string pAssetName, cppx::bitmask<EAssetOpenFlags> pFlags ) = 0;

		virtual bool _NativeCheckAssetExists( const std::string & pAssetName ) const = 0;

	private:
		std::string _dirName;
		AssetNameList _assetNameList;
	};

	class IC3_SYSTEM_CLASS Asset : public SysObject
	{
	public:
		AssetDirectoryHandle const mAssetDirectory;

	public:
		explicit Asset( AssetLoaderHandle pAssetLoader );
		explicit Asset( AssetDirectoryHandle pAssetDirectory );
		virtual ~Asset() noexcept;

		io_size_t ReadData( void * pTargetBuffer, io_size_t pTargetBufferSize, io_size_t pReadSize = kIOSizeMax );
		io_size_t ReadData( cppx::memory_buffer & pBuffer, io_size_t pReadSize = kIOSizeMax );

		template <typename TChar>
		io_size_t ReadData( std::basic_string<TChar> & pString, io_size_t pReadSize = kIOSizeMax )
		{
			return ReadData( pString.data(), pString.length() * sizeof( TChar ), pReadSize );
		}

		template <typename TPValue>
		io_size_t ReadData( std::vector<TPValue> & pVector, io_size_t pReadSize = kIOSizeMax )
		{
			return ReadData( pVector.data(), pVector.size() * sizeof( TPValue ), pReadSize );
		}

		io_size_t ReadAll( cppx::dynamic_memory_buffer & pBuffer, size_t pExtraAllocSize = 0 )
		{
			const auto assetSize = _NativeGetSize();
			pBuffer.resize( assetSize + pExtraAllocSize );
			return ReadData( pBuffer.data(), assetSize, assetSize );
		}

		template <typename TChar>
		io_size_t ReadAll( std::basic_string<TChar> & pString )
		{
			const auto assetSize = _NativeGetSize();
			const auto strLength = assetSize / sizeof( TChar );
			pString.resize( strLength + 1 );
			return ReadData( pString.data(), pString.length() * sizeof( TChar ), assetSize );
		}

		template <typename TPValue>
		io_size_t ReadAll( std::vector<TPValue> & pVector )
		{
			const auto assetSize = _NativeGetSize();
			const auto vectorSize = assetSize / sizeof( TPValue );
			pVector.resize( vectorSize );
			return ReadData( pVector.data(), pVector.size() * sizeof( TPValue ), vectorSize );
		}

		io_offset_t SetReadPointer( io_offset_t pOffset, EIOPointerRefPos pRefPos = EIOPointerRefPos::StreamBase );

		void ResetReadPointer();

		const std::string & GetName() const;

	protected:
		void SetName( std::string pAssetName );

	private:
		virtual io_size_t _NativeReadData( void * pTargetBuffer, io_size_t pReadSize ) = 0;

		virtual io_offset_t _NativeSetReadPointer( io_offset_t pOffset, EIOPointerRefPos pRefPos ) = 0;

		virtual io_size_t _NativeGetSize() const = 0;

	private:
		std::string _name;
	};

} // namespace Ic3::System

#endif // __IC3_SYSTEM_ASSET_SYSTEM_H__
