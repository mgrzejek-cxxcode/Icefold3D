
#include "../GLPrerequisites.h"

namespace Ic3::Graphics::GCI
{

	GLObject::GLObject( GLObjectBaseType pBaseType, GLuint pHandle )
	: mBaseType( pBaseType )
	, mGLHandle( pHandle )
	{}

	GLObject::~GLObject() = default;

	bool GLObject::Release()
	{
		return true;
	}

	bool GLObject::ValidateHandle() const
	{
		return false;
	}

} // namespace Ic3::Graphics::GCI
