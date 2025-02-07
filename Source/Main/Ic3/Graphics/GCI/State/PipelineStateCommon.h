
#pragma once

#ifndef __IC3_GRAPHICS_GCI_PIPELINE_STATE_COMMON_H__
#define __IC3_GRAPHICS_GCI_PIPELINE_STATE_COMMON_H__

#include "CommonGPUStateDefs.h"

namespace Ic3::Graphics::GCI
{

	using pipeline_state_descriptor_id_t = uint16;
	using pipeline_state_object_id_t = uint64;

	enum class EPipelineStateDescriptorType : uint16
	{
		//!! Embedded within the PSO, created solely on the HW3D API level.

		//
		DTBlend,
		DTDepthStencil,
		DTMultiSampling,
		DTRasterizer,
		DTGraphicsShaderLinkage,
		DTIAInputLayout,
		DTShaderRootSignature,

		//!! Not included in the PSO, can be either API-native or dynamic.

		//
		DTIAVertexStream,
		DTRenderPassConfig,

		Unknown
	};

	inline constexpr auto kPipelineStateDescriptorIDAuto = static_cast<pipeline_state_descriptor_id_t>( 0u );

	inline constexpr auto kPipelineStateDescriptorIDInvalid = cppx::meta::limits<pipeline_state_descriptor_id_t>::max_value;

	/*
	 *
	 */
	class PipelineStateDescriptor : public GPUDeviceChildObject
	{
	public:
		pipeline_state_descriptor_id_t const mDescriptorID;

	public:
		PipelineStateDescriptor( GPUDevice & pGPUDevice, pipeline_state_descriptor_id_t pDescriptorID );
		virtual ~PipelineStateDescriptor();

		/**
		 * Queries the dynamic type of the descriptor, expressed as one of the possible values defined in
		 * EPipelineStateDescriptorType enumeration. Implemented by the child descriptor classes.
		 * @return The dynamic type of this descriptor instance.
		 */
		CPPX_ATTR_NO_DISCARD virtual EPipelineStateDescriptorType GetDescriptorType() const noexcept;

		/**
		 * Returns true if this descriptor is a dynamic descriptor. Dynamic descriptor is a descriptor class
		 * that does not contain any API-specific properties (i.e. it is defined fully at the GCI abstraction level).
		 * The following classes are implementations of dynamic descriptors:
		 * - IAVertexStreamStateDescriptorDynamic
		 * - RenderPassConfigStateDescriptorDynamic
		 * - RenderTargetBindingStateDescriptorDynamic
		 * @return
		 */
		CPPX_ATTR_NO_DISCARD virtual bool IsDynamicDescriptor() const noexcept;
	};

	/*
	 * @brief A monolithic state object, containing complete state information for the relative pipeline functionality.
	 * PipelineStateObject serves as a base class for all concrete PSO subtypes. There are two main types of PSOs:
	 * - Compute PSO for the async compute pipeline. Required for executing Dispatch*() calls.
	 * - Graphics PSO for the graphics/rendering pipeline. Required for executing all kind of Draw*() calls.
	 * Both have their dedicated classes: ComputePipelineStateObject and GraphicsPipelineStateObject, respectively.
	 */
	class IC3_GRAPHICS_GCI_CLASS PipelineStateObject : public GPUDeviceChildObject
	{
	public:
		pipeline_state_object_id_t const mStateObjectID;

	public:
		PipelineStateObject( GPUDevice & pGPUDevice, pipeline_state_object_id_t pStateObjectID );
		virtual ~PipelineStateObject();

		/**
		 *
		 */
		CPPX_ATTR_NO_DISCARD virtual EPipelineType GetPipelineType() const noexcept = 0;
	};

	/**
	 *
	 */
	class PipelineStateDescriptorFactory
	{
	public:
		PipelineStateDescriptorFactory() = default;
		virtual ~PipelineStateDescriptorFactory() = default;

		virtual BlendStateDescriptorHandle CreateBlendStateDescriptor(
				pipeline_state_descriptor_id_t pDescriptorID,
				const BlendConfig & pBlendConfig ) = 0;

		virtual DepthStencilStateDescriptorHandle CreateDepthStencilStateDescriptor(
				pipeline_state_descriptor_id_t pDescriptorID,
				const DepthStencilConfig & pDepthStencilConfig ) = 0;

		virtual MultiSamplingStateDescriptorHandle CreateMultiSamplingStateDescriptor(
				pipeline_state_descriptor_id_t pDescriptorID,
				const MultiSamplingConfig & pMultiSamplingConfig ) = 0;

		virtual RasterizerStateDescriptorHandle CreateRasterizerStateDescriptor(
				pipeline_state_descriptor_id_t pDescriptorID,
				const RasterizerConfig & pRasterizerConfig ) = 0;

		virtual GraphicsShaderLinkageStateDescriptorHandle CreateGraphicsShaderLinkageStateDescriptor(
				pipeline_state_descriptor_id_t pDescriptorID,
				const GraphicsShaderBinding & pShaderBinding ) = 0;

		virtual IAInputLayoutStateDescriptorHandle CreateIAInputLayoutStateDescriptor(
				pipeline_state_descriptor_id_t pDescriptorID,
				const IAInputLayoutDefinition & pDefinition,
				Shader & pVertexShaderWithBinary ) = 0;

		virtual IAVertexStreamStateDescriptorHandle CreateIAVertexStreamStateDescriptor(
				pipeline_state_descriptor_id_t pDescriptorID,
				const IAVertexStreamConfiguration & pDefinition ) = 0;

		virtual RenderPassConfigStateDescriptorHandle CreateRenderPassConfigStateDescriptor(
				pipeline_state_descriptor_id_t pDescriptorID,
				const RenderPassConfiguration & pPassConfiguration ) = 0;

		virtual ShaderRootSignatureStateDescriptorHandle CreateShaderRootSignatureStateDescriptor(
				pipeline_state_descriptor_id_t pDescriptorID,
				const ShaderRootSignatureDesc & pRootSignatureDesc ) = 0;
	};

	/**
	 *
	 */
	class PipelineStateDescriptorFactoryNull : public PipelineStateDescriptorFactory
	{
	public:
		PipelineStateDescriptorFactoryNull() = default;
		virtual ~PipelineStateDescriptorFactoryNull() = default;

		virtual BlendStateDescriptorHandle CreateBlendStateDescriptor(
				pipeline_state_descriptor_id_t pDescriptorID,
				const BlendConfig & pBlendConfig ) override final;

		virtual DepthStencilStateDescriptorHandle CreateDepthStencilStateDescriptor(
				pipeline_state_descriptor_id_t pDescriptorID,
				const DepthStencilConfig & pDepthStencilConfig ) override final;

		virtual MultiSamplingStateDescriptorHandle CreateMultiSamplingStateDescriptor(
				pipeline_state_descriptor_id_t pDescriptorID,
				const MultiSamplingConfig & pMultiSamplingConfig ) override final;

		virtual RasterizerStateDescriptorHandle CreateRasterizerStateDescriptor(
				pipeline_state_descriptor_id_t pDescriptorID,
				const RasterizerConfig & pRasterizerConfig ) override final;

		virtual GraphicsShaderLinkageStateDescriptorHandle CreateGraphicsShaderLinkageStateDescriptor(
				pipeline_state_descriptor_id_t pDescriptorID,
				const GraphicsShaderBinding & pShaderBinding ) override final;

		virtual IAInputLayoutStateDescriptorHandle CreateIAInputLayoutStateDescriptor(
				pipeline_state_descriptor_id_t pDescriptorID,
				const IAInputLayoutDefinition & pInputLayoutDefinition,
				Shader & pVertexShaderWithBinary ) override final;

		virtual IAVertexStreamStateDescriptorHandle CreateIAVertexStreamStateDescriptor(
				pipeline_state_descriptor_id_t pDescriptorID,
				const IAVertexStreamConfiguration & pVertexStreamDefinition ) override final;

		virtual RenderPassConfigStateDescriptorHandle CreateRenderPassConfigStateDescriptor(
				pipeline_state_descriptor_id_t pDescriptorID,
				const RenderPassConfiguration & pPassConfiguration ) override final;

		virtual ShaderRootSignatureStateDescriptorHandle CreateShaderRootSignatureStateDescriptor(
				pipeline_state_descriptor_id_t pDescriptorID,
				const ShaderRootSignatureDesc & pRootSignatureDesc ) override final;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_PIPELINE_STATE_COMMON_H__
