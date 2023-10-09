
#ifndef __IC3_NXMAIN_HARDWARE_GEOMETRY_BUFFER_H__
#define __IC3_NXMAIN_HARDWARE_GEOMETRY_BUFFER_H__

#include "HWBuffer.h"

namespace Ic3
{

	class HWGeometryBuffer : public HWBuffer
	{
	public:
		EGeometryDataType const mGeometryDataType;

		explicit HWGeometryBuffer( EGeometryDataType pGeometryDataType );
		virtual ~HWGeometryBuffer();
	};

	class HWVertexBuffer : public HWGeometryBuffer
	{
	public:
		const HWVertexBufferMetrics & mVBMetrics;

		HWVertexBuffer( const HWVertexBufferMetrics & pVBMetrics );
		virtual ~HWVertexBuffer();

		void setBufferMetrics( const HWVertexBufferMetrics & pVBMetrics );

	private:
		HWVertexBufferMetrics _vbMetrics;
	};

	class HWIndexBuffer : public HWGeometryBuffer
	{
	public:
		const HWIndexBufferMetrics & mIBMetrics;

		HWIndexBuffer();
		virtual ~HWIndexBuffer();

		void setBufferMetrics( const HWIndexBufferMetrics & pIBMetrics );

	private:
		HWIndexBufferMetrics _ibMetrics;
	};

} // namespace Ic3

#endif // __IC3_NXMAIN_HARDWARE_GEOMETRY_BUFFER_H__
