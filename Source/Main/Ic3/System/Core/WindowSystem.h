
#ifndef __IC3_SYSTEM_WINDOW_SYSTEM_H__
#define __IC3_SYSTEM_WINDOW_SYSTEM_H__

#include "Frame.h"

namespace Ic3::System
{

	Ic3DeclareClassHandle( DisplayManager );
	Ic3DeclareClassHandle( Window );
	Ic3DeclareClassHandle( WindowManager );

	/// @brief
	class IC3_SYSTEM_CLASS WindowManager : public SysObject
	{
		friend class Window;

	public:
		DisplayManagerHandle const mDisplayManager;

	public:
		explicit WindowManager( DisplayManagerHandle pDisplayManager );
		virtual ~WindowManager() noexcept;

		CPPX_ATTR_NO_DISCARD WindowHandle CreateWindow( WindowCreateInfo pCreateInfo );

		CPPX_ATTR_NO_DISCARD bool CheckFrameGeometry( const FrameGeometry & pFrameGeometry ) const;

		CPPX_ATTR_NO_DISCARD FrameGeometry ValidateFrameGeometry( const FrameGeometry & pFrameGeometry ) const;

		CPPX_ATTR_NO_DISCARD uint32 GetActiveWindowsNum() const;

	friendapi:
		// Used by the Window class.
		// Destroys system-level window internals without invalidating 'pWindow' itself.
		virtual void ReleaseSystemWindow( Window & pWindow ) noexcept;

	private:
		virtual WindowHandle _NativeCreateWindow( WindowCreateInfo pCreateInfo ) = 0;

		virtual void _NativeDestroyWindow( Window & pWindow ) = 0;
	};

	/// @brief
	class IC3_SYSTEM_CLASS Window : public Frame
	{
		friend class WindowManager;

	public:
		WindowManagerHandle const mWindowManager;

	public:
		explicit Window( WindowManagerHandle pWindowManager, void * pNativeData );
		virtual ~Window() noexcept;

		virtual void ResizeClientArea( const FrameSize & pSize ) override final;

		virtual void ResizeFrame( const FrameSize & pSize ) override final;

		virtual void SetFullscreenMode( bool pEnable ) override final;

		virtual void SetTitle( const std::string & pTitleText ) override final;

		virtual void UpdateGeometry(
				const FrameGeometry & pFrameGeometry,
				cppx::bitmask<EFrameGeometryUpdateFlags> pUpdateFlags ) override final;

		CPPX_ATTR_NO_DISCARD virtual FrameSize GetClientAreaSize() const override final;

		CPPX_ATTR_NO_DISCARD virtual FrameSize GetFrameSize() const override final;

		CPPX_ATTR_NO_DISCARD virtual bool IsFullscreen() const override final;

	private:
		virtual void OnDestroySystemObjectRequested() override final;

		virtual void _NativeResize( const FrameSize & pFrameSize, EFrameSizeMode pSizeMode ) = 0;

		virtual void _NativeSetFullscreenMode( bool pEnable ) = 0;

		virtual void _NativeSetTitle( const std::string & pTitle ) = 0;

		virtual void _NativeUpdateGeometry(
				const FrameGeometry & pFrameGeometry,
				cppx::bitmask<EFrameGeometryUpdateFlags> pUpdateFlags ) = 0;

		virtual FrameSize _NativeGetSize( EFrameSizeMode pSizeMode ) const = 0;
	};

} // namespace Ic3::System

#endif // __IC3_SYSTEM_WINDOW_SYSTEM_H__
