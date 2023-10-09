
#include "GraphicsShaderLinkageImmutableState.h"

namespace Ic3::Graphics::GCI
{

	GraphicsShaderLinkageImmutableState::GraphicsShaderLinkageImmutableState(
			GPUDevice & pGPUDevice,
			const GraphicsShaderLinkageCommonProperties & pCommonProperties )
	: GraphicsPipelineImmutableState( pGPUDevice )
	, mCommonProperties( pCommonProperties )
	{}

	GraphicsShaderLinkageImmutableState::~GraphicsShaderLinkageImmutableState() = default;

	Shader * GraphicsShaderLinkageImmutableState::getShader( size_t pIndex ) const noexcept
	{
		return nullptr;
	}

	Shader * GraphicsShaderLinkageImmutableState::getShader( EShaderType pShaderType ) const noexcept
	{
		return nullptr;
	}


	GraphicsShaderLinkageImmutableStateSeparable::GraphicsShaderLinkageImmutableStateSeparable(
			GPUDevice & pGPUDevice,
			const GraphicsShaderLinkageCommonProperties & pCommonProperties,
			const GraphicsShaderSet & pShaderSet )
	: GraphicsShaderLinkageImmutableState( pGPUDevice, pCommonProperties )
	, mShaderSet( pShaderSet )
	{}

	GraphicsShaderLinkageImmutableStateSeparable::~GraphicsShaderLinkageImmutableStateSeparable() = default;

	Shader * GraphicsShaderLinkageImmutableStateSeparable::getShader( size_t pIndex ) const noexcept
	{
		return mShaderSet[pIndex];
	}

	Shader * GraphicsShaderLinkageImmutableStateSeparable::getShader( EShaderType pShaderType ) const noexcept
	{
		return mShaderSet[pShaderType];
	}


	namespace smutil
	{

		GraphicsShaderLinkageCommonProperties getGraphicsShaderLinkageCommonPropertiesForShaderSet(
				const GraphicsShaderSet & pShaderSet ) noexcept
		{
			GraphicsShaderLinkageCommonProperties commonProperties{};
			commonProperties.activeStagesMask = pShaderSet.getActiveShaderStagesMask();
			commonProperties.activeStagesNum = pShaderSet.getActiveShaderStagesNum();
			return commonProperties;
		}

	}

}
