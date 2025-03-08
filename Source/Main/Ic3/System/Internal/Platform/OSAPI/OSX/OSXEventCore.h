
#ifndef __IC3_SYSTEM_PLATFORM_OSAPI_OSX_EVENT_CORE_H__
#define __IC3_SYSTEM_PLATFORM_OSAPI_OSX_EVENT_CORE_H__

#include "OSXCommon.h"
#include <Ic3/System/Events/EventCore.h>
#include <Ic3/System/Events/EventObject.h>
#include <cppx/bitUtils.h>

#import <AppKit/NSEvent.h>

namespace Ic3::System
{

	class OSXEventController;

	namespace Platform
	{

		enum EOSXEventID : NSUInteger
		{
			eOSXEventIDNull                      = 0,
			eOSXEventIDGenericLeftMouseDown      = NSEventTypeLeftMouseDown      ,
			eOSXEventIDGenericLeftMouseUp        = NSEventTypeLeftMouseUp        ,
			eOSXEventIDGenericRightMouseDown     = NSEventTypeRightMouseDown     ,
			eOSXEventIDGenericRightMouseUp       = NSEventTypeRightMouseUp       ,
			eOSXEventIDGenericMouseMoved         = NSEventTypeMouseMoved         ,
			eOSXEventIDGenericLeftMouseDragged   = NSEventTypeLeftMouseDragged   ,
			eOSXEventIDGenericRightMouseDragged  = NSEventTypeRightMouseDragged  ,
			eOSXEventIDGenericMouseEntered       = NSEventTypeMouseEntered       ,
			eOSXEventIDGenericMouseExited        = NSEventTypeMouseExited        ,
			eOSXEventIDGenericKeyDown            = NSEventTypeKeyDown            ,
			eOSXEventIDGenericKeyUp              = NSEventTypeKeyUp              ,
			eOSXEventIDGenericFlagsChanged       = NSEventTypeFlagsChanged       ,
			eOSXEventIDGenericAppKitDefined      = NSEventTypeAppKitDefined      ,
			eOSXEventIDGenericSystemDefined      = NSEventTypeSystemDefined      ,
			eOSXEventIDGenericApplicationDefined = NSEventTypeApplicationDefined ,
			eOSXEventIDGenericPeriodic           = NSEventTypePeriodic           ,
			eOSXEventIDGenericCursorUpdate       = NSEventTypeCursorUpdate       ,
			eOSXEventIDGenericRotate             = NSEventTypeRotate             ,
			eOSXEventIDGenericBeginGesture       = NSEventTypeBeginGesture       ,
			eOSXEventIDGenericEndGesture         = NSEventTypeEndGesture         ,
			eOSXEventIDX21                       ,
			eOSXEventIDGenericScrollWheel        = NSEventTypeScrollWheel        ,
			eOSXEventIDGenericTabletPoint        = NSEventTypeTabletPoint        ,
			eOSXEventIDGenericTabletProximity    = NSEventTypeTabletProximity    ,
			eOSXEventIDGenericOtherMouseDown     = NSEventTypeOtherMouseDown     ,
			eOSXEventIDGenericOtherMouseUp       = NSEventTypeOtherMouseUp       ,
			eOSXEventIDGenericOtherMouseDragged  = NSEventTypeOtherMouseDragged  ,
			eOSXEventIDX28                       ,
			eOSXEventIDGenericGesture            = NSEventTypeGesture            ,
			eOSXEventIDGenericMagnify            = NSEventTypeMagnify            ,
			eOSXEventIDGenericSwipe              = NSEventTypeSwipe              ,
			eOSXEventIDGenericSmartMagnify       = NSEventTypeSmartMagnify       ,
			eOSXEventIDGenericQuickLook          = NSEventTypeQuickLook          ,
			eOSXEventIDGenericPressure           = NSEventTypePressure           ,
			eOSXEventIDX35                       ,
			eOSXEventIDX36                       ,
			eOSXEventIDGenericDirectTouch        = NSEventTypeDirectTouch        ,
			eOSXEventIDGenericChangeMode         = NSEventTypeChangeMode         ,
			eOSXEventIDWindowDidExpose                  ,
			eOSXEventIDWindowDidMove                    ,
			eOSXEventIDWindowDidResize                  ,
			eOSXEventIDWindowMiniaturize                ,
			eOSXEventIDWindowDidDeminiaturize           ,
			eOSXEventIDWindowDidBecomeKey               ,
			eOSXEventIDWindowDidResignKey               ,
			eOSXEventIDWindowDidChangeBackingProperties ,
			eOSXEventIDWindowDidChangeScreenProfile     ,
			eOSXEventIDWindowWillEnterFullScreen        ,
			eOSXEventIDWindowDidEnterFullScreen         ,
			eOSXEventIDWindowWillExitFullScreen         ,
			eOSXEventIDWindowDidExitFullScreen          ,
			eOSXEventIDWindowWillClose                  ,
		};

		static_assert( eOSXEventIDX21 == 21 );
		static_assert( eOSXEventIDX28 == 28 );
		static_assert( eOSXEventIDX35 == 35 );
		static_assert( eOSXEventIDX36 == 36 );

		enum EOSXMouseScrollEventFlags : uint32
		{
			eOSXMouseScrollEventFlagScrollingModeNormalBit = 0x01,
			eOSXMouseScrollEventFlagScrollingModePreciseBit = 0x02,
			eOSXMouseScrollEventFlagScrollingDirectionNormalBit = 0x10,
			eOSXMouseScrollEventFlagScrollingDirectionInvertedBit = 0x20,
		};

		struct OSXEventSourceNativeData : public OSXNativeDataCommon
		{
			NSWindow * mNSWindow = nil;
			NSView * mNSView = nil;
			id mNSEventListener = nil;
		};

		struct OSXEventControllerNativeData : public OSXNativeDataCommon
		{
			CGPoint lastCursorPosReal;
		};

		struct NativeEventType
		{
			EOSXEventID mNSAppEventID = eOSXEventIDNull;

			NSEvent * mNSEvent = nil;

			NSNotification * mNSNotification = nil;

			NSWindow * mNSSourceWindow = nil;

			explicit NativeEventType( NSEvent * pNSEvent )
			: mNSAppEventID( static_cast<EOSXEventID>( [pNSEvent type] ) )
			, mNSEvent( pNSEvent )
			, mNSSourceWindow( [pNSEvent window] )
			{}

			explicit NativeEventType( NSEvent * pNSEvent, NSEventType pOverrideEventID )
			: mNSAppEventID( static_cast<EOSXEventID>( pOverrideEventID ) )
			, mNSEvent( pNSEvent )
			, mNSSourceWindow( [pNSEvent window] )
			{}

			NativeEventType( EOSXEventID pAppEventID, NSNotification * pNSNotification )
			: mNSAppEventID( pAppEventID )
			, mNSNotification( pNSNotification )
			, mNSSourceWindow( static_cast<NSWindow *>( [pNSNotification object] ) )
			{}
		};

		EventSource * osxFindEventSourceByNSWindow( OSXEventController & pEventController, NSWindow * pNSWindow );

		void OSXCreateEventListener( OSXEventSourceNativeData & pEventSourceNativeData );

		bool osxTranslateEvent( EventController & pEventController, EventSource & pEventSource, const NativeEventType & pNativeEvent, EventObject & pOutEvent );

		IC3_SYSTEM_API_NODISCARD bool osxIsEventTypeInputKeyboard( EOSXEventID pEventID );

		IC3_SYSTEM_API_NODISCARD bool osxIsEventTypeInputMouse( EOSXEventID pEventID );

		IC3_SYSTEM_API_NODISCARD bool osxIsEventTypeInputOther( EOSXEventID pEventID );

		IC3_SYSTEM_API_NODISCARD bool osxIsEventTypeWindow( EOSXEventID pEventID );

		IC3_SYSTEM_API_NODISCARD EMouseButtonID OSXQueryMouseButtonID( NSEvent * pNSEvent );

		IC3_SYSTEM_API_NODISCARD Math::Vec2i32 OSXQueryMouseRelativePosition( NSEvent * pNSEvent );

		IC3_SYSTEM_API_NODISCARD cppx::bitmask<EOSXMouseScrollEventFlags> OSXQueryMouseScrollEventFlags( const NativeEventType & pNativeEvent );

	}

	class OSXEventController : public OSXNativeObject<EventController, Platform::OSXEventControllerNativeData>
	{
	public:
		OSXEventController( SysContextHandle pSysContext );
		virtual ~OSXEventController() noexcept;

	private:
		/// @copybrief EventController::_NativeRegisterEventSource
		virtual void _NativeRegisterEventSource( EventSource & pEventSource ) override final;

		/// @copybrief EventController::_NativeUnRegisterEventSource
		virtual void _NativeUnRegisterEventSource( EventSource & pEventSource ) override final;

		/// @copybrief EventController::_NativeDispatchPendingEvents
		virtual bool _NativeDispatchPendingEvents() override final;

		/// @copybrief EventController::_NativeDispatchPendingEventsWait
		virtual bool _NativeDispatchPendingEventsWait() override final;
	};

} // namespace Ic3::System

#endif // __IC3_SYSTEM_PLATFORM_OSAPI_OSX_EVENT_CORE_H__
