
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_GLC_SHADER_PROGRAM_OBJECT_H__
#define __IC3_GRAPHICS_HW3D_GLC_SHADER_PROGRAM_OBJECT_H__

#include "GLShaderCommon.h"
#include <cppx/memoryBuffer.h>

namespace Ic3::Graphics::GCI
{

	Ic3GLDeclareOpenGLObjectHandle( GLShaderObject );
	Ic3GLDeclareOpenGLObjectHandle( GLShaderProgramObject );

	enum class GLShaderProgramType : enum_default_value_t
	{
		Combined,
		Separable
	};

	class GLShaderProgramObject : public GLObject
	{
	public:
		GLShaderProgramType const mGLProgramType;

		GLShaderProgramObject( GLuint pHandle, GLShaderProgramType pProgramType );
		virtual ~GLShaderProgramObject();

		virtual bool Release();
		virtual bool ValidateHandle() const;

		void AttachShader( GLuint pShaderHandle );
		void AttachShader( const GLShaderObject & pShader );

		void DetachAllShaders();
		void DetachShader( GLuint pShaderHandle );
		void DetachShader( const GLShaderObject & pShader );

		bool Link();
		bool Validate();

		void SetAttribLocation( const char * pAttribName, GLuint pLocation );
		void SetSamplerTextureUnit( const char * pSamplerName, GLuint pTextureIndex );
		void SetUniformBlockBinding( const char * pBlockName, GLuint pBinding );

		GLint QueryParameter( GLenum pParameter ) const;
		GLuint QueryVertexAttributeLocation( const char * pAttribName ) const;
		cppx::bitmask<GLbitfield> queryShaderStageMask() const;

		std::string GetInfoLog() const;
		std::vector<GLuint> GetAttachedShaders() const;
		size_t GetAttachedShadersNum() const;
		size_t GetInfoLogLength() const;
		cppx::bitmask<GLbitfield> GetLinkedShaderStageMask() const;

		bool HasAttachedShaders() const;
		bool IsInfoLogEmpty() const;

		static GLuint QueryCurrentShaderProgramBinding();

		static GLShaderProgramObjectHandle Create( GLShaderProgramType pProgramType );

		static GLShaderProgramObjectHandle CreateSeparableModule( GLShaderObject & pShader );
		static GLShaderProgramObjectHandle CreateSeparableModule( GLShaderObject & pShader, const GLShaderDataLayoutMap & pLayoutMap );

		static void SetProgramPreLinkBindings( GLShaderProgramObject & pProgram, const GLShaderDataLayoutMap & pLayoutMap );
		static void SetProgramPostLinkBindings( GLShaderProgramObject & pProgram, const GLShaderDataLayoutMap & pLayoutMap );

	private:
		GLbitfield _linkedShadersStageMask;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_GLC_SHADER_PROGRAM_OBJECT_H__
