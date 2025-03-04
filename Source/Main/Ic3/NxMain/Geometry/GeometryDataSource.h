
#pragma once

#ifndef __IC3_NXMAIN_GEOMETRY_DATA_SOURCE_H__
#define __IC3_NXMAIN_GEOMETRY_DATA_SOURCE_H__

#include "GeometryCommonDefs.h"
#include <cppx/stdHelperAlgo.h>

#include <deque>

namespace Ic3
{

	class GeometryInputDataSource;

	using geometry_input_ref_id_t = uint32;
	
	struct GeometryInputDataRef
	{
		using VertexAttributeDataRefArray = std::vector<GeometryInputElementSubDataRef>;
		GeometryInputElementSubDataRef indexDataRef;
		VertexAttributeDataRefArray vertexDataRefArray;
	};

	struct GeometryInputReference
	{
	};

	struct GeometryInputDataLocalRef
	{
		GeometryInputDataSource * source = nullptr;
		uint32 geometryIndex = 0;
		uint32 indexBufferOffsetInElementsNum = 0;
		uint32 vertexBufferOffsetInElementsNum = 0;
		uint32 indexElementsNum = 0;
		uint32 vertexElementsNum = 0;
	};

	class IC3_NXMAIN_CLASS IGeometryInputDataSource
	{
	public:
		IGeometryInputDataSource() = default;
		~IGeometryInputDataSource() = default;

		virtual geometry_input_ref_id_t appendGeometry( uint32 pVertexElementsNum, uint32 pIndexElementsNum ) noexcept;

	private:
	};

	class GeometryInputDataSourceExternal : public IGeometryInputDataSource
	{
	public:
		void setVertexData(
				geometry_input_ref_id_t pGeometryRefID,
				VertexAttributeShaderSemantics pVertexAttribSemantics,
				const void * pVertexAttribDataPtr,
				size_t pAttributeSize );

		void setIndexData( geometry_input_ref_id_t pGeometryRefID, const void * pIndexDataPtr, size_t pIndexElementSize );
	};

	class GeometryInputDataSourceManaged : public IGeometryInputDataSource
	{
	public:
		CPPX_ATTR_NO_DISCARD GeometrySubDataRefReadWrite setVertexData(
				geometry_input_ref_id_t pGeometryRefID,
				VertexAttributeShaderSemantics pVertexAttribSemantics );

		CPPX_ATTR_NO_DISCARD GeometrySubDataRefReadWrite setIndexData( geometry_input_ref_id_t pGeometryRefID );
	};

	class IC3_NXMAIN_CLASS XXGeometryInputSourceBase
	{
	public:
		XXGeometryInputSourceBase();
		~XXGeometryInputSourceBase();

		CPPX_ATTR_NO_DISCARD GeometrySize getCurrentCapacity() const noexcept;
		CPPX_ATTR_NO_DISCARD bool isIndexedGeometry() const noexcept;

		void addIndexData( GCI::EIndexDataFormat pIndexFormat );

		virtual ResultCode buildInputLayout() = 0;

		virtual geometry_input_ref_id_t appendGeometry( uint32 pVertexElementsNum, uint32 pIndexElementsNum ) = 0;

	private:
		struct AllocState
		{
			uint32 indexElementsCapacity = 0;
			uint32 vertexElementsCapacity = 0;
			uint32 indexElementsAllocOffset = 0;
			uint32 vertexElementsAllocOffset = 0;

			CPPX_ATTR_NO_DISCARD bool empty() const noexcept
			{
				return ( indexElementsAllocOffset == 0 ) && ( vertexElementsAllocOffset == 0 );
			}
		};

		struct LocalIndexBuffer
		{
			GCI::EIndexDataFormat indexElementFormat = GCI::EIndexDataFormat::Undefined;
			uint16 elementSizeInBytes = 0;
			uint32 bufferCapacityInBytes = 0;
			dynamic_memory_buffer buffer;

			CPPX_ATTR_NO_DISCARD bool active() const noexcept
			{
				return elementSizeInBytes > 0;
			}
		};

		struct LocalVertexBuffer
		{
			uint32 elementSizeInBytes = 0;
			uint32 bufferCapacityInBytes = 0;
			dynamic_memory_buffer buffer;

			CPPX_ATTR_NO_DISCARD bool active() const noexcept
			{
				return elementSizeInBytes > 0;
			}
		};

		using AttributeMap = std::unordered_map<std::string, SGeometryInputAttributeInfo>;
		using LocalVertexBufferArray = std::array<LocalVertexBuffer, GCM::kIAMaxDataStreamVertexBuffersNum>;
		using GeometryRefList = std::deque<GeometryInputDataLocation>;

	private:
		AllocState _allocState;
		LocalIndexBuffer _localIndexBuffer;
		LocalVertexBufferArray _localVertexBufferArray;
		GeometryRefList _geometryRefList;
	};

	inline GeometrySize XXGeometryInputSourceBase::getCurrentCapacity() const noexcept
	{
		return {
			_allocState.indexElementsCapacity - _allocState.indexElementsAllocOffset,
			_allocState.vertexElementsCapacity - _allocState.vertexElementsAllocOffset
		};
	}

	inline bool XXGeometryInputSourceBase::isIndexedGeometry() const noexcept
	{
		return _localIndexBuffer.active();
	}

} // namespace Ic3

#endif // __IC3_NXMAIN_GEOMETRY_DATA_SOURCE_H__
