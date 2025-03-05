
#ifndef __IC3_SYSTEM_INTERNAL_EVENT_CORE_PRIVATE_H__
#define __IC3_SYSTEM_INTERNAL_EVENT_CORE_PRIVATE_H__

#include "../Events/EventCore.h"
#include "../Events/EventObject.h"

#include <deque>
#include <list>
#include <unordered_map>

namespace Ic3::System
{
	
	using LocalEventQueue = std::deque<EventObject>;

	enum EEventSystemInternalFlags : uint32
	{
		//
		eEventSystemInternalFlagAppQuitRequestSetBit = 0x010000,
	};

	struct EventSystemConfig
	{
		// Configuration flags. Allow controlling aspects like mouse or keyboard behaviour.
		cppx::bitmask<EEventSystemConfigFlags> configFlags = 0u;

		// Timeout (in milliseconds) after which mouse click sequence is reset.
		cppx::duration_value_t mouseClickSequenceTimeoutMs = 100;
	};

	struct EventSystemSharedState
	{
		// Pointer to the current configuration of the event system. This configuration is managed by EventDispatchers
		// and is only referenced here. This happens when a new dispatcher is set (EventController::_OnActiveDispatcherChange).
		const EventSystemConfig * currentEventSystemConfig = nullptr;

		// Current state of the keyboard cached by the event system.
		EvtSharedInputKeyboardState inputKeyboardState;

		// Current state of the mouse cached by the event system.
		EvtSharedInputMouseState inputMouseState;

		// Internal flags, used by the event system.
		cppx::bitmask<EEventSystemInternalFlags> internalStateFlags = 0u;

		CPPX_ATTR_NO_DISCARD const EventSystemConfig & GetEventSystemConfig() const
		{
			Ic3DebugAssert( currentEventSystemConfig );
			return *currentEventSystemConfig;
		}
	};

	/// @brief Private, implementation-specific data of the EventController class.
	struct EventController::EventControllerPrivateData
	{
		// Current active dispatcher used to forward all events. Initially NULL and can be reset to this state.
		EventDispatcher * activeEventDispatcher = nullptr;

		// Pointer to the primary event source. Set by the user, may be NULL at any given point.
		// Used primarily for the auto-quit behaviour (emitting quit event after last/primary event source is destroyed).
		EventSource * primaryEventSource = nullptr;

		// Container for all dispatchers created in the system.
		std::vector<EventDispatcher *> eventDispatcherList;

		// List of all registered event sources, i.e. all windows/surfaces/views currently observed by our event system.
		std::vector<EventSource *> eventSourceList;

		//
		EventSystemSharedState sharedEventSystemState;

		LocalEventQueue priorityEventQueue;

		LocalEventQueue userEventQueue;

		using InternalEventDispatcherRef = std::vector<EventDispatcher *>::iterator;
		using InternalEventSourceRef = std::vector<EventSource *>::iterator;

		CPPX_ATTR_NO_DISCARD std::pair<bool, InternalEventDispatcherRef> FindEventDispatcherInternal( EventDispatcher * pEventDispatcher )
		{
			std::pair<bool, InternalEventDispatcherRef> result;
			result.second = std::find( eventDispatcherList.begin(), eventDispatcherList.end(), pEventDispatcher );
			result.first = ( result.second != eventDispatcherList.end() );
			return result;
		}

		CPPX_ATTR_NO_DISCARD std::pair<bool, InternalEventSourceRef> FindEventSourceInternal( EventSource * pEventSource )
		{
			std::pair<bool, InternalEventSourceRef> result;
			result.second = std::find( eventSourceList.begin(), eventSourceList.end(), pEventSource );
			result.first = ( result.second != eventSourceList.end() );
			return result;
		}
	};

	/// @brief Private, implementation-specific data of the EventDispatcher class.
	struct EventDispatcher::EventDispatcherPrivateData
	{
		EventSystemConfig currentEventSystemConfig;

		// A default handler. If set, it is called if there is no handler registered for a given code/category/base type.
		EventHandler defaultHandler;

		// Array of handlers registered for EventBaseType.
		std::array<EventHandler, cxEnumEventBaseTypeCount> handlerMapByBaseType;

		// Array of handlers registered for EventCategory.
		std::array<EventHandler, cxEnumEventCategoryCount> handlerMapByCategory;

		// Array of handlers registered for EventCodeIndex (i.e. event code itself).
		std::array<EventHandler, cxEnumEventCodeIndexCount> handlerMapByCodeIndex;
	};

	namespace Platform
	{

		struct NativeEventType;

		/**
		 * @brief Translates a native, OS-specific event into an internal representation.
		 * This is an internal function, implemented at the OS API level. Technically, it could be purely OS-specific,
		 * but having it here gives us the ability to write the dispatching helper function below and avoid duplicating.
		 * @param pEventController
		 * @param pNativeEvent
		 * @param pOutEvent
		 * @return
		 */
		IC3_SYSTEM_API bool NativeEventTranslate(
				EventController & pEventController,
				const NativeEventType & pNativeEvent,
				EventObject & pOutEvent );

		/**
		 * @brief Helper function for translating and dispatching a native event.
		 * @param pEventController EventController to be used for dispatching.
		 * @param pNativeEvent An event to dispatch.
		 * @return True if the event has been successfully dispatched, or false otherwise.
		 */
		inline bool NativeEventDispatch( EventController & pEventController, const NativeEventType & pNativeEvent )
		{
			EventObject eventObject;

			auto & eventSystemSharedState = pEventController.GetEventSystemSharedState();

			eventObject.eventSystemSharedState = &eventSystemSharedState;

			// Translate the input event and store the output in the temporary auto event object.
			// The boolean result indicates whether the translation was successful (event is known).
			if( NativeEventTranslate( pEventController, pNativeEvent, eventObject ) )
			{
				// Dispatch the event through the provided controller object.
				// The boolean result indicates whether there was a handler for this event.
				// False means, that the event has not been processed.
				if( pEventController.DispatchEvent( eventObject ) )
				{
					return true;
				}
			}

			return false;
		}

	}

	void evtUpdateEventInputMouse( EvtInputMouse & pMouseEvent, EventSystemSharedState & pEventSystemSharedState );
	void evtUpdateEventInputMouseButton( EvtInputMouseButton & pMouseButtonEvent, EventSystemSharedState & pEventSystemSharedState );
	void evtUpdateEventInputMouseMove( EvtInputMouseMove & pMouseMoveEvent, EventSystemSharedState & pEventSystemSharedState );

} // namespace Ic3::System

#endif // __IC3_SYSTEM_INTERNAL_EVENT_CORE_PRIVATE_H__
