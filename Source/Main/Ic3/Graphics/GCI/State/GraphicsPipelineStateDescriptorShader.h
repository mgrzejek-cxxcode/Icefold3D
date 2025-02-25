
#pragma once

#ifndef __IC3_GRAPHICS_GCI_GRAPHICS_PIPELINE_STATE_DESCRIPTOR_SHADER_H__
#define __IC3_GRAPHICS_GCI_GRAPHICS_PIPELINE_STATE_DESCRIPTOR_SHADER_H__

#include "GraphicsShaderDefs.h"
#include "PipelineStateDescriptor.h"

namespace Ic3::Graphics::GCI
{

	/**
	 *
	 */
	class GraphicsShaderLinkageDescriptor : public PipelineStateDescriptor
	{
		Ic3DeclareNonCopyable( GraphicsShaderLinkageDescriptor );

	public:
		const GraphicsShaderBindingCommonConfig & mCommonShaderBindingConfig;

	public:
		GraphicsShaderLinkageDescriptor(
				GPUDevice & pGPUDevice,
				pipeline_state_descriptor_id_t pDescriptorID,
				const GraphicsShaderBindingCommonConfig & pCommonShaderBindingConfig );

		virtual ~GraphicsShaderLinkageDescriptor();

		CPPX_ATTR_NO_DISCARD virtual EPipelineStateDescriptorType GetDescriptorType() const noexcept override final;

		CPPX_ATTR_NO_DISCARD bool IsStageActive( uint32 pStageIndex ) const noexcept;

		CPPX_ATTR_NO_DISCARD bool IsStageActive( EShaderType pShaderType ) const noexcept;
	};

	/**
	 *
	 */
	class GraphicsShaderLinkageDescriptorGenericSeparable : public GraphicsShaderLinkageDescriptor
	{
	public:
		GraphicsShaderBinding const mShaderBinding;

	public:
		GraphicsShaderLinkageDescriptorGenericSeparable(
				GPUDevice & pGPUDevice,
				pipeline_state_descriptor_id_t pDescriptorID,
				const GraphicsShaderBinding & pBindingConfiguration );

		virtual ~GraphicsShaderLinkageDescriptorGenericSeparable();

		CPPX_ATTR_NO_DISCARD ShaderHandle GetShader( size_t pStageIndex ) const noexcept;

		CPPX_ATTR_NO_DISCARD ShaderHandle GetShader( EShaderType pShaderType ) const noexcept;

		CPPX_ATTR_NO_DISCARD static TGfxHandle<GraphicsShaderLinkageDescriptorGenericSeparable> CreateFromBindingConfiguration(
				GPUDevice & pGPUDevice,
				const GraphicsShaderBinding & pBindingConfiguration,
				pipeline_state_descriptor_id_t pDescriptorID = kPipelineStateDescriptorIDAuto ) noexcept;

		CPPX_ATTR_NO_DISCARD static TGfxHandle<GraphicsShaderLinkageDescriptorGenericSeparable> CreateFromShaderArray(
				GPUDevice & pGPUDevice,
				const GraphicsShaderArray & pShaderArray,
				pipeline_state_descriptor_id_t pDescriptorID = kPipelineStateDescriptorIDAuto ) noexcept;
	};

	namespace PIM
	{

		class GraphicsShaderLinkageDescriptorNative : public GraphicsShaderLinkageDescriptor
		{
		public:
			GraphicsShaderLinkageDescriptorNative(
					GPUDevice & pGPUDevice,
					pipeline_state_descriptor_id_t pDescriptorID,
					const GraphicsShaderBindingCommonConfig & pCommonShaderBindingConfig );

			virtual ~GraphicsShaderLinkageDescriptorNative();
		};

	} // namespace PIM

} // namespace Ic3::Graphics::GCI

#include "GraphicsPipelineStateDescriptorShader.inl"

#endif // __IC3_GRAPHICS_GCI_GRAPHICS_PIPELINE_STATE_DESCRIPTOR_SHADER_H__
