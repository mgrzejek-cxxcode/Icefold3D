
#ifndef __IC3_SYSTEM_PLATFORM_OSAPI_X11_OPENGL_DRIVER_H__
#define __IC3_SYSTEM_PLATFORM_OSAPI_X11_OPENGL_DRIVER_H__

#include "X11WindowSystem.h"
#include <Ic3/System/OpenGLDriver.h>
#include <GL/glx.h>

namespace Ic3::System
{

	ic3SysDeclareHandle( X11OpenGLSystemDriver );
	ic3SysDeclareHandle( X11OpenGLRenderContext );
	ic3SysDeclareHandle( X11OpenGLRenderContext );

	namespace Platform
	{

		using GLXFBConfigArray = std::vector<GLXFBConfig>;

		struct X11OpenGLDisplaySurfaceNativeData : public X11WindowNativeData
		{
			GLXFBConfig mFBConfig = nullptr;
			XVisualInfo * mVisualInfo = nullptr;
		};

		struct X11OpenGLRenderContextNativeData : public X11NativeDataCommon
		{
			GLXContext contextHandle = nullptr;
		};

		struct X11OpenGLSystemDriverNativeData : public X11NativeDataCommon
		{
			struct InitState
			{
				X11OpenGLDisplaySurfaceNativeData surfaceData;
				X11OpenGLRenderContextNativeData contextData;
			};
			std::unique_ptr<InitState> initState = nullptr;
		};

		struct X11OpenGLVisualConfig
		{
			GLXFBConfig mFBConfig = nullptr;

			XVisualInfo * mXVisualInfo = nullptr;

			explicit operator bool() const
			{
				return mFBConfig && mXVisualInfo;
			}
		};

		constexpr size_t CX_X11_MAX_GLX_FBCONFIG_ATTRIBUTES_NUM = 64u;

	}


	/// @brief X11-specific implementation of the OpenGLSystemDriver class.
	class X11OpenGLSystemDriver : public X11NativeObject<OpenGLSystemDriver, Platform::X11OpenGLSystemDriverNativeData>
	{
	public:
		X11OpenGLSystemDriver( X11DisplayManagerHandle pDisplayManager );
		virtual ~X11OpenGLSystemDriver() noexcept;

	private:
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

	/// @brief X11-specific implementation of the OpenGLDisplaySurface class.
	class X11OpenGLDisplaySurface : public X11NativeObject<OpenGLDisplaySurface, Platform::X11OpenGLDisplaySurfaceNativeData>
	{
	public:
		explicit X11OpenGLDisplaySurface( X11OpenGLSystemDriverHandle pGLSystemDriver );
		virtual ~X11OpenGLDisplaySurface() noexcept;

	private:
		void _ReleaseX11SurfaceState();

		/// @copybrief OpenGLDisplaySurface::_NativeSwapBuffers
		virtual void _NativeSwapBuffers() override final;

        virtual EOpenGLAPIClass _NativeQuerySupportedAPIClass() const noexcept override final;

		/// @copybrief OpenGLDisplaySurface::_NativeQueryRenderAreaSize
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

	/// @brief X11-specific implementation of the OpenGLRenderContext class.
	class X11OpenGLRenderContext : public X11NativeObject<OpenGLRenderContext, Platform::X11OpenGLRenderContextNativeData>
	{
	public:
		explicit X11OpenGLRenderContext( X11OpenGLSystemDriverHandle pGLSystemDriver );
		virtual ~X11OpenGLRenderContext() noexcept;

	private:
		void _ReleaseX11ContextState();

		/// @copybrief OpenGLRenderContext::_NativeBindForCurrentThread
		virtual void _NativeBindForCurrentThread( const OpenGLDisplaySurface & pTargetSurface ) override final;

		/// @copybrief OpenGLRenderContext::_NativeIsCurrent
		virtual bool _NativeSysCheckIsCurrent() const override final;

		/// @copybrief OpenGLRenderContext::_NativeSysValidate
		virtual bool _NativeSysValidate() const override final;
	};

} // namespace Ic3::System

#endif // __IC3_SYSTEM_PLATFORM_OSAPI_X11_OPENGL_DRIVER_H__
