
#pragma once

#ifndef __IC3_NXMAIN_SCF_INDEX_BUILDER_H__
#define __IC3_NXMAIN_SCF_INDEX_BUILDER_H__

#include "scfIndex.h"
#include <cppx/byteArray.h>
#include <cppx/sortedArray.h>
#include <Ic3/System/IO/FileCommon.h>

namespace Ic3
{

	struct SCFInputDataSource
	{
		using DataReadCallback = std::function<uint64( uint64 /* pOffset */, uint64 /* pReadSize */, Dynamicbyte_array & /* pBuffer */ )>;

		uint64 byteSize = 0;

		DataReadCallback readCallback;

		explicit operator bool() const
		{
			return ( byteSize > 0 ) && readCallback;
		}

		static SCFInputDataSource asPlaceholder( uint64 pDataSize );

		static SCFInputDataSource fromFile( System::FileManagerHandle pSysFileManager, const std::string & pFilename );

		static SCFInputDataSource fromMemory( read_only_memory_view pMemoryView );

		template <typename TCallback, typename... TArgs>
		static SCFInputDataSource bindGenericCallback( TCallback pCallback, TArgs && ...pArgs )
		{
			return std::bind( pCallback,
			                  std::placeholders::_1,
			                  std::placeholders::_2,
			                  std::placeholders::_3,
			                  std::forward<TArgs>( pArgs )... );
		}
	};

	struct SCFEntryTemplate
	{
		std::string name;
		std::string path;
		std::string uid;
		uint32 treeSubLevel = 0;
	};

	struct SCFResourceTemplate : public SCFEntryTemplate
	{
		SCFInputDataSource dataSource;
	};

	struct SCFVirtualFolderTemplate : public SCFEntryTemplate
	{
		SortedArray<SCFResourceTemplate> resourceList;
		SortedArray<SCFVirtualFolderTemplate> subFolderList;
	};

	inline const std::string cvSCFEntryUIDEmpty {};

	class SCFIndexBuilder
	{
	public:
		SCFIndexBuilder();
		~SCFIndexBuilder();

		const SCFVirtualFolderTemplate * addFolder( const SCFVirtualFolderTemplate * pParentFolder,
													std::string pFolderName,
													std::string pUID = cvSCFEntryUIDEmpty );

		const SCFVirtualFolderTemplate * addFolder( const std::string & pParentLocation,
													std::string pFolderName,
													std::string pUID = cvSCFEntryUIDEmpty );

		const SCFResourceTemplate * addResource( const SCFVirtualFolderTemplate * pParentFolder,
												 std::string pResourceName,
												 SCFInputDataSource pDataSource,
												 std::string pUID = cvSCFEntryUIDEmpty );

		const SCFResourceTemplate * addResource( const std::string & pParentLocation,
												 std::string pResourceName,
												 SCFInputDataSource pDataSource,
												 std::string pUID = cvSCFEntryUIDEmpty );

		bool removeEntry(  const std::string & pLocation );

		const SCFVirtualFolderTemplate & getRootVirtualFolder() const;

		bool checkNameAndUID( const std::string & pName, const std::string & pUID ) const;

	private:
		SCFVirtualFolderTemplate * _findFolderInternal( const std::string & pFolderLocation,
														SCFVirtualFolderTemplate * pRootFolder = nullptr );

		SCFResourceTemplate * _findResourceInternal( const std::string & pResourceLocation,
													 SCFVirtualFolderTemplate * pRootFolder = nullptr );

	private:
		struct PrivateWorkingData;
		std::unique_ptr<SCFIndex> _fileIndex;
		std::unique_ptr<PrivateWorkingData> _privateWorkingData;
	};

} // namespace Ic3

#endif // __IC3_NXMAIN_SCF_INDEX_BUILDER_H__
