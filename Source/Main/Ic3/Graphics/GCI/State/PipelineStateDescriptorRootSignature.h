
#pragma once

#ifndef __IC3_GRAPHICS_GCI_PIPELINE_STATE_DESCRIPTOR_ROOT_SIGNATURE_H__
#define __IC3_GRAPHICS_GCI_PIPELINE_STATE_DESCRIPTOR_ROOT_SIGNATURE_H__

#include "PipelineStateDescriptor.h"
#include "RootSignature.h"

namespace Ic3::Graphics::GCI
{

	/**
	 *
	 */
	class IC3_GRAPHICS_GCI_CLASS RootSignatureDescriptor : public GCIPipelineStateDescriptor<CachedPipelineStateDescriptor<PipelineStateDescriptor>>
	{
		Ic3DeclareNonCopyable( RootSignatureDescriptor );

	public:
		RootSignatureDescriptor( GPUDevice & pGPUDevice );
		virtual ~RootSignatureDescriptor();

		CPPX_ATTR_NO_DISCARD virtual EPipelineStateDescriptorType GetDescriptorType() const noexcept override final;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_PIPELINE_STATE_DESCRIPTOR_ROOT_SIGNATURE_H__
