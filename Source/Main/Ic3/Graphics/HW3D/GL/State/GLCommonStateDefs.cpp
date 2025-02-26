
#include "GLCommonStateDefs.h"

namespace Ic3::Graphics::GCI
{

	namespace GCU
	{

		GLenum RTOGetGLFramebufferDepthStencilAttachmentIDForRTBufferMask( cppx::bitmask<ERenderTargetBufferFlags> pRTBufferMask )
		{
			GLenum framebufferAttachmentID = 0;

			if( pRTBufferMask.is_set( eRenderTargetBufferMaskDepthStencil ) )
			{
				framebufferAttachmentID = GL_DEPTH_STENCIL_ATTACHMENT;
			}
			else if( pRTBufferMask.is_set( eRenderTargetBufferFlagDepthBit ) )
			{
				framebufferAttachmentID = GL_DEPTH_ATTACHMENT;
			}
			else if( pRTBufferMask.is_set( eRenderTargetBufferFlagStencilBit ) )
			{
				framebufferAttachmentID = GL_STENCIL_ATTACHMENT;
			}

			return framebufferAttachmentID;
		}

	}

}
