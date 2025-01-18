
#ifndef __IC3_SCRIPT_LUA_COMMON_H__
#define __IC3_SCRIPT_LUA_COMMON_H__

#include "LuaPrerequisites.h"

namespace Ic3::Script
{

	template <typename T>
	struct LuaArgType
	{
		using Type = T;
	};

	template <typename T>
	struct LuaArgType<T &>
	{
		using Type = TRef<T>;
	};

	template <typename T>
	struct LuaArgType<const T &>
	{
		using Type = TConstRef<T>;
	};

	template <typename T>
	struct LuaArgProxy
	{
		template <typename Y>
		static typename LuaArgType<Y>::Type get( lua_State * pLuaState, int pIndex )
		{
		}

		template <typename Y>
		static typename LuaArgType<Y>::Type get( lua_State * pLuaState, int pIndex )
		{
		}
	};

	template <typename T>
	struct LuaArgGetter;

	template <typename T>
	struct LuaArgGetter<TRef<T>>
	{
		static TRef<T> get( lua_State * pLuaState, int pIndex )
		{
		}
	};

	template <typename T>
	struct LuaArgGetter<TConstRef<T>>
	{
		static TConstRef<T> get( lua_State * pLuaState, int pIndex )
		{
		}
	};

} // namespace Ic3::Script

#endif //__IC3_SCRIPT_LUA_COMMON_H__
