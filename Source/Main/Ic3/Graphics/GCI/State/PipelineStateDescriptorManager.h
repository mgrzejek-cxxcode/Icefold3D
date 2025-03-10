
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

		CPPX_ATTR_NO_DISCARD PipelineStateDescriptorHandle GetDescriptorByID(
				pipeline_state_descriptor_id_t pDescriptorID ) const noexcept;

		CPPX_ATTR_NO_DISCARD PipelineStateDescriptorHandle GetDescriptorByName(
				const cppx::string_view & pDescriptorName ) const noexcept;

		CPPX_ATTR_NO_DISCARD bool HasDescriptorWithID( pipeline_state_descriptor_id_t pDescriptorID ) const noexcept;

		CPPX_ATTR_NO_DISCARD bool HasDescriptorWithName( const cppx::string_view & pDescriptorName ) const noexcept;

		template <typename TPDescriptorType>
		CPPX_ATTR_NO_DISCARD TGfxHandle<TPDescriptorType> GetCachedDescriptorByID(
				pipeline_state_descriptor_id_t pDescriptorID ) const noexcept;

		template <typename TPDescriptorType>
		CPPX_ATTR_NO_DISCARD TGfxHandle<TPDescriptorType> GetCachedDescriptorByName(
				const cppx::string_view & pDescriptorName ) const noexcept;

		CPPX_ATTR_NO_DISCARD TGfxHandle<PipelineStateDescriptor> GetCachedDescriptorByID(
				EPipelineStateDescriptorType pDescriptorType,
				pipeline_state_descriptor_id_t pDescriptorID ) const noexcept;

		CPPX_ATTR_NO_DISCARD TGfxHandle<PipelineStateDescriptor> GetCachedDescriptorByName(
				EPipelineStateDescriptorType pDescriptorType,
				const cppx::string_view & pDescriptorName ) const noexcept;

		CPPX_ATTR_NO_DISCARD TGfxHandle<PipelineStateDescriptor> GetCachedDescriptorByIDAuto(
				pipeline_state_descriptor_id_t pDescriptorID ) const noexcept;

		template <typename TPDescriptorType>
		CPPX_ATTR_NO_DISCARD bool HasCachedDescriptorWithID( pipeline_state_descriptor_id_t pDescriptorID ) const noexcept;

		template <typename TPDescriptorType>
		CPPX_ATTR_NO_DISCARD bool HasCachedDescriptorWithName( const cppx::string_view & pDescriptorName ) const noexcept;

		CPPX_ATTR_NO_DISCARD bool HasCachedDescriptorWithID(
				EPipelineStateDescriptorType pDescriptorType,
				pipeline_state_descriptor_id_t pDescriptorID ) const noexcept;

		CPPX_ATTR_NO_DISCARD bool HasCachedDescriptorWithName(
				EPipelineStateDescriptorType pDescriptorType,
				const cppx::string_view & pDescriptorName ) const noexcept;

		CPPX_ATTR_NO_DISCARD bool HasCachedDescriptorWithIDAuto(
				pipeline_state_descriptor_id_t pDescriptorID ) const noexcept;

		BlendStateDescriptorHandle CreateBlendStateDescriptor(
				const BlendStateDescriptorCreateInfo & pCreateInfo,
				const cppx::immutable_string & pOptionalDescriptorName = {} );

		DepthStencilStateDescriptorHandle CreateDepthStencilStateDescriptor(
				const DepthStencilStateDescriptorCreateInfo & pCreateInfo,
				const cppx::immutable_string & pOptionalDescriptorName = {} );

		RasterizerStateDescriptorHandle CreateRasterizerStateDescriptor(
				const RasterizerStateDescriptorCreateInfo & pCreateInfo,
				const cppx::immutable_string & pOptionalDescriptorName = {} );

		GraphicsShaderLinkageDescriptorHandle CreateGraphicsShaderLinkageDescriptor(
				const GraphicsShaderLinkageDescriptorCreateInfo & pCreateInfo,
				const cppx::immutable_string & pOptionalDescriptorName = {} );

		VertexAttributeLayoutDescriptorHandle CreateVertexAttributeLayoutDescriptor(
				const VertexAttributeLayoutDescriptorCreateInfo & pCreateInfo,
				const cppx::immutable_string & pOptionalDescriptorName = {} );

		RootSignatureDescriptorHandle CreateRootSignatureDescriptor(
				const RootSignatureDescriptorCreateInfo & pCreateInfo,
				const cppx::immutable_string & pOptionalDescriptorName = {} );

		RenderPassDescriptorHandle CreateRenderPassDescriptor(
				const RenderPassDescriptorCreateInfo & pCreateInfo,
				const cppx::immutable_string & pOptionalDescriptorName = {} );

		RenderTargetDescriptorHandle CreateRenderTargetDescriptor(
				const RenderTargetDescriptorCreateInfo & pCreateInfo,
				const cppx::immutable_string & pOptionalDescriptorName = {} );

		VertexSourceBindingDescriptorHandle CreateVertexSourceBindingDescriptor(
				const VertexSourceBindingDescriptorCreateInfo & pCreateInfo,
				const cppx::immutable_string & pOptionalDescriptorName = {} );

		template <typename TPDescriptorType, typename TPCreateInfo>
		TGfxHandle<TPDescriptorType> CreateCachedDescriptor(
				const TPCreateInfo & pCreateInfo,
				const cppx::immutable_string & pOptionalDescriptorName = {} );

		void ResetCache();

	private:
		CPPX_ATTR_NO_DISCARD bool _ValidateDescriptorCreateInfo( const PipelineStateDescriptorCreateInfoBase & pCreateInfoBase ) const noexcept;

		void _RegisterDescriptorInCommonMap(
				PipelineStateDescriptorHandle pStateDescriptor,
				pipeline_state_descriptor_id_t pDescriptorID,
				cppx::immutable_string pDescriptorName );

	private:
		using CommonCachedDescriptorMapByID = std::unordered_map<pipeline_state_descriptor_id_t, PipelineStateDescriptorHandle>;
		using CommonCachedDescriptorMapByName = std::unordered_map<cppx::immutable_string, PipelineStateDescriptorHandle>;
		PipelineStateDescriptorFactory & _descriptorFactory;
		GraphicsPipelineStateDescriptorCache _graphicsPipelineStateDescriptorCache;
		CommonCachedDescriptorMapByID _commonCachedDescriptorMapByID;
		CommonCachedDescriptorMapByName _commonCachedDescriptorMapByName;
	};

	template <typename TPDescriptorType, typename TPCreateInfo>
	inline TGfxHandle<TPDescriptorType> PipelineStateDescriptorManager::CreateCachedDescriptor(
			const TPCreateInfo & pCreateInfo,
			const cppx::immutable_string & pOptionalDescriptorName )
	{
		return _graphicsPipelineStateDescriptorCache.CreateDescriptor<TPDescriptorType>( pCreateInfo, pOptionalDescriptorName );
	}

	inline PipelineStateDescriptorHandle PipelineStateDescriptorManager::GetDescriptorByID(
			pipeline_state_descriptor_id_t pDescriptorID ) const noexcept
	{
		return cppx::get_map_value_ref_or_default( _commonCachedDescriptorMapByID, pDescriptorID, nullptr );
	}

	inline PipelineStateDescriptorHandle PipelineStateDescriptorManager::GetDescriptorByName(
			const cppx::string_view & pDescriptorName ) const noexcept
	{
		return cppx::get_map_value_ref_or_default( _commonCachedDescriptorMapByName, pDescriptorName, nullptr );
	}

	inline bool PipelineStateDescriptorManager::HasDescriptorWithID( pipeline_state_descriptor_id_t pDescriptorID ) const noexcept
	{
		const auto descriptorPtr = cppx::get_map_value_ptr_or_null( _commonCachedDescriptorMapByID, pDescriptorID );
		return descriptorPtr != nullptr;
	}

	inline bool PipelineStateDescriptorManager::HasDescriptorWithName( const cppx::string_view & pDescriptorName ) const noexcept
	{
		const auto descriptorPtr = cppx::get_map_value_ptr_or_null( _commonCachedDescriptorMapByName, pDescriptorName );
		return descriptorPtr != nullptr;
	}

	template <typename TPDescriptorType>
	inline TGfxHandle<TPDescriptorType> PipelineStateDescriptorManager::GetCachedDescriptorByID(
			pipeline_state_descriptor_id_t pDescriptorID ) const noexcept
	{
		return _graphicsPipelineStateDescriptorCache.GetDescriptorByID<TPDescriptorType>( pDescriptorID );
	}

	template <typename TPDescriptorType>
	inline TGfxHandle<TPDescriptorType> PipelineStateDescriptorManager::GetCachedDescriptorByName(
			const cppx::string_view & pDescriptorName ) const noexcept
	{
		return _graphicsPipelineStateDescriptorCache.GetDescriptorByName<TPDescriptorType>( pDescriptorName );
	}

	inline TGfxHandle<PipelineStateDescriptor> PipelineStateDescriptorManager::GetCachedDescriptorByIDAuto(
			pipeline_state_descriptor_id_t pDescriptorID ) const noexcept
	{
		return GetCachedDescriptorByID( CXU::GetPipelineStateDescriptorIDTypeComponent( pDescriptorID ), pDescriptorID );
	}

	template <typename TPDescriptorType>
	inline bool PipelineStateDescriptorManager::HasCachedDescriptorWithID( pipeline_state_descriptor_id_t pDescriptorID ) const noexcept
	{
		return _graphicsPipelineStateDescriptorCache.HasDescriptorWithID<TPDescriptorType>( pDescriptorID );
	}

	template <typename TPDescriptorType>
	inline bool PipelineStateDescriptorManager::HasCachedDescriptorWithName( const cppx::string_view & pDescriptorName ) const noexcept
	{
		return _graphicsPipelineStateDescriptorCache.HasDescriptorWithName<TPDescriptorType>( pDescriptorName );
	}


	inline bool PipelineStateDescriptorManager::HasCachedDescriptorWithIDAuto(
			pipeline_state_descriptor_id_t pDescriptorID ) const noexcept
	{
		return HasCachedDescriptorWithID( CXU::GetPipelineStateDescriptorIDTypeComponent( pDescriptorID ), pDescriptorID );
	}

}

#endif // __IC3_GRAPHICS_GCI_SHARED_IMMUTABLE_STATE_MANAGER_H__
