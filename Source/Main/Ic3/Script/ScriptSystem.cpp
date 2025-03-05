
#include "ScriptSystem.h"
#include <Ic3/System/IO/FileManager.h>
#include <Ic3/System/SysContext.h>

namespace Ic3::Script
{

	ScriptSystem::ScriptSystem( System::SysContextHandle pSysContext )
	: mSysContext( std::move( pSysContext ) )
	, _sysFileManager( mSysContext->CreateFileManager() )
	{}

	void ScriptSystem::initialize()
	{}

	void ScriptSystem::release()
	{}

	CompiledScript ScriptSystem::compileFile( const char * pName, const char * pFileName )
	{
		CompiledScript compiledScript{};

		if( auto luaFile = _sysFileManager->OpenFile( pFileName, Ic3::System::EIOAccessMode::ReadOnly ) )
		{
			cppx::dynamic_byte_array luaScript;
			luaFile->ReadAuto( luaScript );
			compiledScript = compileScript( pName, luaScript );
		}

		return compiledScript;
	}

	CompiledScript ScriptSystem::compileScript( const char * pName, const cppx::dynamic_byte_array & pScriptSource )
	{
		CompiledScript compiledScript{};

		if( !pScriptSource.empty() )
		{
			cppx::dynamic_byte_array luaBinary;
			if( compileSource( pName, pScriptSource.data(), pScriptSource.size(), luaBinary ) )
			{
				compiledScript.name = pName;
				compiledScript.binary = std::move( luaBinary );
			}
		}

		return compiledScript;
	}

	bool ScriptSystem::executeCompiledScript( const CompiledScript & pCompiledScript )
	{
		if( pCompiledScript )
		{
			return executeCompiledScript( pCompiledScript.name.data(), pCompiledScript.binary.data(), pCompiledScript.binary.size() );
		}

		return false;
	}

}
