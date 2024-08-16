
#include "GraphicsShaderState.h"
#include <Ic3/Graphics/GCI/Resources/Shader.h>

namespace Ic3::Graphics::GCI
{

	GraphicsShaderSet::GraphicsShaderSet()
	: commonShaderArray( _shaderArray )
	, vertexShader( _shaderArray[E_SHADER_STAGE_INDEX_GRAPHICS_VERTEX] )
	, hullShader( _shaderArray[E_SHADER_STAGE_INDEX_GRAPHICS_HULL] )
	, domainShader( _shaderArray[E_SHADER_STAGE_INDEX_GRAPHICS_DOMAIN] )
	, geometryShader( _shaderArray[E_SHADER_STAGE_INDEX_GRAPHICS_GEOMETRY] )
	, pixelShader( _shaderArray[E_SHADER_STAGE_INDEX_GRAPHICS_PIXEL] )
	{}

	GraphicsShaderSet::GraphicsShaderSet( const GraphicsShaderSet & pSource )
	: _shaderArray( pSource._shaderArray )
	, commonShaderArray( _shaderArray )
	, vertexShader( _shaderArray[E_SHADER_STAGE_INDEX_GRAPHICS_VERTEX] )
	, hullShader( _shaderArray[E_SHADER_STAGE_INDEX_GRAPHICS_HULL] )
	, domainShader( _shaderArray[E_SHADER_STAGE_INDEX_GRAPHICS_DOMAIN] )
	, geometryShader( _shaderArray[E_SHADER_STAGE_INDEX_GRAPHICS_GEOMETRY] )
	, pixelShader( _shaderArray[E_SHADER_STAGE_INDEX_GRAPHICS_PIXEL] )
	{}

	GraphicsShaderSet::GraphicsShaderSet( const GraphicsShaderArray & pShaderArray )
	: GraphicsShaderSet()
	{
		setShaders( pShaderArray );
	}

	GraphicsShaderSet & GraphicsShaderSet::operator=( const GraphicsShaderSet & pRhs )
	{
		_shaderArray = pRhs._shaderArray;
		return *this;
	}

	GraphicsShaderSet & GraphicsShaderSet::operator=( const GraphicsShaderArray & pRhs )
	{
		setShaders( pRhs );
		return *this;
	}

	Shader * GraphicsShaderSet::operator[]( size_t pIndex ) const noexcept
	{
		ic3DebugAssert( CxDef::isShaderStageIndexValid( pIndex ) );
		return commonShaderArray[pIndex].get();
	}

	Shader * GraphicsShaderSet::operator[]( EShaderType pShaderType ) const noexcept
	{
		ic3DebugAssert( CxDef::isShaderTypeGraphics( pShaderType ) );
		const auto stageIndex = CxDef::getShaderStageIndex( pShaderType );
		return commonShaderArray[stageIndex].get();
	}

	Bitmask<EShaderStageFlags> GraphicsShaderSet::getActiveShaderStagesMask() const noexcept
	{
		return smutil::getActiveShaderStagesMask( commonShaderArray );
	}

	uint32 GraphicsShaderSet::getActiveShaderStagesNum() const noexcept
	{
		return smutil::getActiveShaderStagesNum( commonShaderArray );
	}

	bool GraphicsShaderSet::empty() const noexcept
	{
		return !vertexShader || !pixelShader;
	}

	bool GraphicsShaderSet::validateShaders() const noexcept
	{
		for( uint32 stageIndex = 0; stageIndex < GCM::SHADER_GRAPHICS_STAGES_NUM; ++stageIndex )
		{
			if( auto shader = _shaderArray[stageIndex] )
			{
				const auto shaderType = CxDef::getShaderTypeFromStageIndex( stageIndex );
				if( shader->mShaderType != shaderType )
				{
					return false;
				}
			}
		}

		return true;
	}

	void GraphicsShaderSet::addShader( ShaderHandle pShader ) noexcept
	{
		if( pShader )
		{
			const auto stageIndex = CxDef::getShaderStageIndex( pShader->mShaderType );
			_shaderArray[stageIndex] = pShader;
		}
	}

	void GraphicsShaderSet::setShaders( const GraphicsShaderArray & pShaderArray ) noexcept
	{
		for( uint32 stageIndex = 0; stageIndex < GCM::SHADER_GRAPHICS_STAGES_NUM; ++stageIndex )
		{
			if( const auto shader = pShaderArray[stageIndex] )
			{
				const auto shaderTypeAtIndex = CxDef::getShaderTypeFromStageIndex( stageIndex );
				if( shader->mShaderType == shaderTypeAtIndex )
				{
					_shaderArray[stageIndex] = shader;
				}
			}
		}
	}

	void GraphicsShaderSet::resetStage( uint32 pStageIndex ) noexcept
	{
		ic3DebugAssert( CxDef::isShaderStageIndexValidGraphics( pStageIndex ) );
		_shaderArray[pStageIndex] = cvNullHandle;
	}

	void GraphicsShaderSet::resetStage( EShaderType pShaderType ) noexcept
	{
		ic3DebugAssert( CxDef::isShaderTypeGraphics( pShaderType ) );
		const auto stageIndex = CxDef::getShaderStageIndex( pShaderType );
		_shaderArray[stageIndex] = cvNullHandle;
	}

	namespace smutil
	{

		Bitmask<EShaderStageFlags> getActiveShaderStagesMask(
				const GraphicsShaderArray & pShaderArray ) noexcept
		{
			Bitmask<EShaderStageFlags> activeStagesMask = 0;

			for( uint32 stageIndex = 0; stageIndex < GCM::SHADER_GRAPHICS_STAGES_NUM; ++stageIndex )
			{
				if( const auto shader = pShaderArray[stageIndex] )
				{
					const auto shaderTypeAtIndex = CxDef::getShaderTypeFromStageIndex( stageIndex );
					if( shader->mShaderType == shaderTypeAtIndex )
					{
						const auto shaderTypeBit = CxDef::makeShaderStageBit( stageIndex );
						activeStagesMask.set( shaderTypeBit );
					}
				}
			}

			return activeStagesMask;
		}

		uint32 getActiveShaderStagesNum( const GraphicsShaderArray & pShaderArray ) noexcept
		{
			const auto activeStagesMask = getActiveShaderStagesMask( pShaderArray );
			const auto activeStagesNum = popCount( activeStagesMask );
			return activeStagesNum;
		}

	}

}
