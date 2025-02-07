
#pragma once

#ifndef __IC3_GRAPHICS_GCI_COMMON_GRAPHICS_CONFIG_IMMUTABLE_STATES_H__
#define __IC3_GRAPHICS_GCI_COMMON_GRAPHICS_CONFIG_IMMUTABLE_STATES_H__

#include "CommonGraphicsConfig.h"
#include "PipelineStateCommon.h"

namespace Ic3::Graphics::GCI
{

	/**
	 *
	 */
	class BlendStateDescriptor : public PipelineStateDescriptor
	{
	public:
		cppx::bitmask<EBlendConfigFlags> const mBlendFlags;

	public:
		Ic3DeclareNonCopyable( BlendStateDescriptor );

		BlendStateDescriptor( GPUDevice & pGPUDevice, cppx::bitmask<EBlendConfigFlags> pBlendFlags );
		virtual ~BlendStateDescriptor();
	};

	/**
	 *
	 */
	class DepthStencilStateDescriptor : public PipelineStateDescriptor
	{
	public:
		cppx::bitmask<EDepthStencilConfigFlags> mDepthStencilFlags;

	public:
		Ic3DeclareNonCopyable( DepthStencilStateDescriptor );

		DepthStencilStateDescriptor( GPUDevice & pGPUDevice, cppx::bitmask<EDepthStencilConfigFlags> pDepthStencilFlags );
		virtual ~DepthStencilStateDescriptor();
	};

	/**
	 *
	 */
	class RasterizerStateDescriptor : public PipelineStateDescriptor
	{
	public:
		cppx::bitmask<ERasterizerConfigFlags> const mRasterizerFlags;

	public:
		Ic3DeclareNonCopyable( RasterizerStateDescriptor );

		RasterizerStateDescriptor( GPUDevice & pGPUDevice, cppx::bitmask<ERasterizerConfigFlags> pRasterizerFlags );
		virtual ~RasterizerStateDescriptor();
	};

	/**
	 *
	 */
	class MultiSamplingStateDescriptor : public PipelineStateDescriptor
	{
	public:
		MultiSamplingConfig const mMSConfig;

	public:
		Ic3DeclareNonCopyable( MultiSamplingStateDescriptor );

		MultiSamplingStateDescriptor( GPUDevice & pGPUDevice, const MultiSamplingConfig & pMSConfig );
		virtual ~MultiSamplingStateDescriptor();
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_COMMON_GRAPHICS_CONFIG_IMMUTABLE_STATES_H__
