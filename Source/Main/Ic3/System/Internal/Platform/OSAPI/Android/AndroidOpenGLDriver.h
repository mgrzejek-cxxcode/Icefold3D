
#ifndef __IC3_SYSTEM_PLATFORM_OSAPI_ANDROID_OPENGL_DRIVER_H__
#define __IC3_SYSTEM_PLATFORM_OSAPI_ANDROID_OPENGL_DRIVER_H__

#include "AndroidCommon.h"
#include <Ic3/System/OpenGLDriver.h>
#include <Ic3/System/Internal/Platform/Shared/EGL/EGLOpenGL.h>
#include <GLES3/gl31.h>

namespace Ic3::System
{

	Ic3SysDeclareHandle( AndroidDisplayManager );
	Ic3SysDeclareHandle( AndroidOpenGLSystemDriver );
	Ic3SysDeclareHandle( AndroidOpenGLRenderContext );
	Ic3SysDeclareHandle( AndroidOpenGLRenderContext );

	namespace Platform
	{

		struct AndroidOpenGLDisplaySurfaceNativeData : public EGLDisplaySurfaceNativeData, public AndroidNativeDataCommon
		{
		};

		struct AndroidOpenGLRenderContextNativeData : public EGLRenderContextNativeData, public AndroidNativeDataCommon
		{
		};

		struct AndroidOpenGLSystemDriverNativeData : public EGLDriverNativeData, public AndroidNativeDataCommon
		{
		};

	}

	/// @brief Android-specific implementation of the OpenGLSystemDriver class.
	class AndroidOpenGLSystemDriver : public AndroidNativeObject<OpenGLSystemDriver, Platform::AndroidOpenGLSystemDriverNativeData>
	{
	public:
		AndroidOpenGLSystemDriver( AndroidDisplayManagerHandle pDisplayManager );
		virtual ~AndroidOpenGLSystemDriver() noexcept;

	private:
		void _InitializeAndroidDriverState();

		void _ReleaseAndroidDriverState();

		/// @copybrief OpenGLSystemDriver::_NativeInitializePlatform
		virtual void _NativeInitializePlatform() override final;

		/// @copybrief OpenGLSystemDriver::_NativeReleaseInitState
		virtual void _NativeReleaseInitState() noexcept override final;

		/// @copybrief OpenGLSystemDriver::_NativeCreateDisplaySurface
		virtual OpenGLDisplaySurfaceHandle _NativeCreateDisplaySurface(
				const OpenGLDisplaySurfaceCreateInfo & pCreateInfo ) override final;

		/// @copybrief OpenGLSystemDriver::_NativeCreateDisplaySurfaceForCurrentThread
		virtual OpenGLDisplaySurfaceHandle _NativeCreateDisplaySurfaceForCurrentThread() override final;

		/// @copybrief OpenGLSystemDriver::_NativeDestroyDisplaySurface
		virtual void _NativeDestroyDisplaySurface( OpenGLDisplaySurface & pDisplaySurface ) override final;

		/// @copybrief OpenGLSystemDriver::_NativeCreateRenderContext
		virtual OpenGLRenderContextHandle _NativeCreateRenderContext(
				OpenGLDisplaySurface & pDisplaySurface,
				const OpenGLRenderContextCreateInfo & pCreateInfo ) override final;

		/// @copybrief OpenGLSystemDriver::_NativeCreateRenderContextForCurrentThread
		virtual OpenGLRenderContextHandle _NativeCreateRenderContextForCurrentThread() override final;

		/// @copybrief OpenGLSystemDriver::_NativeDestroyDisplaySurface
		virtual void _NativeDestroyRenderContext( OpenGLRenderContext & pRenderContext ) override final;

		/// @copybrief OpenGLSystemDriver::_NativeResetContextBinding
		virtual void _NativeResetContextBinding() override final;

		/// @copybrief OpenGLSystemDriver::_NativeQuerySupportedDepthStencilFormats
		virtual std::vector<EDepthStencilFormat> _NativeQuerySupportedDepthStencilFormats(
				EColorFormat pColorFormat ) const override final;

		/// @copybrief OpenGLSystemDriver::_NativeQuerySupportedMSAAModes
		virtual std::vector<EMSAAMode> _NativeQuerySupportedMSAAModes(
				EColorFormat pColorFormat,
				EDepthStencilFormat pDepthStencilFormat ) const override final;

		/// @copybrief OpenGLSystemDriver::_NativeIssAPIClassSupported
		virtual bool _NativeIsAPIClassSupported( EOpenGLAPIClass pAPIClass ) const override final;;

		/// @copybrief OpenGLSystemDriver::_NativeIsRenderContextBound
		virtual bool _NativeIsRenderContextBound() const override final;
	};

	/// @brief Android-specific implementation of the OpenGLDisplaySurface class.
	class AndroidOpenGLDisplaySurface : public AndroidNativeObject<OpenGLDisplaySurface, Platform::AndroidOpenGLDisplaySurfaceNativeData>
	{
	public:
		explicit AndroidOpenGLDisplaySurface( AndroidOpenGLSystemDriverHandle pGLSystemDriver );
		virtual ~AndroidOpenGLDisplaySurface() noexcept;

	private:
		void _ReleaseAndroidSurfaceState();

		/// @copybrief OpenGLDisplaySurface::_NativeSwapBuffers
		virtual void _NativeSwapBuffers() override final;

		/// @copybrief OpenGLDisplaySurface::_NativeQueryRenderAreaSize
		virtual FrameSize _NativeQueryRenderAreaSize() const override final;

		/// @copybrief OpenGLDisplaySurface::_NativeSysValidate
		virtual bool _NativeSysValidate() const override final;

		/// @copybrief OpenGLDisplaySurface::_NativeResize
		virtual void _NativeResize( const FrameSize & pFrameSize, EFrameSizeMode pSizeMode ) override final;

		/// @copybrief OpenGLDisplaySurface::_NativeSetFullscreenMode
		virtual void _NativeSetFullscreenMode( bool pEnable ) override final;

		/// @copybrief OpenGLDisplaySurface::_NativeSetTitle
		virtual void _NativeSetTitle( const std::string & pTitle ) override final;

		/// @copybrief OpenGLDisplaySurface::_NativeUpdateGeometry
		virtual void _NativeUpdateGeometry(
				const FrameGeometry & pFrameGeometry,
				cppx::bitmask<EFrameGeometryUpdateFlags> pUpdateFlags ) override final;

		/// @copybrief OpenGLDisplaySurface::_NativeGetSize
		virtual FrameSize _NativeGetSize( EFrameSizeMode pSizeMode ) const override final;
	};

	/// @brief Android-specific implementation of the OpenGLRenderContext class.
	class AndroidOpenGLRenderContext : public AndroidNativeObject<OpenGLRenderContext, Platform::AndroidOpenGLRenderContextNativeData>
	{
	public:
		explicit AndroidOpenGLRenderContext( AndroidOpenGLSystemDriverHandle pGLSystemDriver );
		virtual ~AndroidOpenGLRenderContext() noexcept;

	private:
		void _ReleaseAndroidContextState();

		/// @copybrief OpenGLRenderContext::_NativeBindForCurrentThread
		virtual void _NativeBindForCurrentThread( const OpenGLDisplaySurface & pTargetSurface ) override final;

		/// @copybrief OpenGLRenderContext::_NativeIsCurrent
		virtual bool _NativeIsCurrent() const override final;

		/// @copybrief OpenGLRenderContext::_NativeSysValidate
		virtual bool _NativeSysValidate() const override final;
	};

} // namespace Ic3::System

#endif // __IC3_SYSTEM_PLATFORM_OSAPI_ANDROID_OPENGL_DRIVER_H__
