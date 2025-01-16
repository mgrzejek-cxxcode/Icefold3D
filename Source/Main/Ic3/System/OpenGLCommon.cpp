
#include "OpenGLCommon.h"
#include <cppx/stdHelperAlgo.h>
#include <sstream>
#include <unordered_map>

namespace Ic3::System
{

	std::string OpenGLSystemVersionInfo::toString() const
	{
		std::ostringstream strStream;
		strStream << "[OpenGL Rendering Context Information]\n";
		strStream << "> API cppx::version: " << apiVersion.num_major << "." << apiVersion.num_minor << "\n";
		strStream << "> Runtime cppx::version: " << apiVersionStr << "\n";
		strStream << "> GLSL cppx::version: " << glslVersionStr << "\n";
		strStream << "> Renderer ID: " << rendererName << "\n";
		strStream << "> Vendor Name: " << vendorName;
		return strStream.str();
	}

	cppx::version OpenGLCoreAPI::QueryRuntimeVersion()
	{
		cppx::version resultVersion;

		int majorVersion = 0;
		glGetIntegerv( GL_MAJOR_VERSION, &majorVersion );
		resultVersion.num_major = static_cast<uint16>( majorVersion );

		int minorVersion = 0;
		glGetIntegerv( GL_MINOR_VERSION, &minorVersion );
		resultVersion.num_minor = static_cast<uint16>( minorVersion );

		return resultVersion;
	}

	bool OpenGLCoreAPI::CheckLastResult()
	{
		GLenum errorCode = glGetError();
		return errorCode == GL_NO_ERROR;
	}

	bool OpenGLCoreAPI::CheckLastError( GLenum pErrorCode )
	{
		GLenum errorCode = glGetError();
		return ( pErrorCode != GL_NO_ERROR ) && ( errorCode == pErrorCode );
	}

	void OpenGLCoreAPI::HandleLastError()
	{
		GLenum errorCode = glGetError();
		if( errorCode != GL_NO_ERROR )
		{
			auto * errorMessageStr = translateErrorCode( errorCode );
			ic3DebugOutputFmt( "OpenGL API error: %s.", errorMessageStr );
			ic3DebugInterrupt();
		}
	}

	void OpenGLCoreAPI::ResetErrorQueue()
	{
		size_t errorsNum = 0;
		while( true )
		{
			GLenum errorCode = glGetError();
			if( errorCode == GL_NO_ERROR )
			{
				break;
			}
			auto * errorMessageStr = translateErrorCode( errorCode );
			ic3DebugOutputFmt( "OpenGL API error: %s.", errorMessageStr );
			++errorsNum;
		}
	}

	const char * OpenGLCoreAPI::translateErrorCode( GLenum pError )
	{
		static const std::unordered_map<GLenum, const char *> errorStringMap =
		{
		#if( IC3_SYSTEM_GL_PLATFORM_TYPE == IC3_SYSTEM_GL_PLATFORM_TYPE_DESKTOP )
			{ GL_STACK_OVERFLOW  , "GL_STACK_OVERFLOW"  },
			{ GL_STACK_UNDERFLOW , "GL_STACK_UNDERFLOW" },
		#endif
			{ GL_INVALID_ENUM				  , "GL_INVALID_ENUM"				  },
			{ GL_INVALID_VALUE				 , "GL_INVALID_VALUE"				 },
			{ GL_INVALID_OPERATION			 , "GL_INVALID_OPERATION"			 },
			{ GL_INVALID_FRAMEBUFFER_OPERATION , "GL_INVALID_FRAMEBUFFER_OPERATION" },
			{ GL_OUT_OF_MEMORY				 , "GL_OUT_OF_MEMORY"				 },
		};
		return cppx::get_map_value_ref_or_default( errorStringMap, pError, "UNKNOWN_ERROR" );
	}

} // namespace Ic3::System
