
#pragma once

#ifndef __IC3_GRAPHICS_GCI_COMMON_TYPES_H__
#define __IC3_GRAPHICS_GCI_COMMON_TYPES_H__

#include <cxm/color.h>
#include <cxm/vector.h>

namespace Ic3::Graphics::GCI
{

	struct ViewportDesc
	{
		struct DepthRange
		{
			float zNear;
			float zFar;
		};

		cxm::vec2u32 origin;
		cxm::size2u size;
		DepthRange depthRange;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_COMMON_TYPES_H__
