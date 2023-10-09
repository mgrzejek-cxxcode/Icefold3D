
#include "../GLPrerequisites.h"

namespace Ic3::Graphics::GCI
{

	GLObject::GLObject( GLObjectBaseType pBaseType, GLuint pHandle )
	: mBaseType( pBaseType )
	, mGLHandle( pHandle )
	{}

	GLObject::~GLObject() = default;

	bool GLObject::release()
	{
		return true;
	}

	bool GLObject::validateHandle() const
	{
		return false;
	}

} // namespace Ic3::Graphics::GCI
