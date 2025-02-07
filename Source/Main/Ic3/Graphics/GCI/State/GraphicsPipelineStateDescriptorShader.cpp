
#include "GraphicsPipelineStateDescriptorShader.h"

namespace Ic3::Graphics::GCI
{

	GraphicsShaderLinkageStateDescriptor::GraphicsShaderLinkageStateDescriptor(
			GPUDevice & pGPUDevice,
			pipeline_state_descriptor_id_t pDescriptorID,
			const GraphicsShaderBindingCommonConfig & pCommonShaderBindingConfig )
	: PipelineStateDescriptor( pGPUDevice, pDescriptorID )
	, mCommonShaderBindingConfig( pCommonShaderBindingConfig )
	{}

	GraphicsShaderLinkageStateDescriptor::~GraphicsShaderLinkageStateDescriptor() = default;

	EPipelineStateDescriptorType GraphicsShaderLinkageStateDescriptor::GetDescriptorType() const noexcept
	{
		return EPipelineStateDescriptorType::DTGraphicsShaderLinkage;
	}

	bool GraphicsShaderLinkageStateDescriptor::IsStageActive( uint32 pStageIndex ) const noexcept
	{
		const auto stageBit = CXU::SHMakeGraphicsShaderStageBit( pStageIndex );
		return mCommonShaderBindingConfig.activeStagesMask.is_set( stageBit );
	}

	bool GraphicsShaderLinkageStateDescriptor::IsStageActive( EShaderType pShaderType ) const noexcept
	{
		const auto stageBit = CXU::SHGetShaderStageBit( pShaderType );
		return mCommonShaderBindingConfig.activeStagesMask.is_set( stageBit );
	}


	GraphicsShaderLinkageStateDescriptorGenericSeparable::GraphicsShaderLinkageStateDescriptorGenericSeparable(
			GPUDevice & pGPUDevice,
			pipeline_state_descriptor_id_t pDescriptorID,
			const GraphicsShaderBinding & pShaderBinding )
	: GraphicsShaderLinkageStateDescriptor( pGPUDevice, pDescriptorID, pShaderBinding )
	{}

	GraphicsShaderLinkageStateDescriptorGenericSeparable::~GraphicsShaderLinkageStateDescriptorGenericSeparable() = default;

	ShaderHandle GraphicsShaderLinkageStateDescriptorGenericSeparable::GetShader( size_t pStageIndex ) const noexcept
	{
		return mShaderBinding[pStageIndex];
	}

	ShaderHandle GraphicsShaderLinkageStateDescriptorGenericSeparable::GetShader( EShaderType pShaderType ) const noexcept
	{
		const auto stageIndex = CXU::SHGetShaderStageIndex( pShaderType );
		return mShaderBinding[stageIndex];
	}

	TGfxHandle<GraphicsShaderLinkageStateDescriptorGenericSeparable> GraphicsShaderLinkageStateDescriptorGenericSeparable::CreateFromBindingConfiguration(
			GPUDevice & pGPUDevice,
			const GraphicsShaderBinding & pShaderBinding,
			pipeline_state_descriptor_id_t pDescriptorID ) noexcept
	{
		if( !GCU::SHValidateGraphicsShaderBinding( pShaderBinding ) )
		{
			return nullptr;
		}

		return CreateDynamicObject<GraphicsShaderLinkageStateDescriptorGenericSeparable>( pGPUDevice, pDescriptorID, pShaderBinding );
	}

	TGfxHandle<GraphicsShaderLinkageStateDescriptorGenericSeparable> GraphicsShaderLinkageStateDescriptorGenericSeparable::CreateFromShaderArray(
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

		return CreateDynamicObject<GraphicsShaderLinkageStateDescriptorGenericSeparable>( pGPUDevice, pDescriptorID, shaderBinding );
	}

}
