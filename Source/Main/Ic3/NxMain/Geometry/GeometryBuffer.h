
#pragma once

#ifndef __IC3_NXMAIN_GEOMETRY_BUFFER_H__
#define __IC3_NXMAIN_GEOMETRY_BUFFER_H__

#include "GeometryCommonDefs.h"
#include <Ic3/Graphics/GCI/Resources/GPUBufferCommon.h>

namespace Ic3
{

	struct GeometryBufferMetrics
	{
		uint32 elementSize;
		uint32 dataSizeInElementsNum;
	};

	class GeometryBuffer
	{
	public:
		GeometryBufferMetrics const mBufferMetrics;

	public:
		void lock();
		void unlock();

	private:
		GCI::GPUBufferReference _gpaBufferRef;
	};

	class IndexBuffer : public GeometryBuffer
	{
	};

	class VertexBuffer : public GeometryBuffer
	{
	};

} // namespace Ic3

#endif // __IC3_NXMAIN_GEOMETRY_BUFFER_H__
