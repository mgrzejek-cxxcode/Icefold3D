
#pragma once

#ifndef __IC3_GRAPHICS_GCI_GRAPHICS_SHADER_BINDING_IMMUTABLE_STATE_H__
#define __IC3_GRAPHICS_GCI_GRAPHICS_SHADER_BINDING_IMMUTABLE_STATE_H__

#include "GraphicsShaderDefs.h"
#include "PipelineStateCommon.h"

namespace Ic3::Graphics::GCI
{

	/**
	 *
	 */
	class GraphicsShaderLinkageStateDescriptor : public PipelineStateDescriptor
	{
		Ic3DeclareNonCopyable( GraphicsShaderLinkageStateDescriptor );

	public:
		const GraphicsShaderBindingCommonConfig & mCommonShaderBindingConfig;

	public:
		GraphicsShaderLinkageStateDescriptor(
				GPUDevice & pGPUDevice,
				pipeline_state_descriptor_id_t pDescriptorID,
				const GraphicsShaderBindingCommonConfig & pCommonShaderBindingConfig );

		virtual ~GraphicsShaderLinkageStateDescriptor();

		CPPX_ATTR_NO_DISCARD virtual EPipelineStateDescriptorType GetDescriptorType() const noexcept override final;

		CPPX_ATTR_NO_DISCARD virtual ShaderHandle GetShader( size_t pStageIndex ) const noexcept = 0;

		CPPX_ATTR_NO_DISCARD virtual ShaderHandle GetShader( EShaderType pShaderType ) const noexcept = 0;

		CPPX_ATTR_NO_DISCARD bool IsStageActive( uint32 pStageIndex ) const noexcept;

		CPPX_ATTR_NO_DISCARD bool IsStageActive( EShaderType pShaderType ) const noexcept;
	};

	/**
	 *
	 */
	class GraphicsShaderLinkageStateDescriptorGenericSeparable : public GraphicsShaderLinkageStateDescriptor
	{
	public:
		GraphicsShaderBinding const mShaderBinding;

	public:
		GraphicsShaderLinkageStateDescriptorGenericSeparable(
				GPUDevice & pGPUDevice,
				pipeline_state_descriptor_id_t pDescriptorID,
				const GraphicsShaderBinding & pShaderBinding );

		virtual ~GraphicsShaderLinkageStateDescriptorGenericSeparable();

		CPPX_ATTR_NO_DISCARD virtual ShaderHandle GetShader( size_t pStageIndex ) const noexcept override final;

		CPPX_ATTR_NO_DISCARD virtual ShaderHandle GetShader( EShaderType pShaderType ) const noexcept override final;

		CPPX_ATTR_NO_DISCARD static TGfxHandle<GraphicsShaderLinkageStateDescriptorGenericSeparable> CreateFromBindingConfiguration(
				GPUDevice & pGPUDevice,
				const GraphicsShaderBinding & pShaderBinding,
				pipeline_state_descriptor_id_t pDescriptorID = kPipelineStateDescriptorIDAuto ) noexcept;

		CPPX_ATTR_NO_DISCARD static TGfxHandle<GraphicsShaderLinkageStateDescriptorGenericSeparable> CreateFromShaderArray(
				GPUDevice & pGPUDevice,
				const GraphicsShaderArray & pShaderArray,
				pipeline_state_descriptor_id_t pDescriptorID = kPipelineStateDescriptorIDAuto ) noexcept;
	};

	class ShaderRootSignature
	{
		// ?
	};

} // namespace Ic3::Graphics::GCI

#include "GraphicsPipelineStateDescriptorShader.inl"

#endif // __IC3_GRAPHICS_GCI_GRAPHICS_SHADER_BINDING_IMMUTABLE_STATE_H__
