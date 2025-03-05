
#pragma once

#ifndef __IC3_NXMAIN_GEOMETRY_STORAGE_CPU_H__
#define __IC3_NXMAIN_GEOMETRY_STORAGE_CPU_H__

#include "GeometryStorage.h"

namespace Ic3
{

	class IC3_NXMAIN_CLASS GeometryDataStorageCPUExternal : public IGeometryDataStorageCPU
	{
	public:
		virtual bool setIndexBufferMemory( void * pBufferMemory, size_t pBufferSizeInBytes ) = 0;

		virtual bool setVertexBufferMemory( size_t pBufferIndex, void * pBufferMemory, size_t pBufferSizeInBytes ) = 0;

		virtual void resetIndexBufferMemory() = 0;

		virtual void resetVertexBufferMemory( size_t pBufferIndex ) = 0;
	};

	class IC3_NXMAIN_CLASS GeometryDataStorageCPUManaged : public IGeometryDataStorageCPU
	{
	public:
		virtual bool initializeIndexBuffer( size_t pBufferSizeInBytes, uint32 pAlignment ) = 0;

		virtual bool initializeVertexBuffer( size_t pBufferIndex, size_t pBufferSizeInBytes, uint32 pAlignment ) = 0;

		virtual void releaseIndexBuffer() = 0;

		virtual void releaseVertexBuffer( size_t pBufferIndex ) = 0;
	};

} // namespace Ic3

#endif // __IC3_NXMAIN_GEOMETRY_STORAGE_CPU_H__
