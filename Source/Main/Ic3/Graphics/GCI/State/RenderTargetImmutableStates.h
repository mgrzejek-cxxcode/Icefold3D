
#pragma once

#ifndef __IC3_GRAPHICS_GCI_RENDER_TARGET_IMMUTABLE_STATES_H__
#define __IC3_GRAPHICS_GCI_RENDER_TARGET_IMMUTABLE_STATES_H__

#include "GraphicsPipelineImmutableState.h"
#include "RenderPassCommon.h"

namespace Ic3::Graphics::GCI
{

	class RenderTargetBindingImmutableState : public GraphicsPipelineImmutableState
	{
	public:
		RenderTargetLayout const mRenderTargetLayout;

	public:
		RenderTargetBindingImmutableState( const RenderTargetBindingImmutableState & ) = delete;
		RenderTargetBindingImmutableState & operator=( const RenderTargetBindingImmutableState & ) = delete;

		RenderTargetBindingImmutableState(
				GPUDevice & pGPUDevice,
				const RenderTargetLayout & pRenderTargetLayout );

		virtual ~RenderTargetBindingImmutableState();

		CPPX_ATTR_NO_DISCARD static const RenderTargetBindingImmutableState & GetDynamicOverrideState();
	};

	class RenderPassConfigurationImmutableState : public GraphicsPipelineImmutableState
	{
	public:
		RenderPassConfigurationImmutableState( const RenderPassConfigurationImmutableState & ) = delete;
		RenderPassConfigurationImmutableState & operator=( const RenderPassConfigurationImmutableState & ) = delete;

		RenderPassConfigurationImmutableState( GPUDevice & pGPUDevice );
		virtual ~RenderPassConfigurationImmutableState();
	};

	class RenderPassConfigurationImmutableStateDefault : public RenderPassConfigurationImmutableState
	{
	public:
		RenderPassConfiguration const mRenderPassConfiguration;

	public:
		RenderPassConfigurationImmutableStateDefault(
				GPUDevice & pGPUDevice,
				const RenderPassConfiguration & pRenderPassConfiguration );

		virtual ~RenderPassConfigurationImmutableStateDefault();

		static TGfxHandle<RenderPassConfigurationImmutableStateDefault> CreateInstance(
				GPUDevice & pGPUDevice,
				const RenderPassConfiguration & pConfiguration );
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_RENDER_TARGET_IMMUTABLE_STATES_H__
