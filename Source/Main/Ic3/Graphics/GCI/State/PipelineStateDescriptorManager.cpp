
#include "PipelineStateDescriptorManager.h"
#include "GraphicsPipelineStateDescriptorCommon.h"
#include "GraphicsPipelineStateDescriptorIA.h"
#include "GraphicsPipelineStateDescriptorRTO.h"
#include "GraphicsPipelineStateDescriptorShader.h"
#include "PipelineStateDescriptorShaderRootSignature.h"

namespace Ic3::Graphics::GCI
{

	PipelineStateDescriptorManager::PipelineStateDescriptorManager(
			GPUDevice & pGPUDevice,
			PipelineStateDescriptorFactory & pDescriptorFactory )
	: GPUDeviceChildObject( pGPUDevice )
	, _descriptorFactory( pDescriptorFactory )
	, _graphicsPipelineStateDescriptorCache( pDescriptorFactory )
	{}

	PipelineStateDescriptorManager::~PipelineStateDescriptorManager() = default;

	TGfxHandle<PipelineStateDescriptor> PipelineStateDescriptorManager::GetCachedDescriptorByID(
			EPipelineStateDescriptorType pDescriptorType,
			pipeline_state_descriptor_id_t pDescriptorID ) const noexcept
	{
		switch( pDescriptorType )
		{
		case EPipelineStateDescriptorType::DTBlend:
			return _graphicsPipelineStateDescriptorCache.GetDescriptorByID<BlendStateDescriptor>( pDescriptorID );

		case EPipelineStateDescriptorType::DTDepthStencil:
			return _graphicsPipelineStateDescriptorCache.GetDescriptorByID<DepthStencilStateDescriptor>( pDescriptorID );

		case EPipelineStateDescriptorType::DTRasterizer:
			return _graphicsPipelineStateDescriptorCache.GetDescriptorByID<RasterizerStateDescriptor>( pDescriptorID );

		case EPipelineStateDescriptorType::DTGraphicsShaderLinkage:
			return _graphicsPipelineStateDescriptorCache.GetDescriptorByID<GraphicsShaderLinkageStateDescriptor>( pDescriptorID );

		case EPipelineStateDescriptorType::DTIAVertexAttributeLayout:
			return _graphicsPipelineStateDescriptorCache.GetDescriptorByID<IAVertexAttributeLayoutStateDescriptor>( pDescriptorID );

		case EPipelineStateDescriptorType::DTShaderRootSignature:
			return _graphicsPipelineStateDescriptorCache.GetDescriptorByID<ShaderRootSignatureStateDescriptor>( pDescriptorID );

		default:
			break;
		}

		return nullptr;
	}

	TGfxHandle<PipelineStateDescriptor> PipelineStateDescriptorManager::GetCachedDescriptorByName(
			EPipelineStateDescriptorType pDescriptorType,
			const cppx::string_view & pDescriptorName ) const noexcept
	{
		switch( pDescriptorType )
		{
		case EPipelineStateDescriptorType::DTBlend:
			return _graphicsPipelineStateDescriptorCache.GetDescriptorByName<BlendStateDescriptor>( pDescriptorName );

		case EPipelineStateDescriptorType::DTDepthStencil:
			return _graphicsPipelineStateDescriptorCache.GetDescriptorByName<DepthStencilStateDescriptor>( pDescriptorName );

		case EPipelineStateDescriptorType::DTRasterizer:
			return _graphicsPipelineStateDescriptorCache.GetDescriptorByName<RasterizerStateDescriptor>( pDescriptorName );

		case EPipelineStateDescriptorType::DTGraphicsShaderLinkage:
			return _graphicsPipelineStateDescriptorCache.GetDescriptorByName<GraphicsShaderLinkageStateDescriptor>( pDescriptorName );

		case EPipelineStateDescriptorType::DTIAVertexAttributeLayout:
			return _graphicsPipelineStateDescriptorCache.GetDescriptorByName<IAVertexAttributeLayoutStateDescriptor>( pDescriptorName );

		case EPipelineStateDescriptorType::DTShaderRootSignature:
			return _graphicsPipelineStateDescriptorCache.GetDescriptorByName<ShaderRootSignatureStateDescriptor>( pDescriptorName );

		default:
			break;
		}

		return nullptr;
	}

	bool PipelineStateDescriptorManager::HasCachedDescriptorWithID(
			EPipelineStateDescriptorType pDescriptorType,
			pipeline_state_descriptor_id_t pDescriptorID ) const noexcept
	{
		switch( pDescriptorType )
		{
		case EPipelineStateDescriptorType::DTBlend:
			return _graphicsPipelineStateDescriptorCache.HasDescriptorWithID<BlendStateDescriptor>( pDescriptorID );

		case EPipelineStateDescriptorType::DTDepthStencil:
			return _graphicsPipelineStateDescriptorCache.HasDescriptorWithID<DepthStencilStateDescriptor>( pDescriptorID );

		case EPipelineStateDescriptorType::DTRasterizer:
			return _graphicsPipelineStateDescriptorCache.HasDescriptorWithID<RasterizerStateDescriptor>( pDescriptorID );

		case EPipelineStateDescriptorType::DTGraphicsShaderLinkage:
			return _graphicsPipelineStateDescriptorCache.HasDescriptorWithID<GraphicsShaderLinkageStateDescriptor>( pDescriptorID );

		case EPipelineStateDescriptorType::DTIAVertexAttributeLayout:
			return _graphicsPipelineStateDescriptorCache.HasDescriptorWithID<IAVertexAttributeLayoutStateDescriptor>( pDescriptorID );

		case EPipelineStateDescriptorType::DTShaderRootSignature:
			return _graphicsPipelineStateDescriptorCache.HasDescriptorWithID<ShaderRootSignatureStateDescriptor>( pDescriptorID );

		default:
			break;
		}

		return false;
	}

	bool PipelineStateDescriptorManager::HasCachedDescriptorWithName(
			EPipelineStateDescriptorType pDescriptorType,
			const cppx::string_view & pDescriptorName ) const noexcept
	{
		switch( pDescriptorType )
		{
		case EPipelineStateDescriptorType::DTBlend:
			return _graphicsPipelineStateDescriptorCache.HasDescriptorWithName<BlendStateDescriptor>( pDescriptorName );

		case EPipelineStateDescriptorType::DTDepthStencil:
			return _graphicsPipelineStateDescriptorCache.HasDescriptorWithName<DepthStencilStateDescriptor>( pDescriptorName );

		case EPipelineStateDescriptorType::DTRasterizer:
			return _graphicsPipelineStateDescriptorCache.HasDescriptorWithName<RasterizerStateDescriptor>( pDescriptorName );

		case EPipelineStateDescriptorType::DTGraphicsShaderLinkage:
			return _graphicsPipelineStateDescriptorCache.HasDescriptorWithName<GraphicsShaderLinkageStateDescriptor>( pDescriptorName );

		case EPipelineStateDescriptorType::DTIAVertexAttributeLayout:
			return _graphicsPipelineStateDescriptorCache.HasDescriptorWithName<IAVertexAttributeLayoutStateDescriptor>( pDescriptorName );

		case EPipelineStateDescriptorType::DTShaderRootSignature:
			return _graphicsPipelineStateDescriptorCache.HasDescriptorWithName<ShaderRootSignatureStateDescriptor>( pDescriptorName );

		default:
			break;
		}

		return false;
	}

	BlendStateDescriptorHandle PipelineStateDescriptorManager::CreateBlendStateDescriptor(
			const BlendStateDescriptorCreateInfo & pCreateInfo,
			const cppx::immutable_string & pOptionalName )
	{
		auto stateDescriptor = CreateCachedDescriptor<BlendStateDescriptor>( pCreateInfo, pOptionalName );
		if( !stateDescriptor )
		{
			stateDescriptor = _descriptorFactory.CreateBlendStateDescriptor( pCreateInfo );
		}
		return stateDescriptor;
	}

	DepthStencilStateDescriptorHandle PipelineStateDescriptorManager::CreateDepthStencilStateDescriptor(
			const DepthStencilStateDescriptorCreateInfo & pCreateInfo,
			const cppx::immutable_string & pOptionalName )
	{
		auto stateDescriptor = CreateCachedDescriptor<DepthStencilStateDescriptor>( pCreateInfo, pOptionalName );
		if( !stateDescriptor )
		{
			stateDescriptor = _descriptorFactory.CreateDepthStencilStateDescriptor( pCreateInfo );
		}
		return stateDescriptor;
	}

	RasterizerStateDescriptorHandle PipelineStateDescriptorManager::CreateRasterizerStateDescriptor(
			const RasterizerStateDescriptorCreateInfo & pCreateInfo,
			const cppx::immutable_string & pOptionalName )
	{
		auto stateDescriptor = CreateCachedDescriptor<RasterizerStateDescriptor>( pCreateInfo, pOptionalName );
		if( !stateDescriptor )
		{
			stateDescriptor = _descriptorFactory.CreateRasterizerStateDescriptor( pCreateInfo );
		}
		return stateDescriptor;
	}

	GraphicsShaderLinkageStateDescriptorHandle PipelineStateDescriptorManager::CreateGraphicsShaderLinkageStateDescriptor(
			const GraphicsShaderLinkageStateDescriptorCreateInfo & pCreateInfo,
			const cppx::immutable_string & pOptionalName )
	{
		auto stateDescriptor = CreateCachedDescriptor<GraphicsShaderLinkageStateDescriptor>( pCreateInfo, pOptionalName );
		if( !stateDescriptor )
		{
			stateDescriptor = _descriptorFactory.CreateGraphicsShaderLinkageStateDescriptor( pCreateInfo );
		}
		return stateDescriptor;
	}

	IAVertexAttributeLayoutStateDescriptorHandle PipelineStateDescriptorManager::CreateIAVertexAttributeLayoutStateDescriptor(
			const IAVertexAttributeLayoutStateDescriptorCreateInfo & pCreateInfo,
			const cppx::immutable_string & pOptionalName )
	{
		auto stateDescriptor = CreateCachedDescriptor<IAVertexAttributeLayoutStateDescriptor>( pCreateInfo, pOptionalName );
		if( !stateDescriptor )
		{
			stateDescriptor = _descriptorFactory.CreateIAVertexAttributeLayoutStateDescriptor( pCreateInfo );
		}
		return stateDescriptor;
	}

	ShaderRootSignatureStateDescriptorHandle PipelineStateDescriptorManager::CreateShaderRootSignatureStateDescriptor(
			const ShaderRootSignatureStateDescriptorCreateInfo & pCreateInfo,
			const cppx::immutable_string & pOptionalName )
	{
		auto stateDescriptor = CreateCachedDescriptor<ShaderRootSignatureStateDescriptor>( pCreateInfo, pOptionalName );
		if( !stateDescriptor )
		{
			stateDescriptor = _descriptorFactory.CreateShaderRootSignatureStateDescriptor( pCreateInfo );
		}
		return stateDescriptor;
	}

	IAVertexStreamBindingStateDescriptorHandle PipelineStateDescriptorManager::CreateIAVertexStreamBindingStateDescriptor(
			const IAVertexStreamBindingStateDescriptorCreateInfo & pCreateInfo,
			const cppx::immutable_string & pOptionalName )
	{
		return _descriptorFactory.CreateIAVertexStreamBindingStateDescriptor( pCreateInfo );
	}

	RenderPassConfigurationStateDescriptorHandle PipelineStateDescriptorManager::CreateRenderPassConfigurationStateDescriptor(
			const RenderPassConfigurationStateDescriptorCreateInfo & pCreateInfo,
			const cppx::immutable_string & pOptionalName )
	{
		return _descriptorFactory.CreateRenderPassConfigurationStateDescriptor( pCreateInfo );
	}
	
} // namespace Ic3::graphics::GCI
