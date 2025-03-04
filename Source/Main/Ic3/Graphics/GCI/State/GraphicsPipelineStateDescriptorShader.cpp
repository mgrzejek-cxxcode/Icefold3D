
#include "GraphicsPipelineStateDescriptorShader.h"

#include "Ic3/Graphics/GCI/Resources/Shader.h"

namespace Ic3::Graphics::GCI
{

	GraphicsShaderLinkageDescriptor::GraphicsShaderLinkageDescriptor(
			GPUDevice & pGPUDevice,
			pipeline_state_descriptor_id_t pDescriptorID,
			const GraphicsShaderBindingCommonConfig & pCommonShaderBindingConfig )
	: PipelineStateDescriptor( pGPUDevice, pDescriptorID )
	, mCommonShaderBindingConfig( pCommonShaderBindingConfig )
	{}

	GraphicsShaderLinkageDescriptor::~GraphicsShaderLinkageDescriptor() = default;

	EPipelineStateDescriptorType GraphicsShaderLinkageDescriptor::GetDescriptorType() const noexcept
	{
		return EPipelineStateDescriptorType::DTGraphicsShaderLinkage;
	}

	bool GraphicsShaderLinkageDescriptor::IsStageActive( uint32 pStageIndex ) const noexcept
	{
		const auto stageBit = CXU::SHMakeGraphicsShaderStageBit( pStageIndex );
		return mCommonShaderBindingConfig.activeStagesMask.is_set( stageBit );
	}

	bool GraphicsShaderLinkageDescriptor::IsStageActive( EShaderType pShaderType ) const noexcept
	{
		const auto stageBit = CXU::SHGetShaderStageBit( pShaderType );
		return mCommonShaderBindingConfig.activeStagesMask.is_set( stageBit );
	}


	GraphicsShaderLinkageDescriptorGenericSeparable::GraphicsShaderLinkageDescriptorGenericSeparable(
			GPUDevice & pGPUDevice,
			pipeline_state_descriptor_id_t pDescriptorID,
			const GraphicsShaderBinding & pBindingConfiguration )
	: GraphicsShaderLinkageDescriptor( pGPUDevice, pDescriptorID, pBindingConfiguration )
	{}

	GraphicsShaderLinkageDescriptorGenericSeparable::~GraphicsShaderLinkageDescriptorGenericSeparable() = default;

	ShaderHandle GraphicsShaderLinkageDescriptorGenericSeparable::GetShader( size_t pStageIndex ) const noexcept
	{
		return mShaderBinding[pStageIndex]->GetHandle<Shader>();
	}

	ShaderHandle GraphicsShaderLinkageDescriptorGenericSeparable::GetShader( EShaderType pShaderType ) const noexcept
	{
		const auto stageIndex = CXU::SHGetShaderStageIndex( pShaderType );
		return mShaderBinding[stageIndex]->GetHandle<Shader>();
	}

	TGfxHandle<GraphicsShaderLinkageDescriptorGenericSeparable> GraphicsShaderLinkageDescriptorGenericSeparable::CreateFromBindingConfiguration(
			GPUDevice & pGPUDevice,
			const GraphicsShaderBinding & pBindingConfiguration,
			pipeline_state_descriptor_id_t pDescriptorID ) noexcept
	{
		if( !GCU::SHValidateGraphicsShaderBinding( pBindingConfiguration ) )
		{
			return nullptr;
		}

		return CreateDynamicObject<GraphicsShaderLinkageDescriptorGenericSeparable>( pGPUDevice, pDescriptorID, pBindingConfiguration );
	}

	TGfxHandle<GraphicsShaderLinkageDescriptorGenericSeparable> GraphicsShaderLinkageDescriptorGenericSeparable::CreateFromShaderArray(
			GPUDevice & pGPUDevice,
			const GraphicsShaderArray & pShaderArray,
			pipeline_state_descriptor_id_t pDescriptorID ) noexcept
	{
		if( !pShaderArray[kShaderStageIndexGraphicsVertex] || !pShaderArray[kShaderStageIndexGraphicsPixel] )
		{
			return nullptr;
		}

		if( !GCU::SHValidateGraphicsShaderArray( pShaderArray ) )
		{
			return nullptr;
		}

		GraphicsShaderBinding shaderBinding{};
		shaderBinding.SetShaders( pShaderArray );

		return CreateDynamicObject<GraphicsShaderLinkageDescriptorGenericSeparable>( pGPUDevice, pDescriptorID, shaderBinding );
	}

}
