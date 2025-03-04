
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_DXCOMMON_PIPELINE_STATE_CONTROLLER_H__
#define __IC3_GRAPHICS_HW3D_DXCOMMON_PIPELINE_STATE_CONTROLLER_H__

#include "DX11inputAssembler.h"
#include "DX11RenderTarget.h"
#include <Ic3/Graphics/GCI/State/SeparablePipelineState.h>

namespace Ic3::Graphics::GCI
{

	class DX11GraphicsPipelineStateObject;

	/// @brief
	class DX11GraphicsPipelineStateController : public GraphicsPipelineStateControllerSeparableShader
	{
		friend class DX11CommandList;

	public:
		GPUDevice & mGPUDevice;
		DX11CommandList * const mDX11CommandList = nullptr;
		ID3D11DeviceContext1 * const mD3D11DeviceContext1 = nullptr;

	public:
		DX11GraphicsPipelineStateController( DX11CommandList & pDX11CommandList );
		~DX11GraphicsPipelineStateController();

		CPPX_ATTR_NO_DISCARD const DX11IASourceBindingDefinition & GetCurrentIASourceBindingDefinition() const noexcept;

		CPPX_ATTR_NO_DISCARD DX11RenderTargetBinding GetCurrentRenderTargetBinding() const noexcept;

		virtual bool ApplyStateChanges() override;

		virtual bool SetGraphicsPipelineStateObject( const GraphicsPipelineStateObject & pGraphicsPipelineStateObject ) override;
		virtual bool ResetGraphicsPipelineStateObject() override;

		virtual bool SetVertexSourceBindingDescriptor( const IAVertexStreamDynamicState & pVertexSourceBinding ) override;
		virtual bool SetVertexSourceBindingDescriptor( const VertexSourceBindingDescriptor & pVertexSourceBinding ) override;
		virtual bool ResetVertexSourceBindingDescriptor() override;

		virtual bool SetRenderTargetBindingState( const RenderTargetBindingDynamicState & pRenderTargetBindingState ) override;
		virtual bool SetRenderTargetBindingState( const RenderTargetBindingCompiledState & pRenderTargetBindingState ) override;
		virtual bool ResetRenderTargetBindingState() override;

		virtual bool SetViewport( const ViewportDesc & pViewportDesc ) override;
		virtual bool SetShaderConstant( shader_input_ref_id_t pParamRefID, const void * pData ) override;
		virtual bool SetShaderConstantBuffer( shader_input_ref_id_t pParamRefID, GPUBuffer & pConstantBuffer ) override;
		virtual bool SetShaderTextureImage( shader_input_ref_id_t pParamRefID, Texture & pTexture ) override;
		virtual bool SetShaderTextureSampler( shader_input_ref_id_t pParamRefID, Sampler & pSampler ) override;

	private:
		cppx::bitmask<uint32> ApplyCommonGraphicsConfigState( const DX11GraphicsPipelineStateObject & pGraphicsPipelineStateObject );
		cppx::bitmask<uint32> ApplyGraphicsShaderState( const SeparableShaderCache & pSeparableShaders );
		void ApplyVertexSourceBindingDescriptor( const DX11IASourceBindingDefinition & pSourceBindingDefinition );
		void ApplyRenderTargetBinding( const DX11RenderTargetBinding & pRenderTargetBinding );

	private:
		DX11IASourceBindingDefinition _dynamicIASourceBindingDefinitionDX11;
		DX11RenderTargetBinding _dynamicRenderTargetBindingDX11;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_DXCOMMON_PIPELINE_STATE_CONTROLLER_H__
