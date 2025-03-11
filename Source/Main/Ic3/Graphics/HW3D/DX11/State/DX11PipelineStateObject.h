
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_DX11_PIPELINE_STATE_OBJECT_H__
#define __IC3_GRAPHICS_HW3D_DX11_PIPELINE_STATE_OBJECT_H__

#include "../DX11gpuDevice.h"
#include <Ic3/Graphics/GCI/State/SeparablePipelineState.h>

namespace Ic3::Graphics::GCI
{

	class DX11GraphicsPipelineStateObject : public GraphicsPipelineStateObjectSeparableShader
	{
	public:
		DX11GraphicsPipelineStateObject(
				DX11GPUDevice & pGPUDevice,
				RenderTargetLayout pRenderTargetLayout,
				RootSignature pRootSignature,
				const SeparableGraphicsPSDSet & pPSOCompiledStates,
				const GraphicsShaderSet & pSeparableShaders );

		virtual ~DX11GraphicsPipelineStateObject();

		CPPX_ATTR_NO_DISCARD const DX11BlendStateDescriptor & GetBlendState() const noexcept;

		CPPX_ATTR_NO_DISCARD const DX11DepthStencilStateDescriptor & GetDepthStencilState() const noexcept;

		CPPX_ATTR_NO_DISCARD const DX11RasterizerStateDescriptor & GetRasterizerState() const noexcept;

		CPPX_ATTR_NO_DISCARD const DX11GraphicsShaderLinkageCompiledState & GetGraphicsShaderLinkageState() const noexcept;

		CPPX_ATTR_NO_DISCARD const DX11VertexAttributeLayoutDescriptor & GetIAVertexAttributeLayout() const noexcept;

		static TGfxHandle<DX11GraphicsPipelineStateObject> Create(
				DX11GPUDevice & pGPUDevice,
				const GraphicsPipelineStateObjectCreateInfo & pCreateInfo );
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_DX11_PIPELINE_STATE_OBJECT_H__
