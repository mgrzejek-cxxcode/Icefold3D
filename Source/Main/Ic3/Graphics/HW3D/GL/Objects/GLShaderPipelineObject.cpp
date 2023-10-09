
#include "GLShaderPipelineObject.h"
#include "GLShaderProgramObject.h"

namespace Ic3::Graphics::GCI
{

	GLShaderPipelineObject::GLShaderPipelineObject( GLuint pHandle )
	: GLObject( GLObjectBaseType::ShaderPipeline, pHandle )
	{}

	GLShaderPipelineObject::~GLShaderPipelineObject() = default;

	GLuint GLShaderPipelineObject::queryCurrentShaderPipelineBinding()
	{
		GLint shaderPipelineHandle = 0;

		glGetIntegerv( GL_PROGRAM_PIPELINE_BINDING, &shaderPipelineHandle );
		ic3OpenGLHandleLastError();

		return static_cast<GLuint>( shaderPipelineHandle );
	}

	GLShaderPipelineObjectHandle GLShaderPipelineObject::create()
	{
		GLuint shaderPipelineHandle = 0;

		glGenProgramPipelines( 1, &shaderPipelineHandle );
		ic3OpenGLHandleLastError();

		return createGLObject<GLShaderPipelineObject>( shaderPipelineHandle );
	}

	bool GLShaderPipelineObject::release()
	{
		glDeleteProgramPipelines( 1, &mGLHandle );
		ic3OpenGLHandleLastError();

		return true;
	}

	bool GLShaderPipelineObject::validateHandle() const
	{
		auto isProgram = glIsProgramPipeline( mGLHandle );
		ic3OpenGLHandleLastError();

		return isProgram != GL_FALSE;
	}

	void GLShaderPipelineObject::attachProgram( GLShaderProgramObject & pProgram )
	{
		auto programStageMask = pProgram.getLinkedShaderStageMask();
		if( programStageMask == 0 )
		{
			return;
		}

		glUseProgramStages( mGLHandle, programStageMask, pProgram.mGLHandle );
		ic3OpenGLHandleLastError();
	}

	void GLShaderPipelineObject::attachProgram( GLShaderProgramObject & pProgram, Bitmask<GLenum> pStageMask )
	{
		auto programStageMask = pProgram.getLinkedShaderStageMask();
		if( programStageMask == 0 )
		{
			// WARN!
			return;
		}
		if( !programStageMask.isSet( pStageMask ) )
		{
			// WARN!
			return;
		}

		glUseProgramStages( mGLHandle, pStageMask, pProgram.mGLHandle );
		ic3OpenGLHandleLastError();
	}

	void GLShaderPipelineObject::setActiveProgram( GLShaderProgramObject & pProgram )
	{
		glActiveShaderProgram( mGLHandle, pProgram.mGLHandle );
		ic3OpenGLHandleLastError();
	}

	void GLShaderPipelineObject::resetActiveProgram()
	{
		glActiveShaderProgram( mGLHandle, 0 );
		ic3OpenGLHandleLastError();
	}

} // namespace Ic3::Graphics::GCI
