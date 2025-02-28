
#include "AssetSystem.h"

namespace Ic3::System
{

	AssetLoader::AssetLoader( SysContextHandle pSysContext )
	: SysObject( pSysContext )
	{}

	AssetLoader::~AssetLoader() noexcept = default;

	AssetHandle AssetLoader::OpenSubAsset( const std::string & pAssetRefName, cppx::bitmask<EAssetOpenFlags> pFlags )
	{
		if( pAssetRefName.empty() )
		{
			return nullptr;
		}

		auto assetRefName = cppx::fs_normalize_path( pAssetRefName );
		auto assetPathInfo = cppx::fs_split_path( std::move( assetRefName ), cppx::fs_api_flag_split_assume_file_bit );

		return _NativeOpenSubAsset( std::move( assetPathInfo ), pFlags );
	}

	AssetDirectoryHandle AssetLoader::OpenDirectory( std::string pDirectoryName )
	{
		if( pDirectoryName.empty() )
		{
			return nullptr;
		}

		auto assetDirectory = _NativeOpenDirectory( std::move( pDirectoryName ) );
		if( assetDirectory )
		{
			assetDirectory->RefreshAssetList();
		}

		return assetDirectory;
	}

	bool AssetLoader::CheckDirectoryExists( const std::string & pDirectoryName ) const
	{
		if( pDirectoryName.empty() )
		{
			return false;
		}
		return _NativeCheckDirectoryExists( pDirectoryName );
	}

	cppx::dynamic_memory_buffer AssetLoader::LoadAsset(
			System::AssetLoader & pAssetLoader,
			const std::string & pAssetPath,
			bool pAppendNullTerm )
	{
		auto psAsset = pAssetLoader.OpenSubAsset(
				pAssetPath,
				System::eAssetOpenFlagNoExtensionBit );

		cppx::dynamic_memory_buffer resultBuffer{};

		const auto extraAllocSize = pAppendNullTerm ? 1u : 0u;
		const auto sourceLength = psAsset->ReadAll( resultBuffer, extraAllocSize );

		if( pAppendNullTerm )
		{
			resultBuffer[sourceLength] = 0;
		}

		return resultBuffer;
	}


	AssetDirectory::AssetDirectory( AssetLoaderHandle pAssetLoader )
	: SysObject( pAssetLoader->mSysContext )
	, mAssetLoader( std::move( pAssetLoader ) )
	{}

	AssetDirectory::~AssetDirectory() noexcept = default;

	void AssetDirectory::RefreshAssetList()
	{
		if( !_assetNameList.empty() )
		{
			_assetNameList.clear();
		}
		_NativeRefreshAssetList();
	}

	AssetHandle AssetDirectory::OpenAsset( std::string pAssetName, cppx::bitmask<EAssetOpenFlags> pFlags )
	{
		if( pAssetName.empty() )
		{
			return nullptr;
		}
		return _NativeOpenAsset( std::move( pAssetName ), pFlags );
	}

	const AssetNameList & AssetDirectory::GetAssetList() const
	{
		return _assetNameList;
	}

	bool AssetDirectory::CheckAssetExists( const std::string & pAssetName ) const
	{
		return _NativeCheckAssetExists( pAssetName );
	}

	const std::string & AssetDirectory::GetDirName() const
	{
		return _dirName;
	}

	void AssetDirectory::AddAsset( std::string pAssetName )
	{
		_assetNameList.push_back( std::move( pAssetName ) );
	}

	void AssetDirectory::SetAssetList( AssetNameList pAssetList )
	{
		_assetNameList = std::move( pAssetList );
	}

	void AssetDirectory::SetDirName( std::string pDirName )
	{
		_dirName = std::move( pDirName );
	}


	Asset::Asset( AssetLoaderHandle pAssetLoader )
	: SysObject( pAssetLoader->mSysContext )
	, mAssetDirectory( nullptr )
	{}

	Asset::Asset( AssetDirectoryHandle pAssetDirectory )
	: SysObject( pAssetDirectory->mSysContext )
	, mAssetDirectory( std::move( pAssetDirectory ) )
	{}

	Asset::~Asset() noexcept = default;

	file_size_t Asset::ReadData( void * pTargetBuffer, file_size_t pTargetBufferSize, file_size_t pReadSize )
	{
		if( !pTargetBuffer || ( pTargetBufferSize == 0 ) || ( pReadSize == 0 ) )
		{
			return 0;
		}

		const auto readSize = cppx::get_min_of( pTargetBufferSize, pReadSize );

		return _NativeReadData( pTargetBuffer, readSize );
	}

	file_size_t Asset::ReadData( cppx::memory_buffer & pBuffer, file_size_t pReadSize )
	{
		return ReadData( pBuffer.data(), pBuffer.size(), pReadSize );
	}

	file_offset_t Asset::SetReadPointer( file_offset_t pOffset, EFilePointerRefPos pRefPos )
	{
		return _NativeSetReadPointer( pOffset, pRefPos );
	}

	void Asset::ResetReadPointer()
	{
		_NativeSetReadPointer( 0, EFilePointerRefPos::FileBeg );
	}

	const std::string & Asset::GetName() const
	{
		return _name;
	}

	void Asset::SetName( std::string pAssetName )
	{
		_name = std::move( pAssetName );
	}

} // namespace Ic3::System
