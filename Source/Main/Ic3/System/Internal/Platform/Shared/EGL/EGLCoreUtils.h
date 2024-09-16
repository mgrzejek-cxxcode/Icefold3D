
#ifndef __IC3_SYSTEM_PLATFORM_OSAPI_EGL_CORE_UTILS_H__
#define __IC3_SYSTEM_PLATFORM_OSAPI_EGL_CORE_UTILS_H__

#include <Ic3/System/OpenGLCommon.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>

namespace Ic3::System
{

	struct EGLError
	{
	public:
		EGLenum mErrorCode;

	public:
		EGLError( EGLenum pErrorCode )
		: mErrorCode( pErrorCode )
		{}

		constexpr explicit operator bool() const
		{
			return mErrorCode != EGL_SUCCESS;
		}
	};

	class EGLCoreAPI
	{
	public:
		static Version queryRuntimeVersion();

		static bool checkLastResult();

		static bool checkLastError( EGLenum pErrorCode );

		static EGLError getLastError();

		static void handleLastError();

		static void resetErrorQueue();

		static const char * translateErrorCode( EGLenum pError );
	};

#define ic3EGLMakeErrorInfo( pEGLError ) \
	OpenGLErrorInfo( pEGLError, EGLCoreAPI::translateErrorCode( pEGLError ) )

#define ic3EGLGetLastErrorInfo() \
	ic3EGLMakeErrorInfo( ::eglGetError() )

#define ic3EGLThrowError( pErrorDesc ) \
	ic3ThrowDesc( E_EXC_SYSTEM_OPENGL_SUBSYS_EGL_ERROR, pErrorDesc )

#define ic3EGLThrowLastError() \
	ic3ThrowEx( E_EXC_SYSTEM_OPENGL_SUBSYS_EGL_ERROR, ic3EGLGetLastErrorInfo() )

} // namespace Ic3::System

#if( IC3_SYSTEM_GL_ENABLE_ERROR_CHECKS )
#  define ic3EGLCheckLastResult()             EGLCoreAPI::checkLastResult()
#  define ic3EGLCheckLastError( pErrorCode )  EGLCoreAPI::checkLastError( pErrorCode )
#  define ic3EGLHandleLastError()             EGLCoreAPI::handleLastError()
#  define ic3EGLHandleLastError()             EGLCoreAPI::handleLastError()
#  define ic3EGLResetErrorQueue()             EGLCoreAPI::resetErrorQueue()
#else
#  define ic3EGLCheckLastResult()
#  define ic3EGLCheckLastError( pErrorCode )
#  define ic3EGLHandleLastError()
#  define ic3EGLResetErrorQueue()
#endif

#endif // __IC3_SYSTEM_PLATFORM_OSAPI_EGL_CORE_UTILS_H__
