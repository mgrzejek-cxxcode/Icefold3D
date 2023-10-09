
#ifndef __IC3_SYSTEM_WINDOW_SYSTEM_H__
#define __IC3_SYSTEM_WINDOW_SYSTEM_H__

#include "Frame.h"
#include "WindowCommon.h"

namespace Ic3::System
{

	ic3DeclareClassHandle( DisplayManager );
	ic3DeclareClassHandle( Window );
	ic3DeclareClassHandle( WindowManager );

	/// @brief
	class WindowManager : public SysObject
	{
		friend class Window;

	public:
		DisplayManagerHandle const mDisplayManager;

	public:
		explicit WindowManager( DisplayManagerHandle pDisplayManager );
		virtual ~WindowManager() noexcept;

		IC3_ATTR_NO_DISCARD WindowHandle createWindow( WindowCreateInfo pCreateInfo );

		IC3_ATTR_NO_DISCARD bool checkFrameGeometry( const FrameGeometry & pFrameGeometry ) const;

		IC3_ATTR_NO_DISCARD FrameGeometry validateFrameGeometry( const FrameGeometry & pFrameGeometry ) const;

		IC3_ATTR_NO_DISCARD uint32 getActiveWindowsNum() const;

	friendapi:
		// Used by the Window class.
		// Destroys system-level window internals without invalidating 'pWindow' itself.
		virtual void releaseSystemWindow( Window & pWindow ) noexcept;

	private:
		virtual WindowHandle _nativeCreateWindow( WindowCreateInfo pCreateInfo ) = 0;

		virtual void _nativeDestroyWindow( Window & pWindow ) = 0;
	};

	/// @brief
	class Window : public Frame
	{
		friend class WindowManager;

	public:
		WindowManagerHandle const mWindowManager;

	public:
		explicit Window( WindowManagerHandle pWindowManager, void * pNativeData );
		virtual ~Window() noexcept;

		virtual void resizeClientArea( const FrameSize & pSize ) override final;

		virtual void resizeFrame( const FrameSize & pSize ) override final;

		virtual void setFullscreenMode( bool pEnable ) override final;

		virtual void setTitle( const std::string & pTitleText ) override final;

		virtual void updateGeometry( const FrameGeometry & pFrameGeometry,
		                             Bitmask<EFrameGeometryUpdateFlags> pUpdateFlags ) override final;

		IC3_ATTR_NO_DISCARD virtual FrameSize getClientAreaSize() const override final;

		IC3_ATTR_NO_DISCARD virtual FrameSize getFrameSize() const override final;

		IC3_ATTR_NO_DISCARD virtual bool isFullscreen() const override final;

	private:
		virtual void onDestroySystemObjectRequested() override final;

		virtual void _nativeResize( const FrameSize & pFrameSize, EFrameSizeMode pSizeMode ) = 0;

		virtual void _nativeSetFullscreenMode( bool pEnable ) = 0;

		virtual void _nativeSetTitle( const std::string & pTitle ) = 0;

		virtual void _nativeUpdateGeometry( const FrameGeometry & pFrameGeometry,
		                                    Bitmask<EFrameGeometryUpdateFlags> pUpdateFlags ) = 0;

		virtual FrameSize _nativeGetSize( EFrameSizeMode pSizeMode ) const = 0;
	};

} // namespace Ic3::System

#endif // __IC3_SYSTEM_WINDOW_SYSTEM_H__
