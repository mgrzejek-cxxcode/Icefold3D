
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
			HDC hdc;
			int pixelFormatIndex;
		};

		struct Win32OpenGLRenderContextNativeData
		{
			HGLRC contextHandle;
		};

		struct Win32OpenGLSystemDriverNativeData
		{
			struct InitState
			{
				Win32OpenGLDisplaySurfaceNativeData surfaceData;
				Win32OpenGLRenderContextNativeData contextData;
			};
			std::unique_ptr<InitState> initState = nullptr;
		};

	}

	/// @brief Win32-specific implementation of the OpenGLSystemDriver class.
	class Win32OpenGLSystemDriver : public Win32NativeObject<OpenGLSystemDriver, Platform::Win32OpenGLSystemDriverNativeData>
	{
	public:
		Win32OpenGLSystemDriver( DisplayManagerHandle pDisplayManager );
		virtual ~Win32OpenGLSystemDriver() noexcept;

	private:
		void _ReleaseWin32DriverState();

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

	/// @brief Win32-specific implementation of the OpenGLDisplaySurface class.
	class Win32OpenGLDisplaySurface : public Win32NativeObject<OpenGLDisplaySurface, Platform::Win32OpenGLDisplaySurfaceNativeData>
	{
	public:
		explicit Win32OpenGLDisplaySurface( Win32OpenGLSystemDriverHandle pGLSystemDriver );
		virtual ~Win32OpenGLDisplaySurface() noexcept;

	private:
		void _ReleaseWin32SurfaceState();

		/// @copybrief OpenGLDisplaySurface::_NativeSwapBuffers
		virtual void _NativeSwapBuffers() override final;

		virtual EOpenGLAPIClass _NativeQuerySupportedAPIClass() const noexcept override final;

		/// @copybrief OpenGLDisplaySurface::_NativeQueryVisualConfig
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

	/// @brief Win32-specific implementation of the OpenGLRenderContext class.
	class Win32OpenGLRenderContext : public Win32NativeObject<OpenGLRenderContext, Platform::Win32OpenGLRenderContextNativeData>
	{
	public:
		explicit Win32OpenGLRenderContext( Win32OpenGLSystemDriverHandle pGLSystemDriver );
		virtual ~Win32OpenGLRenderContext() noexcept;

	private:
		void _ReleaseWin32ContextState();

		/// @copybrief OpenGLRenderContext::_NativeBindForCurrentThread
		virtual void _NativeBindForCurrentThread( const OpenGLDisplaySurface & pTargetSurface ) override final;

		/// @copybrief OpenGLRenderContext::_NativeIsCurrent
		virtual bool _NativeSysCheckIsCurrent() const override final;

		/// @copybrief OpenGLRenderContext::_NativeSysValidate
		virtual bool _NativeSysValidate() const override final;
	};

} // namespace Ic3::System

#endif // __IC3_SYSTEM_PLATFORM_OSAPI_WIN32_OPENGL_DRIVER_H__
