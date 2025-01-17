
#include "GLSamplerObject.h"

namespace Ic3::Graphics::GCI
{

	GLSamplerObject::GLSamplerObject( GLuint pHandle )
	: GLObject( GLObjectBaseType::Sampler, pHandle )
	{}

	GLSamplerObject::~GLSamplerObject() = default;

	GLSamplerObjectHandle GLSamplerObject::Create( const GLSamplerState & pSamplerState )
	{
		GLuint samplerHandle = 0;

		glGenSamplers( 1, &samplerHandle );
		ic3OpenGLHandleLastError();

		GLSamplerObjectHandle openglSamplerObject{ new GLSamplerObject( samplerHandle ) };
		if( !openglSamplerObject->SetSamplerState( pSamplerState ) )
		{
			return nullptr;
		}

		return openglSamplerObject;
	}

	bool GLSamplerObject::Release()
	{
		glDeleteSamplers( 1, &mGLHandle );
		ic3OpenGLHandleLastError();

		return true;
	}

	bool GLSamplerObject::ValidateHandle() const
	{
		GLboolean checkResult = glIsSampler( mGLHandle );
		return checkResult == GL_TRUE;
	}

	bool GLSamplerObject::SetSamplerState( const GLSamplerState & pSamplerState )
	{
		glSamplerParameteri( mGLHandle, GL_TEXTURE_WRAP_S, pSamplerState.addressModeS );
		ic3OpenGLCheckLastResult();

		glSamplerParameteri( mGLHandle, GL_TEXTURE_WRAP_T, pSamplerState.addressModeT );
		ic3OpenGLCheckLastResult();

		glSamplerParameteri( mGLHandle, GL_TEXTURE_WRAP_R, pSamplerState.addressModeR );
		ic3OpenGLCheckLastResult();

		glSamplerParameteri( mGLHandle, GL_TEXTURE_MAG_FILTER, pSamplerState.magFilter );
		ic3OpenGLCheckLastResult();

		glSamplerParameteri( mGLHandle, GL_TEXTURE_MIN_FILTER, pSamplerState.minFilter );
		ic3OpenGLCheckLastResult();

	#if( IC3_GX_GL_FEATURE_SUPPORT_TEXTURE_ANISOTROPIC_FILTER )
		if( pSamplerState.anisotropyLevel > 0 )
		{
			glSamplerParameteri( mGLHandle, GL_TEXTURE_MAX_ANISOTROPY, pSamplerState.anisotropyLevel );
			ic3OpenGLCheckLastResult();
		}
	#endif

		glSamplerParameterf( mGLHandle, GL_TEXTURE_MIN_LOD, pSamplerState.mipLODRange.first );
		ic3OpenGLCheckLastResult();

		glSamplerParameterf( mGLHandle, GL_TEXTURE_MAX_LOD, pSamplerState.mipLODRange.second );
		ic3OpenGLCheckLastResult();

	#if( IC3_GX_GL_FEATURE_SUPPORT_TEXTURE_EXTENDED_ADDRESS_MODE )
		glSamplerParameterfv( mGLHandle, GL_TEXTURE_BORDER_COLOR, &( pSamplerState.borderColor.mRGBA[0] ) );
		ic3OpenGLCheckLastResult();
	#endif

		glSamplerParameteri( mGLHandle, GL_TEXTURE_COMPARE_MODE, pSamplerState.textureCompareMode );
		ic3OpenGLCheckLastResult();

		if( pSamplerState.textureCompareMode == GL_COMPARE_REF_TO_TEXTURE )
		{
			glSamplerParameteri( mGLHandle, GL_TEXTURE_COMPARE_FUNC, pSamplerState.textureCompareFunc );
			ic3OpenGLCheckLastResult();
		}

		return true;
	}

} // namespace Ic3::Graphics::GCI
