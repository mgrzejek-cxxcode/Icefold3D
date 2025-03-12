#pragma once

#ifndef __IC3_GRAPHICS_GCI_SHARED_IMMUTABLE_STATE_MANAGER_H__
#define __IC3_GRAPHICS_GCI_SHARED_IMMUTABLE_STATE_MANAGER_H__

#include "PipelineStateDescriptorCache.h"
#include <cppx/stdHelperAlgo.h>

namespace Ic3::Graphics::GCI
{
	class IC3_GRAPHICS_GCI_CLASS PipelineStateDescriptorManager : public GPUDeviceChildObject
	{
		friend class GPUDevice;

	public:
		PipelineStateDescriptorManager( GPUDevice & pGPUDevice, PipelineStateDescriptorFactory & pDescriptorFactory );
		virtual ~PipelineStateDescriptorManager();

		CPPX_ATTR_NO_DISCARD PipelineStateDescriptorHandle GetCachedDescriptorByID(
				pipeline_state_descriptor_id_t pDescriptorID ) const noexcept;

		CPPX_ATTR_NO_DISCARD PipelineStateDescriptorHandle GetCachedDescriptorOfTypeByID(
				EPipelineStateDescriptorType pDescriptorType,
				pipeline_state_descriptor_id_t pDescriptorID ) const noexcept;

		template <typename TPDescriptorType>
		CPPX_ATTR_NO_DISCARD TGfxHandle<TPDescriptorType> GetCachedDescriptorOfTypeByID(
				pipeline_state_descriptor_id_t pDescriptorID ) const noexcept;

		CPPX_ATTR_NO_DISCARD bool HasCachedDescriptorWithID(
				pipeline_state_descriptor_id_t pDescriptorID ) const noexcept;

		CPPX_ATTR_NO_DISCARD bool HasCachedDescriptorOfTypeWithID(
				EPipelineStateDescriptorType pDescriptorType,
				pipeline_state_descriptor_id_t pDescriptorID ) const noexcept;

		template <typename TPDescriptorType>
		CPPX_ATTR_NO_DISCARD bool HasCachedDescriptorOfTypeWithID(
				pipeline_state_descriptor_id_t pDescriptorID ) const noexcept;

		BlendStateDescriptorHandle CreateBlendStateDescriptor(
				const BlendStateDescriptorCreateInfo & pCreateInfo );

		DepthStencilStateDescriptorHandle CreateDepthStencilStateDescriptor(
				const DepthStencilStateDescriptorCreateInfo & pCreateInfo );

		RasterizerStateDescriptorHandle CreateRasterizerStateDescriptor(
				const RasterizerStateDescriptorCreateInfo & pCreateInfo );

		GraphicsShaderLinkageDescriptorHandle CreateGraphicsShaderLinkageDescriptor(
				const GraphicsShaderLinkageDescriptorCreateInfo & pCreateInfo );

		VertexAttributeLayoutDescriptorHandle CreateVertexAttributeLayoutDescriptor(
				const VertexAttributeLayoutDescriptorCreateInfo & pCreateInfo );

		RootSignatureDescriptorHandle CreateRootSignatureDescriptor(
				const RootSignatureDescriptorCreateInfo & pCreateInfo );

		RenderPassDescriptorHandle CreateRenderPassDescriptor(
				const RenderPassDescriptorCreateInfo & pCreateInfo,
				pipeline_state_descriptor_id_t pDescriptorID = kPipelineStateDescriptorIDInvalid );

		RenderTargetDescriptorHandle CreateRenderTargetDescriptor(
				const RenderTargetDescriptorCreateInfo & pCreateInfo,
				pipeline_state_descriptor_id_t pDescriptorID = kPipelineStateDescriptorIDInvalid );

		VertexSourceBindingDescriptorHandle CreateVertexSourceBindingDescriptor(
				const VertexSourceBindingDescriptorCreateInfo & pCreateInfo,
				pipeline_state_descriptor_id_t pDescriptorID = kPipelineStateDescriptorIDInvalid );

		template <typename TPDescriptorType, typename TPCreateInfo>
		TGfxHandle<TPDescriptorType> CreateCachedDescriptor( const TPCreateInfo & pCreateInfo );

		void ResetCache();

	private:
		static void UpdateDescriptorCoreInfo(
				PipelineStateDescriptor & pDescriptor,
				pipeline_state_descriptor_id_t pDescriptorID,
				const GfxObjectUID & pObjectUID );

	private:
		PipelineStateDescriptorFactory & _descriptorFactory;
		GraphicsPipelineStateDescriptorCache _graphicsPipelineStateDescriptorCache;
	};

	template <typename TPDescriptorType, typename TPCreateInfo>
	inline TGfxHandle<TPDescriptorType> PipelineStateDescriptorManager::CreateCachedDescriptor( const TPCreateInfo & pCreateInfo )
	{
		auto createResult = _graphicsPipelineStateDescriptorCache.CreateDescriptor<TPDescriptorType>( pCreateInfo );
		if( createResult )
		{
			UpdateDescriptorCoreInfo( *createResult.descriptor, createResult.descriptorID, pCreateInfo.gfxObjectUID );
		}
		return createResult.descriptor;
	}

	inline PipelineStateDescriptorHandle PipelineStateDescriptorManager::GetCachedDescriptorByID(
			pipeline_state_descriptor_id_t pDescriptorID ) const noexcept
	{
		const auto descriptorType = CXU::GetPipelineStateDescriptorIDTypeComponent( pDescriptorID );
		return GetCachedDescriptorOfTypeByID( descriptorType, pDescriptorID );
	}

	template <typename TPDescriptorType>
	inline TGfxHandle<TPDescriptorType> PipelineStateDescriptorManager::GetCachedDescriptorOfTypeByID(
			pipeline_state_descriptor_id_t pDescriptorID ) const noexcept
	{
		return _graphicsPipelineStateDescriptorCache.GetDescriptorByID<TPDescriptorType>( pDescriptorID );
	}

	inline bool PipelineStateDescriptorManager::HasCachedDescriptorWithID(
			pipeline_state_descriptor_id_t pDescriptorID ) const noexcept
	{
		const auto descriptorType = CXU::GetPipelineStateDescriptorIDTypeComponent( pDescriptorID );
		return HasCachedDescriptorOfTypeWithID( descriptorType, pDescriptorID );
	}

	template <typename TPDescriptorType>
	inline bool PipelineStateDescriptorManager::HasCachedDescriptorOfTypeWithID( pipeline_state_descriptor_id_t pDescriptorID ) const noexcept
	{
		return _graphicsPipelineStateDescriptorCache.HasDescriptorWithID<TPDescriptorType>( pDescriptorID );
	}

}

#endif // __IC3_GRAPHICS_GCI_SHARED_IMMUTABLE_STATE_MANAGER_H__
