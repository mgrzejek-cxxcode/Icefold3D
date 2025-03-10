
#pragma once

#ifndef __IC3_NXMAIN_GRID_GENERATOR_H__
#define __IC3_NXMAIN_GRID_GENERATOR_H__

#include "../Prerequisites.h"
#include <Ic3/Graphics/GCI/State/commonGPUStateDefs.h>
#include <cxm/vector.h>
#include <cppx/memoryBuffer.h>

namespace Ic3
{

	struct GridGeometry
	{
		using VertexType = cxm::vec3f;
		using IndexType = uint32;

		dynamic_memory_buffer vertices;

		dynamic_memory_buffer indices;

		native_uint verticesNum;

		native_uint indicesNum;

		GCI::ETriangleVerticesOrder verticesOrder;
	};

	class GridGenerator
	{
	public:
		static GridGeometry generate(
				const cxm::vec3f & pOrigin,
				const cxm::vec2u32 & pGridSize,
				const cxm::vec2f & pAdvance,
				GCI::ETriangleVerticesOrder pVerticesOrder = GCI::ETriangleVerticesOrder::CounterClockwise);

		static GridGeometry generateIndexed(
				const cxm::vec3f& pOrigin,
				const cxm::vec2u32& pGridSize,
				const cxm::vec2f & pAdvance,
				GCI::ETriangleVerticesOrder pVerticesOrder = GCI::ETriangleVerticesOrder::CounterClockwise);

	private:
		static void _generateVerticesIndexed(
				const cxm::vec3f & pOrigin,
				const cxm::vec2u32 & pGridSize,
				const cxm::vec2f & pAdvance,
				GridGeometry * pGeometry);

		static void _generateVerticesNoIndexClockwise(
				const cxm::vec3f & pOrigin,
				const cxm::vec2u32 & pGridSize,
				const cxm::vec2f & pAdvance,
				GridGeometry * pGeometry);

		static void _generateVerticesNoIndexCounterClockwise(
				const cxm::vec3f & pOrigin,
				const cxm::vec2u32 & pGridSize,
				const cxm::vec2f & pAdvance,
				GridGeometry * pGeometry);

		static void _generateIndicesClockwise(
				const cxm::vec2u32 & pGridSize,
				GridGeometry * pGeometry);

		static void _generateIndicesCounterClockwise(
				const cxm::vec2u32 & pGridSize,
				GridGeometry * pGeometry);
	};

} // namespace Ic3

#endif // __IC3_NXMAIN_GRID_GENERATOR_H__
