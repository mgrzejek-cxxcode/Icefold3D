
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_GLCOMMON_SHADER_PIPELINE_OBJECT_H__
#define __IC3_GRAPHICS_HW3D_GLCOMMON_SHADER_PIPELINE_OBJECT_H__

#include "../GLPrerequisites.h"

namespace Ic3::Graphics::GCI
{

	ic3GLDeclareOpenGLObjectHandle( GLShaderPipelineObject );
	ic3GLDeclareOpenGLObjectHandle( GLShaderProgramObject );

	class GLShaderPipelineObject : public GLObject
	{
	public:
		GLShaderPipelineObject( GLuint pHandle );
		virtual ~GLShaderPipelineObject();

		virtual bool release();
		virtual bool validateHandle() const;

		void attachProgram( GLShaderProgramObject & pProgram );

		void attachProgram( GLShaderProgramObject & pProgram, Bitmask<GLenum> pStageMask );

		void setActiveProgram( GLShaderProgramObject & pProgram );

		void resetActiveProgram();

		static GLuint queryCurrentShaderPipelineBinding();

		static GLShaderPipelineObjectHandle create();
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_GLCOMMON_SHADER_PIPELINE_OBJECT_H__
