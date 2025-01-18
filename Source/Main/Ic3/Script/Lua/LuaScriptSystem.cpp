
#include "LuaScriptSystem.h"
#include "LuaCoreAPI.h"

namespace Ic3::Script
{

	void LuaScriptSystem::initialize()
	{
		if( !_context.luaState )
		{
			auto * luaState = luaL_newstate();
			LuaCore::openLibrary( luaState, luaopen_base );
			LuaCore::openLibrary( luaState, luaopen_debug);
			LuaCore::openLibrary( luaState, luaopen_io);
			LuaCore::openLibrary( luaState, luaopen_math);
			LuaCore::openLibrary( luaState, luaopen_os);
			LuaCore::openLibrary( luaState, luaopen_package);
			LuaCore::openLibrary( luaState, luaopen_string);
			LuaCore::openLibrary( luaState, luaopen_table);
			LuaCore::openLibrary( luaState, luaopen_utf8);

			_context.luaState = luaState;
		}
	}

	void LuaScriptSystem::release()
	{
		if( _context.luaState )
		{
			lua_close( _context.luaState );
			_context.luaState = nullptr;
		}
	}

	const void * LuaScriptSystem::getContext() const noexcept
	{
		return &_context;
	}

	int luaDumpCodeWriter( lua_State * pLuaState, const void * pData, size_t pLength, void * pArg )
	{
		auto * targetBuffer = reinterpret_cast<cppx::dynamic_byte_array *>( pArg );
		targetBuffer->resize( pLength );
		targetBuffer->setData( pData, pLength, 0 );

		return 0;
	}

	bool LuaScriptSystem::compileSource( const char * pName, const void * pSource, size_t pLength, cppx::dynamic_byte_array & pOutput ) noexcept
	{
		const auto loadResult = luaL_loadbufferx( _context.luaState, reinterpret_cast<const char *>( pSource ), pLength, pName, "t" );
		if( !LuaCore::checkResult( _context.luaState, loadResult ) )
		{
			return false;
		}

		lua_dump( _context.luaState, luaDumpCodeWriter, &pOutput, 0 );
		lua_pop( _context.luaState, 1 );

		return true;
	}

	bool LuaScriptSystem::executeCompiledScript( const char * pName, const void * pBinary, size_t pLength ) noexcept
	{
		const auto loadResult = luaL_loadbufferx( _context.luaState, reinterpret_cast<const char*>( pBinary ), pLength, pName, "t" );
		if( !LuaCore::checkResult( _context.luaState, loadResult ) )
		{
			return false;
		}

		const auto callResult = lua_pcallk( _context.luaState, 0, 0, 0, 0, nullptr );
		if( !LuaCore::checkResult( _context.luaState, callResult ) )
		{
			return false;
		}

		return true;
	}

	bool LuaScriptSystem::executeTextScript( const char * pName, const char * pSource, size_t pLength ) noexcept
	{
		const auto loadResult = luaL_loadbufferx( _context.luaState, pSource, pLength, pName, "t" );
		if( !LuaCore::checkResult( _context.luaState, loadResult ) )
		{
			return false;
		}

		const auto callResult = lua_pcallk( _context.luaState, 0, 0, 0, 0, nullptr );
		if( !LuaCore::checkResult( _context.luaState, callResult ) )
		{
			return false;
		}

		return true;
	}

	void LuaScriptSystem::registerLib( const char * pName, const luaL_Reg * pReg )
	{
		luaL_newlib( _context.luaState, pReg );
		lua_setglobal( _context.luaState, pName );
	}

	void LuaScriptSystem::registerType( const char * pLibName, const char * pMetaName, const luaL_Reg * pLib, const luaL_Reg * pMeta)
	{
		luaL_newmetatable( _context.luaState, pMetaName );

		lua_pushvalue(_context.luaState, -1 );
		lua_setfield( _context.luaState, -2, "__index" );

		luaL_setfuncs( _context.luaState, pMeta, 0 );
		luaL_newlib( _context.luaState, pLib );

		lua_setglobal( _context.luaState, pLibName);
	}

}
