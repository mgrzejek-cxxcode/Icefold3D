
#include "ScriptSystem.h"
#include <Ic3/System/FileManager.h>
#include <Ic3/System/SysContext.h>

namespace Ic3::Script
{

	ScriptSystem::ScriptSystem( System::SysContextHandle pSysContext )
	: mSysContext( std::move( pSysContext ) )
	, _sysFileManager( mSysContext->createFileManager() )
	{}

	void ScriptSystem::initialize()
	{}

	void ScriptSystem::release()
	{}

	CompiledScript ScriptSystem::compileFile( const char * pName, const char * pFileName )
	{
		CompiledScript compiledScript{};

		if( auto luaFile = _sysFileManager->openFile( pFileName, Ic3::System::EFileOpenMode::ReadOnly ) )
		{
			Ic3::Cppx::DynamicByteArray luaScript;
			luaFile->readAuto( luaScript );
			compiledScript = compileScript( pName, luaScript );
		}

		return compiledScript;
	}

	CompiledScript ScriptSystem::compileScript( const char * pName, const Cppx::DynamicByteArray & pScriptSource )
	{
		CompiledScript compiledScript{};

		if( !pScriptSource.empty() )
		{
			Ic3::Cppx::DynamicByteArray luaBinary;
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
