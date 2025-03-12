
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_GLC_PIPELINE_STATE_CONTROLLER_H__
#define __IC3_GRAPHICS_HW3D_GLC_PIPELINE_STATE_CONTROLLER_H__

#include "GLGraphicsPipelineStateIA.h"
#include "GLGraphicsPipelineStateRTO.h"
#include "GLGlobalStateCache.h"
#include <Ic3/Graphics/GCI/State/GraphicsPipelineStateImplRenderPassGeneric.h>
#include <Ic3/Graphics/GCI/State/GraphicsPipelineStateImplSeparableState.h>

namespace Ic3::Graphics::GCI
{

	struct GLIAIndexBufferBinding;
	struct GLIAVertexBufferArrayBindings;

	class GLCommandList;
	class GLGraphicsPipelineStateObject;
	class GLGraphicsShaderLinkageDescriptorCore;
	class GLGraphicsShaderLinkageDescriptorCompat;

	/// @brief
	class GLGraphicsPipelineStateController
			: public GraphicsPipelineStateControllerRenderPassGeneric<GraphicsPipelineStateControllerSeparable>
	{
		friend class GLCommandList;

	public:
		GLGPUDevice & mGLGPUDevice;

	public:
		GLGraphicsPipelineStateController( GLCommandList & pGLCommandList );
		virtual ~GLGraphicsPipelineStateController();

		CPPX_ATTR_NO_DISCARD const GLDrawTopologyProperties & GetCurrentDrawTopologyProperties() const noexcept;

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
		CPPX_ATTR_NO_DISCARD const GLRenderTargetBinding & GetGLRenderTargetBinding() const noexcept;

		CPPX_ATTR_NO_DISCARD const GLIAVertexSourceBinding & GetGLVertexSourceBinding() const noexcept;

	private:
		using BaseStateControllerType = GraphicsPipelineStateControllerRenderPassGeneric<GraphicsPipelineStateControllerSeparable>;

		virtual void UpdateShaderInputInlineConstantData(
				const GLGraphicsShaderLinkageDescriptor & pShaderLinkageDescriptor,
				const ShaderInputParameterConstant & pConstantInfo,
				const void * pConstantData ) = 0;

		cppx::bitmask<uint32> BindCommonConfigDescriptors( const GLGraphicsPipelineStateObject & pGLGraphicsPipelineStateObject );

		static void ApplyGLRenderTargetBinding( const GLRenderTargetBinding & pGLRenderTargetBinding );

		static void ApplyGraphicsPipelineDynamicConfig( const GraphicsPipelineDynamicConfig & pDynamicPipelineConfig );

	protected:
		struct GLCurrentPipelineBindings
		{
			const GLRenderTargetBinding * renderTargetBinding = nullptr;

			const GLIAVertexSourceBinding * vertexSourceBinding = nullptr;
		};

		GLDrawTopologyProperties _glcCurrentDrawTopologyProperties;
		GLCurrentPipelineBindings _glcCurrentPipelineBindings;
		GLGlobalStateCache _glcGlobalStateCache;
	};

	class GLGraphicsPipelineStateControllerCore : public GLGraphicsPipelineStateController
	{
	public:
		GLGraphicsPipelineStateControllerCore( GLCommandList & pGLCommandList );
		virtual ~GLGraphicsPipelineStateControllerCore();

		virtual bool ApplyStateChanges() override final;

	private:
		virtual void UpdateShaderInputInlineConstantData(
				const GLGraphicsShaderLinkageDescriptor & pShaderLinkageDescriptor,
				const ShaderInputParameterConstant & pConstantInfo,
				const void * pConstantData ) override final;

		static void ApplyVertexBufferBindings( const GLIAVertexBufferArrayBindings & pVertexBufferBindings );
	};

	class GLGraphicsPipelineStateControllerCompat : public GLGraphicsPipelineStateController
	{
	public:
		GLGraphicsPipelineStateControllerCompat( GLCommandList & pGLCommandList );
		virtual ~GLGraphicsPipelineStateControllerCompat();

		virtual bool ApplyStateChanges() override final;

	private:
		virtual void UpdateShaderInputInlineConstantData(
				const GLGraphicsShaderLinkageDescriptor & pShaderState,
				const ShaderInputParameterConstant & pConstantInfo,
				const void * pConstantData ) override final;

		const GLVertexArrayObject & GetCachedVertexArrayObject(
				const GLIAVertexAttributeLayout & pGLAttributeLayoutDefinition,
				const GLIAVertexSourceBinding & pGLVertexSourceBinding );
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_GLC_PIPELINE_STATE_CONTROLLER_H__
