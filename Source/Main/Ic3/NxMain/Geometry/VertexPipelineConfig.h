
#pragma once

#ifndef __IC3_NXMAIN_VERTEX_INPUT_LAYOUT_H__
#define __IC3_NXMAIN_VERTEX_INPUT_LAYOUT_H__

#include "GeometryCommonDefs.h"
#include "../GCI/VertexPipelineLayouts.h"

namespace Ic3
{

	class SVertexPipelineConfig
	{
		CVertexDataLayout dataLayout;

		GCI::IAInputLayoutImmutableStateHandle gciInputLayoutDefault;

		GCI::IAInputLayoutImmutableStateHandle gciInputLayoutPosition;

		GCI::IAInputLayoutImmutableStateHandle gciInputLayoutPositionNormal;
	};

} // namespace Ic3

#endif // __IC3_NXMAIN_VERTEX_INPUT_LAYOUT_H__
