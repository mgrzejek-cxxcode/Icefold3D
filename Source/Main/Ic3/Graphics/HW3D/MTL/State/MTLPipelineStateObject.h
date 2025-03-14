
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_MTLCOMMON_PIPELINE_STATE_OBJECT_H__
#define __IC3_GRAPHICS_HW3D_MTLCOMMON_PIPELINE_STATE_OBJECT_H__

#include "../MTLPrerequisites.h"
#include <Ic3/Graphics/GCI/State/GraphicsPipelineStateCommon.h>

namespace Ic3::Graphics::GCI
{

	class MetalGraphicsPipelineStateObject : public GraphicsPipelineStateObject
	{
	public:
		id<MTLRenderPipelineState> const mMTLPipelineState;

	public:
		MetalGraphicsPipelineStateObject(
				MetalGPUDevice & pGPUDevice,
				RenderTargetLayout pRenderTargetLayout,
				RootSignature pRootSignature,
				id<MTLRenderPipelineState> pMTLPipelineState );

		virtual ~MetalGraphicsPipelineStateObject();

		static TGfxHandle<MetalGraphicsPipelineStateObject> Create(
				MetalGPUDevice & pGPUDevice,
				const GraphicsPipelineStateObjectCreateInfo & pCreateInfo );
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_MTLCOMMON_PIPELINE_STATE_OBJECT_H__
