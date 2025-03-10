
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
				pipeline_state_descriptor_id_t pDescriptorID,
				const GraphicsShaderLinkageDescriptorCreateInfo & pCreateInfo ) override = 0;

		virtual VertexAttributeLayoutDescriptorHandle CreateVertexAttributeLayoutDescriptor(
				pipeline_state_descriptor_id_t pDescriptorID,
				const VertexAttributeLayoutDescriptorCreateInfo & pCreateInfo ) override = 0;

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

		virtual RootSignatureDescriptorHandle CreateRootSignatureDescriptor(
				pipeline_state_descriptor_id_t pDescriptorID,
				const RootSignatureDescriptorCreateInfo & pCreateInfo ) override final;

		virtual RenderPassDescriptorHandle CreateRenderPassDescriptor(
				pipeline_state_descriptor_id_t pDescriptorID,
				const RenderPassDescriptorCreateInfo & pCreateInfo ) override final;

		virtual RenderTargetDescriptorHandle CreateRenderTargetDescriptor(
				pipeline_state_descriptor_id_t pDescriptorID,
				const RenderTargetDescriptorCreateInfo & pCreateInfo ) override final;

		virtual VertexSourceBindingDescriptorHandle CreateVertexSourceBindingDescriptor(
				pipeline_state_descriptor_id_t pDescriptorID,
				const VertexSourceBindingDescriptorCreateInfo & pCreateInfo ) override final;
	};

	class GLPipelineStateDescriptorFactoryCore : public GLPipelineStateDescriptorFactory
	{
	public:
		GLPipelineStateDescriptorFactoryCore( GLGPUDevice & pGPUDevice );
		virtual ~GLPipelineStateDescriptorFactoryCore();

		virtual GraphicsShaderLinkageDescriptorHandle CreateGraphicsShaderLinkageDescriptor(
				pipeline_state_descriptor_id_t pDescriptorID,
				const GraphicsShaderLinkageDescriptorCreateInfo & pCreateInfo ) override final;

		virtual VertexAttributeLayoutDescriptorHandle CreateVertexAttributeLayoutDescriptor(
				pipeline_state_descriptor_id_t pDescriptorID,
				const VertexAttributeLayoutDescriptorCreateInfo & pCreateInfo ) override final;
	};

	class GLPipelineStateDescriptorFactoryCompat : public GLPipelineStateDescriptorFactory
	{
	public:
		GLPipelineStateDescriptorFactoryCompat( GLGPUDevice & pGPUDevice );
		virtual ~GLPipelineStateDescriptorFactoryCompat();

		virtual GraphicsShaderLinkageDescriptorHandle CreateGraphicsShaderLinkageDescriptor(
				pipeline_state_descriptor_id_t pDescriptorID,
				const GraphicsShaderLinkageDescriptorCreateInfo & pCreateInfo ) override final;

		virtual VertexAttributeLayoutDescriptorHandle CreateVertexAttributeLayoutDescriptor(
				pipeline_state_descriptor_id_t pDescriptorID,
				const VertexAttributeLayoutDescriptorCreateInfo & pCreateInfo ) override final;
	};

}

#endif // __IC3_GRAPHICS_HW3D_GLC_PIPELINE_IMMUTABLE_STATE_FACTORY_H__
