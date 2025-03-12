
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
		return CreateCachedDescriptor<BlendStateDescriptor>( pCreateInfo );
	}

	DepthStencilStateDescriptorHandle PipelineStateDescriptorManager::CreateDepthStencilStateDescriptor(
			const DepthStencilStateDescriptorCreateInfo & pCreateInfo )
	{
		return CreateCachedDescriptor<DepthStencilStateDescriptor>( pCreateInfo );
	}

	RasterizerStateDescriptorHandle PipelineStateDescriptorManager::CreateRasterizerStateDescriptor(
			const RasterizerStateDescriptorCreateInfo & pCreateInfo )
	{
		return CreateCachedDescriptor<RasterizerStateDescriptor>( pCreateInfo );
	}

	GraphicsShaderLinkageDescriptorHandle PipelineStateDescriptorManager::CreateGraphicsShaderLinkageDescriptor(
			const GraphicsShaderLinkageDescriptorCreateInfo & pCreateInfo )
	{
		return CreateCachedDescriptor<GraphicsShaderLinkageDescriptor>( pCreateInfo );
	}

	VertexAttributeLayoutDescriptorHandle PipelineStateDescriptorManager::CreateVertexAttributeLayoutDescriptor(
			const VertexAttributeLayoutDescriptorCreateInfo & pCreateInfo )
	{
		return CreateCachedDescriptor<VertexAttributeLayoutDescriptor>( pCreateInfo );
	}

	RootSignatureDescriptorHandle PipelineStateDescriptorManager::CreateRootSignatureDescriptor(
			const RootSignatureDescriptorCreateInfo & pCreateInfo )
	{
		return CreateCachedDescriptor<RootSignatureDescriptor>( pCreateInfo );
	}

	RenderPassDescriptorHandle PipelineStateDescriptorManager::CreateRenderPassDescriptor(
			const RenderPassDescriptorCreateInfo & pCreateInfo,
			pipeline_state_descriptor_id_t pDescriptorID )
	{
		const auto stateDescriptor = _descriptorFactory.CreateRenderPassDescriptor( pCreateInfo );
		if( stateDescriptor )
		{
			UpdateDescriptorCoreInfo( *stateDescriptor, pDescriptorID, pCreateInfo.gfxObjectUID );
		}
		return stateDescriptor;
	}

	RenderTargetDescriptorHandle PipelineStateDescriptorManager::CreateRenderTargetDescriptor(
			const RenderTargetDescriptorCreateInfo & pCreateInfo,
			pipeline_state_descriptor_id_t pDescriptorID )
	{
		const auto stateDescriptor = _descriptorFactory.CreateRenderTargetDescriptor( pCreateInfo );
		if( stateDescriptor )
		{
			UpdateDescriptorCoreInfo( *stateDescriptor, pDescriptorID, pCreateInfo.gfxObjectUID );
		}
		return stateDescriptor;
	}

	VertexSourceBindingDescriptorHandle PipelineStateDescriptorManager::CreateVertexSourceBindingDescriptor(
			const VertexSourceBindingDescriptorCreateInfo & pCreateInfo,
			pipeline_state_descriptor_id_t pDescriptorID )
	{
		const auto stateDescriptor = _descriptorFactory.CreateVertexSourceBindingDescriptor( pCreateInfo );
		if( stateDescriptor )
		{
			UpdateDescriptorCoreInfo( *stateDescriptor, pDescriptorID, pCreateInfo.gfxObjectUID );
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

	void PipelineStateDescriptorManager::UpdateDescriptorCoreInfo(
			PipelineStateDescriptor & pDescriptor,
			pipeline_state_descriptor_id_t pDescriptorID,
			const GfxObjectUID & pObjectUID )
	{
		if( !pDescriptor.HasValidDescriptoIDSet() && CXU::IsPipelineStateDescriptorIDValid( pDescriptorID ) )
		{
			pDescriptor.SetDescriptorID( pDescriptorID );
		}

		if( pObjectUID )
		{
			pDescriptor.SetObjectUID( pObjectUID );
		}
	}
	
} // namespace Ic3::graphics::GCI
