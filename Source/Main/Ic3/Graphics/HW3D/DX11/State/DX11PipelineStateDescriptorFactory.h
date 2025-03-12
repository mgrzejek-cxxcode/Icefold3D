
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_DX11_PIPELINE_IMMUTABLE_STATE_FACTORY_H__
#define __IC3_GRAPHICS_HW3D_DX11_PIPELINE_IMMUTABLE_STATE_FACTORY_H__

#include "../DX11Prerequisites.h"
#include <Ic3/Graphics/GCI/State/PipelineStateDescriptorFactory.h>

namespace Ic3::Graphics::GCI
{

	class IC3_GX_DX11_CLASS DX11PipelineStateDescriptorFactory : public PipelineStateDescriptorFactory
	{
	public:
		DX11GPUDevice & mDX11GPUDevice;

	public:
		DX11PipelineStateDescriptorFactory( DX11GPUDevice & pGPUDevice );
		virtual ~DX11PipelineStateDescriptorFactory();

		virtual GPUDevice & GetGPUDevice() const noexcept override final;

		virtual BlendStateDescriptorHandle CreateBlendStateDescriptor(
			const BlendStateDescriptorCreateInfo & pCreateInfo ) override final;

		virtual DepthStencilStateDescriptorHandle CreateDepthStencilStateDescriptor(
			const DepthStencilStateDescriptorCreateInfo & pCreateInfo ) override final;

		virtual RasterizerStateDescriptorHandle CreateRasterizerStateDescriptor(
			const RasterizerStateDescriptorCreateInfo & pCreateInfo ) override final;

		virtual GraphicsShaderLinkageDescriptorHandle CreateGraphicsShaderLinkageDescriptor(
			const GraphicsShaderLinkageDescriptorCreateInfo & pCreateInfo ) override final;

		virtual VertexAttributeLayoutDescriptorHandle CreateVertexAttributeLayoutDescriptor(
			const VertexAttributeLayoutDescriptorCreateInfo & pCreateInfo ) override final;

		virtual RootSignatureDescriptorHandle CreateRootSignatureDescriptor(
			const RootSignatureDescriptorCreateInfo & pCreateInfo ) override final;

		virtual RenderPassDescriptorHandle CreateRenderPassDescriptor(
			const RenderPassDescriptorCreateInfo & pCreateInfo ) override final;

		virtual RenderTargetDescriptorHandle CreateRenderTargetDescriptor(
			const RenderTargetDescriptorCreateInfo & pCreateInfo ) override final;

		virtual VertexSourceBindingDescriptorHandle CreateVertexSourceBindingDescriptor(
			const VertexSourceBindingDescriptorCreateInfo & pCreateInfo ) override final;
	};
	
} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_DX11_PIPELINE_IMMUTABLE_STATE_FACTORY_H__
