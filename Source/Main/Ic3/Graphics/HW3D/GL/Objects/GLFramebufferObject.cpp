
#include "GLFramebufferObject.h"
#include "GLRenderbufferObject.h"
#include "GLTextureObject.h"
#include "../State/GLCommonStateDefs.h"

namespace Ic3::Graphics::GCI
{

	GLFramebufferObject::GLFramebufferObject( GLuint pHandle )
	: GLObject( GLObjectBaseType::Framebuffer, pHandle )
	{}

	GLFramebufferObject::~GLFramebufferObject() = default;

	GLFramebufferObjectHandle GLFramebufferObject::Create()
	{
		GLuint framebufferHandle = 0;

		glGenFramebuffers( 1, &framebufferHandle );
		Ic3OpenGLHandleLastError();

		return CreateGLObject<GLFramebufferObject>( framebufferHandle );
	}

	GLFramebufferObjectHandle GLFramebufferObject::CreateForDefaultFramebuffer()
	{
		return CreateGLObject<GLFramebufferObject>( 0u );
	}

	bool GLFramebufferObject::Release()
	{
		if( mGLHandle != 0 )
		{
			glDeleteFramebuffers( 1, &mGLHandle );
			Ic3OpenGLHandleLastError();
		}

		return true;
	}

	bool GLFramebufferObject::ValidateHandle() const
	{
		auto isBuffer = glIsFramebuffer( mGLHandle );
		Ic3OpenGLHandleLastError();

		return isBuffer != GL_FALSE;
	}

	void GLFramebufferObject::BindColorRenderbuffer(
			uint32 pColorAttachmentIndex,
			GLRenderbufferObject & pGLRenderbuffer,
			GLenum pActiveBindTarget )
	{
		const auto framebufferBindTarget = CheckActiveBindTarget( pActiveBindTarget );

		glFramebufferRenderbuffer(
				framebufferBindTarget,
				GL_COLOR_ATTACHMENT0 + pColorAttachmentIndex,
				GL_RENDERBUFFER,
				pGLRenderbuffer.mGLHandle );
		Ic3OpenGLHandleLastError();
	}

	void GLFramebufferObject::BindDepthStencilRenderbuffer(
			GLRenderbufferObject & pGLRenderbuffer,
			cppx::bitmask<uint32> pBufferMask,
			GLenum pActiveBindTarget )
	{
		const auto framebufferBindTarget = CheckActiveBindTarget( pActiveBindTarget );
		const auto fboAttachmentID = GCU::GetFramebufferDepthStencilAttachmentIDForRTBufferMask( pBufferMask );

		glFramebufferRenderbuffer(
				framebufferBindTarget,
				fboAttachmentID,
				GL_RENDERBUFFER,
				pGLRenderbuffer.mGLHandle );
		Ic3OpenGLHandleLastError();
	}

	void GLFramebufferObject::BindColorTexture(
			uint32 pColorAttachmentIndex,
			GLTextureObject & pGLTexture,
			const TextureSubResource & pSubResource,
			GLenum pActiveBindTarget )
	{
		const auto framebufferBindTarget = CheckActiveBindTarget( pActiveBindTarget );

		switch( pGLTexture.mGLTextureBindTarget )
		{
			case GL_TEXTURE_2D:
			{
				glFramebufferTexture2D(
						framebufferBindTarget,
						GL_COLOR_ATTACHMENT0 + pColorAttachmentIndex,
						GL_TEXTURE_2D,
						pGLTexture.mGLHandle,
						cppx::numeric_cast<GLint>( pSubResource.uSubRes2D.mipLevel ) );
				Ic3OpenGLHandleLastError();
				break;
			}
			case GL_TEXTURE_2D_ARRAY:
			{
				glFramebufferTextureLayer(
						framebufferBindTarget,
						GL_COLOR_ATTACHMENT0 + pColorAttachmentIndex,
						pGLTexture.mGLHandle,
						cppx::numeric_cast<GLint>( pSubResource.uSubRes2D.mipLevel ),
						cppx::numeric_cast<GLint>( pSubResource.uSubRes2DArray.arrayIndex ) );
				Ic3OpenGLHandleLastError();
				break;
			}
			case GL_TEXTURE_2D_MULTISAMPLE:
			{
				glFramebufferTexture2D(
						framebufferBindTarget,
						GL_COLOR_ATTACHMENT0 + pColorAttachmentIndex,
						GL_TEXTURE_2D_MULTISAMPLE,
						pGLTexture.mGLHandle,
						cppx::numeric_cast<GLint>( pSubResource.uSubRes2D.mipLevel ) );
				Ic3OpenGLHandleLastError();
				break;
			}
			case GL_TEXTURE_3D:
			{
				glFramebufferTextureLayer(
						framebufferBindTarget,
						GL_COLOR_ATTACHMENT0 + pColorAttachmentIndex,
						pGLTexture.mGLHandle,
						cppx::numeric_cast<GLint>( pSubResource.uSubRes3D.mipLevel ),
						cppx::numeric_cast<GLint>( pSubResource.uSubRes3D.depthLayerIndex ) );
				Ic3OpenGLHandleLastError();
				break;
			}
			case GL_TEXTURE_CUBE_MAP:
			{
				glFramebufferTexture2D(
						GL_TEXTURE_CUBE_MAP_POSITIVE_X + pSubResource.uSubResCubeMap.faceIndex,
						GL_COLOR_ATTACHMENT0 + pColorAttachmentIndex,
						GL_TEXTURE_2D,
						pGLTexture.mGLHandle,
						cppx::numeric_cast<GLint>( pSubResource.uSubResCubeMap.mipLevel ) );
				Ic3OpenGLHandleLastError();
				break;
			}
		}
	}

	void GLFramebufferObject::BindDepthStencilTexture(
			GLTextureObject & pGLTexture,
			const TextureSubResource & pSubResource,
			cppx::bitmask<uint32> pBufferMask,
			GLenum pActiveBindTarget )
	{
		const auto framebufferBindTarget = CheckActiveBindTarget( pActiveBindTarget );
		const auto fboAttachmentID = GCU::GetFramebufferDepthStencilAttachmentIDForRTBufferMask( pBufferMask );

		switch( pGLTexture.mGLTextureBindTarget )
		{
			case GL_TEXTURE_2D:
			{
				glFramebufferTexture2D(
						framebufferBindTarget,
						fboAttachmentID,
						GL_TEXTURE_2D,
						pGLTexture.mGLHandle,
						cppx::numeric_cast<GLint>( pSubResource.uSubRes2D.mipLevel ) );
				Ic3OpenGLHandleLastError();
				break;
			}
			case GL_TEXTURE_2D_ARRAY:
			{
				glFramebufferTextureLayer(
						framebufferBindTarget,
						fboAttachmentID,
						pGLTexture.mGLHandle,
						cppx::numeric_cast<GLint>( pSubResource.uSubRes2DArray.mipLevel ),
						cppx::numeric_cast<GLint>( pSubResource.uSubRes2DArray.arrayIndex ) );
				Ic3OpenGLHandleLastError();
				break;
			}
			case GL_TEXTURE_2D_MULTISAMPLE:
			{
				glFramebufferTexture2D(
						framebufferBindTarget,
						fboAttachmentID,
						GL_TEXTURE_2D_MULTISAMPLE,
						pGLTexture.mGLHandle,
						cppx::numeric_cast<GLint>( pSubResource.uSubRes2D.mipLevel ) );
				Ic3OpenGLHandleLastError();
				break;
			}
			case GL_TEXTURE_3D:
			{
				glFramebufferTextureLayer(
						framebufferBindTarget,
						fboAttachmentID,
						pGLTexture.mGLHandle,
						cppx::numeric_cast<GLint>( pSubResource.uSubRes3D.mipLevel ),
						cppx::numeric_cast<GLint>( pSubResource.uSubRes3D.depthLayerIndex ) );
				Ic3OpenGLHandleLastError();
				break;
			}
			case GL_TEXTURE_CUBE_MAP:
			{
				glFramebufferTexture2D(
						GL_TEXTURE_CUBE_MAP_POSITIVE_X + pSubResource.uSubResCubeMap.faceIndex,
						fboAttachmentID,
						GL_TEXTURE_2D,
						pGLTexture.mGLHandle,
						cppx::numeric_cast<GLint>( pSubResource.uSubResCubeMap.mipLevel ) );
				Ic3OpenGLHandleLastError();
				break;
			}
		}
	}

	bool GLFramebufferObject::CheckStatus( GLenum pActiveBindTarget ) const
	{
		auto framebufferBindTarget = CheckActiveBindTarget( pActiveBindTarget );

		GLenum framebufferStatus = glCheckFramebufferStatus( framebufferBindTarget );
		Ic3OpenGLHandleLastError();

		if( framebufferStatus != GL_FRAMEBUFFER_COMPLETE )
		{
			switch( framebufferStatus )
			{
				case GL_FRAMEBUFFER_UNDEFINED:
					Ic3DebugInterrupt();
					break;

				case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
					Ic3DebugInterrupt();
					break;

				case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
					Ic3DebugInterrupt();
					break;

				case GL_FRAMEBUFFER_UNSUPPORTED:
					Ic3DebugInterrupt();
					break;

				case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
					Ic3DebugInterrupt();
					break;
			}

			return false;
		}

		return true;
	}

	GLenum GLFramebufferObject::CheckActiveBindTarget( GLenum pBindTarget ) const
	{
		if( pBindTarget == 0 )
		{
			pBindTarget = GL_FRAMEBUFFER;

			glBindFramebuffer( GL_FRAMEBUFFER, mGLHandle );
			Ic3OpenGLHandleLastError();
		}

		return pBindTarget;
	}

} // namespace Ic3::Graphics::GCI
