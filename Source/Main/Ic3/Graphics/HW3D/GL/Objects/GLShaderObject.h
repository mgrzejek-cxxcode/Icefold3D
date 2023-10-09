
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_GLCOMMON_SHADER_OBJECT_H__
#define __IC3_GRAPHICS_HW3D_GLCOMMON_SHADER_OBJECT_H__

#include "GLShaderCommon.h"
#include <Ic3/Cppx/MemoryBuffer.h>

namespace Ic3::Graphics::GCI
{

	ic3GLDeclareOpenGLObjectHandle( GLShaderObject );

	class GLShaderObject : public GLObject
	{
	public:
		GLenum const mGLShaderType;
		GLbitfield const mGLShaderStageMaskBit;

		GLShaderObject( GLuint pHandle, GLenum pGLShaderType, GLenum pGLShaderStageMaskBit );
		virtual ~GLShaderObject();

		virtual bool release();
		virtual bool validateHandle() const;

		bool compileSource( const void * pShaderSource, size_t pSourceLength );
		bool loadBinary( GLenum pFormat, const void * pBinary, size_t pBinarySize );

		void setDataLayoutMap( GLShaderDataLayoutMap pLayoutMap );

		IC3_ATTR_NO_DISCARD GLShaderDataLayoutMap * getDataLayoutMap() const noexcept;

		IC3_ATTR_NO_DISCARD GLint queryParameter( GLenum pParameter ) const;

		IC3_ATTR_NO_DISCARD std::string getInfoLog() const;
		IC3_ATTR_NO_DISCARD std::string getSource() const;

		IC3_ATTR_NO_DISCARD size_t getInfoLogLength() const;
		IC3_ATTR_NO_DISCARD size_t getSourceLength() const;

		IC3_ATTR_NO_DISCARD bool isInfoLogEmpty() const;
		IC3_ATTR_NO_DISCARD bool isSourceEmpty() const;

		static bool checkBinaryCodeSupport();

		static bool checkBinaryFormatSupport( GLenum pFormat );

		static GLbitfield getStageMaskForEShaderType( GLenum pGLShaderType );

		static GLShaderObjectHandle create( GLenum pGLShaderType );
		static GLShaderObjectHandle createWithSource( GLenum pGLShaderType, const void * pShaderSource, size_t pSourceLength );

	private:
		std::unique_ptr<GLShaderDataLayoutMap> _dataLayoutMap;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_GLCOMMON_SHADER_OBJECT_H__
