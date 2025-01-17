
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
		GpuDevice & mGpuDevice;
		DX11CommandList * const mDX11CommandList = nullptr;
		ID3D11DeviceContext1 * const mD3D11DeviceContext1 = nullptr;

	public:
		DX11GraphicsPipelineStateController( DX11CommandList & pDX11CommandList );
		~DX11GraphicsPipelineStateController();

		CPPX_ATTR_NO_DISCARD const DX11IAVertexStreamDefinition & GetCurrentIAVertexStreamDefinition() const noexcept;

		CPPX_ATTR_NO_DISCARD DX11RenderTargetBindingData GetCurrentRenderTargetBinding() const noexcept;

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
		virtual bool SetShaderConstantBuffer( shader_input_ref_id_t pParamRefID, GpuBuffer & pConstantBuffer ) override;
		virtual bool SetShaderTextureImage( shader_input_ref_id_t pParamRefID, Texture & pTexture ) override;
		virtual bool SetShaderTextureSampler( shader_input_ref_id_t pParamRefID, Sampler & pSampler ) override;

	private:
		cppx::bitmask<uint32> ApplyCommonGraphicsConfigState( const DX11GraphicsPipelineStateObject & pGraphicsPSO );
		cppx::bitmask<uint32> ApplyGraphicsShaderState( const SeparableShaderCache & pSeparableShaders );
		void ApplyIAVertexStreamState( const DX11IAVertexStreamDefinition & pVertexStreamDefinition );
		void ApplyRenderTargetBinding( const DX11RenderTargetBindingData & pRenderTargetBindingData );

	private:
		DX11IAVertexStreamDefinition _dynamicIAVertexStreamDefinitionDX11;
		DX11RenderTargetBindingData _dynamicRenderTargetBindingDataDX11;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_DXCOMMON_PIPELINE_STATE_CONTROLLER_H__
