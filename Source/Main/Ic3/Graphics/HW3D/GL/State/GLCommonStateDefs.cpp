
#include "GLCommonStateDefs.h"

namespace Ic3::Graphics::GCI
{

	namespace GCU
	{

		GLenum GetFramebufferDepthStencilAttachmentIDForRTBufferMask( cppx::bitmask<ERenderTargetBufferFlags> pRTBufferMask )
		{
			GLenum framebufferAttachmentID = 0;

			if( pRTBufferMask.is_set( E_RENDER_TARGET_BUFFER_MASK_DEPTH_STENCIL ) )
			{
				framebufferAttachmentID = GL_DEPTH_STENCIL_ATTACHMENT;
			}
			else if( pRTBufferMask.is_set( E_RENDER_TARGET_BUFFER_FLAG_DEPTH_BIT ) )
			{
				framebufferAttachmentID = GL_DEPTH_ATTACHMENT;
			}
			else if( pRTBufferMask.is_set( E_RENDER_TARGET_BUFFER_FLAG_STENCIL_BIT ) )
			{
				framebufferAttachmentID = GL_STENCIL_ATTACHMENT;
			}

			return framebufferAttachmentID;
		}

	}

}
