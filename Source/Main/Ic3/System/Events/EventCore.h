
#ifndef __IC3_SYSTEM_EVENT_CORE_H__
#define __IC3_SYSTEM_EVENT_CORE_H__

#include "EventSource.h"
#include <functional>

namespace Ic3::System
{

	Ic3SysDeclareHandle( EventController );
	Ic3SysDeclareHandle( EventDispatcher );

	struct EventSystemConfig;
	struct EventSystemSharedState;
	struct EvtInputKeyboard;
	struct EvtInputMouse;

	using EventSourceFindPredicate = std::function<bool( const EventSource & )>;
	using EventSourceNativeDataFindPredicate = std::function<bool( const void * )>;

	/// @brief A set of flags which modify the behaviour of the event system. Used in EventController::SetEventSystemConfigFlags().
	enum EEventSystemConfigFlags : uint32
	{
		//
		eEventSystemConfigFlagIdleProcessingModeBit = 0x1,

		// If set, an AppQuit event will be posted after the last event source is destroyed.
		eEventSystemConfigFlagEnableAutoQuitOnLastSourceDestroyBit = 0x10,

		// If set, an AppQuit event will be posted after the primary event source is destroyed.
		// See EventController::RegisterPrimaryEventSource() and EventController::SetPrimaryEventSource().
		eEventSystemConfigFlagEnableAutoQuitOnPrimarySourceDestroyBit = 0x20,

		//
		eEventSystemConfigMaskEnableAutoQuit = 0x30,

		//
		eEventSystemConfigFlagEnableMouseDoubleClickBit = 0x40,

		//
		eEventSystemConfigFlagEnableMouseMultiClickBit = 0x80 | eEventSystemConfigFlagEnableMouseDoubleClickBit,

		//
		eEventSystemConfigMaskEnableMouseClickProcessing = 0xC0,

		// The flags below control what approach is taken to handle automatic app quit
		// in case one of the situations above happens (last source/primary source is gone).

		// When an auto-quit is triggerred, the even is immediately dispatched to the target dispatcher and executed.
		// Since the whole logic happens during event pre-processing, this flag essentially causes the event source's
		// "Close" event to be replaced with the "AppQuit". This is a very aggressive setup and should be used carefully.
		eEventSystemConfigFlagSetAutoQuitModeDirectEventDispatchBit = 0x0100,

		// When an auto-quit is triggerred, the AppQuit event is pushed to the front of the priority event queue.
		// That means it will be the next event dispatched. The event source's "Close" event is delivered as usual.
		eEventSystemConfigFlagSetAutoQuitModePostPriorityEventBit = 0x0200,

		// When an auto-quit is triggerred, the AppQuit event is pushed to the back of the priority event queue.
		// That means it will MOST LIKELY be the next event dispatched, UNLESS the user code heavily uses the priority
		// queue and there are other events already in it. The event source's "Close" event is delivered as usual.
		eEventSystemConfigFlagSetAutoQuitModePostUserEventBit = 0x0400,

		// When an auto-quit is triggerred, the AppQuit event is not triggerred immediately. Instead, an internal flag
		// is set instead. The controller keeps fetching the priority events and peeking the system queue, but no new
		// events are allowed. When all pending events are processed, system automatically generates the AppQuit event.
		eEventSystemConfigFlagSetAutoQuitModeSetInternalRequestOnlyBit = 0x0800,

		//
		eEventSystemConfigMaskSetAutoQuitMode = 0x0F00,

		eEventSystemConfigMaskAll = 0xFFFF
	};

	/// @brief
	class IC3_SYSTEM_CLASS EventController : public SysObject
	{
		friend class EventDispatcher;
		friend class EventSource;

	public:
		explicit EventController( SysContextHandle pSysContext );
		virtual ~EventController() noexcept;

		CPPX_ATTR_NO_DISCARD EventDispatcherHandle CreateEventDispatcher();

		bool SetActiveEventDispatcher( EventDispatcher & pEventDispatcher );

		bool ResetActiveEventDispatcher();

		void RegisterEventSource( EventSource & pEventSource );

		bool RegisterPrimaryEventSource( EventSource & pEventSource );

		void UnregisterEventSource( EventSource & pEventSource );

		void SetPrimaryEventSource( EventSource * pEventSource );

		bool DispatchEvent( EventObject pEvent );

		void PushUserEvent( EventObject pEvent );

		EventObject & EmplaceUserEvent();

		void PushPriorityEvent( EventObject pEvent );

		EventObject & EmplacePriorityEvent();

		// Note on event fetching order:
		// Event controller works with three different event queues:
		// 1) Local priority queue:
		//  The name is rather self-descriptive. This queue can be populated using dedicated PushPriorityEvent() and
		//  EmplacePriorityEvent() functions. It has the highest priority and is always checked first. This queue
		//  is also used for additional system-level events.
		// 2) Local user queue:
		//  This is simply a lower-prioritised version of the first queue. It is checked as second
		//  and can be populated  using dedicated PushUserEvent() and EmplaceUserEvent() functions.
		// 3) System queue:
		//  "THE" event queue, coming directly from the system. Checked as last, after priority and user queues.

		/// @brief Retrieves pending events from the queue using waiting policy based on the current configuration.
		/// #
		/// This function retrieves pending events from queues and dispatches them. If no events are currently
		/// available, the function behaves as follows:
		/// 1) If E_EVENT_SYSTEM_CONFIG_FLAG_IDLE_PROCESSING_MODE_BIT is not set, this function returns immediately.
		/// 2) Otherwise, it blocks until a new events arrives in the system queue (just like DispatchPendingEventsWait()
		///  would normally do).
		uint32 DispatchPendingEventsAuto();

		uint32 DispatchPendingEventsPeek( uint32 pLimit = cppx::cve::int32_max );

		uint32 DispatchPendingEventsWait( uint32 pLimit = cppx::cve::int32_max );

		uint32 DispatchPendingEventsWaitTimeout( const cppx::microseconds & pTimeout, uint32 pLimit = cppx::cve::int32_max );

		void ValidateActiveDispatcherState() const;

		CPPX_ATTR_NO_DISCARD EventSystemSharedState & GetEventSystemSharedState() noexcept;

		CPPX_ATTR_NO_DISCARD bool CheckEventSystemConfigFlags( cppx::bitmask<EEventSystemConfigFlags> pFlags ) const;

		CPPX_ATTR_NO_DISCARD EventSource * FindEventSource( const EventSourceFindPredicate & pPredicate ) const;

		CPPX_ATTR_NO_DISCARD EventSource * FindEventSource( const EventSourceNativeDataFindPredicate & pPredicate ) const;

		CPPX_ATTR_NO_DISCARD bool IsActiveDispatcherSet() const noexcept;

		CPPX_ATTR_NO_DISCARD size_t GetRegisteredEventSourcesNum() const noexcept;

		CPPX_ATTR_NO_DISCARD EventSource * GetPrimaryEventSource() const noexcept;

		CPPX_ATTR_NO_DISCARD EventSource * GetRegisteredEventSourceByIndex( size_t pIndex ) const noexcept;

		CPPX_ATTR_NO_DISCARD bool IsEventSourceRegistered( const EventSource & pEventSource ) const noexcept;

	friendapi( protected ):
		// Used by the EventSource class. It is called inside its destructor.
		void OnEventSourceDestroy( EventSource & pEventSource ) noexcept;

		// Used by the EventDispatcher class. It is called inside its destructor.
		void OnEventDispatcherDestroy( EventDispatcher & pEventDispatcher ) noexcept;

	private:
		// System-level call. Pulls events from the system queue.
		// If the queue is empty, returns immediately.
		virtual bool _NativeDispatchPendingEvents() = 0;

		// System-level call. Pulls events from the system queue.
		// If the queue is empty, waits until an event arrives (infinite timeout).
		virtual bool _NativeDispatchPendingEventsWait() = 0;

		// System-level call. Pulls events from the system queue.
		// If the queue is empty, waits until an event arrives or the specified timeout occurs.
		// A separate function because on some OSes timeout-based event fetching may be tricky (yes, you again, Win32).
		virtual bool _NativeDispatchPendingEventsWaitTimeout( const cppx::microseconds & pTimeout ) { return false; }

		// Registers the event source at the system level. Empty for most OSes.
		// Win32: replaces WNDPROC with a custom one and allocates extra window data.
		virtual void _NativeRegisterEventSource( EventSource & pEventSource );

		// Unregisters the event source at the system level. Empty for most OSes.
		// Win32: restores the original (default) WNDPROC and releases the allocated data.
		virtual void _NativeUnRegisterEventSource( EventSource & pEventSource );

		// Private utility function. Handles internal details of replacing active dispatcher with another one (or NULL).
		void _OnActiveDispatcherChange( EventDispatcher * pEventDispatcher );

		// Private utility function. Pulls events from the priority queue, which comes before the system one.
		bool _ProcessLocalQueues();

		// Private utility function. Sends an "AppQuit" event as a reaction to removal of the last/priority event source.
		// See SetEventSystemConfigFlags() and EEventSystemConfigFlags::E_EVENT_SYSTEM_CONFIG_FLAG_ENABLE_AUTO_QUIT_ON_xxx.
		bool _CheckAndPostAppAutoQuitEvent( EEventCode pEvent, EventSource & pEventSource );

		//
		void _SetInternalStateFlags( cppx::bitmask<uint32> pFlags, bool pSetOrUnset );

		//
		CPPX_ATTR_NO_DISCARD uint32 _CheckInternalStateFlags( cppx::bitmask<uint32> pFlags ) const;

		//
		CPPX_ATTR_NO_DISCARD cppx::bitmask<uint32> _GetInternalStateFlags() const;

	protected:
		struct EventControllerPrivateData;
		std::unique_ptr<EventControllerPrivateData> _privateData;
	};

	/// @brief
	class IC3_SYSTEM_CLASS EventDispatcher : public SysObject
	{
		friend class EventController;

	public:
		explicit EventDispatcher( EventControllerHandle pEventController );
		virtual ~EventDispatcher() noexcept;

		void SetActive();

		void SetEventHandler( EEventBaseType pBaseType, EventHandler pHandler );
		void SetEventHandler( EEventCategory pCategory, EventHandler pHandler );
		void SetEventHandler( EEventCodeIndex pCodeIndex, EventHandler pHandler );
		void SetDefaultEventHandler( EventHandler pHandler );

		void ResetEventHandler( EEventBaseType pBaseType );
		void ResetEventHandler( EEventCategory pCategory );
		void ResetEventHandler( EEventCodeIndex pCodeIndex );
		void ResetDefaultEventHandler();

		void SetEventSystemConfigFlags( cppx::bitmask<EEventSystemConfigFlags> pFlags, bool pSetOrUnset = true );

		void SetIdleProcessingMode( bool pIdle );

		bool PostEvent( EventObject pEvent );
		bool PostEvent( event_code_value_t pEventCode );
		bool PostEventAppQuit();
		bool PostEventAppTerminate();

		CPPX_ATTR_NO_DISCARD bool CheckEventSystemConfigFlags( cppx::bitmask<EEventSystemConfigFlags> pFlags ) const;

		CPPX_ATTR_NO_DISCARD cppx::bitmask<EEventSystemConfigFlags> GetEventSystemConfigFlags() const;

	friendapi( private ):
		const EventSystemConfig & GetEventSystemConfig() const;

	private:
		bool _PreProcessEvent( EventObject & pEvent );
		bool _PreProcessEventKeyboard( EvtInputKeyboard & pKeyboardEvent, EventSystemSharedState & pSharedState );
		bool _PreProcessEventMouse( EvtInputMouse & pMouseEvent, EventSystemSharedState & pSharedState );

	protected:
		struct EventDispatcherPrivateData;
		EventControllerHandle _eventControllerActiveRef;
		std::unique_ptr<EventDispatcherPrivateData> _privateData;
	};

} // namespace Ic3::System

#endif // __IC3_SYSTEM_EVENT_CORE_H__
