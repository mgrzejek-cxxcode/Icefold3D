
#ifndef __IC3_SYSTEM_PLATFORM_OSAPI_OSX_OPENGL_DRIVER_H__
#define __IC3_SYSTEM_PLATFORM_OSAPI_OSX_OPENGL_DRIVER_H__

#include "OSXWindowSystem.h"
#include <Ic3/System/OpenGLDriver.h>

#ifdef __clang__
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif

#import <AppKit/NSOpenGL.h>
#import <Foundation/NSThread.h>
#import <OpenGL/OpenGL.h>

namespace Ic3::System
{

	ic3SysDeclareHandle( OSXOpenGLSystemDriver );
	ic3SysDeclareHandle( OSXOpenGLDisplaySurface );
	ic3SysDeclareHandle( OSXOpenGLRenderContext );

	namespace Platform
	{

		struct OSXOpenGLDisplaySurfaceNativeData : public OSXWindowNativeData
		{
			NSOpenGLPixelFormat * mNSPixelFormat = nil;
		};

		struct OSXOpenGLRenderContextNativeData : public OSXNativeDataCommon
		{
			NSOpenGLContext * mNSContextHandle = nil;
		};

		struct OSXOpenGLSystemDriverNativeData : public OSXNativeDataCommon
		{
		};

		constexpr size_t CX_OSX_MAX_NSGL_FBCONFIG_ATTRIBUTES_NUM = 32u;

	}

	/// @brief OSX-specific implementation of the OpenGLSystemDriver class.
	class OSXOpenGLSystemDriver : public OSXNativeObject<OpenGLSystemDriver, Platform::OSXOpenGLSystemDriverNativeData>
	{
	public:
		OSXOpenGLSystemDriver( OSXDisplayManagerHandle pDisplayManager );
		virtual ~OSXOpenGLSystemDriver() noexcept;

	private:
		void _InitializeOSXDriverState();
		void _ReleaseOSXDriverState();

		/// @copybrief OpenGLSystemDriver::_NativeInitializePlatform
		virtual void _NativeInitializePlatform() override final;

		/// @copybrief OpenGLSystemDriver::_NativeReleaseInitState
		virtual void _NativeReleaseInitState() noexcept override final;

		/// @copybrief OpenGLSystemDriver::_NativeQueryVersionSupportInfo
		virtual OpenGLVersionSupportInfo _NativeQueryVersionSupportInfo() const noexcept override final;

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

	private:
		NSOpenGLPixelFormatAttribute _nsSupportedOpenGLVersion;
	};

	/// @brief OSX-specific implementation of the OpenGLDisplaySurface class.
	class OSXOpenGLDisplaySurface : public OSXNativeObject<OpenGLDisplaySurface, Platform::OSXOpenGLDisplaySurfaceNativeData>
	{
	public:
		explicit OSXOpenGLDisplaySurface( OSXOpenGLSystemDriverHandle pGLSystemDriver );
		virtual ~OSXOpenGLDisplaySurface() noexcept;

	private:
		void _ReleaseOSXSurfaceState();

		/// @copybrief OpenGLDisplaySurface::_NativeSwapBuffers
		virtual void _NativeSwapBuffers() override final;

		/// @copybrief OpenGLDisplaySurface::_NativeQuerySupportedAPIClass
		virtual EOpenGLAPIClass _NativeQuerySupportedAPIClass() const noexcept override final;

		virtual VisualConfig _NativeQueryVisualConfig() const override final;

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

		/// @copybrief OpenGLDisplaySurface::_NativeIsFullscreen
		virtual bool _NativeIsFullscreen() const override final;
	};

	/// @brief OSX-specific implementation of the OpenGLRenderContext class.
	class OSXOpenGLRenderContext : public OSXNativeObject<OpenGLRenderContext, Platform::OSXOpenGLRenderContextNativeData>
	{
	public:
		explicit OSXOpenGLRenderContext( OSXOpenGLSystemDriverHandle pGLSystemDriver );
		virtual ~OSXOpenGLRenderContext() noexcept;

	private:
		void _ReleaseOSXContextState();

		/// @copybrief OpenGLRenderContext::_NativeBindForCurrentThread
		virtual void _NativeBindForCurrentThread( const OpenGLDisplaySurface & pTargetSurface ) override final;

		/// @copybrief OpenGLRenderContext::_NativeIsCurrent
		virtual bool _NativeSysCheckIsCurrent() const override final;

		/// @copybrief OpenGLRenderContext::_NativeSysValidate
		virtual bool _NativeSysValidate() const override final;
	};
	
} // namespace Ic3::System

#ifdef __clang__
#  pragma clang diagnostic pop
#endif

#endif // __IC3_SYSTEM_PLATFORM_OSAPI_OSX_OPENGL_DRIVER_H__
