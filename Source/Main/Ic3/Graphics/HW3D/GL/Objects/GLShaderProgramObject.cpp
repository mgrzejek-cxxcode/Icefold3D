
#include "GLShaderProgramObject.h"
#include "GLShaderObject.h"
#include <cppx/byteArray.h>

namespace Ic3::Graphics::GCI
{

	GLShaderProgramObject::GLShaderProgramObject( GLuint pHandle, GLShaderProgramType pProgramType )
	: GLObject( GLObjectBaseType::ShaderProgram, pHandle )
	, mGLProgramType( pProgramType )
	{ }

	GLShaderProgramObject::~GLShaderProgramObject() = default;

	GLuint GLShaderProgramObject::QueryCurrentShaderProgramBinding()
	{
		GLint shaderProgramHandle = 0;

		glGetIntegerv( GL_CURRENT_PROGRAM, &shaderProgramHandle );
		ic3OpenGLHandleLastError();

		return static_cast<GLuint>( shaderProgramHandle );
	}

	GLShaderProgramObjectHandle GLShaderProgramObject::Create( GLShaderProgramType pProgramType )
	{
		auto programHandle = glCreateProgram();
		ic3OpenGLHandleLastError();

		if( pProgramType == GLShaderProgramType::Separable )
		{
			glProgramParameteri( programHandle, GL_PROGRAM_SEPARABLE, GL_TRUE );
			ic3OpenGLHandleLastError();
		}

		GLShaderProgramObjectHandle openglProgramObject{ new GLShaderProgramObject( programHandle, pProgramType ) };

		return openglProgramObject;
	}

	GLShaderProgramObjectHandle GLShaderProgramObject::CreateSeparableModule( GLShaderObject & pShader )
	{
		auto programObject = GLShaderProgramObject::Create( GLShaderProgramType::Separable );
		programObject->AttachShader( pShader );
		programObject->Link();
		programObject->DetachShader( pShader );
		return programObject;
	}

	GLShaderProgramObjectHandle GLShaderProgramObject::CreateSeparableModule( GLShaderObject & pShader, const GLShaderDataLayoutMap & pLayoutMap )
	{
		auto programObject = GLShaderProgramObject::Create( GLShaderProgramType::Separable );
		programObject->AttachShader( pShader );
		SetProgramPreLinkBindings( *programObject, pLayoutMap );
		programObject->Link();
		SetProgramPostLinkBindings( *programObject, pLayoutMap );
		programObject->DetachShader( pShader );
		return programObject;
	}

	void GLShaderProgramObject::SetProgramPreLinkBindings( GLShaderProgramObject & pProgram, const GLShaderDataLayoutMap & pLayoutMap )
	{
		for( const auto & attributeLocation : pLayoutMap.attributeLocations )
		{
			glBindAttribLocation( pProgram.mGLHandle, attributeLocation.second, attributeLocation.first.data() );
			ic3OpenGLHandleLastError();
		}

		for( const auto & fragDataLocation : pLayoutMap.fragDataLocations )
		{
			glBindFragDataLocation( pProgram.mGLHandle, fragDataLocation.second, fragDataLocation.first.data() );
			ic3OpenGLHandleLastError();
		}
	}

	void GLShaderProgramObject::SetProgramPostLinkBindings( GLShaderProgramObject & pProgram, const GLShaderDataLayoutMap & pLayoutMap )
	{
		for( const auto & samplerBinding : pLayoutMap.samplerBindings )
		{
			GLint samplerVariableLocation = glGetUniformLocation( pProgram.mGLHandle, samplerBinding.first.data() );
			ic3OpenGLHandleLastError();

			if( samplerVariableLocation != -1 )
			{
				glProgramUniform1i( pProgram.mGLHandle, samplerVariableLocation, samplerBinding.second );
				ic3OpenGLHandleLastError();
			}
		}

		for( const auto & uniformBlockBinding : pLayoutMap.uniformBlockBindings )
		{
			GLint blockIndex = glGetUniformBlockIndex( pProgram.mGLHandle, uniformBlockBinding.first.data() );
			ic3OpenGLHandleLastError();

			if( blockIndex != -1 )
			{
				glUniformBlockBinding( pProgram.mGLHandle, blockIndex, uniformBlockBinding.second );
				ic3OpenGLHandleLastError();
			}
		}
	}

	bool GLShaderProgramObject::Release()
	{
		auto deleteStatus = QueryParameter( GL_DELETE_STATUS );
		if( deleteStatus == GL_FALSE )
		{
			glDeleteProgram( mGLHandle );
			ic3OpenGLHandleLastError();

			return true;
		}

		return false;
	}

	bool GLShaderProgramObject::ValidateHandle() const
	{
		auto isProgram = glIsProgram( mGLHandle );
		ic3OpenGLHandleLastError();

		return isProgram != GL_FALSE;
	}

	void GLShaderProgramObject::AttachShader( GLuint pShaderHandle )
	{
		glAttachShader( mGLHandle, pShaderHandle );
		ic3OpenGLHandleLastError();
	}

	void GLShaderProgramObject::AttachShader( const GLShaderObject & pShader )
	{
		glAttachShader( mGLHandle, pShader.mGLHandle );
		ic3OpenGLHandleLastError();
	}

	void GLShaderProgramObject::DetachAllShaders()
	{
		auto attachedShaders = GetAttachedShaders();
		for( auto & shaderHandle : attachedShaders )
		{
			glDetachShader( mGLHandle, shaderHandle );
			ic3OpenGLHandleLastError();
		}
	}

	void GLShaderProgramObject::DetachShader( GLuint pShaderHandle )
	{
		glDetachShader( mGLHandle, pShaderHandle );
		ic3OpenGLHandleLastError();
	}

	void GLShaderProgramObject::DetachShader( const GLShaderObject & pShader )
	{
		glDetachShader( mGLHandle, pShader.mGLHandle );
		ic3OpenGLHandleLastError();
	}

	bool GLShaderProgramObject::Link()
	{
		auto attachedShadersStageMask = queryShaderStageMask();

		glLinkProgram( mGLHandle );
		ic3OpenGLHandleLastError();

		auto linkStatus = QueryParameter( GL_LINK_STATUS );
		if( linkStatus == GL_FALSE )
		{
			auto infoLog = GetInfoLog();
			ic3DebugOutput( infoLog.data() );
			ic3DebugInterrupt();
			return false;
		}

		_linkedShadersStageMask = attachedShadersStageMask;

		return true;
	}

	bool GLShaderProgramObject::Validate()
	{
		glValidateProgram( mGLHandle );
		ic3OpenGLHandleLastError();

		auto validateStatus = QueryParameter( GL_VALIDATE_STATUS );
		if( validateStatus == GL_FALSE )
		{
			auto infoLog = GetInfoLog();
			ic3DebugOutput( infoLog.data() );
			ic3DebugInterrupt();
			return false;
		}

		return true;
	}

	void GLShaderProgramObject::SetAttribLocation( const char * pAttribName, GLuint pLocation )
	{
		IC3_DEBUG_CODE({
           auto linkResult = QueryParameter( GL_LINK_STATUS );
           ic3DebugAssert( linkResult == GL_FALSE );
		});

		glBindAttribLocation( mGLHandle, pLocation, pAttribName );
		ic3OpenGLHandleLastError();
	}

	void GLShaderProgramObject::SetSamplerTextureUnit( const char * pSamplerName, GLuint pTextureIndex )
	{
		IC3_DEBUG_CODE({
			auto linkResult = QueryParameter( GL_LINK_STATUS );
			ic3DebugAssert( linkResult == GL_FALSE );
		});

		GLint samplerLocation = glGetUniformLocation( mGLHandle, pSamplerName );
		ic3OpenGLHandleLastError();

		glProgramUniform1i( mGLHandle, samplerLocation, pTextureIndex );
		ic3OpenGLHandleLastError();
	}

	void GLShaderProgramObject::SetUniformBlockBinding( const char * pBlockName, GLuint pBinding )
	{
		IC3_DEBUG_CODE({
			auto linkResult = QueryParameter( GL_LINK_STATUS );
			ic3DebugAssert( linkResult == GL_FALSE );
		});

		GLint uniformBlockIndex = glGetUniformBlockIndex( mGLHandle, pBlockName );
		ic3OpenGLHandleLastError();

		glUniformBlockBinding( mGLHandle, uniformBlockIndex, pBinding );
		ic3OpenGLHandleLastError();
	}

	GLint GLShaderProgramObject::QueryParameter( GLenum pParameter ) const
	{
		GLint parameterValue = GL_INVALID_VALUE;

		glGetProgramiv( mGLHandle, pParameter, &parameterValue );
		ic3OpenGLHandleLastError();

		return parameterValue;
	}

	GLuint GLShaderProgramObject::QueryVertexAttributeLocation( const char * pAttribName ) const
	{
		auto attribLocation = glGetAttribLocation( mGLHandle, reinterpret_cast<const GLchar *>( pAttribName ) );
		ic3OpenGLHandleLastError();

		if( attribLocation == -1 )
		{
			// Warn?
			return cvGLInvalidProgramLocation;
		}

		return static_cast<GLuint>( attribLocation );
	}

	cppx::bitmask<GLbitfield> GLShaderProgramObject::queryShaderStageMask() const
	{
		cppx::bitmask<GLbitfield> shaderStageMask = 0;

		auto attachedShaders = GetAttachedShaders();
		for( auto shaderHandle : attachedShaders )
		{
			GLint shaderType = 0;
			glGetShaderiv( shaderHandle, GL_SHADER_TYPE, &shaderType );
			ic3OpenGLHandleLastError();

			auto shaderStageBit = GLShaderObject::GetStageMaskForEShaderType( shaderType );
			shaderStageMask.set( shaderStageBit );
		}

		return shaderStageMask;
	}

	std::string GLShaderProgramObject::GetInfoLog() const
	{
		std::string infoLog {};

		auto infoLogLength = GetInfoLogLength();
		if ( infoLogLength > 0 )
		{
			cppx::dynamic_byte_array infoLogBuffer;
			infoLogBuffer.resize( infoLogLength );

			glGetProgramInfoLog( mGLHandle, static_cast<GLsizei>( infoLogLength ), nullptr, infoLogBuffer.dataAs<GLchar>() );
			ic3OpenGLHandleLastError();

			// Note: length returned by the GL includes null terminator!
			infoLog.assign( infoLogBuffer.dataAs<GLchar>(), infoLogLength - 1 );
		}

		return infoLog;
	}

	std::vector<GLuint> GLShaderProgramObject::GetAttachedShaders() const
	{
		std::vector<GLuint> shaderArray;

		auto attachedShadersNum = QueryParameter( GL_ATTACHED_SHADERS );
		if( attachedShadersNum > 0 )
		{
			shaderArray.resize( attachedShadersNum );

			GLsizei returnedShadersNum = 0;
			glGetAttachedShaders( mGLHandle, 64, &returnedShadersNum, shaderArray.data() );
			ic3OpenGLHandleLastError();

			ic3DebugAssert( returnedShadersNum == attachedShadersNum );
		}

		return shaderArray;
	}

	size_t GLShaderProgramObject::GetAttachedShadersNum() const
	{
		auto attachedShadersNum = QueryParameter( GL_ATTACHED_SHADERS );
		return attachedShadersNum;
	}

	size_t GLShaderProgramObject::GetInfoLogLength() const
	{
		auto infoLogLength = QueryParameter( GL_INFO_LOG_LENGTH );
		return infoLogLength;
	}

	cppx::bitmask<GLbitfield> GLShaderProgramObject::GetLinkedShaderStageMask() const
	{
		return _linkedShadersStageMask;
	}

	bool GLShaderProgramObject::HasAttachedShaders() const
	{
		auto attachedShadersNum = GetAttachedShadersNum();
		return attachedShadersNum > 0;
	}

	bool GLShaderProgramObject::IsInfoLogEmpty() const
	{
		auto infoLogLength = GetInfoLogLength();
		return infoLogLength > 0;
	}

} // namespace Ic3::Graphics::GCI
