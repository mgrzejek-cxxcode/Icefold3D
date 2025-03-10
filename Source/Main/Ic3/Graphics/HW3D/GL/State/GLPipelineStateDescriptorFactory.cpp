
#include "GLPipelineStateDescriptorFactory.h"
#include "GLGraphicsPipelineStateCommon.h"
#include "GLGraphicsPipelineStateShader.h"
#include "GLGraphicsPipelineStateIA.h"
#include "GLGraphicsPipelineStateRTO.h"
#include "../GLGPUDevice.h"
#include <IC3/Graphics/GCI/State/PipelineStateDescriptorRootSignature.h>
#include <IC3/Graphics/GCI/State/GraphicsPipelineStateImplGenericRenderPass.h>

namespace Ic3::Graphics::GCI
{

	GLPipelineStateDescriptorFactory::GLPipelineStateDescriptorFactory( GLGPUDevice & pGPUDevice )
	: PipelineStateDescriptorFactory()
	, mGLGPUDevice( pGPUDevice )
	{}

	GLPipelineStateDescriptorFactory::~GLPipelineStateDescriptorFactory() = default;

	GPUDevice & GLPipelineStateDescriptorFactory::GetGPUDevice() const noexcept
	{
		return mGLGPUDevice;
	}

	BlendStateDescriptorHandle GLPipelineStateDescriptorFactory::CreateBlendStateDescriptor(
			pipeline_state_descriptor_id_t pDescriptorID,
			const BlendStateDescriptorCreateInfo & pCreateInfo )
	{
		return GLBlendStateDescriptor::CreateInstance( mGLGPUDevice, pDescriptorID, pCreateInfo );
	}

	DepthStencilStateDescriptorHandle GLPipelineStateDescriptorFactory::CreateDepthStencilStateDescriptor(
			pipeline_state_descriptor_id_t pDescriptorID,
			const DepthStencilStateDescriptorCreateInfo & pCreateInfo )
	{
		return GLDepthStencilStateDescriptor::CreateInstance( mGLGPUDevice, pDescriptorID, pCreateInfo );
	}

	RasterizerStateDescriptorHandle GLPipelineStateDescriptorFactory::CreateRasterizerStateDescriptor(
			pipeline_state_descriptor_id_t pDescriptorID,
			const RasterizerStateDescriptorCreateInfo & pCreateInfo )
	{
		return GLRasterizerStateDescriptor::CreateInstance( mGLGPUDevice, pDescriptorID, pCreateInfo );
	}

	RootSignatureDescriptorHandle GLPipelineStateDescriptorFactory::CreateRootSignatureDescriptor(
			pipeline_state_descriptor_id_t pDescriptorID,
			const RootSignatureDescriptorCreateInfo & pCreateInfo )
	{
		return PIM::RootSignatureDescriptorGeneric::CreateInstance( mGLGPUDevice, pDescriptorID, pCreateInfo );
	}

	RenderPassDescriptorHandle GLPipelineStateDescriptorFactory::CreateRenderPassDescriptor(
			pipeline_state_descriptor_id_t pDescriptorID,
			const RenderPassDescriptorCreateInfo & pCreateInfo )
	{
		return RenderPassDescriptorGeneric::CreateInstance(
			mGLGPUDevice,
			pDescriptorID,
			pCreateInfo,
			{
				eRenderPassAttachmentActionFlagLoadClearBit,
				eRenderPassAttachmentActionFlagStoreResolveBit
			});
	}

	RenderTargetDescriptorHandle GLPipelineStateDescriptorFactory::CreateRenderTargetDescriptor(
			pipeline_state_descriptor_id_t pDescriptorID,
			const RenderTargetDescriptorCreateInfo & pCreateInfo )
	{
		return GLRenderTargetDescriptor::CreateInstance( mGLGPUDevice, pDescriptorID, pCreateInfo );
	}

	VertexSourceBindingDescriptorHandle GLPipelineStateDescriptorFactory::CreateVertexSourceBindingDescriptor(
			pipeline_state_descriptor_id_t pDescriptorID,
			const VertexSourceBindingDescriptorCreateInfo & pCreateInfo )
	{
		return GLVertexSourceBindingDescriptor::CreateInstance( mGLGPUDevice, pDescriptorID, pCreateInfo );
	}


	GLPipelineStateDescriptorFactoryCore::GLPipelineStateDescriptorFactoryCore( GLGPUDevice & pGPUDevice )
	: GLPipelineStateDescriptorFactory( pGPUDevice )
	{}

	GLPipelineStateDescriptorFactoryCore::~GLPipelineStateDescriptorFactoryCore() = default;

	GraphicsShaderLinkageDescriptorHandle GLPipelineStateDescriptorFactoryCore::CreateGraphicsShaderLinkageDescriptor(
			pipeline_state_descriptor_id_t pDescriptorID,
			const GraphicsShaderLinkageDescriptorCreateInfo & pCreateInfo )
	{
		return GLGraphicsShaderLinkageDescriptorCore::CreateInstance( mGLGPUDevice, pDescriptorID, pCreateInfo );
	}

	VertexAttributeLayoutDescriptorHandle GLPipelineStateDescriptorFactoryCore::CreateVertexAttributeLayoutDescriptor(
			pipeline_state_descriptor_id_t pDescriptorID,
			const VertexAttributeLayoutDescriptorCreateInfo & pCreateInfo )
	{
		return GLVertexAttributeLayoutDescriptorCore::CreateInstance( mGLGPUDevice, pDescriptorID, pCreateInfo );
	}


	GLPipelineStateDescriptorFactoryCompat::GLPipelineStateDescriptorFactoryCompat( GLGPUDevice & pGPUDevice )
	: GLPipelineStateDescriptorFactory( pGPUDevice )
	{}

	GLPipelineStateDescriptorFactoryCompat::~GLPipelineStateDescriptorFactoryCompat() = default;

	GraphicsShaderLinkageDescriptorHandle GLPipelineStateDescriptorFactoryCompat::CreateGraphicsShaderLinkageDescriptor(
			pipeline_state_descriptor_id_t pDescriptorID,
			const GraphicsShaderLinkageDescriptorCreateInfo & pCreateInfo )
	{
		return GLGraphicsShaderLinkageDescriptorCompat::CreateInstance( mGLGPUDevice, pDescriptorID, pCreateInfo );
	}

	VertexAttributeLayoutDescriptorHandle GLPipelineStateDescriptorFactoryCompat::CreateVertexAttributeLayoutDescriptor(
			pipeline_state_descriptor_id_t pDescriptorID,
			const VertexAttributeLayoutDescriptorCreateInfo & pCreateInfo )
	{
		return GLVertexAttributeLayoutDescriptorCompat::CreateInstance( mGLGPUDevice, pDescriptorID, pCreateInfo );
	}

}
