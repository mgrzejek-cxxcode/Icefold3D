
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
	class IC3_GRAPHICS_GCI_CLASS VertexAttributeLayoutDescriptor : public PipelineStateDescriptor
	{
	public:
		const IAVertexAttributeLayoutCommonConfig & mCommonAttributeLayoutConfig;

	public:
		Ic3DeclareNonCopyable( VertexAttributeLayoutDescriptor );

		VertexAttributeLayoutDescriptor( GPUDevice & pGPUDevice, const IAVertexAttributeLayoutCommonConfig & pCommonAttributeLayoutConfig );
		virtual ~VertexAttributeLayoutDescriptor();

		CPPX_ATTR_NO_DISCARD virtual EPipelineStateDescriptorType GetDescriptorType() const noexcept override final;
	};

	/**
	 * Represents an IA vertex source binding state (vertex/index buffer binding) for the graphics pipeline.
	 * This descriptor can be either optimized at the API level or specified in a dynamic manner.
	 * @see VertexSourceBindingDescriptorNative
	 * @see VertexSourceBindingDescriptorDynamic
	 */
	class IC3_GRAPHICS_GCI_CLASS VertexSourceBindingDescriptor : public PipelineStateDescriptor
	{
	public:
		const IAVertexSourceBindingCommonConfig & mCommonSourceBindingConfig;

	public:
		Ic3DeclareNonCopyable( VertexSourceBindingDescriptor );

		VertexSourceBindingDescriptor( GPUDevice & pGPUDevice, const IAVertexSourceBindingCommonConfig & pCommonSourceBindingConfig );
		virtual ~VertexSourceBindingDescriptor();

		CPPX_ATTR_NO_DISCARD virtual EPipelineStateDescriptorType GetDescriptorType() const noexcept override final;
	};


	/// namespace PIM: Private Implementation
	namespace PIM
	{

		/**
		 * Represents an IA vertex attribute layout state which has been pre-processed and compiled into the API-specific representation.
		 * @see VertexAttributeLayoutDescriptor
		 */
		class IC3_GRAPHICS_GCI_CLASS VertexAttributeLayoutDescriptorNative : public VertexAttributeLayoutDescriptor
		{
			Ic3DeclareNonCopyable( VertexAttributeLayoutDescriptorNative );

		public:
			VertexAttributeLayoutDescriptorNative( GPUDevice & pGPUDevice, const IAVertexAttributeLayoutCommonConfig & pCommonAttributeLayoutConfig );
			virtual ~VertexAttributeLayoutDescriptorNative();
		};

		/**
		 * Represents an IA vertex source binding state which has been pre-processed and compiled into the API-specific representation.
		 * @see VertexSourceBindingDescriptor
		 */
		class IC3_GRAPHICS_GCI_CLASS VertexSourceBindingDescriptorNative : public VertexSourceBindingDescriptor
		{
			Ic3DeclareNonCopyable( VertexSourceBindingDescriptorNative );

		public:
			VertexSourceBindingDescriptorNative( GPUDevice & pGPUDevice, const IAVertexSourceBindingCommonConfig & pCommonSourceBindingConfig );
			virtual ~VertexSourceBindingDescriptorNative();
		};

	} // namespace PIM

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_GRAPHICS_PIPELINE_STATE_DESCRIPTOR_IA_H__
