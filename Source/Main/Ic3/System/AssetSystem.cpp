
#include "AssetSystem.h"

namespace Ic3::System
{

	AssetLoader::AssetLoader( SysContextHandle pSysContext )
	: SysObject( pSysContext )
	{}

	AssetLoader::~AssetLoader() noexcept = default;

	AssetHandle AssetLoader::openSubAsset( const std::string & pAssetRefName, TBitmask<EAssetOpenFlags> pFlags )
	{
		if( pAssetRefName.empty() )
		{
			return nullptr;
		}

		auto assetRefName = Cppx::fsNormalizePath( pAssetRefName );
		auto assetPathInfo = fsSplitPath( std::move( assetRefName ), Cppx::eFSAPISplitPathFlagAssumeFileBit );

		return _nativeOpenSubAsset( std::move( assetPathInfo ), pFlags );
	}

	AssetDirectoryHandle AssetLoader::openDirectory( std::string pDirectoryName )
	{
		if( pDirectoryName.empty() )
		{
			return nullptr;
		}

		auto assetDirectory = _nativeOpenDirectory( std::move( pDirectoryName ) );
		if( assetDirectory )
		{
			assetDirectory->refreshAssetList();
		}

		return assetDirectory;
	}

	bool AssetLoader::checkDirectoryExists( const std::string & pDirectoryName ) const
	{
		if( pDirectoryName.empty() )
		{
			return false;
		}
		return _nativeCheckDirectoryExists( pDirectoryName );
	}


	AssetDirectory::AssetDirectory( AssetLoaderHandle pAssetLoader )
	: SysObject( pAssetLoader->mSysContext )
	, mAssetLoader( std::move( pAssetLoader ) )
	{}

	AssetDirectory::~AssetDirectory() noexcept = default;

	void AssetDirectory::refreshAssetList()
	{
		if( !_assetNameList.empty() )
		{
			_assetNameList.clear();
		}
		_nativeRefreshAssetList();
	}

	AssetHandle AssetDirectory::openAsset( std::string pAssetName, TBitmask<EAssetOpenFlags> pFlags )
	{
		if( pAssetName.empty() )
		{
			return nullptr;
		}
		return _nativeOpenAsset( std::move( pAssetName ), pFlags );
	}

	const AssetNameList & AssetDirectory::getAssetList() const
	{
		return _assetNameList;
	}

	bool AssetDirectory::checkAssetExists( const std::string & pAssetName ) const
	{
		return _nativeCheckAssetExists( pAssetName );
	}

	const std::string & AssetDirectory::getDirName() const
	{
		return _dirName;
	}

	void AssetDirectory::addAsset( std::string pAssetName )
	{
		_assetNameList.push_back( std::move( pAssetName ) );
	}

	void AssetDirectory::setAssetList( AssetNameList pAssetList )
	{
		_assetNameList = std::move( pAssetList );
	}

	void AssetDirectory::setDirName( std::string pDirName )
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

	file_size_t Asset::readData( void * pTargetBuffer, file_size_t pTargetBufferSize, file_size_t pReadSize )
	{
		if( !pTargetBuffer || ( pTargetBufferSize == 0 ) || ( pReadSize == 0 ) )
		{
			return 0;
		}

		const auto readSize = Cppx::getMinOf( pTargetBufferSize, pReadSize );

		return _nativeReadData( pTargetBuffer, readSize );
	}

	file_size_t Asset::readData( Cppx::MemoryBuffer & pBuffer, file_size_t pReadSize )
	{
		return readData( pBuffer.data(), pBuffer.size(), pReadSize );
	}

	file_offset_t Asset::setReadPointer( file_offset_t pOffset, EFilePointerRefPos pRefPos )
	{
		return _nativeSetReadPointer( pOffset, pRefPos );
	}

	void Asset::resetReadPointer()
	{
		_nativeSetReadPointer( 0, EFilePointerRefPos::FileBeg );
	}

	const std::string & Asset::getName() const
	{
		return _name;
	}

	void Asset::setName( std::string pAssetName )
	{
		_name = std::move( pAssetName );
	}

} // namespace Ic3::System
