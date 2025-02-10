
#pragma once

#ifndef __IC3_GRAPHICS_GCI_SHARED_IMMUTABLE_STATE_MANAGER_H__
#define __IC3_GRAPHICS_GCI_SHARED_IMMUTABLE_STATE_MANAGER_H__

#include "PipelineStateDescriptorCache.h"

namespace Ic3::Graphics::GCI
{

	class PipelineStateDescriptorManager : public GPUDeviceChildObject
	{
		friend class GPUDevice;

	public:
		PipelineStateDescriptorManager( GPUDevice & pGPUDevice, PipelineStateDescriptorFactory & pDescriptorFactory );
		virtual ~PipelineStateDescriptorManager();

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
		CPPX_ATTR_NO_DISCARD bool HasCachedDescriptorWithID(
				pipeline_state_descriptor_id_t pDescriptorID ) const noexcept;

		template <typename TPDescriptorType>
		CPPX_ATTR_NO_DISCARD bool HasCachedDescriptorWithName(
				const cppx::string_view & pDescriptorName ) const noexcept;

		CPPX_ATTR_NO_DISCARD bool HasCachedDescriptorWithID(
				EPipelineStateDescriptorType pDescriptorType,
				pipeline_state_descriptor_id_t pDescriptorID ) const noexcept;

		CPPX_ATTR_NO_DISCARD bool HasCachedDescriptorWithName(
				EPipelineStateDescriptorType pDescriptorType,
				const cppx::string_view & pDescriptorName ) const noexcept;

		CPPX_ATTR_NO_DISCARD bool HasCachedDescriptorWithIDAuto(
				pipeline_state_descriptor_id_t pDescriptorID ) const noexcept;

		CPPX_ATTR_NO_DISCARD const BlendSettings & GetDefaultBlendSettings();
		CPPX_ATTR_NO_DISCARD BlendStateDescriptorHandle GetDefaultBlendStateDescriptor();

		CPPX_ATTR_NO_DISCARD const DepthStencilSettings & GetDefaultDepthStencilSettings();
		CPPX_ATTR_NO_DISCARD DepthStencilStateDescriptorHandle GetDefaultDepthStencilStateDescriptor();

		CPPX_ATTR_NO_DISCARD const DepthStencilSettings & GetDefaultDepthStencilSettingsWithDepthTestEnabled();
		CPPX_ATTR_NO_DISCARD DepthStencilStateDescriptorHandle GetDefaultDepthStencilStateDescriptorWithDepthTestEnabled();

		CPPX_ATTR_NO_DISCARD const RasterizerSettings & GetDefaultRasterizerSettings();
		CPPX_ATTR_NO_DISCARD RasterizerStateDescriptorHandle GetDefaultRasterizerStateDescriptor();

		CPPX_ATTR_NO_DISCARD BlendStateDescriptorHandle CreateBlendStateDescriptor(
				const BlendStateDescriptorCreateInfo & pCreateInfo,
				const cppx::immutable_string & pOptionalDescriptorName = {} );

		CPPX_ATTR_NO_DISCARD DepthStencilStateDescriptorHandle CreateDepthStencilStateDescriptor(
				const DepthStencilStateDescriptorCreateInfo & pCreateInfo,
				const cppx::immutable_string & pOptionalDescriptorName = {} );

		CPPX_ATTR_NO_DISCARD GraphicsShaderLinkageStateDescriptorHandle CreateGraphicsShaderLinkageStateDescriptor(
				const GraphicsShaderLinkageStateDescriptorCreateInfo & pCreateInfo,
				const cppx::immutable_string & pOptionalDescriptorName = {} );

		CPPX_ATTR_NO_DISCARD IAVertexAttributeLayoutStateDescriptorHandle CreateIAVertexAttributeLayoutStateDescriptor(
				const IAVertexAttributeLayoutStateDescriptorCreateInfo & pCreateInfo,
				const cppx::immutable_string & pOptionalDescriptorName = {} );

		CPPX_ATTR_NO_DISCARD RasterizerStateDescriptorHandle CreateRasterizerStateDescriptor(
				const RasterizerStateDescriptorCreateInfo & pCreateInfo,
				const cppx::immutable_string & pOptionalDescriptorName = {} );

		CPPX_ATTR_NO_DISCARD ShaderRootSignatureStateDescriptorHandle CreateShaderRootSignatureStateDescriptor(
				const ShaderRootSignatureStateDescriptorCreateInfo & pCreateInfo,
				const cppx::immutable_string & pOptionalDescriptorName = {} );

		CPPX_ATTR_NO_DISCARD IAVertexStreamBindingStateDescriptorHandle CreateIAVertexStreamBindingStateDescriptor(
				const IAVertexStreamBindingStateDescriptorCreateInfo & pCreateInfo,
				const cppx::immutable_string & pOptionalDescriptorName = {} );

		CPPX_ATTR_NO_DISCARD RenderPassConfigurationStateDescriptorHandle CreateRenderPassConfigurationStateDescriptor(
				const RenderPassConfigurationStateDescriptorCreateInfo & pCreateInfo,
				const cppx::immutable_string & pOptionalDescriptorName = {} );

		template <typename TPDescriptorType, typename TPCreateInfo>
		CPPX_ATTR_NO_DISCARD TGfxHandle<TPDescriptorType> CreateCachedDescriptor(
				const TPCreateInfo & pCreateInfo,
				const cppx::immutable_string & pOptionalDescriptorName = {} );

		template <typename TPDescriptorType>
		void ResetDescriptorCache();

		void ResetDescriptorCache( EPipelineStateDescriptorType pDescriptorType );

		void Reset( cppx::bitmask<EPipelineStateDescriptorTypeFlags> pResetMask = ePipelineStateDescriptorTypeMaskAll );

	friendapi:
		///
		bool CreateDefaultStateDescriptors();

	private:
		PipelineStateDescriptorFactory & _descriptorFactory;
		GraphicsPipelineStateDescriptorCache _graphicsPipelineStateDescriptorCache;
	};

	template <typename TPDescriptorType, typename TPCreateInfo>
	inline TGfxHandle<TPDescriptorType> PipelineStateDescriptorManager::CreateCachedDescriptor(
			const TPCreateInfo & pCreateInfo,
			const cppx::immutable_string & pOptionalDescriptorName )
	{
		return _graphicsPipelineStateDescriptorCache.CreateDescriptor<TPDescriptorType>( pCreateInfo, pOptionalDescriptorName );
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
	inline bool PipelineStateDescriptorManager::HasCachedDescriptorWithID(
			pipeline_state_descriptor_id_t pDescriptorID ) const noexcept
	{
		return _graphicsPipelineStateDescriptorCache.HasDescriptorWithID<TPDescriptorType>( pDescriptorID );
	}

	template <typename TPDescriptorType>
	inline bool PipelineStateDescriptorManager::HasCachedDescriptorWithName(
			const cppx::string_view & pDescriptorName ) const noexcept
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
