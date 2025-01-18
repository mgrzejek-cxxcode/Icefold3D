
#include "GLShaderPipelineObject.h"
#include "GLShaderProgramObject.h"

namespace Ic3::Graphics::GCI
{

	GLShaderPipelineObject::GLShaderPipelineObject( GLuint pHandle )
	: GLObject( GLObjectBaseType::ShaderPipeline, pHandle )
	{}

	GLShaderPipelineObject::~GLShaderPipelineObject() = default;

	GLuint GLShaderPipelineObject::QueryCurrentShaderPipelineBinding()
	{
		GLint shaderPipelineHandle = 0;

		glGetIntegerv( GL_PROGRAM_PIPELINE_BINDING, &shaderPipelineHandle );
		Ic3OpenGLHandleLastError();

		return static_cast<GLuint>( shaderPipelineHandle );
	}

	GLShaderPipelineObjectHandle GLShaderPipelineObject::Create()
	{
		GLuint shaderPipelineHandle = 0;

		glGenProgramPipelines( 1, &shaderPipelineHandle );
		Ic3OpenGLHandleLastError();

		return CreateGLObject<GLShaderPipelineObject>( shaderPipelineHandle );
	}

	bool GLShaderPipelineObject::Release()
	{
		glDeleteProgramPipelines( 1, &mGLHandle );
		Ic3OpenGLHandleLastError();

		return true;
	}

	bool GLShaderPipelineObject::ValidateHandle() const
	{
		auto isProgram = glIsProgramPipeline( mGLHandle );
		Ic3OpenGLHandleLastError();

		return isProgram != GL_FALSE;
	}

	void GLShaderPipelineObject::AttachProgram( GLShaderProgramObject & pProgram )
	{
		auto programStageMask = pProgram.GetLinkedShaderStageMask();
		if( programStageMask == 0 )
		{
			return;
		}

		glUseProgramStages( mGLHandle, programStageMask, pProgram.mGLHandle );
		Ic3OpenGLHandleLastError();
	}

	void GLShaderPipelineObject::AttachProgram( GLShaderProgramObject & pProgram, cppx::bitmask<GLenum> pStageMask )
	{
		auto programStageMask = pProgram.GetLinkedShaderStageMask();
		if( programStageMask == 0 )
		{
			// WARN!
			return;
		}
		if( !programStageMask.is_set( pStageMask ) )
		{
			// WARN!
			return;
		}

		glUseProgramStages( mGLHandle, pStageMask, pProgram.mGLHandle );
		Ic3OpenGLHandleLastError();
	}

	void GLShaderPipelineObject::SetActiveProgram( GLShaderProgramObject & pProgram )
	{
		glActiveShaderProgram( mGLHandle, pProgram.mGLHandle );
		Ic3OpenGLHandleLastError();
	}

	void GLShaderPipelineObject::ResetActiveProgram()
	{
		glActiveShaderProgram( mGLHandle, 0 );
		Ic3OpenGLHandleLastError();
	}

} // namespace Ic3::Graphics::GCI
