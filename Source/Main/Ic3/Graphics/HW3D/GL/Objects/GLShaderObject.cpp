
#include "GLShaderObject.h"
#include <Ic3/Cppx/ByteArray.h>
#include <Ic3/Cppx/STLHelperAlgo.h>

namespace Ic3::Graphics::GCI
{

	GLShaderObject::GLShaderObject( GLuint pHandle, GLenum pGLShaderType, GLenum pGLShaderStageMaskBit )
	: GLObject( GLObjectBaseType::Shader, pHandle )
	, mGLShaderType( pGLShaderType )
	, mGLShaderStageMaskBit( pGLShaderStageMaskBit )
	{}

	GLShaderObject::~GLShaderObject() = default;

	GLShaderObjectHandle GLShaderObject::create( GLenum pGLShaderType )
	{
		auto shaderStageMaskBit = getStageMaskForEShaderType( pGLShaderType );
		if( shaderStageMaskBit == cvGLInvalidValue )
		{
			return nullptr;
		}

		auto shaderHandle = glCreateShader( pGLShaderType );
		ic3OpenGLHandleLastError();

		GLShaderObjectHandle openglShaderObject{ new GLShaderObject( shaderHandle, pGLShaderType, shaderStageMaskBit ) };

		return openglShaderObject;
	}

	GLShaderObjectHandle GLShaderObject::createWithSource( GLenum pGLShaderType, const void * pSource, size_t pSourceLength )
	{
		if( !pSource || ( pSourceLength == 0 ) )
		{
			return nullptr;
		}

		auto shaderObject = create( pGLShaderType );
		if( !shaderObject )
		{
			return nullptr;
		}

		if( !shaderObject->compileSource( pSource, pSourceLength ) )
		{
			return nullptr;
		}

		return shaderObject;
	}

	bool GLShaderObject::release()
	{
		auto deleteStatus = queryParameter( GL_DELETE_STATUS );
		if( deleteStatus == GL_FALSE )
		{
			glDeleteShader( mGLHandle );
			ic3OpenGLHandleLastError();

			return true;
		}

		return false;
	}

	bool GLShaderObject::validateHandle() const
	{
		GLboolean checkResult = glIsShader( mGLHandle );
		return checkResult == GL_TRUE;
	}

	bool GLShaderObject::compileSource( const void * pSource, size_t pSourceLength )
	{
		const auto * sourceBuffer = reinterpret_cast<const GLchar*>( pSource );
		const auto sourceLength = static_cast<GLint>( pSourceLength );

		glShaderSource( mGLHandle, 1, &sourceBuffer, &sourceLength );
		ic3OpenGLHandleLastError();

		glCompileShader( mGLHandle );
		ic3OpenGLHandleLastError();

		auto compileStatus = queryParameter( GL_COMPILE_STATUS );
		if ( compileStatus != GL_TRUE )
		{
			auto infoLog = getInfoLog();
			ic3DebugOutput( infoLog.data() );
			ic3DebugInterrupt();
			return false;
		}

		return true;
	}

	bool GLShaderObject::loadBinary( GLenum pFormat, const void * pBinary, size_t pBinarySize )
	{
		if( !checkBinaryCodeSupport() )
		{
			return false;
		}

		glShaderBinary( 1, &mGLHandle, pFormat, pBinary, static_cast<GLsizei>( pBinarySize ) );
		ic3OpenGLHandleLastError();

		return true;
	}

	void GLShaderObject::setDataLayoutMap( GLShaderDataLayoutMap pLayoutMap )
	{
		_dataLayoutMap = std::make_unique<GLShaderDataLayoutMap>( std::move( pLayoutMap ) );
	}

	GLShaderDataLayoutMap * GLShaderObject::getDataLayoutMap() const noexcept
	{
		return _dataLayoutMap.get();
	}

	GLint GLShaderObject::queryParameter( GLenum pParameter ) const
	{
		GLint parameterValue = GL_INVALID_VALUE;

		glGetShaderiv( mGLHandle, pParameter, &parameterValue );
		ic3OpenGLHandleLastError();

		return parameterValue;
	}

	std::string GLShaderObject::getInfoLog() const
	{
		std::string infoLog {};

		auto infoLogLength = getInfoLogLength();
		if( infoLogLength > 0 )
		{
			Ic3::DynamicByteArray infoLogBuffer;
			infoLogBuffer.resize( infoLogLength );

			glGetShaderInfoLog( mGLHandle, static_cast<GLsizei>( infoLogLength ), nullptr, infoLogBuffer.dataAs<GLchar>() );
			ic3OpenGLHandleLastError();

			// Note: length returned by the GL includes null terminator!
			infoLog.assign( infoLogBuffer.dataAs<GLchar>(), infoLogLength - 1 );
		}

		return infoLog;
	}

	std::string GLShaderObject::getSource() const
	{
		std::string source {};

		auto sourceLength = getSourceLength();
		if (sourceLength > 0)
		{
			Ic3::DynamicByteArray sourceBuffer;
			sourceBuffer.resize( sourceLength );

			glGetShaderSource( mGLHandle, static_cast<GLsizei>( sourceLength ), nullptr, sourceBuffer.dataAs<GLchar>() );
			ic3OpenGLHandleLastError();

			// Note: length returned by the GL includes null terminator!
			source.assign( sourceBuffer.dataAs<GLchar>(), sourceLength - 1 );
		}

		return source;
	}

	size_t GLShaderObject::getInfoLogLength() const
	{
		auto infoLogLength = queryParameter( GL_INFO_LOG_LENGTH );
		return infoLogLength;
	}

	size_t GLShaderObject::getSourceLength() const
	{
		auto sourceLength = queryParameter( GL_SHADER_SOURCE_LENGTH );
		return sourceLength;
	}

	bool GLShaderObject::isInfoLogEmpty() const
	{
		auto infoLogLength = getInfoLogLength();
		return infoLogLength == 0;
	}

	bool GLShaderObject::isSourceEmpty() const
	{
		auto sourceLength = getSourceLength();
		return sourceLength == 0;
	}

	bool GLShaderObject::checkBinaryCodeSupport()
	{
		GLint shaderBinaryFormatsNum = 0;
		glGetIntegerv( GL_NUM_SHADER_BINARY_FORMATS, &shaderBinaryFormatsNum );
		ic3OpenGLHandleLastError();

		return shaderBinaryFormatsNum > 0;
	}

	bool GLShaderObject::checkBinaryFormatSupport( GLenum pFormat )
	{
		GLint shaderBinaryFormatsNum = 0;
		glGetIntegerv( GL_NUM_SHADER_BINARY_FORMATS, &shaderBinaryFormatsNum );
		ic3OpenGLHandleLastError();

		return shaderBinaryFormatsNum > 0;
	}

	GLbitfield GLShaderObject::getStageMaskForEShaderType( GLenum pGLShaderType )
	{
		switch( pGLShaderType )
		{
			ic3CaseReturn( GL_VERTEX_SHADER   , GL_VERTEX_SHADER_BIT   );
			ic3CaseReturn( GL_FRAGMENT_SHADER , GL_FRAGMENT_SHADER_BIT );
		#if( ICFGX_GL_FEATURE_SUPPORT_SHADER_TYPE_GEOMETRY )
			ic3CaseReturn( GL_GEOMETRY_SHADER , GL_GEOMETRY_SHADER_BIT );
		#endif
		#if( ICFGX_GL_FEATURE_SUPPORT_SHADER_TYPE_TESSELATION )
			ic3CaseReturn( GL_TESS_CONTROL_SHADER    , GL_TESS_CONTROL_SHADER_BIT    );
			ic3CaseReturn( GL_TESS_EVALUATION_SHADER , GL_TESS_EVALUATION_SHADER_BIT );
		#endif
		#if( ICFGX_GL_FEATURE_SUPPORT_SHADER_TYPE_COMPUTE )
			ic3CaseReturn( GL_COMPUTE_SHADER , GL_COMPUTE_SHADER_BIT );
		#endif
		};
		return GL_IC3_ERR_INVALID_PARAM;
	}

} // namespace Ic3::Graphics::GCI
