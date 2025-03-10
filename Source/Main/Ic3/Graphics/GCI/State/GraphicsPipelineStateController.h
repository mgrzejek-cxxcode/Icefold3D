
#pragma once

#ifndef __IC3_GRAPHICS_GCI_GRAPHICS_PIPELINE_STATE_CONTROLLER_H__
#define __IC3_GRAPHICS_GCI_GRAPHICS_PIPELINE_STATE_CONTROLLER_H__

#include "GraphicsPipelineStateCommon.h"
#include "GraphicsPipelineStateDescriptorIA.h"
#include "GraphicsPipelineStateDescriptorRTO.h"
#include "PipelineStateController.h"

namespace Ic3::Graphics::GCI
{

	class GraphicsPipelineStateDescriptorFactory;

	using graphics_state_update_mask_value_t = uint64;

	/// @brief Defines bit flags describing what kind of changes are pending for
	enum EGraphicsStateUpdateCommonFlags : graphics_state_update_mask_value_t
	{
		eGraphicsStateUpdateFlagCommonPipelineStateObjectBit = 0x01,
		eGraphicsStateUpdateFlagCommonRenderTargetDescriptorBit = 0x02,
		eGraphicsStateUpdateFlagCommonVertexSourceBindingDescriptorBit = 0x04,
		eGraphicsStateUpdateMaskCommonAll = 0x07
	};

	/**
	 * GraphicsPipelineStateController is a proxy responsible for managing state changes made to the graphics pipeline
	 * during the rendering process. All state changes made through the CommandContext are eventually dispatched to the
	 * StateControlled owned by the current thread, for which the CommandContext has been acquired.
	 * This class is fully implemented at the API level, as it is responsible for executing API-specific commands
	 * that propagate the state changes to that API.
	 */
	class IC3_GRAPHICS_GCI_CLASS GraphicsPipelineStateController : public PipelineStateController
	{
		friend class CommandList;

	public:
		GraphicsPipelineStateController();
		virtual ~GraphicsPipelineStateController();

		/**
		 * 
		 * @return
		 */
		virtual bool ApplyStateChanges() = 0;

		/**
		 * 
		 * @param pRenderPassDescriptor 
		 * @return 
		 */
		virtual bool SetRenderPassDescriptor( const RenderPassDescriptor & pRenderPassDescriptor );

		/**
		 * 
		 * @param pRenderPassDescriptor 
		 * @return 
		 */
		virtual bool SetRenderPassDescriptorDynamic( RenderPassDescriptorDynamic & pRenderPassDescriptor );

		/**
		 * 
		 */
		virtual void ResetRenderPassDescriptor();

		/**
		 * Binds the specified GraphicsPipelineStateObject to the pipeline. Returns true if any change has been made.
		 * @param pGraphicsPipelineStateObject
		 * @return True if anything has been changed or false otherwise.
		 *
		 * @note
		 * If this function returns false, it means that either the specified PSO or a different PSO with the exact
		 * same configuration was already bound to this state controller.
		 *
		 * @note
		 * Subclasses should always call the base method first and check the result before doing the actual update.
		 */
		virtual bool SetGraphicsPipelineStateObject( const GraphicsPipelineStateObject & pGraphicsPipelineStateObject );

		/**
		 * Resets the current PSO and clears the binding. Returns true if any change has been made.
		 * @return True if anything has been changed or false otherwise. False means there was no PSO bound prior to this call.
		 *
		 * @note
		 * If this function returns false, it means that there was no PSO bound prior to this call.
		 *
		 * @note
		 * Subclasses should always call the base method first and check the result before doing the actual update.
		 */
		virtual bool ResetGraphicsPipelineStateObject();

		/**
		 *
		 * @param pRenderTargetDescriptor
		 * @return
		 */
		virtual bool SetRenderTargetDescriptor( const RenderTargetDescriptor & pRenderTargetDescriptor );

		/**
		 *
		 * @param pRenderTargetDescriptor
		 * @return
		 */
		virtual bool SetRenderTargetDescriptorDynamic( RenderTargetDescriptorDynamic & pRenderTargetDescriptor );

		/**
		 *
		 * @return
		 */
		virtual bool ResetRenderTargetDescriptor();

		/**
		 *
		 * @param pVertexSourceBindingDescriptor
		 * @return
		 */
		virtual bool SetVertexSourceBindingDescriptor( const VertexSourceBindingDescriptor & pVertexSourceBindingDescriptor );

		/**
		 *
		 * @param pVertexSourceBindingDescriptor
		 * @return
		 */
		virtual bool SetVertexSourceBindingDescriptorDynamic( VertexSourceBindingDescriptorDynamic & pVertexSourceBindingDescriptor );

		/**
		 *
		 * @return
		 */
		virtual bool ResetVertexSourceBindingDescriptor();

		/**
		 *
		 */
		virtual bool SetViewport( const ViewportDesc & pViewportDesc );

		virtual bool SetShaderConstant( shader_input_ref_id_t pParamRefID, const void * pData );
		virtual bool SetShaderConstantBuffer( shader_input_ref_id_t pParamRefID, GPUBuffer & pConstantBuffer );
		virtual bool SetShaderTextureImage( shader_input_ref_id_t pParamRefID, Texture & pTexture );
		virtual bool SetShaderTextureSampler( shader_input_ref_id_t pParamRefID, Sampler & pSampler );

		/**
		 * Sets the dynamic blend constant color that overrides the config from a BlendStateDescriptor.
		 * @return Mask with the current config state - contains all bits for currently enabled dynamic configs.
		 */
		cppx::bitmask<EGraphicsPipelineDynamicConfigFlags> SetDynamicBlendConstantColor(
				const cxm::rgba_color_r32_norm & pBlendConstantColor );

		/**
		 * Sets the dynamic clear config that overrides the config from a RenderPassDescriptor.
		 * @return Mask with the current config state - contains all bits for currently enabled dynamic configs.
		 */
		cppx::bitmask<EGraphicsPipelineDynamicConfigFlags> SetDynamicRenderTargetClearConfig(
				const RenderTargetAttachmentClearConfig & pClearConfig );

		/**
		 * Sets the dynamic ref value used for stencil testing that overrides the value from a DepthStencilStateDescriptor.
		 * @return Mask with the current config state - contains all bits for currently enabled dynamic configs.
		 */
		cppx::bitmask<EGraphicsPipelineDynamicConfigFlags> SetDynamicStencilTestRefValue( uint8 pStencilRefValue );

		/**
		 * Resets (disables) the specified parts of the dynamic pipeline config, which will be fed with values
		 * from their respective state descriptors.
		 * @param pConfigMask A bitmask that specifies which parts of the dynamic config to disable.
		 */
		void ResetDynamicPipelineConfig( cppx::bitmask<EGraphicsPipelineDynamicConfigFlags> pConfigMask = eGraphicsPipelineDynamicConfigMaskAll );

		/**
		 * Returns the current dynamic configuration set for the graphics pipeline.
		 * @return Current dynamic configuration for the graphics pipeline.
		 */
		CPPX_ATTR_NO_DISCARD const GraphicsPipelineDynamicConfig & GetPipelineDynamicConfig() const noexcept;

		/**
		 * Returns true if the specified portion of the dynamic pipeline config is enabled or false otherwise.
		 * @param pConfigMask A bitmask that specifies which portions of the config to check.
		 * @return True if the specified portion of the dynamic pipeline config is enabled or false otherwise.
		 */
		CPPX_ATTR_NO_DISCARD bool IsDynamicPipelineConfigEnabled(
				cppx::bitmask<EGraphicsPipelineDynamicConfigFlags> pConfigMask ) const noexcept;

		/**
		 * Returns true if there are any pending changes in the bindings or false otherwise.
		 * A change in the binding happens whenever a different object of one of the following types is bound:
		 * - GraphicsPipelineStateObject
		 * - RenderTargetDescriptor
		 * - VertexSourceBindingDescriptor
		 */
		CPPX_ATTR_NO_DISCARD bool HasPendingStateChanges() const noexcept;

		/**
		 */
		CPPX_ATTR_NO_DISCARD bool IsRenderPassDescriptorSet() const noexcept;

		/**
		 * 
		 * @tparam TGraphicsPipelineStateObject 
		 * @return 
		 */
		template <typename TGraphicsPipelineStateObject = GraphicsPipelineStateObject>
		CPPX_ATTR_NO_DISCARD const TGraphicsPipelineStateObject * GetCurrentGraphicsPipelineStateObject() const noexcept;

		template <typename TGraphicsPipelineStateObject = GraphicsPipelineStateObject>
		CPPX_ATTR_NO_DISCARD const TGraphicsPipelineStateObject & GetCurrentGraphicsPipelineStateObjectRef() const;

		template <typename TRenderTargetDescriptor = RenderTargetDescriptor>
		CPPX_ATTR_NO_DISCARD const TRenderTargetDescriptor * GetCurrentRenderTargetDescriptor() const noexcept;

		template <typename TRenderTargetDescriptor = RenderTargetDescriptor>
		CPPX_ATTR_NO_DISCARD const TRenderTargetDescriptor & GetCurrentRenderTargetDescriptorRef() const;

		template <typename TRenderPassDescriptor = RenderPassDescriptor>
		CPPX_ATTR_NO_DISCARD const TRenderPassDescriptor * GetCurrentRenderPassDescriptor() const noexcept;

		template <typename TRenderPassDescriptor = RenderPassDescriptor>
		CPPX_ATTR_NO_DISCARD const TRenderPassDescriptor & GetCurrentRenderPassDescriptorRef() const;

		template <typename TVertexSourceBindingDescriptor = VertexSourceBindingDescriptor>
		CPPX_ATTR_NO_DISCARD const TVertexSourceBindingDescriptor * GetCurrentVertexSourceBindingDescriptor() const noexcept;

		template <typename TVertexSourceBindingDescriptor = VertexSourceBindingDescriptor>
		CPPX_ATTR_NO_DISCARD const TVertexSourceBindingDescriptor & GetCurrentVertexSourceBindingDescriptorRef() const;

	protected:
		cppx::bitmask<graphics_state_update_mask_value_t> ResetStateUpdateMask();

	protected:
		struct CurrentPipelineBindings
		{
			const GraphicsPipelineStateObject * pipelineStateObject = nullptr;

			const RenderPassDescriptor * renderPassDescriptor = nullptr;

			const RenderTargetDescriptor * renderTargetDescriptor = nullptr;

			const VertexSourceBindingDescriptor * vertexSourceBindingDescriptor = nullptr;
		};

		CurrentPipelineBindings _currentPipelineBindings;

		GraphicsPipelineDynamicConfig _currentPipelineDynamicConfig;

		cppx::bitmask<graphics_state_update_mask_value_t> _stateUpdateMask = 0;
	};

} // namespace Ic3::Graphics::GCI

#include "GraphicsPipelineStateController.inl"

#endif // __IC3_GRAPHICS_GCI_GRAPHICS_PIPELINE_STATE_CONTROLLER_H__
