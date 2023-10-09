
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_GLCOMMON_FRAMEBUFFER_OBJECT_H__
#define __IC3_GRAPHICS_HW3D_GLCOMMON_FRAMEBUFFER_OBJECT_H__

#include "../GLPrerequisites.h"
#include <Ic3/Graphics/GCI/Resources/TextureCommon.h>

namespace Ic3::Graphics::GCI
{

	class GLRenderbufferObject;
	class GLTextureObject;

	ic3GLDeclareOpenGLObjectHandle( GLFramebufferObject );

	class GLFramebufferObject : public GLObject
	{
	public:
		GLFramebufferObject( GLuint pHandle );
		virtual ~GLFramebufferObject();

		virtual bool release();
		virtual bool validateHandle() const;

		void bindColorRenderbuffer(
				uint32 pColorAttachmentIndex,
				GLRenderbufferObject & pGLRenderbuffer,
				GLenum pActiveBindTarget = 0 );

		void bindDepthStencilRenderbuffer(
				GLRenderbufferObject & pGLRenderbuffer,
				Bitmask<uint32> pBufferMask,
				GLenum pActiveBindTarget = 0 );

		void bindColorTexture(
				uint32 pColorAttachmentIndex,
				GLTextureObject & pGLTexture,
				const TextureSubResource & pSubResource,
				GLenum pActiveBindTarget = 0 );

		void bindDepthStencilTexture(
				GLTextureObject & pGLTexture,
				const TextureSubResource & pSubResource,
				Bitmask<uint32> pBufferMask,
				GLenum pActiveBindTarget = 0 );

		bool checkStatus( GLenum pActiveBindTarget = 0 ) const;

		static GLFramebufferObjectHandle create();

		static GLFramebufferObjectHandle createForDefaultFramebuffer();

	private:
		GLenum checkActiveBindTarget( GLenum pBindTarget ) const;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_GLCOMMON_FRAMEBUFFER_OBJECT_H__
