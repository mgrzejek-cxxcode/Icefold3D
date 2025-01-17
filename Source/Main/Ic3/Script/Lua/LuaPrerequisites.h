
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

#define Ic3LuaDefineValueDataTypeMapping( pType, pValue ) \
	template <> \
	struct LuaDataType<pType> \
	{ \
		using Type = pValue; \
	};

#define Ic3LuaDefineValueDataTypeMappingAuto( pType ) \
	template <> \
	struct LuaDataType<pType> \
	{ \
		using Type = pType; \
	};

	Ic3LuaDefineValueDataTypeMappingAuto( void * );
	Ic3LuaDefineValueDataTypeMappingAuto( const char * );
	Ic3LuaDefineValueDataTypeMappingAuto( bool );
	Ic3LuaDefineValueDataTypeMappingAuto( char );
	Ic3LuaDefineValueDataTypeMappingAuto( short );
	Ic3LuaDefineValueDataTypeMappingAuto( unsigned short );
	Ic3LuaDefineValueDataTypeMappingAuto( int );
	Ic3LuaDefineValueDataTypeMappingAuto( unsigned int );
	Ic3LuaDefineValueDataTypeMappingAuto( long );
	Ic3LuaDefineValueDataTypeMappingAuto( unsigned long );
	Ic3LuaDefineValueDataTypeMappingAuto( long long );
	Ic3LuaDefineValueDataTypeMappingAuto( unsigned long long );
	Ic3LuaDefineValueDataTypeMappingAuto( float );
	Ic3LuaDefineValueDataTypeMappingAuto( double );
	Ic3LuaDefineValueDataTypeMappingAuto( long double );
	
	Ic3LuaDefineValueDataTypeMapping( std::string, const std::string & );

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
