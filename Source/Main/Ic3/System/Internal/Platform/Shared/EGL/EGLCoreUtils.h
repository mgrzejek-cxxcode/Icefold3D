
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
#  define ic3EGLCheckLastResult()             Platform::EACheckLastResult()
#  define ic3EGLCheckLastError( pErrorCode )  Platform::EACheckLastError( pErrorCode )
#  define ic3EGLHandleLastError()             Platform::EAHandleLastError()
#  define ic3EGLHandleLastError()             Platform::EAHandleLastError()
#  define ic3EGLResetErrorQueue()             Platform::EAResetErrorQueue()
#else
#  define ic3EGLCheckLastResult()
#  define ic3EGLCheckLastError( pErrorCode )
#  define ic3EGLHandleLastError()
#  define ic3EGLResetErrorQueue()
#endif

#endif // __IC3_SYSTEM_PLATFORM_SHARED_EGL_CORE_UTILS_H__
