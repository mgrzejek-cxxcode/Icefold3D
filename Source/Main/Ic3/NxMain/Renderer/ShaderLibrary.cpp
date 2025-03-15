
#include "ShaderLibrary.h"
#include <Ic3/Graphics/GCI/GPUDevice.h>
#include <Ic3/Graphics/GCI/GPUUtils.h>

namespace Ic3
{

	ShaderLibrary::ShaderLibrary( const CoreEngineState & pCES )
	: CoreEngineObject( pCES )
	{}

	ShaderLibrary::~ShaderLibrary() = default;

	GCI::ShaderHandle ShaderLibrary::GetShader( GfxObjectID pShaderID ) const noexcept
	{
		const auto shaderIter = _shaderMapByID.find( pShaderID );
		return ( shaderIter != _shaderMapByID.end() ) ? shaderIter->second : nullptr;
	}

	GCI::ShaderHandle ShaderLibrary::GetShader( const GfxObjectName & pShaderName ) const noexcept
	{
		const auto shaderIter = _shaderMapByName.find( pShaderName );
		return ( shaderIter != _shaderMapByName.end() ) ? shaderIter->second : nullptr;
	}

	uint32 ShaderLibrary::Append( const ShaderLibrary & pOtherLibrary, bool pOverwriteExisting )
	{
		uint32 addedShadersNum = 0;
		for( const auto &[shaderID, shaderHandle] : pOtherLibrary._shaderMapByID )
		{
			if( RegisterShader( shaderHandle, shaderID, pOverwriteExisting ) )
			{
				++addedShadersNum;
			}
		}
		for( const auto & [shaderName, shaderHandle] : pOtherLibrary._shaderMapByName )
		{
			if( RegisterShader( shaderHandle, shaderName, pOverwriteExisting ) )
			{
				++addedShadersNum;
			}
		}

		return addedShadersNum;
	}

	bool ShaderLibrary::RegisterShader(
			GCI::ShaderHandle pShaderObject,
			GfxObjectID pShaderID,
			bool pOverwriteExisting )
	{
		if( !Graphics::IsGfxObjectIDValid( pShaderID ) || !pShaderObject )
		{
			Ic3DebugOutputNx( "Cannot register shader: invalid ID or shader handle" );
			return false;
		}

		auto existingShaderIter = _shaderMapByID.find( pShaderID );
		if( existingShaderIter != _shaderMapByID.end() )
		{
			if( !pOverwriteExisting )
			{
				return false;
			}

			existingShaderIter->second = pShaderObject;
		}
		else
		{
			_shaderMapByID.insert( { pShaderID, pShaderObject } );
		}

		return true;
	}

	bool ShaderLibrary::RegisterShader(
			GCI::ShaderHandle pShaderObject,
			const GfxObjectName & pShaderName,
			bool pOverwriteExisting )
	{
		if( pShaderName.empty() || !pShaderObject )
		{
			Ic3DebugOutputNx( "Cannot register shader: invalid name or shader handle" );
			return false;
		}

		auto existingShaderIter = _shaderMapByName.find( pShaderName );
		if( existingShaderIter != _shaderMapByName.end() )
		{
			if( !pOverwriteExisting )
			{
				return false;
			}

			existingShaderIter->second = pShaderObject;
		}
		else
		{
			_shaderMapByName.insert( { pShaderName, pShaderObject } );
		}

		return true;
	}

	bool ShaderLibrary::RegisterShader(
			GCI::ShaderHandle pShaderObject,
			GfxObjectID pShaderID,
			const GfxObjectName & pShaderName,
			bool pOverwriteExisting )
	{
		const auto regByIDResult = RegisterShader( pShaderObject, pShaderID, pOverwriteExisting );
		const auto regByNameResult = RegisterShader( pShaderObject, pShaderName, pOverwriteExisting );

		return regByIDResult && regByNameResult;
	}

}
