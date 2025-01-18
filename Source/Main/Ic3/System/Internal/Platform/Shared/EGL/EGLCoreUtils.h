
#ifndef __IC3_SYSTEM_PLATFORM_SHARED_EGL_CORE_UTILS_H__
#define __IC3_SYSTEM_PLATFORM_SHARED_EGL_CORE_UTILS_H__

#include <Ic3/System/OpenGLCommon.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>

namespace Ic3::System
{

	namespace Platform
	{

		struct EGLError
		{
		public:
			EGLenum errorCode;

		public:
			EGLError( EGLenum pErrorCode )
					: errorCode( pErrorCode )
			{}

			constexpr explicit operator bool() const
			{
				return errorCode != EGL_SUCCESS;
			}
		};

		cppx::version EAQueryRuntimeVersion();

		bool EACheckLastResult();

		bool EACheckLastError( EGLenum pErrorCode );

		EGLError EAGetLastError();

		void EAHandleLastError();

		void EAResetErrorQueue();

		const char * EATranslateErrorCode( EGLenum pError );

	}

#define Ic3EGLMakeErrorInfo( pEGLError ) \
	OpenGLErrorInfo( pEGLError, EGLCoreAPI::translateErrorCode( pEGLError ) )

#define Ic3EGLGetLastErrorInfo() \
	Ic3EGLMakeErrorInfo( ::eglGetError() )

#define Ic3EGLThrowError( pErrorDesc ) \
	Ic3ThrowDesc( E_EXC_SYSTEM_OPENGL_SUBSYS_EGL_ERROR, pErrorDesc )

#define Ic3EGLThrowLastError() \
	Ic3ThrowEx( E_EXC_SYSTEM_OPENGL_SUBSYS_EGL_ERROR, Ic3EGLGetLastErrorInfo() )

} // namespace Ic3::System

#if( IC3_SYSTEM_GL_ENABLE_ERROR_CHECKS )
#  define Ic3EGLCheckLastResult()             Platform::EACheckLastResult()
#  define Ic3EGLCheckLastError( pErrorCode )  Platform::EACheckLastError( pErrorCode )
#  define Ic3EGLHandleLastError()             Platform::EAHandleLastError()
#  define Ic3EGLHandleLastError()             Platform::EAHandleLastError()
#  define Ic3EGLResetErrorQueue()             Platform::EAResetErrorQueue()
#else
#  define Ic3EGLCheckLastResult()
#  define Ic3EGLCheckLastError( pErrorCode )
#  define Ic3EGLHandleLastError()
#  define Ic3EGLResetErrorQueue()
#endif

#endif // __IC3_SYSTEM_PLATFORM_SHARED_EGL_CORE_UTILS_H__
