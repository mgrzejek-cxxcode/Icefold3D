
#include "GLPipelineStateDescriptorFactory.h"
#include "GLGraphicsPipelineStateCommon.h"
#include "GLGraphicsPipelineStateShader.h"
#include "GLGraphicsPipelineStateIA.h"
#include "GLGraphicsPipelineStateRTO.h"
#include <Ic3/Graphics/HW3D/GL/GLGPUDevice.h>
#include <IC3/Graphics/GCI/State/GraphicsPipelineStateImplRenderPassGeneric.h>
#include <IC3/Graphics/GCI/State/PipelineStateImplRootSignatureGeneric.h>

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
			const BlendStateDescriptorCreateInfo & pCreateInfo )
	{
		return GLBlendStateDescriptor::CreateInstance( mGLGPUDevice, pCreateInfo );
	}

	DepthStencilStateDescriptorHandle GLPipelineStateDescriptorFactory::CreateDepthStencilStateDescriptor(
			const DepthStencilStateDescriptorCreateInfo & pCreateInfo )
	{
		return GLDepthStencilStateDescriptor::CreateInstance( mGLGPUDevice, pCreateInfo );
	}

	RasterizerStateDescriptorHandle GLPipelineStateDescriptorFactory::CreateRasterizerStateDescriptor(
			const RasterizerStateDescriptorCreateInfo & pCreateInfo )
	{
		return GLRasterizerStateDescriptor::CreateInstance( mGLGPUDevice, pCreateInfo );
	}

	RootSignatureDescriptorHandle GLPipelineStateDescriptorFactory::CreateRootSignatureDescriptor(
			const RootSignatureDescriptorCreateInfo & pCreateInfo )
	{
		return RootSignatureDescriptorGeneric::CreateInstance( mGLGPUDevice, pCreateInfo );
	}

	RenderPassDescriptorHandle GLPipelineStateDescriptorFactory::CreateRenderPassDescriptor(
			const RenderPassDescriptorCreateInfo & pCreateInfo )
	{
		return RenderPassDescriptorGeneric::CreateInstance(
			mGLGPUDevice,
			pCreateInfo,
			{
				eRenderPassAttachmentActionFlagLoadClearBit,
				eRenderPassAttachmentActionFlagStoreResolveBit
			});
	}

	RenderTargetDescriptorHandle GLPipelineStateDescriptorFactory::CreateRenderTargetDescriptor(
			const RenderTargetDescriptorCreateInfo & pCreateInfo )
	{
		return GLRenderTargetDescriptor::CreateInstance( mGLGPUDevice, pCreateInfo );
	}

	VertexSourceBindingDescriptorHandle GLPipelineStateDescriptorFactory::CreateVertexSourceBindingDescriptor(
			const VertexSourceBindingDescriptorCreateInfo & pCreateInfo )
	{
		return GLVertexSourceBindingDescriptor::CreateInstance( mGLGPUDevice, pCreateInfo );
	}


	GLPipelineStateDescriptorFactoryCore::GLPipelineStateDescriptorFactoryCore( GLGPUDevice & pGPUDevice )
	: GLPipelineStateDescriptorFactory( pGPUDevice )
	{}

	GLPipelineStateDescriptorFactoryCore::~GLPipelineStateDescriptorFactoryCore() = default;

	GraphicsShaderLinkageDescriptorHandle GLPipelineStateDescriptorFactoryCore::CreateGraphicsShaderLinkageDescriptor(
			const GraphicsShaderLinkageDescriptorCreateInfo & pCreateInfo )
	{
		return GLGraphicsShaderLinkageDescriptorCore::CreateInstance( mGLGPUDevice, pCreateInfo );
	}

	VertexAttributeLayoutDescriptorHandle GLPipelineStateDescriptorFactoryCore::CreateVertexAttributeLayoutDescriptor(
			const VertexAttributeLayoutDescriptorCreateInfo & pCreateInfo )
	{
		return GLVertexAttributeLayoutDescriptorCore::CreateInstance( mGLGPUDevice, pCreateInfo );
	}


	GLPipelineStateDescriptorFactoryCompat::GLPipelineStateDescriptorFactoryCompat( GLGPUDevice & pGPUDevice )
	: GLPipelineStateDescriptorFactory( pGPUDevice )
	{}

	GLPipelineStateDescriptorFactoryCompat::~GLPipelineStateDescriptorFactoryCompat() = default;

	GraphicsShaderLinkageDescriptorHandle GLPipelineStateDescriptorFactoryCompat::CreateGraphicsShaderLinkageDescriptor(
			const GraphicsShaderLinkageDescriptorCreateInfo & pCreateInfo )
	{
		return GLGraphicsShaderLinkageDescriptorCompat::CreateInstance( mGLGPUDevice, pCreateInfo );
	}

	VertexAttributeLayoutDescriptorHandle GLPipelineStateDescriptorFactoryCompat::CreateVertexAttributeLayoutDescriptor(
			const VertexAttributeLayoutDescriptorCreateInfo & pCreateInfo )
	{
		return GLVertexAttributeLayoutDescriptorCompat::CreateInstance( mGLGPUDevice, pCreateInfo );
	}

}
