
#include "LuaCoreAPI.h"

#undef ExsTraceInfo
#undef ExsTraceError
#define ExsTraceInfo(...)
#define ExsTraceError(...)

namespace Ic3::Script
{

	namespace LuaCore
	{

		void openLibrary( lua_State * pLuaState, LuaCallback pLoadCallback )
		{
			pLoadCallback( pLuaState );
			lua_pop( pLuaState, 1 );
		}

		bool checkResult( lua_State * pLuaState, int pResult )
		{
			if( pResult != LUA_OK )
			{
				printError( pLuaState, pResult );
				return false;
			}

			return true;
		}

		void printError( lua_State * pLuaState, int pResult )
		{
			size_t errorStrLength = 0;
			const char * errorStrBuffer = luaL_checklstring( pLuaState, -1, &errorStrLength );

			if( errorStrBuffer && ( errorStrLength > 0 ) )
			{
				printf("[LuaError] %s\n", errorStrBuffer );
			}
		}

		bool queryMetatable( lua_State * pLuaState, const char * pMetatableName )
		{
			const auto luaType = luaL_getmetatable( pLuaState, pMetatableName );
			if( luaType != LUA_TTABLE )
			{
				if( luaType == LUA_TNIL )
				{
					ExsTraceInfo(TRC_Script_System, "Metatable {%s} not found.", metatableName);
				}
				else
				{
					const char * typeName = lua_typename( pLuaState, luaType );
					ExsTraceInfo( TRC_Script_System, "{%s} is not a metatable. Found %s.", metatableName, typeName );
				}

				lua_pop( pLuaState, 1 );

				return false;
			}
			return true;
		}

		void typeError( lua_State * pLuaState, int pIndex, const char * pExpectedStr )
		{
			ExsTraceError(TRC_Script_System, "Type error (%s expected).", metatableName);
		}

		void * newObject( lua_State * pLuaState, size_t pSize, const char* pMetatableName, bool pBalanced )
		{
			bool metatableValid = queryMetatable( pLuaState, pMetatableName );
			if( metatableValid )
			{
				// Stack: [..., metatable]

				void * dataPtr = lua_newuserdata( pLuaState, pSize );
				// Stack: [..., metatable, userdata]

				lua_insert( pLuaState, -2) ;
				// Stack: [..., userdata, metatable]

				lua_setmetatable( pLuaState, -2 );
				// Stack: [..., userdata]

				if( pBalanced )
				{
					lua_pop( pLuaState, 1 );
					// Stack: [...]
				}

				return dataPtr;
			}

			return nullptr;
		}

		void dumpStack( lua_State * pLuaState )
		{
			//return;
			int topIndex = lua_gettop( pLuaState );
			if( topIndex == 0 )
			{
				printf( "Stack empty." );
			}
			else
			{
				printf( "Stack size = %u: ", topIndex );
			}

			for( int iElem = 1; iElem <= topIndex; ++iElem )
			{
				if( iElem > 1 )
				{
					printf( "," );
				}
				
				int elementType = lua_type( pLuaState, iElem );
				switch( elementType )
				{
				case LUA_TSTRING:
					printf( "string (\"%s\")", lua_tostring( pLuaState, iElem ) );
					break;

				case LUA_TBOOLEAN:
					printf( "bool (%s)", lua_toboolean( pLuaState, iElem ) ? "true" : "false" );
					break;

				case LUA_TNUMBER:
					printf( "number (%g)", lua_tonumber( pLuaState, iElem ) );
					break;

				default:
					printf( "%s", lua_typename( pLuaState, elementType ) );
					break;
				}
				printf("  ");
			}
			printf("\n");
		}

		void popStack( lua_State * pLuaState, int pCount )
		{
			lua_pop( pLuaState, pCount );
		}

		bool getBoolean( lua_State * pLuaState, int pIndex )
		{
			int booleanValue = lua_toboolean( pLuaState, pIndex );
			return booleanValue != 0;
		}

		lua_Integer getInteger( lua_State * pLuaState, int pIndex )
		{
			int isInteger = 0;
			const auto integerValue = lua_tointegerx( pLuaState, pIndex, &isInteger );

			if( !isInteger )
			{
				//... THROW
			}

			return integerValue;
		}


		lua_Number getFloatingPoint( lua_State * pLuaState, int pIndex )
		{
			int isNumber = 0;
			const auto numberValue = lua_tonumberx( pLuaState, pIndex, &isNumber );

			if( !isNumber )
			{
				//... THROW
			}

			return numberValue;
		}


		void * getPointer( lua_State * pLuaState, int pIndex )
		{
			void * userDataPtr = lua_touserdata( pLuaState, pIndex );

			if( !userDataPtr )
			{
				//... THROW
			}

			return userDataPtr;
		}


		const char * getString( lua_State * pLuaState, int pIndex )
		{
			const auto isNumberValue = lua_isnumber( pLuaState, pIndex );
			const auto isStringValue = lua_isstring( pLuaState, pIndex );
			const auto type = lua_type( pLuaState, pIndex );
			if( !isStringValue )
			{
				ic3DebugInterrupt();
			}

			size_t strLength = 0;
			const char * strBuffer = lua_tolstring( pLuaState, pIndex, &strLength );

			if( !strBuffer )
			{
				//... THROW
			}

			return strBuffer;
		}


		void * getUserData( lua_State * pLuaState, int pIndex, const char * pMetatableName )
		{
			void * userDataPtr = lua_touserdata( pLuaState, pIndex );

			if( userDataPtr )
			{
				if( lua_getmetatable( pLuaState, pIndex ) != 0 )
				{
					luaL_getmetatable( pLuaState, pMetatableName );
					if( !lua_rawequal( pLuaState, -1, -2 ))
					{
						ExsTraceError(TRC_Script_System, "Invalid user data: metatable mismatch.");
						userDataPtr = nullptr;
					}

					lua_pop( pLuaState, 2 );
				}
				else
				{
					ExsTraceError(TRC_Script_System, "Invalid user data: metatable missing.");
					userDataPtr = nullptr;
				}
			}

			return userDataPtr;
		}


		void pushBoolean( lua_State * pLuaState, bool pBooleanValue )
		{
			lua_pushboolean( pLuaState, pBooleanValue ? 1 : 0 );
		}


		void pushInteger( lua_State * pLuaState, lua_Integer pIntegerValue )
		{
			lua_pushinteger( pLuaState, pIntegerValue );
		}


		void pushFloatingPoint( lua_State * pLuaState, lua_Number pNumberValue )
		{
			lua_pushnumber( pLuaState, pNumberValue );
		}


		void pushPointer( lua_State * pLuaState, void * pPointer )
		{
			lua_pushlightuserdata( pLuaState, pPointer );
		}


		void pushString( lua_State * pLuaState, const char * pString, size_t pLength )
		{
			if( !pString || ( pLength == 0 ) )
			{
				lua_pushlstring( pLuaState, "", 0 );
			}
			else
			{
				lua_pushlstring( pLuaState, pString, pLength );
			}
		}


		bool pushUserData( lua_State * pLuaState, void * pUserData, const char * pMetatableName )
		{
			bool metatableValid = queryMetatable( pLuaState, pMetatableName );
			if( metatableValid )
			{
				// Stack: [..., metatable]

				lua_pushlightuserdata( pLuaState, pUserData );
				// Stack: [..., metatable, userdata]

				lua_insert( pLuaState, -2 );
				// Stack: [..., userdata, metatable]

				lua_setmetatable( pLuaState, -2 );
				// Stack: [..., userdata]

				//luaL_ref( pLuaState, 0 );
				return true;
			}

			lua_pushnil( pLuaState );

			return false;
		}

	} // namespace LuaCore

} // namespace Ic3::Script
