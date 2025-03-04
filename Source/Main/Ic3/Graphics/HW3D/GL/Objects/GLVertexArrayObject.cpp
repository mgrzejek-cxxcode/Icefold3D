
#include "GLVertexArrayObject.h"

namespace Ic3::Graphics::GCI
{

	GLVertexArrayObject::GLVertexArrayObject( GLuint pHandle )
	: GLObject( GLObjectBaseType::VertexArray, pHandle )
	{}

	GLVertexArrayObject::~GLVertexArrayObject() = default;

	GLVertexArrayObjectHandle GLVertexArrayObject::Create()
	{
		GLuint vertexArrayHandle = 0;

		glGenVertexArrays( 1, &vertexArrayHandle );
		Ic3OpenGLHandleLastError();

		return CreateGLObject<GLVertexArrayObject>( vertexArrayHandle );
	}

	bool GLVertexArrayObject::Release()
	{
		glDeleteVertexArrays( 1, &mGLHandle );
		Ic3OpenGLHandleLastError();

		return true;
	}

	bool GLVertexArrayObject::ValidateHandle() const
	{
		const auto checkResult = glIsVertexArray( mGLHandle );
		Ic3OpenGLHandleLastError();

		return checkResult != GL_FALSE;
	}

} // namespace Ic3::Graphics::GCI
