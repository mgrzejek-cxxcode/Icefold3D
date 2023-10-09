
#pragma once

#ifndef __IC3_NXMAIN_SHADOW_RENDERING_CONTEXT_H__
#define __IC3_NXMAIN_SHADOW_RENDERING_CONTEXT_H__

#include "CommonRendererDefs.h"

namespace Ic3
{

	struct ContextResourcesBindingState
	{
		GCI::GPUBufferHandle constantBuffers[CxDefs::GPA_SHADER_BINDING_METRIC_CB_MAX_NUM];

		TextureUnitBinding textureUnits[CxDefs::GPA_SHADER_BINDING_METRIC_TEX_MAX_NUM];
	};

	class RenderingContext
	{
	public:
		GCI::GPUDevice & mGPUDevice;
		GCI::CommandContext & mGPUCommandContext;

	public:
		RenderingContext( GCI::CommandContext & pGPUCommandContext );
		~RenderingContext();

		void createConstantBuffer( native_uint pIndex, memory_size_t pBufferSize, bool pMapped = false );

		void bindConstantBuffer( native_uint pIndex, GCI::GPUBufferHandle pConstantBuffer );

		void bindTexture( native_uint pIndex, GCI::TextureHandle pTexture, GCI::SamplerHandle pSampler );

		GCI::GPUBufferHandle getConstantBuffer( native_uint pIndex ) const noexcept;

	private:
		ContextResourcesBindingState _bindingState;
	};

} // namespace Ic3

#endif // __IC3_NXMAIN_SHADOW_RENDERING_CONTEXT_H__
