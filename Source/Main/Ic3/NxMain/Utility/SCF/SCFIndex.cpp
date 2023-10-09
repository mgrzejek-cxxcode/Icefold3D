
#include "scfIndex.h"
#include <Ic3/NxMain/exception.h>
#include <Ic3/Cppx/pathNameIterator.h>

namespace Ic3
{

	SCFIndex::SCFIndex() = default;

	SCFIndex::~SCFIndex() = default;

	SCFEntry * SCFIndex::findEntryByPath( const std::string & pEntryPath ) const noexcept
	{
		ic3DebugAssert( _rootFolder );
		return _rootFolder->findEntry( pEntryPath, ESCFFindMode::Recursive );
	}

	SCFEntry & SCFIndex::getEntryByPath( const std::string & pEntryPath ) const
	{
		ic3DebugAssert( _rootFolder );
		return _rootFolder->getEntry( pEntryPath, ESCFFindMode::Recursive );
	}

	SCFEntry * SCFIndex::findEntryByUUID( const std::string & pUUID ) const noexcept
	{
		ic3DebugAssert( _rootFolder );
		auto entryIter = _entryByUIDMap.find( pUUID );
		if( entryIter == _entryByUIDMap.end() )
		{
			return nullptr;
		}
		return entryIter->second;
	}

	SCFEntry & SCFIndex::getEntryByUUID( const std::string & pUUID ) const
	{
		ic3DebugAssert( _rootFolder );
		auto entryIter = _entryByUIDMap.find( pUUID );
		if( entryIter == _entryByUIDMap.end() )
		{
			ic3ThrowDesc( E_EXC_ESM_MAIN_SCF_ERROR, "UUID " + pUUID + " not found in the index" );
		}
		return *( entryIter->second );
	}

	bool SCFIndex::enumerateEntries( SCFEntryList & pList, bool pClearList ) const
	{
		ic3DebugAssert( _rootFolder );
		return _rootFolder->enumerateEntries( pList, ESCFFindMode::Recursive, pClearList );
	}

	bool SCFIndex::enumerateEntries( SCFEntryList & pList, const SCFEntryPredicate & pPredicate, bool pClearList ) const
	{
		ic3DebugAssert( _rootFolder );
		return _rootFolder->enumerateEntries( pList, ESCFFindMode::Recursive, pPredicate, pClearList );
	}

	SCFEntryList SCFIndex::enumerateEntries() const
	{
		ic3DebugAssert( _rootFolder );
		return _rootFolder->enumerateEntries( ESCFFindMode::Recursive );
	}

	SCFEntryList SCFIndex::enumerateEntries( const SCFEntryPredicate & pPredicate ) const
	{
		ic3DebugAssert( _rootFolder );
		return _rootFolder->enumerateEntries( ESCFFindMode::Recursive, pPredicate );
	}

	SCFVirtualFolder & SCFIndex::rootFolder() const
	{
		return *_rootFolder;
	}

	SCFVirtualFolder & SCFIndex::initRootFolder( SCFVirtualFolderInfo pFolderInfo )
	{
		_rootFolder = std::make_unique<SCFVirtualFolder>( *this, std::move( pFolderInfo ) );
		return *_rootFolder;
	}

	void SCFIndex::addEntry( SCFEntry & pEntry )
	{
		if( !pEntry.mInfo->uid.empty() )
		{
			_entryByUIDMap[pEntry.mInfo->uid] = &pEntry;
		}
	}

	void SCFIndex::setResourceDataReadCallback( ResourceDataReadCallback pCallback )
	{
		_rdReadCallback = std::move( pCallback );
	}

	uint64 SCFIndex::readResourceData( void * pTarget, uint64 pSize, uint64 pOffset ) const
	{
		if( !_rdReadCallback )
		{
			return 0;
		}
		return _rdReadCallback( pTarget, pSize, pOffset );
	}

} // namespace Ic3
