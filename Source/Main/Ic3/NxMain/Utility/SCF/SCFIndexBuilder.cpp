
#include "scfIndexBuilder.h"
#include <Ic3/NxMain/exception.h>
#include <cppx/pathNameIterator.h>
#include <cppx/sortedArray.h>
#include <Ic3/System/IO/FileSystem.h>

#include <unordered_map>
#include <unordered_set>

namespace Ic3
{

	bool operator==( const SCFResourceTemplate & pLhs, const SCFResourceTemplate & pRhs );
	bool operator==( const SCFResourceTemplate & pLhs, const std::string & pRhs );

	bool operator<( const SCFResourceTemplate & pLhs, const SCFResourceTemplate & pRhs );
	bool operator<( const SCFResourceTemplate & pLhs, const std::string & pRhs );

	bool operator==( const SCFVirtualFolderTemplate & pLhs, const SCFVirtualFolderTemplate & pRhs );
	bool operator==( const SCFVirtualFolderTemplate & pLhs, const std::string & pRhs );

	bool operator<( const SCFVirtualFolderTemplate & pLhs, const SCFVirtualFolderTemplate & pRhs );
	bool operator<( const SCFVirtualFolderTemplate & pLhs, const std::string & pRhs );


	SCFInputDataSource SCFInputDataSource::asPlaceholder( uint64 pDataSize )
	{
		if( pDataSize == 0 )
		{
			return {};
		}

		SCFInputDataSource dataSource;
		dataSource.byteSize = pDataSize;
		dataSource.readCallback =
			[pDataSize]( uint64 pOffset, uint64 pReadSize, Dynamicbyte_array & pTarget ) -> uint64 {
				const auto readSize = mem_check_requested_copy_size( pDataSize, pReadSize, pOffset );
				if( readSize > 0 )
				{
					pTarget.resize( cppx::numeric_cast<size_t>( readSize ) );
					pTarget.fill( 0x7 );
					return readSize;
				}
				return 0;
			};

		return dataSource;
	}

	SCFInputDataSource SCFInputDataSource::fromFile( System::FileManagerHandle pSysFileManager, const std::string & pFilename )
	{
		if( !pSysFileManager || pFilename.empty() )
		{
			return {};
		}

		auto sourceFile = pSysFileManager->openFile( pFilename, System::EIOAccessMode::ReadOnly );
		if( !sourceFile )
		{
			return {};
		}

		const auto fileSize = sourceFile->getSize();

		SCFInputDataSource dataSource;
		dataSource.byteSize = fileSize;
		dataSource.readCallback =
			[pSysFileManager, pFilename, fileSize]( uint64 pOffset, uint64 pReadSize, Dynamicbyte_array & pTarget ) -> uint64 {
				const auto readSize = mem_check_requested_copy_size( fileSize, pReadSize, pOffset );
				if( readSize > 0 )
				{
					if( auto sourceFile = pSysFileManager->openFile( pFilename, System::EIOAccessMode::ReadOnly ) )
					{
						sourceFile->setFilePointer( cppx::numeric_cast<System::io_offset_t>( pOffset ) );
						return sourceFile->readAuto( pTarget, cppx::numeric_cast<System::io_size_t>( readSize ) );
					}
				}
				return 0;
			};

		return dataSource;
	}

	SCFInputDataSource SCFInputDataSource::fromMemory( read_only_memory_view pMemoryView )
	{
		if( !pMemoryView )
		{
			return {};
		}

		SCFInputDataSource dataSource;
		dataSource.byteSize = pMemoryView.size();
		dataSource.readCallback =
			[pMemoryView]( uint64 pOffset, uint64 pReadSize, Dynamicbyte_array & pTarget ) -> uint64 {
				const auto readSize = mem_check_requested_copy_size( pMemoryView.size(), pReadSize, pOffset );
				if( readSize > 0 )
				{
					pTarget.resize( cppx::numeric_cast<size_t>( readSize ) );

					mem_copy_unchecked( pTarget.data(),
									  pTarget.size(),
									  pMemoryView.data() + pOffset,
									  cppx::numeric_cast<size_t>( readSize ) );

					return readSize;
				}
				return 0;
			};

		return dataSource;
	}


	struct SCFIndexBuilder::PrivateWorkingData
	{
		SCFVirtualFolderTemplate rootFolder;
		std::unordered_set<std::string> globalPathSet;
		std::unordered_set<std::string> globalUIDSet;
	};


	SCFIndexBuilder::SCFIndexBuilder()
	: _privateWorkingData{ std::make_unique<PrivateWorkingData>() }
	{}

	SCFIndexBuilder::~SCFIndexBuilder() = default;

	const SCFVirtualFolderTemplate * SCFIndexBuilder::addFolder( const SCFVirtualFolderTemplate * pParentFolder,
																 std::string pFolderName,
																 std::string pUID )
	{
		if( !pParentFolder )
		{
			pParentFolder = &( _privateWorkingData->rootFolder );
		}

		return addFolder( pParentFolder->path, std::move( pFolderName ), std::move( pUID ) );
	}

	const SCFVirtualFolderTemplate * SCFIndexBuilder::addFolder( const std::string & pParentLocation,
																 std::string pFolderName,
																 std::string pUID )
	{
		auto * parentFolder = _findFolderInternal( pParentLocation, nullptr );
		if( !parentFolder )
		{
			return nullptr;
		}

		if( !checkNameAndUID( pFolderName, pUID ) )
		{
			return nullptr;
		}

		auto newFolderPath = parentFolder->path + "/" + pFolderName;

		auto existingPathIter = _privateWorkingData->globalPathSet.find( newFolderPath );
		if( existingPathIter != _privateWorkingData->globalPathSet.end() )
		{
			return nullptr;
		}

		SCFVirtualFolderTemplate virtualFolder;
		virtualFolder.name = std::move( pFolderName );
		virtualFolder.uid = std::move( pUID );
		virtualFolder.path = std::move( newFolderPath );
		virtualFolder.treeSubLevel = parentFolder->treeSubLevel + 1;

		// Add the folder to the sub-folder list of its parent.
		auto newFolderIter = parentFolder->subFolderList.insert( std::move( virtualFolder ) );

		// Insert the full path to global list for easier validation.
		_privateWorkingData->globalPathSet.insert( newFolderIter->path );

		if( !newFolderIter->uid.empty() )
		{
			// If UID was specified, add it to the global UID list as well.
			_privateWorkingData->globalUIDSet.insert( newFolderIter->uid );
		}

		return &( *newFolderIter );

	}

	const SCFResourceTemplate * SCFIndexBuilder::addResource( const SCFVirtualFolderTemplate * pParentFolder,
															  std::string pResourceName,
															  SCFInputDataSource pDataSource,
															  std::string pUID )
	{
		if( !pParentFolder )
		{
			pParentFolder = &( _privateWorkingData->rootFolder );
		}

		return addResource( pParentFolder->path, std::move( pResourceName ), std::move( pDataSource ), std::move( pUID ) );
	}

	const SCFResourceTemplate * SCFIndexBuilder::addResource( const std::string & pParentLocation,
															  std::string pResourceName,
															  SCFInputDataSource pDataSource,
															  std::string pUID )
	{
		if( !pDataSource || !checkNameAndUID( pResourceName, pUID ) )
		{
			return nullptr;
		}

		auto * parentFolder = _findFolderInternal( pParentLocation, nullptr );
		if( !parentFolder )
		{
			return nullptr;
		}

		auto newResourcePath = parentFolder->path + "/" + pResourceName;

		auto existingPathIter = _privateWorkingData->globalPathSet.find( newResourcePath );
		if( existingPathIter != _privateWorkingData->globalPathSet.end() )
		{
			return nullptr;
		}

		SCFResourceTemplate resource;
		resource.name = std::move( pResourceName );
		resource.uid = std::move( pUID );
		resource.path = std::move( newResourcePath );
		resource.treeSubLevel = parentFolder->treeSubLevel + 1;
		resource.dataSource = std::move( pDataSource );

		// Add the folder to the resource list of its parent.
		auto resourceIter = parentFolder->resourceList.insert( std::move( resource ) );

		// Insert the full path to global list for easier validation.
		_privateWorkingData->globalPathSet.insert( resourceIter->path );

		if( !resourceIter->uid.empty() )
		{
			// If UID was specified, add it to the global UID list as well.
			_privateWorkingData->globalUIDSet.insert( resourceIter->uid );
		}

		return &( *resourceIter );
	}

	bool SCFIndexBuilder::removeEntry(  const std::string & pLocation )
	{
		return false;
	}

	const SCFVirtualFolderTemplate & SCFIndexBuilder::getRootVirtualFolder() const
	{
		return _privateWorkingData->rootFolder;
	}

	bool SCFIndexBuilder::checkNameAndUID( const std::string & pName, const std::string & pUID ) const
	{
		if( pName.empty() )
		{
			return false;
		}

		const auto separatorPos = pName.find_first_of( "/\\", 0 );
		if( separatorPos != std::string::npos )
		{
			return false;
		}

		if( !pUID.empty() )
		{
			const auto existingUIDIter = _privateWorkingData->globalUIDSet.find( pUID );
			if( existingUIDIter != _privateWorkingData->globalUIDSet.end() )
			{
				return false;
			}
		}

		return true;
	}

	SCFVirtualFolderTemplate * SCFIndexBuilder::_findFolderInternal( const std::string & pFolderLocation,
																	 SCFVirtualFolderTemplate * pRootFolder )
	{
		if( !pRootFolder )
		{
			pRootFolder = &( _privateWorkingData->rootFolder );
		}

		SCFVirtualFolderTemplate * currentFolderPtr = pRootFolder;

		for( auto pathNameIterator = path_name_iterator( pFolderLocation ); !pathNameIterator.empty(); ++pathNameIterator )
		{
			// Iterator yields a name of a next sub-folder.
			const auto & subFolderName = pathNameIterator.name();

			// Get the sub-folder from the sub-folder list. If there is no such thing,
			// it means the path specified is invalid. Simply return nullptr in such case.
			const auto subFolderIter = currentFolderPtr->subFolderList.find( subFolderName );

			if( subFolderIter == currentFolderPtr->subFolderList.end() )
			{
				currentFolderPtr = nullptr;
				break;
			}

			// Set the folder pointer to this sub-folder and continue.
			currentFolderPtr = &( *subFolderIter );
		}

		// Either a valid folder with a requested path or nullptr.
		return currentFolderPtr;
	}

	SCFResourceTemplate * SCFIndexBuilder::_findResourceInternal( const std::string & pResourceLocation,
																  SCFVirtualFolderTemplate * pRootFolder )
	{
		if( !pRootFolder )
		{
			pRootFolder = &( _privateWorkingData->rootFolder );
		}

		SCFVirtualFolderTemplate * currentFolderPtr = pRootFolder;
		SCFResourceTemplate * resourcePtr = nullptr;

		for( auto pathNameIterator = path_name_iterator( pResourceLocation ); !pathNameIterator.empty(); ++pathNameIterator )
		{
			// Iterator yields a name of a next sub-folder or a resource.
			const auto & subLocationName = pathNameIterator.name();

			// Check if there are still some path components to be iterated over. If so, the current
			// one refers to a sub-folder. Otherwise, (the last path component) - it is a resource name.
			if( pathNameIterator.hasNext() )
			{
				const auto subFolderIter = currentFolderPtr->subFolderList.find( subLocationName );
				if( subFolderIter == currentFolderPtr->subFolderList.end() )
				{
					break;
				}

				currentFolderPtr = &( *subFolderIter );
			}
			else
			{
				const auto resourceIter = pRootFolder->resourceList.find( subLocationName );
				if( resourceIter == pRootFolder->resourceList.end() )
				{
					break;
				}

				resourcePtr = &( *resourceIter );
			}
		}

		return resourcePtr;
	}



	bool operator==( const SCFResourceTemplate & pLhs, const SCFResourceTemplate & pRhs )
	{
		return pLhs.name == pRhs.name;
	}

	bool operator==( const SCFResourceTemplate & pLhs, const std::string & pRhs )
	{
		return pLhs.name == pRhs;
	}

	bool operator<( const SCFResourceTemplate & pLhs, const SCFResourceTemplate & pRhs )
	{
		return pLhs.name < pRhs.name;
	}

	bool operator<( const SCFResourceTemplate & pLhs, const std::string & pRhs )
	{
		return pLhs.name < pRhs;
	}

	bool operator==( const SCFVirtualFolderTemplate & pLhs, const SCFVirtualFolderTemplate & pRhs )
	{
		return pLhs.name == pRhs.name;
	}

	bool operator==( const SCFVirtualFolderTemplate & pLhs, const std::string & pRhs )
	{
		return pLhs.name == pRhs;
	}

	bool operator<( const SCFVirtualFolderTemplate & pLhs, const SCFVirtualFolderTemplate & pRhs )
	{
		return pLhs.name < pRhs.name;
	}

	bool operator<( const SCFVirtualFolderTemplate & pLhs, const std::string & pRhs )
	{
		return pLhs.name < pRhs;
	}

} // namespace Ic3
