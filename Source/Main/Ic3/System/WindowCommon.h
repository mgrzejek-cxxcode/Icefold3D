
#ifndef __IC3_SYSTEM_WINDOW_COMMON_H__
#define __IC3_SYSTEM_WINDOW_COMMON_H__

#include "Frame.h"
#include "Visual.h"

namespace Ic3::System
{

    Ic3SysDeclareHandle( Window );
    Ic3SysDeclareHandle( WindowManager );

	struct WindowCreateInfo
	{
		/// Geometry of the window's frame (position, size, style, etc).
		FrameGeometry frameGeometry = CX_FRAME_GEOMETRY_DEFAULT;

		/// Window title (text visible on the title bar - if present).
		std::string title;

		/// Visual config for the window. Describes primary rendering properties of the surface.
		VisualConfig visualConfig;
	};

} // namespace Ic3::System

#endif // __IC3_SYSTEM_WINDOW_COMMON_H__
