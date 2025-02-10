
#pragma once

#ifndef __IC3_GRAPHICS_GCI_PIPELINE_STATE_DESCRIPTOR_SHADER_ROOT_SIGNATURE_H__
#define __IC3_GRAPHICS_GCI_PIPELINE_STATE_DESCRIPTOR_SHADER_ROOT_SIGNATURE_H__

#include "PipelineStateCommon.h"
#include "ShaderRootSignature.h"

namespace Ic3::Graphics::GCI
{

	/**
	 *
	 */
	class ShaderRootSignatureStateDescriptor : public PipelineStateDescriptor
	{
		Ic3DeclareNonCopyable( ShaderRootSignatureStateDescriptor );

	public:
		ShaderRootSignatureStateDescriptor(
				GPUDevice & pGPUDevice,
				pipeline_state_descriptor_id_t pDescriptorID );

		virtual ~ShaderRootSignatureStateDescriptor();

		CPPX_ATTR_NO_DISCARD virtual EPipelineStateDescriptorType GetDescriptorType() const noexcept override final;
	};


	namespace PIM
	{

		/**
		 *
		 */
		class ShaderRootSignatureStateDescriptorGeneric : public PipelineStateDescriptor
		{
			Ic3DeclareNonCopyable( ShaderRootSignatureStateDescriptorGeneric );

		public:
			ShaderRootSignature const mShaderRootSignature;

		public:
			ShaderRootSignatureStateDescriptorGeneric(
					GPUDevice & pGPUDevice,
					pipeline_state_descriptor_id_t pDescriptorID,
					const ShaderRootSignature & pShaderRootSignature );

			virtual ~ShaderRootSignatureStateDescriptorGeneric();

			static TGfxHandle<ShaderRootSignatureStateDescriptorGeneric> CreateFromSignatureDesc(
					GPUDevice & pGPUDevice,
					const ShaderRootSignatureDesc & pPassConfiguration,
					pipeline_state_descriptor_id_t pDescriptorID = kPipelineStateDescriptorIDAuto );
		};

	} // namespace PIM

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_PIPELINE_STATE_DESCRIPTOR_SHADER_ROOT_SIGNATURE_H__
