
#include "GLRenderbufferObject.h"

namespace Ic3::Graphics::GCI
{

	GLRenderbufferObject::GLRenderbufferObject( GLuint pHandle, const GLRenderbufferCreateInfo & pGLCreateInfo )
	: GLObject( GLObjectBaseType::Renderbuffer, pHandle )
	, dimensions( pGLCreateInfo.dimensions )
	, mGLInternalFormat( pGLCreateInfo.internalFormat )
	{}

	GLRenderbufferObject::~GLRenderbufferObject() = default;

	GLRenderbufferObjectHandle GLRenderbufferObject::Create( const GLRenderbufferCreateInfo & pGLCreateInfo )
	{
		GLuint renderbufferHandle = 0;

		glGenRenderbuffers( 1, &renderbufferHandle );
		Ic3OpenGLHandleLastError();

		glBindRenderbuffer( GL_RENDERBUFFER, renderbufferHandle );
		Ic3OpenGLHandleLastError();

		GLRenderbufferObjectHandle openglRenderbufferObject{ new GLRenderbufferObject( renderbufferHandle, pGLCreateInfo ) };
		if( !openglRenderbufferObject->Initialize( pGLCreateInfo ) )
		{
			return nullptr;
		}

		glBindRenderbuffer( GL_RENDERBUFFER, 0 );
		Ic3OpenGLHandleLastError();

		return openglRenderbufferObject;
	}

	bool GLRenderbufferObject::Release()
	{
		glDeleteRenderbuffers( 1, &mGLHandle );
		Ic3OpenGLHandleLastError();

		return true;
	}

	bool GLRenderbufferObject::ValidateHandle() const
	{
		auto isBuffer = glIsRenderbuffer( mGLHandle );
		Ic3OpenGLHandleLastError();

		return isBuffer != GL_FALSE;
	}

	bool GLRenderbufferObject::Initialize( const GLRenderbufferCreateInfo & pGLCreateInfo )
	{
		if( pGLCreateInfo.msaaLevel == 0 )
		{
			glRenderbufferStorage( GL_RENDERBUFFER,
			                       pGLCreateInfo.internalFormat,
			                       pGLCreateInfo.dimensions.x,
			                       pGLCreateInfo.dimensions.y );
			Ic3OpenGLHandleLastError();
		}
		else
		{
			glRenderbufferStorageMultisample( GL_RENDERBUFFER,
			                                  pGLCreateInfo.msaaLevel,
			                                  pGLCreateInfo.internalFormat,
			                                  pGLCreateInfo.dimensions.x,
			                                  pGLCreateInfo.dimensions.y );
			Ic3OpenGLHandleLastError();
		}

		return true;
	}

} // namespace Ic3::Graphics::GCI
