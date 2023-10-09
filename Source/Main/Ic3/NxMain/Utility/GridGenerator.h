
#pragma once

#ifndef __IC3_NXMAIN_GRID_GENERATOR_H__
#define __IC3_NXMAIN_GRID_GENERATOR_H__

#include "../Prerequisites.h"
#include <Ic3/Graphics/GCI/State/commonGPUStateDefs.h>
#include <Ic3/Math/Vector.h>
#include <Ic3/Cppx/MemoryBuffer.h>

namespace Ic3
{

	struct GridGeometry
	{
		using VertexType = Math::Vec3f;
		using IndexType = uint32;

		DynamicMemoryBuffer vertices;

		DynamicMemoryBuffer indices;

		native_uint verticesNum;

		native_uint indicesNum;

		GCI::ETriangleVerticesOrder verticesOrder;
	};

	class GridGenerator
	{
	public:
		static GridGeometry generate(
				const Math::Vec3f & pOrigin,
				const Math::Vec2u32 & pGridSize,
				const Math::Vec2f & pAdvance,
				GCI::ETriangleVerticesOrder pVerticesOrder = GCI::ETriangleVerticesOrder::CounterClockwise);

		static GridGeometry generateIndexed(
				const Math::Vec3f& pOrigin,
				const Math::Vec2u32& pGridSize,
				const Math::Vec2f & pAdvance,
				GCI::ETriangleVerticesOrder pVerticesOrder = GCI::ETriangleVerticesOrder::CounterClockwise);

	private:
		static void _generateVerticesIndexed(
				const Math::Vec3f & pOrigin,
				const Math::Vec2u32 & pGridSize,
				const Math::Vec2f & pAdvance,
				GridGeometry * pGeometry);

		static void _generateVerticesNoIndexClockwise(
				const Math::Vec3f & pOrigin,
				const Math::Vec2u32 & pGridSize,
				const Math::Vec2f & pAdvance,
				GridGeometry * pGeometry);

		static void _generateVerticesNoIndexCounterClockwise(
				const Math::Vec3f & pOrigin,
				const Math::Vec2u32 & pGridSize,
				const Math::Vec2f & pAdvance,
				GridGeometry * pGeometry);

		static void _generateIndicesClockwise(
				const Math::Vec2u32 & pGridSize,
				GridGeometry * pGeometry);

		static void _generateIndicesCounterClockwise(
				const Math::Vec2u32 & pGridSize,
				GridGeometry * pGeometry);
	};

} // namespace Ic3

#endif // __IC3_NXMAIN_GRID_GENERATOR_H__
