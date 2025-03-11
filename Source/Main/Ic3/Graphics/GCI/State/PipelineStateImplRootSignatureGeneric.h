
#pragma once

#ifndef __IC3_GRAPHICS_GCI_PIPELINE_STATE_IMPL_ROOT_SIGNATURE_GENERIC_H__
#define __IC3_GRAPHICS_GCI_PIPELINE_STATE_IMPL_ROOT_SIGNATURE_GENERIC_H__

#include "PipelineStateDescriptorRootSignature.h"
#include "RootSignature.h"

namespace Ic3::Graphics::GCI
{

	class IC3_GRAPHICS_GCI_CLASS RootSignatureDescriptorGeneric : public RootSignatureDescriptor
	{
		Ic3DeclareNonCopyable( RootSignatureDescriptorGeneric );

	public:
		RootSignature const mRootSignature;

	public:
		RootSignatureDescriptorGeneric( GPUDevice & pGPUDevice, RootSignature pRootSignature );
		virtual ~RootSignatureDescriptorGeneric();

		CPPX_ATTR_NO_DISCARD static TGfxHandle<RootSignatureDescriptorGeneric> CreateInstance(
				GPUDevice & pGPUDevice,
				const RootSignatureDescriptorCreateInfo & pCreateInfo );
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_PIPELINE_STATE_IMPL_ROOT_SIGNATURE_GENERIC_H__
