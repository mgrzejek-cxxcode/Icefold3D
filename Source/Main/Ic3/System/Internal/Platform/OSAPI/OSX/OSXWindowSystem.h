
#ifndef __IC3_SYSTEM_PLATFORM_OSAPI_OSX_WINDOW_SYSTEM_H__
#define __IC3_SYSTEM_PLATFORM_OSAPI_OSX_WINDOW_SYSTEM_H__

#include "OSXEventCore.h"
#include <Ic3/System/WindowSystem.h>
#import <AppKit/NSScreen.h>
#import <AppKit/NSWindow.h>

namespace Ic3::System
{

	Ic3SysDeclareHandle( OSXDisplayManager );
	Ic3SysDeclareHandle( OSXWindow );
	Ic3SysDeclareHandle( OSXWindowManager );

	namespace Platform
	{

		struct OSXWindowNativeData : public OSXEventSourceNativeData
		{
			OSXWindow * mParentWindow = nullptr;

			NSScreen * mNSTargetScreen = nullptr;
		};

		struct OSXFrameGeometry
		{
			NSRect mFrameRect;
			NSUInteger mStyle;
		};

		void OSXCreateWindow(
				OSXWindowNativeData & pWindowNativeData,
				NSScreen * pTargetScreen,
				const WindowCreateInfo & pCreateInfo );

		void OSXCreateWindowDefaultView( OSXWindowNativeData & pWindowNativeData );

		void OSXSetInputWindow( OSXWindowNativeData & pWindowNativeData );

		void OSXSetFrameTitle( NSWindow * pNSWindow, const std::string & pTitle );

		void OSXUpdateFrameGeometry(
				NSWindow * pNSWindow,
				const FrameGeometry & pFrameGeometry,
				cppx::bitmask<EFrameGeometryUpdateFlags> pUpdateFlags );

		IC3_SYSTEM_API_NODISCARD FrameSize OSXGetFrameSize( NSWindow * pNSWindow, EFrameSizeMode pSizeMode );

		IC3_SYSTEM_API_NODISCARD NSUInteger OSXTranslateFrameStyle( EFrameStyle pStyle );

	}

	class OSXWindowManager : public OSXNativeObject<WindowManager, Platform::OSXNativeDataCommon>
	{
	public:
		explicit OSXWindowManager( OSXDisplayManagerHandle pDisplayManager );
		virtual ~OSXWindowManager() noexcept;

	private:
		// @override WindowManager::_NativeCreateWindow
		virtual WindowHandle _NativeCreateWindow( WindowCreateInfo pCreateInfo ) override final;

		// @override WindowManager::_NativeDestroyWindow
		virtual void _NativeDestroyWindow( Window & pWindow ) override final;
	};

	class OSXWindow : public OSXNativeObject<Window, Platform::OSXWindowNativeData>
	{
		friend class OSXWindowManager;

	public:
		explicit OSXWindow( OSXWindowManagerHandle pWindowManager );
		virtual ~OSXWindow() noexcept;

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

#endif // __IC3_SYSTEM_PLATFORM_OSAPI_OSX_WINDOW_SYSTEM_H__
