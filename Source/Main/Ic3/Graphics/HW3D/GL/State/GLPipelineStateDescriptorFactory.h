
#ifndef __IC3_GRAPHICS_HW3D_GLC_PIPELINE_IMMUTABLE_STATE_FACTORY_H__
#define __IC3_GRAPHICS_HW3D_GLC_PIPELINE_IMMUTABLE_STATE_FACTORY_H__

#include "../GLPrerequisites.h"
#include <Ic3/Graphics/GCI/State/PipelineStateDescriptorFactory.h>

namespace Ic3::Graphics::GCI
{

	class GLPipelineStateDescriptorFactory : public PipelineStateDescriptorFactory
	{
	public:
		GLGPUDevice & mGLGPUDevice;

	public:
		GLPipelineStateDescriptorFactory( GLGPUDevice & pGPUDevice );
		virtual ~GLPipelineStateDescriptorFactory();

		virtual GraphicsShaderLinkageDescriptorHandle CreateGraphicsShaderLinkageDescriptor(
				const GraphicsShaderLinkageDescriptorCreateInfo & pCreateInfo ) override = 0;

		virtual VertexAttributeLayoutDescriptorHandle CreateVertexAttributeLayoutDescriptor(
				const VertexAttributeLayoutDescriptorCreateInfo & pCreateInfo ) override = 0;

		virtual GPUDevice & GetGPUDevice() const noexcept override final;

		virtual BlendStateDescriptorHandle CreateBlendStateDescriptor(
				const BlendStateDescriptorCreateInfo & pCreateInfo ) override final;

		virtual DepthStencilStateDescriptorHandle CreateDepthStencilStateDescriptor(
				const DepthStencilStateDescriptorCreateInfo & pCreateInfo ) override final;

		virtual RasterizerStateDescriptorHandle CreateRasterizerStateDescriptor(
				const RasterizerStateDescriptorCreateInfo & pCreateInfo ) override final;

		virtual RootSignatureDescriptorHandle CreateRootSignatureDescriptor(
				const RootSignatureDescriptorCreateInfo & pCreateInfo ) override final;

		virtual RenderPassDescriptorHandle CreateRenderPassDescriptor(
				const RenderPassDescriptorCreateInfo & pCreateInfo ) override final;

		virtual RenderTargetDescriptorHandle CreateRenderTargetDescriptor(
				const RenderTargetDescriptorCreateInfo & pCreateInfo ) override final;

		virtual VertexSourceBindingDescriptorHandle CreateVertexSourceBindingDescriptor(
				const VertexSourceBindingDescriptorCreateInfo & pCreateInfo ) override final;
	};

	class GLPipelineStateDescriptorFactoryCore : public GLPipelineStateDescriptorFactory
	{
	public:
		GLPipelineStateDescriptorFactoryCore( GLGPUDevice & pGPUDevice );
		virtual ~GLPipelineStateDescriptorFactoryCore();

		virtual GraphicsShaderLinkageDescriptorHandle CreateGraphicsShaderLinkageDescriptor(
				const GraphicsShaderLinkageDescriptorCreateInfo & pCreateInfo ) override final;

		virtual VertexAttributeLayoutDescriptorHandle CreateVertexAttributeLayoutDescriptor(
				const VertexAttributeLayoutDescriptorCreateInfo & pCreateInfo ) override final;
	};

	class GLPipelineStateDescriptorFactoryCompat : public GLPipelineStateDescriptorFactory
	{
	public:
		GLPipelineStateDescriptorFactoryCompat( GLGPUDevice & pGPUDevice );
		virtual ~GLPipelineStateDescriptorFactoryCompat();

		virtual GraphicsShaderLinkageDescriptorHandle CreateGraphicsShaderLinkageDescriptor(
				const GraphicsShaderLinkageDescriptorCreateInfo & pCreateInfo ) override final;

		virtual VertexAttributeLayoutDescriptorHandle CreateVertexAttributeLayoutDescriptor(
				const VertexAttributeLayoutDescriptorCreateInfo & pCreateInfo ) override final;
	};

}

#endif // __IC3_GRAPHICS_HW3D_GLC_PIPELINE_IMMUTABLE_STATE_FACTORY_H__
