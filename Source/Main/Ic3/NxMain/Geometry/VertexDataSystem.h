
#pragma once

#ifndef __IC3_NXMAIN_VERTEX_DATA_SYSTEM_H__
#define __IC3_NXMAIN_VERTEX_DATA_SYSTEM_H__

#include "VertexPipelineConfig.h"
#include <deque>
#include <unordered_map>

namespace Ic3
{

	using VertexDataLayoutHandle = std::shared_ptr<VertexDataLayoutBase>;

	class IC3_NXMAIN_CLASS VertexDataSystem
	{
	public:
	private:
		using GeometryVertexLayoutStore = std::list<VertexDataLayoutHandle>;
		using GeometryVertexLayoutMap = std::unordered_map<HfsIdentifier, uint32>;

		GeometryVertexLayoutStore _geometryVertexLayoutStore;
		GeometryVertexLayoutMap _geometryVertexLayoutMap;
	};

} // namespace Ic3

#endif // __IC3_NXMAIN_VERTEX_DATA_SYSTEM_H__
