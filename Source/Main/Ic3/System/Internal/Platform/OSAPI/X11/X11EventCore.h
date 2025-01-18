
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

			cppx::bitmask<uint32> mSysWindowFlags = 0;
		};

		struct NativeEventType
		{
			XEvent mXEvent;
		};

		EventSource * X11FindEventSourceByXWindow(X11EventController & pEventController, XWindow pWindowXID );

		bool X11TranslateEvent( EventSource & pEventSource, const XEvent & pXEvent, EventObject & pOutEvent );

	}

	class X11EventController : public X11NativeObject<EventController, Platform::X11NativeDataCommon>
	{
	public:
		X11EventController( SysContextHandle pSysContext );
		virtual ~X11EventController() noexcept;

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

#endif // __IC3_SYSTEM_PLATFORM_OSAPI_X11_EVENT_CORE_H__
