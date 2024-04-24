
#pragma once

#ifndef __IC3_SCRIPT_LUA_VARIABLES_H__
#define __IC3_SCRIPT_LUA_VARIABLES_H__

#include "../LuaCoreAPI.h"

namespace Ic3::Script
{

	namespace LuaWrappers
	{

		template <typename TVariableType>
		struct MemberVariable;


		template <class TClass, class TValue>
		struct MemberVariable<TValue TClass::*>
		{
			static void get( lua_State * pLuaState, TValue TClass::* pVariablePtr )
			{
				// Stack: [..., userdata]

				TClass * thisPtr = LuaCore::getThisPointer<TClass>( pLuaState, 0 );
				auto value = thisPtr->*( pVariablePtr );
				LuaCore::pushValue<TValue>( pLuaState, value );
			}

			static void set( lua_State * pLuaState, TValue TClass::* pVariablePtr )
			{
				// Stack: [..., userdata, value]

				TClass * thisPtr = LuaCore::getThisPointer<TClass>( pLuaState, 1 );
				const auto newValue = LuaCore::getValue<TValue>( pLuaState, -1 );
				thisPtr->*( pVariablePtr ) = newValue;
			}
		};

		template <typename TVariableType, TVariableType tVariablePtr>
		struct MemberVariableGetterWrapper
		{
			static int proxyCall( lua_State * pLuaState )
			{
				MemberVariable<TVariableType>::get( pLuaState, tVariablePtr );
				return 1;
			}
		};


		template <typename TVariableType, TVariableType tVariablePtr>
		struct MemberVariableSetterWrapper
		{
			static int proxyCall( lua_State * pLuaState )
			{
				MemberVariable<TVariableType>::set( pLuaState, tVariablePtr );
				return 0;
			}
		};


		#define ic3ScriptMemberVariableGetter( pVariable ) \
			LuaWrappers::MemberVariableGetterWrapper<decltype( pVariable ), pVariable>::proxyCall

		#define ic3ScriptMemberVariableSetter( pVariable ) \
			LuaWrappers::MemberVariableSetterWrapper<decltype( pVariable ), pVariable>::proxyCall

	}

} // namespace Ic3::Script

#endif /* __IC3_SCRIPT_LUA_VARIABLES_H__ */
