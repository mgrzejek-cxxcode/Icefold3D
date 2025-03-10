
#include "../Internal/EventCorePrivate.h"
#include <Ic3/Math/VectorOps.h>

namespace Ic3::System
{

	EventController::EventController( SysContextHandle pSysContext )
	: SysObject( std::move( pSysContext ) )
	, _privateData( std::make_unique<EventControllerPrivateData>() )
	{}

	EventController::~EventController() noexcept = default;

	EventDispatcherHandle EventController::CreateEventDispatcher()
	{
		auto dispatcher = CreateSysObject<EventDispatcher>( GetHandle<EventController>() );

		_privateData->eventDispatcherList.push_back( dispatcher.get() );

		return dispatcher;
	}

	void EventController::RegisterEventSource( EventSource & pEventSource )
	{
		_NativeRegisterEventSource( pEventSource );

		auto eventSourceRef = _privateData->FindEventSourceInternal( &pEventSource );
		if( eventSourceRef.first )
		{
			Ic3Throw( eExcCodeDebugPlaceholder );
		}

		_privateData->eventSourceList.push_back( &pEventSource );

		pEventSource.SetEventController( GetHandle<EventController>() );
	}

	bool EventController::RegisterPrimaryEventSource( EventSource & pEventSource )
	{
		if( _privateData->primaryEventSource )
		{
			// If the primary event source has been already set, we abort the process.
			// This is an all-or-nothing transaction - there should be no automatic overwriting here.
			return false;
		}

		_privateData->primaryEventSource = &pEventSource;

		RegisterEventSource( pEventSource );

		return true;
	}

	void EventController::UnregisterEventSource( EventSource & pEventSource )
	{
		auto eventSourceRef = _privateData->FindEventSourceInternal( &pEventSource );
		if( !eventSourceRef.first )
		{
			Ic3DebugInterrupt();
			return;
		}

		_privateData->eventSourceList.erase( eventSourceRef.second );

		_NativeUnRegisterEventSource( pEventSource );

		if( &pEventSource == _privateData->primaryEventSource )
		{
			_privateData->primaryEventSource = nullptr;
		}

		pEventSource.SetEventController( nullptr );
	}

	void EventController::SetPrimaryEventSource( EventSource * pEventSource )
	{
		if( pEventSource )
		{
			// If the specified event source is not NULL, it should be a valid object,
			// already registered within this event controller. Check if that is the case.
			auto eventSourceRef = _privateData->FindEventSourceInternal( pEventSource );

			if( !eventSourceRef.first )
			{
				// Unknown event source. Most likely it has not been registered yet.
				Ic3Throw( eExcCodeDebugPlaceholder );
			}
		}

		_privateData->primaryEventSource = pEventSource;
	}

	bool EventController::SetActiveEventDispatcher( EventDispatcher & pEventDispatcher )
	{
		if( &pEventDispatcher != _privateData->activeEventDispatcher )
		{
			_OnActiveDispatcherChange( &pEventDispatcher );
			return true;
		}
		return false;
	}

	bool EventController::ResetActiveEventDispatcher()
	{
		if( _privateData->activeEventDispatcher  )
		{
			_OnActiveDispatcherChange( nullptr );
			return true;
		}
		return false;
	}

	bool EventController::DispatchEvent( EventObject pEvent )
	{
		ValidateActiveDispatcherState();

		const auto dispatchResult = _privateData->activeEventDispatcher->PostEvent( pEvent );

		if( pEvent.uCode == eEventCodeWindowUpdateDestroy )
		{
			//
			if( auto * eventSource = pEvent.uEvtWindowUpdateDestroy.eventSource )
			{
				//
				_CheckAndPostAppAutoQuitEvent( eEventCodeWindowUpdateDestroy, *eventSource );
			}
		}

		return dispatchResult;
	}

	void EventController::PushUserEvent( EventObject pEvent )
	{
		_privateData->userEventQueue.push_back( pEvent );
	}

	EventObject & EventController::EmplaceUserEvent()
	{
		return _privateData->userEventQueue.emplace_back();
	}

	void EventController::PushPriorityEvent( EventObject pEvent )
	{
		_privateData->priorityEventQueue.push_back( pEvent );
	}

	EventObject & EventController::EmplacePriorityEvent()
	{
		return _privateData->priorityEventQueue.emplace_back();
	}

	uint32 EventController::DispatchPendingEventsAuto()
	{
		ValidateActiveDispatcherState();

		uint32 eventCounter = 0;

		while( _ProcessLocalQueues() )
		{
			++eventCounter;
		}

		while( _NativeDispatchPendingEvents() )
		{
			++eventCounter;
		}

		if( eventCounter == 0 )
		{
			if( CheckEventSystemConfigFlags( eEventSystemConfigFlagIdleProcessingModeBit ) )
			{
				_NativeDispatchPendingEventsWait();
			}
		}

		return eventCounter;
	}

	uint32 EventController::DispatchPendingEventsPeek( uint32 pLimit )
	{
		ValidateActiveDispatcherState();

		uint32 eventCounter = 0;

		while( _ProcessLocalQueues() && ( eventCounter <= pLimit ) )
		{
			++eventCounter;
		}

		while( _NativeDispatchPendingEvents() && ( eventCounter <= pLimit ) )
		{
			++eventCounter;
		}

		return eventCounter;
	}

	uint32 EventController::DispatchPendingEventsWait( uint32 pLimit )
	{
		ValidateActiveDispatcherState();

		uint32 eventCounter = 0;

		while( _ProcessLocalQueues() && ( eventCounter <= pLimit ) )
		{
			++eventCounter;
		}

		while( _NativeDispatchPendingEvents() && ( eventCounter <= pLimit ) )
		{
			++eventCounter;
		}

		if( eventCounter == 0 )
		{
			_NativeDispatchPendingEventsWait();
		}

		return eventCounter;
	}

	uint32 EventController::DispatchPendingEventsWaitTimeout( const cppx::microseconds & pTimeout, uint32 pLimit )
	{
		ValidateActiveDispatcherState();

		uint32 eventCounter = 0;

		while( _ProcessLocalQueues() && ( eventCounter <= pLimit ) )
		{
			++eventCounter;
		}

		while( _NativeDispatchPendingEvents() && ( eventCounter <= pLimit ) )
		{
			++eventCounter;
		}

		if( eventCounter == 0 )
		{
			_NativeDispatchPendingEventsWaitTimeout( pTimeout );
		}

		return eventCounter;
	}

	void EventController::ValidateActiveDispatcherState() const
	{
		if( !IsActiveDispatcherSet() )
		{
			Ic3Throw( eExcCodeDebugPlaceholder );
		}
	}

	EventSystemSharedState & EventController::GetEventSystemSharedState() noexcept
	{
		return _privateData->sharedEventSystemState;
	}

	bool EventController::CheckEventSystemConfigFlags( cppx::bitmask<EEventSystemConfigFlags> pFlags ) const
	{
		if( !IsActiveDispatcherSet() )
		{
			return false;
		}
		return _privateData->activeEventDispatcher->CheckEventSystemConfigFlags( pFlags );
	}

	EventSource * EventController::FindEventSource( const EventSourceFindPredicate & pPredicate ) const
	{
		for( auto * eventSource : _privateData->eventSourceList )
		{
			if( eventSource && pPredicate( *eventSource ) )
			{
				return eventSource;
			}
		}
		return nullptr;
	}

	EventSource * EventController::FindEventSource( const EventSourceNativeDataFindPredicate & pPredicate ) const
	{
		for( auto * eventSource : _privateData->eventSourceList )
		{
			if( pPredicate( eventSource->GetEventSourceNativeData() ) )
			{
				return eventSource;
			}
		}
		return nullptr;
	}

	bool EventController::IsActiveDispatcherSet() const noexcept
	{
		if( !_privateData->activeEventDispatcher )
		{
			// No active dispatcher set. This basically makes the event system useless, as no events
			// can be forwarded to the respective handlers. Since there can be multiple dispatchers
			// switched at runtime according to needs, this is a crucial check to make.

			return false;
		}

		if( !_privateData->sharedEventSystemState.currentEventSystemConfig )
		{
			// Missing per-dispatcher data. Configuration and input state belong to a dispatcher,
			// so the system can properly adjust when they are changed.
			// Currently, this situation is rather impossible to occur (config and inputState are
			// stored by value and set/cleared inside _OnActiveDispatcherChange()), but if we ever
			// change this, this check will help to catch potential issues much faster.

			return false;
		}

		return true;
	}

	size_t EventController::GetRegisteredEventSourcesNum() const noexcept
	{
		return _privateData->eventSourceList.size();
	}

	EventSource * EventController::GetPrimaryEventSource() const noexcept
	{
		return _privateData->primaryEventSource;
	}

	EventSource * EventController::GetRegisteredEventSourceByIndex( size_t pIndex ) const noexcept
	{
		return _privateData->eventSourceList[pIndex];
	}

	bool EventController::IsEventSourceRegistered( const EventSource & pEventSource ) const noexcept
	{
		for( auto * eventSource : _privateData->eventSourceList )
		{
			if( &pEventSource == eventSource )
			{
				return true;
			}
		}
		return false;
	}

	void EventController::OnEventSourceDestroy( EventSource & pEventSource ) noexcept
	{
		UnregisterEventSource( pEventSource );
	}

	void EventController::OnEventDispatcherDestroy( EventDispatcher & pEventDispatcher ) noexcept
	{
		auto eventDispatcherRef = _privateData->FindEventDispatcherInternal( &pEventDispatcher );

		if( !eventDispatcherRef.first )
		{
			Ic3DebugInterrupt();
			return;
		}

		_privateData->eventDispatcherList.erase( eventDispatcherRef.second );
	}

	void EventController::_NativeRegisterEventSource( EventSource & /* pEventSource */ )
	{}

	void EventController::_NativeUnRegisterEventSource( EventSource & /* pEventSource */ )
	{}

	void EventController::_OnActiveDispatcherChange( EventDispatcher * pEventDispatcher )
	{
		if( pEventDispatcher )
		{
			auto & eventSystemConfig = pEventDispatcher->GetEventSystemConfig();
			_privateData->sharedEventSystemState.currentEventSystemConfig = &eventSystemConfig;
		}
		else
		{
			_privateData->sharedEventSystemState.currentEventSystemConfig = nullptr;
		}

		_privateData->activeEventDispatcher = pEventDispatcher;
	}

	bool EventController::_ProcessLocalQueues()
	{
		auto & priorityQueue = _privateData->priorityEventQueue;

		if( !priorityQueue.empty() )
		{
			auto nextPriorityEvent = priorityQueue.front();
			priorityQueue.pop_front();

			if( DispatchEvent( nextPriorityEvent ) )
			{
				return true;
			}
		}

		auto & userQueue = _privateData->userEventQueue;

		if( !userQueue.empty() )
		{
			auto nextUserEvent = userQueue.front();
			userQueue.pop_front();

			if( DispatchEvent( nextUserEvent ) )
			{
				return true;
			}
		}

		return false;
	}

	bool EventController::_CheckAndPostAppAutoQuitEvent( EEventCode pEvent, EventSource & pEventSource )
	{
		if( pEvent != eEventCodeWindowUpdateDestroy )
		{
			return false;
		}

		bool postAutoQuit = false;

		if( pEventSource.IsLastEventSource() )
		{
			if( CheckEventSystemConfigFlags( eEventSystemConfigFlagEnableAutoQuitOnLastSourceDestroyBit ) )
			{
				postAutoQuit = true;
			}
		}

		if( !postAutoQuit && pEventSource.IsPrimaryEventSource() )
		{
			if( CheckEventSystemConfigFlags( eEventSystemConfigFlagEnableAutoQuitOnPrimarySourceDestroyBit ) )
			{
				postAutoQuit = true;
			}
		}

		if( postAutoQuit )
		{
			auto internalStateMask = _GetInternalStateFlags();

			// Dedicated mask for quick checking if any of the quit modes has been properly set.
			if( !internalStateMask.is_set_any_of( eEventSystemConfigMaskSetAutoQuitMode ) )
			{
				// If not, we default to the one based on user message. This is probably the best one, as it allows
				// to still process pending important messages before breaking the whole event lopp.
				internalStateMask.set( eEventSystemConfigFlagSetAutoQuitModePostUserEventBit );
			}

			if( internalStateMask.is_set( eEventSystemConfigFlagSetAutoQuitModeDirectEventDispatchBit ) )
			{
				// Dispatch event immediately. This basically causes the handler for the AppQuit to be executed here immediately.
				DispatchEvent( EventObject{eEventCodeAppActivityQuit } );
			}
			else if( internalStateMask.is_set( eEventSystemConfigFlagSetAutoQuitModePostPriorityEventBit ) )
			{
				// Push the AppQuit event to the end of the priority queue.
				PushPriorityEvent( EventObject{eEventCodeAppActivityQuit } );
			}
			else if( internalStateMask.is_set( eEventSystemConfigFlagSetAutoQuitModePostUserEventBit ) )
			{
				// Push the AppQuit event to the end of the user queue.
				PushUserEvent( EventObject{eEventCodeAppActivityQuit } );
			}
			else if( internalStateMask.is_set( eEventSystemConfigFlagSetAutoQuitModeSetInternalRequestOnlyBit ) )
			{
				// Only set the internal flag to mark a quit request has been issued. The system will process it when there are no events.
				_SetInternalStateFlags( eEventSystemInternalFlagAppQuitRequestSetBit, true );
			}
			else
			{
				Ic3DebugInterrupt();
			}
		}

		return postAutoQuit;
	}

	void EventController::_SetInternalStateFlags( cppx::bitmask<uint32> pFlags, bool pSetOrUnset )
	{
		_privateData->sharedEventSystemState.internalStateFlags.set_or_unset( pFlags, pSetOrUnset );
	}

	uint32 EventController::_CheckInternalStateFlags( cppx::bitmask<uint32> pFlags ) const
	{
		return ( _privateData->sharedEventSystemState.internalStateFlags & pFlags );
	}

	cppx::bitmask<uint32> EventController::_GetInternalStateFlags() const
	{
		return _privateData->sharedEventSystemState.internalStateFlags;
	}


	EventDispatcher::EventDispatcher( EventControllerHandle pEventController )
	: SysObject( pEventController->mSysContext )
	, _eventControllerActiveRef( std::move( pEventController ) )
	, _privateData( std::make_unique<EventDispatcherPrivateData>() )
	{}

	EventDispatcher::~EventDispatcher() noexcept
	{
		if( _eventControllerActiveRef )
		{
			_eventControllerActiveRef->OnEventDispatcherDestroy( *this );
			_eventControllerActiveRef = nullptr;
		}
	}

	void EventDispatcher::SetActive()
	{
		_eventControllerActiveRef->SetActiveEventDispatcher( *this );
	}

	void EventDispatcher::SetEventHandler( EEventBaseType pBaseType, EventHandler pHandler )
	{
		// EventBaseType enum is numbered from zero to have constant access time to its entry.
		auto baseTypeValue = static_cast<size_t>( pBaseType );
		// Check for possible violation attempt.
		if( baseTypeValue >= cxEnumEventBaseTypeCount )
		{
			Ic3Throw( eExcCodeDebugPlaceholder );
		}
		_privateData->handlerMapByBaseType[baseTypeValue] = std::move( pHandler );
	}

	void EventDispatcher::SetEventHandler( EEventCategory pCategory, EventHandler pHandler )
	{
		// EventCategory enum is numbered from zero to have constant access time to its entry.
		auto categoryValue = static_cast<size_t>( pCategory );
		// Check for possible violation attempt.
		if( categoryValue >= cxEnumEventCategoryCount )
		{
			Ic3Throw( eExcCodeDebugPlaceholder );
		}
		_privateData->handlerMapByCategory[categoryValue] = std::move( pHandler );
	}

	void EventDispatcher::SetEventHandler( EEventCodeIndex pCodeIndex, EventHandler pHandler )
	{
		// EventCodeIndex enum is numbered from zero to have constant access time to its entry.
		auto codeIndexValue = static_cast<size_t>( pCodeIndex );
		// Check for possible violation attempt.
		if( codeIndexValue >= cxEnumEventCodeIndexCount )
		{
			Ic3Throw( eExcCodeDebugPlaceholder );
		}
		_privateData->handlerMapByCodeIndex[codeIndexValue] = std::move( pHandler );
	}

	void EventDispatcher::SetDefaultEventHandler( EventHandler pHandler )
	{
		_privateData->defaultHandler = std::move( pHandler );
	}

	void EventDispatcher::ResetEventHandler( EEventBaseType pBaseType )
	{
		SetEventHandler( pBaseType, nullptr );
	}

	void EventDispatcher::ResetEventHandler( EEventCategory pCategory )
	{
		SetEventHandler( pCategory, nullptr );
	}

	void EventDispatcher::ResetEventHandler( EEventCodeIndex pCodeIndex )
	{
		SetEventHandler( pCodeIndex, nullptr );
	}

	void EventDispatcher::ResetDefaultEventHandler()
	{
		SetDefaultEventHandler( nullptr );
	}

	void EventDispatcher::SetEventSystemConfigFlags( cppx::bitmask<EEventSystemConfigFlags> pFlags, bool pSetOrUnset )
	{
		const auto configFlagsOnly = (pFlags & eEventSystemConfigMaskAll );
		Ic3DebugAssert( configFlagsOnly == pFlags );
		_privateData->currentEventSystemConfig.configFlags.set_or_unset( pFlags, pSetOrUnset );
	}

	void EventDispatcher::SetIdleProcessingMode( bool pIdle )
	{
		_privateData->currentEventSystemConfig.configFlags.set_or_unset( eEventSystemConfigFlagIdleProcessingModeBit,
		                                                                 pIdle );
	}

	bool EventDispatcher::PostEvent( EventObject pEvent )
	{
		if( CXU::validateEventCode( pEvent.uCode ) )
		{
			if( _PreProcessEvent( pEvent ) )
			{
				pEvent.uCommonData.timeStamp = PerfCounter::QueryCounter();

				{
					auto codeIndexValue = static_cast<size_t>( CXU::GetEventCodeCodeIndex( pEvent.uCode ) );
					auto & eventHandler = _privateData->handlerMapByCodeIndex[codeIndexValue];
					if( eventHandler && eventHandler( pEvent ) )
					{
						return true;
					}
				}
				{
					auto categoryValue = static_cast<size_t>( CXU::GetEventCodeCategory( pEvent.uCode ) );
					auto & eventHandler = _privateData->handlerMapByCategory[categoryValue];
					if( eventHandler && eventHandler( pEvent ) )
					{
						return true;
					}
				}
				{
					auto baseTypeValue = static_cast<size_t>( CXU::GetEventCodeBaseType( pEvent.uCode ) );
					auto & eventHandler = _privateData->handlerMapByBaseType[baseTypeValue];
					if( eventHandler && eventHandler( pEvent ) )
					{
						return true;
					}
				}
				{
					auto & defaultHandler = _privateData->defaultHandler;
					if( defaultHandler && defaultHandler( pEvent ) )
					{
						return true;
					}
				}
			}
		}

		return false;
	}

	bool EventDispatcher::PostEvent( event_code_value_t pEventCode )
	{
		return PostEvent( EventObject( pEventCode ) );
	}

	bool EventDispatcher::PostEventAppQuit()
	{
		return PostEvent( eEventCodeAppActivityQuit );
	}

	bool EventDispatcher::PostEventAppTerminate()
	{
		return PostEvent( eEventCodeAppActivityTerminate );
	}

	bool EventDispatcher::CheckEventSystemConfigFlags( cppx::bitmask<EEventSystemConfigFlags> pFlags ) const
	{
		const auto configFlagsOnly = (pFlags & eEventSystemConfigMaskAll );
		Ic3DebugAssert( configFlagsOnly == pFlags );
		return _privateData->currentEventSystemConfig.configFlags.is_set( configFlagsOnly );
	}

	cppx::bitmask<EEventSystemConfigFlags> EventDispatcher::GetEventSystemConfigFlags() const
	{
		return _privateData->currentEventSystemConfig.configFlags;
	}

	const EventSystemConfig & EventDispatcher::GetEventSystemConfig() const
	{
		return _privateData->currentEventSystemConfig;
	}

	bool EventDispatcher::_PreProcessEvent( EventObject & pEvent )
	{
		const auto eventCategory = pEvent.GetCategory();

		auto & eventSystemSharedState = _eventControllerActiveRef->GetEventSystemSharedState();

		if( eventCategory == EEventCategory::InputKeyboard )
		{
			return _PreProcessEventKeyboard( pEvent.uEvtInputKeyboard, eventSystemSharedState );
		}

		if( eventCategory == EEventCategory::InputMouse )
		{
			return _PreProcessEventMouse( pEvent.uEvtInputMouse, eventSystemSharedState );
		}

		return true;
	}

	bool EventDispatcher::_PreProcessEventKeyboard( EvtInputKeyboard & pKeyboardEvent, EventSystemSharedState & pSharedState )
	{
		if( pKeyboardEvent.keyCode == EKeyCode::Unknown )
		{
			// Discard unknown key events
			return false;
		}

		if( pKeyboardEvent.keyAction == EKeyActionType::Press )
		{
			pSharedState.inputKeyboardState.keyStateMap[pKeyboardEvent.keyCode] = true;
		}
		else
		{
			pSharedState.inputKeyboardState.keyStateMap[pKeyboardEvent.keyCode] = false;
		}

		return true;
	}

	bool EventDispatcher::_PreProcessEventMouse( EvtInputMouse & pMouseEvent, EventSystemSharedState & pSharedState )
	{
		return true;
	}


	void evtUpdateEventInputMouse( EvtInputMouse & pMouseEvent, EventSystemSharedState & pEventSystemSharedState )
	{
		if( pEventSystemSharedState.inputMouseState.lastCursorPos == CX_EVENT_MOUSE_POS_INVALID )
		{
			pEventSystemSharedState.inputMouseState.lastCursorPos = pMouseEvent.cursorPos;
		}
	}

	void evtUpdateEventInputMouseButton( EvtInputMouseButton & pMouseButtonEvent, EventSystemSharedState & pEventSystemSharedState )
	{
		evtUpdateEventInputMouse( pMouseButtonEvent, pEventSystemSharedState );

		const auto & eventSystemConfig = pEventSystemSharedState.GetEventSystemConfig();
		auto & inputMouseState = pEventSystemSharedState.inputMouseState;

		if( eventSystemConfig.configFlags.is_set( eEventSystemConfigFlagEnableMouseMultiClickBit ) )
		{
			if( pMouseButtonEvent.buttonAction == EMouseButtonActionType::Click )
			{
				bool multiClickEventSet = false;

				if( inputMouseState.lastPressButtonID == pMouseButtonEvent.buttonID )
				{
					auto lastClickDiff = pMouseButtonEvent.timeStamp - inputMouseState.lastPressTimestamp;
					auto lastClickDiffMs = PerfCounter::ConvertToMilliseconds( lastClickDiff );
					if( lastClickDiffMs <= eventSystemConfig.mouseClickSequenceTimeoutMs )
					{
						if( inputMouseState.currentMultiClickSequenceLength == 1 )
						{
							pMouseButtonEvent.buttonAction = EMouseButtonActionType::DoubleClick;
							inputMouseState.currentMultiClickSequenceLength = 2;
							multiClickEventSet = true;
						}
						else if( eventSystemConfig.configFlags.is_set( eEventSystemConfigFlagEnableMouseMultiClickBit ) )
						{
							pMouseButtonEvent.buttonAction = EMouseButtonActionType::MultiClick;
							inputMouseState.currentMultiClickSequenceLength += 1;
							multiClickEventSet = true;
						}
					}
				}

				if( !multiClickEventSet )
				{
					// Multi-click has not been detected/updated. Possible reasons: different button ID,
					// click timeout (mouse button clicked too slow), multi-click support is disabled.
					inputMouseState.lastPressButtonID = pMouseButtonEvent.buttonID;
					inputMouseState.currentMultiClickSequenceLength = 1;
				}

				inputMouseState.lastPressTimestamp = pMouseButtonEvent.timeStamp;
			}
		}

		const auto buttonMask = CXU::GetMouseButtonFlagFromButtonID( pMouseButtonEvent.buttonID );
		if( pMouseButtonEvent.buttonAction == EMouseButtonActionType::Click )
		{
			pMouseButtonEvent.buttonStateMask.set( buttonMask );
		}
		else if( pMouseButtonEvent.buttonAction == EMouseButtonActionType::Release )
		{
			pMouseButtonEvent.buttonStateMask.unset( buttonMask );
		}
	}

	void evtUpdateEventInputMouseMove( EvtInputMouseMove & pMouseMoveEvent, EventSystemSharedState & pEventSystemSharedState )
	{
		evtUpdateEventInputMouse( pMouseMoveEvent, pEventSystemSharedState );

		auto & inputMouseState = pEventSystemSharedState.inputMouseState;

		pMouseMoveEvent.movementDelta = pMouseMoveEvent.cursorPos - inputMouseState.lastCursorPos;

		inputMouseState.lastCursorPos = pMouseMoveEvent.cursorPos;
	}

} // namespace Ic3::System
