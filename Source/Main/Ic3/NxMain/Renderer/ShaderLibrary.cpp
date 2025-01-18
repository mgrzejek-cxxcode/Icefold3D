
#include "ShaderLibrary.h"
#include <Ic3/Graphics/GCI/GPUDevice.h>
#include <Ic3/Graphics/GCI/GPUUtils.h>

namespace Ic3
{

	ShaderLibrary::ShaderLibrary( const CoreEngineState & pCES )
	: CoreEngineObject( pCES )
	{}

	ShaderLibrary::~ShaderLibrary() = default;

	GCI::ShaderHandle ShaderLibrary::getShader( GfxObjectID pShaderID ) const noexcept
	{
		const auto shaderIter = _shaderMap.find( pShaderID );
		return ( shaderIter != _shaderMap.end() ) ? shaderIter->second : nullptr;
	}

	GCI::ShaderHandle ShaderLibrary::getShader( const GfxObjectName & pShaderName ) const noexcept
	{
		const auto uniqueShaderID = Graphics::GenerateGfxObjectID( pShaderName );
		return getShader( uniqueShaderID );
	}

	uint32 ShaderLibrary::append( const ShaderLibrary & pOtherLibrary )
	{
		uint32 addedShadersNum = 0;

		for( const auto & shaderDef : pOtherLibrary._shaderMap )
		{
			if( registerShader( shaderDef.first, shaderDef.second ) )
			{
				++addedShadersNum;
			}
		}

		return addedShadersNum;
	}

	bool ShaderLibrary::registerShader( GfxObjectID pShaderID, GCI::ShaderHandle pShaderObject )
	{
		if( !pShaderID || !pShaderObject )
		{
			Ic3DebugOutput( "Cannot register shader: empty name or shader handle" );
			return false;
		}

		auto existingShaderIter = _shaderMap.find( pShaderID );
		if( existingShaderIter != _shaderMap.end() )
		{
			return false;
		}

		_shaderMap.insert( { pShaderID, pShaderObject } );

		return true;
	}

	bool ShaderLibrary::registerShader( const GfxObjectName & pShaderName, GCI::ShaderHandle pShaderObject )
	{
		const auto uniqueShaderID = Graphics::GenerateGfxObjectID( pShaderName );
		return registerShader( uniqueShaderID, pShaderObject );
	}

}
