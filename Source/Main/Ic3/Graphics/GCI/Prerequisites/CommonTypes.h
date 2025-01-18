
#pragma once

#ifndef __IC3_GRAPHICS_GCI_COMMON_TYPES_H__
#define __IC3_GRAPHICS_GCI_COMMON_TYPES_H__

#include <Ic3/Math/Color.h>
#include <Ic3/Math/Vector.h>

namespace Ic3::Graphics::GCI
{

	struct ViewportDesc
	{
		struct DepthRange
		{
			float zNear;
			float zFar;
		};

		Math::Vec2u32 origin;
		Math::Size2u size;
		DepthRange depthRange;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_COMMON_TYPES_H__
