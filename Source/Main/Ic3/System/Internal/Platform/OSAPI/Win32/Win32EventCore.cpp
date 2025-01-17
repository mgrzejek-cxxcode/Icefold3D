
#include "Win32WindowSystem.h"
#include <Ic3/System/Internal/EventCorePrivate.h>
#include <Ic3/Math/VectorOps.h>

#if( PCL_TARGET_SYSAPI == PCL_TARGET_SYSAPI_WIN32 )
namespace Ic3::System
{

	namespace Platform
	{

		LRESULT __stdcall _Win32EventSourceProxyEventProc( HWND pHWND, UINT pMessage, WPARAM pWparam, LPARAM pLparam );

		bool _Win32TranslateAppOrWindowEvent(
				Win32EventController & pEventControlle,
				EventSource & pEventSource,
				const MSG & pMSG,
				EventObject & pOutEvent );

		bool _Win32TranslateInputEvent(
				Win32EventController & pEventController,
				EventSource & pEventSource,
				const MSG & pMSG,
				EventObject & pOutEvent );

		bool _Win32TranslateInputEventKeyboard( Win32EventController & pEventController, const MSG & pMSG, EventObject & pOutEvent );

		bool _Win32TranslateInputEventMouse( Win32EventController & pEventController, const MSG & pMSG, EventObject & pOutEvent );

		bool _Win32TranslateInputEventTouch( Win32EventController & pEventController, const MSG & pMSG, EventObject & pOutEvent );

		bool _Win32TranslateInputEventGesture( Win32EventController & pEventController, const MSG & pMSG, EventObject & pOutEvent );

		EWindowVisibilityState _Win32TranslateWindowVisibility( WPARAM pWparam );

		EKeyCode _Win32GetSysKeyCode( WPARAM pWparam );

		cppx::bitmask<EMouseButtonFlagBits> _Win32GetMouseButtonStateMask( WPARAM pWparam );

	}
	
	
	Win32EventController::Win32EventController( SysContextHandle pSysContext )
	: Win32NativeObject( std::move( pSysContext ) )
	{}

	Win32EventController::~Win32EventController() noexcept
	{}

	void Win32EventController::_NativeRegisterEventSource( EventSource & pEventSource )
	{
		auto * eventSourceNativeData = pEventSource.getEventSourceNativeDataAs<Platform::Win32EventSourceNativeData>();
		Ic3DebugAssert( eventSourceNativeData != nullptr );

		auto * win32EventSourceState = new Platform::Win32EventSourceState();
		win32EventSourceState->eventController = this;
		win32EventSourceState->mSavedEventCallback = ::GetWindowLongPtrA( eventSourceNativeData->hwnd, GWLP_WNDPROC );
		win32EventSourceState->mSavedEventCallbackUserData = ::GetWindowLongPtrA( eventSourceNativeData->hwnd, GWLP_USERDATA );

		auto win32EventSourceStateAddress = reinterpret_cast<LONG_PTR>( win32EventSourceState );
		::SetWindowLongPtrA( eventSourceNativeData->hwnd, GWLP_USERDATA, win32EventSourceStateAddress );

		auto eventSourceProcAddress = reinterpret_cast<LONG_PTR>( Platform::_Win32EventSourceProxyEventProc );
		::SetWindowLongPtrA( eventSourceNativeData->hwnd, GWLP_WNDPROC, eventSourceProcAddress );

		// Trigger the update of the window to ensure changes are visible.
		::SetWindowPos( eventSourceNativeData->hwnd, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED );
	}

	void Win32EventController::_NativeUnRegisterEventSource( EventSource & pEventSource )
	{
        auto * eventSourceNativeData = pEventSource.getEventSourceNativeDataAs<Platform::Win32EventSourceNativeData>();
        Ic3DebugAssert( eventSourceNativeData != nullptr );

		LONG_PTR windowUserData = ::GetWindowLongPtrA( eventSourceNativeData->hwnd, GWLP_USERDATA );

		if( auto * win32EventSourceState = reinterpret_cast<Platform::Win32EventSourceState *>( windowUserData ) )
		{
            ::SetWindowLongPtrA( eventSourceNativeData->hwnd, GWLP_WNDPROC, win32EventSourceState->mSavedEventCallback );

            ::SetWindowLongPtrA( eventSourceNativeData->hwnd, GWLP_USERDATA, win32EventSourceState->mSavedEventCallbackUserData );

            // Trigger the update of the window to ensure changes are visible.
            ::SetWindowPos( eventSourceNativeData->hwnd, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED );
		}
	}

	bool Win32EventController::_NativeDispatchPendingEvents()
	{
		MSG pMSG;
		if( ::PeekMessageA( &pMSG, nullptr, 0, 0, PM_REMOVE ) != FALSE )
		{
			::TranslateMessage( &pMSG );
			::DispatchMessageA( &pMSG );

			return true;
		}

		return false;
	}

	bool Win32EventController::_NativeDispatchPendingEventsWait()
	{
		MSG pMSG;
		if( ::GetMessageA( &pMSG, nullptr, 0, 0 ) != FALSE )
		{
			::TranslateMessage( &pMSG );
			::DispatchMessageA( &pMSG );

			return true;
		}

		return false;
	}


	namespace Platform
	{

		bool NativeEventTranslate( EventController & pEventController, const NativeEventType & pNativeEvent, EventObject & pOutEvent )
		{
			auto * win32EventController = pEventController.QueryInterface<Win32EventController>();

			return Win32TranslateEvent( *win32EventController, pNativeEvent, pOutEvent );
		}

		EventSource * Win32FindEventSourceByHWND( Win32EventController & pEventController, HWND pHWND )
		{
			auto * eventSource = pEventController.FindEventSource( [pHWND]( const EventSource & pEventSource ) -> bool {
				const auto * eventSourceNativeData = pEventSource.getEventSourceNativeDataAs<Platform::Win32EventSourceNativeData>();
				return eventSourceNativeData->hwnd == pHWND;
			});
			return eventSource;
		}

		bool Win32TranslateEvent( Win32EventController & pEventController, const MSG & pMSG, EventObject & pOutEvent )
		{
			auto * eventSource = Win32FindEventSourceByHWND( pEventController, pMSG.mHWND );

			if( pMSG.message >= WM_KEYFIRST && pMSG.message <= WM_TOUCH )
			{
				if( eventSource && _Win32TranslateInputEvent( pEventController, *eventSource, pMSG, pOutEvent ) )
				{
					return true;
				}
			}
			else
			{
				if( eventSource && _Win32TranslateAppOrWindowEvent( pEventController, *eventSource, pMSG, pOutEvent ) )
				{
					return true;
				}
			}

			return false;
		}

		LRESULT __stdcall Win32DefaultWindowEventCallback( HWND pHWND, UINT pMessage, WPARAM pWparam, LPARAM pLparam )
		{
			switch( pMessage )
			{
				case WM_NCCREATE:
				{
					LONG_PTR windowUserData = ::GetWindowLongPtrA( pHWND, GWLP_USERDATA );
					if( windowUserData != 0 )
					{
						// TODO: Log this!
						Ic3DebugInterrupt();
					}
					auto * win32EventSourceState = new Platform::Win32EventSourceState();
					auto win32EventSourceStateAddress = reinterpret_cast<LONG_PTR>( win32EventSourceState );
					::SetWindowLongPtrA( pHWND, GWLP_USERDATA, win32EventSourceStateAddress );
					break;
				}
				case WM_NCDESTROY:
				{
					LONG_PTR windowUserData = ::GetWindowLongPtrA( pHWND, GWLP_USERDATA );
					if( windowUserData != 0 )
					{
						auto * win32EventSourceState = reinterpret_cast<Platform::Win32EventSourceState *>( windowUserData );
						delete win32EventSourceState;
						::SetWindowLongPtrA( pHWND, GWLP_USERDATA, 0 );
					}
					break;
				}
			}
			return ::DefWindowProcA( pHWND, pMessage, pWparam, pLparam );
		}

		LRESULT __stdcall _Win32EventSourceProxyEventProc( HWND pHWND, UINT pMessage, WPARAM pWparam, LPARAM pLparam )
		{
			// The UserData value is a pointer to the internal EventState of the current window.
			// It is created before this callback gets injected and released when NCDESTROY message arrives.
			LONG_PTR windowUserData = ::GetWindowLongPtrA( pHWND, GWLP_USERDATA );

			if( windowUserData != 0 )
			{
				// This our state.
				// TODO: Some validation might be useful to check if this callback is used correctly.
				auto * win32EventSourceState = reinterpret_cast<Win32EventSourceState *>( windowUserData );

				NativeEventType nativeEvent;
				nativeEvent.mHWND = pHWND;
				nativeEvent.message = pMessage;
				nativeEvent.wParam = pWparam;
				nativeEvent.lParam = pLparam;

				nativeEventDispatch( *( win32EventSourceState->eventController ), nativeEvent );
			}

			return ::DefWindowProcA( pHWND, pMessage, pWparam, pLparam );
		}

		bool _Win32TranslateAppOrWindowEvent(
				Win32EventController & pEventController,
				EventSource & pEventSource,
				const MSG & pMSG,
				EventObject & pOutEvent )
		{
			switch ( pMSG.message )
			{
				case WM_SHOWWINDOW:
				{
					auto & eWindowUpdateVisibility = pOutEvent.eWindowUpdateVisibility;
					eWindowUpdateVisibility.eventCode = E_EVENT_CODE_WINDOW_UPDATE_VISIBILITY;
					eWindowUpdateVisibility.newVisibilityState = _Win32TranslateWindowVisibility( pMSG.wParam );
					eWindowUpdateVisibility.eventSource = &pEventSource;
					break;
				}
				case WM_SIZE:
				{
					auto & eWindowUpdateResize = pOutEvent.eWindowUpdateResize;
					eWindowUpdateResize.eventCode = E_EVENT_CODE_WINDOW_UPDATE_RESIZE;
					eWindowUpdateResize.newSize.x = LOWORD( pMSG.lParam );
					eWindowUpdateResize.newSize.y = HIWORD( pMSG.lParam );
					eWindowUpdateResize.eventSource = &pEventSource;
					break;
				}
				case WM_CLOSE:
				{
					auto & eWindowUpdateDestroy = pOutEvent.eWindowUpdateDestroy;
					eWindowUpdateDestroy.eventCode = E_EVENT_CODE_WINDOW_UPDATE_DESTROY;
					eWindowUpdateDestroy.eventSource = &pEventSource;
					break;
				}
				case WM_QUIT:
				{
					auto & eAppActivityQuit = pOutEvent.eAppActivityQuit;
					eAppActivityQuit.eventCode = E_EVENT_CODE_APP_ACTIVITY_QUIT;
					break;
				}
				// This is our custom message for fullscreen state changes.
				case Platform::CX_WIN32_MESSAGE_ID_FULLSCREEN_STATE_CHANGE:
				{
					auto * win32WindowNativeData = pEventSource.getEventSourceNativeDataAs<Platform::Win32WindowNativeData>();

					// Fullscreen state is encoded in the WPARAM argument. Note, that it will always differ
					// from the current fullscreen state (this is checked upfront before sending the message).
					const auto IsFullscreenEnabled = ( pMSG.wParam != 0 );

					// The actual update. Sets proper window geometry, updates the style.
					// This is handled automatically by the WM on X11, but here we need to explicitly do it.
					Platform::Win32UpdateWindowFullscreenState( *win32WindowNativeData, IsFullscreenEnabled );

					auto & eWindowUpdateFullscreen = pOutEvent.eWindowUpdateFullscreen;
					eWindowUpdateFullscreen.eventCode = E_EVENT_CODE_WINDOW_UPDATE_FULLSCREEN;
					eWindowUpdateFullscreen.fullscreenState = IsFullscreenEnabled ? EActiveState::ENABLED : EActiveState::DISABLED;
					eWindowUpdateFullscreen.eventSource = &pEventSource;

					break;
				}
				default:
				{
					return false;
				}
			}

			return true;
		}

		bool _Win32TranslateInputEvent(
				Win32EventController & pEventController,
				EventSource & pEventSource,
				const MSG & pMSG,
				EventObject & pOutEvent )
		{
			if( ( pMSG.message >= WM_KEYFIRST ) && ( pMSG.message <= WM_KEYLAST ) )
			{
				return _Win32TranslateInputEventKeyboard( pEventController, pMSG, pOutEvent );
			}
			else if( ( pMSG.message >= WM_MOUSEFIRST ) && ( pMSG.message <= WM_MOUSELAST ) )
			{
				return _Win32TranslateInputEventMouse( pEventController, pMSG, pOutEvent );
			}
		#if( WINVER >= 0x0602 )
			else if( pMSG.message == WM_TOUCH )
			{
				return _Win32TranslateInputEventTouch( pEventController, pMSG, pOutEvent );
			}
		#endif
			else if( ( pMSG.message == WM_GESTURE ) || ( pMSG.message == WM_GESTURENOTIFY ) )
			{
				return _Win32TranslateInputEventGesture( pEventController, pMSG, pOutEvent );
			}
			else
			{
				return false;
			}
		}

		bool _Win32TranslateInputEventKeyboard( Win32EventController & pEventController, const MSG & pMSG, EventObject & pOutEvent )
		{
			auto & inputKeyboardState = pEventController.getEventSystemSharedState().inputKeyboardState;

			switch( pMSG.message )
			{
				case WM_KEYDOWN:
				case WM_SYSKEYDOWN:
				{
					auto & eInputKeyboard = pOutEvent.eInputKeyboard;
					eInputKeyboard.eventCode = E_EVENT_CODE_INPUT_KEYBOARD;
					eInputKeyboard.inputKeyboardState = &inputKeyboardState;
					eInputKeyboard.keyAction = EKeyActionType::Press;
					eInputKeyboard.keyCode = _Win32GetSysKeyCode( pMSG.wParam );
					break;
				}
				case WM_KEYUP:
				case WM_SYSKEYUP:
				{
					auto & eInputKeyboard = pOutEvent.eInputKeyboard;
					eInputKeyboard.eventCode = E_EVENT_CODE_INPUT_KEYBOARD;
					eInputKeyboard.inputKeyboardState = &inputKeyboardState;
					eInputKeyboard.keyAction = EKeyActionType::Release;
					eInputKeyboard.keyCode = _Win32GetSysKeyCode( pMSG.wParam );
					break;
				}
				default:
				{
					return false;
				}
			}

			return true;
		}

		bool _Win32TranslateInputEventMouse( Win32EventController & pEventController, const MSG & pMSG, EventObject & pOutEvent )
		{
			const auto cursorPos = Math::Vec2i32{
				GET_X_LPARAM( pMSG.lParam ),
				GET_Y_LPARAM( pMSG.lParam )
			};

			auto & inputMouseState = pEventController.getEventSystemSharedState().inputMouseState;

			if( inputMouseState.lastCursorPos == CX_EVENT_MOUSE_POS_INVALID )
			{
				inputMouseState.lastCursorPos = cursorPos;
			}

			switch( pMSG.message )
			{
				case WM_MOUSEMOVE:
				{
					auto & eInputMouseMove = pOutEvent.eInputMouseMove;
					eInputMouseMove.eventCode = E_EVENT_CODE_INPUT_MOUSE_MOVE;
					eInputMouseMove.cursorPos = cursorPos;
					eInputMouseMove.buttonStateMask = _Win32GetMouseButtonStateMask( pMSG.wParam );
					eInputMouseMove.movementDelta = cursorPos - inputMouseState.lastCursorPos;
					eInputMouseMove.inputMouseState = &inputMouseState;
					inputMouseState.lastCursorPos = cursorPos;
					break;
				}
				case WM_LBUTTONDOWN:
				{
					auto & eInputMouseButton = pOutEvent.eInputMouseButton;
					eInputMouseButton.eventCode = E_EVENT_CODE_INPUT_MOUSE_BUTTON;
					eInputMouseButton.cursorPos = cursorPos;
					eInputMouseButton.buttonAction = EMouseButtonActionType::Click;
					eInputMouseButton.buttonID = EMouseButtonID::Left;
					eInputMouseButton.buttonStateMask = _Win32GetMouseButtonStateMask( pMSG.wParam );
					break;
				}
				case WM_LBUTTONUP:
				{
					auto & eInputMouseButton = pOutEvent.eInputMouseButton;
					eInputMouseButton.eventCode = E_EVENT_CODE_INPUT_MOUSE_BUTTON;
					eInputMouseButton.cursorPos = cursorPos;
					eInputMouseButton.buttonAction = EMouseButtonActionType::Release;
					eInputMouseButton.buttonID = EMouseButtonID::Left;
					eInputMouseButton.buttonStateMask = _Win32GetMouseButtonStateMask( pMSG.wParam );
					break;
				}
				case WM_LBUTTONDBLCLK:
				{
					pOutEvent.code = E_EVENT_CODE_UNDEFINED;
					break;
				}
				case WM_RBUTTONDOWN:
				{
					auto & eInputMouseButton = pOutEvent.eInputMouseButton;
					eInputMouseButton.eventCode = E_EVENT_CODE_INPUT_MOUSE_BUTTON;
					eInputMouseButton.cursorPos = cursorPos;
					eInputMouseButton.buttonAction = EMouseButtonActionType::Click;
					eInputMouseButton.buttonID = EMouseButtonID::Right;
					eInputMouseButton.buttonStateMask = _Win32GetMouseButtonStateMask( pMSG.wParam );
					break;
				}
				case WM_RBUTTONUP:
				{
					auto & eInputMouseButton = pOutEvent.eInputMouseButton;
					eInputMouseButton.eventCode = E_EVENT_CODE_INPUT_MOUSE_BUTTON;
					eInputMouseButton.cursorPos = cursorPos;
					eInputMouseButton.buttonAction = EMouseButtonActionType::Release;
					eInputMouseButton.buttonID = EMouseButtonID::Right;
					eInputMouseButton.buttonStateMask = _Win32GetMouseButtonStateMask( pMSG.wParam );
					break;
				}
				case WM_RBUTTONDBLCLK:
				{
					pOutEvent.code = E_EVENT_CODE_UNDEFINED;
					break;
				}
				case WM_MBUTTONDOWN:
				{
					auto & eInputMouseButton = pOutEvent.eInputMouseButton;
					eInputMouseButton.eventCode = E_EVENT_CODE_INPUT_MOUSE_BUTTON;
					eInputMouseButton.cursorPos = cursorPos;
					eInputMouseButton.buttonAction = EMouseButtonActionType::Click;
					eInputMouseButton.buttonID = EMouseButtonID::Middle;
					eInputMouseButton.buttonStateMask = _Win32GetMouseButtonStateMask( pMSG.wParam );
					break;
				}
				case WM_MBUTTONUP:
				{
					auto & eInputMouseButton = pOutEvent.eInputMouseButton;
					eInputMouseButton.eventCode = E_EVENT_CODE_INPUT_MOUSE_BUTTON;
					eInputMouseButton.cursorPos = cursorPos;
					eInputMouseButton.buttonAction = EMouseButtonActionType::Release;
					eInputMouseButton.buttonID = EMouseButtonID::Middle;
					eInputMouseButton.buttonStateMask = _Win32GetMouseButtonStateMask( pMSG.wParam );
					break;
				}
				case WM_MBUTTONDBLCLK:
				{
					pOutEvent.code = E_EVENT_CODE_UNDEFINED;
					break;
				}
				case WM_MOUSEWHEEL:
				{
					auto & eInputMouseScroll = pOutEvent.eInputMouseScroll;
					pOutEvent.code = E_EVENT_CODE_INPUT_MOUSE_SCROLL;
					eInputMouseScroll.eventCode = E_EVENT_CODE_INPUT_MOUSE_SCROLL;
					eInputMouseScroll.scrollDelta.x = 0;
					eInputMouseScroll.scrollDelta.y = static_cast<double>( GET_WHEEL_DELTA_WPARAM( pMSG.wParam ) ) / WHEEL_DELTA;
					break;
				}
				case WM_XBUTTONDOWN:
				{
					auto & eInputMouseButton = pOutEvent.eInputMouseButton;
					eInputMouseButton.eventCode = E_EVENT_CODE_INPUT_MOUSE_BUTTON;
					eInputMouseButton.cursorPos = cursorPos;
					eInputMouseButton.buttonAction = EMouseButtonActionType::Click;
					const auto xButtonID = GET_XBUTTON_WPARAM( pMSG.wParam );
					eInputMouseButton.buttonID = ( xButtonID == XBUTTON1 ) ? EMouseButtonID::XB1 : EMouseButtonID::XB2;
					eInputMouseButton.buttonStateMask = _Win32GetMouseButtonStateMask( pMSG.wParam );
					break;
				}
				case WM_XBUTTONUP:
				{
					auto & eInputMouseButton = pOutEvent.eInputMouseButton;
					eInputMouseButton.eventCode = E_EVENT_CODE_INPUT_MOUSE_BUTTON;
					eInputMouseButton.cursorPos = cursorPos;
					eInputMouseButton.buttonAction = EMouseButtonActionType::Release;
					const auto xButtonID = GET_XBUTTON_WPARAM( pMSG.wParam );
					eInputMouseButton.buttonID = ( xButtonID == XBUTTON1 ) ? EMouseButtonID::XB1 : EMouseButtonID::XB2;
					eInputMouseButton.buttonStateMask = _Win32GetMouseButtonStateMask( pMSG.wParam );
					break;
				}
				case WM_XBUTTONDBLCLK:
				{
					pOutEvent.code = E_EVENT_CODE_UNDEFINED;
					break;
				}
				case WM_MOUSEHWHEEL:
				{
					auto & eInputMouseScroll = pOutEvent.eInputMouseScroll;
					eInputMouseScroll.eventCode = E_EVENT_CODE_INPUT_MOUSE_SCROLL;
					eInputMouseScroll.scrollDelta.x = static_cast<double>( GET_WHEEL_DELTA_WPARAM( pMSG.wParam ) ) / WHEEL_DELTA;
					eInputMouseScroll.scrollDelta.y = 0.0;
					break;
				}
			}

			return pOutEvent.code != E_EVENT_CODE_UNDEFINED;
		}

		bool _Win32TranslateInputEventTouch( Win32EventController & pEventController, const MSG & pMSG, EventObject & pOutEvent )
		{
			return false;
		}

		bool _Win32TranslateInputEventGesture( Win32EventController & pEventController, const MSG & pMSG, EventObject & pOutEvent )
		{
			return false;
		}

		EWindowVisibilityState _Win32TranslateWindowVisibility( WPARAM pWparam )
		{
			if( pWparam != FALSE )
			{
				return EWindowVisibilityState::VISIBLE;
			}
			else
			{
				return EWindowVisibilityState::HIDDEN;
			}
		}

		static const EKeyCode sASCIIKeyCodeMap_08_7B[] =
		{
			/* 0x0008 */ EKeyCode::Backspace,
			/* 0x0009 */ EKeyCode::Tab,
			/* 0x000A */ EKeyCode::Unknown,
			/* 0x000B */ EKeyCode::Unknown,
			/* 0x000C */ EKeyCode::Unknown,
			/* 0x000D */ EKeyCode::Enter,
			/* 0x000E */ EKeyCode::Unknown,
			/* 0x000F */ EKeyCode::Unknown,
			/* 0x0010 */ EKeyCode::ShiftLeft,
			/* 0x0011 */ EKeyCode::CtrlLeft,
			/* 0x0012 */ EKeyCode::AltLeft,
			/* 0x0013 */ EKeyCode::Unknown,
			/* 0x0014 */ EKeyCode::CapsLock,
			/* 0x0015 */ EKeyCode::Unknown,
			/* 0x0016 */ EKeyCode::Unknown,
			/* 0x0017 */ EKeyCode::Unknown,
			/* 0x0018 */ EKeyCode::Unknown,
			/* 0x0019 */ EKeyCode::Unknown,
			/* 0x001A */ EKeyCode::Unknown,
			/* 0x001B */ EKeyCode::Escape,
			/* 0x001C */ EKeyCode::Unknown,
			/* 0x001D */ EKeyCode::Unknown,
			/* 0x001E */ EKeyCode::Unknown,
			/* 0x001F */ EKeyCode::Unknown,
			/* 0x0020 */ EKeyCode::Space,
			/* 0x0021 */ EKeyCode::PageUp,
			/* 0x0022 */ EKeyCode::PageDown,
			/* 0x0023 */ EKeyCode::End,
			/* 0x0024 */ EKeyCode::Home,
			/* 0x0025 */ EKeyCode::ArrowLeft,
			/* 0x0026 */ EKeyCode::ArrowUp,
			/* 0x0027 */ EKeyCode::ArrowRight,
			/* 0x0028 */ EKeyCode::ArrowDown,
			/* 0x0029 */ EKeyCode::Unknown,
			/* 0x002A */ EKeyCode::Unknown,
			/* 0x002B */ EKeyCode::Unknown,
			/* 0x002C */ EKeyCode::Unknown,
			/* 0x002D */ EKeyCode::Insert,
			/* 0x002E */ EKeyCode::Delete,
			/* 0x002F */ EKeyCode::Unknown,
			/* 0x0030 */ EKeyCode::Num0,
			/* 0x0031 */ EKeyCode::Num1,
			/* 0x0032 */ EKeyCode::Num2,
			/* 0x0033 */ EKeyCode::Num3,
			/* 0x0034 */ EKeyCode::Num4,
			/* 0x0035 */ EKeyCode::Num5,
			/* 0x0036 */ EKeyCode::Num6,
			/* 0x0037 */ EKeyCode::Num7,
			/* 0x0038 */ EKeyCode::Num8,
			/* 0x0039 */ EKeyCode::Num9,
			/* 0x003A */ EKeyCode::Unknown,
			/* 0x003B */ EKeyCode::Unknown,
			/* 0x003C */ EKeyCode::Unknown,
			/* 0x003D */ EKeyCode::Unknown,
			/* 0x003E */ EKeyCode::Unknown,
			/* 0x003F */ EKeyCode::Unknown,
			/* 0x0040 */ EKeyCode::Unknown,
			/* 0x0041 */ EKeyCode::CharA,
			/* 0x0042 */ EKeyCode::CharB,
			/* 0x0043 */ EKeyCode::CharC,
			/* 0x0044 */ EKeyCode::CharD,
			/* 0x0045 */ EKeyCode::CharE,
			/* 0x0046 */ EKeyCode::CharF,
			/* 0x0047 */ EKeyCode::CharG,
			/* 0x0048 */ EKeyCode::CharH,
			/* 0x0049 */ EKeyCode::CharI,
			/* 0x004A */ EKeyCode::CharJ,
			/* 0x004B */ EKeyCode::CharK,
			/* 0x004C */ EKeyCode::CharL,
			/* 0x004D */ EKeyCode::CharM,
			/* 0x004E */ EKeyCode::CharN,
			/* 0x004F */ EKeyCode::CharO,
			/* 0x0050 */ EKeyCode::CharP,
			/* 0x0051 */ EKeyCode::CharQ,
			/* 0x0052 */ EKeyCode::CharR,
			/* 0x0053 */ EKeyCode::CharS,
			/* 0x0054 */ EKeyCode::CharT,
			/* 0x0055 */ EKeyCode::CharU,
			/* 0x0056 */ EKeyCode::CharV,
			/* 0x0057 */ EKeyCode::CharW,
			/* 0x0058 */ EKeyCode::CharX,
			/* 0x0059 */ EKeyCode::CharY,
			/* 0x005A */ EKeyCode::CharZ,
			/* 0x005B */ EKeyCode::Unknown,
			/* 0x005C */ EKeyCode::Unknown,
			/* 0x005D */ EKeyCode::Unknown,
			/* 0x005E */ EKeyCode::Unknown,
			/* 0x005F */ EKeyCode::Unknown,
			/* 0x0060 */ EKeyCode::NumPad0,
			/* 0x0061 */ EKeyCode::NumPad1,
			/* 0x0062 */ EKeyCode::NumPad2,
			/* 0x0063 */ EKeyCode::NumPad3,
			/* 0x0064 */ EKeyCode::NumPad4,
			/* 0x0065 */ EKeyCode::NumPad5,
			/* 0x0066 */ EKeyCode::NumPad6,
			/* 0x0067 */ EKeyCode::NumPad7,
			/* 0x0068 */ EKeyCode::NumPad8,
			/* 0x0069 */ EKeyCode::NumPad9,
			/* 0x006A */ EKeyCode::NumPadMul,
			/* 0x006B */ EKeyCode::NumPadAdd,
			/* 0x006C */ EKeyCode::Unknown,
			/* 0x006D */ EKeyCode::NumPadSub,
			/* 0x006E */ EKeyCode::NumPadDot,
			/* 0x006F */ EKeyCode::NumPadDiv,
			/* 0x0070 */ EKeyCode::F1,
			/* 0x0071 */ EKeyCode::F2,
			/* 0x0072 */ EKeyCode::F3,
			/* 0x0073 */ EKeyCode::F4,
			/* 0x0074 */ EKeyCode::F5,
			/* 0x0075 */ EKeyCode::F6,
			/* 0x0076 */ EKeyCode::F7,
			/* 0x0077 */ EKeyCode::F8,
			/* 0x0078 */ EKeyCode::F9,
			/* 0x0079 */ EKeyCode::F10,
			/* 0x007A */ EKeyCode::F11,
			/* 0x007B */ EKeyCode::F12
		};

		static const EKeyCode sASCIIKeyCodeMap_A0_A5[] =
		{
			/* 0x00A0 */ EKeyCode::ShiftLeft,
			/* 0x00A1 */ EKeyCode::ShiftRight,
			/* 0x00A2 */ EKeyCode::CtrlLeft,
			/* 0x00A3 */ EKeyCode::CtrlRight,
			/* 0x00A4 */ EKeyCode::AltLeft,
			/* 0x00A5 */ EKeyCode::AltRight,
		};

		EKeyCode _Win32GetSysKeyCode( WPARAM pWparam )
		{
			if( ( pWparam >= 0x0008 ) && ( pWparam <= 0x007B ) )
			{
				const auto baseIndex = pWparam - 0x0008;
				return sASCIIKeyCodeMap_08_7B[baseIndex];
			}

			if( ( pWparam >= 0x00A0 ) && ( pWparam <= 0x00A5 ) )
			{
				const auto baseIndex = pWparam - 0x00A0;
				return sASCIIKeyCodeMap_A0_A5[baseIndex];
			}

			return EKeyCode::Unknown;
		}

		cppx::bitmask<EMouseButtonFlagBits> _Win32GetMouseButtonStateMask( WPARAM pWparam )
		{
			return static_cast<EMouseButtonFlagBits>( static_cast<uint32>( pWparam ) & E_MOUSE_BUTTON_FLAG_ALL_BIT );
		}

	}

} // namespace Ic3::System
#endif // PCL_TARGET_SYSAPI_WIN32
