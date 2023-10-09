
#include "gridGenerator.h"

namespace Ic3
{

	GridGeometry GridGenerator::generate(
			const Math::Vec3f & pOrigin,
			const Math::Vec2u32 & pGridSize,
			const Math::Vec2f & pAdvance,
			GCI::ETriangleVerticesOrder pVerticesOrder )
	{
		GridGeometry gridGeometry;
		gridGeometry.indicesNum = 0;
		gridGeometry.verticesNum = 0;

		if( pVerticesOrder == GCI::ETriangleVerticesOrder::Clockwise )
		{
			_generateVerticesNoIndexClockwise( pOrigin, pGridSize, pAdvance, &gridGeometry );
			gridGeometry.verticesOrder = GCI::ETriangleVerticesOrder::Clockwise;
		}
		else
		{
			_generateVerticesNoIndexCounterClockwise( pOrigin, pGridSize, pAdvance, &gridGeometry );
			gridGeometry.verticesOrder = GCI::ETriangleVerticesOrder::CounterClockwise;
		}

		return gridGeometry;
	}

	GridGeometry GridGenerator::generateIndexed(
			const Math::Vec3f& pOrigin,
			const Math::Vec2u32& pGridSize,
			const Math::Vec2f & pAdvance,
			GCI::ETriangleVerticesOrder pVerticesOrder )
	{
		GridGeometry gridGeometry;
		gridGeometry.indicesNum = 0;
		gridGeometry.verticesNum = 0;

		if( pVerticesOrder == GCI::ETriangleVerticesOrder::Clockwise )
		{
			_generateVerticesIndexed( pOrigin, pGridSize, pAdvance, &gridGeometry );
			_generateIndicesClockwise( pGridSize, &gridGeometry );
			gridGeometry.verticesOrder = GCI::ETriangleVerticesOrder::Clockwise;
		}
		else
		{
			_generateVerticesIndexed( pOrigin, pGridSize, pAdvance, &gridGeometry );
			_generateIndicesCounterClockwise( pGridSize, &gridGeometry );
			gridGeometry.verticesOrder = GCI::ETriangleVerticesOrder::CounterClockwise;
		}

		return gridGeometry;
	}

	void GridGenerator::_generateVerticesIndexed(
			const Math::Vec3f & pOrigin,
			const Math::Vec2u32 & pGridSize,
			const Math::Vec2f & pAdvance,
			GridGeometry * pGeometry)
	{
	}

	void GridGenerator::_generateVerticesNoIndexClockwise(
			const Math::Vec3f & pOrigin,
			const Math::Vec2u32 & pGridSize,
			const Math::Vec2f & pAdvance,
			GridGeometry * pGeometry)
	{
	}

	void GridGenerator::_generateVerticesNoIndexCounterClockwise(
			const Math::Vec3f & pOrigin,
			const Math::Vec2u32 & pGridSize,
			const Math::Vec2f & pAdvance,
			GridGeometry * pGeometry)
	{
	}

	void GridGenerator::_generateIndicesClockwise(
			const Math::Vec2u32 & pGridSize,
			GridGeometry * pGeometry)
	{
	}

	void GridGenerator::_generateIndicesCounterClockwise(
			const Math::Vec2u32 & pGridSize,
			GridGeometry * pGeometry)
	{
	}

}
