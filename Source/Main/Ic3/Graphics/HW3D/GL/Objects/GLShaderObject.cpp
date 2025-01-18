
#include "GLShaderObject.h"
#include <cppx/byteArray.h>
#include <cppx/stdHelperAlgo.h>

namespace Ic3::Graphics::GCI
{

	GLShaderObject::GLShaderObject( GLuint pHandle, GLenum pGLShaderType, GLenum pGLShaderStageMaskBit )
	: GLObject( GLObjectBaseType::Shader, pHandle )
	, mGLShaderType( pGLShaderType )
	, mGLShaderStageMaskBit( pGLShaderStageMaskBit )
	{}

	GLShaderObject::~GLShaderObject() = default;

	GLShaderObjectHandle GLShaderObject::Create( GLenum pGLShaderType )
	{
		auto shaderStageMaskBit = GetStageMaskForEShaderType( pGLShaderType );
		if( shaderStageMaskBit == cvGLInvalidValue )
		{
			return nullptr;
		}

		auto shaderHandle = glCreateShader( pGLShaderType );
		Ic3OpenGLHandleLastError();

		GLShaderObjectHandle openglShaderObject{ new GLShaderObject( shaderHandle, pGLShaderType, shaderStageMaskBit ) };

		return openglShaderObject;
	}

	GLShaderObjectHandle GLShaderObject::CreateWithSource( GLenum pGLShaderType, const void * pSource, size_t pSourceLength )
	{
		if( !pSource || ( pSourceLength == 0 ) )
		{
			return nullptr;
		}

		auto shaderObject = Create( pGLShaderType );
		if( !shaderObject )
		{
			return nullptr;
		}

		if( !shaderObject->CompileSource( pSource, pSourceLength ) )
		{
			return nullptr;
		}

		return shaderObject;
	}

	bool GLShaderObject::Release()
	{
		auto deleteStatus = QueryParameter( GL_DELETE_STATUS );
		if( deleteStatus == GL_FALSE )
		{
			glDeleteShader( mGLHandle );
			Ic3OpenGLHandleLastError();

			return true;
		}

		return false;
	}

	bool GLShaderObject::ValidateHandle() const
	{
		GLboolean checkResult = glIsShader( mGLHandle );
		return checkResult == GL_TRUE;
	}

	bool GLShaderObject::CompileSource( const void * pSource, size_t pSourceLength )
	{
		const auto * sourceBuffer = reinterpret_cast<const GLchar*>( pSource );
		const auto sourceLength = static_cast<GLint>( pSourceLength );

		glShaderSource( mGLHandle, 1, &sourceBuffer, &sourceLength );
		Ic3OpenGLHandleLastError();

		glCompileShader( mGLHandle );
		Ic3OpenGLHandleLastError();

		auto compileStatus = QueryParameter( GL_COMPILE_STATUS );
		if ( compileStatus != GL_TRUE )
		{
			auto infoLog = GetInfoLog();
			Ic3DebugOutput( infoLog.data() );
			Ic3DebugInterrupt();
			return false;
		}

		return true;
	}

	bool GLShaderObject::LoadBinary( GLenum pFormat, const void * pBinary, size_t pBinarySize )
	{
		if( !CheckBinaryCodeSupport() )
		{
			return false;
		}

		glShaderBinary( 1, &mGLHandle, pFormat, pBinary, static_cast<GLsizei>( pBinarySize ) );
		Ic3OpenGLHandleLastError();

		return true;
	}

	void GLShaderObject::SetDataLayoutMap( GLShaderDataLayoutMap pLayoutMap )
	{
		_dataLayoutMap = std::make_unique<GLShaderDataLayoutMap>( std::move( pLayoutMap ) );
	}

	GLShaderDataLayoutMap * GLShaderObject::GetDataLayoutMap() const noexcept
	{
		return _dataLayoutMap.get();
	}

	GLint GLShaderObject::QueryParameter( GLenum pParameter ) const
	{
		GLint parameterValue = GL_INVALID_VALUE;

		glGetShaderiv( mGLHandle, pParameter, &parameterValue );
		Ic3OpenGLHandleLastError();

		return parameterValue;
	}

	std::string GLShaderObject::GetInfoLog() const
	{
		std::string infoLog {};

		auto infoLogLength = GetInfoLogLength();
		if( infoLogLength > 0 )
		{
			cppx::dynamic_byte_array infoLogBuffer;
			infoLogBuffer.resize( infoLogLength );

			glGetShaderInfoLog( mGLHandle, static_cast<GLsizei>( infoLogLength ), nullptr, infoLogBuffer.dataAs<GLchar>() );
			Ic3OpenGLHandleLastError();

			// Note: length returned by the GL includes null terminator!
			infoLog.assign( infoLogBuffer.dataAs<GLchar>(), infoLogLength - 1 );
		}

		return infoLog;
	}

	std::string GLShaderObject::GetSource() const
	{
		std::string source {};

		auto sourceLength = GetSourceLength();
		if (sourceLength > 0)
		{
			cppx::dynamic_byte_array sourceBuffer;
			sourceBuffer.resize( sourceLength );

			glGetShaderSource( mGLHandle, static_cast<GLsizei>( sourceLength ), nullptr, sourceBuffer.dataAs<GLchar>() );
			Ic3OpenGLHandleLastError();

			// Note: length returned by the GL includes null terminator!
			source.assign( sourceBuffer.dataAs<GLchar>(), sourceLength - 1 );
		}

		return source;
	}

	size_t GLShaderObject::GetInfoLogLength() const
	{
		auto infoLogLength = QueryParameter( GL_INFO_LOG_LENGTH );
		return infoLogLength;
	}

	size_t GLShaderObject::GetSourceLength() const
	{
		auto sourceLength = QueryParameter( GL_SHADER_SOURCE_LENGTH );
		return sourceLength;
	}

	bool GLShaderObject::IsInfoLogEmpty() const
	{
		auto infoLogLength = GetInfoLogLength();
		return infoLogLength == 0;
	}

	bool GLShaderObject::IsSourceEmpty() const
	{
		auto sourceLength = GetSourceLength();
		return sourceLength == 0;
	}

	bool GLShaderObject::CheckBinaryCodeSupport()
	{
		GLint shaderBinaryFormatsNum = 0;
		glGetIntegerv( GL_NUM_SHADER_BINARY_FORMATS, &shaderBinaryFormatsNum );
		Ic3OpenGLHandleLastError();

		return shaderBinaryFormatsNum > 0;
	}

	bool GLShaderObject::CheckBinaryFormatSupport( GLenum pFormat )
	{
		GLint shaderBinaryFormatsNum = 0;
		glGetIntegerv( GL_NUM_SHADER_BINARY_FORMATS, &shaderBinaryFormatsNum );
		Ic3OpenGLHandleLastError();

		return shaderBinaryFormatsNum > 0;
	}

	GLbitfield GLShaderObject::GetStageMaskForEShaderType( GLenum pGLShaderType )
	{
		switch( pGLShaderType )
		{
			Ic3CaseReturn( GL_VERTEX_SHADER   , GL_VERTEX_SHADER_BIT   );
			Ic3CaseReturn( GL_FRAGMENT_SHADER , GL_FRAGMENT_SHADER_BIT );
		#if( IC3_GX_GL_FEATURE_SUPPORT_SHADER_TYPE_GEOMETRY )
			Ic3CaseReturn( GL_GEOMETRY_SHADER , GL_GEOMETRY_SHADER_BIT );
		#endif
		#if( IC3_GX_GL_FEATURE_SUPPORT_SHADER_TYPE_TESSELATION )
			Ic3CaseReturn( GL_TESS_CONTROL_SHADER    , GL_TESS_CONTROL_SHADER_BIT    );
			Ic3CaseReturn( GL_TESS_EVALUATION_SHADER , GL_TESS_EVALUATION_SHADER_BIT );
		#endif
		#if( IC3_GX_GL_FEATURE_SUPPORT_SHADER_TYPE_COMPUTE )
			Ic3CaseReturn( GL_COMPUTE_SHADER , GL_COMPUTE_SHADER_BIT );
		#endif
		};
		return GL_IC3_ERR_INVALID_PARAM;
	}

} // namespace Ic3::Graphics::GCI
