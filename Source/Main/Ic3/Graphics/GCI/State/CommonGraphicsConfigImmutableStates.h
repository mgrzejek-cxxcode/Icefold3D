
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
		cppx::bitmask<EBlendConfigFlags> const mBlendFlags;

	public:
		BlendImmutableState( const BlendImmutableState & ) = delete;
		BlendImmutableState & operator=( const BlendImmutableState & ) = delete;

		BlendImmutableState( GpuDevice & pGpuDevice, cppx::bitmask<EBlendConfigFlags> pBlendFlags );
		virtual ~BlendImmutableState();
	};

	/// @brief
	class DepthStencilImmutableState : public GraphicsPipelineImmutableState
	{
	public:
		cppx::bitmask<EDepthStencilConfigFlags> mDepthStencilFlags;

	public:
		DepthStencilImmutableState( const DepthStencilImmutableState & ) = delete;
		DepthStencilImmutableState & operator=( const DepthStencilImmutableState & ) = delete;

		DepthStencilImmutableState( GpuDevice & pGpuDevice, cppx::bitmask<EDepthStencilConfigFlags> pDepthStencilFlags );
		virtual ~DepthStencilImmutableState();
	};

	/// @brief
	class RasterizerImmutableState : public GraphicsPipelineImmutableState
	{
	public:
		cppx::bitmask<ERasterizerConfigFlags> const mRasterizerFlags;

	public:
		RasterizerImmutableState( const RasterizerImmutableState & ) = delete;
		RasterizerImmutableState & operator=( const RasterizerImmutableState & ) = delete;

		RasterizerImmutableState( GpuDevice & pGpuDevice, cppx::bitmask<ERasterizerConfigFlags> pRasterizerFlags );
		virtual ~RasterizerImmutableState();
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_COMMON_GRAPHICS_CONFIG_IMMUTABLE_STATES_H__
