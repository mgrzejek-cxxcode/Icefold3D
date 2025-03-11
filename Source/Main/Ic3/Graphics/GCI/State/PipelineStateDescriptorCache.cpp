
#include "PipelineStateDescriptor.h"
#include "PipelineStateDescriptorCache.h"
#include "PipelineStateDescriptorFactory.h"

namespace Ic3::Graphics::GCI
{


	GraphicsPipelineStateDescriptorFactoryCacheAdapter::GraphicsPipelineStateDescriptorFactoryCacheAdapter(
			PipelineStateDescriptorFactory & pDescriptorFactory )
	: _descriptorFactory( &pDescriptorFactory )
	{}

	GraphicsPipelineStateDescriptorFactoryCacheAdapter::~GraphicsPipelineStateDescriptorFactoryCacheAdapter() = default;

	BlendStateDescriptorHandle GraphicsPipelineStateDescriptorFactoryCacheAdapter::CreateDescriptor(
			const BlendStateDescriptorCreateInfo & pCreateInfo )
	{
		return _descriptorFactory->CreateBlendStateDescriptor( pCreateInfo );
	}

	DepthStencilStateDescriptorHandle GraphicsPipelineStateDescriptorFactoryCacheAdapter::CreateDescriptor(
			const DepthStencilStateDescriptorCreateInfo & pCreateInfo )
	{
		return _descriptorFactory->CreateDepthStencilStateDescriptor( pCreateInfo );
	}

	RasterizerStateDescriptorHandle GraphicsPipelineStateDescriptorFactoryCacheAdapter::CreateDescriptor(
			const RasterizerStateDescriptorCreateInfo & pCreateInfo )
	{
		return _descriptorFactory->CreateRasterizerStateDescriptor( pCreateInfo );
	}

	GraphicsShaderLinkageDescriptorHandle GraphicsPipelineStateDescriptorFactoryCacheAdapter::CreateDescriptor(
			const GraphicsShaderLinkageDescriptorCreateInfo & pCreateInfo )
	{
		return _descriptorFactory->CreateGraphicsShaderLinkageDescriptor( pCreateInfo );
	}

	VertexAttributeLayoutDescriptorHandle GraphicsPipelineStateDescriptorFactoryCacheAdapter::CreateDescriptor(
			const VertexAttributeLayoutDescriptorCreateInfo & pCreateInfo )
	{
		return _descriptorFactory->CreateVertexAttributeLayoutDescriptor( pCreateInfo );
	}

	RootSignatureDescriptorHandle GraphicsPipelineStateDescriptorFactoryCacheAdapter::CreateDescriptor(
			const RootSignatureDescriptorCreateInfo & pCreateInfo )
	{
		return _descriptorFactory->CreateRootSignatureDescriptor( pCreateInfo );
	}


	GraphicsPipelineStateDescriptorCache::GraphicsPipelineStateDescriptorCache( PipelineStateDescriptorFactory & pDescriptorFactory )
	: mDescriptorFactory( pDescriptorFactory )
	, _descriptorFactoryAdapter( pDescriptorFactory )
	, _cacheUnitBlendState( _descriptorFactoryAdapter )
	, _cacheUnitDepthStencilState( _descriptorFactoryAdapter )
	, _cacheUnitRasterizerState( _descriptorFactoryAdapter )
	, _cacheUnitGraphicsShaderLinkage( _descriptorFactoryAdapter )
	, _cacheUnitVertexAttributeLayout( _descriptorFactoryAdapter )
	, _cacheUnitRootSignature( _descriptorFactoryAdapter )
	{}

	void GraphicsPipelineStateDescriptorCache::Reset( cppx::bitmask<EPipelineStateDescriptorTypeFlags> pResetMask )
	{
		if( pResetMask.is_set( ePipelineStateDescriptorTypeFlagBlendBit ) )
		{
			_cacheUnitBlendState.Reset();
		}

		if( pResetMask.is_set( ePipelineStateDescriptorTypeFlagDepthStencilBit ) )
		{
			_cacheUnitDepthStencilState.Reset();
		}

		if( pResetMask.is_set( ePipelineStateDescriptorTypeFlagRasterizerBit ) )
		{
			_cacheUnitRasterizerState.Reset();
		}

		if( pResetMask.is_set( ePipelineStateDescriptorTypeFlagGraphicsShaderLinkageBit ) )
		{
			_cacheUnitGraphicsShaderLinkage.Reset();
		}

		if( pResetMask.is_set( ePipelineStateDescriptorTypeFlagIAVertexAttributeLayoutBit ) )
		{
			_cacheUnitVertexAttributeLayout.Reset();
		}

		if( pResetMask.is_set( ePipelineStateDescriptorTypeFlagRootSignatureBit ) )
		{
			_cacheUnitRootSignature.Reset();
		}
	}

} // namespace Ic3::Graphics::GCI
