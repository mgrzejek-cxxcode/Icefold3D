
#ifndef __IC3_SYSTEM_PLATFORM_OSAPI_ANDROID_EVENT_CORE_H__
#define __IC3_SYSTEM_PLATFORM_OSAPI_ANDROID_EVENT_CORE_H__

#include "AndroidCommon.h"
#include <Ic3/System/EventCore.h>
#include <Ic3/System/EventObject.h>

namespace Ic3::System
{

	namespace Platform
	{

		enum class AndroidNativeEventType : enum_default_value_t
		{
			AppCommand,
			Input,
			Unknown
		};

		enum : int32_t
		{
			E_ANDROID_EVT_CMD_USER_DESTROY_REQUESTED = 0xFF00,
		};

		inline constexpr int32_t CX_ANDROID_EVENT_LOOPER_WAIT_TIMEOUT_IMMEDIATE = 0;

		inline constexpr int32_t CX_ANDROID_EVENT_LOOPER_WAIT_TIMEOUT_INFINITY = -1;

		struct NativeEventType
		{
			AndroidNativeEventType type;
			union
			{
				int32 eAppCommand;
				AInputEvent * eInputEvent;
			};
		};

		using AndroidNativeEvent = NativeEventType;

	}

	class AndroidEventController : public AndroidNativeObject<EventController, Platform::AndroidNativeDataCommon>
	{
	public:
		AndroidEventController( SysContextHandle pSysContext );
		virtual ~AndroidEventController() noexcept;

		using EventController::getEventSystemInternalConfig;
		using EventController::getEventDispatcherInputState;

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

#endif // __IC3_SYSTEM_PLATFORM_OSAPI_ANDROID_EVENT_CORE_H__
