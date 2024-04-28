
#ifndef __IC3_SYSTEM_PLATFORM_OSAPI_OSX_WINDOW_SYSTEM_H__
#define __IC3_SYSTEM_PLATFORM_OSAPI_OSX_WINDOW_SYSTEM_H__

#include "OSXEventCore.h"
#include <Ic3/System/WindowSystem.h>
#import <AppKit/NSScreen.h>
#import <AppKit/NSWindow.h>

namespace Ic3::System
{

	ic3SysDeclareHandle( OSXDisplayManager );
	ic3SysDeclareHandle( OSXWindow );
	ic3SysDeclareHandle( OSXWindowManager );

	namespace Platform
	{

		struct OSXWindowNativeData : public OSXEventSourceNativeData
		{
			OSXWindow * parentWindow = nullptr;

			NSScreen * nsTargetScreen = nullptr;
		};

		struct OSXFrameGeometry
		{
			NSRect frameRect;
			NSUInteger style;
		};

		void osxCreateWindow( OSXWindowNativeData & pWindowNativeData, NSScreen * pTargetScreen, const WindowCreateInfo & pCreateInfo );

		void osxCreateWindowDefaultView( OSXWindowNativeData & pWindowNativeData );

		void osxSetInputWindow( OSXWindowNativeData & pWindowNativeData );

		void osxSetFrameTitle( NSWindow * pNSWindow, const std::string & pTitle );

		void osxUpdateFrameGeometry( NSWindow * pNSWindow, const FrameGeometry & pFrameGeometry, Bitmask<EFrameGeometryUpdateFlags> pUpdateFlags );

		IC3_SYSTEM_API_NODISCARD FrameSize osxGetFrameSize( NSWindow * pNSWindow, EFrameSizeMode pSizeMode );

		IC3_SYSTEM_API_NODISCARD NSUInteger osxTranslateFrameStyle( EFrameStyle pStyle );

	}

	class OSXWindowManager : public OSXNativeObject<WindowManager, Platform::OSXNativeDataCommon>
	{
	public:
		explicit OSXWindowManager( OSXDisplayManagerHandle pDisplayManager );
		virtual ~OSXWindowManager() noexcept;

	private:
		// @override WindowManager::_nativeCreateWindow
		virtual WindowHandle _nativeCreateWindow( WindowCreateInfo pCreateInfo ) override final;

		// @override WindowManager::_nativeDestroyWindow
		virtual void _nativeDestroyWindow( Window & pWindow ) override final;
	};

	class OSXWindow : public OSXNativeObject<Window, Platform::OSXWindowNativeData>
	{
		friend class OSXWindowManager;

	public:
		explicit OSXWindow( OSXWindowManagerHandle pWindowManager );
		virtual ~OSXWindow() noexcept;

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

#endif // __IC3_SYSTEM_PLATFORM_OSAPI_OSX_WINDOW_SYSTEM_H__
