
#ifndef __IC3_GRAPHICS_HW3D_GLC_COMMON_STATE_DEFS_H__
#define __IC3_GRAPHICS_HW3D_GLC_COMMON_STATE_DEFS_H__

#include "../GLPrerequisites.h"
#include <Ic3/Graphics/GCI/State/RenderTargetCommon.h>

namespace Ic3::Graphics::GCI
{

	namespace Utilities
	{

		CPPX_ATTR_NO_DISCARD GLenum RTOGetGLFramebufferDepthStencilAttachmentIDForRTBufferMask(
				cppx::bitmask<ERenderTargetBufferFlags> pRTBufferMask );

	}

}

#endif // __IC3_GRAPHICS_HW3D_GLC_COMMON_STATE_DEFS_H__
