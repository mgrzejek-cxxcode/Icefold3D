
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_GLC_SHADER_PIPELINE_OBJECT_H__
#define __IC3_GRAPHICS_HW3D_GLC_SHADER_PIPELINE_OBJECT_H__

#include "../GLPrerequisites.h"

namespace Ic3::Graphics::GCI
{

	Ic3GLDeclareOpenGLObjectHandle( GLShaderPipelineObject );
	Ic3GLDeclareOpenGLObjectHandle( GLShaderProgramObject );

	class GLShaderPipelineObject : public GLObject
	{
	public:
		GLShaderPipelineObject( GLuint pHandle );
		virtual ~GLShaderPipelineObject();

		virtual bool Release();
		virtual bool ValidateHandle() const;

		void AttachProgram( GLShaderProgramObject & pProgram );

		void AttachProgram( GLShaderProgramObject & pProgram, cppx::bitmask<GLenum> pStageMask );

		void SetActiveProgram( GLShaderProgramObject & pProgram );

		void ResetActiveProgram();

		static GLuint QueryCurrentShaderPipelineBinding();

		static GLShaderPipelineObjectHandle Create();
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_GLC_SHADER_PIPELINE_OBJECT_H__
