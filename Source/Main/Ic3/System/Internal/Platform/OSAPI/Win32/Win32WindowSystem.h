
#ifndef __IC3_SYSTEM_PLATFORM_OSAPI_WIN32_WINDOW_SYSTEM_H__
#define __IC3_SYSTEM_PLATFORM_OSAPI_WIN32_WINDOW_SYSTEM_H__

#include <Ic3/System/WindowSystem.h>
#include "Win32EventCore.h"

namespace Ic3::System
{

	ic3SysDeclareHandle( Win32DisplayManager );
	ic3SysDeclareHandle( Win32Window );
	ic3SysDeclareHandle( Win32WindowManager );

	namespace platform
	{

		struct Win32WindowNativeData : public Win32EventSourceNativeData
		{
			ATOM wndClsID = 0;
			LPCSTR wndClsName = nullptr;
			HMODULE moduleHandle = nullptr;
			FrameGeometry fsCachedGeometry = {};
		};

		struct Win32FrameGeometry
		{
			RECT frameRect;
			DWORD style;
		};

		void win32CreateWindow( Win32WindowNativeData & pWindowNativeData, const WindowCreateInfo & pCreateInfo );

		void win32DestroyWindow( Win32WindowNativeData & pWindowNativeData );

        void win32ChangeWindowFullscreenState( Win32WindowNativeData & pWindowNativeData, bool pSetFullscreen );

		void win32UpdateWindowFullscreenState( Win32WindowNativeData & pWindowNativeData, bool pSetFullscreen );

		void win32SetFrameTitle( HWND pHWND, const std::string & pTitle );

		void win32UpdateFrameGeometry( HWND pHWND, const FrameGeometry & pFrameGeometry, Bitmask<EFrameGeometryUpdateFlags> pUpdateFlags );

		IC3_SYSTEM_API_NODISCARD FrameSize win32GetFrameSize( HWND pHWND, EFrameSizeMode pSizeMode );

		IC3_SYSTEM_API_NODISCARD bool win32IsFullscreenWindow( const Win32WindowNativeData & pWindowNativeData );

	}

	class Win32WindowManager : public WindowManager
	{
	public:
		explicit Win32WindowManager( Win32DisplayManagerHandle pDisplayManager );
		virtual ~Win32WindowManager() noexcept;

	private:
		// @override WindowManager::_nativeCreateWindow
		virtual WindowHandle _nativeCreateWindow( WindowCreateInfo pCreateInfo ) override final;

        // @override WindowManager::_nativeDestroyWindow
        virtual void _nativeDestroyWindow( Window & pWindow ) override final;
	};

	class Win32Window : public Win32NativeObject<Window, platform::Win32WindowNativeData>
	{
		friend class Win32WindowManager;

	public:
		explicit Win32Window( Win32WindowManagerHandle pWindowManager );
		virtual ~Win32Window() noexcept;

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

#endif // __IC3_SYSTEM_PLATFORM_OSAPI_WIN32_WINDOW_SYSTEM_H__
