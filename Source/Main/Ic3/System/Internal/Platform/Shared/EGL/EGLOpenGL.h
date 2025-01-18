
#ifndef __IC3_SYSTEM_PLATFORM_SHARED_EGL_OPENGL_DRIVER_H__
#define __IC3_SYSTEM_PLATFORM_SHARED_EGL_OPENGL_DRIVER_H__

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
			EGLDisplay eglDisplay = EGL_NO_DISPLAY;
			EGLNativeWindowType eglNativeWindowHandle = nullptr;
			EGLConfig eglFBConfig = nullptr;
			EGLSurface eglSurfaceHandle = nullptr;
		};

		struct EGLRenderContextNativeData
		{
			EGLDisplay eglDisplay = EGL_NO_DISPLAY;
			EGLContext eglContextHandle = nullptr;
		};

		struct EGLDriverNativeData
		{
			EGLDisplay eglDisplay = EGL_NO_DISPLAY;
			EGLNativeWindowType eglNativeWindowHandle = nullptr;
			cppx::version eglVersion;
		};

		void EAInitializeGLDriver( EGLDriverNativeData & pEGLDriverNativeData );

		void EAReleaseGLDriver( EGLDriverNativeData & pEGLDriverNativeData );

		EGLConfig EAChooseCoreFBConfig(
				EGLDisplay pDisplay,
				const VisualConfig & pVisualConfig,
				const cppx::version & pTargetAPIVersion );

		EGLint EAQueryFBConfigAttribute( EGLDisplay pEGLDisplay, EGLConfig pEGLConfig, EGLenum pAttribute );

		void EACreateSurface(
				EGLDisplaySurfaceNativeData & pEGLSurfaceNativeData,
				EGLDisplay pEGLDisplay,
				EGLNativeWindowType pWindow,
				EGLConfig pEGLConfig,
				const VisualConfig & pVisualConfig );

		void EACreateSurfaceForCurrentThread( EGLDisplaySurfaceNativeData & pEGLSurfaceNativeData );

		void EADestroySurface( EGLDisplaySurfaceNativeData & pEGLSurfaceNativeData );

		void EACreateCoreContext(
				EGLRenderContextNativeData & pEGLContextNativeData,
				const EGLDisplaySurfaceNativeData & pEGLSurfaceNativeData,
				const OpenGLRenderContextCreateInfo & pCreateInfo );

		void EACreateCoreContextForCurrentThread( EGLRenderContextNativeData & pEGLContextNativeData );

		void EADestroyRenderContext( EGLRenderContextNativeData & pEGLContextNativeData );

		void EABindContextForCurrentThread(
				const EGLRenderContextNativeData & pEGLContextNativeData,
				const EGLDisplaySurfaceNativeData & pEGLSurfaceNativeData );

	}

} // namespace Ic3::System

#endif // __IC3_SYSTEM_PLATFORM_SHARED_EGL_OPENGL_DRIVER_H__
