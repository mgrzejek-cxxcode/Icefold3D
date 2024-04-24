
#pragma once

#ifndef __IC3_SCRIPT_LUA_CLASS_H__
#define __IC3_SCRIPT_LUA_CLASS_H__

#include "LuaFunctions.h"

namespace Ic3::Script
{

	namespace LuaWrappers
	{

		template <typename TClass>
		inline void * allocNewObject( lua_State * pLuaState )
		{
			const size_t allocSize = sizeof( TClass );
			const char * metatableName = LuaCore::getTypeMetatableName<TClass>();
			void * objectPtr = LuaCore::newObject( pLuaState, allocSize, metatableName );
			return objectPtr;
		}

		template <class TCtor>
		struct Constructor;

		template <typename TClass, typename TRet, typename... TArgs>
		struct Constructor<TRet *( TClass *, TArgs... )>
		{
			using ArgsWrapper = FunctionArgs<TArgs...>;
			using ArgsSequence = typename ArgsWrapper::SeqType;

			static const auto sArgsNum = ArgsWrapper::sArgsNum;

			template <int... tSeq>
			static TRet * execute( TRet *( *pFuncPtr )( TClass *, TArgs... ),
								   TClass * pObjectPtr,
								   const std::tuple<TArgs...> & pArgs,
								   std::integer_sequence<int, tSeq...> )
			{
				return ( pFuncPtr )( pObjectPtr, std::get<tSeq>( pArgs )... );
			}

			static void construct( lua_State * pLuaState, TRet *( *pFuncPtr )( void *, TArgs... ) )
			{
				void * memPtr = allocNewObject<TRet>( pLuaState );
				auto * objectPtr = reinterpret_cast<TRet *>( memPtr );
				auto argsTuple = ArgsWrapper::getArgs( pLuaState, 0, ArgsSequence() );
				execute( pFuncPtr, objectPtr, argsTuple, ArgsSequence() );
				LuaCore::pushValue<TRet>( pLuaState, objectPtr );
			}
		};

		template <class TDtor>
		struct Destructor;

		template <>
		struct Destructor<void( void * )>
		{
			using ArgsWrapper = FunctionArgs<void *>;
			using ArgsSequence = typename ArgsWrapper::SeqType;

			static const auto sArgsNum = ArgsWrapper::sArgsNum;

			static void execute( void( *pFuncPtr )( void * ), const std::tuple<void *> & pArgs)
			{
				return ( pFuncPtr )( std::get<0>( pArgs ) );
			}

			static void destroy( lua_State * pLuaState, void( *pFuncPtr )( void * ) )
			{
				auto argsTuple = ArgsWrapper::getArgs( pLuaState, 0, ArgsSequence() );
				execute( pFuncPtr, argsTuple );
			}
		};

		template <class TConstructorType, TConstructorType tConstructorPtr>
		struct ConstructorWrapper
		{
			static int proxyCall( lua_State * pLuaState )
			{
				Constructor<TConstructorType>::construct( pLuaState, tConstructorPtr );
				return 1;
			}
		};


		template <class TDestructorType, TDestructorType tDestructorPtr>
		struct DestructorWrapper
		{
			static int proxyCall( lua_State * pLuaState )
			{
				Destructor<TDestructorType>::destroy( pLuaState, tDestructorPtr );
				return 1;
			}
		};

#define ic3ScriptConstructor( pConstructor ) \
	LuaWrappers::ConstructorWrapper<decltype( pConstructor ), pConstructor>::proxyCall

#define ic3ScriptDestructor( pDestructor ) \
	LuaWrappers::DestructorWrapper<decltype( pDestructor ), pDestructor>::proxyCall

	}

} // namespace Ic3::Script

#endif /* __IC3_SCRIPT_LUA_CLASS_H__ */
