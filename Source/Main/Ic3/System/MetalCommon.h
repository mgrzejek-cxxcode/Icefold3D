
#ifndef __IC3_SYSTEM_METAL_COMMON_H__
#define __IC3_SYSTEM_METAL_COMMON_H__

#include "Visual.h"
#include "WindowCommon.h"
#include <objc/objc.h>

namespace Ic3::System
{

	Ic3SysDeclareHandle( DisplayManager );
	Ic3SysDeclareHandle( MetalDevice );
	Ic3SysDeclareHandle( MetalDisplaySurface );
	Ic3SysDeclareHandle( MetalSystemDriver );

	/// @brief
	enum EMetalSurfaceCreateFlags : uint32
	{
		/// Surface is created as a fullscreen layer/window, adjusted to the selected display dimensions.
		eMetalDisplaySurfaceCreateFlagFullscreenBit = 0x1000,

		/// Enables adaptive sync for the surface: v-sync is performed only when the frame rate exceeds vertical frequency.
		eMetalDisplaySurfaceCreateFlagSyncAdaptiveBit = 0x2000,

		/// Disables vertical sync for the surface: buffers are swapped immediately upon request.
		eMetalDisplaySurfaceCreateFlagSyncDisabledBit = 0x4000,

		/// Enables vertical sync for the surface: swap is performed during a v-blank.
		eMetalDisplaySurfaceCreateFlagSyncVerticalBit = 0x8000,
	};

	struct MetalSystemDriverCreateInfo
	{
	};

}

#endif // __IC3_SYSTEM_METAL_COMMON_H__
