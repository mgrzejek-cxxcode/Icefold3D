
#pragma once

#ifndef __Exs_ScriptSystem_ScriptSystem_H__
#define __Exs_ScriptSystem_ScriptSystem_H__

#include "CompiledScript.h"


namespace Exs
{


	class CompiledScript;


	class EXS_LIBCLASS_SCRIPTSYSTEM ScriptSystem
	{
	private:
		ScriptEngineState	_state;

	public:
		ScriptSystem();
		~ScriptSystem();

		void RunGC();
		
		CompiledScript* CreateScript(const char* name, const Byte* binary, Size_t length);
		CompiledScript* CompileScript(const char* name, const void* source, Size_t length);
		
		Result Execute(CompiledScript* script);
		Result Parse(const std::string& command, const char* name = nullptr);

		void RegisterLib(const char* name, const luaL_Reg* reg);
		void RegisterType(const char* libName, const char* metaName, const luaL_Reg* lib, const luaL_Reg* meta);
		
		const ScriptEngineState& GetEngineState() const;
	};


	inline const ScriptEngineState& ScriptSystem::GetEngineState() const
	{
		return this->_state;
	}


}


#endif /* __Exs_ScriptSystem_ScriptSystem_H__ */
