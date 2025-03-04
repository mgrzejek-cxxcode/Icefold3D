
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

	TGfxHandle<PipelineStateDescriptor> PipelineStateDescriptorManager::GetCachedDescriptorByID(
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

	TGfxHandle<PipelineStateDescriptor> PipelineStateDescriptorManager::GetCachedDescriptorByName(
			EPipelineStateDescriptorType pDescriptorType,
			const cppx::string_view & pDescriptorName ) const noexcept
	{
		switch( pDescriptorType )
		{
		case EPipelineStateDescriptorType::DTBlendState:
			return _graphicsPipelineStateDescriptorCache.GetDescriptorByName<BlendStateDescriptor>( pDescriptorName );

		case EPipelineStateDescriptorType::DTDepthStencilState:
			return _graphicsPipelineStateDescriptorCache.GetDescriptorByName<DepthStencilStateDescriptor>( pDescriptorName );

		case EPipelineStateDescriptorType::DTRasterizerState:
			return _graphicsPipelineStateDescriptorCache.GetDescriptorByName<RasterizerStateDescriptor>( pDescriptorName );

		case EPipelineStateDescriptorType::DTGraphicsShaderLinkage:
			return _graphicsPipelineStateDescriptorCache.GetDescriptorByName<GraphicsShaderLinkageDescriptor>( pDescriptorName );

		case EPipelineStateDescriptorType::DTVertexAttributeLayout:
			return _graphicsPipelineStateDescriptorCache.GetDescriptorByName<VertexAttributeLayoutDescriptor>( pDescriptorName );

		case EPipelineStateDescriptorType::DTRootSignature:
			return _graphicsPipelineStateDescriptorCache.GetDescriptorByName<RootSignatureDescriptor>( pDescriptorName );

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

	bool PipelineStateDescriptorManager::HasCachedDescriptorWithName(
			EPipelineStateDescriptorType pDescriptorType,
			const cppx::string_view & pDescriptorName ) const noexcept
	{
		switch( pDescriptorType )
		{
		case EPipelineStateDescriptorType::DTBlendState:
			return _graphicsPipelineStateDescriptorCache.HasDescriptorWithName<BlendStateDescriptor>( pDescriptorName );

		case EPipelineStateDescriptorType::DTDepthStencilState:
			return _graphicsPipelineStateDescriptorCache.HasDescriptorWithName<DepthStencilStateDescriptor>( pDescriptorName );

		case EPipelineStateDescriptorType::DTRasterizerState:
			return _graphicsPipelineStateDescriptorCache.HasDescriptorWithName<RasterizerStateDescriptor>( pDescriptorName );

		case EPipelineStateDescriptorType::DTGraphicsShaderLinkage:
			return _graphicsPipelineStateDescriptorCache.HasDescriptorWithName<GraphicsShaderLinkageDescriptor>( pDescriptorName );

		case EPipelineStateDescriptorType::DTVertexAttributeLayout:
			return _graphicsPipelineStateDescriptorCache.HasDescriptorWithName<VertexAttributeLayoutDescriptor>( pDescriptorName );

		case EPipelineStateDescriptorType::DTRootSignature:
			return _graphicsPipelineStateDescriptorCache.HasDescriptorWithName<RootSignatureDescriptor>( pDescriptorName );

		default:
			break;
		}

		return false;
	}

	BlendStateDescriptorHandle PipelineStateDescriptorManager::CreateBlendStateDescriptor(
			const BlendStateDescriptorCreateInfo & pCreateInfo,
			const cppx::immutable_string & pOptionalDescriptorName )
	{
		if( !_ValidateDescriptorCreateInfo( pCreateInfo ) )
		{
			return nullptr;
		}
		auto stateDescriptor = CreateCachedDescriptor<BlendStateDescriptor>( pCreateInfo, pOptionalDescriptorName );
		if( !stateDescriptor )
		{
			stateDescriptor = _descriptorFactory.CreateBlendStateDescriptor( pCreateInfo );
		}
		return stateDescriptor;
	}

	DepthStencilStateDescriptorHandle PipelineStateDescriptorManager::CreateDepthStencilStateDescriptor(
			const DepthStencilStateDescriptorCreateInfo & pCreateInfo,
			const cppx::immutable_string & pOptionalDescriptorName )
	{
		if( !_ValidateDescriptorCreateInfo( pCreateInfo ) )
		{
			return nullptr;
		}
		auto stateDescriptor = CreateCachedDescriptor<DepthStencilStateDescriptor>( pCreateInfo, pOptionalDescriptorName );
		if( !stateDescriptor )
		{
			stateDescriptor = _descriptorFactory.CreateDepthStencilStateDescriptor( pCreateInfo );
		}
		return stateDescriptor;
	}

	RasterizerStateDescriptorHandle PipelineStateDescriptorManager::CreateRasterizerStateDescriptor(
			const RasterizerStateDescriptorCreateInfo & pCreateInfo,
			const cppx::immutable_string & pOptionalDescriptorName )
	{
		if( !_ValidateDescriptorCreateInfo( pCreateInfo ) )
		{
			return nullptr;
		}
		auto stateDescriptor = CreateCachedDescriptor<RasterizerStateDescriptor>( pCreateInfo, pOptionalDescriptorName );
		if( !stateDescriptor )
		{
			stateDescriptor = _descriptorFactory.CreateRasterizerStateDescriptor( pCreateInfo );
		}
		return stateDescriptor;
	}

	GraphicsShaderLinkageDescriptorHandle PipelineStateDescriptorManager::CreateGraphicsShaderLinkageDescriptor(
			const GraphicsShaderLinkageDescriptorCreateInfo & pCreateInfo,
			const cppx::immutable_string & pOptionalDescriptorName )
	{
		if( !_ValidateDescriptorCreateInfo( pCreateInfo ) )
		{
			return nullptr;
		}
		auto stateDescriptor = CreateCachedDescriptor<GraphicsShaderLinkageDescriptor>( pCreateInfo, pOptionalDescriptorName );
		if( !stateDescriptor )
		{
			stateDescriptor = _descriptorFactory.CreateGraphicsShaderLinkageDescriptor( pCreateInfo );
		}
		return stateDescriptor;
	}

	VertexAttributeLayoutDescriptorHandle PipelineStateDescriptorManager::CreateVertexAttributeLayoutDescriptor(
			const VertexAttributeLayoutDescriptorCreateInfo & pCreateInfo,
			const cppx::immutable_string & pOptionalDescriptorName )
	{
		if( !_ValidateDescriptorCreateInfo( pCreateInfo ) )
		{
			return nullptr;
		}
		auto stateDescriptor = CreateCachedDescriptor<VertexAttributeLayoutDescriptor>( pCreateInfo, pOptionalDescriptorName );
		if( !stateDescriptor )
		{
			stateDescriptor = _descriptorFactory.CreateVertexAttributeLayoutDescriptor( pCreateInfo );
		}
		return stateDescriptor;
	}

	RootSignatureDescriptorHandle PipelineStateDescriptorManager::CreateRootSignatureDescriptor(
			const RootSignatureDescriptorCreateInfo & pCreateInfo,
			const cppx::immutable_string & pOptionalDescriptorName )
	{
		if( !_ValidateDescriptorCreateInfo( pCreateInfo ) )
		{
			return nullptr;
		}
		auto stateDescriptor = CreateCachedDescriptor<RootSignatureDescriptor>( pCreateInfo, pOptionalDescriptorName );
		if( !stateDescriptor )
		{
			stateDescriptor = _descriptorFactory.CreateRootSignatureDescriptor( pCreateInfo );
		}
		return stateDescriptor;
	}

	RenderPassDescriptorHandle PipelineStateDescriptorManager::CreateRenderPassDescriptor(
			const RenderPassDescriptorCreateInfo & pCreateInfo,
			const cppx::immutable_string & pOptionalDescriptorName )
	{
		if( !_ValidateDescriptorCreateInfo( pCreateInfo ) )
		{
			return nullptr;
		}
		return _descriptorFactory.CreateRenderPassDescriptor( pCreateInfo );
	}

	RenderTargetDescriptorHandle PipelineStateDescriptorManager::CreateRenderTargetDescriptor(
			const RenderTargetDescriptorCreateInfo & pCreateInfo,
			const cppx::immutable_string & pOptionalDescriptorName )
	{
		if( !_ValidateDescriptorCreateInfo( pCreateInfo ) )
		{
			return nullptr;
		}
		return _descriptorFactory.CreateRenderTargetDescriptor( pCreateInfo );
	}

	VertexSourceBindingDescriptorHandle PipelineStateDescriptorManager::CreateVertexSourceBindingDescriptor(
			const VertexSourceBindingDescriptorCreateInfo & pCreateInfo,
			const cppx::immutable_string & pOptionalDescriptorName )
	{
		if( !_ValidateDescriptorCreateInfo( pCreateInfo ) )
		{
			return nullptr;
		}
		return _descriptorFactory.CreateVertexSourceBindingDescriptor( pCreateInfo );
	}

	void PipelineStateDescriptorManager::ResetCache()
	{
		_graphicsPipelineStateDescriptorCache.ResetSubCache<BlendStateDescriptor>();
		_graphicsPipelineStateDescriptorCache.ResetSubCache<DepthStencilStateDescriptor>();
		_graphicsPipelineStateDescriptorCache.ResetSubCache<RasterizerStateDescriptor>();
		_graphicsPipelineStateDescriptorCache.ResetSubCache<GraphicsShaderLinkageDescriptor>();
		_graphicsPipelineStateDescriptorCache.ResetSubCache<VertexAttributeLayoutDescriptor>();
		_graphicsPipelineStateDescriptorCache.ResetSubCache<RootSignatureDescriptor>();

		_commonCachedDescriptorMapByID.clear();
		_commonCachedDescriptorMapByName.clear();
	}

	bool PipelineStateDescriptorManager::_ValidateDescriptorCreateInfo( const PipelineStateDescriptorCreateInfoBase & pCreateInfoBase ) const noexcept
	{
		return pCreateInfoBase.Validate();
	}

	void PipelineStateDescriptorManager::_RegisterDescriptorInCommonMap(
			PipelineStateDescriptorHandle pStateDescriptor,
			pipeline_state_descriptor_id_t pDescriptorID,
			cppx::immutable_string pDescriptorName )
	{
		_commonCachedDescriptorMapByID[pDescriptorID] = pStateDescriptor;

		if( !pDescriptorName.empty() )
		{
			_commonCachedDescriptorMapByName[pDescriptorName] = pStateDescriptor;
		}
	}
	
} // namespace Ic3::graphics::GCI
