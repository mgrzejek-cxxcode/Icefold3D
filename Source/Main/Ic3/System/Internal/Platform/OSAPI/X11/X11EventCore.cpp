
#include "X11EventCore.h"
#include "X11WindowSystem.h"
#include <Ic3/System/Internal/EventCorePrivate.h>
#include <Ic3/Math/VectorOps.h>
#include <X11/keysym.h>

#if( PCL_TARGET_SYSAPI == PCL_TARGET_SYSAPI_X11 )
namespace Ic3::System
{

	namespace Platform
	{

		bool _X11TranslateGenericEvent( EventSource & pEventSource, const XEvent & pXEvent, EventObject & pOutEvent );
		
		bool _X11TranslateInputEvent( EventSource & pEventSource, const XEvent & pXEvent, EventObject & pOutEvent );
		
		bool _X11TranslateSystemEvent( EventSource & pEventSource, const XEvent & pXEvent, EventObject & pOutEvent );
		
		EKeyCode _X11GetSysKeyCode( KeySym pXkeySym );
		
		EX11MouseButtonID _X11GetMouseButtonID( uint32 pButton );
		
		cppx::bitmask<EMouseButtonFlagBits> _X11GetMouseButtonStateMask( uint32 pState );
		
	}


	X11EventController::X11EventController( SysContextHandle pSysContext )
	: X11NativeObject( std::move( pSysContext ) )
	{}

	X11EventController::~X11EventController() noexcept
	{}

	void X11EventController::_NativeRegisterEventSource( EventSource & /* pEventSource */ )
	{}

	void X11EventController::_NativeUnRegisterEventSource( EventSource & /* pEventSource */ )
	{}

	bool X11EventController::_NativeDispatchPendingEvents()
	{
		auto & xSessionData = Platform::X11GetXSessionData( *this );

		Platform::NativeEventType x11NativeEvent{};
		if( XEventsQueued( xSessionData.displayHandle, QueuedAfterReading ) > 0 )
		{
			XNextEvent( xSessionData.displayHandle, &( x11NativeEvent.mXEvent ) );
			Platform::nativeEventDispatch( *this, x11NativeEvent );
			return true;
		}

		return false;
	}

	bool X11EventController::_NativeDispatchPendingEventsWait()
	{
		auto & xSessionData = Platform::X11GetXSessionData( *this );

		Platform::NativeEventType x11NativeEvent{};
		if( XEventsQueued( xSessionData.displayHandle, QueuedAfterFlush ) > 0 )
		{
			XNextEvent( xSessionData.displayHandle, &( x11NativeEvent.mXEvent ) );
			Platform::nativeEventDispatch( *this, x11NativeEvent );
			return true;
		}

		return false;
	}


	namespace Platform
	{

		bool NativeEventTranslate( EventController & pEventController, const NativeEventType & pNativeEvent, EventObject & pOutEvent )
		{
			auto * x11EventController = pEventController.QueryInterface<X11EventController>();
            
			if( auto * eventSource = X11FindEventSourceByXWindow( *x11EventController, pNativeEvent.mXEvent.xany.window ) )
			{
				return X11TranslateEvent( *eventSource, pNativeEvent.mXEvent, pOutEvent );
			}

			return false;
		}

		EventSource * X11FindEventSourceByXWindow( X11EventController & pEventController, XWindow pWindowXID )
		{
			auto * eventSource = pEventController.FindEventSource( [pWindowXID]( const EventSource & pEventSource ) -> bool {
				const auto * eventSourceNativeData = pEventSource.getEventSourceNativeDataAs<Platform::X11EventSourceNativeData>();
				return eventSourceNativeData->mWindowXID == pWindowXID;
			});
			return eventSource;
		}

		const char * translateEventTypeName( int pEventType )
		{
			switch( pEventType )
			{
				ic3CaseReturnStr( KeyPress );
				ic3CaseReturnStr( KeyRelease );
				ic3CaseReturnStr( ButtonPress );
				ic3CaseReturnStr( ButtonRelease );
				ic3CaseReturnStr( MotionNotify );
				ic3CaseReturnStr( EnterNotify );
				ic3CaseReturnStr( LeaveNotify );
				ic3CaseReturnStr( FocusIn );
				ic3CaseReturnStr( FocusOut );
				ic3CaseReturnStr( KeymapNotify );
				ic3CaseReturnStr( Expose );
				ic3CaseReturnStr( GraphicsExpose );
				ic3CaseReturnStr( NoExpose );
				ic3CaseReturnStr( VisibilityNotify );
				ic3CaseReturnStr( CreateNotify );
				ic3CaseReturnStr( DestroyNotify );
				ic3CaseReturnStr( UnmapNotify );
				ic3CaseReturnStr( MapNotify );
				ic3CaseReturnStr( MapRequest );
				ic3CaseReturnStr( ReparentNotify );
				ic3CaseReturnStr( ConfigureNotify );
				ic3CaseReturnStr( ConfigureRequest );
				ic3CaseReturnStr( GravityNotify );
				ic3CaseReturnStr( ResizeRequest );
				ic3CaseReturnStr( CirculateNotify );
				ic3CaseReturnStr( CirculateRequest );
				ic3CaseReturnStr( PropertyNotify );
				ic3CaseReturnStr( SelectionClear );
				ic3CaseReturnStr( SelectionRequest );
				ic3CaseReturnStr( SelectionNotify );
				ic3CaseReturnStr( ColormapNotify );
				ic3CaseReturnStr( ClientMessage );
				ic3CaseReturnStr( MappingNotify );
				ic3CaseReturnStr( GenericEvent );
				default: break;
			}
			return "UNKNOWN";
		}

		bool X11TranslateEvent( EventSource & pEventSource, const XEvent & pXEvent, EventObject & pOutEvent )
		{
			if( ( pXEvent.type >= KeyPress ) && ( pXEvent.type <= MotionNotify ) )
			{
				if( _X11TranslateInputEvent( pEventSource, pXEvent, pOutEvent ) )
				{
					return true;
				}
			}
			else if( ( pXEvent.type >= EnterNotify ) && ( pXEvent.type <= PropertyNotify ) )
			{
				if( _X11TranslateGenericEvent( pEventSource, pXEvent, pOutEvent  ) )
				{
					return true;
				}
			}
			else
			{
				if( _X11TranslateSystemEvent( pEventSource, pXEvent, pOutEvent ) )
				{
					return true;
				}
			}

			return false;
		}

		bool _X11TranslateGenericEvent( EventSource & pEventSource, const XEvent & pXEvent, EventObject & pOutEvent )
		{
			switch( pXEvent.type )
			{
				case EnterNotify:
				case LeaveNotify:
				case FocusIn:
				case FocusOut:
				case KeymapNotify:
				case Expose:
				case GraphicsExpose:
				case NoExpose:
				case VisibilityNotify:
				case CreateNotify:
					break;

				case DestroyNotify:
				{
					auto & eWindowUpdateDestroy = pOutEvent.uEvtWindowUpdateDestroy;
					eWindowUpdateDestroy.mEventCode = eEventCodeWindowUpdateDestroy;
					eWindowUpdateDestroy.eventSource = &pEventSource;
					break;
				}
				case UnmapNotify:
				{
					auto & eWindowUpdateVisibility = pOutEvent.uEvtWindowUpdateVisibility;
					eWindowUpdateVisibility.mEventCode = eEventCodeWindowUpdateVisibility;
					eWindowUpdateVisibility.newVisibilityState = EWindowVisibilityState::Hidden;
					eWindowUpdateVisibility.eventSource = &pEventSource;
					break;
				}
				case MapNotify:
				{
					auto & eWindowUpdateVisibility = pOutEvent.uEvtWindowUpdateVisibility;
					eWindowUpdateVisibility.mEventCode = eEventCodeWindowUpdateVisibility;
					eWindowUpdateVisibility.newVisibilityState = EWindowVisibilityState::Visible;
					eWindowUpdateVisibility.eventSource = &pEventSource;
					break;
				}
				case MapRequest:
				case ReparentNotify:
				case ConfigureNotify:
				case ConfigureRequest:
				case GravityNotify:
				case ResizeRequest:
				case CirculateNotify:
				case CirculateRequest:
					break;

				case PropertyNotify:
				{
					auto * eventSourceNativeData = pEventSource.getEventSourceNativeDataAs<Platform::X11EventSourceNativeData>();
					auto & xSessionData = X11GetXSessionData( *eventSourceNativeData );

					if( pXEvent.xproperty.atom == xSessionData.atomCache.wmState )
					{
						// PropertyNotify is emitted after the state has been changed.
						// Check what is the current state of WM_STATE_FULLSCREEN.
						const auto IsFullscreenWindow = Platform::X11IsFullscreenWindow( xSessionData.displayHandle, eventSourceNativeData->mWindowXID );

						// This is the cached fullscreen state, updated on each change.
						const auto previousFullscreenState = eventSourceNativeData->mSysWindowFlags.is_set( eX11SystemWindowFlagWMStateFullscreen );

						if( IsFullscreenWindow != previousFullscreenState )
						{
							auto & eWindowUpdateFullscreen = pOutEvent.uEvtWindowUpdateFullscreen;
							eWindowUpdateFullscreen.mEventCode = eEventCodeWindowUpdateFullscreen;
							eWindowUpdateFullscreen.fullscreenState = IsFullscreenWindow ? EActiveState::ENABLED : EActiveState::DISABLED;
							eWindowUpdateFullscreen.eventSource = &pEventSource;
							// Save the current state inside the native data of the event source.
							eventSourceNativeData->mSysWindowFlags.setOrUnset( eX11SystemWindowFlagWMStateFullscreen, IsFullscreenWindow );
						}
					}
					break;
				}
				default:
				{
					return false;
				}
			}

			return true;
		}

		bool _X11TranslateInputEventMouseButton(
                EventSource & pEventSource,
                const XEvent & pXEvent,
                EventObject & pOutEvent,
                EMouseButtonActionType pActionType );

		bool _X11TranslateInputEvent( EventSource & pEventSource, const XEvent & pXEvent, EventObject & pOutEvent )
		{
            auto & eventController = pEventSource.getEventControllerRef();

			switch( pXEvent.type )
			{
				// Note on the keysym index:
				// The standard rules for obtaining a KeySym from a KeyPress event make use of only the Group 1
				// and Group 2 KeySyms, no interpretation of other KeySyms in the list is given. Which group to
				// use is determined by the modifier state.
				// [Source: https://tronche.com/gui/x/xlib/input/keyboard-encoding.html#KeySym]
				case KeyPress:
				{
					auto & inputKeyboardState = eventController.getEventSystemSharedState().mInputKeyboardState;

					// XLookupKeysym requires non-const XKeyEvent parameter.
					// Use locally copied one to keep the API right (we don't want to drop 'const for input event param).
					auto xKey = pXEvent.xkey;
					auto keysym = XLookupKeysym( &xKey, 0 );
					auto keycode = _X11GetSysKeyCode( keysym );
					auto & eInputKeyboard = pOutEvent.uEvtInputKeyboard;
                    eInputKeyboard.mEventCode = eEventCodeInputKeyboard;
                    eInputKeyboard.mInputKeyboardState = &inputKeyboardState;
                    eInputKeyboard.mKeyCode = keycode;
                    eInputKeyboard.mKeyAction = EKeyActionType::Press;

					break;
				}
				case KeyRelease:
				{
                    auto & inputKeyboardState = eventController.getEventSystemSharedState().mInputKeyboardState;

					// XLookupKeysym requires non-const XKeyEvent parameter.
					// Use locally copied one to keep the API right (we don't want to drop 'const for input event param).
					auto xKey = pXEvent.xkey;
					auto keysym = XLookupKeysym( &xKey, 0 );
					auto keycode = _X11GetSysKeyCode( keysym );
					auto & eInputKeyboard = pOutEvent.uEvtInputKeyboard;
                    eInputKeyboard.mEventCode = eEventCodeInputKeyboard;
                    eInputKeyboard.mInputKeyboardState = &inputKeyboardState;
                    eInputKeyboard.mKeyCode = keycode;
                    eInputKeyboard.mKeyAction = EKeyActionType::Release;

					break;
				}
				case ButtonPress:
				{
					_X11TranslateInputEventMouseButton(
                            pEventSource,
                            pXEvent,
                            pOutEvent,
                            EMouseButtonActionType::Click );

					break;
				}
				case ButtonRelease:
				{
					_X11TranslateInputEventMouseButton(
                            pEventSource,
                            pXEvent,
                            pOutEvent,
                            EMouseButtonActionType::Release );


					break;
				}
				case MotionNotify:
				{
                    auto & inputMouseState = eventController.getEventSystemSharedState().inputMouseState;
					const Math::Vec2i32 cursorPos { pXEvent.xmotion.x, pXEvent.xmotion.y };

					if( inputMouseState.lastCursorPos == CX_EVENT_MOUSE_POS_INVALID )
					{
						inputMouseState.lastCursorPos = cursorPos;
					}

					auto & eInputMouseMove = pOutEvent.uEvtInputMouseMove;
					eInputMouseMove.mEventCode = eEventCodeInputMouseMove;
					eInputMouseMove.cursorPos = cursorPos;

					// Old method, based on the state mask provided by the system. Although *some* modifiers are there,
					// extra mouse buttons (X1 and X2) are not reported. Since there is our own internal state mask kept
					// inside the EventInputState object, we just use that. This line is supposed to be kept here just
					// for the future reference or if additional modifiers need to be supported.
					// eInputMouseMove.buttonStateMask = _X11GetMouseButtonStateMask( pXEvent.xmotion.state );

					eInputMouseMove.buttonStateMask = inputMouseState.buttonStateMask;
					eInputMouseMove.movementDelta = cursorPos - inputMouseState.lastCursorPos;

					inputMouseState.lastCursorPos = cursorPos;

					break;
				}
				default:
				{
					return false;
				}
			}

			return pOutEvent.uCommonData.mEventCode != eEventCodeUndefined;
		}

		bool _X11TranslateInputEventMouseButton(
                EventSource & pEventSource,
                const XEvent & pXEvent,
                EventObject & pOutEvent,
                EMouseButtonActionType pActionType )
		{
            auto & eventController = pEventSource.getEventControllerRef();

            const auto x11MouseButtonID = _X11GetMouseButtonID( pXEvent.xbutton.button );

			switch( x11MouseButtonID )
			{
				case EX11MouseButtonID::Left:
				{
					auto & inputMouseState = eventController.getEventSystemSharedState().inputMouseState;
					auto & eInputMouseButton = pOutEvent.uEvtInputMouseButton;
					eInputMouseButton.mEventCode = eEventCodeInputMouseButton;
					eInputMouseButton.cursorPos = { pXEvent.xbutton.x, pXEvent.xbutton.y };;
					eInputMouseButton.buttonAction = pActionType;
					eInputMouseButton.buttonID = EMouseButtonID::Left;

					if( pActionType == EMouseButtonActionType::Click )
					{
						inputMouseState.buttonStateMask.set( eMouseButtonFlagLeftBit );
					}
					else if( pActionType == EMouseButtonActionType::Release )
					{
						inputMouseState.buttonStateMask.unset( eMouseButtonFlagLeftBit );
					}

					break;
				}
				case EX11MouseButtonID::Middle:
				{
                    auto & inputMouseState = eventController.getEventSystemSharedState().inputMouseState;
					auto & eInputMouseButton = pOutEvent.uEvtInputMouseButton;
					eInputMouseButton.mEventCode = eEventCodeInputMouseButton;
					eInputMouseButton.cursorPos = { pXEvent.xbutton.x, pXEvent.xbutton.y };;
					eInputMouseButton.buttonAction = pActionType;
					eInputMouseButton.buttonID = EMouseButtonID::Middle;

					if( pActionType == EMouseButtonActionType::Click )
					{
						inputMouseState.buttonStateMask.set( eMouseButtonFlagMiddleBit );
					}
					else if( pActionType == EMouseButtonActionType::Release )
					{
						inputMouseState.buttonStateMask.unset( eMouseButtonFlagMiddleBit );
					}

					break;
				}
				case EX11MouseButtonID::Right:
				{
                    auto & inputMouseState = eventController.getEventSystemSharedState().inputMouseState;
					auto & eInputMouseButton = pOutEvent.uEvtInputMouseButton;
					eInputMouseButton.mEventCode = eEventCodeInputMouseButton;
					eInputMouseButton.cursorPos = { pXEvent.xbutton.x, pXEvent.xbutton.y };;
					eInputMouseButton.buttonAction = pActionType;
					eInputMouseButton.buttonID = EMouseButtonID::Right;

					if( pActionType == EMouseButtonActionType::Click )
					{
						inputMouseState.buttonStateMask.set( eMouseButtonFlagRightBit );
					}
					else if( pActionType == EMouseButtonActionType::Release )
					{
						inputMouseState.buttonStateMask.unset( eMouseButtonFlagRightBit );
					}

					break;
				}
				case EX11MouseButtonID::VWheelUp:
				{
					auto & eInputMouseScroll = pOutEvent.uEvtInputMouseScroll;
					eInputMouseScroll.mEventCode = eEventCodeInputMouseScroll;
					eInputMouseScroll.scrollDelta.x = 0;
					eInputMouseScroll.scrollDelta.y = 100;
					break;
				}
				case EX11MouseButtonID::VWheelDown:
				{
					auto & eInputMouseScroll = pOutEvent.uEvtInputMouseScroll;
					eInputMouseScroll.mEventCode = eEventCodeInputMouseScroll;
					eInputMouseScroll.scrollDelta.x = 0;
					eInputMouseScroll.scrollDelta.y = -100;

					break;
				}
				case EX11MouseButtonID::HWheelLeft:
				{
					auto & eInputMouseScroll = pOutEvent.uEvtInputMouseScroll;
					eInputMouseScroll.mEventCode = eEventCodeInputMouseScroll;
					eInputMouseScroll.scrollDelta.x = -100;
					eInputMouseScroll.scrollDelta.y = 0;

					break;
				}
				case EX11MouseButtonID::HWheelRight:
				{
					auto & eInputMouseScroll = pOutEvent.uEvtInputMouseScroll;
					eInputMouseScroll.mEventCode = eEventCodeInputMouseScroll;
					eInputMouseScroll.scrollDelta.x = 100;
					eInputMouseScroll.scrollDelta.y = 0;

					break;
				}
				case EX11MouseButtonID::XBT1:
				{
                    auto & inputMouseState = eventController.getEventSystemSharedState().inputMouseState;
					auto & eInputMouseButton = pOutEvent.uEvtInputMouseButton;
					eInputMouseButton.mEventCode = eEventCodeInputMouseButton;
					eInputMouseButton.cursorPos = { pXEvent.xbutton.x, pXEvent.xbutton.y };;
					eInputMouseButton.buttonAction = pActionType;
					eInputMouseButton.buttonID = EMouseButtonID::XB1;

					if( pActionType == EMouseButtonActionType::Click )
					{
						inputMouseState.buttonStateMask.set( eMouseButtonFlagXB1Bit );
					}
					else if( pActionType == EMouseButtonActionType::Release )
					{
						inputMouseState.buttonStateMask.unset( eMouseButtonFlagXB1Bit );
					}

					break;
				}
				case EX11MouseButtonID::XBT2:
				{
                    auto & inputMouseState = eventController.getEventSystemSharedState().inputMouseState;
					auto & eInputMouseButton = pOutEvent.uEvtInputMouseButton;
					eInputMouseButton.mEventCode = eEventCodeInputMouseButton;
					eInputMouseButton.cursorPos = { pXEvent.xbutton.x, pXEvent.xbutton.y };;
					eInputMouseButton.buttonAction = pActionType;
					eInputMouseButton.buttonID = EMouseButtonID::XB2;

					if( pActionType == EMouseButtonActionType::Click )
					{
						inputMouseState.buttonStateMask.set( eMouseButtonFlagXB2Bit );
					}
					else if( pActionType == EMouseButtonActionType::Release )
					{
						inputMouseState.buttonStateMask.unset( eMouseButtonFlagXB2Bit );
					}

					break;
				}
				default:
				{
					return false;
				}
			}

			return true;
		}

		bool _X11TranslateSystemEvent( EventSource & pEventSource, const XEvent & pXEvent, EventObject & pOutEvent )
		{
			switch( pXEvent.type )
			{
				case ClientMessage:
				{
					auto & xSessionData = Platform::X11GetXSessionData( *( pEventSource.mSysContext ) );

					// printf("--ClientMessage: %s\n", XGetAtomName( pXEvent.xclient.displayHandle, pXEvent.xclient.message_type ) );

					if( pXEvent.xclient.message_type == xSessionData.atomCache.wmProtocol )
					{
						// Window Message Protocol (WMP) id is stored in data.l[0].
						if( pXEvent.xclient.data.l[0] == xSessionData.atomCache.wmProtocolDelete )
						{
							auto & eventData = pOutEvent.uEvtWindowUpdateDestroy;
							eventData.mEventCode = eEventCodeWindowUpdateDestroy;
							eventData.eventSource = &pEventSource;
						}
					}

					break;
				}
				default:
				{
					return false;
				}
			}

			return true;
		}


		static const EKeyCode x11KeycodeMap_20_7A[] =
		{
			/* 0x0020 */ EKeyCode::Space,
			/* 0x00xx */ EKeyCode::Unknown,
			/* 0x00xx */ EKeyCode::Unknown,
			/* 0x00xx */ EKeyCode::Unknown,
			/* 0x00xx */ EKeyCode::Unknown,
			/* 0x00xx */ EKeyCode::Unknown,
			/* 0x00xx */ EKeyCode::Unknown,
			/* 0x00xx */ EKeyCode::Unknown,
			/* 0x00xx */ EKeyCode::Unknown,
			/* 0x00xx */ EKeyCode::Unknown,
			/* 0x00xx */ EKeyCode::Unknown,
			/* 0x00xx */ EKeyCode::Unknown,
			/* 0x00xx */ EKeyCode::Unknown,
			/* 0x00xx */ EKeyCode::Unknown,
			/* 0x00xx */ EKeyCode::Unknown,
			/* 0x00xx */ EKeyCode::Unknown,
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
			/* 0x00xx */ EKeyCode::Unknown,
			/* 0x00xx */ EKeyCode::Unknown,
			/* 0x00xx */ EKeyCode::Unknown,
			/* 0x00xx */ EKeyCode::Unknown,
			/* 0x00xx */ EKeyCode::Unknown,
			/* 0x00xx */ EKeyCode::Unknown,
			/* 0x00xx */ EKeyCode::Unknown,
			/* 0x0041 */ EKeyCode::CharA,
			/* 0x0042 */ EKeyCode::CharB,
			/* 0x0043 */ EKeyCode::CharC,
			/* 0x0044 */ EKeyCode::CharD,
			/* 0x0045 */ EKeyCode::CharE,
			/* 0x0046 */ EKeyCode::CharF,
			/* 0x0047 */ EKeyCode::CharG,
			/* 0x0048 */ EKeyCode::CharH,
			/* 0x0049 */ EKeyCode::CharI,
			/* 0x004a */ EKeyCode::CharJ,
			/* 0x004b */ EKeyCode::CharK,
			/* 0x004c */ EKeyCode::CharL,
			/* 0x004d */ EKeyCode::CharM,
			/* 0x004e */ EKeyCode::CharN,
			/* 0x004f */ EKeyCode::CharO,
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
			/* 0x005a */ EKeyCode::CharZ,
			/* 0x00xx */ EKeyCode::Unknown,
			/* 0x00xx */ EKeyCode::Unknown,
			/* 0x00xx */ EKeyCode::Unknown,
			/* 0x00xx */ EKeyCode::Unknown,
			/* 0x00xx */ EKeyCode::Unknown,
			/* 0x00xx */ EKeyCode::Unknown,
			/* 0x0061 */ EKeyCode::CharA,
			/* 0x0062 */ EKeyCode::CharB,
			/* 0x0063 */ EKeyCode::CharC,
			/* 0x0064 */ EKeyCode::CharD,
			/* 0x0065 */ EKeyCode::CharE,
			/* 0x0066 */ EKeyCode::CharF,
			/* 0x0067 */ EKeyCode::CharG,
			/* 0x0068 */ EKeyCode::CharH,
			/* 0x0069 */ EKeyCode::CharI,
			/* 0x006a */ EKeyCode::CharJ,
			/* 0x006b */ EKeyCode::CharK,
			/* 0x006c */ EKeyCode::CharL,
			/* 0x006d */ EKeyCode::CharM,
			/* 0x006e */ EKeyCode::CharN,
			/* 0x006f */ EKeyCode::CharO,
			/* 0x0070 */ EKeyCode::CharP,
			/* 0x0071 */ EKeyCode::CharQ,
			/* 0x0072 */ EKeyCode::CharR,
			/* 0x0073 */ EKeyCode::CharS,
			/* 0x0074 */ EKeyCode::CharT,
			/* 0x0075 */ EKeyCode::CharU,
			/* 0x0076 */ EKeyCode::CharV,
			/* 0x0077 */ EKeyCode::CharW,
			/* 0x0078 */ EKeyCode::CharX,
			/* 0x0079 */ EKeyCode::CharY,
			/* 0x007a */ EKeyCode::CharZ,
		};

		static const EKeyCode x11KeycodeMap_50_57[] =
		{
			/* 0xff50 */ EKeyCode::Home,
			/* 0xff51 */ EKeyCode::ArrowLeft,
			/* 0xff52 */ EKeyCode::ArrowUp,
			/* 0xff53 */ EKeyCode::ArrowRight,
			/* 0xff54 */ EKeyCode::ArrowDown,
			/* 0xff55 */ EKeyCode::PageUp,
			/* 0xff56 */ EKeyCode::PageDown,
			/* 0xff57 */ EKeyCode::End
		};

		static const EKeyCode x11KeycodeMap_AA_B9[] =
		{
			/* 0xffaa */ EKeyCode::NumPadMul,
			/* 0xffab */ EKeyCode::NumPadAdd,
			/* 0xffac */ EKeyCode::Unknown,
			/* 0xffad */ EKeyCode::NumPadSub,
			/* 0xffae */ EKeyCode::NumPadDot,
			/* 0xffaf */ EKeyCode::NumPadDiv,
			/* 0xffb0 */ EKeyCode::NumPad0,
			/* 0xffb1 */ EKeyCode::NumPad1,
			/* 0xffb2 */ EKeyCode::NumPad2,
			/* 0xffb3 */ EKeyCode::NumPad3,
			/* 0xffb4 */ EKeyCode::NumPad4,
			/* 0xffb5 */ EKeyCode::NumPad5,
			/* 0xffb6 */ EKeyCode::NumPad6,
			/* 0xffb7 */ EKeyCode::NumPad7,
			/* 0xffb8 */ EKeyCode::NumPad8,
			/* 0xffb9 */ EKeyCode::NumPad9,
		};

		static const EKeyCode x11KeycodeMap_BE_C9[] =
		{
			/* 0xffbe */ EKeyCode::F1,
			/* 0xffbf */ EKeyCode::F2,
			/* 0xffc0 */ EKeyCode::F3,
			/* 0xffc1 */ EKeyCode::F4,
			/* 0xffc2 */ EKeyCode::F5,
			/* 0xffc3 */ EKeyCode::F6,
			/* 0xffc4 */ EKeyCode::F7,
			/* 0xffc5 */ EKeyCode::F8,
			/* 0xffc6 */ EKeyCode::F9,
			/* 0xffc7 */ EKeyCode::F10,
			/* 0xffc8 */ EKeyCode::F11,
			/* 0xffc9 */ EKeyCode::F12
		};

		static const EKeyCode x11KeycodeMap_E1_EA[] =
		{
			/* 0xffe1 */ EKeyCode::ShiftLeft,
			/* 0xffe2 */ EKeyCode::ShiftRight,
			/* 0xffe3 */ EKeyCode::CtrlLeft,
			/* 0xffe4 */ EKeyCode::CtrlRight,
			/* 0xffe5 */ EKeyCode::CapsLock,
			/* 0xffe6 */ EKeyCode::Unknown,
			/* 0xffe7 */ EKeyCode::Unknown,
			/* 0xffe8 */ EKeyCode::Unknown,
			/* 0xffe9 */ EKeyCode::AltLeft,
			/* 0xffea */ EKeyCode::AltRight
		};


		EKeyCode _X11GetSysKeyCode( KeySym pXkeySym )
		{
			if( ( pXkeySym >= 0x0020 ) && ( pXkeySym <= 0x007a ) )
			{
				auto baseIndex = static_cast<uint32>( pXkeySym ) - 0x0020;
				return x11KeycodeMap_20_7A[baseIndex];
			}

			if( ( pXkeySym >= 0xff50 ) && ( pXkeySym <= 0xffea ) )
			{
				if( ( pXkeySym >= 0xff50 ) && ( pXkeySym <= 0xff57 ) )
				{
					auto baseIndex = static_cast<uint32>( pXkeySym ) - 0xff50;
					return x11KeycodeMap_50_57[baseIndex];
				}

				if( ( pXkeySym >= 0xffaa ) && ( pXkeySym <= 0xffb9 ) )
				{
					auto baseIndex = static_cast<uint32>( pXkeySym ) - 0xffaa;
					return x11KeycodeMap_AA_B9[baseIndex];
				}

				if( ( pXkeySym >= 0xffbe ) && ( pXkeySym <= 0xffc9 ) )
				{
					auto baseIndex = static_cast<uint32>( pXkeySym ) - 0xffbe;
					return x11KeycodeMap_BE_C9[baseIndex];
				}

				if( ( pXkeySym >= 0xffe1 ) && ( pXkeySym <= 0xffea ) )
				{
					auto baseIndex = static_cast<uint32>( pXkeySym ) - 0xffe1;
					return x11KeycodeMap_E1_EA[baseIndex];
				}
			}

			if( pXkeySym == XK_Tab )
			{
				return EKeyCode::Tab;
			}
			else if( pXkeySym == XK_Return )
			{
				return EKeyCode::Enter;
			}
			else if( pXkeySym == XK_Escape )
			{
				return EKeyCode::Escape;
			}
			else if( pXkeySym == XK_BackSpace )
			{
				return EKeyCode::Backspace;
			}
			else if( pXkeySym == XK_Insert )
			{
				return EKeyCode::Insert;
			}
			else if( pXkeySym == XK_Delete )
			{
				return EKeyCode::Delete;
			}

			return EKeyCode::Unknown;
		}

		static constexpr EX11MouseButtonID x11MouseButtonIDMap[] =
		{
			EX11MouseButtonID::Left,
			EX11MouseButtonID::Middle,
			EX11MouseButtonID::Right,
			EX11MouseButtonID::VWheelUp,
			EX11MouseButtonID::VWheelDown,
			EX11MouseButtonID::HWheelLeft,
			EX11MouseButtonID::HWheelRight,
			EX11MouseButtonID::XBT1,
			EX11MouseButtonID::XBT2,
			EX11MouseButtonID::Unknown
		};
		static constexpr auto x11MouseButtonMaxIndex = cppx::static_array_size( x11MouseButtonIDMap ) - 1;

		EX11MouseButtonID _X11GetMouseButtonID( uint32 pButton )
		{
			auto mouseButtonIndex = static_cast<size_t>( pButton - 1 );
			mouseButtonIndex = cppx::get_min_of( mouseButtonIndex, x11MouseButtonMaxIndex );
			return x11MouseButtonIDMap[mouseButtonIndex];
		}

		cppx::bitmask<EMouseButtonFlagBits> _X11GetMouseButtonStateMask( uint32 pState )
		{
            cppx::bitmask<uint32> inputStateMask = pState;
            cppx::bitmask<EMouseButtonFlagBits> buttonStateMask;
			if( pState != 0 )
			{
				if( inputStateMask.is_set_any_of( Button1Mask | Button1MotionMask ) )
				{
					inputStateMask.unset( Button1Mask | Button1MotionMask );
					buttonStateMask.set( eMouseButtonFlagLeftBit );
				}
				if( inputStateMask.is_set_any_of( Button2Mask | Button2MotionMask ) )
				{
					inputStateMask.unset( Button2Mask | Button2MotionMask );
					buttonStateMask.set( eMouseButtonFlagMiddleBit );
				}
				if( inputStateMask.is_set_any_of( Button3Mask | Button3MotionMask ) )
				{
					inputStateMask.unset( Button3Mask | Button3MotionMask );
					buttonStateMask.set( eMouseButtonFlagRightBit );
				}
			}
			return buttonStateMask;
		}
	}

} // namespace Ic3::System
#endif // PCL_TARGET_SYSAPI_X11
