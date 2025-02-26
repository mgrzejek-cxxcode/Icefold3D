
#include "PipelineStateDescriptorFactory.h"
#include "PipelineStateDescriptorManager.h"
#include "GraphicsPipelineStateDescriptorCommon.h"
#include "GraphicsPipelineStateDescriptorIA.h"
#include "GraphicsPipelineStateDescriptorRTO.h"
#include "GraphicsPipelineStateDescriptorShader.h"
#include "PipelineStateDescriptorRootSignature.h"

#include <cppx/unique_del_ptr.h>

void F()
{
	cppx::unique_del_ptr<void> ptr{};
}

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

	const BlendSettings & PipelineStateDescriptorManager::GetDefaultBlendSettings()
	{
		static const BlendSettings defaultBlendSettings{};
		return defaultBlendSettings;
	}

	BlendStateDescriptorHandle PipelineStateDescriptorManager::GetDefaultBlendStateDescriptor()
	{
		return nullptr;
	}

	const DepthStencilSettings & PipelineStateDescriptorManager::GetDefaultDepthStencilSettings()
	{
		static const DepthStencilSettings defaultDepthStencilSettings{};
		return defaultDepthStencilSettings;
	}

	DepthStencilStateDescriptorHandle PipelineStateDescriptorManager::GetDefaultDepthStencilStateDescriptor()
	{
		return nullptr;
	}

	const DepthStencilSettings & PipelineStateDescriptorManager::GetDefaultDepthStencilSettingsWithDepthTestEnabled()
	{
		static const DepthStencilSettings defaultDepthStencilSettingsWithDepthTestEnabled{};
		return defaultDepthStencilSettingsWithDepthTestEnabled;
	}

	DepthStencilStateDescriptorHandle PipelineStateDescriptorManager::GetDefaultDepthStencilStateDescriptorWithDepthTestEnabled()
	{
		return nullptr;
	}

	const RasterizerSettings & PipelineStateDescriptorManager::GetDefaultRasterizerSettings()
	{
		static const RasterizerSettings defaultRasterizerSettings{};
		return defaultRasterizerSettings;
	}

	RasterizerStateDescriptorHandle PipelineStateDescriptorManager::GetDefaultRasterizerStateDescriptor()
	{
		return nullptr;
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

	GraphicsShaderLinkageDescriptorHandle PipelineStateDescriptorManager::CreateGraphicsShaderLinkageDescriptor(
			const GraphicsShaderLinkageDescriptorCreateInfo & pCreateInfo,
			const cppx::immutable_string & pOptionalName )
	{
		auto stateDescriptor = CreateCachedDescriptor<GraphicsShaderLinkageDescriptor>( pCreateInfo, pOptionalName );
		if( !stateDescriptor )
		{
			stateDescriptor = _descriptorFactory.CreateGraphicsShaderLinkageDescriptor( pCreateInfo );
		}
		return stateDescriptor;
	}

	VertexAttributeLayoutDescriptorHandle PipelineStateDescriptorManager::CreateVertexAttributeLayoutDescriptor(
			const VertexAttributeLayoutDescriptorCreateInfo & pCreateInfo,
			const cppx::immutable_string & pOptionalName )
	{
		auto stateDescriptor = CreateCachedDescriptor<VertexAttributeLayoutDescriptor>( pCreateInfo, pOptionalName );
		if( !stateDescriptor )
		{
			stateDescriptor = _descriptorFactory.CreateVertexAttributeLayoutDescriptor( pCreateInfo );
		}
		return stateDescriptor;
	}

	RootSignatureDescriptorHandle PipelineStateDescriptorManager::CreateRootSignatureDescriptor(
			const RootSignatureDescriptorCreateInfo & pCreateInfo,
			const cppx::immutable_string & pOptionalName )
	{
		auto stateDescriptor = CreateCachedDescriptor<RootSignatureDescriptor>( pCreateInfo, pOptionalName );
		if( !stateDescriptor )
		{
			stateDescriptor = _descriptorFactory.CreateRootSignatureDescriptor( pCreateInfo );
		}
		return stateDescriptor;
	}

	RenderPassDescriptorHandle PipelineStateDescriptorManager::CreateRenderPassDescriptor(
			const RenderPassDescriptorCreateInfo & pCreateInfo,
			const cppx::immutable_string & pOptionalName )
	{
		return _descriptorFactory.CreateRenderPassDescriptor( pCreateInfo );
	}

	RenderTargetDescriptorHandle PipelineStateDescriptorManager::CreateRenderTargetDescriptor(
			const RenderTargetDescriptorCreateInfo & pCreateInfo,
			const cppx::immutable_string & pOptionalDescriptorName )
	{
		return _descriptorFactory.CreateRenderTargetDescriptor( pCreateInfo );
	}

	VertexSourceBindingDescriptorHandle PipelineStateDescriptorManager::CreateVertexSourceBindingDescriptor(
			const VertexSourceBindingDescriptorCreateInfo & pCreateInfo,
			const cppx::immutable_string & pOptionalName )
	{
		return _descriptorFactory.CreateVertexSourceBindingDescriptor( pCreateInfo );
	}

	void PipelineStateDescriptorManager::ResetDescriptorCache( EPipelineStateDescriptorType pDescriptorType )
	{
		switch( pDescriptorType )
		{
		case EPipelineStateDescriptorType::DTBlendState:
			_graphicsPipelineStateDescriptorCache.ResetSubCache<BlendStateDescriptor>();
			break;

		case EPipelineStateDescriptorType::DTDepthStencilState:
			_graphicsPipelineStateDescriptorCache.ResetSubCache<DepthStencilStateDescriptor>();
			break;

		case EPipelineStateDescriptorType::DTRasterizerState:
			_graphicsPipelineStateDescriptorCache.ResetSubCache<RasterizerStateDescriptor>();
			break;

		case EPipelineStateDescriptorType::DTGraphicsShaderLinkage:
			_graphicsPipelineStateDescriptorCache.ResetSubCache<GraphicsShaderLinkageDescriptor>();
			break;

		case EPipelineStateDescriptorType::DTVertexAttributeLayout:
			_graphicsPipelineStateDescriptorCache.ResetSubCache<VertexAttributeLayoutDescriptor>();
			break;

		case EPipelineStateDescriptorType::DTRootSignature:
			_graphicsPipelineStateDescriptorCache.ResetSubCache<RootSignatureDescriptor>();
			break;

		default:
			break;
		}
	}

	void PipelineStateDescriptorManager::Reset( cppx::bitmask<EPipelineStateDescriptorTypeFlags> pResetMask )
	{
		if( pResetMask.is_set( ePipelineStateDescriptorTypeFlagBlendBit ) )
		{
			_graphicsPipelineStateDescriptorCache.ResetSubCache<BlendStateDescriptor>();
		}
		if( pResetMask.is_set( ePipelineStateDescriptorTypeFlagDepthStencilBit ) )
		{
			_graphicsPipelineStateDescriptorCache.ResetSubCache<DepthStencilStateDescriptor>();
		}
		if( pResetMask.is_set( ePipelineStateDescriptorTypeFlagRasterizerBit ) )
		{
			_graphicsPipelineStateDescriptorCache.ResetSubCache<RasterizerStateDescriptor>();
		}
		if( pResetMask.is_set( ePipelineStateDescriptorTypeFlagGraphicsShaderLinkageBit ) )
		{
			_graphicsPipelineStateDescriptorCache.ResetSubCache<GraphicsShaderLinkageDescriptor>();
		}
		if( pResetMask.is_set( ePipelineStateDescriptorTypeFlagIAVertexAttributeLayoutBit ) )
		{
			_graphicsPipelineStateDescriptorCache.ResetSubCache<VertexAttributeLayoutDescriptor>();
		}
		if( pResetMask.is_set( ePipelineStateDescriptorTypeFlagRootSignatureBit ) )
		{
			_graphicsPipelineStateDescriptorCache.ResetSubCache<RootSignatureDescriptor>();
		}
	}

	bool PipelineStateDescriptorManager::CreateDefaultStateDescriptors()
	{
		return true;
	}
	
} // namespace Ic3::graphics::GCI
