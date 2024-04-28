
#ifndef __IC3_SCRIPT_LUA_PREREQUISITES_H__
#define __IC3_SCRIPT_LUA_PREREQUISITES_H__

#include "../Prerequisites.h"

extern "C"
{
#include <lua/lua.h>
#include <lua/lualib.h>
#include <lua/lauxlib.h>
}

namespace Ic3::Script
{

	using LuaCallback = int(*)( lua_State * );

	template <class T>
	struct LuaDataType
	{
		using Type = T*;
	};

#define ic3LuaDefineValueDataTypeMapping( pType, pValue ) \
	template <> \
	struct LuaDataType<pType> \
	{ \
		using Type = pValue; \
	};

#define ic3LuaDefineValueDataTypeMappingAuto( pType ) \
	template <> \
	struct LuaDataType<pType> \
	{ \
		using Type = pType; \
	};

	ic3LuaDefineValueDataTypeMappingAuto( void * );
	ic3LuaDefineValueDataTypeMappingAuto( const char * );
	ic3LuaDefineValueDataTypeMappingAuto( bool );
	ic3LuaDefineValueDataTypeMappingAuto( char );
	ic3LuaDefineValueDataTypeMappingAuto( short );
	ic3LuaDefineValueDataTypeMappingAuto( unsigned short );
	ic3LuaDefineValueDataTypeMappingAuto( int );
	ic3LuaDefineValueDataTypeMappingAuto( unsigned int );
	ic3LuaDefineValueDataTypeMappingAuto( long );
	ic3LuaDefineValueDataTypeMappingAuto( unsigned long );
	ic3LuaDefineValueDataTypeMappingAuto( long long );
	ic3LuaDefineValueDataTypeMappingAuto( unsigned long long );
	ic3LuaDefineValueDataTypeMappingAuto( float );
	ic3LuaDefineValueDataTypeMappingAuto( double );
	ic3LuaDefineValueDataTypeMappingAuto( long double );
	
	ic3LuaDefineValueDataTypeMapping( std::string, const std::string & );

	namespace LuaCore
	{

		template <class T>
		inline const char * getTypeMetatableName()
		{
			return TypeMetaInfo<T>::sQualifiedTypeName;

			static std::string metatableName{};

			if( metatableName.empty() )
			{
				metatableName = typeid(T).name();
				metatableName += "_MetaTable";
			}

			return metatableName.data();
		}

	} // namespace LuaCore

} // namespace Ic3::Script

#endif //__IC3_SCRIPT_LUA_PREREQUISITES_H__
