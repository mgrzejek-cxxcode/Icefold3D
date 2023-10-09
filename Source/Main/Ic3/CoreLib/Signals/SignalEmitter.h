
#pragma once

#ifndef __IC3_CORELIB_SIGNAL_EMITTER_H__
#define __IC3_CORELIB_SIGNAL_EMITTER_H__

#include "SignalCommon.h"

#include <list> // Used for list of event handlers
#include <unordered_map> // Used for receiver->HandlerList mapping

namespace Ic3
{

	template <typename TClass, typename TEvent>
	class EventEmitter;

	template <typename TClass, event_code_value_t tEventCode, typename... TEventArgs>
	class EventEmitter< TClass, Event<tEventCode, TEventArgs...> >
	{
	public:
		using Handler = std::function<void( TClass &, TEventArgs... )>;
		using HandlerList = std::list<Handler>;
		using HandlerMap = std::unordered_map<uintptr_t, HandlerList>;
		using HandlerRef = typename HandlerList::iterator;
		using HandlerRefList = std::list<HandlerRef>;

	public:
		explicit EventEmitter( TClass & pSourceObjectRef )
		: _sourceObjectRef( pSourceObjectRef )
		{}

		void emit( TEventArgs &&... pArgs )
		{
			for( auto & handlerRef : _handlerOrderedRefList )
			{
				( *handlerRef )( _sourceObjectRef, std::forward<TEventArgs>( pArgs )... );
			}
		}

		template <typename TRet, typename TReceiver>
		void connect( uintptr_t pRefID, std::function<TRet( TClass &, TEventArgs... )> pHandler ) const
		{
			auto & handlerList = _handlerMap[pRefID];

			auto newHandlerRef = handlerList.insert( handlerList.end(), std::move( pHandler ) );
			_handlerOrderedRefList.push_back( newHandlerRef );
		}

		template <typename TRet, typename TReceiver>
		void connect( TReceiver * pReceiver, TRet( TReceiver:: * pSlot )( TClass &, TEventArgs... ) ) const
		{
			auto refID = reinterpret_cast<uintptr_t>( pReceiver );
			auto & handlerList = _handlerMap[refID];

			auto newHandlerRef = handlerList.insert(
				handlerList.end(),
				[pReceiver, pSlot]( TClass & pSource, TEventArgs &&... pEventArgs ) -> void {
					( pReceiver->*pSlot )( pSource, std::forward<TEventArgs>( pEventArgs )... );
				} );
			_handlerOrderedRefList.push_back( newHandlerRef );
		}

		template <typename TRet, typename TReceiver>
		bool connectUnique( uintptr_t pRefID, std::function<TRet( TClass &, TEventArgs... )> pHandler ) const
		{
			auto & handlerList = _handlerMap[pRefID];
			if( !handlerList.empty() )
			{
				return false;
			}

			auto newHandlerRef = handlerList.insert( handlerList.end(), std::move( pHandler ) );
			_handlerOrderedRefList.push_back( newHandlerRef );

			return true;
		}

		template <typename TRet, typename TReceiver>
		bool connectUnique( TReceiver * pReceiver, TRet( TReceiver:: * pSlot )( TClass &, TEventArgs... ) ) const
		{
			auto refID = reinterpret_cast<uintptr_t>( pReceiver );
			auto & handlerList = _handlerMap[refID];
			if( !handlerList.empty() )
			{
				return false;
			}

			auto newHandlerRef = handlerList.insert(
				handlerList.end(),
				[pReceiver, pSlot]( TClass & pSource, TEventArgs &&... pEventArgs ) -> void {
					( pReceiver->*pSlot )( pSource, std::forward<TEventArgs>( pEventArgs )... );
				} );
			_handlerOrderedRefList.push_back( newHandlerRef );

			return true;
		}

	private:
		TClass & _sourceObjectRef;
		mutable HandlerMap _handlerMap;
		mutable HandlerRefList _handlerOrderedRefList;
	};

	// template <event_code_value_t tEventCode, typename... TEventArgs, typename TRet, typename TReceiver>
	// void eventConnect( const EventEmitter< Event<tEventCode, TEventArgs...> > & pEmitter, TRet( TReceiver:: * pSlot )( TEventArgs... ), TReceiver * pReceiver )
	// {
	// 	pEmitter.connect( pSlot, pReceiver );
	// }
	// template <event_code_value_t tEventCode, typename... TEventArgs, typename TReceiver>
	// void eventDisconnect( const EventEmitter< Event<tEventCode, TEventArgs...> > & pEmitter, TReceiver * pReceiver )
	// {
	// 	pEmitter.disconnect( pReceiver );
	// }

#define ic3AddEvent( pEventType, pVariableName ) \
	private: \
		EventEmitter<pEventType> _evt##pVariableName; \
	public: \
		const EventEmitter<pEventType> & mEvt##pVariableName = _evt##pVariableName;

#define slots

#define ic3DeclareEvents( pEvents ) \
	public: \
		struct Events \
		{ \
			pEvents \
		}; \
	private: \
		Events _events; \
	public: \
		const Events & mEvents = _events;

}

#endif // __IC3_CORELIB_SIGNAL_EMITTER_H__
