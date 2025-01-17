
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
		ic3OpenGLHandleLastError();

		glBindRenderbuffer( GL_RENDERBUFFER, renderbufferHandle );
		ic3OpenGLHandleLastError();

		GLRenderbufferObjectHandle openglRenderbufferObject{ new GLRenderbufferObject( renderbufferHandle, pGLCreateInfo ) };
		if( !openglRenderbufferObject->Initialize( pGLCreateInfo ) )
		{
			return nullptr;
		}

		glBindRenderbuffer( GL_RENDERBUFFER, 0 );
		ic3OpenGLHandleLastError();

		return openglRenderbufferObject;
	}

	bool GLRenderbufferObject::Release()
	{
		glDeleteRenderbuffers( 1, &mGLHandle );
		ic3OpenGLHandleLastError();

		return true;
	}

	bool GLRenderbufferObject::ValidateHandle() const
	{
		auto isBuffer = glIsRenderbuffer( mGLHandle );
		ic3OpenGLHandleLastError();

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
			ic3OpenGLHandleLastError();
		}
		else
		{
			glRenderbufferStorageMultisample( GL_RENDERBUFFER,
			                                  pGLCreateInfo.msaaLevel,
			                                  pGLCreateInfo.internalFormat,
			                                  pGLCreateInfo.dimensions.x,
			                                  pGLCreateInfo.dimensions.y );
			ic3OpenGLHandleLastError();
		}

		return true;
	}

} // namespace Ic3::Graphics::GCI
