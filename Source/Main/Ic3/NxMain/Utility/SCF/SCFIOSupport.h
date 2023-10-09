
#pragma once

#ifndef __IC3_NXMAIN_SCF_IO_SUPPORT_H__
#define __IC3_NXMAIN_SCF_IO_SUPPORT_H__

#include "scfCommon.h"
#include <Ic3/Cppx/ByteArray.h>
#include <Ic3/System/FileCommon.h>

namespace Ic3
{

	struct SCFResourceTemplate;
	struct SCFVirtualFolderTemplate;

	class SCFIndex;
	class SCFIndexBuilder;

	class SCFIOProxy
	{
	public:
		SCFIOProxy( System::FileManagerHandle pSysFileManager );

		void saveIndex( const std::string & pFilename, const SCFIndexBuilder & pBuilder );

		void loadIndex( const std::string & pFilename, SCFIndex & pIndex );

	private:
		using InternalFileReadCallback = std::function<uint64( void *, uint64 )>;
		using InternalFileWriteCallback = std::function<uint64( const void *, uint64 )>;

		void writeFolderData( System::FileHandle pSysFile,
		                      const SCFVirtualFolderTemplate & pFolder,
		                      DynamicByteArray & pGdsCache,
		                      const InternalFileWriteCallback & pFileWriteCallback );

		void writeResourceData( System::FileHandle pSysFile,
		                        const SCFResourceTemplate & pResource,
		                        DynamicByteArray & pGdsCache,
		                        const InternalFileWriteCallback & pFileWriteCallback );

		void readFolder( System::FileHandle pSysFile,
		                 SCFVirtualFolder & pFolder,
		                 DynamicByteArray & pGdsCache,
		                 const InternalFileReadCallback & pFileReadCallback );

	private:
		System::FileManagerHandle _sysFileManager;
	};

	namespace GDS
	{

		gds_size_t serialize( byte * pOutputBuffer, const SCFEntryInfo & pValue );
		gds_size_t deserialize( const byte * pInputDesc, SCFEntryInfo & pValue );
		gds_size_t evalByteSize( const SCFEntryInfo & pValue );

		gds_size_t serialize( byte * pOutputBuffer, const SCFResourceInfo & pValue );
		gds_size_t deserialize( const byte * pInputDesc, SCFResourceInfo & pValue );
		gds_size_t evalByteSize( const SCFResourceInfo & pValue );

		gds_size_t serialize( byte * pOutputBuffer, const SCFVirtualFolderInfo & pValue );
		gds_size_t deserialize( const byte * pInputDesc, SCFVirtualFolderInfo & pValue );
		gds_size_t evalByteSize( const SCFVirtualFolderInfo & pValue );

	}

} // namespace Ic3

#endif // __IC3_NXMAIN_SCF_IO_SUPPORT_H__
