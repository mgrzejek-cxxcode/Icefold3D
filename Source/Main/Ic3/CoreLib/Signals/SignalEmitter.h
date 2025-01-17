
#pragma once

#ifndef __IC3_CORELIB_SIGNAL_EMITTER_H__
#define __IC3_CORELIB_SIGNAL_EMITTER_H__

#include "SignalCommon.h"

#include <list> // Used for list of event handlers
#include <unordered_map> // Used for receiver->HandlerList mapping

namespace Ic3
{

	template <typename TPClass, typename TPEvent>
	class EventEmitter;

	template <typename TPClass, event_code_value_t tpEventCode, typename... TPEventArgs>
	class EventEmitter< TPClass, Event<tpEventCode, TPEventArgs...> >
	{
	public:
		using Handler = std::function<void( TPClass &, TPEventArgs... )>;
		using HandlerList = std::list<Handler>;
		using HandlerMap = std::unordered_map<uintptr_t, HandlerList>;
		using HandlerRef = typename HandlerList::iterator;
		using HandlerRefList = std::list<HandlerRef>;

	public:
		explicit EventEmitter( TPClass & pSourceObjectRef )
		: _sourceObjectRef( pSourceObjectRef )
		{}

		void Emit( TPEventArgs &&... pArgs )
		{
			for( auto & handlerRef : _handlerOrderedRefList )
			{
				( *handlerRef )( _sourceObjectRef, std::forward<TPEventArgs>( pArgs )... );
			}
		}

		template <typename TRet, typename TReceiver>
		void Connect( uintptr_t pRefID, std::function<TRet( TPClass &, TPEventArgs... )> pHandler ) const
		{
			auto & handlerList = _handlerMap[pRefID];

			auto newHandlerRef = handlerList.insert( handlerList.end(), std::move( pHandler ) );
			_handlerOrderedRefList.push_back( newHandlerRef );
		}

		template <typename TRet, typename TReceiver>
		void Connect( TReceiver * pReceiver, TRet( TReceiver:: * pSlot )( TPClass &, TPEventArgs... ) ) const
		{
			auto refID = reinterpret_cast<uintptr_t>( pReceiver );
			auto & handlerList = _handlerMap[refID];

			auto newHandlerRef = handlerList.insert(
				handlerList.end(),
				[pReceiver, pSlot]( TPClass & pSource, TPEventArgs &&... pEventArgs ) -> void {
					( pReceiver->*pSlot )( pSource, std::forward<TPEventArgs>( pEventArgs )... );
				} );
			_handlerOrderedRefList.push_back( newHandlerRef );
		}

		template <typename TRet, typename TReceiver>
		bool ConnectUnique( uintptr_t pRefID, std::function<TRet( TPClass &, TPEventArgs... )> pHandler ) const
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
		bool ConnectUnique( TReceiver * pReceiver, TRet( TReceiver:: * pSlot )( TPClass &, TPEventArgs... ) ) const
		{
			auto refID = reinterpret_cast<uintptr_t>( pReceiver );
			auto & handlerList = _handlerMap[refID];
			if( !handlerList.empty() )
			{
				return false;
			}

			auto newHandlerRef = handlerList.insert(
				handlerList.end(),
				[pReceiver, pSlot]( TPClass & pSource, TPEventArgs &&... pEventArgs ) -> void {
					( pReceiver->*pSlot )( pSource, std::forward<TPEventArgs>( pEventArgs )... );
				} );
			_handlerOrderedRefList.push_back( newHandlerRef );

			return true;
		}

		template <typename TRet, typename TReceiver>
		bool Disconnect( TReceiver * pReceiver, TRet( TReceiver:: * pSlot )( TPClass &, TPEventArgs... ) ) const
		{
			return false;
		}

	private:
		HandlerList * _GetHandlersForReceiver( uintptr_t pReceiverRefID ) const
		{
			const auto handlerMapIter = _handlerMap.find( pReceiverRefID );
			return ( handlerMapIter != _handlerMap.end() ) ? &( handlerMapIter->second ) : nullptr;
		}

	private:
		TPClass & _sourceObjectRef;
		mutable HandlerMap _handlerMap;
		mutable HandlerRefList _handlerOrderedRefList;
	};

	// template <event_code_value_t tpEventCode, typename... TPEventArgs, typename TRet, typename TReceiver>
	// void eventConnect( const EventEmitter< Event<tpEventCode, TPEventArgs...> > & pEmitter, TRet( TReceiver:: * pSlot )( TPEventArgs... ), TReceiver * pReceiver )
	// {
	// 	pEmitter.connect( pSlot, pReceiver );
	// }
	// template <event_code_value_t tpEventCode, typename... TPEventArgs, typename TReceiver>
	// void eventDisconnect( const EventEmitter< Event<tpEventCode, TPEventArgs...> > & pEmitter, TReceiver * pReceiver )
	// {
	// 	pEmitter.disconnect( pReceiver );
	// }

#define Ic3AddEvent( pEventType, pVariableName ) \
	private: \
		EventEmitter<pEventType> _evt##pVariableName; \
	public: \
		const EventEmitter<pEventType> & evt##pVariableName = _evt##pVariableName;

#define slots

#define Ic3DeclareEvents( pEvents ) \
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
