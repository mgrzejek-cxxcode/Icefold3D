
#include "Win32WindowSystem.h"
#include "Win32DisplaySystem.h"

#if( PCL_TARGET_SYSAPI == PCL_TARGET_SYSAPI_WIN32 )
namespace Ic3::System
{

	namespace Platform
	{

		// Registers custom window class in the system.
		void _win32RegisterWndClass( Win32WindowNativeData & pWindowNativeData );

		// Translates a common-level frame style into a Win32-specific style mask.
		DWORD _Win32TranslateFrameStyle( EFrameStyle pStyle );

		//
		EFrameStyle _win32QueryWindowStyle( HWND pHWND );

		//
		FrameGeometry _win32QueryCurrentWindowGeometry( HWND pHWND );

		// Translates the specified geometry definition for a new frame (create) into a Win32-specific format.
		Win32FrameGeometry _win32CheckFrameGeometry( const FrameGeometry & pFrameGeometry );

		// Checks frame geometry/style definition which is to be applied to an existing frame (update).
		// Returns a Win32-specific geometry format, validated and adjusted to the selected style.
		Win32FrameGeometry _win32CheckFrameGeometryUpdate( HWND pWindowHwnd,
		                                                   const FrameGeometry & pFrameGeometry,
		                                                   cppx::bitmask<EFrameGeometryUpdateFlags> pUpdateFlags );

	}


	Win32WindowManager::Win32WindowManager( Win32DisplayManagerHandle pDisplayManager )
	: WindowManager( std::move( pDisplayManager ) )
	{}

	Win32WindowManager::~Win32WindowManager() noexcept = default;

	WindowHandle Win32WindowManager::_NativeCreateWindow( WindowCreateInfo pCreateInfo )
	{
		auto windowObject = CreateSysObject<Win32Window>( GetHandle<Win32WindowManager>() );

		Platform::Win32CreateWindow( windowObject->mNativeData, pCreateInfo );

		::ShowWindow( windowObject->mNativeData.mHWND, SW_SHOWNORMAL );

		return windowObject;
	}

    void Win32WindowManager::_NativeDestroyWindow( Window & pWindow )
    {
        auto * win32Window = pWindow.QueryInterface<Win32Window>();

        Platform::Win32DestroyWindow( win32Window->mNativeData );
    }


	Win32Window::Win32Window( Win32WindowManagerHandle pWindowManager )
	: Win32NativeObject( std::move( pWindowManager ), &mNativeData )
	{}

	Win32Window::~Win32Window() noexcept = default;

	void Win32Window::_NativeResize( const FrameSize & pFrameSize, EFrameSizeMode pSizeMode )
	{}

	void Win32Window::_NativeSetFullscreenMode( bool pEnable )
	{
		Platform::Win32ChangeWindowFullscreenState( mNativeData, pEnable );
	}

	void Win32Window::_NativeSetTitle( const std::string & pTitle )
	{
		Platform::Win32SetFrameTitle( mNativeData.mHWND, pTitle );
	}

	void Win32Window::_NativeUpdateGeometry(
			const FrameGeometry & pFrameGeometry,
			cppx::bitmask<EFrameGeometryUpdateFlags> pUpdateFlags )
	{
		Platform::Win32UpdateFrameGeometry( mNativeData.mHWND, pFrameGeometry, pUpdateFlags );
	}

	FrameSize Win32Window::_NativeGetSize( EFrameSizeMode pSizeMode ) const
	{
		return Platform::Win32GetFrameSize( mNativeData.mHWND, pSizeMode );
	}


	namespace Platform
	{

		void Win32CreateWindow( Win32WindowNativeData & pWindowNativeData, const WindowCreateInfo & pCreateInfo )
		{
			// Register window class. Will fetch it if already registered.
			_win32RegisterWndClass( pWindowNativeData );

			auto geometryUpdateFlags = E_FRAME_GEOMETRY_UPDATE_FLAGS_ALL_DEFAULT;
			auto win32FrameGeometry = _win32CheckFrameGeometry( pCreateInfo.frameGeometry );

			HWND windowHwnd = ::CreateWindowExA( WS_EX_APPWINDOW | WS_EX_OVERLAPPEDWINDOW,
			                                     pWindowNativeData.wndClsName,
			                                     pCreateInfo.title.c_str(),
			                                     win32FrameGeometry.style,
			                                     win32FrameGeometry.frameRect.left,
			                                     win32FrameGeometry.frameRect.top,
			                                     win32FrameGeometry.frameRect.right - win32FrameGeometry.frameRect.left,
			                                     win32FrameGeometry.frameRect.bottom - win32FrameGeometry.frameRect.top,
			                                     nullptr,
			                                     nullptr,
			                                     pWindowNativeData.moduleHandle,
			                                     &pWindowNativeData );

			if( windowHwnd == nullptr )
			{
				Ic3Throw( eExcCodeDebugPlaceholder );
			}

			::SetWindowPos( windowHwnd,
			                nullptr,
			                win32FrameGeometry.frameRect.left,
			                win32FrameGeometry.frameRect.top,
			                0, // Width of the frame
			                0, // Height of the frame
			                SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED );

			// Retrieve the current number of windows created with our class (ClassRefCounter).
			auto wndClassRefCounter = ::GetClassLongPtrA( windowHwnd, 0 );

			// Increment the counter and store the new value.
			::SetClassLongPtrA( windowHwnd, 0, wndClassRefCounter + 1 );

			pWindowNativeData.mHWND = windowHwnd;
		}

		void Win32DestroyWindow( Win32WindowNativeData & pWindowNativeData )
		{
			if( !pWindowNativeData.mHWND )
			{
				return;
			}

			// Retrieve the current number of windows created with our class (ClassRefCounter).
			auto wndClassRefCounter = ::GetClassLongPtrA( pWindowNativeData.mHWND, 0 );

			// We are destroying one of the windows, decrement the counter.
			--wndClassRefCounter;

			::SetClassLongPtrA( pWindowNativeData.mHWND, 0, wndClassRefCounter );

			::DestroyWindow( pWindowNativeData.mHWND );

			if( wndClassRefCounter == 0 )
			{
				::UnregisterClassA( pWindowNativeData.wndClsName, pWindowNativeData.moduleHandle );
			}

			pWindowNativeData.mHWND = nullptr;
			pWindowNativeData.wndClsID = 0;
			pWindowNativeData.wndClsName = nullptr;
			pWindowNativeData.moduleHandle = nullptr;
		}

		void Win32ChangeWindowFullscreenState( Win32WindowNativeData & pWindowNativeData, bool pSetFullscreen )
		{
			const auto currentFullscreenState = pWindowNativeData.mSysWindowFlags.is_set( eWin32SystemWindowFlagWMStateFullscreen );

			if( currentFullscreenState != pSetFullscreen )
			{
				// We need to go through the event system and properly emit an EvtWindowUpdateFullscreen.
				// To avoid mixing event and window systems, we simply use the OS ability to post messages
				// to any window event queue.
				// Out custom message indicates a fullscreen state change request. Already pre-validated,
				// it just needs to be handled and dispatched.

				const auto messageID = Platform::CX_WIN32_MESSAGE_ID_FULLSCREEN_STATE_CHANGE;
				const auto wParam = static_cast<WPARAM>( pSetFullscreen ? 1 : 0 );

				// This is an asynchronous post. This functions returns immediately - the message will be
				// put into the queue and fetched when the event system pulls the event queue next time.
				::PostMessageA( pWindowNativeData.mHWND, messageID, wParam, 0 );
			}
		}

		void Win32UpdateWindowFullscreenState( Win32WindowNativeData & pWindowNativeData, bool pSetFullscreen )
		{
			FrameGeometry newFrameGeometry{};
			cppx::bitmask<EFrameGeometryUpdateFlags> updateFlags = E_FRAME_GEOMETRY_UPDATE_FLAG_POSITION_BIT | E_FRAME_GEOMETRY_UPDATE_FLAG_STYLE_BIT;

			if( pSetFullscreen )
			{
				// _win32QueryCurrentWindowGeometry() returns the geometry of the whole window, not only the client area!
				pWindowNativeData.mFSCachedGeometry = _win32QueryCurrentWindowGeometry( pWindowNativeData.mHWND );

				const auto screenDC = ::GetWindowDC( nullptr );

				const auto xBorderSize = ::GetSystemMetrics(SM_CXEDGE);
				const auto yBorderSize = ::GetSystemMetrics(SM_CYEDGE);

				newFrameGeometry.position.x = -xBorderSize;
				newFrameGeometry.position.y = -yBorderSize;
				newFrameGeometry.size.x = ::GetDeviceCaps( screenDC, HORZRES ) + 2 * xBorderSize;
				newFrameGeometry.size.y = ::GetDeviceCaps( screenDC, VERTRES ) + 2 * yBorderSize;
				newFrameGeometry.style = EFrameStyle::OVERLAY;
				updateFlags.set( E_FRAME_GEOMETRY_UPDATE_FLAG_SIZE_CLIENT_AREA_BIT );
			}
			else
			{
				// The geometry has the size of the whole window. That is because some window styles cannot be passed
				// to AdjustWindowRect(). To get around that, we set the outer rect directly, bypassing adjustment.

				newFrameGeometry = pWindowNativeData.mFSCachedGeometry;
				updateFlags.set( E_FRAME_GEOMETRY_UPDATE_FLAG_SIZE_OUTER_RECT_BIT );
			}

			Win32UpdateFrameGeometry( pWindowNativeData.mHWND, newFrameGeometry, updateFlags );

			pWindowNativeData.mSysWindowFlags.setOrUnset( eWin32SystemWindowFlagWMStateFullscreen, pSetFullscreen );
		}

		void Win32SetFrameTitle( HWND pHWND, const std::string & pTitle )
		{
			::SetWindowTextA( pHWND, pTitle.c_str() );

			::SetWindowPos( pHWND, nullptr, 0, 0, 0, 0, SWP_NOZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW );
		}

		void Win32UpdateFrameGeometry( HWND pHWND, const FrameGeometry & pFrameGeometry, cppx::bitmask<EFrameGeometryUpdateFlags> pUpdateFlags )
		{
			auto win32Geometry = Platform::_win32CheckFrameGeometryUpdate( pHWND, pFrameGeometry, pUpdateFlags );

			::SetWindowLongA( pHWND, GWL_STYLE, win32Geometry.style );

			::SetWindowPos( pHWND,
			                nullptr,
			                win32Geometry.frameRect.left, // X coordinate
			                win32Geometry.frameRect.top, // Y coordinate
			                win32Geometry.frameRect.right - win32Geometry.frameRect.left, // Width of the frame
			                win32Geometry.frameRect.bottom - win32Geometry.frameRect.top, // Height of the frame
			                SWP_NOZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW );
		}

		FrameSize Win32GetFrameSize( HWND pHWND, EFrameSizeMode pSizeMode )
		{
			RECT frameRect;
			if( pSizeMode == EFrameSizeMode::CLIENT_AREA )
			{
				::GetClientRect( pHWND, &frameRect );
				::AdjustWindowRect( &frameRect, WS_VISIBLE, FALSE );
			}
			else
			{
				::GetWindowRect( pHWND, &frameRect );
			}

			FrameSize resultFrameSize;
			resultFrameSize.x = frameRect.right - frameRect.left;
			resultFrameSize.y = frameRect.bottom - frameRect.top;

			return resultFrameSize;
		}

		bool Win32IsFullscreenWindow( const Win32WindowNativeData & pWindowNativeData )
		{
			return pWindowNativeData.mSysWindowFlags.is_set( Platform::eWin32SystemWindowFlagWMStateFullscreen );
		}

		void _win32RegisterWndClass( Win32WindowNativeData & pWindowNativeData )
		{
			//
			const LPCSTR wndClassName = "IC3_SYSTEM_WND_CLS";
			//
			const HMODULE wndProcModuleHandle = ::GetModuleHandleA( nullptr );

			WNDCLASSEXA windowClass;
			windowClass.cbSize = sizeof( WNDCLASSEXA );

			//
			BOOL classFindResult = ::GetClassInfoExA( wndProcModuleHandle, wndClassName, &windowClass );

			if( classFindResult == FALSE )
			{
				windowClass.cbSize = sizeof( WNDCLASSEXA );
				// Note this! We need one integer value for a simple ref counter.
				windowClass.cbClsExtra = sizeof( LONG_PTR );
				windowClass.cbWndExtra = 0;
				windowClass.hbrBackground = static_cast<HBRUSH>( ::GetStockObject( GRAY_BRUSH ) );
				windowClass.hCursor = ::LoadCursorA( nullptr, IDC_ARROW );
				windowClass.hIcon = ::LoadIconA( nullptr, IDI_WINLOGO );
				windowClass.hIconSm = ::LoadIconA( nullptr, IDI_WINLOGO );
				windowClass.hInstance = wndProcModuleHandle;
				windowClass.lpszClassName = wndClassName;
				windowClass.lpfnWndProc = Win32DefaultWindowEventCallback;
				windowClass.lpszMenuName = nullptr;
				windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;

				//
				auto wndClassID = ::RegisterClassExA( &windowClass );
				if( wndClassID == 0 )
				{
					Ic3Throw( eExcCodeDebugPlaceholder );
				}

				pWindowNativeData.wndClsID = wndClassID;
			}

			pWindowNativeData.wndClsName = wndClassName;
			pWindowNativeData.moduleHandle = wndProcModuleHandle;
		}

		DWORD _Win32TranslateFrameStyle( EFrameStyle pStyle )
		{
			//
			constexpr DWORD cvCaptionFrameStyle = WS_CAPTION | WS_SYSMENU;
			//
			constexpr DWORD cvFixedFrameStyle = WS_CAPTION | WS_SYSMENU | WS_OVERLAPPED | WS_MINIMIZEBOX;
			//
			constexpr DWORD cvOverlayFrameStyle = WS_POPUP | WS_EX_TOPMOST;
			//
			constexpr DWORD cvResizeableFrameStyle = cvFixedFrameStyle | WS_SIZEBOX | WS_MAXIMIZEBOX;

			//
			DWORD resultStyle = cppx::meta::limits<DWORD>::max_value;

			switch ( pStyle )
			{
				case EFrameStyle::Caption:
				{
					resultStyle = cvCaptionFrameStyle;
					break;
				}
				case EFrameStyle::Fixed:
				{
					resultStyle = cvFixedFrameStyle;
					break;
				}
				case EFrameStyle::OVERLAY:
				{
					resultStyle = cvOverlayFrameStyle;
					break;
				}
				case EFrameStyle::Resizeable:
				{
					resultStyle = cvResizeableFrameStyle;
					break;
				}
				default:
				{
					resultStyle = 0u;
					break;
				}
			}

			return resultStyle;
		}

		EFrameStyle _win32QueryWindowStyle( HWND pHWND )
		{
			const DWORD cvStyleSupportMask = WS_CAPTION | WS_SYSMENU | WS_OVERLAPPED | WS_MINIMIZEBOX |
			                                 WS_POPUP | WS_EX_TOPMOST | WS_SIZEBOX | WS_MAXIMIZEBOX;

			const auto cvWindowStyle = ::GetWindowLongA( pHWND, GWL_STYLE );

			cppx::bitmask<DWORD> windowStyleMask = ( cvWindowStyle & cvStyleSupportMask );

			EFrameStyle frameStyle = EFrameStyle::Unspecified;

			if( windowStyleMask.is_set( WS_POPUP ) && !windowStyleMask.is_set( WS_SYSMENU ) )
			{
				frameStyle = EFrameStyle::OVERLAY;
			}
			else if( windowStyleMask.is_set_any_of( WS_SIZEBOX | WS_MAXIMIZEBOX ) )
			{
				frameStyle = EFrameStyle::Resizeable;
			}
			else if( windowStyleMask.is_set( WS_OVERLAPPED ) )
			{
				frameStyle = EFrameStyle::Fixed;
			}
			else if( windowStyleMask.is_set_any_of( WS_SYSMENU | WS_CAPTION ) )
			{
				frameStyle = EFrameStyle::Caption;
			}

			return frameStyle;
		}

		FrameGeometry _win32QueryCurrentWindowGeometry( HWND pHWND )
		{
			RECT windowRect;
			::GetWindowRect( pHWND, &windowRect );

			FrameGeometry windowGeometry{};
			windowGeometry.position.x = windowRect.left;
			windowGeometry.position.y = windowRect.top;
			windowGeometry.size.x = static_cast<uint32>( windowRect.right - windowRect.left );
			windowGeometry.size.y = static_cast<uint32>( windowRect.bottom - windowRect.top );
			windowGeometry.style = _win32QueryWindowStyle( pHWND );

			return windowGeometry;
		}

		Win32FrameGeometry _win32CheckFrameGeometry( const FrameGeometry & pFrameGeometry )
		{
			Win32FrameGeometry win32Geometry{};
			win32Geometry.frameRect.left = static_cast<LONG>( pFrameGeometry.position.x );
			win32Geometry.frameRect.top = static_cast<LONG>( pFrameGeometry.position.y );
			win32Geometry.frameRect.right = win32Geometry.frameRect.left + static_cast<LONG>( pFrameGeometry.size.x );
			win32Geometry.frameRect.bottom = win32Geometry.frameRect.top + static_cast<LONG>( pFrameGeometry.size.y );
			win32Geometry.style = _Win32TranslateFrameStyle( pFrameGeometry.style );

			::AdjustWindowRect( &( win32Geometry.frameRect ), win32Geometry.style, FALSE );

			return win32Geometry;
		}

		Win32FrameGeometry _win32CheckFrameGeometryUpdate( HWND pWindowHwnd,
		                                                   const FrameGeometry & pFrameGeometry,
		                                                   cppx::bitmask<EFrameGeometryUpdateFlags> pUpdateFlags )
		{
			Win32FrameGeometry win32Geometry{};

			if( pUpdateFlags.is_set( E_FRAME_GEOMETRY_UPDATE_FLAG_STYLE_BIT ) )
			{
				// If the style-related flag is set, it means the window style will change.
				// Current style is irrelevant - translate the new one and save it in result.
				win32Geometry.style = _Win32TranslateFrameStyle( pFrameGeometry.style );
			}
			else
			{
				win32Geometry.style = ::GetWindowLongA( pWindowHwnd, GWL_STYLE );
			}

			if( pUpdateFlags.is_set_any_of( E_FRAME_GEOMETRY_UPDATE_FLAG_POSITION_BIT | E_FRAME_GEOMETRY_UPDATE_FLAG_SIZE_BIT ) )
			{
				RECT currentRect;
				::GetWindowRect( pWindowHwnd, &currentRect );

				auto framePosX = currentRect.left;
				auto framePosY = currentRect.top;
				auto frameWidth = currentRect.right - currentRect.left;
				auto frameHeight = currentRect.bottom - currentRect.top;

				if( pUpdateFlags.is_set( E_FRAME_GEOMETRY_UPDATE_FLAG_POSITION_BIT ) )
				{
					framePosX = static_cast<LONG>( pFrameGeometry.position.x );
					framePosY = static_cast<LONG>( pFrameGeometry.position.y );
				}
				if( pUpdateFlags.is_set( E_FRAME_GEOMETRY_UPDATE_FLAG_SIZE_BIT ) )
				{
					frameWidth = static_cast<LONG>( pFrameGeometry.size.x );
					frameHeight = static_cast<LONG>( pFrameGeometry.size.y );
				}

				win32Geometry.frameRect.left = framePosX;
				win32Geometry.frameRect.top = framePosY;
				win32Geometry.frameRect.right = framePosX + frameWidth;
				win32Geometry.frameRect.bottom = framePosY + frameHeight;

				if( pUpdateFlags.is_set( E_FRAME_GEOMETRY_UPDATE_FLAG_SIZE_CLIENT_AREA_BIT ) )
				{
					::AdjustWindowRect( &( win32Geometry.frameRect ), win32Geometry.style, FALSE );

					// AdjustWindowRect() modifies the position as well. We need to preserve the calculated values.
					// So, we compute the offset applied to the framePosX/Y and adjust the whole rect accordingly.

					const auto xOffset = win32Geometry.frameRect.left - framePosX;
					const auto yOffset = win32Geometry.frameRect.top - framePosY;

					win32Geometry.frameRect.left -= xOffset;
					win32Geometry.frameRect.right -= xOffset;
					win32Geometry.frameRect.top -= yOffset;
					win32Geometry.frameRect.bottom -= yOffset;
				}
			}
			else
			{
				::GetWindowRect( pWindowHwnd, &( win32Geometry.frameRect ) );
			}

			return win32Geometry;
		}

	}

} // namespace Ic3::System
#endif // PCL_TARGET_SYSAPI_WIN32
