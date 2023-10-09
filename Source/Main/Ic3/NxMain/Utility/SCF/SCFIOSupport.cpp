
#include "scfIOSupport.h"
#include "scfIndexBuilder.h"
#include <Ic3/System/FileManager.h>
#include <Ic3/CoreLib/utility/gdsCore.h>
#include <vector>

namespace Ic3
{

	SCFIOProxy::SCFIOProxy( System::FileManagerHandle pSysFileManager )
	: _sysFileManager( std::move( pSysFileManager ) )
	{}

	void SCFIOProxy::saveIndex( const std::string & pFilename, const SCFIndexBuilder & pBuilder )
	{
		auto file = _sysFileManager->openFile( pFilename, System::EFileOpenMode::WriteOverwrite );
		if( !file )
		{
			return;
		}

		InternalFileWriteCallback fileWriteCallback =
			[file]( const void * pInputData, uint64 pWriteSize ) -> uint64 {
				const auto writeSize = numeric_cast<System::file_size_t>( pWriteSize );
				return file->write( pInputData, writeSize, writeSize );
			};

		DynamicByteArray sharedBuffer;

		const auto & rootFolder = pBuilder.getRootVirtualFolder();

		writeFolderData( file, rootFolder, sharedBuffer, fileWriteCallback );
	}

	void SCFIOProxy::loadIndex( const std::string & pFilename, SCFIndex & pIndex )
	{
		auto file = _sysFileManager->openFile( pFilename, System::EFileOpenMode::ReadOnly );
		if( !file )
		{
			return;
		}

		InternalFileReadCallback fileReadCallback =
			[file]( void * pOutputBuffer, uint64 pReadSize ) -> uint64 {
				const auto readSize = numeric_cast<System::file_size_t>( pReadSize );
				return file->read( pOutputBuffer, readSize, readSize );
			};

		DynamicByteArray sharedBuffer;

		SCFVirtualFolderInfo scfRootFolderInfo {};
		GDSCore::deserializeExternal( scfRootFolderInfo, fileReadCallback, sharedBuffer );

		auto & rootFolder = pIndex.initRootFolder(std::move( scfRootFolderInfo ) );

		readFolder( file, rootFolder, sharedBuffer, fileReadCallback );

		SCFIndex::ResourceDataReadCallback resourceDataReadCallback =
			[file]( void * pOutputBuffer, uint64 pReadSize, uint64 pBaseOffset ) -> uint64 {
				const auto readSize = numeric_cast<System::file_size_t>( pReadSize );
				file->setFilePointer( numeric_cast<System::file_offset_t>( pBaseOffset ) );
				return file->read( pOutputBuffer, readSize, readSize );
		};

		pIndex.setResourceDataReadCallback( std::move( resourceDataReadCallback ) );
	}

	void SCFIOProxy::writeFolderData( System::FileHandle pSysFile,
									  const SCFVirtualFolderTemplate & pFolder,
									  DynamicByteArray & pGdsCache,
									  const InternalFileWriteCallback & pFileWriteCallback )
	{
		SCFVirtualFolderInfo scfFolderInfo;
		scfFolderInfo.entryType = ESCFEntryType::VirtualFolder;
		scfFolderInfo.name = pFolder.name;
		scfFolderInfo.uid = pFolder.uid;
		scfFolderInfo.treeSubLevel = pFolder.treeSubLevel;
		scfFolderInfo.resourcesNum = static_cast<uint32>( pFolder.resourceList.size() );
		scfFolderInfo.subFoldersNum = static_cast<uint32>( pFolder.subFolderList.size() );

		GDSCore::serializeExternal( scfFolderInfo, pFileWriteCallback, pGdsCache );

		for( const auto & resource : pFolder.resourceList )
		{
			writeResourceData( pSysFile, resource, pGdsCache, pFileWriteCallback );
		}

		for( const auto & subFolder : pFolder.subFolderList )
		{
			writeFolderData( pSysFile, subFolder, pGdsCache, pFileWriteCallback );
		}
	}

	void SCFIOProxy::writeResourceData( System::FileHandle pSysFile,
										const SCFResourceTemplate & pResource,
										DynamicByteArray & pGdsCache,
										const InternalFileWriteCallback & pFileWriteCallback )
	{
		SCFResourceInfo scfResourceInfo;
		scfResourceInfo.entryType = ESCFEntryType::VirtualFolder;
		scfResourceInfo.name = pResource.name;
		scfResourceInfo.uid = pResource.uid;
		scfResourceInfo.treeSubLevel = pResource.treeSubLevel;

		const auto currentFilePtrOffset = pSysFile->getFilePointer();
		const auto resourceEntrySize = GDSCore::evalByteSizeWithMetaData( scfResourceInfo );

		scfResourceInfo.dataOffset = currentFilePtrOffset + resourceEntrySize;
		scfResourceInfo.dataSize = pResource.dataSource.byteSize;

		GDSCore::serializeExternal( scfResourceInfo, pFileWriteCallback, pGdsCache );

		const auto sMaxSingleDataWriteSize = 2048;
		
		for( uint64 currentReadPtr = 0; currentReadPtr < scfResourceInfo.dataSize; )
		{
			const auto readSize = pResource.dataSource.readCallback( currentReadPtr, sMaxSingleDataWriteSize, pGdsCache );
			ic3DebugAssert( readSize > 0 );

			pSysFile->write( pGdsCache.data(), pGdsCache.size() );

			currentReadPtr += readSize;
		}
	}

	void SCFIOProxy::readFolder( System::FileHandle pSysFile,
								 SCFVirtualFolder & pFolder,
								 DynamicByteArray & pGdsCache,
								 const InternalFileReadCallback & pFileReadCallback )
	{
		for( uint32 resourceIndex = 0; resourceIndex < pFolder.mFolderInfo.resourcesNum; ++resourceIndex )
		{
			SCFResourceInfo scfResourceInfo;
			GDSCore::deserializeExternal( scfResourceInfo, pFileReadCallback, pGdsCache );

			// The resource path is not serialized, don't forget this line.
			scfResourceInfo.path = pFolder.mFolderInfo.path + "/" + scfResourceInfo.name;

			auto & scfResource = pFolder.addResource( std::move( scfResourceInfo ) );

			const auto skipResourceDataOffset = scfResource.mResourceInfo.dataOffset + scfResource.mResourceInfo.dataSize;
			pSysFile->setFilePointer( static_cast<System::file_offset_t>( skipResourceDataOffset ) );
		}

		for( uint32 subFolderIndex = 0; subFolderIndex < pFolder.mFolderInfo.subFoldersNum; ++subFolderIndex )
		{
			SCFVirtualFolderInfo scfFolderInfo {};
			GDSCore::deserializeExternal( scfFolderInfo, pFileReadCallback, pGdsCache );

			// The folder path is not serialized, don't forget this line.
			scfFolderInfo.path = pFolder.mFolderInfo.path + "/" + scfFolderInfo.name;

			auto & scfFolder = pFolder.addSubFolder( std::move( scfFolderInfo ) );

			readFolder( pSysFile, scfFolder, pGdsCache, pFileReadCallback );
		}
	}

	namespace GDS
	{

		gds_size_t serialize( byte * pOutputBuffer, const SCFEntryInfo & pValue )
		{
			return GDSCore::serializeAll( pOutputBuffer,
								 pValue.entryType,
								 pValue.name,
								 pValue.uid,
								 pValue.treeSubLevel );
		}

		gds_size_t deserialize( const byte * pInputData, SCFEntryInfo & pValue )
		{
			return GDSCore::deserializeAll( pInputData,
								   pValue.entryType,
								   pValue.name,
								   pValue.uid,
								   pValue.treeSubLevel );
		}

		gds_size_t evalByteSize( const SCFEntryInfo & pValue )
		{
			return GDSCore::evalByteSizeAll( pValue.entryType,
									pValue.name,
									pValue.uid,
									pValue.treeSubLevel );
		}

		gds_size_t serialize( byte * pOutputBuffer, const SCFResourceInfo & pValue )
		{
			return GDSCore::serializeAll( pOutputBuffer,
								 static_cast<const SCFEntryInfo &>( pValue ),
								 pValue.dataOffset,
								 pValue.dataSize );
		}

		gds_size_t deserialize( const byte * pInputData, SCFResourceInfo & pValue )
		{
			return GDSCore::deserializeAll( pInputData,
								   static_cast<SCFEntryInfo &>( pValue ),
								   pValue.dataOffset,
								   pValue.dataSize );
		}

		gds_size_t evalByteSize( const SCFResourceInfo & pValue )
		{
			return GDSCore::evalByteSizeAll( static_cast<const SCFEntryInfo &>( pValue ),
									pValue.dataOffset,
									pValue.dataSize );
		}

		gds_size_t serialize( byte * pOutputBuffer, const SCFVirtualFolderInfo & pValue )
		{
			return GDSCore::serializeAll( pOutputBuffer,
								 static_cast<const SCFEntryInfo &>( pValue ),
								 pValue.resourcesNum,
								 pValue.subFoldersNum );
		}

		gds_size_t deserialize( const byte * pInputData, SCFVirtualFolderInfo & pValue )
		{
			return GDSCore::deserializeAll( pInputData,
								   static_cast<SCFEntryInfo &>( pValue ),
								   pValue.resourcesNum,
								   pValue.subFoldersNum );
		}

		gds_size_t evalByteSize( const SCFVirtualFolderInfo & pValue )
		{
			return GDSCore::evalByteSizeAll( static_cast<const SCFEntryInfo &>( pValue ),
									pValue.resourcesNum,
									pValue.subFoldersNum );
		}

	}

} // namespace Ic3
