
#include "EGLCoreUtils.h"
#include <cppx/stdHelperAlgo.h>
#include <cppx/stringExt.h>
#include <cppx/stringUtils.h>
#include <unordered_map>

namespace Ic3::System
{

	namespace Platform
	{

		cppx::version EAQueryRuntimeVersion()
		{
			EGLDisplay eDisplay = ::eglGetCurrentDisplay();
			std::string versionString = ::eglQueryString( eDisplay, EGL_VERSION );

			auto dotPos = versionString.find_first_of( '.' );
			auto endPos = versionString.find_first_not_of( "0123456789", dotPos );

			cppx::version resultVersion;

			const auto majorVersion = cppx::from_string<int32>( versionString.substr( 0, dotPos ) );
			resultVersion.num_major = static_cast<uint16>( majorVersion );

			const auto minorVersion = cppx::from_string<int32>( versionString.substr( dotPos + 1, endPos ) );
			resultVersion.num_minor = static_cast<uint16>( minorVersion );

			return resultVersion;
		}

		bool EACheckLastResult()
		{
			GLenum errorCode = ::eglGetError();
			return errorCode == EGL_SUCCESS;
		}

		bool EACheckLastError( GLenum pErrorCode )
		{
			GLenum errorCode = ::eglGetError();
			return ( pErrorCode != EGL_SUCCESS ) && ( errorCode == pErrorCode );
		}

		EGLError EAGetLastError()
		{
			GLenum errorCode = ::eglGetError();
			return EGLError{ errorCode };
		}

		void EAHandleLastError()
		{
			GLenum errorCode = ::eglGetError();
			if( errorCode != EGL_SUCCESS )
			{
				auto * errorMessageStr = EATranslateErrorCode( errorCode );
				ic3DebugOutputFmt( "OpenGL API error: %s.", errorMessageStr );
				 ic3DebugInterrupt();
			}
		}

		void EAResetErrorQueue()
		{
			size_t errorsNum = 0;
			while( true )
			{
				GLenum errorCode = ::eglGetError();
				if( errorCode == EGL_SUCCESS )
				{
					break;
				}
				auto * errorMessageStr = EATranslateErrorCode( errorCode );
				ic3DebugOutputFmt( "OpenGL API error: %s.", errorMessageStr );
				++errorsNum;
			}
		}

		const char * EATranslateErrorCode( GLenum pError )
		{
			switch( pError )
			{
			case EGL_SUCCESS             : return "EGL_SUCCESS"             ;
			case EGL_NOT_INITIALIZED     : return "EGL_NOT_INITIALIZED"     ;
			case EGL_BAD_ACCESS          : return "EGL_BAD_ACCESS"          ;
			case EGL_BAD_ALLOC           : return "EGL_BAD_ALLOC"           ;
			case EGL_BAD_ATTRIBUTE       : return "EGL_BAD_ATTRIBUTE"       ;
			case EGL_BAD_CONFIG          : return "EGL_BAD_CONFIG"          ;
			case EGL_BAD_CONTEXT         : return "EGL_BAD_CONTEXT"         ;
			case EGL_BAD_CURRENT_SURFACE : return "EGL_BAD_CURRENT_SURFACE" ;
			case EGL_BAD_DISPLAY		 : return "EGL_BAD_DISPLAY"         ;
			case EGL_BAD_MATCH           : return "EGL_BAD_MATCH"           ;
			case EGL_BAD_NATIVE_PIXMAP   : return "EGL_BAD_NATIVE_PIXMAP"   ;
			case EGL_BAD_NATIVE_WINDOW   : return "EGL_BAD_NATIVE_WINDOW"   ;
			case EGL_BAD_PARAMETER       : return "EGL_BAD_PARAMETER"       ;
			case EGL_BAD_SURFACE         : return "EGL_BAD_SURFACE"         ;
			case EGL_CONTEXT_LOST        : return "EGL_CONTEXT_LOST"        ;
			}
			return "<UNKNOWN>";
		}

	} // namespace Platform

}
