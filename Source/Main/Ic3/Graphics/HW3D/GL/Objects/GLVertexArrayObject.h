
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_GLC_VERTEX_ARRAY_OBJECT_H__
#define __IC3_GRAPHICS_HW3D_GLC_VERTEX_ARRAY_OBJECT_H__

#include "../GLPrerequisites.h"

namespace Ic3::Graphics::GCI
{

	Ic3GLDeclareOpenGLObjectHandle( GLVertexArrayObject );

	class GLVertexArrayObject : public GLObject
	{
	public:
		GLVertexArrayObject( GLuint pHandle );
		virtual ~GLVertexArrayObject();

		virtual bool Release();
		virtual bool ValidateHandle() const;

		static GLVertexArrayObjectHandle Create();
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_GLC_VERTEX_ARRAY_OBJECT_H__
