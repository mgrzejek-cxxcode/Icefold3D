
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
		Bitmask<EBlendConfigFlags> const mBlendFlags;

	public:
		BlendImmutableState( const BlendImmutableState & ) = delete;
		BlendImmutableState & operator=( const BlendImmutableState & ) = delete;

		BlendImmutableState( GPUDevice & pGPUDevice, Bitmask<EBlendConfigFlags> pBlendFlags );
		virtual ~BlendImmutableState();
	};

	/// @brief
	class DepthStencilImmutableState : public GraphicsPipelineImmutableState
	{
	public:
		Bitmask<EDepthStencilConfigFlags> mDepthStencilFlags;

	public:
		DepthStencilImmutableState( const DepthStencilImmutableState & ) = delete;
		DepthStencilImmutableState & operator=( const DepthStencilImmutableState & ) = delete;

		DepthStencilImmutableState( GPUDevice & pGPUDevice, Bitmask<EDepthStencilConfigFlags> pDepthStencilFlags );
		virtual ~DepthStencilImmutableState();
	};

	/// @brief
	class RasterizerImmutableState : public GraphicsPipelineImmutableState
	{
	public:
		Bitmask<ERasterizerConfigFlags> const mRasterizerFlags;

	public:
		RasterizerImmutableState( const RasterizerImmutableState & ) = delete;
		RasterizerImmutableState & operator=( const RasterizerImmutableState & ) = delete;

		RasterizerImmutableState( GPUDevice & pGPUDevice, Bitmask<ERasterizerConfigFlags> pRasterizerFlags );
		virtual ~RasterizerImmutableState();
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_COMMON_GRAPHICS_CONFIG_IMMUTABLE_STATES_H__
