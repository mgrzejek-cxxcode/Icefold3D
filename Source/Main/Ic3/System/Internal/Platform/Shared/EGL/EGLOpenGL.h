
#ifndef __IC3_SYSTEM_PLATFORM_OSAPI_EGL_OPENGL_DRIVER_H__
#define __IC3_SYSTEM_PLATFORM_OSAPI_EGL_OPENGL_DRIVER_H__

#include <Ic3/System/OpenGLCommon.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include "eglCoreUtils.h"

namespace Ic3::System
{

	namespace Platform
	{

		inline constexpr size_t cxEGLMaxEGLConfigsNum = 256u;

		inline constexpr size_t cxEGLMaxEGLConfigAttributesNum = 64u;

		struct EGLDisplaySurfaceNativeData
		{
			EGLDisplay mEGLDisplay = EGL_NO_DISPLAY;
			EGLNativeWindowType mEGLNativeWindowHandle = nullptr;
			EGLConfig mEGLFBConfig = nullptr;
			EGLSurface mEGLSurfaceHandle = nullptr;
		};

		struct EGLRenderContextNativeData
		{
			EGLDisplay mEGLDisplay = EGL_NO_DISPLAY;
			EGLContext mEGLContextHandle = nullptr;
		};

		struct EGLDriverNativeData
		{
			EGLDisplay mEGLDisplay = EGL_NO_DISPLAY;
			EGLNativeWindowType mEGLNativeWindowHandle = nullptr;
			Version mEGLVersion;
		};

		void eglInitializeGLDriver( EGLDriverNativeData & pEGLDriverNativeData );

		void eglReleaseGLDriver( EGLDriverNativeData & pEGLDriverNativeData );

		EGLConfig eglChooseCoreFBConfig( EGLDisplay pDisplay,
		                                 const VisualConfig & pVisualConfig,
		                                 const Version & pTargetAPIVersion );

		EGLint eglQueryFBConfigAttribute( EGLDisplay pEGLDisplay, EGLConfig pEGLConfig, EGLenum pAttribute );

		void eglCreateSurface( EGLDisplaySurfaceNativeData & pEGLSurfaceNativeData,
		                       EGLDisplay pEGLDisplay,
		                       EGLNativeWindowType pWindow,
		                       EGLConfig pEGLConfig,
		                       const VisualConfig & pVisualConfig );

		void eglCreateSurfaceForCurrentThread( EGLDisplaySurfaceNativeData & pEGLSurfaceNativeData );

		void eglDestroySurface( EGLDisplaySurfaceNativeData & pEGLSurfaceNativeData );

		void eglCreateCoreContext( EGLRenderContextNativeData & pEGLContextNativeData,
		                           const EGLDisplaySurfaceNativeData & pEGLSurfaceNativeData,
		                           const OpenGLRenderContextCreateInfo & pCreateInfo );

		void eglCreateCoreContextForCurrentThread( EGLRenderContextNativeData & pEGLContextNativeData );

		void eglDestroyRenderContext( EGLRenderContextNativeData & pEGLContextNativeData );

		void eglBindContextForCurrentThread( const EGLRenderContextNativeData & pEGLContextNativeData,
		                                     const EGLDisplaySurfaceNativeData & pEGLSurfaceNativeData );

	}

} // namespace Ic3::System

#endif // __IC3_SYSTEM_PLATFORM_OSAPI_EGL_OPENGL_DRIVER_H__
