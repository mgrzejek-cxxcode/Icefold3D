
#pragma once

#ifndef __IC3_GRAPHICS_GCI_GRAPHICS_PIPELINE_STATE_H__
#define __IC3_GRAPHICS_GCI_GRAPHICS_PIPELINE_STATE_H__

#include "CommonGraphicsConfig.h"
#include "InputAssemblerCommon.h"

namespace Ic3::Graphics::GCI
{

	class Shader;

	enum class EGraphicsPipelineImmutableStateType : uint16
	{
		Blend,
		DepthStencil,
		ShaderLinkage,
		IAInputLayout,
		IAVertexStream,
		Rasterizer,
		RenderTargetBinding,
		RenderPassConfiguration,
		Unknown
	};

	class GraphicsPipelineImmutableState : public GPUDeviceChildObject
	{
	public:
		GraphicsPipelineImmutableState( const GraphicsPipelineImmutableState & ) = delete;
		GraphicsPipelineImmutableState & operator=( const GraphicsPipelineImmutableState & ) = delete;

		GraphicsPipelineImmutableState( GPUDevice & pGPUDevice );
		virtual ~GraphicsPipelineImmutableState();

		CPPX_ATTR_NO_DISCARD virtual EGraphicsPipelineImmutableStateType QueryStateType() const noexcept;

		CPPX_ATTR_NO_DISCARD virtual bool IsDynamicOverrideState() const noexcept;
	};

	/// @brief Dummy trait type for the descriptor cache. List all the things required for the traits to provide.
	class PipelineImmutableStateFactory
	{
	public:
		PipelineImmutableStateFactory() = default;
		virtual ~PipelineImmutableStateFactory() = default;

		virtual BlendImmutableStateHandle CreateBlendState( const BlendConfig & pConfig ) = 0;
		virtual DepthStencilImmutableStateHandle CreateDepthStencilState( const DepthStencilConfig & pConfig ) = 0;
		virtual GraphicsShaderLinkageImmutableStateHandle CreateGraphicsShaderLinkageState( const GraphicsShaderSet & pShaderSet ) = 0;
		virtual IAInputLayoutImmutableStateHandle CreateIAInputLayoutState( const IAInputLayoutDefinition & pDefinition, Shader & pVertexShaderWithBinary ) = 0;
		virtual IAVertexStreamImmutableStateHandle CreateIAVertexStreamState( const IAVertexStreamDefinition & pDefinition ) = 0;
		virtual RasterizerImmutableStateHandle CreateRasterizerState( const RasterizerConfig & pConfig ) = 0;
		virtual RenderTargetBindingImmutableStateHandle CreateRenderTargetBindingState( const RenderTargetBindingDefinition & pDefinition ) = 0;
		virtual RenderPassConfigurationImmutableStateHandle CreateRenderPassState( const RenderPassConfiguration & pConfiguration ) = 0;

	protected:
		virtual RenderPassConfigurationImmutableStateHandle CreateRenderPassStateDefault(
				GPUDevice & pGPUDevice,
				const RenderPassConfiguration & pConfiguration );
	};

	/// @brief
	class PipelineImmutableStateFactoryNull : public PipelineImmutableStateFactory
	{
	public:
		PipelineImmutableStateFactoryNull() = default;
		virtual ~PipelineImmutableStateFactoryNull() = default;

		virtual BlendImmutableStateHandle CreateBlendState( const BlendConfig & ) override final;
		virtual DepthStencilImmutableStateHandle CreateDepthStencilState( const DepthStencilConfig & ) override final;
		virtual GraphicsShaderLinkageImmutableStateHandle CreateGraphicsShaderLinkageState( const GraphicsShaderSet & ) override final;
		virtual IAInputLayoutImmutableStateHandle CreateIAInputLayoutState( const IAInputLayoutDefinition &, Shader & ) override final;
		virtual IAVertexStreamImmutableStateHandle CreateIAVertexStreamState( const IAVertexStreamDefinition & ) override final;
		virtual RasterizerImmutableStateHandle CreateRasterizerState( const RasterizerConfig & ) override final;
		virtual RenderTargetBindingImmutableStateHandle CreateRenderTargetBindingState( const RenderTargetBindingDefinition & ) override final;
		virtual RenderPassConfigurationImmutableStateHandle CreateRenderPassState( const RenderPassConfiguration & ) override final;
	};

	/// @brief
	class PipelineImmutableStateFactorySeparableShader : public PipelineImmutableStateFactory
	{
	public:
		GPUDevice & mGPUDevice;

	public:
		PipelineImmutableStateFactorySeparableShader( GPUDevice & pGPUDevice );
		virtual ~PipelineImmutableStateFactorySeparableShader();

		virtual GraphicsShaderLinkageImmutableStateHandle CreateGraphicsShaderLinkageState( const GraphicsShaderSet & pShaderSet ) override;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_GRAPHICS_PIPELINE_STATE_H__
