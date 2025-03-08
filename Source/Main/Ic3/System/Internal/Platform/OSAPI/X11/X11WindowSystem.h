
#ifndef __IC3_SYSTEM_PLATFORM_OSAPI_X11_WINDOW_SYSTEM_H__
#define __IC3_SYSTEM_PLATFORM_OSAPI_X11_WINDOW_SYSTEM_H__

#include "X11EventCore.h"
#include <Ic3/System/Core/WindowSystem.h>

namespace Ic3::System
{

	Ic3SysDeclareHandle( X11DisplayManager );
	Ic3SysDeclareHandle( X11Window );
	Ic3SysDeclareHandle( X11WindowManager );

	namespace Platform
	{

		struct X11WindowNativeData : public X11EventSourceNativeData
		{
			Colormap xColormap = eXIDNone;
		};

		struct X11WindowCreateInfo : public WindowCreateInfo
		{
			Visual * windowVisual = nullptr;
			int colorDepth = 0;
			bool fullscreenMode = false;
		};

		void X11CreateWindow( X11WindowNativeData & pWindowNativeData, const X11WindowCreateInfo & pCreateInfo );

		void X11WindowPostCreateUpdate( X11WindowNativeData & pWindowNativeData, const X11WindowCreateInfo & pCreateInfo );
		
		void X11DestroyWindow( X11WindowNativeData & pWindowNativeData );

		void X11SetWindowFullscreenState( X11WindowNativeData & pWindowNativeData, bool pSetFullscreen );

		void X11SetFrameTitle( const X11WindowNativeData & pWindowNativeData, const std::string & pTitle );

		void X11UpdateFrameGeometry(
				const X11WindowNativeData & pWindowNativeData,
				const FrameGeometry & pFrameGeometry,
				cppx::bitmask<EFrameGeometryUpdateFlags> pUpdateFlags );

		CPPX_ATTR_NO_DISCARD FrameSize X11GetFrameSize( const X11WindowNativeData & pWindowNativeData, EFrameSizeMode pSizeMode );

		CPPX_ATTR_NO_DISCARD bool X11IsFullscreenWindow( XDisplay pDisplay, XWindow pWindow );

		CPPX_ATTR_NO_DISCARD bool X11IsFullscreenWindow( const X11WindowNativeData & pWindowNativeData );

		CPPX_ATTR_NO_DISCARD std::vector<Atom> X11QueryWindowPropertyValueArray(
				XDisplay pDisplay,
				XWindow pWindow,
				const char * pPropertyName );

		CPPX_ATTR_NO_DISCARD bool X11CheckWindowPropertyValueSet(
				XDisplay pDisplay,
				XWindow pWindow,
				const char * pPropertyName,
				const char * pValueID );

	}

	class IC3_SYSTEM_CLASS X11WindowManager : public X11NativeObject<WindowManager, Platform::X11NativeDataCommon>
	{
	public:
		explicit X11WindowManager( X11DisplayManagerHandle pDisplayManager );
		virtual ~X11WindowManager() noexcept;

	private:
		// @override WindowManager::_NativeCreateWindow
		virtual WindowHandle _NativeCreateWindow( WindowCreateInfo pCreateInfo ) override final;

		// @override WindowManager::_NativeDestroyWindow
		virtual void _NativeDestroyWindow( Window & pWindow ) override final;
	};

	class IC3_SYSTEM_CLASS X11Window : public X11NativeObject<Window, Platform::X11WindowNativeData>
	{
		friend class X11WindowManager;

	public:
		explicit X11Window( X11WindowManagerHandle pWindowManager );
		virtual ~X11Window() noexcept;

	private:
		// @override Window::_NativeResize
		virtual void _NativeResize( const FrameSize & pFrameSize, EFrameSizeMode pSizeMode ) override final;

		// @override Window::_NativeSetFullscreenMode
		virtual void _NativeSetFullscreenMode( bool pEnable ) override final;

		// @override Window::_NativeSetTitle
		virtual void _NativeSetTitle( const std::string & pTitle ) override final;

		// @override Window::_NativeUpdateGeometry
		virtual void _NativeUpdateGeometry(
				const FrameGeometry & pFrameGeometry,
				cppx::bitmask<EFrameGeometryUpdateFlags> pUpdateFlags ) override final;

		// @override Window::_NativeGetSize
		virtual FrameSize _NativeGetSize( EFrameSizeMode pSizeMode ) const override final;
	};

} // namespace Ic3::System

#endif // __IC3_SYSTEM_PLATFORM_OSAPI_X11_WINDOW_SYSTEM_H__
