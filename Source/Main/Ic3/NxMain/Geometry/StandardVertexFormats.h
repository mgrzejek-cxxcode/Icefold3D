
#pragma once

#ifndef __IC3_NXMAIN_STANDARD_VERTEX_FORMATS_H__
#define __IC3_NXMAIN_STANDARD_VERTEX_FORMATS_H__

#include "GeometryVertexFormat.h"

namespace Ic3
{

	enum class EStandardVertexFormatID : enum_default_value_t
	{
		VF2DDefault = 1,
		VF3DDefaultTex1 = 2,
		VF3DDefaultTex4 = 3,
		VF3DDefaultInstance1 = 4
	};

	struct SVF2DDefault
	{
		Math::Vec3f position;
	};

} // namespace Ic3

#endif // __IC3_NXMAIN_STANDARD_VERTEX_FORMATS_H__
