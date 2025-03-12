
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
		Ic3OpenGLHandleLastError();

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
		Ic3OpenGLHandleLastError();

		return true;
	}

	bool GLSamplerObject::ValidateHandle() const
	{
		const auto checkResult = glIsSampler( mGLHandle );
		Ic3OpenGLHandleLastError();

		return checkResult != GL_FALSE;
	}

	bool GLSamplerObject::SetSamplerState( const GLSamplerState & pSamplerState )
	{
		glSamplerParameteri( mGLHandle, GL_TEXTURE_WRAP_S, pSamplerState.addressModeS );
		Ic3OpenGLCheckLastResult();

		glSamplerParameteri( mGLHandle, GL_TEXTURE_WRAP_T, pSamplerState.addressModeT );
		Ic3OpenGLCheckLastResult();

		glSamplerParameteri( mGLHandle, GL_TEXTURE_WRAP_R, pSamplerState.addressModeR );
		Ic3OpenGLCheckLastResult();

		glSamplerParameteri( mGLHandle, GL_TEXTURE_MAG_FILTER, pSamplerState.magFilter );
		Ic3OpenGLCheckLastResult();

		glSamplerParameteri( mGLHandle, GL_TEXTURE_MIN_FILTER, pSamplerState.minFilter );
		Ic3OpenGLCheckLastResult();

	#if( IC3_GX_GL_FEATURE_SUPPORT_TEXTURE_ANISOTROPIC_FILTER )
		if( pSamplerState.anisotropyLevel > 0 )
		{
			glSamplerParameteri( mGLHandle, GL_TEXTURE_MAX_ANISOTROPY, pSamplerState.anisotropyLevel );
			Ic3OpenGLCheckLastResult();
		}
	#endif

		glSamplerParameterf( mGLHandle, GL_TEXTURE_MIN_LOD, pSamplerState.mipLODRange.first );
		Ic3OpenGLCheckLastResult();

		glSamplerParameterf( mGLHandle, GL_TEXTURE_MAX_LOD, pSamplerState.mipLODRange.second );
		Ic3OpenGLCheckLastResult();

	#if( IC3_GX_GL_FEATURE_SUPPORT_TEXTURE_EXTENDED_ADDRESS_MODE )
		glSamplerParameterfv( mGLHandle, GL_TEXTURE_BORDER_COLOR, &( pSamplerState.borderColor.uv_rgba[0] ) );
		Ic3OpenGLCheckLastResult();
	#endif

		glSamplerParameteri( mGLHandle, GL_TEXTURE_COMPARE_MODE, pSamplerState.textureCompareMode );
		Ic3OpenGLCheckLastResult();

		if( pSamplerState.textureCompareMode == GL_COMPARE_REF_TO_TEXTURE )
		{
			glSamplerParameteri( mGLHandle, GL_TEXTURE_COMPARE_FUNC, pSamplerState.textureCompareFunc );
			Ic3OpenGLCheckLastResult();
		}

		return true;
	}

} // namespace Ic3::Graphics::GCI
