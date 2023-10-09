
#ifndef __IC3_SYSTEM_PLATFORM_OSAPI_X11_WINDOW_SYSTEM_H__
#define __IC3_SYSTEM_PLATFORM_OSAPI_X11_WINDOW_SYSTEM_H__

#include "X11EventCore.h"
#include <Ic3/System/WindowSystem.h>

namespace Ic3::System
{

	ic3SysDeclareHandle( X11DisplayManager );
	ic3SysDeclareHandle( X11Window );
	ic3SysDeclareHandle( X11WindowManager );

	namespace platform
	{

		struct X11WindowNativeData : public X11EventSourceNativeData
		{
			Colormap xColormap = E_X11_XID_NONE;
		};

		struct X11WindowCreateInfo : public WindowCreateInfo
		{
			Visual * windowVisual = nullptr;
			int colorDepth = 0;
			bool fullscreenMode = false;
		};

		void x11CreateWindow( X11WindowNativeData & pWindowNativeData, const X11WindowCreateInfo & pCreateInfo );

		void x11WindowPostCreateUpdate( X11WindowNativeData & pWindowNativeData, const X11WindowCreateInfo & pCreateInfo );
		
		void x11DestroyWindow( X11WindowNativeData & pWindowNativeData );

		void x11SetWindowFullscreenState( X11WindowNativeData & pWindowNativeData, bool pSetFullscreen );

		void x11SetFrameTitle( const X11WindowNativeData & pWindowNativeData, const std::string & pTitle );

		void x11UpdateFrameGeometry( const X11WindowNativeData & pWindowNativeData,
		                             const FrameGeometry & pFrameGeometry,
		                             Bitmask<EFrameGeometryUpdateFlags> pUpdateFlags );

		IC3_SYSTEM_API_NODISCARD FrameSize x11GetFrameSize( const X11WindowNativeData & pWindowNativeData, EFrameSizeMode pSizeMode );

		IC3_SYSTEM_API_NODISCARD bool x11IsFullscreenWindow( XDisplay pDisplay, XWindow pWindow );

		IC3_SYSTEM_API_NODISCARD bool x11IsFullscreenWindow( const X11WindowNativeData & pWindowNativeData );

		IC3_SYSTEM_API_NODISCARD std::vector<Atom> x11QueryWindowPropertyValueArray( XDisplay pDisplay,
		                                                                             XWindow pWindow,
		                                                                             const char * pPropertyName );

		IC3_SYSTEM_API_NODISCARD bool x11CheckWindowPropertyValueSet( XDisplay pDisplay,
		                                                              XWindow pWindow,
		                                                              const char * pPropertyName,
		                                                              const char * pValueID );

	}

	class X11WindowManager : public X11NativeObject<WindowManager, platform::X11NativeDataCommon>
	{
	public:
		explicit X11WindowManager( X11DisplayManagerHandle pDisplayManager );
		virtual ~X11WindowManager() noexcept;

	private:
		// @override WindowManager::_nativeCreateWindow
		virtual WindowHandle _nativeCreateWindow( WindowCreateInfo pCreateInfo ) override final;

		// @override WindowManager::_nativeDestroyWindow
		virtual void _nativeDestroyWindow( Window & pWindow ) override final;
	};

	class X11Window : public X11NativeObject<Window, platform::X11WindowNativeData>
	{
		friend class X11WindowManager;

	public:
		explicit X11Window( X11WindowManagerHandle pWindowManager );
		virtual ~X11Window() noexcept;

	private:
		// @override Window::_nativeResize
		virtual void _nativeResize( const FrameSize & pFrameSize, EFrameSizeMode pSizeMode ) override final;

		// @override Window::_nativeSetFullscreenMode
		virtual void _nativeSetFullscreenMode( bool pEnable ) override final;

		// @override Window::_nativeSetTitle
		virtual void _nativeSetTitle( const std::string & pTitle ) override final;

		// @override Window::_nativeUpdateGeometry
		virtual void _nativeUpdateGeometry( const FrameGeometry & pFrameGeometry,
											Bitmask<EFrameGeometryUpdateFlags> pUpdateFlags ) override final;

		// @override Window::_nativeGetSize
		virtual FrameSize _nativeGetSize( EFrameSizeMode pSizeMode ) const override final;
	};

} // namespace Ic3::System

#endif // __IC3_SYSTEM_PLATFORM_OSAPI_X11_WINDOW_SYSTEM_H__
