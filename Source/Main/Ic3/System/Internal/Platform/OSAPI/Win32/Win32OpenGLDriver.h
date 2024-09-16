
#ifndef __IC3_SYSTEM_PLATFORM_OSAPI_WIN32_OPENGL_DRIVER_H__
#define __IC3_SYSTEM_PLATFORM_OSAPI_WIN32_OPENGL_DRIVER_H__

#include <Ic3/System/OpenGLDriver.h>
#include "Win32WindowSystem.h"
#include <GL/wglew.h>

namespace Ic3::System
{

	ic3SysDeclareHandle( Win32OpenGLSystemDriver );
	ic3SysDeclareHandle( Win32OpenGLRenderContext );
	ic3SysDeclareHandle( Win32OpenGLRenderContext );

	namespace Platform
	{

		inline constexpr size_t CX_WIN32_WGL_MAX_PIXEL_FORMATS_NUM = 256u;
		
		inline constexpr size_t CX_WIN32_WGL_MAX_PIXEL_FORMAT_ATTRIBUTES_NUM = 64u;

		struct Win32OpenGLDisplaySurfaceNativeData : public Win32WindowNativeData
		{
			HDC mHDC;
			int mPixelFormatIndex;
		};

		struct Win32OpenGLRenderContextNativeData
		{
			HGLRC mContextHandle;
		};

		struct Win32OpenGLSystemDriverNativeData
		{
			struct InitState
			{
				Win32OpenGLDisplaySurfaceNativeData mSurfaceData;
				Win32OpenGLRenderContextNativeData mContextData;
			};
			std::unique_ptr<InitState> mInitState = nullptr;
		};

	}

	/// @brief Win32-specific implementation of the OpenGLSystemDriver class.
	class Win32OpenGLSystemDriver : public Win32NativeObject<OpenGLSystemDriver, Platform::Win32OpenGLSystemDriverNativeData>
	{
	public:
		Win32OpenGLSystemDriver( DisplayManagerHandle pDisplayManager );
		virtual ~Win32OpenGLSystemDriver() noexcept;

	private:
		void _releaseWin32DriverState();

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

	/// @brief Win32-specific implementation of the OpenGLDisplaySurface class.
	class Win32OpenGLDisplaySurface : public Win32NativeObject<OpenGLDisplaySurface, Platform::Win32OpenGLDisplaySurfaceNativeData>
	{
	public:
		explicit Win32OpenGLDisplaySurface( Win32OpenGLSystemDriverHandle pGLSystemDriver );
		virtual ~Win32OpenGLDisplaySurface() noexcept;

	private:
		void _releaseWin32SurfaceState();

		/// @copybrief OpenGLDisplaySurface::_nativeSwapBuffers
		virtual void _nativeSwapBuffers() override final;

		virtual EOpenGLAPIClass _nativeQuerySupportedAPIClass() const noexcept override final;

		/// @copybrief OpenGLDisplaySurface::_nativeQueryVisualConfig
		virtual VisualConfig _nativeQueryVisualConfig() const override final;

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

        /// @copybrief OpenGLDisplaySurface::_nativeIsFullscreen
        virtual bool _nativeIsFullscreen() const override final;
	};

	/// @brief Win32-specific implementation of the OpenGLRenderContext class.
	class Win32OpenGLRenderContext : public Win32NativeObject<OpenGLRenderContext, Platform::Win32OpenGLRenderContextNativeData>
	{
	public:
		explicit Win32OpenGLRenderContext( Win32OpenGLSystemDriverHandle pGLSystemDriver );
		virtual ~Win32OpenGLRenderContext() noexcept;

	private:
		void _releaseWin32ContextState();

		/// @copybrief OpenGLRenderContext::_nativeBindForCurrentThread
		virtual void _nativeBindForCurrentThread( const OpenGLDisplaySurface & pTargetSurface ) override final;

		/// @copybrief OpenGLRenderContext::_nativeIsCurrent
		virtual bool _nativeSysCheckIsCurrent() const override final;

		/// @copybrief OpenGLRenderContext::_nativeSysValidate
		virtual bool _nativeSysValidate() const override final;
	};

} // namespace Ic3::System

#endif // __IC3_SYSTEM_PLATFORM_OSAPI_WIN32_OPENGL_DRIVER_H__
