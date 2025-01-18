
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_GLCOMMON_PIPELINE_STATE_CONTROLLER_H__
#define __IC3_GRAPHICS_HW3D_GLCOMMON_PIPELINE_STATE_CONTROLLER_H__

#include "GLInputAssembler.h"
#include "GLRenderTarget.h"
#include "GLVertexArrayObjectCache.h"
#include "GLGlobalStateCache.h"
#include "../Objects/GLVertexArrayObject.h"
#include <Ic3/Graphics/GCI/State/SeparablePipelineState.h>

namespace Ic3::Graphics::GCI
{

	struct GLIAIndexBufferBinding;
	struct GLIAVertexBuffersBindings;

	class GLCommandList;
	class GLGraphicsPipelineStateObject;
	class GLGraphicsShaderLinkageImmutableStateCore;
	class GLGraphicsShaderLinkageImmutableStateCompat;

	/// @brief
	class GLGraphicsPipelineStateController : public GraphicsPipelineStateControllerSeparable
	{
		friend class GLCommandList;

	public:
		GLGraphicsPipelineStateController();
		~GLGraphicsPipelineStateController();

		CPPX_ATTR_NO_DISCARD const GLDrawTopologyProperties & GetGLDrawTopologyProperties() const noexcept;

		CPPX_ATTR_NO_DISCARD const GLIAVertexStreamDefinition & GetCurrentIAVertexStreamDefinition() const noexcept;

		CPPX_ATTR_NO_DISCARD GLRenderTargetBindingInfo GetCurrentRenderTargetBindingInfo() const noexcept;

		virtual bool ApplyStateChanges() override;

		virtual bool SetGraphicsPipelineStateObject( const GraphicsPipelineStateObject & pGraphicsPSO ) override;
		virtual bool ResetGraphicsPipelineStateObject() override;

		virtual bool SetIAVertexStreamState( const IAVertexStreamDynamicState & pIAVertexStreamState ) override;
		virtual bool SetIAVertexStreamState( const IAVertexStreamImmutableState & pIAVertexStreamState ) override;
		virtual bool ResetIAVertexStreamState() override;

		virtual bool SetRenderTargetBindingState( const RenderTargetBindingDynamicState & pRenderTargetBindingState ) override;
		virtual bool SetRenderTargetBindingState( const RenderTargetBindingImmutableState & pRenderTargetBindingState ) override;
		virtual bool ResetRenderTargetBindingState() override;

		virtual bool SetViewport( const ViewportDesc & pViewportDesc ) override;
		virtual bool SetShaderConstant( shader_input_ref_id_t pParamRefID, const void * pData ) override;
		virtual bool SetShaderConstantBuffer( shader_input_ref_id_t pParamRefID, GPUBuffer & pConstantBuffer ) override;
		virtual bool SetShaderTextureImage( shader_input_ref_id_t pParamRefID, Texture & pTexture ) override;
		virtual bool SetShaderTextureSampler( shader_input_ref_id_t pParamRefID, Sampler & pSampler ) override;

	private:
		cppx::bitmask<uint32> ApplyCommonGraphicsConfigState( const GLGraphicsPipelineStateObject & pGraphicsPSO );
		static void ApplyGraphicsPipelineDynamicState( const GraphicsPipelineDynamicState & pDynamicState );
		static void ApplyGLRenderTargetBinding( const GLRenderTargetBindingInfo & pGLRenderTargetBinding );

	private:
		virtual void UpdateShaderInputInlineConstantData(
				const GLGraphicsShaderLinkageImmutableState & pShaderState,
				const ShaderInputParameterConstant & pConstantInfo,
				const void * pConstantData ) = 0;

	protected:
		GLDrawTopologyProperties _currentDrawTopologyProperties;
		GLIAVertexStreamDefinition _dynamicIAVertexStreamDefinition;
		GLRenderTargetBindingDefinition _dynamicRenderTargetBindingDefinition;
		GLGlobalStateCache _globalStateCache;
	};

	class GLGraphicsPipelineStateControllerCore : public GLGraphicsPipelineStateController
	{
	public:
		virtual bool ApplyStateChanges() override final;

	private:
		virtual void UpdateShaderInputInlineConstantData(
				const GLGraphicsShaderLinkageImmutableState & pShaderState,
				const ShaderInputParameterConstant & pConstantInfo,
				const void * pConstantData ) override final;

		static void ApplyGLIAVertexBufferBindings( const GLIAVertexBuffersBindings & pVertexBufferBindings );
	};

	class GLGraphicsPipelineStateControllerCompat : public GLGraphicsPipelineStateController
	{
	public:
		virtual bool ApplyStateChanges() override final;

	private:
		virtual void UpdateShaderInputInlineConstantData(
				const GLGraphicsShaderLinkageImmutableState & pShaderState,
				const ShaderInputParameterConstant & pConstantInfo,
				const void * pConstantData ) override final;

		const GLVertexArrayObject & GetCachedVertexArrayObject(
				const GLIAInputLayoutImmutableStateCompat & pInputLayoutState,
				const GLIAVertexStreamImmutableState & pVertexStreamState );

		const GLVertexArrayObject & GetCachedVertexArrayObject(
				const GLIAInputLayoutDefinition & pInputLayoutDefinition,
				const GLIAVertexStreamDefinition & pVertexStreamDefinition );

	private:
		GLVertexArrayObjectCache _vaoCache;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_GLCOMMON_PIPELINE_STATE_CONTROLLER_H__
