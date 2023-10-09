
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
				ShaderInputSignature pShaderInputSignature,
				const SeparablePSOStateSet & pPSOImmutableStates,
				const GraphicsShaderSet & pSeparableShaders );

		virtual ~DX11GraphicsPipelineStateObject();

		IC3_ATTR_NO_DISCARD const DX11BlendImmutableState & getBlendState() const noexcept;

		IC3_ATTR_NO_DISCARD const DX11DepthStencilImmutableState & getDepthStencilState() const noexcept;

		IC3_ATTR_NO_DISCARD const DX11RasterizerImmutableState & getRasterizerState() const noexcept;

		IC3_ATTR_NO_DISCARD const DX11GraphicsShaderLinkageImmutableState & getGraphicsShaderLinkageState() const noexcept;

		IC3_ATTR_NO_DISCARD const DX11IAInputLayoutImmutableState & getIAInputLayoutState() const noexcept;

		static GpaHandle<DX11GraphicsPipelineStateObject> create(
				DX11GPUDevice & pGPUDevice,
				const GraphicsPipelineStateObjectCreateInfo & pCreateInfo );
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_DX11_PIPELINE_STATE_OBJECT_H__
