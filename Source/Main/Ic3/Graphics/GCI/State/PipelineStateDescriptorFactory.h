
#pragma once

#ifndef __IC3_GRAPHICS_GCI_PIPELINE_STATE_DESCRIPTOR_FACTORY_H__
#define __IC3_GRAPHICS_GCI_PIPELINE_STATE_DESCRIPTOR_FACTORY_H__

#include "CommonGPUStateDefs.h"
#include "PipelineStateCommon.h"

namespace Ic3::Graphics::GCI
{

	/**
	 *
	 */
	class IC3_GRAPHICS_GCI_CLASS PipelineStateDescriptorFactory
	{
	public:
		PipelineStateDescriptorFactory() = default;
		virtual ~PipelineStateDescriptorFactory() = default;

		virtual GPUDevice & GetGPUDevice() const noexcept = 0;

		virtual BlendStateDescriptorHandle CreateBlendStateDescriptor(
				pipeline_state_descriptor_id_t pDescriptorID,
				const BlendStateDescriptorCreateInfo & pCreateInfo ) = 0;

		virtual DepthStencilStateDescriptorHandle CreateDepthStencilStateDescriptor(
				pipeline_state_descriptor_id_t pDescriptorID,
				const DepthStencilStateDescriptorCreateInfo & pCreateInfo ) = 0;

		virtual RasterizerStateDescriptorHandle CreateRasterizerStateDescriptor(
				pipeline_state_descriptor_id_t pDescriptorID,
				const RasterizerStateDescriptorCreateInfo & pCreateInfo ) = 0;

		virtual GraphicsShaderLinkageDescriptorHandle CreateGraphicsShaderLinkageDescriptor(
				pipeline_state_descriptor_id_t pDescriptorID,
				const GraphicsShaderLinkageDescriptorCreateInfo & pCreateInfo ) = 0;

		virtual VertexAttributeLayoutDescriptorHandle CreateVertexAttributeLayoutDescriptor(
				pipeline_state_descriptor_id_t pDescriptorID,
				const VertexAttributeLayoutDescriptorCreateInfo & pCreateInfo ) = 0;

		virtual RootSignatureDescriptorHandle CreateRootSignatureDescriptor(
				pipeline_state_descriptor_id_t pDescriptorID,
				const RootSignatureDescriptorCreateInfo & pCreateInfo) = 0;

		virtual RenderPassDescriptorHandle CreateRenderPassDescriptor(
			pipeline_state_descriptor_id_t pDescriptorID,
				const RenderPassDescriptorCreateInfo & pCreateInfo ) = 0;

		virtual RenderTargetDescriptorHandle CreateRenderTargetDescriptor(
				pipeline_state_descriptor_id_t pDescriptorID,
				const RenderTargetDescriptorCreateInfo & pCreateInfo ) = 0;

		virtual VertexSourceBindingDescriptorHandle CreateVertexSourceBindingDescriptor(
				pipeline_state_descriptor_id_t pDescriptorID,
				const VertexSourceBindingDescriptorCreateInfo & pCreateInfo ) = 0;
	};

	/**
	 *
	 */
	class IC3_GRAPHICS_GCI_CLASS PipelineStateDescriptorFactoryNull : public PipelineStateDescriptorFactory
	{
	public:
		PipelineStateDescriptorFactoryNull() = default;
		virtual ~PipelineStateDescriptorFactoryNull() = default;

		virtual GPUDevice & GetGPUDevice() const noexcept override final;

		virtual BlendStateDescriptorHandle CreateBlendStateDescriptor(
				pipeline_state_descriptor_id_t pDescriptorID,
				const BlendStateDescriptorCreateInfo & pCreateInfo ) override final;

		virtual DepthStencilStateDescriptorHandle CreateDepthStencilStateDescriptor(
				pipeline_state_descriptor_id_t pDescriptorID,
				const DepthStencilStateDescriptorCreateInfo & pCreateInfo ) override final;

		virtual RasterizerStateDescriptorHandle CreateRasterizerStateDescriptor(
				pipeline_state_descriptor_id_t pDescriptorID,
				const RasterizerStateDescriptorCreateInfo & pCreateInfo ) override final;

		virtual GraphicsShaderLinkageDescriptorHandle CreateGraphicsShaderLinkageDescriptor(
				pipeline_state_descriptor_id_t pDescriptorID,
				const GraphicsShaderLinkageDescriptorCreateInfo & pCreateInfo ) override final;

		virtual VertexAttributeLayoutDescriptorHandle CreateVertexAttributeLayoutDescriptor(
				pipeline_state_descriptor_id_t pDescriptorID,
				const VertexAttributeLayoutDescriptorCreateInfo & pCreateInfo ) override final;

		virtual RootSignatureDescriptorHandle CreateRootSignatureDescriptor(
				pipeline_state_descriptor_id_t pDescriptorID,
				const RootSignatureDescriptorCreateInfo & pCreateInfo ) override final;

		virtual RenderPassDescriptorHandle CreateRenderPassDescriptor(
				pipeline_state_descriptor_id_t pDescriptorID,
				const RenderPassDescriptorCreateInfo & pCreateInfo ) override final;

		virtual VertexSourceBindingDescriptorHandle CreateVertexSourceBindingDescriptor(
				pipeline_state_descriptor_id_t pDescriptorID,
				const VertexSourceBindingDescriptorCreateInfo & pCreateInfo ) override final;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_PIPELINE_STATE_DESCRIPTOR_FACTORY_H__
