
#include "GraphicsPipelineStateDescriptorShader.h"

#include "Ic3/Graphics/GCI/Resources/Shader.h"

namespace Ic3::Graphics::GCI
{

	GraphicsShaderLinkageDescriptor::GraphicsShaderLinkageDescriptor(
			GPUDevice & pGPUDevice,
			const GraphicsShaderBindingCommonConfig & pCommonShaderBindingConfig )
	: CachedPipelineStateDescriptor( pGPUDevice )
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
			const GraphicsShaderBinding & pShaderBinding )
	: GCIPipelineStateDescriptor( pGPUDevice, pShaderBinding )
	, mShaderBinding( pShaderBinding )
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
			const GraphicsShaderBinding & pBindingConfiguration ) noexcept
	{
		if( !Utilities::SHValidateGraphicsShaderBinding( pBindingConfiguration ) )
		{
			return nullptr;
		}

		return CreateDynamicObject<GraphicsShaderLinkageDescriptorGenericSeparable>( pGPUDevice, pBindingConfiguration );
	}

	TGfxHandle<GraphicsShaderLinkageDescriptorGenericSeparable> GraphicsShaderLinkageDescriptorGenericSeparable::CreateFromShaderArray(
			GPUDevice & pGPUDevice,
			const GraphicsShaderArray & pShaderArray ) noexcept
	{
		if( !pShaderArray[kShaderStageIndexGraphicsVertex] || !pShaderArray[kShaderStageIndexGraphicsPixel] )
		{
			return nullptr;
		}

		if( !Utilities::SHValidateGraphicsShaderArray( pShaderArray ) )
		{
			return nullptr;
		}

		GraphicsShaderBinding shaderBinding{};
		shaderBinding.SetShaders( pShaderArray );

		return CreateDynamicObject<GraphicsShaderLinkageDescriptorGenericSeparable>( pGPUDevice, shaderBinding );
	}

}
