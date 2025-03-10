
#include "gridGenerator.h"

namespace Ic3
{

	GridGeometry GridGenerator::generate(
			const cxm::vec3f & pOrigin,
			const cxm::vec2u32 & pGridSize,
			const cxm::vec2f & pAdvance,
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
			const cxm::vec3f& pOrigin,
			const cxm::vec2u32& pGridSize,
			const cxm::vec2f & pAdvance,
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
			const cxm::vec3f & pOrigin,
			const cxm::vec2u32 & pGridSize,
			const cxm::vec2f & pAdvance,
			GridGeometry * pGeometry)
	{
	}

	void GridGenerator::_generateVerticesNoIndexClockwise(
			const cxm::vec3f & pOrigin,
			const cxm::vec2u32 & pGridSize,
			const cxm::vec2f & pAdvance,
			GridGeometry * pGeometry)
	{
	}

	void GridGenerator::_generateVerticesNoIndexCounterClockwise(
			const cxm::vec3f & pOrigin,
			const cxm::vec2u32 & pGridSize,
			const cxm::vec2f & pAdvance,
			GridGeometry * pGeometry)
	{
	}

	void GridGenerator::_generateIndicesClockwise(
			const cxm::vec2u32 & pGridSize,
			GridGeometry * pGeometry)
	{
	}

	void GridGenerator::_generateIndicesCounterClockwise(
			const cxm::vec2u32 & pGridSize,
			GridGeometry * pGeometry)
	{
	}

}
