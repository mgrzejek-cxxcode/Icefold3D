
#pragma once

#ifndef __IC3_NXMAIN_GEOMETRY_BUFFER_H__
#define __IC3_NXMAIN_GEOMETRY_BUFFER_H__

#include "GeometryCommonDefs.h"
#include "../GCI/HardwareBuffer.h"

namespace Ic3
{

	struct GeometryBufferMetrics
	{
		uint32 elementSize;
		uint32 dataSizeInElementsNum;
	};

	using VertexBufferMetrics = GeometryBufferMetrics;
	using IndexBufferMetrics = GeometryBufferMetrics;

	class GeometryBuffer : public HardwareBuffer
	{
	public:
		CPPX_ATTR_NO_DISCARD virtual const GeometryBufferMetrics & getMetrics() const noexcept;

		template <typename TMetrics>
		CPPX_ATTR_NO_DISCARD const TMetrics & getMetricsAs() const noexcept;

		/// Concept:
		/// Geometry buffers can be shared across multiple GeometryStorage instances, however, the used regions cannot
		/// overlap (obvious).
		///
		/// We need to allow creation of a GeometryStorage where each buffer can be either created (CreateInfo) or
		/// shared from an existing GeometryBuffer.
		/// This functionality should be added here - underlying GPUBuffers shouldn't know anything about this.
		///
		/// Idea:
		/// Upon creation with shared buffers, GeometryStorage does:
		///
		/// sharedBuffer->setReservedRegion( ... );
		///
		/// GeometryBuffer::setReservedRegion( pRegion )
		/// {
		///   if( regionNotReserved( pRegion ) )
		///     _reservedRegions.add( pRegion );
		/// }
		virtual ResultCode setReservedRegion( const MemoryRegion & pReservedRegion );
	};

	class IndexBuffer : public GeometryBuffer
	{
	};

	class VertexBuffer : public GeometryBuffer
	{
	};

} // namespace Ic3

#endif // __IC3_NXMAIN_GEOMETRY_BUFFER_H__
