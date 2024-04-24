
#include "RefData.h"
#include "LuaScriptSystem.h"
#include "Interop/LuaClass.h"
#include "Interop/LuaVariables.h"

namespace Ic3::Script
{

	int RefData::_cnt = 0;

	RefData::RefData( const char * pName )
	: name( pName )
	, uid( ( ptrdiff_t )pName )
	{
		++_cnt;
	}

	RefData::~RefData()
	{
		--_cnt;
		printf("RefData<%s> --> destroy\n", this->name.data());
	}

	void RefData::add( const char * pStr )
	{
		this->_data.emplace_back( pStr );
	}


	void RefData::addTwo( const char * pStr1, const char * pStr2 )
	{
		this->_data.emplace_back( pStr1 );
		this->_data.emplace_back( pStr2 );
	}


	void RefData::print()
	{
		printf("[RefData object]\n");
		for( size_t n = 0; n < this->_data.size(); ++n )
		{
			auto & str = this->_data[n];
			printf("%u: %s\n", ( uint32_t )n, str.data() );
		}
	}

	RefData* NewRefData(void* mem, const char* name)
	{
		printf("[NewRefData]\n");
		new (mem) RefData(name);
		return (RefData*)mem;
	}

	void DeleteRefData(void* mem)
	{
		printf("[DeleteRefData]\n");
		auto* rd = (RefData*)mem;
		rd->~RefData();
	}

	int RDIndexMeta(lua_State* lua)
	{
		//Stack: userdata, index

		lua_pushvalue(lua, lua_upvalueindex(1));
		const char* meta_name = lua_tostring(lua, -1);
		lua_pop(lua, 1);
		luaL_getmetatable(lua, meta_name);
		
		//Stack: userdata, index, metatable

		lua_pushliteral(lua, "__propertyget");
		lua_rawget(lua, -2);
		
		//Stack: userdata, index, metatable, propertyget_table

		lua_pushvalue(lua, -3); // Duplicate index
		lua_rawget(lua, -2); // Get 'index' from 'propertyget_table'

		if(!lua_iscfunction(lua, -1))
		{
			//Stack: userdata, index, metatable, propertyget_table, nil
			LuaCore::dumpStack(lua);
			
			lua_pop(lua, 2); // Pop nil and propertyget_table
			lua_pushliteral(lua, "__memberfunctions");
			lua_rawget(lua, -2);
		
			//Stack: userdata, index, metatable, memberfunctions_table
			lua_pushvalue(lua, -3); // Duplicate index
			lua_rawget(lua, -2); // Get 'index' from 'memberfunctions_table'

			if(lua_isnil(lua, -1))
			{
				//ExsTraceError(TRC_Script_System, "Index_Error");
			}

			lua_insert(lua, -3);
			lua_pop(lua, 2);

			return 1;
		}
		
		//Stack: userdata, index, metatable, propertyget_table, function
		LuaCore::dumpStack(lua);

		lua_CFunction func = lua_tocfunction(lua, -1);
		lua_pop(lua, 3); // pop function, propertyget_table, metatable
		lua_pushvalue(lua, -2);

		int result = func(lua);
		
		lua_insert(lua, -2);
		lua_pop(lua, 1);
		LuaCore::dumpStack(lua);

		return result;
	}

	int RDNewIndexMeta(lua_State* lua)
	{
		//Stack: userdata, index, value

		lua_pushvalue(lua, lua_upvalueindex(1));
		const char* meta_name = lua_tostring(lua, -1);
		lua_pop(lua, 1);
		luaL_getmetatable(lua, meta_name);
		
		//Stack: userdata, index, value, metatable

		lua_pushliteral(lua, "__propertyset");
		lua_rawget(lua, -2);
		
		//Stack: userdata, index, value, metatable, propertyset_table

		lua_pushvalue(lua, -4); // Duplicate index
		lua_rawget(lua, -2); // Get 'index' from 'propertyget_table'
		
		if(!lua_iscfunction(lua, -1))
		{
			//ExsTraceError(TRC_Script_System, "Index_Error");
			return 1;
		}

		LuaCore::dumpStack(lua);

		lua_CFunction func = lua_tocfunction(lua, -1);

		lua_pop(lua, 3); // pop function, propertyset_table, metatable
		lua_pushvalue(lua, -3); // Push userdata
		lua_pushvalue(lua, -2); // Push value

		int result = func(lua);

		LuaCore::dumpStack(lua);
		lua_pop(lua, 2);

		return result;
	}

	void RefData::registerType( ScriptSystem * pScriptSystem )
	{
		const luaL_Reg reg_spec_fstatic[] =
		{
			{ "new", ic3ScriptConstructor( NewRefData ) },
			{ "getCnt", ic3ScriptFunction( RefData::getCnt ) },
			{ nullptr, nullptr }
		};

		const luaL_Reg reg_spec_fmember[] =
		{
			{ "add", ic3ScriptFunction( &RefData::add ) },
			{ "addTwo", ic3ScriptFunction( &RefData::addTwo ) },
			{ "print", ic3ScriptFunction( &RefData::print ) },
			{ "getSize", ic3ScriptFunction( &RefData::getSize ) },
			{ "getName", ic3ScriptFunction( &RefData::getName ) },
			{ nullptr, nullptr }
		};

		const luaL_Reg reg_spec_vget[] =
		{
			//{ "name", ic3ScriptMemberVariableGetter(&RefData::name) },
			{ "uid", ic3ScriptMemberVariableGetter( &RefData::uid ) },
			{ "value", ic3ScriptMemberVariableGetter( &RefData::value ) },
			{ "multiplier", ic3ScriptMemberVariableGetter( &RefData::multiplier ) },
			{ nullptr, nullptr }
		};

		const luaL_Reg reg_spec_vset[] =
		{
			{ "value", ic3ScriptMemberVariableSetter(&RefData::value) },
			{ "multiplier", ic3ScriptMemberVariableSetter(&RefData::multiplier) },
			{ nullptr, nullptr }
		};

		const luaL_Reg reg_meta[] =
		{
			{ "__gc", ic3ScriptDestructor(DeleteRefData) },
			{ nullptr, nullptr }
		};

		auto * luaCtx = ( ( LuaScriptSystem * )pScriptSystem )->getContextAs<LuaContext>();

		auto * lua_state = luaCtx->luaState;
		const char * meta_name = LuaCore::getTypeMetatableName<RefData>();

		lua_newtable(lua_state);
		luaL_setfuncs(lua_state, reg_spec_fstatic, 0);
		int fstatic_table = lua_gettop(lua_state);

		LuaCore::dumpStack(lua_state);

		lua_newtable(lua_state);
		lua_pushstring(lua_state, "None");
		lua_pushinteger(lua_state, (int)RefData::Color::None);
		lua_settable(lua_state, -3);
		lua_pushstring(lua_state, "White");
		lua_pushinteger(lua_state, (int)RefData::Color::White);
		lua_settable(lua_state, -3);
		lua_pushstring(lua_state, "Black");
		lua_pushinteger(lua_state, (int)RefData::Color::Black);
		lua_settable(lua_state, -3);
		lua_pushstring(lua_state, "Green");
		lua_pushinteger(lua_state, (int)RefData::Color::Green);
		lua_settable(lua_state, -3);
		
		lua_pushstring(lua_state, "Color");
		lua_insert(lua_state, -2);
		lua_settable(lua_state, -3);

		LuaCore::dumpStack(lua_state);

		luaL_newmetatable(lua_state, meta_name);
		luaL_setfuncs(lua_state, reg_meta, 0);
		int fmeta_table = lua_gettop(lua_state);
		
		lua_pushliteral(lua_state, "__metatable");
		lua_pushliteral(lua_state, "unavailable");
		lua_rawset(lua_state, fmeta_table); // fmeta_table.__metatable = "unavailable"

		lua_pushliteral(lua_state, "__memberfunctions");
		lua_newtable(lua_state);
		luaL_setfuncs(lua_state, reg_spec_fmember, 0);
		lua_settable(lua_state, -3); // fmeta_table.__memberfunctions = reg_spec_fmember
		
		lua_pushstring(lua_state, "__propertyget");
		lua_newtable(lua_state);
		luaL_setfuncs(lua_state, reg_spec_vget, 0);
		lua_settable(lua_state, -3); // fmeta_table.__propertyget = reg_spec_vget
		
		lua_pushstring(lua_state, "__propertyset");
		lua_newtable(lua_state);
		luaL_setfuncs(lua_state, reg_spec_vset, 0);
		lua_settable(lua_state, -3); // fmeta_table.__propertyset = reg_spec_vset

		lua_pushliteral(lua_state, "__index");
		lua_pushstring(lua_state, meta_name);
		lua_pushcclosure(lua_state, RDIndexMeta, 1);
		lua_rawset(lua_state, fmeta_table);
		LuaCore::dumpStack(lua_state);

		lua_pushliteral(lua_state, "__newindex");
		lua_pushstring(lua_state, meta_name);
		lua_pushcclosure(lua_state, RDNewIndexMeta, 1);
		lua_rawset(lua_state, fmeta_table);
		LuaCore::dumpStack(lua_state);
		
		lua_pushvalue(lua_state, fstatic_table);
		lua_setglobal(lua_state, "RefData");
	}


}
