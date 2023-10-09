
#pragma once

#ifndef __IC3_NXMAIN_GEOMETRY_DATA_SOURCE_H__
#define __IC3_NXMAIN_GEOMETRY_DATA_SOURCE_H__

#include "GeometryVertexFormat.h"

namespace Ic3
{

	struct GeometryDataMetrics
	{
		uint32 indexDataSizeInElementsNum;
		uint32 vertexDataSizeInElementsNum;
	};

	class GeometryDataSource
	{
	public:
		GeometryDataSource(
				uint32 pVertexDataCapacityInElementsNum
				);

		~GeometryDataSource();

		void initializeAttributeStorage(
				VertexAttributeSemantics pAttribute,
				uint32 pElementStrideInBytes );

	private:
		struct DataCapacityInfo
		{
		};

		struct IndexDataBufferInfo
		{
			uint32 indexElementSizeInBytes;
			uint32 dataSizeInBytes;
			DynamicMemoryBuffer buffer;
		};

		struct VertexDataBufferInfo
		{
			uint32 elementStrideInBytes;
			uint32 dataSizeInBytes;
			DynamicMemoryBuffer buffer;
		};

		struct VertexAttributeDataInfo
		{
			uint16 bufferIndex;
			uint16 elementSizeInBytes;
			uint32 baseOffset;
			GCI::EVertexAttribFormat dataFormat;
		};

		using VertexDataBufferArray = std::vector<VertexDataBufferInfo>;
		using AttributeList = std::list<VertexAttributeSemantics>;
		using AttributeDataMap = std::unordered_map<std::string_view, VertexAttributeDataInfo>;

	private:
		uint32
	};

} // namespace Ic3

#endif // __IC3_NXMAIN_GEOMETRY_DATA_SOURCE_H__
