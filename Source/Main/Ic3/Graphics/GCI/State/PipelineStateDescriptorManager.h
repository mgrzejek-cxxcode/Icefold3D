
#pragma once

#ifndef __IC3_GRAPHICS_GCI_SHARED_IMMUTABLE_STATE_MANAGER_H__
#define __IC3_GRAPHICS_GCI_SHARED_IMMUTABLE_STATE_MANAGER_H__

#include "PipelineStateDescriptorCache.h"

namespace Ic3::Graphics::GCI
{

	using sm_separable_state_id_t = uint32;
	using sm_separable_state_combined_id_mask_t = uint64;

	using sm_blend_state_underlying_id_t = uint8;
	using sm_depth_stencil_state_underlying_id_t = uint8;
	using sm_input_layout_state_underlying_id_t = uint8;
	using sm_rasterizer_state_underlying_id_t = uint8;
	using sm_shader_binding_state_underlying_id_t = uint16;
	using sm_shader_input_signature_state_underlying_id_t = uint16;
	using sm_render_target_layout_state_underlying_id_t = uint8;

	struct PipelineStateObjectID
	{
		sm_separable_state_combined_id_mask_t immutableStateIDMask;
		sm_render_target_layout_state_underlying_id_t renderTargetLayoutID;
	};

	class PipelineSeparableStateCache
	{
	};

	class PipelineStateObjectCache
	{
	public:

	};

	class PipelineStateManager
	{
		friend class GPUDevice;

	public:
		CPPX_ATTR_NO_DISCARD BlendStateDescriptorHandle CreateBlendStateDescriptor( const BlendConfig & pConfig );
		CPPX_ATTR_NO_DISCARD BlendStateDescriptorHandle CreateBlendStateDescriptor( pipeline_state_descriptor_id_t pDescriptorID, const BlendConfig & pConfig );

		CPPX_ATTR_NO_DISCARD DepthStencilStateDescriptorHandle CreateDepthStencilStateDescriptor( const DepthStencilConfig & pConfig );
		CPPX_ATTR_NO_DISCARD DepthStencilStateDescriptorHandle CreateDepthStencilStateDescriptor( pipeline_state_descriptor_id_t pDescriptorID, const DepthStencilConfig & pConfig );

		CPPX_ATTR_NO_DISCARD GraphicsShaderLinkageStateDescriptorHandle CreateGraphicsShaderLinkageStateDescriptor( const GraphicsShaderSet & pShaderSet );
		CPPX_ATTR_NO_DISCARD GraphicsShaderLinkageStateDescriptorHandle CreateGraphicsShaderLinkageStateDescriptor( pipeline_state_descriptor_id_t pDescriptorID, const GraphicsShaderSet & pShaderSet );

		CPPX_ATTR_NO_DISCARD IAInputLayoutStateDescriptorHandle CreateIAInputLayoutStateDescriptor( const IAInputLayoutDefinition & pDefinition, Shader & pVertexShaderWithBinary );
		CPPX_ATTR_NO_DISCARD IAInputLayoutStateDescriptorHandle CreateIAInputLayoutStateDescriptor( pipeline_state_descriptor_id_t pDescriptorID, const IAInputLayoutDefinition & pDefinition, Shader & pVertexShaderWithBinary );

		CPPX_ATTR_NO_DISCARD RasterizerStateDescriptorHandle CreateRasterizerStateDescriptor( const RasterizerConfig & pConfig );
		CPPX_ATTR_NO_DISCARD RasterizerStateDescriptorHandle CreateRasterizerStateDescriptor( pipeline_state_descriptor_id_t pDescriptorID, const RasterizerConfig & pConfig );

		CPPX_ATTR_NO_DISCARD MultiSamplingStateDescriptorHandle CreateMultiSamplingStateDescriptor( const MultiSamplingConfig & pConfig );
		CPPX_ATTR_NO_DISCARD MultiSamplingStateDescriptorHandle CreateMultiSamplingStateDescriptor( pipeline_state_descriptor_id_t pDescriptorID, const MultiSamplingConfig & pConfig );

		CPPX_ATTR_NO_DISCARD ShaderRootSignatureStateDescriptorHandle CreateShaderRootSignatureStateDescriptor( const ShaderRootSignature & pRootSignature );
		CPPX_ATTR_NO_DISCARD ShaderRootSignatureStateDescriptorHandle CreateShaderRootSignatureStateDescriptor( pipeline_state_descriptor_id_t pDescriptorID, const ShaderRootSignature & pRootSignature );

		CPPX_ATTR_NO_DISCARD const BlendConfig & GetDefaultBlendConfig();

		CPPX_ATTR_NO_DISCARD BlendStateDescriptorHandle GetDefaultBlendStateDescriptor();

		CPPX_ATTR_NO_DISCARD const DepthStencilConfig & GetDefaultDepthStencilConfig();
		CPPX_ATTR_NO_DISCARD const DepthStencilConfig & GetDefaultDepthStencilConfigWithDepthTestEnabled();

		CPPX_ATTR_NO_DISCARD DepthStencilStateDescriptorHandle GetDefaultDepthStencilStateDescriptor();

		CPPX_ATTR_NO_DISCARD const RasterizerConfig & GetDefaultRasterizerConfig();

		CPPX_ATTR_NO_DISCARD RasterizerStateDescriptorHandle GetDefaultRasterizerStateDescriptor();

		CPPX_ATTR_NO_DISCARD const MultiSamplingConfig & GetDefaultMultiSamplingConfig();

		CPPX_ATTR_NO_DISCARD MultiSamplingStateDescriptorHandle GetDefaultMultiSamplingStateDescriptor();

	friendapi:
		void CreateDefaultStateDescriptors();

	private:
		GraphicsPipelineStateDescriptorCache _graphicsPipelineStateDescriptorCache;
	};

}

#endif // __IC3_GRAPHICS_GCI_SHARED_IMMUTABLE_STATE_MANAGER_H__
