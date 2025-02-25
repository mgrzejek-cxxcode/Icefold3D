
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_GLC_SHADER_OBJECT_H__
#define __IC3_GRAPHICS_HW3D_GLC_SHADER_OBJECT_H__

#include "GLShaderCommon.h"
#include <cppx/memoryBuffer.h>

namespace Ic3::Graphics::GCI
{

	Ic3GLDeclareOpenGLObjectHandle( GLShaderObject );

	class GLShaderObject : public GLObject
	{
	public:
		GLenum const mGLShaderType;
		GLbitfield const mGLShaderStageMaskBit;

		GLShaderObject( GLuint pHandle, GLenum pGLShaderType, GLenum pGLShaderStageMaskBit );
		virtual ~GLShaderObject();

		virtual bool Release();
		virtual bool ValidateHandle() const;

		bool CompileSource( const void * pShaderSource, size_t pSourceLength );
		bool LoadBinary( GLenum pFormat, const void * pBinary, size_t pBinarySize );

		void SetDataLayoutMap( GLShaderDataLayoutMap pLayoutMap );

		CPPX_ATTR_NO_DISCARD GLShaderDataLayoutMap * GetDataLayoutMap() const noexcept;

		CPPX_ATTR_NO_DISCARD GLint QueryParameter( GLenum pParameter ) const;

		CPPX_ATTR_NO_DISCARD std::string GetInfoLog() const;
		CPPX_ATTR_NO_DISCARD std::string GetSource() const;

		CPPX_ATTR_NO_DISCARD size_t GetInfoLogLength() const;
		CPPX_ATTR_NO_DISCARD size_t GetSourceLength() const;

		CPPX_ATTR_NO_DISCARD bool IsInfoLogEmpty() const;
		CPPX_ATTR_NO_DISCARD bool IsSourceEmpty() const;

		static bool CheckBinaryCodeSupport();

		static bool CheckBinaryFormatSupport( GLenum pFormat );

		static GLbitfield GetStageMaskForEShaderType( GLenum pGLShaderType );

		static GLShaderObjectHandle Create( GLenum pGLShaderType );
		static GLShaderObjectHandle CreateWithSource( GLenum pGLShaderType, const void * pShaderSource, size_t pSourceLength );

	private:
		std::unique_ptr<GLShaderDataLayoutMap> _dataLayoutMap;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_GLC_SHADER_OBJECT_H__
