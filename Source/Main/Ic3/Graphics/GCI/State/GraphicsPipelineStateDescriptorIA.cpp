
#include "GraphicsPipelineStateDescriptorIA.h"
#include <Ic3/Graphics/GCI/GPUDevice.h>

namespace Ic3::Graphics::GCI
{

	class IAVertexStreamStateDescriptorDynamicOverride : public IAVertexStreamStateDescriptor
	{
	public:
		IAVertexStreamStateDescriptorDynamicOverride()
		: IAVertexStreamStateDescriptor( GPUDevice::GetNullDevice(), {} )
		{}

		virtual ~IAVertexStreamStateDescriptorDynamicOverride() = default;

		bool IsDynamicDescriptor() const noexcept override final
		{
			return true;
		}
	};


	IAInputLayoutStateDescriptor::IAInputLayoutStateDescriptor(
			GPUDevice & pGPUDevice,
			const IAInputLayoutStateCommonProperties & pCommonProperties )
	: GraphicsPipelineStateDescriptor( pGPUDevice )
	, mCommonProperties( pCommonProperties )
	{}

	IAInputLayoutStateDescriptor::~IAInputLayoutStateDescriptor() = default;

	EGraphicsPipelineStateDescriptorType IAInputLayoutStateDescriptor::QueryStateType() const noexcept
	{
		return EGraphicsPipelineStateDescriptorType::IAInputLayout;
	}


	IAVertexStreamStateDescriptor::IAVertexStreamStateDescriptor(
			GPUDevice & pGPUDevice,
			const IAVertexStreamStateCommonProperties & pCommonProperties )
	: GraphicsPipelineStateDescriptor( pGPUDevice )
	, mCommonProperties( pCommonProperties )
	{}

	IAVertexStreamStateDescriptor::~IAVertexStreamStateDescriptor() = default;

	EGraphicsPipelineStateDescriptorType IAVertexStreamStateDescriptor::QueryStateType() const noexcept
	{
		return EGraphicsPipelineStateDescriptorType::IAVertexStream;
	}

	const IAVertexStreamStateDescriptor & IAVertexStreamStateDescriptor::GetDynamicOverrideState()
	{
		static const IAVertexStreamStateDescriptorDynamicOverride sDynamicOverrideState{};
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
