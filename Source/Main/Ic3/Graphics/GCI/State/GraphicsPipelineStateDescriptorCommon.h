
#pragma once

#ifndef __IC3_GRAPHICS_GCI_COMMON_GRAPHICS_CONFIG_IMMUTABLE_STATES_H__
#define __IC3_GRAPHICS_GCI_COMMON_GRAPHICS_CONFIG_IMMUTABLE_STATES_H__

#include "CommonGraphicsConfig.h"
#include "PipelineStateDescriptor.h"

namespace Ic3::Graphics::GCI
{

	/**
	 *
	 */
	class IC3_GRAPHICS_GCI_CLASS BlendStateDescriptor : public PipelineStateDescriptor
	{
	public:
		cppx::bitmask<EBlendConfigFlags> const mBlendFlags;

	public:
		Ic3DeclareNonCopyable( BlendStateDescriptor );

		BlendStateDescriptor(
				GPUDevice & pGPUDevice,
				cppx::bitmask<EBlendConfigFlags> pBlendFlags );

		virtual ~BlendStateDescriptor();

		CPPX_ATTR_NO_DISCARD virtual EPipelineStateDescriptorType GetDescriptorType() const noexcept override final;
	};

	/**
	 *
	 */
	class IC3_GRAPHICS_GCI_CLASS DepthStencilStateDescriptor : public PipelineStateDescriptor
	{
	public:
		cppx::bitmask<EDepthStencilConfigFlags> mDepthStencilFlags;

	public:
		Ic3DeclareNonCopyable( DepthStencilStateDescriptor );

		DepthStencilStateDescriptor(
				GPUDevice & pGPUDevice,
				cppx::bitmask<EDepthStencilConfigFlags> pDepthStencilFlags );

		virtual ~DepthStencilStateDescriptor();

		CPPX_ATTR_NO_DISCARD virtual EPipelineStateDescriptorType GetDescriptorType() const noexcept override final;
	};

	/**
	 *
	 */
	class IC3_GRAPHICS_GCI_CLASS RasterizerStateDescriptor : public PipelineStateDescriptor
	{
	public:
		cppx::bitmask<ERasterizerConfigFlags> const mRasterizerFlags;

	public:
		Ic3DeclareNonCopyable( RasterizerStateDescriptor );

		RasterizerStateDescriptor(
				GPUDevice & pGPUDevice,
				cppx::bitmask<ERasterizerConfigFlags> pRasterizerFlags );

		virtual ~RasterizerStateDescriptor();

		CPPX_ATTR_NO_DISCARD virtual EPipelineStateDescriptorType GetDescriptorType() const noexcept override final;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_COMMON_GRAPHICS_CONFIG_IMMUTABLE_STATES_H__
