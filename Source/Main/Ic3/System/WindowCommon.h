
#ifndef __IC3_SYSTEM_WINDOW_COMMON_H__
#define __IC3_SYSTEM_WINDOW_COMMON_H__

#include "Frame.h"
#include "Visual.h"

namespace Ic3::System
{

    ic3SysDeclareHandle( Window );
    ic3SysDeclareHandle( WindowManager );

	struct WindowCreateInfo
	{
		/// Geometry of the window's frame (position, size, style, etc).
		FrameGeometry mFrameGeometry = CX_FRAME_GEOMETRY_DEFAULT;

		/// Window title (text visible on the title bar - if present).
		std::string mTitle;

		/// Visual config for the window. Describes primary rendering properties of the surface.
		VisualConfig mVisualConfig;
	};

} // namespace Ic3::System

#endif // __IC3_SYSTEM_WINDOW_COMMON_H__
