
#pragma once

#ifndef __IC3_GRAPHICS_GCI_GRAPHICS_PIPELINE_STATE_DESCRIPTOR_IA_H__
#define __IC3_GRAPHICS_GCI_GRAPHICS_PIPELINE_STATE_DESCRIPTOR_IA_H__

#include "PipelineStateDescriptor.h"
#include "InputAssemblerCommon.h"

namespace Ic3::Graphics::GCI
{

	/**
	 * Represents an IA vertex attribute layout state for the graphics pipeline.
	 * This descriptor is always an API-specific one (native/"compiled").
	 */
	class IC3_GRAPHICS_GCI_CLASS VertexAttributeLayoutDescriptor : public CachedPipelineStateDescriptor<PipelineStateDescriptor>
	{
	public:
		const IAVertexAttributeLayoutMetaData & mAttributeLayoutMetaData;

	public:
		Ic3DeclareNonCopyable( VertexAttributeLayoutDescriptor );

		VertexAttributeLayoutDescriptor( GPUDevice & pGPUDevice, const IAVertexAttributeLayoutMetaData & pAttributeLayoutMetaData );
		virtual ~VertexAttributeLayoutDescriptor();

		CPPX_ATTR_NO_DISCARD virtual EPipelineStateDescriptorType GetDescriptorType() const noexcept override final;
	};

	/**
	 * Represents an IA vertex source binding state (vertex/index buffer binding) for the graphics pipeline.
	 * This descriptor can be either optimized at the API level or specified in a dynamic manner.
	 * @see VertexSourceBindingDescriptorNative
	 * @see VertexSourceBindingDescriptorDynamic
	 */
	class IC3_GRAPHICS_GCI_CLASS VertexSourceBindingDescriptor : public NonCachedPipelineStateDescriptor<PipelineStateDescriptor>
	{
	public:
		const IAVertexSourceBindingCommonConfig & mCommonSourceBindingConfig;

	public:
		Ic3DeclareNonCopyable( VertexSourceBindingDescriptor );

		VertexSourceBindingDescriptor( GPUDevice & pGPUDevice, const IAVertexSourceBindingCommonConfig & pCommonSourceBindingConfig );
		virtual ~VertexSourceBindingDescriptor();

		CPPX_ATTR_NO_DISCARD virtual EPipelineStateDescriptorType GetDescriptorType() const noexcept override final;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_GRAPHICS_PIPELINE_STATE_DESCRIPTOR_IA_H__
