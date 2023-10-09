
#include "GLVertexArrayObject.h"

namespace Ic3::Graphics::GCI
{

	GLVertexArrayObject::GLVertexArrayObject( GLuint pHandle )
	: GLObject( GLObjectBaseType::VertexArray, pHandle )
	{}

	GLVertexArrayObject::~GLVertexArrayObject() = default;

	GLVertexArrayObjectHandle GLVertexArrayObject::create()
	{
		GLuint vertexArrayHandle = 0;

		glGenVertexArrays( 1, &vertexArrayHandle );
		ic3OpenGLHandleLastError();

		return createGLObject<GLVertexArrayObject>( vertexArrayHandle );
	}

	bool GLVertexArrayObject::release()
	{
		glDeleteVertexArrays( 1, &mGLHandle );
		ic3OpenGLHandleLastError();

		return true;
	}

	bool GLVertexArrayObject::validateHandle() const
	{
		auto isProgram = glIsVertexArray( mGLHandle );
		ic3OpenGLHandleLastError();

		return isProgram != GL_FALSE;
	}

} // namespace Ic3::Graphics::GCI
