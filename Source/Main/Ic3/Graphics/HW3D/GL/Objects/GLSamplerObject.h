
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_GLC_SAMPLER_OBJECT_H__
#define __IC3_GRAPHICS_HW3D_GLC_SAMPLER_OBJECT_H__

#include "../GLPrerequisites.h"

namespace Ic3::Graphics::GCI
{

	Ic3GLDeclareOpenGLObjectHandle( GLSamplerObject );

	struct GLSamplerState
	{
		GLenum addressModeS;
		GLenum addressModeT;
		GLenum addressModeR;
		GLint anisotropyLevel;
		GLenum magFilter;
		GLenum minFilter;
		GLenum textureCompareMode;
		GLenum textureCompareFunc;
		cxm::rgba_color_r32_norm borderColor;
		std::pair<GLfloat, GLfloat> mipLODRange;
	};

	class GLSamplerObject : public GLObject
	{
	public:
		GLSamplerObject( GLuint pHandle );
		virtual ~GLSamplerObject();

		virtual bool Release();
		virtual bool ValidateHandle() const;

		bool SetSamplerState( const GLSamplerState & pSamplerState );

		static GLSamplerObjectHandle Create( const GLSamplerState & pSamplerState );
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_GLC_SAMPLER_OBJECT_H__
