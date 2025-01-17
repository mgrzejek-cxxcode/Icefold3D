
#include "GraphicsShaderLinkageImmutableState.h"

namespace Ic3::Graphics::GCI
{

	GraphicsShaderLinkageImmutableState::GraphicsShaderLinkageImmutableState(
			GpuDevice & pGpuDevice,
			const GraphicsShaderLinkageCommonProperties & pCommonProperties )
	: GraphicsPipelineImmutableState( pGpuDevice )
	, mCommonProperties( pCommonProperties )
	{}

	GraphicsShaderLinkageImmutableState::~GraphicsShaderLinkageImmutableState() = default;

	Shader * GraphicsShaderLinkageImmutableState::GetShader( size_t pIndex ) const noexcept
	{
		return nullptr;
	}

	Shader * GraphicsShaderLinkageImmutableState::GetShader( EShaderType pShaderType ) const noexcept
	{
		return nullptr;
	}


	GraphicsShaderLinkageImmutableStateSeparable::GraphicsShaderLinkageImmutableStateSeparable(
			GpuDevice & pGpuDevice,
			const GraphicsShaderLinkageCommonProperties & pCommonProperties,
			const GraphicsShaderSet & pShaderSet )
	: GraphicsShaderLinkageImmutableState( pGpuDevice, pCommonProperties )
	, mShaderSet( pShaderSet )
	{}

	GraphicsShaderLinkageImmutableStateSeparable::~GraphicsShaderLinkageImmutableStateSeparable() = default;

	Shader * GraphicsShaderLinkageImmutableStateSeparable::GetShader( size_t pIndex ) const noexcept
	{
		return mShaderSet[pIndex];
	}

	Shader * GraphicsShaderLinkageImmutableStateSeparable::GetShader( EShaderType pShaderType ) const noexcept
	{
		return mShaderSet[pShaderType];
	}


	namespace SMU
	{

		GraphicsShaderLinkageCommonProperties GetGraphicsShaderLinkageCommonPropertiesForShaderSet(
				const GraphicsShaderSet & pShaderSet ) noexcept
		{
			GraphicsShaderLinkageCommonProperties commonProperties{};
			commonProperties.mActiveStagesMask = pShaderSet.GetActiveShaderStagesMask();
			commonProperties.mActiveStagesNum = pShaderSet.GetActiveShaderStagesNum();
			return commonProperties;
		}

	}

}
