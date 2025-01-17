
#include "GraphicsShaderState.h"
#include <Ic3/Graphics/GCI/Resources/Shader.h>

namespace Ic3::Graphics::GCI
{

	GraphicsShaderSet::GraphicsShaderSet()
	: vertexShader( commonShaderArray[eShaderStageIndexGraphicsVertex] )
	, hullShader( commonShaderArray[eShaderStageIndexGraphicsHull] )
	, domainShader( commonShaderArray[eShaderStageIndexGraphicsDomain] )
	, geometryShader( commonShaderArray[eShaderStageIndexGraphicsGeometry] )
	, pixelShader( commonShaderArray[eShaderStageIndexGraphicsPixel] )
	{}

	GraphicsShaderSet::GraphicsShaderSet( const GraphicsShaderSet & pSource )
	: commonShaderArray( pSource.commonShaderArray )
	, vertexShader( commonShaderArray[eShaderStageIndexGraphicsVertex] )
	, hullShader( commonShaderArray[eShaderStageIndexGraphicsHull] )
	, domainShader( commonShaderArray[eShaderStageIndexGraphicsDomain] )
	, geometryShader( commonShaderArray[eShaderStageIndexGraphicsGeometry] )
	, pixelShader( commonShaderArray[eShaderStageIndexGraphicsPixel] )
	{}

	GraphicsShaderSet::GraphicsShaderSet( const GraphicsShaderArray & pShaderArray )
	: GraphicsShaderSet()
	{
		SetShaders( pShaderArray );
	}

	GraphicsShaderSet & GraphicsShaderSet::operator=( const GraphicsShaderSet & pRhs )
	{
		commonShaderArray = pRhs.commonShaderArray;
		return *this;
	}

	GraphicsShaderSet & GraphicsShaderSet::operator=( const GraphicsShaderArray & pRhs )
	{
		SetShaders( pRhs );
		return *this;
	}

	Shader * GraphicsShaderSet::operator[]( size_t pIndex ) const noexcept
	{
		ic3DebugAssert( CxDef::IsShaderStageIndexValid( pIndex ) );
		return commonShaderArray[pIndex].get();
	}

	Shader * GraphicsShaderSet::operator[]( EShaderType pShaderType ) const noexcept
	{
		ic3DebugAssert( CxDef::IsShaderTypeGraphics( pShaderType ) );
		const auto stageIndex = CxDef::GetShaderStageIndex( pShaderType );
		return commonShaderArray[stageIndex].get();
	}

	cppx::bitmask<EShaderStageFlags> GraphicsShaderSet::GetActiveShaderStagesMask() const noexcept
	{
		return SMU::GetActiveShaderStagesMask( commonShaderArray );
	}

	uint32 GraphicsShaderSet::GetActiveShaderStagesNum() const noexcept
	{
		return SMU::GetActiveShaderStagesNum( commonShaderArray );
	}

	bool GraphicsShaderSet::IsEmpty() const noexcept
	{
		return !vertexShader || !pixelShader;
	}

	bool GraphicsShaderSet::ValidateShaders() const noexcept
	{
		for( uint32 stageIndex = 0; stageIndex < GCM::cxShaderGraphicsStagesNum; ++stageIndex )
		{
			if( auto shader = commonShaderArray[stageIndex] )
			{
				const auto shaderType = CxDef::GetShaderTypeFromStageIndex( stageIndex );
				if( shader->mShaderType != shaderType )
				{
					return false;
				}
			}
		}

		return true;
	}

	void GraphicsShaderSet::AddShader( ShaderHandle pShader ) noexcept
	{
		if( pShader )
		{
			const auto stageIndex = CxDef::GetShaderStageIndex( pShader->mShaderType );
			commonShaderArray[stageIndex] = pShader;
		}
	}

	void GraphicsShaderSet::SetShaders( const GraphicsShaderArray & pShaderArray ) noexcept
	{
		for( uint32 stageIndex = 0; stageIndex < GCM::cxShaderGraphicsStagesNum; ++stageIndex )
		{
			if( const auto shader = pShaderArray[stageIndex] )
			{
				const auto shaderTypeAtIndex = CxDef::GetShaderTypeFromStageIndex( stageIndex );
				if( shader->mShaderType == shaderTypeAtIndex )
				{
					commonShaderArray[stageIndex] = shader;
				}
			}
		}
	}

	void GraphicsShaderSet::ResetStage( uint32 pStageIndex ) noexcept
	{
		ic3DebugAssert( CxDef::IsShaderStageIndexValidGraphics( pStageIndex ) );
		commonShaderArray[pStageIndex] = cvNullHandle;
	}

	void GraphicsShaderSet::ResetStage( EShaderType pShaderType ) noexcept
	{
		ic3DebugAssert( CxDef::IsShaderTypeGraphics( pShaderType ) );
		const auto stageIndex = CxDef::GetShaderStageIndex( pShaderType );
		commonShaderArray[stageIndex] = cvNullHandle;
	}

	namespace SMU
	{

		cppx::bitmask<EShaderStageFlags> GetActiveShaderStagesMask( const GraphicsShaderArray & pShaderArray ) noexcept
		{
			cppx::bitmask<EShaderStageFlags> activeStagesMask = 0;

			for( uint32 stageIndex = 0; stageIndex < GCM::cxShaderGraphicsStagesNum; ++stageIndex )
			{
				if( const auto shader = pShaderArray[stageIndex] )
				{
					const auto shaderTypeAtIndex = CxDef::GetShaderTypeFromStageIndex( stageIndex );
					if( shader->mShaderType == shaderTypeAtIndex )
					{
						const auto shaderTypeBit = CxDef::makeShaderStageBit( stageIndex );
						activeStagesMask.set( shaderTypeBit );
					}
				}
			}

			return activeStagesMask;
		}

		uint32 GetActiveShaderStagesNum( const GraphicsShaderArray & pShaderArray ) noexcept
		{
			const auto activeStagesMask = GetActiveShaderStagesMask( pShaderArray );
			const auto activeStagesNum = pop_count( activeStagesMask );
			return activeStagesNum;
		}

	}

}
