
#ifndef __IC3_SYSTEM_METAL_COMMON_H__
#define __IC3_SYSTEM_METAL_COMMON_H__

#include "Visual.h"
#include "WindowCommon.h"
#include <objc/objc.h>

namespace Ic3::System
{

	ic3SysDeclareHandle( DisplayManager );
	ic3SysDeclareHandle( MetalDevice );
	ic3SysDeclareHandle( MetalDisplaySurface );
	ic3SysDeclareHandle( MetalSystemDriver );

	/// @brief
	enum EMetalSurfaceCreateFlags : uint32
	{
		/// Surface is created as a fullscreen layer/window, adjusted to the selected display dimensions.
		E_METAL_DISPLAY_SURFACE_CREATE_FLAG_FULLSCREEN_BIT = 0x1000,

		/// Enables adaptive sync for the surface: v-sync is performed only when the frame rate exceeds vertical frequency.
		E_METAL_DISPLAY_SURFACE_CREATE_FLAG_SYNC_ADAPTIVE_BIT = 0x2000,

		/// Disables vertical sync for the surface: buffers are swapped immediately upon request.
		E_METAL_DISPLAY_SURFACE_CREATE_FLAG_SYNC_DISABLED_BIT = 0x4000,

		/// Enables vertical sync for the surface: swap is performed during a v-blank.
		E_METAL_DISPLAY_SURFACE_CREATE_FLAG_SYNC_VERTICAL_BIT = 0x8000,
	};

	struct MetalSystemDriverCreateInfo
	{
	};

}

#endif // __IC3_SYSTEM_METAL_COMMON_H__
