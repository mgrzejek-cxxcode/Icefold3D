
#include "OSXEventCore.h"
#include "NSIEventListener.h"
#include "NSIWindow.h"
#include "NSIKeyboardSupport.h"
#include <Ic3/Cppx/Range.h>
#include <Ic3/System/Internal/EventCorePrivate.h>
#include <AppKit/NSScreen.h>

namespace Ic3::System
{

	namespace Platform
	{

		void _osxTranslateEventInputKeyboard( EventController & pEventController, EventSource & pEventSource, const NativeEventType & pNativeEvent, EventObject & pOutEvent );

		bool _osxReadMouseMoveEvent( EventController & pEventController, const NativeEventType & pNativeEvent, EventObject & pOutEvent );

		void _osxTranslateEventInputMouse( EventController & pEventController, EventSource & pEventSource, const NativeEventType & pNativeEvent, EventObject & pOutEvent );

		void _osxTranslateEventInputOther( EventController & pEventController, EventSource & pEventSource, const NativeEventType & pNativeEvent, EventObject & pOutEvent );

		void _osxTranslateEventWindow( EventController & pEventController, EventSource & pEventSource, const NativeEventType & pNativeEvent, EventObject & pOutEvent );

	}

	OSXEventController::OSXEventController( SysContextHandle pSysContext )
	: OSXNativeObject( std::move( pSysContext ) )
	{}

	OSXEventController::~OSXEventController() noexcept = default;

	void OSXEventController::_nativeRegisterEventSource( EventSource & pEventSource )
	{
	@autoreleasepool
	{
		auto * eventSourceNativeData =
			pEventSource.getEventSourceNativeDataAs<Platform::OSXEventSourceNativeData>();

		auto * nsEventListener = ( NSOSXEventListener * )eventSourceNativeData->mNSEventListener;

		[nsEventListener bind:this];
	}
	}

	void OSXEventController::_nativeUnregisterEventSource( EventSource & pEventSource )
	{
	@autoreleasepool
	{
		auto * eventSourceNativeData =
				pEventSource.getEventSourceNativeDataAs<Platform::OSXEventSourceNativeData>();

		auto * nsEventListener = ( NSOSXEventListener * )eventSourceNativeData->mNSEventListener;

		[nsEventListener unbind];
	}
	}

	bool OSXEventController::_nativeDispatchPendingEvents()
	{
		for( NSEvent * nsEvent = nullptr; ; )
		{
			nsEvent = [NSApp nextEventMatchingMask:NSEventMaskAny
			                             untilDate:[NSDate distantPast]
			                                inMode:NSDefaultRunLoopMode
			                               dequeue:YES];

			if( nsEvent == nil )
			{
				return false;
			}

			[NSApp sendEvent:nsEvent];
		}

		return true;
	}

	bool OSXEventController::_nativeDispatchPendingEventsWait()
	{
		for( NSEvent * nsEvent = nullptr; ; )
		{
			nsEvent = [NSApp nextEventMatchingMask:NSEventMaskAny
			                             untilDate:[NSDate distantFuture]
			                                inMode:NSDefaultRunLoopMode
			                               dequeue:YES];

			if( nsEvent == nil )
			{
				return false;
			}

			[NSApp sendEvent:nsEvent];
		}

		return true;
	}

	namespace Platform
	{

		bool nativeEventTranslate( EventController & pEventController, const NativeEventType & pNativeEvent, EventObject & pOutEvent )
		{
			auto * osxEventController = pEventController.queryInterface<OSXEventController>();

			if( auto * eventSource = osxFindEventSourceByNSWindow( *osxEventController, pNativeEvent.mNSSourceWindow ) )
			{
				return osxTranslateEvent( pEventController, *eventSource, pNativeEvent, pOutEvent );
			}
			
			return false;
		}

		EventSource * osxFindEventSourceByNSWindow( OSXEventController & pEventController, NSWindow * pNSWindow )
		{
			auto * eventSource = pEventController.findEventSource( [pNSWindow]( const EventSource & pEventSource ) -> bool {
				const auto * eventSourceNativeData = pEventSource.getEventSourceNativeDataAs<Platform::OSXEventSourceNativeData>();
				return eventSourceNativeData->mNSWindow == pNSWindow;
			});
			return eventSource;
		}

		void osxCreateEventListener( OSXEventSourceNativeData & pEventSourceNativeData )
		{
		@autoreleasepool
		{
			if( ![( id )pEventSourceNativeData.mNSWindow isKindOfClass:[NSOSXWindow class]] )
			{
				return;
			}

			auto * nsWindow = ( NSOSXWindow * )( pEventSourceNativeData.mNSWindow );

			@try
			{
				auto * nsEventListener = [[NSOSXEventListener alloc] initForNSWindow:nsWindow];
				pEventSourceNativeData.mNSEventListener = nsEventListener;
			}
			@catch( NSException * pException )
			{
				const auto message = [[pException reason] UTF8String];
				ic3DebugInterrupt();
			}
		}
		}

		bool osxTranslateEvent( EventController & pEventController, EventSource & pEventSource, const NativeEventType & pNativeEvent, EventObject & pOutEvent )
		{
			if( osxIsEventTypeInputKeyboard( pNativeEvent.mNSAppEventID ) )
			{
				_osxTranslateEventInputKeyboard( pEventController, pEventSource, pNativeEvent, pOutEvent );
			}
			else if( osxIsEventTypeInputMouse( pNativeEvent.mNSAppEventID ) )
			{
				_osxTranslateEventInputMouse( pEventController, pEventSource, pNativeEvent, pOutEvent );
			}
			else if( osxIsEventTypeInputOther( pNativeEvent.mNSAppEventID ) )
			{
				_osxTranslateEventInputOther( pEventController, pEventSource, pNativeEvent, pOutEvent );
			}
			else if( osxIsEventTypeWindow( pNativeEvent.mNSAppEventID ) )
			{
				_osxTranslateEventWindow( pEventController, pEventSource, pNativeEvent, pOutEvent );
			}

			return pOutEvent.uCode != eEventCodeUndefined;
		}

		bool osxIsEventTypeInputKeyboard( EOSXEventID pEventID )
		{
			return ( pEventID >= eOSXEventIDGenericKeyDown ) && ( pEventID <= eOSXEventIDGenericFlagsChanged );
		}

		bool osxIsEventTypeInputMouse( EOSXEventID pEventID )
		{
			return ( ( pEventID >= eOSXEventIDGenericLeftMouseDown ) && ( pEventID <= eOSXEventIDGenericMouseExited ) ) ||
			       ( ( pEventID >= eOSXEventIDGenericScrollWheel ) && ( pEventID <= eOSXEventIDGenericOtherMouseDragged ) );
		}

		bool osxIsEventTypeInputOther( EOSXEventID pEventID )
		{
			return ( pEventID >= eOSXEventIDGenericGesture ) && ( pEventID <= eOSXEventIDGenericPressure );
		}

		bool osxIsEventTypeWindow( EOSXEventID pEventID )
		{
			return ( pEventID >= eOSXEventIDWindowDidExpose ) && ( pEventID <= eOSXEventIDWindowWillClose );
		}

		EMouseButtonID osxQueryMouseButtonID( NSEvent * pNSEvent )
		{
			static constexpr EMouseButtonID sMouseButtonIDArray[] =
			{
				/* 0 */ EMouseButtonID::Left,
				/* 1 */ EMouseButtonID::Right,
				/* 2 */ EMouseButtonID::Middle,
				/* 3 */ EMouseButtonID::XB1,
				/* 4 */ EMouseButtonID::XB2,
			};
			return Cppx::staticArrayElement( sMouseButtonIDArray, [pNSEvent buttonNumber] );
		}

		struct OSXMouseCursorState
		{
			CGPoint positionScreenSpace;
			CGPoint positionWindowSpace;
			bool isInsideWindowFrame = false;
			bool isInsideWindowClientArea = false;

			bool isInsideWindow() const noexcept
			{
				return isInsideWindowClientArea;
			}

			MouseCursorPos positionScreenSpaceI32() const
			{
				return {
					static_cast<int32>( std::round( positionScreenSpace.x ) ),
					static_cast<int32>( std::round( positionScreenSpace.y ) )
				};
			}

			MouseCursorPos positionWindowSpaceI32() const
			{
				return {
					static_cast<int32>( std::round( positionWindowSpace.x ) ),
					static_cast<int32>( std::round( positionWindowSpace.y ) )
				};
			}
		};

		OSXMouseCursorState osxQueryMouseCursorState( NSEvent * pNSEvent )
		{
			auto * nsSourceWindow = [pNSEvent window];

			const auto nsWindowFrame = [nsSourceWindow frame];
			const auto nsScreenFrame = [[nsSourceWindow screen] frame];
			const auto nsWindowStyle = [nsSourceWindow styleMask];

			const auto nsWindowClientAreaSize =
					[NSWindow contentRectForFrameRect:nsWindowFrame
					                        styleMask:nsWindowStyle];

			const auto mousePositionScreenSpace = [NSEvent mouseLocation];
			const auto mousePositionWindowSpace = [pNSEvent locationInWindow];
			bool mouseIsInsideWindowFrame = false;
			bool mouseIsInsideWindowClientArea = false;

			if( Cppx::rangeContains( { 0.0, nsWindowClientAreaSize.size.width }, mousePositionWindowSpace.x ) &&
			    Cppx::rangeContains( { 0.0, nsWindowClientAreaSize.size.height }, mousePositionWindowSpace.y ) )
			{
				mouseIsInsideWindowFrame = true;
				mouseIsInsideWindowClientArea = true;
			}
			else if( Cppx::rangeContains( { 0.0, nsWindowFrame.size.width }, mousePositionWindowSpace.x ) &&
			         Cppx::rangeContains( { 0.0, nsWindowFrame.size.height }, mousePositionWindowSpace.y ) )
			{
				mouseIsInsideWindowFrame = true;
			}

			OSXMouseCursorState osxCursorState;
			osxCursorState.positionScreenSpace.x = mousePositionScreenSpace.x;
			osxCursorState.positionScreenSpace.y = nsScreenFrame.size.height - mousePositionScreenSpace.y;
			osxCursorState.positionWindowSpace.x = mousePositionWindowSpace.x;
			osxCursorState.positionWindowSpace.y = nsWindowFrame.size.height - mousePositionWindowSpace.y;
			osxCursorState.isInsideWindowFrame = mouseIsInsideWindowFrame;
			osxCursorState.isInsideWindowClientArea = mouseIsInsideWindowClientArea;

			return osxCursorState;
		}

		TBitmask<EMouseButtonFlagBits> queryMouseButtonsActiveMask( NSEvent * pNSEvent )
		{
			const auto nsButtonMask = [NSEvent pressedMouseButtons];
			ic3DebugOutputFmt( "MouseButtonMask: %d", nsButtonMask );
			return nsButtonMask;
		}

		TBitmask<EOSXMouseScrollEventFlags> osxQueryMouseScrollEventFlags( const NativeEventType & pNativeEvent )
		{
			TBitmask<EOSXMouseScrollEventFlags> scrollEventMask = 0;

			if( [pNativeEvent.mNSEvent respondsToSelector:@selector(hasPreciseScrollingDeltas)] )
			{
				if( [pNativeEvent.mNSEvent hasPreciseScrollingDeltas] )
				{
					scrollEventMask.set( eOSXMouseScrollEventFlagScrollingModePreciseBit );
				}
				else
				{
					scrollEventMask.set( eOSXMouseScrollEventFlagScrollingModeNormalBit );
				}
			}

			if( [pNativeEvent.mNSEvent respondsToSelector:@selector(isDirectionInvertedFromDevice)] )
			{
				if( [pNativeEvent.mNSEvent isDirectionInvertedFromDevice] )
				{
					scrollEventMask.set( eOSXMouseScrollEventFlagScrollingDirectionInvertedBit );
				}
				else
				{
					scrollEventMask.set( eOSXMouseScrollEventFlagScrollingDirectionNormalBit );
				}
			}

			return scrollEventMask;
		}

		EKeyCode _osxGetSysKeyCodeV1( unsigned short pNSKeyCode );
		EKeyCode _osxGetSysKeyCodeV2( unsigned short pNSKeyCode );

		void _osxTranslateEventInputKeyboard( EventController & pEventController, EventSource & pEventSource, const NativeEventType & pNativeEvent, EventObject & pOutEvent )
		{
			auto & inputKeyboardState = pEventController.getEventSystemSharedState().mInputKeyboardState;

			switch( pNativeEvent.mNSAppEventID )
			{
				case eOSXEventIDGenericKeyDown:
				{
					auto & eInputKeyboard = pOutEvent.uEvtInputKeyboard;
					eInputKeyboard.mEventCode = eEventCodeInputKeyboard;
					inputKeyboardState.mActiveModifiersMask = osxTranslateModifierKeyFlags( [pNativeEvent.mNSEvent modifierFlags] );
					eInputKeyboard.mInputKeyboardState = &inputKeyboardState;
					eInputKeyboard.mKeyAction = EKeyActionType::Press;
					eInputKeyboard.mKeyCode = _osxGetSysKeyCodeV1( [pNativeEvent.mNSEvent keyCode] );
					break;
				}
				case eOSXEventIDGenericKeyUp:
				{
					auto & eInputKeyboard = pOutEvent.uEvtInputKeyboard;
					eInputKeyboard.mEventCode = eEventCodeInputKeyboard;
					inputKeyboardState.mActiveModifiersMask = osxTranslateModifierKeyFlags( [pNativeEvent.mNSEvent modifierFlags] );
					eInputKeyboard.mInputKeyboardState = &inputKeyboardState;
					eInputKeyboard.mKeyAction = EKeyActionType::Release;
					eInputKeyboard.mKeyCode = _osxGetSysKeyCodeV1( [pNativeEvent.mNSEvent keyCode] );
					break;
				}
				case eOSXEventIDGenericFlagsChanged:
				{
					const auto currentModifierFlags = osxTranslateModifierKeyFlags( [pNativeEvent.mNSEvent modifierFlags] );
					const auto previousModifierFlags = inputKeyboardState.mActiveModifiersMask;
					const auto modifierDiff = currentModifierFlags ^ previousModifierFlags;
					const auto modifierKeyCode = evt::getModifierKeyCodeFromModifierFlags( modifierDiff );

					auto & eInputKeyboard = pOutEvent.uEvtInputKeyboard;
					eInputKeyboard.mEventCode = eEventCodeInputKeyboard;
					inputKeyboardState.mActiveModifiersMask = currentModifierFlags;
					eInputKeyboard.mInputKeyboardState = &inputKeyboardState;
					eInputKeyboard.mKeyCode = evt::getModifierKeyCodeFromModifierFlags( modifierDiff );

					if( !previousModifierFlags.isSet( modifierDiff ) )
					{
						eInputKeyboard.mKeyAction = EKeyActionType::Press;
					}
					else
					{
						eInputKeyboard.mKeyAction = EKeyActionType::Release;
					}

					break;
				}
				default:
				{
					ic3DebugInterrupt();
				}
			}
		}

		EvtInputMouseScroll & osxProcessMouseScrollEvent( EventController & pEventController, const OSXMouseCursorState & pCursorState, EventObject & pOutEvent )
		{
			auto & controllerNativeData = pEventController.getInterface<OSXEventController>()->mNativeData;
			auto & inputMouseState = pEventController.getEventSystemSharedState().mInputMouseState;
			auto & eInputMouseScroll = pOutEvent.uEvtInputMouseScroll;

			if( inputMouseState.mLastCursorPos == CX_EVENT_MOUSE_POS_INVALID )
			{
				controllerNativeData.mLastCursorPosReal = pCursorState.positionScreenSpace;
				inputMouseState.mLastCursorPos = pCursorState.positionScreenSpaceI32();
			}

			eInputMouseScroll.mEventCode = eEventCodeInputMouseButton;
			eInputMouseScroll.mCursorPos = pCursorState.positionScreenSpaceI32();
			eInputMouseScroll.mInputMouseState = &inputMouseState;

			controllerNativeData.mLastCursorPosReal = pCursorState.positionScreenSpace;
			inputMouseState.mLastCursorPos = pCursorState.positionScreenSpaceI32();

			return eInputMouseScroll;
		}

		EvtInputMouseButton & osxProcessMouseButtonEvent( EventController & pEventController, const OSXMouseCursorState & pCursorState, EventObject & pOutEvent )
		{
			auto & controllerNativeData = pEventController.getInterface<OSXEventController>()->mNativeData;
			auto & inputMouseState = pEventController.getEventSystemSharedState().mInputMouseState;
			auto & eInputMouseButton = pOutEvent.uEvtInputMouseButton;

			if( inputMouseState.mLastCursorPos == CX_EVENT_MOUSE_POS_INVALID )
			{
				controllerNativeData.mLastCursorPosReal = pCursorState.positionScreenSpace;
				inputMouseState.mLastCursorPos = pCursorState.positionScreenSpaceI32();
			}

			eInputMouseButton.mEventCode = eEventCodeInputMouseButton;
			eInputMouseButton.mCursorPos = pCursorState.positionScreenSpaceI32();
			eInputMouseButton.mInputMouseState = &inputMouseState;

			controllerNativeData.mLastCursorPosReal = pCursorState.positionScreenSpace;
			inputMouseState.mLastCursorPos = pCursorState.positionScreenSpaceI32();

			return eInputMouseButton;
		}

		EvtInputMouseMove & osxProcessMouseMoveEvent( EventController & pEventController, const OSXMouseCursorState & pCursorState, EventObject & pOutEvent )
		{
			auto & controllerNativeData = pEventController.getInterface<OSXEventController>()->mNativeData;
			auto & inputMouseState = pEventController.getEventSystemSharedState().mInputMouseState;
			auto & eInputMouseMove = pOutEvent.uEvtInputMouseMove;

			if( inputMouseState.mLastCursorPos == CX_EVENT_MOUSE_POS_INVALID )
			{
				controllerNativeData.mLastCursorPosReal = pCursorState.positionScreenSpace;
				inputMouseState.mLastCursorPos = pCursorState.positionScreenSpaceI32();
			}

			eInputMouseMove.mEventCode = eEventCodeInputMouseMove;
			eInputMouseMove.mCursorPos = pCursorState.positionScreenSpaceI32();
			eInputMouseMove.mMovementDelta.x = static_cast<int32>( std::round( pCursorState.positionScreenSpace.x - inputMouseState.mLastCursorPos.x ) );
			eInputMouseMove.mMovementDelta.y = static_cast<int32>( std::round( pCursorState.positionScreenSpace.y - inputMouseState.mLastCursorPos.y ) );
			eInputMouseMove.mInputMouseState = &inputMouseState;

			controllerNativeData.mLastCursorPosReal = pCursorState.positionScreenSpace;
			inputMouseState.mLastCursorPos = pCursorState.positionScreenSpaceI32();

			return eInputMouseMove;
		}

		void _osxTranslateEventInputMouse( EventController & pEventController, EventSource & pEventSource, const NativeEventType & pNativeEvent, EventObject & pOutEvent )
		{
			const auto mouseCursorState = osxQueryMouseCursorState( pNativeEvent.mNSEvent );

			if( !mouseCursorState.isInsideWindow() )
			{
				auto & inputMouseState = pEventController.getEventSystemSharedState().mInputMouseState;
				inputMouseState.mLastCursorPos = CX_EVENT_MOUSE_POS_INVALID;
				return;
			}

			switch( pNativeEvent.mNSAppEventID )
			{
				case eOSXEventIDGenericLeftMouseDown:
				{
					auto & eInputMouseButton = osxProcessMouseButtonEvent( pEventController, mouseCursorState, pOutEvent );
					eInputMouseButton.mButtonAction = EMouseButtonActionType::Click;
					eInputMouseButton.mButtonID = EMouseButtonID::Left;
					break;
				}
				case eOSXEventIDGenericLeftMouseUp:
				{
					auto & eInputMouseButton = osxProcessMouseButtonEvent( pEventController, mouseCursorState, pOutEvent );
					eInputMouseButton.mButtonAction = EMouseButtonActionType::Release;
					eInputMouseButton.mButtonID = EMouseButtonID::Left;
					break;
				}
				case eOSXEventIDGenericRightMouseDown:
				{
					auto & eInputMouseButton = osxProcessMouseButtonEvent( pEventController, mouseCursorState, pOutEvent );
					eInputMouseButton.mButtonAction = EMouseButtonActionType::Click;
					eInputMouseButton.mButtonID = EMouseButtonID::Right;
					break;
				}
				case eOSXEventIDGenericRightMouseUp:
				{
					auto & eInputMouseButton = osxProcessMouseButtonEvent( pEventController, mouseCursorState, pOutEvent );
					eInputMouseButton.mButtonAction = EMouseButtonActionType::Release;
					eInputMouseButton.mButtonID = EMouseButtonID::Right;
					break;
				}
				case eOSXEventIDGenericMouseMoved:
				{
					auto & eInputMouseMove = osxProcessMouseMoveEvent( pEventController, mouseCursorState, pOutEvent );
					eInputMouseMove.mButtonStateMask = 0;
					break;
				}
				case eOSXEventIDGenericLeftMouseDragged:
				{
					auto & eInputMouseMove = osxProcessMouseMoveEvent( pEventController, mouseCursorState, pOutEvent );
					eInputMouseMove.mButtonStateMask = eMouseButtonFlagLeftBit;
					break;
				}
				case eOSXEventIDGenericRightMouseDragged:
				{
					auto & eInputMouseMove = osxProcessMouseMoveEvent( pEventController, mouseCursorState, pOutEvent );
					eInputMouseMove.mButtonStateMask = eMouseButtonFlagRightBit;
					break;
				}
				case eOSXEventIDGenericMouseEntered:
				case eOSXEventIDGenericMouseExited:
				{
					break;
				}
				case eOSXEventIDGenericScrollWheel:
				{
					auto & eInputMouseScroll = osxProcessMouseScrollEvent( pEventController, mouseCursorState, pOutEvent );
					{
						const auto scrollDeltaX = [pNativeEvent.mNSEvent scrollingDeltaX];
						const auto scrollDeltaY = [pNativeEvent.mNSEvent scrollingDeltaY];

						eInputMouseScroll.mScrollDelta.x = -scrollDeltaX;
						eInputMouseScroll.mScrollDelta.y = scrollDeltaY;
						eInputMouseScroll.mScrollDirection = EMouseScrollDirection::Normal;

						const auto scrollEventMask = osxQueryMouseScrollEventFlags( pNativeEvent );

						if( scrollEventMask.isSet( eOSXMouseScrollEventFlagScrollingModeNormalBit ) )
						{
							eInputMouseScroll.mScrollDelta.x *= 0.1;
							eInputMouseScroll.mScrollDelta.y *= 0.1;
						}

						if( scrollEventMask.isSet( eOSXMouseScrollEventFlagScrollingDirectionInvertedBit ) )
						{
							eInputMouseScroll.mScrollDirection = EMouseScrollDirection::Inverted;
						}
					}
					break;
				}
				case eOSXEventIDGenericTabletPoint:
				case eOSXEventIDGenericTabletProximity:
				{
					break;
				}
				case eOSXEventIDGenericOtherMouseDown:
				{
					auto & eInputMouseButton = osxProcessMouseButtonEvent( pEventController, mouseCursorState, pOutEvent );
					eInputMouseButton.mButtonAction = EMouseButtonActionType::Click;
					eInputMouseButton.mButtonID = osxQueryMouseButtonID( pNativeEvent.mNSEvent );
					break;
				}
				case eOSXEventIDGenericOtherMouseUp:
				{
					auto & eInputMouseButton = osxProcessMouseButtonEvent( pEventController, mouseCursorState, pOutEvent );
					eInputMouseButton.mButtonAction = EMouseButtonActionType::Release;
					eInputMouseButton.mButtonID = osxQueryMouseButtonID( pNativeEvent.mNSEvent );
					break;
				}
				case eOSXEventIDGenericOtherMouseDragged:
				{
					auto & eInputMouseMove = osxProcessMouseMoveEvent( pEventController, mouseCursorState, pOutEvent );
					const auto buttonID = osxQueryMouseButtonID( pNativeEvent.mNSEvent );
					const auto buttonFlag = CxDef::getMouseButtonFlagFromButtonID( buttonID );
					eInputMouseMove.mButtonStateMask = buttonFlag;
					break;
				}
				default:
				{
					ic3DebugInterrupt();
				}
			}
		}

		void _osxTranslateEventInputOther( EventController & pEventController, EventSource & pEventSource, const NativeEventType & pNativeEvent, EventObject & pOutEvent )
		{
			switch( pNativeEvent.mNSAppEventID )
			{
				case eOSXEventIDGenericGesture:
				case eOSXEventIDGenericMagnify:
				case eOSXEventIDGenericSwipe:
				case eOSXEventIDGenericSmartMagnify:
				case eOSXEventIDGenericQuickLook:
				case eOSXEventIDGenericPressure:
				{
					break;
				}
				default:
				{
					ic3DebugInterrupt();
				}
			}
		}

		void _osxTranslateEventWindow( EventController & pEventController, EventSource & pEventSource, const NativeEventType & pNativeEvent, EventObject & pOutEvent )
		{
			switch( pNativeEvent.mNSAppEventID )
			{
				case eOSXEventIDWindowDidExpose:
				case eOSXEventIDWindowDidMove:
				case eOSXEventIDWindowDidResize:
				case eOSXEventIDWindowMiniaturize:
				case eOSXEventIDWindowDidDeminiaturize:
				case eOSXEventIDWindowDidBecomeKey:
				case eOSXEventIDWindowDidResignKey:
				case eOSXEventIDWindowDidChangeBackingProperties:
				case eOSXEventIDWindowDidChangeScreenProfile:
				case eOSXEventIDWindowWillEnterFullScreen:
				case eOSXEventIDWindowDidEnterFullScreen:
				case eOSXEventIDWindowWillExitFullScreen:
				case eOSXEventIDWindowDidExitFullScreen:
				{
					break;
				}
				case eOSXEventIDWindowWillClose:
				{
					auto & eWindowUpdateDestroy = pOutEvent.uEvtWindowUpdateDestroy;
					eWindowUpdateDestroy.mEventCode = eEventCodeWindowUpdateDestroy;
					eWindowUpdateDestroy.mEventSource = &pEventSource;
					break;
				}
				default:
				{
					ic3DebugInterrupt();
				}
			}
		}

		// KeyCodes V1 mapping reference (Apple Script):
		// https://eastmanreference.com/complete-list-of-applescript-key-codes

		static const EKeyCode sAppleKeyCodeMap_V1_00_7F[] =
		{
			/* 0x00  0 */ EKeyCode::CharA,
			/* 0x01  1 */ EKeyCode::CharS,
			/* 0x02  2 */ EKeyCode::CharD,
			/* 0x03  3 */ EKeyCode::CharF,
			/* 0x04  4 */ EKeyCode::CharH,
			/* 0x05  5 */ EKeyCode::CharG,
			/* 0x06  6 */ EKeyCode::CharZ,
			/* 0x07  7 */ EKeyCode::CharX,
			/* 0x08  8 */ EKeyCode::CharC,
			/* 0x09  9 */ EKeyCode::CharV,
			/* 0x0A 10 */ EKeyCode::Unknown,
			/* 0x0B 11 */ EKeyCode::CharB,
			/* 0x0C 12 */ EKeyCode::CharQ,
			/* 0x0D 13 */ EKeyCode::CharW,
			/* 0x0E 14 */ EKeyCode::CharE,
			/* 0x0F 15 */ EKeyCode::CharR,
			/* 0x10 16 */ EKeyCode::CharY,
			/* 0x11 17 */ EKeyCode::CharT,
			/* 0x12 18 */ EKeyCode::Num1,
			/* 0x13 19 */ EKeyCode::Num2,
			/* 0x14 20 */ EKeyCode::Num3,
			/* 0x15 21 */ EKeyCode::Num4,
			/* 0x16 22 */ EKeyCode::Num6,
			/* 0x17 23 */ EKeyCode::Num5,
			/* 0x18 24 */ EKeyCode::Unknown,
			/* 0x19 25 */ EKeyCode::Num9,
			/* 0x1A 26 */ EKeyCode::Num7,
			/* 0x1B 27 */ EKeyCode::Unknown,
			/* 0x1C 28 */ EKeyCode::Num8,
			/* 0x1D 29 */ EKeyCode::Num0,
			/* 0x1E 30 */ EKeyCode::Unknown,
			/* 0x1F 31 */ EKeyCode::CharO,
			/* 0x20 32 */ EKeyCode::CharU,
			/* 0x21 33 */ EKeyCode::Unknown,
			/* 0x22 34 */ EKeyCode::CharI,
			/* 0x23 35 */ EKeyCode::CharP,
			/* 0x24 36 */ EKeyCode::Enter,
			/* 0x25 37 */ EKeyCode::CharL,
			/* 0x26 38 */ EKeyCode::CharJ,
			/* 0x27 39 */ EKeyCode::Unknown,
			/* 0x28 40 */ EKeyCode::CharK,
			/* 0x29 41 */ EKeyCode::Unknown,
			/* 0x2A 42 */ EKeyCode::Unknown,
			/* 0x2B 43 */ EKeyCode::Unknown,
			/* 0x2C 44 */ EKeyCode::Unknown,
			/* 0x2D 45 */ EKeyCode::CharN,
			/* 0x2E 46 */ EKeyCode::CharM,
			/* 0x2F 47 */ EKeyCode::Unknown,
			/* 0x30 48 */ EKeyCode::Tab,
			/* 0x31 49 */ EKeyCode::Space,
			/* 0x32 50 */ EKeyCode::Unknown,
			/* 0x33 51 */ EKeyCode::Unknown,
			/* 0x34 52 */ EKeyCode::Unknown,
			/* 0x35 53 */ EKeyCode::Escape,
			/* 0x36 54 */ EKeyCode::Unknown,
			/* 0x37 55 */ EKeyCode::Unknown,
			/* 0x38 56 */ EKeyCode::Unknown,
			/* 0x39 57 */ EKeyCode::Unknown,
			/* 0x3A 58 */ EKeyCode::Unknown,
			/* 0x3B 59 */ EKeyCode::Unknown,
			/* 0x3C 60 */ EKeyCode::Unknown,
			/* 0x3D 61 */ EKeyCode::Unknown,
			/* 0x3E 62 */ EKeyCode::Unknown,
			/* 0x3F 63 */ EKeyCode::Unknown,
			/* 0x40 64 */ EKeyCode::Unknown,
			/* 0x41 65 */ EKeyCode::Unknown,
			/* 0x42 66 */ EKeyCode::Unknown,
			/* 0x43 67 */ EKeyCode::Unknown,
			/* 0x44 68 */ EKeyCode::Unknown,
			/* 0x45 69 */ EKeyCode::Unknown,
			/* 0x46 70 */ EKeyCode::Unknown,
			/* 0x47 71 */ EKeyCode::Unknown,
			/* 0x48 72 */ EKeyCode::Unknown,
			/* 0x49 73 */ EKeyCode::Unknown,
			/* 0x4A 74 */ EKeyCode::Unknown,
			/* 0x4B 75 */ EKeyCode::Unknown,
			/* 0x4C 76 */ EKeyCode::Unknown,
			/* 0x4D 77 */ EKeyCode::Unknown,
			/* 0x4E 78 */ EKeyCode::Unknown,
			/* 0x4F 79 */ EKeyCode::Unknown,
			/* 0x50 80 */ EKeyCode::Unknown,
			/* 0x51 81 */ EKeyCode::Unknown,
			/* 0x52 82 */ EKeyCode::Unknown,
			/* 0x53 83 */ EKeyCode::Unknown,
			/* 0x54 84 */ EKeyCode::Unknown,
			/* 0x55 85 */ EKeyCode::Unknown,
			/* 0x56 86 */ EKeyCode::Unknown,
			/* 0x57 87 */ EKeyCode::Unknown,
			/* 0x58 88 */ EKeyCode::Unknown,
			/* 0x59 89 */ EKeyCode::Unknown,
			/* 0x5A 90 */ EKeyCode::Unknown,
			/* 0x5B 91 */ EKeyCode::Unknown,
			/* 0x5C 92 */ EKeyCode::Unknown,
			/* 0x5D 93 */ EKeyCode::Unknown,
			/* 0x5E 94 */ EKeyCode::Unknown,
			/* 0x5F 95 */ EKeyCode::Unknown,
			/* 0x60 96 */ EKeyCode::F5,
			/* 0x61 97 */ EKeyCode::F6,
			/* 0x62 98 */ EKeyCode::F7,
			/* 0x63 99 */ EKeyCode::F3,
			/* 0x64 100 */ EKeyCode::F8,
			/* 0x65 101 */ EKeyCode::F9,
			/* 0x66 102 */ EKeyCode::Unknown,
			/* 0x67 103 */ EKeyCode::F11,
			/* 0x68 104 */ EKeyCode::Unknown,
			/* 0x69 105 */ EKeyCode::Unknown,
			/* 0x6A 106 */ EKeyCode::Unknown,
			/* 0x6B 107 */ EKeyCode::Unknown,
			/* 0x6C 108 */ EKeyCode::Unknown,
			/* 0x6D 109 */ EKeyCode::F10,
			/* 0x6E 110 */ EKeyCode::Unknown,
			/* 0x6F 111 */ EKeyCode::F12,
			/* 0x70 112 */ EKeyCode::Unknown,
			/* 0x71 113 */ EKeyCode::Unknown,
			/* 0x72 114 */ EKeyCode::Insert,
			/* 0x73 115 */ EKeyCode::Home,
			/* 0x74 116 */ EKeyCode::PageUp,
			/* 0x75 117 */ EKeyCode::Delete,
			/* 0x76 118 */ EKeyCode::F4,
			/* 0x77 119 */ EKeyCode::End,
			/* 0x78 120 */ EKeyCode::F2,
			/* 0x79 121 */ EKeyCode::PageDown,
			/* 0x7A 122 */ EKeyCode::F1,
			/* 0x7B 123 */ EKeyCode::ArrowLeft,
			/* 0x7C 124 */ EKeyCode::ArrowRight,
			/* 0x7D 125 */ EKeyCode::ArrowDown,
			/* 0x7E 126 */ EKeyCode::ArrowUp,
			/* 0x7F 127 */ EKeyCode::Unknown,
		};

		EKeyCode _osxGetSysKeyCodeV1( unsigned short pNSKeyCode )
		{
			if( ( pNSKeyCode >= 0x00 ) && ( pNSKeyCode <= 0x7F ) )
			{
				const auto baseIndex = pNSKeyCode;
				return sAppleKeyCodeMap_V1_00_7F[baseIndex];
			}

			return EKeyCode::Unknown;
		}

		// KeyCodes V2 mapping reference (USB-compliant):
		// https://www.usb.org/sites/default/files/documents/hut1_12v2.pdf
		// https://developer.apple.com/library/archive/technotes/tn2450/_index.html

		static const EKeyCode sAppleKeyCodeMap_V2_04_73[] =
		{
			/* 0x04 */ EKeyCode::CharA,
			/* 0x05 */ EKeyCode::CharB,
			/* 0x06 */ EKeyCode::CharC,
			/* 0x07 */ EKeyCode::CharD,
			/* 0x08 */ EKeyCode::CharE,
			/* 0x09 */ EKeyCode::CharF,
			/* 0x0A */ EKeyCode::CharG,
			/* 0x0B */ EKeyCode::CharH,
			/* 0x0C */ EKeyCode::CharI,
			/* 0x0D */ EKeyCode::CharJ,
			/* 0x0E */ EKeyCode::CharK,
			/* 0x0F */ EKeyCode::CharL,
			/* 0x10 */ EKeyCode::CharM,
			/* 0x11 */ EKeyCode::CharN,
			/* 0x12 */ EKeyCode::CharO,
			/* 0x13 */ EKeyCode::CharP,
			/* 0x14 */ EKeyCode::CharQ,
			/* 0x15 */ EKeyCode::CharR,
			/* 0x16 */ EKeyCode::CharS,
			/* 0x17 */ EKeyCode::CharT,
			/* 0x18 */ EKeyCode::CharU,
			/* 0x19 */ EKeyCode::CharV,
			/* 0x1A */ EKeyCode::CharW,
			/* 0x1B */ EKeyCode::CharX,
			/* 0x1C */ EKeyCode::CharY,
			/* 0x1D */ EKeyCode::CharZ,
            /* 0x1E */ EKeyCode::Num0,
			/* 0x1F */ EKeyCode::Num1,
			/* 0x20 */ EKeyCode::Num2,
			/* 0x21 */ EKeyCode::Num3,
			/* 0x22 */ EKeyCode::Num4,
			/* 0x23 */ EKeyCode::Num5,
			/* 0x24 */ EKeyCode::Num6,
			/* 0x25 */ EKeyCode::Num7,
			/* 0x26 */ EKeyCode::Num8,
			/* 0x27 */ EKeyCode::Num9,
			/* 0x28 */ EKeyCode::Enter,
			/* 0x29 */ EKeyCode::Escape,
			/* 0x2A */ EKeyCode::Backspace,
			/* 0x2B */ EKeyCode::Tab,
			/* 0x2C */ EKeyCode::Space,
			/* 0x2D */ /* - and _        */ EKeyCode::Unknown,
			/* 0x2E */ /* = and +        */ EKeyCode::Unknown,
			/* 0x2F */ /* [ and {        */ EKeyCode::Unknown,
			/* 0x30 */ /* ] and }        */ EKeyCode::Unknown,
			/* 0x31 */ /* \ and |        */ EKeyCode::Unknown,
			/* 0x32 */ /* Non-US # and ~ */ EKeyCode::Unknown,
			/* 0x33 */ /* ; and :        */ EKeyCode::Unknown,
			/* 0x34 */ /* ' and "        */ EKeyCode::Unknown,
			/* 0x35 */ /* Tilde          */ EKeyCode::Unknown,
			/* 0x36 */ /* , and "<"      */ EKeyCode::Unknown,
			/* 0x37 */ /* . and ">"      */ EKeyCode::Unknown,
			/* 0x38 */ /* / and ?        */ EKeyCode::Unknown,
			/* 0x39 */ EKeyCode::CapsLock,
			/* 0x3A */ EKeyCode::F1,
			/* 0x3B */ EKeyCode::F2,
			/* 0x3C */ EKeyCode::F3,
			/* 0x3D */ EKeyCode::F4,
			/* 0x3E */ EKeyCode::F5,
			/* 0x3F */ EKeyCode::F6,
			/* 0x40 */ EKeyCode::F7,
			/* 0x41 */ EKeyCode::F8,
			/* 0x42 */ EKeyCode::F9,
			/* 0x43 */ EKeyCode::F10,
			/* 0x44 */ EKeyCode::F11,
			/* 0x45 */ EKeyCode::F12,
			/* 0x46 */ /* Print Screen   */ EKeyCode::Unknown,
			/* 0x47 */ /* Scroll Lock    */ EKeyCode::Unknown,
			/* 0x48 */ /* Pause          */ EKeyCode::Unknown,
			/* 0x49 */ EKeyCode::Insert,
			/* 0x4A */ EKeyCode::Home,
			/* 0x4B */ EKeyCode::PageUp,
			/* 0x4C */ EKeyCode::Delete,
			/* 0x4D */ EKeyCode::End,
			/* 0x4E */ EKeyCode::PageDown,
			/* 0x4F */ EKeyCode::ArrowRight,
			/* 0x50 */ EKeyCode::ArrowLeft,
			/* 0x51 */ EKeyCode::ArrowDown,
			/* 0x52 */ EKeyCode::ArrowUp,
			/* 0x53 */ /* Keypad Num Lock */ EKeyCode::Unknown,
			/* 0x54 */ EKeyCode::NumPadDiv,
			/* 0x55 */ EKeyCode::NumPadMul,
			/* 0x56 */ EKeyCode::NumPadSub,
			/* 0x57 */ EKeyCode::NumPadAdd,
			/* 0x58 */ EKeyCode::Enter,
			/* 0x59 */ EKeyCode::NumPad0,
			/* 0x5A */ EKeyCode::NumPad1,
			/* 0x5B */ EKeyCode::NumPad2,
			/* 0x5C */ EKeyCode::NumPad3,
			/* 0x5D */ EKeyCode::NumPad4,
			/* 0x5E */ EKeyCode::NumPad5,
			/* 0x5F */ EKeyCode::NumPad6,
			/* 0x60 */ EKeyCode::NumPad7,
			/* 0x61 */ EKeyCode::NumPad8,
			/* 0x62 */ EKeyCode::NumPad9,
			/* 0x63 */ EKeyCode::NumPadDot,
			/* 0x64 */ /* \ and | */ EKeyCode::Unknown,
		};

		static const EKeyCode sAppleKeyCodeMap_V2_E0_E7[] =
		{
			/* 0xE0 */ EKeyCode::CtrlLeft,
			/* 0xE1 */ EKeyCode::ShiftLeft,
			/* 0xE2 */ EKeyCode::AltLeft,
			/* 0xE3 */ /* Keyboard Left GUI */ EKeyCode::Unknown,
			/* 0xE4 */ EKeyCode::CtrlRight,
			/* 0xE5 */ EKeyCode::ShiftRight,
			/* 0xE6 */ EKeyCode::AltRight,
			/* 0xE7 */ /* Keyboard Right GUI */ EKeyCode::Unknown,
		};

		EKeyCode _osxGetSysKeyCodeV2( unsigned short pNSKeyCode )
		{
			if( ( pNSKeyCode >= 0x04 ) && ( pNSKeyCode <= 0x73 ) )
			{
				const auto baseIndex = pNSKeyCode - 0x04;
				return sAppleKeyCodeMap_V2_04_73[baseIndex];
			}

			if( ( pNSKeyCode >= 0xE0 ) && ( pNSKeyCode <= 0xE7 ) )
			{
				const auto baseIndex = pNSKeyCode - 0xE0;
				return sAppleKeyCodeMap_V2_E0_E7[baseIndex];
			}

			return EKeyCode::Unknown;
		}

	}
	
}
