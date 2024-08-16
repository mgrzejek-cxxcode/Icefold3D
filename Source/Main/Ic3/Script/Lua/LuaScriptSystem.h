
#ifndef __IC3_SCRIPT_LUA_SCRIPT_SYSTEM_H__
#define __IC3_SCRIPT_LUA_SCRIPT_SYSTEM_H__

#include "LuaPrerequisites.h"
#include "../ScriptSystem.h"

namespace Ic3::Script
{

	struct LuaContext
	{
		lua_State * luaState = nullptr;
	};

	class LuaScriptSystem : public ScriptSystem
	{
	public:
		explicit LuaScriptSystem( System::SysContextHandle pSysContext )
		: ScriptSystem( std::move( pSysContext ) )
		{}

		virtual ~LuaScriptSystem() override = default;

		virtual void initialize() override;

		virtual void release() override;

		[[nodiscard]] virtual const void * getContext() const noexcept override;

		virtual bool compileSource( const char * pName, const void * pSource, size_t pLength, Cppx::DynamicByteArray & pOutput ) noexcept override;

		virtual bool executeCompiledScript( const char * pName, const void * pBinary, size_t pLength ) noexcept override;

		virtual bool executeTextScript( const char * pName, const char * pSource, size_t pLength ) noexcept override;

		[[nodiscard]] const LuaContext * getLuaContext() const noexcept
		{
			return &_context;
		}

		void registerLib( const char * pName, const luaL_Reg * pReg );

		void registerType( const char * pLIbName, const char * pMetaName, const luaL_Reg * pLib, const luaL_Reg * pMeta);

	private:
		LuaContext _context;
	};

} // namespace Ic3::Script

#endif //__IC3_SCRIPT_LUA_SCRIPT_SYSTEM_H__
