
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
			GLXContext mContextHandle = nullptr;
		};

		struct X11OpenGLSystemDriverNativeData : public X11NativeDataCommon
		{
			struct InitState
			{
				X11OpenGLDisplaySurfaceNativeData mSurfaceData;
				X11OpenGLRenderContextNativeData mContextData;
			};
			std::unique_ptr<InitState> mInitState = nullptr;
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

	/// @brief X11-specific implementation of the OpenGLDisplaySurface class.
	class X11OpenGLDisplaySurface : public X11NativeObject<OpenGLDisplaySurface, Platform::X11OpenGLDisplaySurfaceNativeData>
	{
	public:
		explicit X11OpenGLDisplaySurface( X11OpenGLSystemDriverHandle pGLSystemDriver );
		virtual ~X11OpenGLDisplaySurface() noexcept;

	private:
		void _releaseX11SurfaceState();

		/// @copybrief OpenGLDisplaySurface::_nativeSwapBuffers
		virtual void _nativeSwapBuffers() override final;

        virtual EOpenGLAPIClass _nativeQuerySupportedAPIClass() const noexcept override final;

		/// @copybrief OpenGLDisplaySurface::_nativeQueryRenderAreaSize
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

	/// @brief X11-specific implementation of the OpenGLRenderContext class.
	class X11OpenGLRenderContext : public X11NativeObject<OpenGLRenderContext, Platform::X11OpenGLRenderContextNativeData>
	{
	public:
		explicit X11OpenGLRenderContext( X11OpenGLSystemDriverHandle pGLSystemDriver );
		virtual ~X11OpenGLRenderContext() noexcept;

	private:
		void _releaseX11ContextState();

		/// @copybrief OpenGLRenderContext::_nativeBindForCurrentThread
		virtual void _nativeBindForCurrentThread( const OpenGLDisplaySurface & pTargetSurface ) override final;

		/// @copybrief OpenGLRenderContext::_nativeIsCurrent
		virtual bool _nativeSysCheckIsCurrent() const override final;

		/// @copybrief OpenGLRenderContext::_nativeSysValidate
		virtual bool _nativeSysValidate() const override final;
	};

} // namespace Ic3::System

#endif // __IC3_SYSTEM_PLATFORM_OSAPI_X11_OPENGL_DRIVER_H__
