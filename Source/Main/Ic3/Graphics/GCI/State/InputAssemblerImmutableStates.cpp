
#include "InputAssemblerImmutableStates.h"
#include <Ic3/Graphics/GCI/GpuDevice.h>

namespace Ic3::Graphics::GCI
{

	class IAVertexStreamImmutableStateDynamicOverride : public IAVertexStreamImmutableState
	{
	public:
		IAVertexStreamImmutableStateDynamicOverride()
		: IAVertexStreamImmutableState( GpuDevice::nullDevice(), {} )
		{}

		virtual ~IAVertexStreamImmutableStateDynamicOverride() = default;

		bool IsDynamicOverrideState() const noexcept override final
		{
			return true;
		}
	};


	IAInputLayoutImmutableState::IAInputLayoutImmutableState(
			GpuDevice & pGpuDevice,
			const IAInputLayoutStateCommonProperties & pCommonProperties )
	: GraphicsPipelineImmutableState( pGpuDevice )
	, mCommonProperties( pCommonProperties )
	{}

	IAInputLayoutImmutableState::~IAInputLayoutImmutableState() = default;

	EGraphicsPipelineImmutableStateType IAInputLayoutImmutableState::QueryStateType() const noexcept
	{
		return EGraphicsPipelineImmutableStateType::IAInputLayout;
	}


	IAVertexStreamImmutableState::IAVertexStreamImmutableState(
			GpuDevice & pGpuDevice,
			const IAVertexStreamStateCommonProperties & pCommonProperties )
	: GraphicsPipelineImmutableState( pGpuDevice )
	, mCommonProperties( pCommonProperties )
	{}

	IAVertexStreamImmutableState::~IAVertexStreamImmutableState() = default;

	EGraphicsPipelineImmutableStateType IAVertexStreamImmutableState::QueryStateType() const noexcept
	{
		return EGraphicsPipelineImmutableStateType::IAVertexStream;
	}

	const IAVertexStreamImmutableState & IAVertexStreamImmutableState::GetDynamicOverrideState()
	{
		static const IAVertexStreamImmutableStateDynamicOverride sDynamicOverrideState{};
		return sDynamicOverrideState;
	}

	namespace SMU
	{

		IAInputLayoutStateCommonProperties GetIAInputLayoutStateCommonProperties(
				const IAInputLayoutDefinition & pInputLayoutDefinition )
		{
			IAInputLayoutStateCommonProperties properties{};
			properties.activeAttributesMask = pInputLayoutDefinition.activeAttributesMask;
			properties.primitiveTopology = pInputLayoutDefinition.primitiveTopology;
			properties.activeAttributesNum = pop_count( pInputLayoutDefinition.activeAttributesMask );
			return properties;
		}

		IAVertexStreamStateCommonProperties GetIAVertexStreamStateCommonProperties(
				const IAVertexStreamDefinition & pVertexStreamDefinition )
		{
			IAVertexStreamStateCommonProperties properties{};
			return properties;
		}

	}

}
