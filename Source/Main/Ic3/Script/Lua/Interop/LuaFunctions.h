
#pragma once

#ifndef __IC3_SCRIPT_LUA_FUNCTIONS_H__
#define __IC3_SCRIPT_LUA_FUNCTIONS_H__

#include "../LuaCoreAPI.h"

namespace Ic3::Script
{

	namespace LuaWrappers
	{

		template <typename... TArgs>
		struct FunctionArgs
		{
			static const size_t sArgsNum = sizeof...( TArgs );

			using SeqType = std::make_integer_sequence<int, sArgsNum>;

			template <int tArgsNum>
			struct Arg
			{
				using Type = typename std::tuple_element< sArgsNum, std::tuple<TArgs...> >::type;
			};

			/// @brief Retrieves arguments from the lua stack and returns them packed into a tuple.
			/// @param lua Lua state object. </param>
			/// @param stackHeadSize Number of elements on the stack *after* last argument. </param>
			/// @param sequence Generated integer sequence, used for proper pack expansion. </param>
			template <int... tSeq>
			static std::tuple<TArgs...> getArgs( lua_State * pLuaState,
												 int pStackHeadSize,
												 std::integer_sequence<int, tSeq...> pSequence )
			{
				return std::tuple<TArgs...>( LuaCore::getFunctionArgument<TArgs>( pLuaState, sArgsNum, tSeq - pStackHeadSize )... );
			}
		};


		template <typename TClass, typename TRet, typename... TArgs>
		struct MemberFunction
		{
			template <int... tSeq>
			static TRet execute( TRet( TClass::* pFuncPtr )( TArgs... ),
								 TClass * pInstancePtr,
								 const std::tuple<TArgs...> & pArgs,
								 std::integer_sequence<int, tSeq...> )
			{
				return ( pInstancePtr->*( pFuncPtr ) )( std::get<tSeq>( pArgs )... );
			}

			template <int... tSeq>
			static TRet execute( TRet( TClass::* pFuncPtr )( TArgs... ) const,
								 const TClass * pInstancePtr,
								 const std::tuple<TArgs...> & pArgs,
								 std::integer_sequence<int, tSeq...> )
			{
				return ( pInstancePtr->*( pFuncPtr ) )( std::get<tSeq>( pArgs )... );
			}
		};


		template <typename TRet, typename... TArgs>
		struct StaticFunction
		{
			template <int... tSeq>
			static TRet execute( TRet( *pFuncPtr )( TArgs... ),
								 const std::tuple<TArgs...>& pArgs,
								 std::integer_sequence<int, tSeq...> )
			{
				return ( pFuncPtr )( std::get<tSeq>( pArgs )... );
			}
		};

		template <typename TFunc>
		struct Function;

		template <typename TClass, typename... TArgs>
		struct Function<void( TClass::* )( TArgs... )> : public MemberFunction<TClass, void, TArgs...>
		{
			using ArgsWrapper = FunctionArgs<TArgs...>;
			using ArgsSequence = typename ArgsWrapper::SeqType;
			using BaseFunction = MemberFunction<TClass, void, TArgs...>;

			static const auto sArgsNum = ArgsWrapper::sArgsNum;

			static int call( lua_State * pLuaState, void( TClass::* pFuncPtr )( TArgs... ) )
			{
				TClass * thisPtr = LuaCore::getThisPointer<TClass>( pLuaState, sArgsNum );
				auto argsTuple = ArgsWrapper::getArgs( pLuaState, 0, ArgsSequence() );
				BaseFunction::execute( pFuncPtr, thisPtr, argsTuple, ArgsSequence() );
				return 0;
			}
		};

		template <typename TClass, typename... TArgs>
		struct Function<void( TClass::* )( TArgs... ) const> : public MemberFunction<TClass, void, TArgs...>
		{
			using ArgsWrapper = FunctionArgs<TArgs...>;
			using ArgsSequence = typename ArgsWrapper::SeqType;
			using BaseFunction = MemberFunction<TClass, void, TArgs...>;

			static const auto sArgsNum = ArgsWrapper::sArgsNum;

			static int call( lua_State * pLuaState, void( TClass::* pFuncPtr )( TArgs... ) const )
			{
				const TClass * thisPtr = LuaCore::getThisPointer<TClass>( pLuaState, sArgsNum );
				auto argsTuple = ArgsWrapper::getArgs( pLuaState, 0, ArgsSequence() );
				BaseFunction::execute( pFuncPtr, thisPtr, argsTuple, ArgsSequence() );
				return 0;
			}
		};

		template <typename TClass, typename TRet, typename... TArgs>
		struct Function<TRet( TClass::* )( TArgs... )> : public MemberFunction<TClass, TRet, TArgs...>
		{
			using ArgsWrapper = FunctionArgs<TArgs...>;
			using ArgsSequence = typename ArgsWrapper::SeqType;
			using BaseFunction = MemberFunction<TClass, TRet, TArgs...>;

			static const auto sArgsNum = ArgsWrapper::sArgsNum;

			static int call( lua_State * pLuaState, TRet( TClass::* pFuncPtr )( TArgs... ) )
			{
				TClass * thisPtr = LuaCore::getThisPointer<TClass>( pLuaState, sArgsNum );
				auto argsTuple = ArgsWrapper::getArgs( pLuaState, 0, ArgsSequence() );
				auto callResult = BaseFunction::execute( pFuncPtr, thisPtr, argsTuple, ArgsSequence() );
				LuaCore::pushValue<TRet>( pLuaState, callResult );
				return 1;
			}
		};

		template <typename TClass, typename TRet, typename... TArgs>
		struct Function<TRet( TClass::* )( TArgs... ) const> : public MemberFunction<TClass, TRet, TArgs...>
		{
			using ArgsWrapper = FunctionArgs<TArgs...>;
			using ArgsSequence = typename ArgsWrapper::SeqType;
			using BaseFunction = MemberFunction<TClass, TRet, TArgs...>;
			using NakedRetType = typename std::remove_reference<typename std::remove_cv<TRet>::type>::type;

			static const auto sArgsNum = ArgsWrapper::sArgsNum;

			static int call( lua_State * pLuaState, TRet( TClass::* pFuncPtr )( TArgs... ) const )
			{
				const TClass * thisPtr = LuaCore::getThisPointer<TClass>( pLuaState, sArgsNum );
				auto argsTuple = ArgsWrapper::getArgs( pLuaState, 0, ArgsSequence() );
				auto callResult = BaseFunction::execute( pFuncPtr, thisPtr, argsTuple, ArgsSequence() );
				LuaCore::pushValue( pLuaState, callResult );
				return 1;
			}
		};

		template <typename... TArgs>
		struct Function<void( TArgs... )> : public StaticFunction<void, TArgs...>
		{
			using ArgsWrapper = FunctionArgs<TArgs...>;
			using ArgsSequence = typename ArgsWrapper::SeqType;
			using BaseFunction = StaticFunction<void, TArgs...>;

			static const auto sArgsNum = ArgsWrapper::sArgsNum;

			static int call( lua_State * pLuaState, void( *pFuncPtr )( TArgs... ) )
			{
				auto argsTuple = ArgsWrapper::getArgs( pLuaState, 0, ArgsSequence() );
				BaseFunction::execute( pFuncPtr, argsTuple, ArgsSequence() );
				return 0;
			}
		};

		template <typename TRet, typename... TArgs>
		struct Function<TRet( TArgs... )> : public StaticFunction<TRet, TArgs...>
		{
			using ArgsWrapper = FunctionArgs<TArgs...>;
			using ArgsSequence = typename ArgsWrapper::SeqType;
			using BaseFunction = StaticFunction<TRet, TArgs...>;

					static const auto sArgsNum = ArgsWrapper::sArgsNum;

			static int call( lua_State * pLuaState, TRet( *pFuncPtr )( TArgs... ) )
			{
				auto argsTuple = ArgsWrapper::getArgs( pLuaState, 0, ArgsSequence() );
				auto callResult = BaseFunction::execute( pFuncPtr, argsTuple, ArgsSequence() );
				LuaCore::pushValue<TRet>( pLuaState, callResult );
				return 1;
			}
		};

		template <typename TFunctionType, TFunctionType tFunctionPtr>
		struct FunctionWrapper
		{
			static int proxyCall( lua_State * pLuaState )
			{
				return Function<TFunctionType>::call( pLuaState, tFunctionPtr );
			}
		};

		#define ic3ScriptFunction( pFunction ) \
			LuaWrappers::FunctionWrapper<decltype( pFunction ), pFunction>::proxyCall

	}

} // namespace Ic3::Script

#endif /* __IC3_SCRIPT_LUA_FUNCTIONS_H__ */
