
#pragma once

#ifndef __IC3_GRAPHICS_GCI_COMMON_GRAPHICS_CONFIG_IMMUTABLE_STATES_H__
#define __IC3_GRAPHICS_GCI_COMMON_GRAPHICS_CONFIG_IMMUTABLE_STATES_H__

#include "CommonGraphicsConfig.h"
#include "GraphicsPipelineImmutableState.h"

namespace Ic3::Graphics::GCI
{

	/// @brief
	class BlendImmutableState : public GraphicsPipelineImmutableState
	{
	public:
		TBitmask<EBlendConfigFlags> const mBlendFlags;

	public:
		BlendImmutableState( const BlendImmutableState & ) = delete;
		BlendImmutableState & operator=( const BlendImmutableState & ) = delete;

		BlendImmutableState( GPUDevice & pGPUDevice, TBitmask<EBlendConfigFlags> pBlendFlags );
		virtual ~BlendImmutableState();
	};

	/// @brief
	class DepthStencilImmutableState : public GraphicsPipelineImmutableState
	{
	public:
		TBitmask<EDepthStencilConfigFlags> mDepthStencilFlags;

	public:
		DepthStencilImmutableState( const DepthStencilImmutableState & ) = delete;
		DepthStencilImmutableState & operator=( const DepthStencilImmutableState & ) = delete;

		DepthStencilImmutableState( GPUDevice & pGPUDevice, TBitmask<EDepthStencilConfigFlags> pDepthStencilFlags );
		virtual ~DepthStencilImmutableState();
	};

	/// @brief
	class RasterizerImmutableState : public GraphicsPipelineImmutableState
	{
	public:
		TBitmask<ERasterizerConfigFlags> const mRasterizerFlags;

	public:
		RasterizerImmutableState( const RasterizerImmutableState & ) = delete;
		RasterizerImmutableState & operator=( const RasterizerImmutableState & ) = delete;

		RasterizerImmutableState( GPUDevice & pGPUDevice, TBitmask<ERasterizerConfigFlags> pRasterizerFlags );
		virtual ~RasterizerImmutableState();
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_COMMON_GRAPHICS_CONFIG_IMMUTABLE_STATES_H__
