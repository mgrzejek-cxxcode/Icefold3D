
#pragma once

#ifndef __IC3_SCRIPT_LUA_VALUES_H__
#define __IC3_SCRIPT_LUA_VALUES_H__

#include "../LuaCoreAPI.h"

namespace Ic3::Script
{

	namespace LuaWrappers
	{

		template <typename T>
		struct PushObjectProxy
		{
			static void push( T && pObject )
			{
				// temporary object - create a new, full user-data in lua, initialize with this copy
				// Lua takes ownership of the object lifetime control
			}

			static void push( T & pObject )
			{
				// lvalue object - create a new light user-value in lua, pass this pointer
				// Lua takes ownership of the object lifetime control
			}

			static void push( const T & pObject )
			{
				// lvalue object - create a new light user-value in lua, pass this pointer
				// Lua takes ownership of the object lifetime control
			}
		};

		template <typename T>
		struct PushValueProxy
		{
			static void push( T pValue )
			{
				// for built-in types, vectors, raw pointers
			}
		};

	}

} // namespace Ic3::Script

#endif /* __IC3_SCRIPT_LUA_VALUES_H__ */
