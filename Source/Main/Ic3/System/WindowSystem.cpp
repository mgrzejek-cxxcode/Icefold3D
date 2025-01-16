
#include "DisplaySystem.h"
#include "WindowSystem.h"

namespace Ic3::System
{

	WindowManager::WindowManager( DisplayManagerHandle pDisplayManager )
	: SysObject( pDisplayManager->mSysContext )
	, mDisplayManager( std::move( pDisplayManager ) )
	{}

	WindowManager::~WindowManager() noexcept = default;

	WindowHandle WindowManager::CreateWindow( WindowCreateInfo pCreateInfo )
	{
		auto createInfo = std::move( pCreateInfo );

		createInfo.frameGeometry = ValidateFrameGeometry( createInfo.frameGeometry );

		if( createInfo.title.empty() )
		{
			createInfo.title = "Tessline-3DX / Native Window";
		}

		return _NativeCreateWindow( std::move( createInfo ) );
	}

	bool WindowManager::CheckFrameGeometry( const FrameGeometry & pFrameGeometry ) const
	{
		return mDisplayManager->CheckFrameGeometry( pFrameGeometry );
	}

	FrameGeometry WindowManager::ValidateFrameGeometry( const FrameGeometry & pFrameGeometry ) const
	{
		return mDisplayManager->ValidateFrameGeometry( pFrameGeometry );
	}

	void WindowManager::ReleaseSystemWindow( Window & pWindow ) noexcept
	{
		try
		{
			_NativeDestroyWindow( pWindow );
		}
		catch( const Exception & pException )
		{
			( pException );
			ic3DebugInterrupt();
		}
		catch( ... )
		{
			ic3DebugInterrupt();
		}
	}


	Window::Window( WindowManagerHandle pWindowManager, void * pNativeData )
	: Frame( pWindowManager->mSysContext )
	, mWindowManager( std::move( pWindowManager ) )
	{
		SetEventSourceNativeData( pNativeData );
	}

	Window::~Window() noexcept
	{
		ResetEventSourceNativeData();
	}

	void Window::ResizeClientArea( const FrameSize & pSize )
	{
		FrameGeometry newFrameGeometry{};
		newFrameGeometry.mPosition = cxFramePosAuto;
		newFrameGeometry.size = pSize;
		newFrameGeometry.mStyle = EFrameStyle::Unspecified;

		newFrameGeometry = mWindowManager->ValidateFrameGeometry( newFrameGeometry );

		const auto updateFlags = eFrameGeometryUpdateFlagPositionBit | eFrameGeometryUpdateFlagSizeClientAreaBit;
		_NativeUpdateGeometry( newFrameGeometry, updateFlags );
	}

	void Window::ResizeFrame( const FrameSize & pSize )
	{
		FrameGeometry newFrameGeometry{};
		newFrameGeometry.mPosition = cxFramePosAuto;
		newFrameGeometry.size = pSize;
		newFrameGeometry.mStyle = EFrameStyle::Unspecified;

		newFrameGeometry = mWindowManager->ValidateFrameGeometry( newFrameGeometry );

		const auto updateFlags = eFrameGeometryUpdateFlagPositionBit | eFrameGeometryUpdateFlagSizeOuterRectBit;
		_NativeUpdateGeometry( newFrameGeometry, updateFlags );
	}

	void Window::SetFullscreenMode( bool pEnable )
	{
		_NativeSetFullscreenMode( pEnable );
	}

	void Window::SetTitle( const std::string & pTitleText )
	{
		_NativeSetTitle( pTitleText );
	}

	void Window::UpdateGeometry(
			const FrameGeometry & pFrameGeometry,
			cppx::bitmask<EFrameGeometryUpdateFlags> pUpdateFlags )
	{
		auto newFrameGeometry = mWindowManager->ValidateFrameGeometry( pFrameGeometry );
		_NativeUpdateGeometry( newFrameGeometry, pUpdateFlags );
	}

	FrameSize Window::GetClientAreaSize() const
	{
		return _NativeGetSize( EFrameSizeMode::ClientArea );
	}

	FrameSize Window::GetFrameSize() const
	{
		return _NativeGetSize( EFrameSizeMode::OuterRect );
	}

	bool Window::IsFullscreen() const
	{
		return false;
	}

	void Window::OnDestroySystemObjectRequested()
	{
		EventSource::OnDestroySystemObjectRequested();

		// Window itself must be destroyed at the very end - it can still be an active event source!
		// Destroying it first would make it impossible to handle the event source removal (e.g. in
		// case of Win32, HWND would be already invalid inside EventSource::OnDestroySystemObjectRequested()).

		mWindowManager->ReleaseSystemWindow( *this );
	}

} // namespace Ic3::System
