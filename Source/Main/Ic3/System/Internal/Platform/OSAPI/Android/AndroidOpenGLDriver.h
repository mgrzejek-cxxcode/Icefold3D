
#ifndef __IC3_SYSTEM_PLATFORM_OSAPI_ANDROID_OPENGL_DRIVER_H__
#define __IC3_SYSTEM_PLATFORM_OSAPI_ANDROID_OPENGL_DRIVER_H__

#include "AndroidCommon.h"
#include <Ic3/System/OpenGLDriver.h>
#include <Ic3/System/Internal/Platform/Shared/EGL/EGLOpenGL.h>
#include <GLES3/gl31.h>

namespace Ic3::System
{

	ic3SysDeclareHandle( AndroidDisplayManager );
	ic3SysDeclareHandle( AndroidOpenGLSystemDriver );
	ic3SysDeclareHandle( AndroidOpenGLRenderContext );
	ic3SysDeclareHandle( AndroidOpenGLRenderContext );

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
		void _initializeAndroidDriverState();

		void _releaseAndroidDriverState();

		/// @copybrief OpenGLSystemDriver::_nativeInitializePlatform
		virtual void _nativeInitializePlatform() override final;

		/// @copybrief OpenGLSystemDriver::_nativeReleaseInitState
		virtual void _nativeReleaseInitState() noexcept override final;

		/// @copybrief OpenGLSystemDriver::_nativeCreateDisplaySurface
		virtual OpenGLDisplaySurfaceHandle _nativeCreateDisplaySurface( const OpenGLDisplaySurfaceCreateInfo & pCreateInfo ) override final;

		/// @copybrief OpenGLSystemDriver::_nativeCreateDisplaySurfaceForCurrentThread
		virtual OpenGLDisplaySurfaceHandle _nativeCreateDisplaySurfaceForCurrentThread() override final;

		/// @copybrief OpenGLSystemDriver::_nativeDestroyDisplaySurface
		virtual void _nativeDestroyDisplaySurface( OpenGLDisplaySurface & pDisplaySurface ) override final;

		/// @copybrief OpenGLSystemDriver::_nativeCreateRenderContext
		virtual OpenGLRenderContextHandle _nativeCreateRenderContext( OpenGLDisplaySurface & pDisplaySurface,
		                                                              const OpenGLRenderContextCreateInfo & pCreateInfo ) override final;

		/// @copybrief OpenGLSystemDriver::_nativeCreateRenderContextForCurrentThread
		virtual OpenGLRenderContextHandle _nativeCreateRenderContextForCurrentThread() override final;

		/// @copybrief OpenGLSystemDriver::_nativeDestroyDisplaySurface
		virtual void _nativeDestroyRenderContext( OpenGLRenderContext & pRenderContext ) override final;

		/// @copybrief OpenGLSystemDriver::_nativeResetContextBinding
		virtual void _nativeResetContextBinding() override final;

		/// @copybrief OpenGLSystemDriver::_nativeQuerySupportedDepthStencilFormats
		virtual std::vector<EDepthStencilFormat> _nativeQuerySupportedDepthStencilFormats( EColorFormat pColorFormat ) const override final;

		/// @copybrief OpenGLSystemDriver::_nativeQuerySupportedMSAAModes
		virtual std::vector<EMSAAMode> _nativeQuerySupportedMSAAModes( EColorFormat pColorFormat,
		                                                               EDepthStencilFormat pDepthStencilFormat ) const override final;

		/// @copybrief OpenGLSystemDriver::_nativeIssAPIClassSupported
		virtual bool _nativeIsAPIClassSupported( EOpenGLAPIClass pAPIClass ) const override final;;

		/// @copybrief OpenGLSystemDriver::_nativeIsRenderContextBound
		virtual bool _nativeIsRenderContextBound() const override final;
	};

	/// @brief Android-specific implementation of the OpenGLDisplaySurface class.
	class AndroidOpenGLDisplaySurface : public AndroidNativeObject<OpenGLDisplaySurface, Platform::AndroidOpenGLDisplaySurfaceNativeData>
	{
	public:
		explicit AndroidOpenGLDisplaySurface( AndroidOpenGLSystemDriverHandle pGLSystemDriver );
		virtual ~AndroidOpenGLDisplaySurface() noexcept;

	private:
		void _releaseAndroidSurfaceState();

		/// @copybrief OpenGLDisplaySurface::_nativeSwapBuffers
		virtual void _nativeSwapBuffers() override final;

		/// @copybrief OpenGLDisplaySurface::_nativeQueryRenderAreaSize
		virtual FrameSize _nativeQueryRenderAreaSize() const override final;

		/// @copybrief OpenGLDisplaySurface::_nativeSysValidate
		virtual bool _nativeSysValidate() const override final;

		/// @copybrief OpenGLDisplaySurface::_nativeResize
		virtual void _nativeResize( const FrameSize & pFrameSize, EFrameSizeMode pSizeMode ) override final;

		/// @copybrief OpenGLDisplaySurface::_nativeSetFullscreenMode
		virtual void _nativeSetFullscreenMode( bool pEnable ) override final;

		/// @copybrief OpenGLDisplaySurface::_nativeSetTitle
		virtual void _nativeSetTitle( const std::string & pTitle ) override final;

		/// @copybrief OpenGLDisplaySurface::_nativeUpdateGeometry
		virtual void _nativeUpdateGeometry( const FrameGeometry & pFrameGeometry,
		                                    TBitmask<EFrameGeometryUpdateFlags> pUpdateFlags ) override final;

		/// @copybrief OpenGLDisplaySurface::_nativeGetSize
		virtual FrameSize _nativeGetSize( EFrameSizeMode pSizeMode ) const override final;
	};

	/// @brief Android-specific implementation of the OpenGLRenderContext class.
	class AndroidOpenGLRenderContext : public AndroidNativeObject<OpenGLRenderContext, Platform::AndroidOpenGLRenderContextNativeData>
	{
	public:
		explicit AndroidOpenGLRenderContext( AndroidOpenGLSystemDriverHandle pGLSystemDriver );
		virtual ~AndroidOpenGLRenderContext() noexcept;

	private:
		void _releaseAndroidContextState();

		/// @copybrief OpenGLRenderContext::_nativeBindForCurrentThread
		virtual void _nativeBindForCurrentThread( const OpenGLDisplaySurface & pTargetSurface ) override final;

		/// @copybrief OpenGLRenderContext::_nativeIsCurrent
		virtual bool _nativeIsCurrent() const override final;

		/// @copybrief OpenGLRenderContext::_nativeSysValidate
		virtual bool _nativeSysValidate() const override final;
	};

} // namespace Ic3::System

#endif // __IC3_SYSTEM_PLATFORM_OSAPI_ANDROID_OPENGL_DRIVER_H__
