
#ifndef __IC3_SYSTEM_PLATFORM_OSAPI_ANDROID_WINDOW_H__
#define __IC3_SYSTEM_PLATFORM_OSAPI_ANDROID_WINDOW_H__

#include "AndroidEventCore.h"
#include <Ic3/System/WindowSystem.h>

namespace Ic3::System
{

	ic3SysDeclareHandle( AndroidWindow );
	ic3SysDeclareHandle( AndroidWindowManager );

	namespace platform
	{

		using AndroidWindowManagerNativeData = AndroidNativeDataCommon;
		using AndroidWindowNativeData = AndroidNativeDataCommon;

		IC3_SYSTEM_API_NODISCARD Math::Size2u androidQueryNativeWindowSize( ANativeWindow * pANativeWindow );

	}

	class AndroidWindowManager : public WindowManager, public NativeObject<platform::AndroidWindowManagerNativeData>
	{
	public:
		explicit AndroidWindowManager( DisplayManagerHandle pDisplayManager );
		virtual ~AndroidWindowManager() noexcept;

	private:
		// @override WindowManager::_nativeCreateWindow
		virtual WindowHandle _nativeCreateWindow( WindowCreateInfo pCreateInfo ) override final;
	};

	class AndroidWindow : public Window, public NativeObject<platform::AndroidWindowNativeData>
	{
		friend class AndroidWindowManager;

	public:
		explicit AndroidWindow( AndroidWindowManagerHandle pWindowManager );
		virtual ~AndroidWindow() noexcept;

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

#endif // __IC3_SYSTEM_PLATFORM_OSAPI_ANDROID_WINDOW_H__
