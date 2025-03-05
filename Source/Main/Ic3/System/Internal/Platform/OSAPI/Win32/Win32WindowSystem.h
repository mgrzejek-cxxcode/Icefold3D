
#ifndef __IC3_SYSTEM_PLATFORM_OSAPI_WIN32_WINDOW_SYSTEM_H__
#define __IC3_SYSTEM_PLATFORM_OSAPI_WIN32_WINDOW_SYSTEM_H__

#include <Ic3/System/Core/WindowSystem.h>
#include "Win32EventCore.h"

namespace Ic3::System
{

	Ic3SysDeclareHandle( Win32DisplayManager );
	Ic3SysDeclareHandle( Win32Window );
	Ic3SysDeclareHandle( Win32WindowManager );

	namespace Platform
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

		void Win32CreateWindow( Win32WindowNativeData & pWindowNativeData, const WindowCreateInfo & pCreateInfo );

		void Win32DestroyWindow( Win32WindowNativeData & pWindowNativeData );

        void Win32ChangeWindowFullscreenState( Win32WindowNativeData & pWindowNativeData, bool pSetFullscreen );

		void Win32UpdateWindowFullscreenState( Win32WindowNativeData & pWindowNativeData, bool pSetFullscreen );

		void Win32SetFrameTitle( HWND pHWND, const std::string & pTitle );

		void Win32UpdateFrameGeometry(
				HWND pHWND,
				const FrameGeometry & pFrameGeometry,
				cppx::bitmask<EFrameGeometryUpdateFlags> pUpdateFlags );

		IC3_SYSTEM_API_NODISCARD FrameSize Win32GetFrameSize( HWND pHWND, EFrameSizeMode pSizeMode );

		IC3_SYSTEM_API_NODISCARD bool Win32IsFullscreenWindow( const Win32WindowNativeData & pWindowNativeData );

	}

	class IC3_SYSTEM_CLASS Win32WindowManager : public WindowManager
	{
	public:
		explicit Win32WindowManager( Win32DisplayManagerHandle pDisplayManager );
		virtual ~Win32WindowManager() noexcept;

	private:
		// @override WindowManager::_NativeCreateWindow
		virtual WindowHandle _NativeCreateWindow( WindowCreateInfo pCreateInfo ) override final;

        // @override WindowManager::_NativeDestroyWindow
        virtual void _NativeDestroyWindow( Window & pWindow ) override final;
	};

	class IC3_SYSTEM_CLASS Win32Window : public Win32NativeObject<Window, Platform::Win32WindowNativeData>
	{
		friend class Win32WindowManager;

	public:
		explicit Win32Window( Win32WindowManagerHandle pWindowManager );
		virtual ~Win32Window() noexcept;

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

#endif // __IC3_SYSTEM_PLATFORM_OSAPI_WIN32_WINDOW_SYSTEM_H__
