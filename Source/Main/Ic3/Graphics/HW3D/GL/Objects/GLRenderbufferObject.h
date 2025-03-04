
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_GLC_RENDERBUFFER_OBJECT_H__
#define __IC3_GRAPHICS_HW3D_GLC_RENDERBUFFER_OBJECT_H__

#include "../GLPrerequisites.h"

namespace Ic3::Graphics::GCI
{

	Ic3GLDeclareOpenGLObjectHandle( GLRenderbufferObject );

	struct GLRenderbufferCreateInfo
	{
		Math::Vec2u32 dimensions;
		GLenum internalFormat = 0;
		GLuint msaaLevel = 0;
	};

	class GLRenderbufferObject : public GLObject
	{
	public:
		Math::Vec2u32 const dimensions;
		GLenum const mGLInternalFormat;

		GLRenderbufferObject( GLuint pHandle, const GLRenderbufferCreateInfo & pGLCreateInfo );
		virtual ~GLRenderbufferObject();

		virtual bool Release();
		virtual bool ValidateHandle() const;

		static GLRenderbufferObjectHandle Create( const GLRenderbufferCreateInfo & pGLCreateInfo );

	private:
		bool Initialize( const GLRenderbufferCreateInfo & pGLCreateInfo );
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_GLC_RENDERBUFFER_OBJECT_H__
