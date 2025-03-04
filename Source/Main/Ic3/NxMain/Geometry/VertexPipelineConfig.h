
#pragma once

#ifndef __IC3_NXMAIN_VERTEX_INPUT_LAYOUT_H__
#define __IC3_NXMAIN_VERTEX_INPUT_LAYOUT_H__

#include "GeometryCommonDefs.h"
#include "../GCI/VertexFormatSignature.h"

namespace Ic3
{

	class VertexPipelineConfig
	{
		TSharedHandle<VertexFormatSignature> dataLayout;

		GCI::IAInputLayoutCompiledStateHandle gciInputLayoutDefault;

		GCI::IAInputLayoutCompiledStateHandle gciInputLayoutPosition;

		GCI::IAInputLayoutCompiledStateHandle gciInputLayoutPositionNormal;
	};

} // namespace Ic3

#endif // __IC3_NXMAIN_VERTEX_INPUT_LAYOUT_H__
