
#ifndef __IC3_SYSTEM_PLATFORM_OSAPI_X11_EVENT_CORE_H__
#define __IC3_SYSTEM_PLATFORM_OSAPI_X11_EVENT_CORE_H__

#include "X11Common.h"
#include <Ic3/System/EventCore.h>
#include <Ic3/System/EventObject.h>

namespace Ic3::System
{

	class X11EventController;

	namespace Platform
	{

		enum class EX11MouseButtonID : enum_default_value_t
		{
			Unknown,
			Left,
			Middle,
			Right,
			VWheelUp,
			VWheelDown,
			HWheelLeft,
			HWheelRight,
			XBT1,
			XBT2
		};

		enum EX11SystemWindowFlags : uint32
		{
			eX11SystemWindowFlagWMStateFullscreen = 0x8000
		};

		struct X11EventSourceNativeData : public X11NativeDataCommon
		{
			XWindow mWindowXID = eXIDNone;

			TBitmask<uint32> mSysWindowFlags = 0;
		};

		struct NativeEventType
		{
			XEvent mXEvent;
		};

		IC3_SYSTEM_API_NODISCARD EventSource * x11FindEventSourceByXWindow( X11EventController & pEventController, XWindow pWindowXID );

		bool x11TranslateEvent( EventSource & pEventSource, const XEvent & pXEvent, EventObject & pOutEvent );

	}

	class X11EventController : public X11NativeObject<EventController, Platform::X11NativeDataCommon>
	{
	public:
		X11EventController( SysContextHandle pSysContext );
		virtual ~X11EventController() noexcept;

	private:
		/// @copybrief EventController::_nativeRegisterEventSource
		virtual void _nativeRegisterEventSource( EventSource & pEventSource ) override final;

		/// @copybrief EventController::_nativeUnregisterEventSource
		virtual void _nativeUnregisterEventSource( EventSource & pEventSource ) override final;

		/// @copybrief EventController::_nativeDispatchPendingEvents
		virtual bool _nativeDispatchPendingEvents() override final;

		/// @copybrief EventController::_nativeDispatchPendingEventsWait
		virtual bool _nativeDispatchPendingEventsWait() override final;
	};

} // namespace Ic3::System

#endif // __IC3_SYSTEM_PLATFORM_OSAPI_X11_EVENT_CORE_H__
