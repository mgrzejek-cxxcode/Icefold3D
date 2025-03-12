
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_DX11_GRAPHICS_PIPELINE_STATE_OBJECT_H__
#define __IC3_GRAPHICS_HW3D_DX11_GRAPHICS_PIPELINE_STATE_OBJECT_H__

#include "../DX11Prerequisites.h"
#include <Ic3/Graphics/GCI/State/GraphicsPipelineStateImplSeparableState.h>
#include <Ic3/Graphics/GCI/State/PipelineStateImplRootSignatureGeneric.h>

namespace Ic3::Graphics::GCI
{

	class IC3_GX_DX11_CLASS DX11GraphicsPipelineStateObject : public GraphicsPipelineStateObjectSeparableShader
	{
	public:
		DX11GraphicsPipelineStateObject(
				DX11GPUDevice & pGPUDevice,
				pipeline_state_object_id_t pStateObjectID,
				const RenderTargetLayout & pRenderTargetLayout,
				const SeparableGraphicsPSDSet & pSeparableDescriptors,
				const GraphicsShaderBinding & pGraphicsShaderBinding );

		virtual ~DX11GraphicsPipelineStateObject();

		CPPX_ATTR_NO_DISCARD const DX11BlendStateDescriptor & GetDX11BlendStateDescriptor() const noexcept;

		CPPX_ATTR_NO_DISCARD const DX11DepthStencilStateDescriptor & GetDX11DepthStencilStateDescriptor() const noexcept;

		CPPX_ATTR_NO_DISCARD const DX11RasterizerStateDescriptor & GetDX11RasterizerStateDescriptor() const noexcept;

		CPPX_ATTR_NO_DISCARD const DX11GraphicsShaderLinkageDescriptor & GetDX11ShaderLinkageDescriptor() const noexcept;

		CPPX_ATTR_NO_DISCARD const DX11VertexAttributeLayoutDescriptor & GetDX11VertexAttributeLayoutDescriptor() const noexcept;

		CPPX_ATTR_NO_DISCARD const DX11RootSignatureDescriptor & GetDX11RootSignatureDescriptor() const noexcept;

		static TGfxHandle<DX11GraphicsPipelineStateObject> CreateInstance(
				DX11GPUDevice & pGPUDevice,
				const GraphicsPipelineStateObjectCreateInfo & pCreateInfo );
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_DX11_GRAPHICS_PIPELINE_STATE_OBJECT_H__
