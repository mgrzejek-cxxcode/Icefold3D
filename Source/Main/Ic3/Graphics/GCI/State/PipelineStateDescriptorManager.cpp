
#include "PipelineStateDescriptorFactory.h"
#include "PipelineStateDescriptorManager.h"
#include "GraphicsPipelineStateDescriptorCommon.h"
#include "GraphicsPipelineStateDescriptorIA.h"
#include "GraphicsPipelineStateDescriptorRTO.h"
#include "GraphicsPipelineStateDescriptorShader.h"
#include "PipelineStateDescriptorRootSignature.h"

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

	TGfxHandle<PipelineStateDescriptor> PipelineStateDescriptorManager::GetCachedDescriptorOfTypeByID(
			EPipelineStateDescriptorType pDescriptorType,
			pipeline_state_descriptor_id_t pDescriptorID ) const noexcept
	{
		switch( pDescriptorType )
		{
		case EPipelineStateDescriptorType::DTBlendState:
			return _graphicsPipelineStateDescriptorCache.GetDescriptorByID<BlendStateDescriptor>( pDescriptorID );

		case EPipelineStateDescriptorType::DTDepthStencilState:
			return _graphicsPipelineStateDescriptorCache.GetDescriptorByID<DepthStencilStateDescriptor>( pDescriptorID );

		case EPipelineStateDescriptorType::DTRasterizerState:
			return _graphicsPipelineStateDescriptorCache.GetDescriptorByID<RasterizerStateDescriptor>( pDescriptorID );

		case EPipelineStateDescriptorType::DTGraphicsShaderLinkage:
			return _graphicsPipelineStateDescriptorCache.GetDescriptorByID<GraphicsShaderLinkageDescriptor>( pDescriptorID );

		case EPipelineStateDescriptorType::DTVertexAttributeLayout:
			return _graphicsPipelineStateDescriptorCache.GetDescriptorByID<VertexAttributeLayoutDescriptor>( pDescriptorID );

		case EPipelineStateDescriptorType::DTRootSignature:
			return _graphicsPipelineStateDescriptorCache.GetDescriptorByID<RootSignatureDescriptor>( pDescriptorID );

		default:
			break;
		}

		return nullptr;
	}

	bool PipelineStateDescriptorManager::HasCachedDescriptorOfTypeWithID(
			EPipelineStateDescriptorType pDescriptorType,
			pipeline_state_descriptor_id_t pDescriptorID ) const noexcept
	{
		switch( pDescriptorType )
		{
		case EPipelineStateDescriptorType::DTBlendState:
			return _graphicsPipelineStateDescriptorCache.HasDescriptorWithID<BlendStateDescriptor>( pDescriptorID );

		case EPipelineStateDescriptorType::DTDepthStencilState:
			return _graphicsPipelineStateDescriptorCache.HasDescriptorWithID<DepthStencilStateDescriptor>( pDescriptorID );

		case EPipelineStateDescriptorType::DTRasterizerState:
			return _graphicsPipelineStateDescriptorCache.HasDescriptorWithID<RasterizerStateDescriptor>( pDescriptorID );

		case EPipelineStateDescriptorType::DTGraphicsShaderLinkage:
			return _graphicsPipelineStateDescriptorCache.HasDescriptorWithID<GraphicsShaderLinkageDescriptor>( pDescriptorID );

		case EPipelineStateDescriptorType::DTVertexAttributeLayout:
			return _graphicsPipelineStateDescriptorCache.HasDescriptorWithID<VertexAttributeLayoutDescriptor>( pDescriptorID );

		case EPipelineStateDescriptorType::DTRootSignature:
			return _graphicsPipelineStateDescriptorCache.HasDescriptorWithID<RootSignatureDescriptor>( pDescriptorID );

		default:
			break;
		}

		return false;
	}

	BlendStateDescriptorHandle PipelineStateDescriptorManager::CreateBlendStateDescriptor(
			const BlendStateDescriptorCreateInfo & pCreateInfo )
	{
		auto stateDescriptor = CreateCachedDescriptor<BlendStateDescriptor>( pCreateInfo );
		if( !stateDescriptor && pCreateInfo.gfxObjectUID )
		{
			stateDescriptor->SetObjectUID( pCreateInfo.gfxObjectUID );
		}
		return stateDescriptor;
	}

	DepthStencilStateDescriptorHandle PipelineStateDescriptorManager::CreateDepthStencilStateDescriptor(
			const DepthStencilStateDescriptorCreateInfo & pCreateInfo )
	{
		auto stateDescriptor = CreateCachedDescriptor<DepthStencilStateDescriptor>( pCreateInfo );
		if( !stateDescriptor && pCreateInfo.gfxObjectUID )
		{
			stateDescriptor->SetObjectUID( pCreateInfo.gfxObjectUID );
		}
		return stateDescriptor;
	}

	RasterizerStateDescriptorHandle PipelineStateDescriptorManager::CreateRasterizerStateDescriptor(
			const RasterizerStateDescriptorCreateInfo & pCreateInfo )
	{
		auto stateDescriptor = CreateCachedDescriptor<RasterizerStateDescriptor>( pCreateInfo );
		if( !stateDescriptor && pCreateInfo.gfxObjectUID )
		{
			stateDescriptor->SetObjectUID( pCreateInfo.gfxObjectUID );
		}
		return stateDescriptor;
	}

	GraphicsShaderLinkageDescriptorHandle PipelineStateDescriptorManager::CreateGraphicsShaderLinkageDescriptor(
			const GraphicsShaderLinkageDescriptorCreateInfo & pCreateInfo )
	{
		auto stateDescriptor = CreateCachedDescriptor<GraphicsShaderLinkageDescriptor>( pCreateInfo );
		if( stateDescriptor && pCreateInfo.gfxObjectUID )
		{
			stateDescriptor->SetObjectUID( pCreateInfo.gfxObjectUID );
		}
		return stateDescriptor;
	}

	VertexAttributeLayoutDescriptorHandle PipelineStateDescriptorManager::CreateVertexAttributeLayoutDescriptor(
			const VertexAttributeLayoutDescriptorCreateInfo & pCreateInfo )
	{
		auto stateDescriptor = CreateCachedDescriptor<VertexAttributeLayoutDescriptor>( pCreateInfo );
		if( stateDescriptor && pCreateInfo.gfxObjectUID )
		{
			stateDescriptor->SetObjectUID( pCreateInfo.gfxObjectUID );
		}
		return stateDescriptor;
	}

	RootSignatureDescriptorHandle PipelineStateDescriptorManager::CreateRootSignatureDescriptor(
			const RootSignatureDescriptorCreateInfo & pCreateInfo )
	{
		auto stateDescriptor = CreateCachedDescriptor<RootSignatureDescriptor>( pCreateInfo );
		if( !stateDescriptor && pCreateInfo.gfxObjectUID )
		{
			stateDescriptor->SetObjectUID( pCreateInfo.gfxObjectUID );
		}
		return stateDescriptor;
	}

	RenderPassDescriptorHandle PipelineStateDescriptorManager::CreateRenderPassDescriptor(
			const RenderPassDescriptorCreateInfo & pCreateInfo,
			pipeline_state_descriptor_id_t pDescriptorID )
	{
		const auto stateDescriptor = _descriptorFactory.CreateRenderPassDescriptor( pDescriptorID, pCreateInfo );
		if( stateDescriptor && pCreateInfo.gfxObjectUID )
		{
			stateDescriptor->SetObjectUID( pCreateInfo.gfxObjectUID );
		}
		return stateDescriptor;
	}

	RenderTargetDescriptorHandle PipelineStateDescriptorManager::CreateRenderTargetDescriptor(
			const RenderTargetDescriptorCreateInfo & pCreateInfo,
			pipeline_state_descriptor_id_t pDescriptorID )
	{
		const auto stateDescriptor = _descriptorFactory.CreateRenderTargetDescriptor( pDescriptorID, pCreateInfo );
		if( stateDescriptor && pCreateInfo.gfxObjectUID )
		{
			stateDescriptor->SetObjectUID( pCreateInfo.gfxObjectUID );
		}
		return stateDescriptor;
	}

	VertexSourceBindingDescriptorHandle PipelineStateDescriptorManager::CreateVertexSourceBindingDescriptor(
			const VertexSourceBindingDescriptorCreateInfo & pCreateInfo,
			pipeline_state_descriptor_id_t pDescriptorID )
	{
		const auto stateDescriptor = _descriptorFactory.CreateVertexSourceBindingDescriptor( pDescriptorID, pCreateInfo );
		if( stateDescriptor && pCreateInfo.gfxObjectUID )
		{
			stateDescriptor->SetObjectUID( pCreateInfo.gfxObjectUID );
		}
		return stateDescriptor;
	}

	void PipelineStateDescriptorManager::ResetCache()
	{
		_graphicsPipelineStateDescriptorCache.ResetSubCache<BlendStateDescriptor>();
		_graphicsPipelineStateDescriptorCache.ResetSubCache<DepthStencilStateDescriptor>();
		_graphicsPipelineStateDescriptorCache.ResetSubCache<RasterizerStateDescriptor>();
		_graphicsPipelineStateDescriptorCache.ResetSubCache<GraphicsShaderLinkageDescriptor>();
		_graphicsPipelineStateDescriptorCache.ResetSubCache<VertexAttributeLayoutDescriptor>();
		_graphicsPipelineStateDescriptorCache.ResetSubCache<RootSignatureDescriptor>();
	}
	
} // namespace Ic3::graphics::GCI
