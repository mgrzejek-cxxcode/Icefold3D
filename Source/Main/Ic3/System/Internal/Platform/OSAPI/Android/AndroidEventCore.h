
#ifndef __IC3_SYSTEM_PLATFORM_OSAPI_ANDROID_EVENT_CORE_H__
#define __IC3_SYSTEM_PLATFORM_OSAPI_ANDROID_EVENT_CORE_H__

#include "AndroidCommon.h"
#include <Ic3/System/Events/EventCore.h>
#include <Ic3/System/Events/EventObject.h>

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
			eAndroidEvtCmdUserDestroyRequested = 0xFF00,
		};

		inline constexpr int32_t cxAndroidEventLooperWaitTimeoutImmediate = 0;

		inline constexpr int32_t cxAndroidEventLooperWaitTimeoutInfinity = -1;

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

		using EventController::GetEventSystemInternalConfig;
		using EventController::GetEventDispatcherInputState;

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

#endif // __IC3_SYSTEM_PLATFORM_OSAPI_ANDROID_EVENT_CORE_H__
