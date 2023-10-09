
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
		FrameGeometry frameGeometry = CX_FRAME_GEOMETRY_DEFAULT;

		std::string title;

		/// Visual config for the window. Describes primary rendering properties of the surface.
		VisualConfig visualConfig;
	};

} // namespace Ic3::System

#endif // __IC3_SYSTEM_WINDOW_COMMON_H__
