
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
		setShaders( pShaderArray );
	}

	GraphicsShaderSet & GraphicsShaderSet::operator=( const GraphicsShaderSet & pRhs )
	{
		commonShaderArray = pRhs.commonShaderArray;
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

	TBitmask<EShaderStageFlags> GraphicsShaderSet::getActiveShaderStagesMask() const noexcept
	{
		return SMU::getActiveShaderStagesMask( commonShaderArray );
	}

	uint32 GraphicsShaderSet::getActiveShaderStagesNum() const noexcept
	{
		return SMU::getActiveShaderStagesNum( commonShaderArray );
	}

	bool GraphicsShaderSet::empty() const noexcept
	{
		return !vertexShader || !pixelShader;
	}

	bool GraphicsShaderSet::validateShaders() const noexcept
	{
		for( uint32 stageIndex = 0; stageIndex < GCM::cxShaderGraphicsStagesNum; ++stageIndex )
		{
			if( auto shader = commonShaderArray[stageIndex] )
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
			commonShaderArray[stageIndex] = pShader;
		}
	}

	void GraphicsShaderSet::setShaders( const GraphicsShaderArray & pShaderArray ) noexcept
	{
		for( uint32 stageIndex = 0; stageIndex < GCM::cxShaderGraphicsStagesNum; ++stageIndex )
		{
			if( const auto shader = pShaderArray[stageIndex] )
			{
				const auto shaderTypeAtIndex = CxDef::getShaderTypeFromStageIndex( stageIndex );
				if( shader->mShaderType == shaderTypeAtIndex )
				{
					commonShaderArray[stageIndex] = shader;
				}
			}
		}
	}

	void GraphicsShaderSet::resetStage( uint32 pStageIndex ) noexcept
	{
		ic3DebugAssert( CxDef::isShaderStageIndexValidGraphics( pStageIndex ) );
		commonShaderArray[pStageIndex] = cvNullHandle;
	}

	void GraphicsShaderSet::resetStage( EShaderType pShaderType ) noexcept
	{
		ic3DebugAssert( CxDef::isShaderTypeGraphics( pShaderType ) );
		const auto stageIndex = CxDef::getShaderStageIndex( pShaderType );
		commonShaderArray[stageIndex] = cvNullHandle;
	}

	namespace SMU
	{

		TBitmask<EShaderStageFlags> getActiveShaderStagesMask( const GraphicsShaderArray & pShaderArray ) noexcept
		{
			TBitmask<EShaderStageFlags> activeStagesMask = 0;

			for( uint32 stageIndex = 0; stageIndex < GCM::cxShaderGraphicsStagesNum; ++stageIndex )
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
