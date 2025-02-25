
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_GLC_FRAMEBUFFER_OBJECT_H__
#define __IC3_GRAPHICS_HW3D_GLC_FRAMEBUFFER_OBJECT_H__

#include "../GLPrerequisites.h"
#include <Ic3/Graphics/GCI/Resources/TextureCommon.h>

namespace Ic3::Graphics::GCI
{

	class GLRenderbufferObject;
	class GLTextureObject;

	Ic3GLDeclareOpenGLObjectHandle( GLFramebufferObject );

	class GLFramebufferObject : public GLObject
	{
	public:
		GLFramebufferObject( GLuint pHandle );
		virtual ~GLFramebufferObject();

		virtual bool Release();
		virtual bool ValidateHandle() const;

		void BindColorRenderbuffer(
				uint32 pColorAttachmentIndex,
				GLRenderbufferObject & pGLRenderbuffer,
				GLenum pActiveBindTarget = 0 );

		void BindDepthStencilRenderbuffer(
				GLRenderbufferObject & pGLRenderbuffer,
				cppx::bitmask<uint32> pBufferMask,
				GLenum pActiveBindTarget = 0 );

		void BindColorTexture(
				uint32 pColorAttachmentIndex,
				GLTextureObject & pGLTexture,
				const TextureSubResource & pSubResource,
				GLenum pActiveBindTarget = 0 );

		void BindDepthStencilTexture(
				GLTextureObject & pGLTexture,
				const TextureSubResource & pSubResource,
				cppx::bitmask<uint32> pBufferMask,
				GLenum pActiveBindTarget = 0 );

		bool CheckStatus( GLenum pActiveBindTarget = 0 ) const;

		static GLFramebufferObjectHandle Create();

		static GLFramebufferObjectHandle CreateForDefaultFramebuffer();

	private:
		GLenum CheckActiveBindTarget( GLenum pBindTarget ) const;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_GLC_FRAMEBUFFER_OBJECT_H__
