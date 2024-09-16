
#include "Internal/EventCorePrivate.h"
#include <Ic3/Math/VectorOps.h>

namespace Ic3::System
{

	EventController::EventController( SysContextHandle pSysContext )
	: SysObject( std::move( pSysContext ) )
	, _privateData( std::make_unique<EventControllerPrivateData>() )
	{}

	EventController::~EventController() noexcept = default;

	EventDispatcherHandle EventController::createEventDispatcher()
	{
		auto dispatcher = createSysObject<EventDispatcher>( getHandle<EventController>() );

		_privateData->mEventDispatcherList.push_back( dispatcher.get() );

		return dispatcher;
	}

	void EventController::registerEventSource( EventSource & pEventSource )
	{
		_nativeRegisterEventSource( pEventSource );

		auto eventSourceRef = _privateData->findEventSourceInternal( &pEventSource );
		if( eventSourceRef.first )
		{
			ic3Throw( E_EXC_DEBUG_PLACEHOLDER );
		}

		_privateData->mEventSourceList.push_back( &pEventSource );

		pEventSource.setEventController( getHandle<EventController>() );
	}

	bool EventController::registerPrimaryEventSource( EventSource & pEventSource )
	{
		if( _privateData->mPrimaryEventSource )
		{
			// If the primary event source has been already set, we abort the process.
			// This is an all-or-nothing transaction - there should be no automatic overwriting here.
			return false;
		}

		_privateData->mPrimaryEventSource = &pEventSource;

		registerEventSource( pEventSource );

		return true;
	}

	void EventController::unregisterEventSource( EventSource & pEventSource )
	{
		auto eventSourceRef = _privateData->findEventSourceInternal( &pEventSource );
		if( !eventSourceRef.first )
		{
			ic3DebugInterrupt();
			return;
		}

		_privateData->mEventSourceList.erase( eventSourceRef.second );

		_nativeUnregisterEventSource( pEventSource );

		if( &pEventSource == _privateData->mPrimaryEventSource )
		{
			_privateData->mPrimaryEventSource = nullptr;
		}

		pEventSource.setEventController( nullptr );
	}

	void EventController::setPrimaryEventSource( EventSource * pEventSource )
	{
		if( pEventSource )
		{
			// If the specified event source is not NULL, it should be a valid object,
			// already registered within this event controller. Check if that is the case.
			auto eventSourceRef = _privateData->findEventSourceInternal( pEventSource );

			if( !eventSourceRef.first )
			{
				// Unknown event source. Most likely it has not been registered yet.
				ic3Throw( E_EXC_DEBUG_PLACEHOLDER );
			}
		}

		_privateData->mPrimaryEventSource = pEventSource;
	}

	bool EventController::setActiveEventDispatcher( EventDispatcher & pEventDispatcher )
	{
		if( &pEventDispatcher != _privateData->mActiveEventDispatcher )
		{
			_onActiveDispatcherChange( &pEventDispatcher );
			return true;
		}
		return false;
	}

	bool EventController::resetActiveEventDispatcher()
	{
		if( _privateData->mActiveEventDispatcher  )
		{
			_onActiveDispatcherChange( nullptr );
			return true;
		}
		return false;
	}

	bool EventController::dispatchEvent( EventObject pEvent )
	{
		validateActiveDispatcherState();

		const auto dispatchResult = _privateData->mActiveEventDispatcher->postEvent( pEvent );

		if( pEvent.uCode == eEventCodeWindowUpdateDestroy )
		{
			//
			if( auto * eventSource = pEvent.uEvtWindowUpdateDestroy.mEventSource )
			{
				//
				_checkAndPostAppAutoQuitEvent( eEventCodeWindowUpdateDestroy, *eventSource );
			}
		}

		return dispatchResult;
	}

	void EventController::pushUserEvent( EventObject pEvent )
	{
		_privateData->mUserEventQueue.push_back( pEvent );
	}

	EventObject & EventController::emplaceUserEvent()
	{
		return _privateData->mUserEventQueue.emplace_back();
	}

	void EventController::pushPriorityEvent( EventObject pEvent )
	{
		_privateData->mPriorityEventQueue.push_back( pEvent );
	}

	EventObject & EventController::emplacePriorityEvent()
	{
		return _privateData->mPriorityEventQueue.emplace_back();
	}

	uint32 EventController::dispatchPendingEventsAuto()
	{
		validateActiveDispatcherState();

		uint32 eventCounter = 0;

		while( _processLocalQueues() )
		{
			++eventCounter;
		}

		while( _nativeDispatchPendingEvents() )
		{
			++eventCounter;
		}

		if( eventCounter == 0 )
		{
			if( checkEventSystemConfigFlags( eEventSystemConfigFlagIdleProcessingModeBit ) )
			{
				_nativeDispatchPendingEventsWait();
			}
		}

		return eventCounter;
	}

	uint32 EventController::dispatchPendingEventsPeek( uint32 pLimit )
	{
		validateActiveDispatcherState();

		uint32 eventCounter = 0;

		while( _processLocalQueues() && ( eventCounter <= pLimit ) )
		{
			++eventCounter;
		}

		while( _nativeDispatchPendingEvents() && ( eventCounter <= pLimit ) )
		{
			++eventCounter;
		}

		return eventCounter;
	}

	uint32 EventController::dispatchPendingEventsWait( uint32 pLimit )
	{
		validateActiveDispatcherState();

		uint32 eventCounter = 0;

		while( _processLocalQueues() && ( eventCounter <= pLimit ) )
		{
			++eventCounter;
		}

		while( _nativeDispatchPendingEvents() && ( eventCounter <= pLimit ) )
		{
			++eventCounter;
		}

		if( eventCounter == 0 )
		{
			_nativeDispatchPendingEventsWait();
		}

		return eventCounter;
	}

	uint32 EventController::dispatchPendingEventsWaitTimeout( const Cppx::Microseconds & pTimeout, uint32 pLimit )
	{
		validateActiveDispatcherState();

		uint32 eventCounter = 0;

		while( _processLocalQueues() && ( eventCounter <= pLimit ) )
		{
			++eventCounter;
		}

		while( _nativeDispatchPendingEvents() && ( eventCounter <= pLimit ) )
		{
			++eventCounter;
		}

		if( eventCounter == 0 )
		{
			_nativeDispatchPendingEventsWaitTimeout( pTimeout );
		}

		return eventCounter;
	}

	void EventController::validateActiveDispatcherState() const
	{
		if( !isActiveDispatcherSet() )
		{
			ic3Throw( E_EXC_DEBUG_PLACEHOLDER );
		}
	}

	EventSystemSharedState & EventController::getEventSystemSharedState() noexcept
	{
		return _privateData->mSharedEventSystemState;
	}

	bool EventController::checkEventSystemConfigFlags( TBitmask<EEventSystemConfigFlags> pFlags ) const
	{
		if( !isActiveDispatcherSet() )
		{
			return false;
		}
		return _privateData->mActiveEventDispatcher->checkEventSystemConfigFlags( pFlags );
	}

	EventSource * EventController::findEventSource( const EventSourceFindPredicate & pPredicate ) const
	{
		for( auto * eventSource : _privateData->mEventSourceList )
		{
			if( eventSource && pPredicate( *eventSource ) )
			{
				return eventSource;
			}
		}
		return nullptr;
	}

	EventSource * EventController::findEventSource( const EventSourceNativeDataFindPredicate & pPredicate ) const
	{
		for( auto * eventSource : _privateData->mEventSourceList )
		{
			if( pPredicate( eventSource->getEventSourceNativeData() ) )
			{
				return eventSource;
			}
		}
		return nullptr;
	}

	bool EventController::isActiveDispatcherSet() const noexcept
	{
		if( !_privateData->mActiveEventDispatcher )
		{
			// No active dispatcher set. This basically makes the event system useless, as no events
			// can be forwarded to the respective handlers. Since there can be multiple dispatchers
			// switched at runtime according to needs, this is a crucial check to make.

			return false;
		}

		if( !_privateData->mSharedEventSystemState.mCurrentEventSystemConfig )
		{
			// Missing per-dispatcher data. Configuration and input state belong to a dispatcher,
			// so the system can properly adjust when they are changed.
			// Currently, this situation is rather impossible to occur (config and inputState are
			// stored by value and set/cleared inside _onActiveDispatcherChange()), but if we ever
			// change this, this check will help to catch potential issues much faster.

			return false;
		}

		return true;
	}

	size_t EventController::getRegisteredEventSourcesNum() const noexcept
	{
		return _privateData->mEventSourceList.size();
	}

	EventSource * EventController::getPrimaryEventSource() const noexcept
	{
		return _privateData->mPrimaryEventSource;
	}

	EventSource * EventController::getRegisteredEventSourceByIndex( size_t pIndex ) const noexcept
	{
		return _privateData->mEventSourceList[pIndex];
	}

	bool EventController::isEventSourceRegistered( const EventSource & pEventSource ) const noexcept
	{
		for( auto * eventSource : _privateData->mEventSourceList )
		{
			if( &pEventSource == eventSource )
			{
				return true;
			}
		}
		return false;
	}

	void EventController::onEventSourceDestroy( EventSource & pEventSource ) noexcept
	{
		unregisterEventSource( pEventSource );
	}

	void EventController::onEventDispatcherDestroy( EventDispatcher & pEventDispatcher ) noexcept
	{
		auto eventDispatcherRef = _privateData->findEventDispatcherInternal( &pEventDispatcher );

		if( !eventDispatcherRef.first )
		{
			ic3DebugInterrupt();
			return;
		}

		_privateData->mEventDispatcherList.erase( eventDispatcherRef.second );
	}

	void EventController::_nativeRegisterEventSource( EventSource & /* pEventSource */ )
	{}

	void EventController::_nativeUnregisterEventSource( EventSource & /* pEventSource */ )
	{}

	void EventController::_onActiveDispatcherChange( EventDispatcher * pEventDispatcher )
	{
		if( pEventDispatcher )
		{
			auto & eventSystemConfig = pEventDispatcher->getEventSystemConfig();
			_privateData->mSharedEventSystemState.mCurrentEventSystemConfig = &eventSystemConfig;
		}
		else
		{
			_privateData->mSharedEventSystemState.mCurrentEventSystemConfig = nullptr;
		}

		_privateData->mActiveEventDispatcher = pEventDispatcher;
	}

	bool EventController::_processLocalQueues()
	{
		auto & priorityQueue = _privateData->mPriorityEventQueue;

		if( !priorityQueue.empty() )
		{
			auto nextPriorityEvent = priorityQueue.front();
			priorityQueue.pop_front();

			if( dispatchEvent( nextPriorityEvent ) )
			{
				return true;
			}
		}

		auto & userQueue = _privateData->mUserEventQueue;

		if( !userQueue.empty() )
		{
			auto nextUserEvent = userQueue.front();
			userQueue.pop_front();

			if( dispatchEvent( nextUserEvent ) )
			{
				return true;
			}
		}

		return false;
	}

	bool EventController::_checkAndPostAppAutoQuitEvent( EEventCode pEvent, EventSource & pEventSource )
	{
		if( pEvent != eEventCodeWindowUpdateDestroy )
		{
			return false;
		}

		bool postAutoQuit = false;

		if( pEventSource.isLastEventSource() )
		{
			if( checkEventSystemConfigFlags( eEventSystemConfigFlagEnableAutoQuitOnLastSourceDestroyBit ) )
			{
				postAutoQuit = true;
			}
		}

		if( !postAutoQuit && pEventSource.isPrimaryEventSource() )
		{
			if( checkEventSystemConfigFlags( eEventSystemConfigFlagEnableAutoQuitOnPrimarySourceDestroyBit ) )
			{
				postAutoQuit = true;
			}
		}

		if( postAutoQuit )
		{
			auto internalStateMask = _getInternalStateFlags();

			// Dedicated mask for quick checking if any of the quit modes has been properly set.
			if( !internalStateMask.isSetAnyOf( eEventSystemConfigMaskSetAutoQuitMode ) )
			{
				// If not, we default to the one based on user message. This is probably the best one, as it allows
				// to still process pending important messages before breaking the whole event lopp.
				internalStateMask.set( eEventSystemConfigFlagSetAutoQuitModePostUserEventBit );
			}

			if( internalStateMask.isSet( eEventSystemConfigFlagSetAutoQuitModeDirectEventDispatchBit ) )
			{
				// Dispatch event immediately. This basically causes the handler for the AppQuit to be executed here immediately.
				dispatchEvent( EventObject{eEventCodeAppActivityQuit } );
			}
			else if( internalStateMask.isSet( eEventSystemConfigFlagSetAutoQuitModePostPriorityEventBit ) )
			{
				// Push the AppQuit event to the end of the priority queue.
				pushPriorityEvent( EventObject{eEventCodeAppActivityQuit } );
			}
			else if( internalStateMask.isSet( eEventSystemConfigFlagSetAutoQuitModePostUserEventBit ) )
			{
				// Push the AppQuit event to the end of the user queue.
				pushUserEvent( EventObject{eEventCodeAppActivityQuit } );
			}
			else if( internalStateMask.isSet( eEventSystemConfigFlagSetAutoQuitModeSetInternalRequestOnlyBit ) )
			{
				// Only set the internal flag to mark a quit request has been issued. The system will process it when there are no events.
				_setInternalStateFlags( eEventSystemInternalFlagAppQuitRequestSetBit, true );
			}
			else
			{
				ic3DebugInterrupt();
			}
		}

		return postAutoQuit;
	}

	void EventController::_setInternalStateFlags( TBitmask<uint32> pFlags, bool pSetOrUnset )
	{
		_privateData->mSharedEventSystemState.mInternalStateFlags.setOrUnset( pFlags, pSetOrUnset );
	}

	uint32 EventController::_checkInternalStateFlags( TBitmask<uint32> pFlags ) const
	{
		return (_privateData->mSharedEventSystemState.mInternalStateFlags & pFlags );
	}

	TBitmask<uint32> EventController::_getInternalStateFlags() const
	{
		return _privateData->mSharedEventSystemState.mInternalStateFlags;
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
			_eventControllerActiveRef->onEventDispatcherDestroy( *this );
			_eventControllerActiveRef = nullptr;
		}
	}

	void EventDispatcher::setActive()
	{
		_eventControllerActiveRef->setActiveEventDispatcher( *this );
	}

	void EventDispatcher::setEventHandler( EEventBaseType pBaseType, EventHandler pHandler )
	{
		// EventBaseType enum is numbered from zero to have constant access time to its entry.
		auto baseTypeValue = static_cast<size_t>( pBaseType );
		// Check for possible violation attempt.
		if( baseTypeValue >= cxEnumEventBaseTypeCount )
		{
			ic3Throw( E_EXC_DEBUG_PLACEHOLDER );
		}
		_privateData->mHandlerMapByBaseType[baseTypeValue] = std::move( pHandler );
	}

	void EventDispatcher::setEventHandler( EEventCategory pCategory, EventHandler pHandler )
	{
		// EventCategory enum is numbered from zero to have constant access time to its entry.
		auto categoryValue = static_cast<size_t>( pCategory );
		// Check for possible violation attempt.
		if( categoryValue >= cxEnumEventCategoryCount )
		{
			ic3Throw( E_EXC_DEBUG_PLACEHOLDER );
		}
		_privateData->mHandlerMapByCategory[categoryValue] = std::move( pHandler );
	}

	void EventDispatcher::setEventHandler( EEventCodeIndex pCodeIndex, EventHandler pHandler )
	{
		// EventCodeIndex enum is numbered from zero to have constant access time to its entry.
		auto codeIndexValue = static_cast<size_t>( pCodeIndex );
		// Check for possible violation attempt.
		if( codeIndexValue >= cxEnumEventCodeIndexCount )
		{
			ic3Throw( E_EXC_DEBUG_PLACEHOLDER );
		}
		_privateData->mHandlerMapByCodeIndex[codeIndexValue] = std::move( pHandler );
	}

	void EventDispatcher::setDefaultEventHandler( EventHandler pHandler )
	{
		_privateData->mDefaultHandler = std::move( pHandler );
	}

	void EventDispatcher::resetEventHandler( EEventBaseType pBaseType )
	{
		setEventHandler( pBaseType, nullptr );
	}

	void EventDispatcher::resetEventHandler( EEventCategory pCategory )
	{
		setEventHandler( pCategory, nullptr );
	}

	void EventDispatcher::resetEventHandler( EEventCodeIndex pCodeIndex )
	{
		setEventHandler( pCodeIndex, nullptr );
	}

	void EventDispatcher::resetDefaultEventHandler()
	{
		setDefaultEventHandler( nullptr );
	}

	void EventDispatcher::setEventSystemConfigFlags( TBitmask<EEventSystemConfigFlags> pFlags, bool pSetOrUnset )
	{
		const auto configFlagsOnly = (pFlags & eEventSystemConfigMaskAll );
		ic3DebugAssert( configFlagsOnly == pFlags );
		_privateData->mCurrentEventSystemConfig.mConfigFlags.setOrUnset( pFlags, pSetOrUnset );
	}

	void EventDispatcher::setIdleProcessingMode( bool pIdle )
	{
		_privateData->mCurrentEventSystemConfig.mConfigFlags.setOrUnset( eEventSystemConfigFlagIdleProcessingModeBit, pIdle );
	}

	bool EventDispatcher::postEvent( EventObject pEvent )
	{
		if( CxDef::validateEventCode( pEvent.uCode ) )
		{
			if( _preProcessEvent( pEvent ) )
			{
				pEvent.uCommonData.mTimeStamp = PerfCounter::queryCurrentStamp();

				{
					auto codeIndexValue = static_cast<size_t>( CxDef::getEventCodeCodeIndex( pEvent.uCode ) );
					auto & eventHandler = _privateData->mHandlerMapByCodeIndex[codeIndexValue];
					if( eventHandler && eventHandler( pEvent ) )
					{
						return true;
					}
				}
				{
					auto categoryValue = static_cast<size_t>( CxDef::getEventCodeCategory( pEvent.uCode ) );
					auto & eventHandler = _privateData->mHandlerMapByCategory[categoryValue];
					if( eventHandler && eventHandler( pEvent ) )
					{
						return true;
					}
				}
				{
					auto baseTypeValue = static_cast<size_t>( CxDef::getEventCodeBaseType( pEvent.uCode ) );
					auto & eventHandler = _privateData->mHandlerMapByBaseType[baseTypeValue];
					if( eventHandler && eventHandler( pEvent ) )
					{
						return true;
					}
				}
				{
					auto & defaultHandler = _privateData->mDefaultHandler;
					if( defaultHandler && defaultHandler( pEvent ) )
					{
						return true;
					}
				}
			}
		}

		return false;
	}

	bool EventDispatcher::postEvent( event_code_value_t pEventCode )
	{
		return postEvent( EventObject( pEventCode ) );
	}

	bool EventDispatcher::postEventAppQuit()
	{
		return postEvent( eEventCodeAppActivityQuit );
	}

	bool EventDispatcher::postEventAppTerminate()
	{
		return postEvent( eEventCodeAppActivityTerminate );
	}

	bool EventDispatcher::checkEventSystemConfigFlags( TBitmask<EEventSystemConfigFlags> pFlags ) const
	{
		const auto configFlagsOnly = (pFlags & eEventSystemConfigMaskAll );
		ic3DebugAssert( configFlagsOnly == pFlags );
		return _privateData->mCurrentEventSystemConfig.mConfigFlags.isSet( configFlagsOnly );
	}

	TBitmask<EEventSystemConfigFlags> EventDispatcher::getEventSystemConfigFlags() const
	{
		return _privateData->mCurrentEventSystemConfig.mConfigFlags;
	}

	const EventSystemConfig & EventDispatcher::getEventSystemConfig() const
	{
		return _privateData->mCurrentEventSystemConfig;
	}

	bool EventDispatcher::_preProcessEvent( EventObject & pEvent )
	{
		const auto eventCategory = pEvent.category();

		auto & eventSystemSharedState = _eventControllerActiveRef->getEventSystemSharedState();

		if( eventCategory == EEventCategory::InputKeyboard )
		{
			return _preProcessEventKeyboard( pEvent.uEvtInputKeyboard, eventSystemSharedState );
		}

		if( eventCategory == EEventCategory::InputMouse )
		{
			return _preProcessEventMouse( pEvent.uEvtInputMouse, eventSystemSharedState );
		}

		return true;
	}

	bool EventDispatcher::_preProcessEventKeyboard( EvtInputKeyboard & pKeyboardEvent, EventSystemSharedState & pSharedState )
	{
		if( pKeyboardEvent.mKeyCode == EKeyCode::Unknown )
		{
			// Discard unknown key events
			return false;
		}

		if( pKeyboardEvent.mKeyAction == EKeyActionType::Press )
		{
			pSharedState.mInputKeyboardState.mKeyStateMap[pKeyboardEvent.mKeyCode] = true;
		}
		else
		{
			pSharedState.mInputKeyboardState.mKeyStateMap[pKeyboardEvent.mKeyCode] = false;
		}

		return true;
	}

	bool EventDispatcher::_preProcessEventMouse( EvtInputMouse & pMouseEvent, EventSystemSharedState & pSharedState )
	{
		return true;
	}


	void evtUpdateEventInputMouse( EvtInputMouse & pMouseEvent, EventSystemSharedState & pEventSystemSharedState )
	{
		if( pEventSystemSharedState.mInputMouseState.mLastCursorPos == CX_EVENT_MOUSE_POS_INVALID )
		{
			pEventSystemSharedState.mInputMouseState.mLastCursorPos = pMouseEvent.mCursorPos;
		}
	}

	void evtUpdateEventInputMouseButton( EvtInputMouseButton & pMouseButtonEvent, EventSystemSharedState & pEventSystemSharedState )
	{
		evtUpdateEventInputMouse( pMouseButtonEvent, pEventSystemSharedState );

		const auto & eventSystemConfig = pEventSystemSharedState.getEventSystemConfig();
		auto & inputMouseState = pEventSystemSharedState.mInputMouseState;

		if( eventSystemConfig.mConfigFlags.isSet( eEventSystemConfigFlagEnableMouseMultiClickBit ) )
		{
			if( pMouseButtonEvent.mButtonAction == EMouseButtonActionType::Click )
			{
				bool multiClickEventSet = false;

				if( inputMouseState.mLastPressButtonID == pMouseButtonEvent.mButtonID )
				{
					auto lastClickDiff = pMouseButtonEvent.mTimeStamp - inputMouseState.mLastPressTimestamp;
					auto lastClickDiffMs = PerfCounter::convertToDuration<Cppx::EDurationPeriod::Millisecond>( lastClickDiff );
					if( lastClickDiffMs <= eventSystemConfig.mMouseClickSequenceTimeoutMs )
					{
						if( inputMouseState.mCurrentMultiClickSequenceLength == 1 )
						{
							pMouseButtonEvent.mButtonAction = EMouseButtonActionType::DoubleClick;
							inputMouseState.mCurrentMultiClickSequenceLength = 2;
							multiClickEventSet = true;
						}
						else if( eventSystemConfig.mConfigFlags.isSet( eEventSystemConfigFlagEnableMouseMultiClickBit ) )
						{
							pMouseButtonEvent.mButtonAction = EMouseButtonActionType::MultiClick;
							inputMouseState.mCurrentMultiClickSequenceLength += 1;
							multiClickEventSet = true;
						}
					}
				}

				if( !multiClickEventSet )
				{
					// Multi-click has not been detected/updated. Possible reasons: different button ID,
					// click timeout (mouse button clicked too slow), multi-click support is disabled.
					inputMouseState.mLastPressButtonID = pMouseButtonEvent.mButtonID;
					inputMouseState.mCurrentMultiClickSequenceLength = 1;
				}

				inputMouseState.mLastPressTimestamp = pMouseButtonEvent.mTimeStamp;
			}
		}

		const auto buttonMask = CxDef::getMouseButtonFlagFromButtonID( pMouseButtonEvent.mButtonID );
		if( pMouseButtonEvent.mButtonAction == EMouseButtonActionType::Click )
		{
			pMouseButtonEvent.mButtonStateMask.set( buttonMask );
		}
		else if( pMouseButtonEvent.mButtonAction == EMouseButtonActionType::Release )
		{
			pMouseButtonEvent.mButtonStateMask.unset( buttonMask );
		}
	}

	void evtUpdateEventInputMouseMove( EvtInputMouseMove & pMouseMoveEvent, EventSystemSharedState & pEventSystemSharedState )
	{
		evtUpdateEventInputMouse( pMouseMoveEvent, pEventSystemSharedState );

		auto & inputMouseState = pEventSystemSharedState.mInputMouseState;

		pMouseMoveEvent.mMovementDelta = pMouseMoveEvent.mCursorPos - inputMouseState.mLastCursorPos;

		inputMouseState.mLastCursorPos = pMouseMoveEvent.mCursorPos;
	}

} // namespace Ic3::System
