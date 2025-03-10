
#ifndef __IC3_SYSTEM_PLATFORM_OSAPI_ANDROID_WINDOW_H__
#define __IC3_SYSTEM_PLATFORM_OSAPI_ANDROID_WINDOW_H__

#include "AndroidEventCore.h"
#include <Ic3/System/Core/WindowSystem.h>

namespace Ic3::System
{

	Ic3SysDeclareHandle( AndroidWindow );
	Ic3SysDeclareHandle( AndroidWindowManager );

	namespace Platform
	{

		using AndroidWindowManagerNativeData = AndroidNativeDataCommon;
		using AndroidWindowNativeData = AndroidNativeDataCommon;

		IC3_SYSTEM_API_NODISCARD cxm::size2u AndroidQueryNativeWindowSize( ANativeWindow * pANativeWindow );

	}

	class AndroidWindowManager : public WindowManager, public NativeObject<Platform::AndroidWindowManagerNativeData>
	{
	public:
		explicit AndroidWindowManager( DisplayManagerHandle pDisplayManager );
		virtual ~AndroidWindowManager() noexcept;

	private:
		// @override WindowManager::_NativeCreateWindow
		virtual WindowHandle _NativeCreateWindow( WindowCreateInfo pCreateInfo ) override final;
	};

	class AndroidWindow : public Window, public NativeObject<Platform::AndroidWindowNativeData>
	{
		friend class AndroidWindowManager;

	public:
		explicit AndroidWindow( AndroidWindowManagerHandle pWindowManager );
		virtual ~AndroidWindow() noexcept;

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

#endif // __IC3_SYSTEM_PLATFORM_OSAPI_ANDROID_WINDOW_H__
