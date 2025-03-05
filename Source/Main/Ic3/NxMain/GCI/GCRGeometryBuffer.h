
#ifndef __IC3_NXMAIN_HARDWARE_GEOMETRY_BUFFER_H__
#define __IC3_NXMAIN_HARDWARE_GEOMETRY_BUFFER_H__

#include "GCRBuffer.h"

namespace Ic3
{

	class GCRGeometryBuffer : public GCRBuffer
	{
	public:
		EGeometryDataType const mGeometryDataType;

		explicit GCRGeometryBuffer( EGeometryDataType pGeometryDataType );
		virtual ~GCRGeometryBuffer();
	};

	class GCRVertexBuffer : public GCRGeometryBuffer
	{
	public:
		const GCRVertexBufferMetrics & mVBMetrics;

		GCRVertexBuffer( const GCRVertexBufferMetrics & pVBMetrics );
		virtual ~GCRVertexBuffer();

		void setBufferMetrics( const GCRVertexBufferMetrics & pVBMetrics );

	private:
		GCRVertexBufferMetrics _vbMetrics;
	};

	class GCRIndexBuffer : public GCRGeometryBuffer
	{
	public:
		const GCRIndexBufferMetrics & mIBMetrics;

		GCRIndexBuffer();
		virtual ~GCRIndexBuffer();

		void setBufferMetrics( const GCRIndexBufferMetrics & pIBMetrics );

	private:
		GCRIndexBufferMetrics _ibMetrics;
	};

} // namespace Ic3

#endif // __IC3_NXMAIN_HARDWARE_GEOMETRY_BUFFER_H__
