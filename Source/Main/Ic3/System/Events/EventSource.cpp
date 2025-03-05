
#include "EventSource.h"
#include "EventCore.h"

namespace Ic3::System
{

	EventSource::EventSource( SysContextHandle pSysContext )
	: SysObject( std::move( pSysContext ) )
	{}

	EventSource::~EventSource() noexcept
	{
		if( _eventControllerActiveRef )
		{
			_eventControllerActiveRef->OnEventSourceDestroy( *this );
			_eventControllerActiveRef = nullptr;
		}
	}

	void * EventSource::GetEventSourceNativeData() const
	{
		return _eventSourceNativeData;
	}

    EventController * EventSource::GetEventController() const
    {
        return _eventControllerActiveRef.get();
    }

    EventController & EventSource::GetEventControllerRef() const
    {
        Ic3DebugAssert( _eventControllerActiveRef );
        return *_eventControllerActiveRef;
    }

	bool EventSource::IsPrimaryEventSource() const
	{
		return _eventControllerActiveRef && ( this == _eventControllerActiveRef->GetPrimaryEventSource() );
	}

	bool EventSource::IsLastEventSource() const
	{
		if( _eventControllerActiveRef && ( _eventControllerActiveRef->GetRegisteredEventSourcesNum() == 1 ) )
		{
			// EventController has one event source registered. They are indexed starting from 0 - get that last one.
			auto * lastRegEventSource = _eventControllerActiveRef->GetRegisteredEventSourceByIndex( 0 );

			if( this == lastRegEventSource )
			{
				return true;
			}

			// This is veeery weird. If this source has a controller set (which means it is a registered event source),
			// it definitely should be present on that controller's event source list. Investigate if this gets triggered.
			Ic3DebugInterrupt();
		}

		return false;
	}

	void EventSource::OnDestroySystemObjectRequested()
	{
		if( _eventControllerActiveRef )
		{
			_eventControllerActiveRef->OnEventSourceDestroy( *this );
			_eventControllerActiveRef = nullptr;
		}
	}

	void EventSource::SetEventController( EventControllerHandle pEventController )
	{
		_eventControllerActiveRef = std::move( pEventController );
	}

	void EventSource::SetEventSourceNativeData( void * pNativeData )
	{
		_eventSourceNativeData = pNativeData;
	}

	void EventSource::ResetEventSourceNativeData()
	{
		_eventSourceNativeData = nullptr;
	}

} // namespace Ic3::System
