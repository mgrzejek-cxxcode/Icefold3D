
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_DXCOMMON_PIPELINE_STATE_CONTROLLER_H__
#define __IC3_GRAPHICS_HW3D_DXCOMMON_PIPELINE_STATE_CONTROLLER_H__

#include "DX11GraphicsPipelineStateIA.h"
#include "DX11GraphicsPipelineStateRTO.h"
#include <Ic3/Graphics/GCI/State/GraphicsPipelineStateImplRenderPassGeneric.h>
#include <Ic3/Graphics/GCI/State/GraphicsPipelineStateImplSeparableState.h>

namespace Ic3::Graphics::GCI
{

	class DX11GraphicsPipelineStateObject;

	/// @brief
	class IC3_GX_DX11_CLASS DX11GraphicsPipelineStateController
			: public GraphicsPipelineStateControllerRenderPassGeneric<GraphicsPipelineStateControllerSeparableShader>
	{
		friend class DX11CommandList;

	public:
		GPUDevice & mGPUDevice;
		DX11CommandList * const mDX11CommandList = nullptr;
		ID3D11DeviceContext1 * const mD3D11DeviceContext1 = nullptr;

	public:
		DX11GraphicsPipelineStateController( DX11CommandList & pDX11CommandList );
		~DX11GraphicsPipelineStateController();

		//CPPX_ATTR_NO_DISCARD const DX11IASourceBindingDefinition & GetCurrentIASourceBindingDefinition() const noexcept;

		//CPPX_ATTR_NO_DISCARD DX11RenderTargetBinding GetCurrentRenderTargetBinding() const noexcept;

		virtual bool ApplyStateChanges() override;

		/**
		 * @see GraphicsPipelineStateController::SetGraphicsPipelineStateObject()
		 */
		virtual bool SetGraphicsPipelineStateObject( const GraphicsPipelineStateObject & pGraphicsPipelineStateObject ) override;

		/**
		 * @see GraphicsPipelineStateController::ResetGraphicsPipelineStateObject()
		 */
		virtual bool ResetGraphicsPipelineStateObject() override;

		/**
		 * @see GraphicsPipelineStateControllerRenderPassGeneric::SetRenderPassDescriptor()
		 */
		virtual bool SetRenderPassDescriptor( const RenderPassDescriptor & pRenderPassDescriptor ) override final;

		/**
		 * @see GraphicsPipelineStateControllerRenderPassGeneric::SetRenderPassDescriptorDynamic()
		 */
		virtual bool SetRenderPassDescriptorDynamic( RenderPassDescriptorDynamic & pRenderPassDescriptor ) override final;

		/**
		 * @see GraphicsPipelineStateControllerRenderPassGeneric::ResetRenderPassDescriptor()
		 */
		virtual void ResetRenderPassDescriptor() override final;

		/**
		 * @see GraphicsPipelineStateController::SetRenderTargetDescriptor()
		 */
		virtual bool SetRenderTargetDescriptor( const RenderTargetDescriptor & pRenderTargetDescriptor ) override;

		virtual bool SetRenderTargetDescriptorDynamic( RenderTargetDescriptorDynamic & pRenderTargetDescriptor ) override;

		/**
		 * @see GraphicsPipelineStateController::ResetRenderTargetDescriptor()
		 */
		virtual bool ResetRenderTargetDescriptor() override;

		/**
		 * @see GraphicsPipelineStateController::SetVertexSourceBindingDescriptor()
		 */
		virtual bool SetVertexSourceBindingDescriptor( const VertexSourceBindingDescriptor & pVertexSourceBindingDescriptor ) override;

		virtual bool SetVertexSourceBindingDescriptorDynamic( VertexSourceBindingDescriptorDynamic & pVertexSourceBindingDescriptor ) override;

		/**
		 * @see GraphicsPipelineStateController::ResetVertexSourceBindingDescriptor()
		 */
		virtual bool ResetVertexSourceBindingDescriptor() override;

		virtual bool SetViewport( const ViewportDesc & pViewportDesc ) override;
		virtual bool SetShaderConstant( shader_input_ref_id_t pParamRefID, const void * pData ) override;
		virtual bool SetShaderConstantBuffer( shader_input_ref_id_t pParamRefID, GPUBuffer & pConstantBuffer ) override;
		virtual bool SetShaderTextureImage( shader_input_ref_id_t pParamRefID, Texture & pTexture ) override;
		virtual bool SetShaderTextureSampler( shader_input_ref_id_t pParamRefID, Sampler & pSampler ) override;

	protected:
		CPPX_ATTR_NO_DISCARD const DX11RenderTargetBinding & GetDX11RenderTargetBinding() const noexcept;

		CPPX_ATTR_NO_DISCARD const DX11IAVertexSourceBinding & GetDX11VertexSourceBinding() const noexcept;

	private:
		using BaseStateControllerType = GraphicsPipelineStateControllerRenderPassGeneric<GraphicsPipelineStateControllerSeparableShader>;

		cppx::bitmask<uint32> BindCommonConfigDescriptors( const DX11GraphicsPipelineStateObject & pDX11GraphicsPipelineStateObject );
		
		cppx::bitmask<uint32> BindSeparableGraphicsShaders( const SeparableShaderCache & pSeparableShaders );

		void SetDX11RenderTargetBinding( const DX11RenderTargetBinding & pDX11RenderTargetBinding );
		
		void SetDX11VertexSourceBinding( const DX11IAVertexSourceBinding & pDX11VertexSourceBinding );

		static const cxm::rgba_color_r32_norm * SelectBlendConstantFactor(
			const DX11BlendStateDescriptor & pDX11BlendStateDescriptor,
			const GraphicsPipelineDynamicConfig & pDynamicConfig ) noexcept;

		static uint8 SelectStencilTestRefValue(
			const DX11DepthStencilStateDescriptor & pDX11DepthStencilStateDescriptor,
			const GraphicsPipelineDynamicConfig & pDynamicConfig ) noexcept;

	private:
		struct DX11CurrentPipelineBindings
		{
			const DX11RenderTargetBinding * renderTargetBinding = nullptr;

			const DX11IAVertexSourceBinding * vertexSourceBinding = nullptr;
		};


		DX11CurrentPipelineBindings _dx11CurrentPipelineBindings;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_DXCOMMON_PIPELINE_STATE_CONTROLLER_H__
