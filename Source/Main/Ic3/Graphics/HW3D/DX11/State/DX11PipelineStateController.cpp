
#include "DX11pipelineStateController.h"
#include "DX11pipelineStateObject.h"
#include "DX11CommonGraphicsConfig.h"
#include "DX11inputAssembler.h"
#include "DX11graphicsShaderState.h"
#include "DX11RenderTarget.h"
#include "../DX11CommandList.h"
#include "../Resources/DX11GpuBuffer.h"
#include "../Resources/DX11Sampler.h"
#include "../Resources/DX11Shader.h"
#include "../Resources/DX11Texture.h"

#include <Ic3/Graphics/GCI/State/InputAssemblerDynamicStates.h>
#include <Ic3/Graphics/GCI/State/RenderTargetDynamicStates.h>

namespace Ic3::Graphics::GCI
{

	DX11GraphicsPipelineStateController::DX11GraphicsPipelineStateController( DX11CommandList & pDX11CommandList )
	: GraphicsPipelineStateControllerSeparableShader()
	, mGpuDevice( pDX11CommandList.mGpuDevice )
	, mDX11CommandList( &pDX11CommandList )
	, mD3D11DeviceContext1( pDX11CommandList.mD3D11DeviceContext1.Get() )
	{}

	DX11GraphicsPipelineStateController::~DX11GraphicsPipelineStateController() = default;

	const DX11IAVertexStreamDefinition & DX11GraphicsPipelineStateController::GetCurrentIAVertexStreamDefinition() const noexcept
	{
		if( IsIAVertexStreamStateDynamic() )
		{
			return _dynamicIAVertexStreamDefinitionDX11;
		}
		else
		{
			const auto * dx11VertexStreamState = _currentCommonState.iaVertexStreamState->QueryInterface<DX11IAVertexStreamImmutableState>();
			return dx11VertexStreamState->mDX11VertexStreamDefinition;
		}
	}

	DX11RenderTargetBindingData DX11GraphicsPipelineStateController::GetCurrentRenderTargetBinding() const noexcept
	{
		if( IsRenderTargetStateDynamic() )
		{
			return _dynamicRenderTargetBindingDataDX11;
		}
		else
		{
			const auto * dx11RTBindingState = _currentCommonState.renderTargetBindingState->QueryInterface<DX11RenderTargetBindingImmutableState>();
			return dx11RTBindingState->mDX11RTBindingData;
		}
	}

	bool DX11GraphicsPipelineStateController::ApplyStateChanges()
	{
		cppx::bitmask<uint32> executedUpdatesMask = 0;

		if( _stateUpdateMask.is_set( E_GRAPHICS_STATE_UPDATE_FLAG_COMMON_PSO_BIT ) )
		{
			if( _stateUpdateMask.is_set_any_of( E_GRAPHICS_STATE_UPDATE_MASK_SEPARABLE_STATES_ALL ) )
			{
				auto * dx11GraphicsPSO = _currentCommonState.graphicsPSO->QueryInterface<DX11GraphicsPipelineStateObject>();
				const auto commonPSOUpdateMask = ApplyCommonGraphicsConfigState( *dx11GraphicsPSO );
				executedUpdatesMask.set( commonPSOUpdateMask );
			}

			if( _stateUpdateMask.is_set( eGraphicsStateUpdateFlagSeparableStateSHADER_LINKAGE_BIT ) )
			{
				const auto & separableShaderCache = GetCurrentSeparableShaders();
				const auto shaderUpdateMask = ApplyGraphicsShaderState( separableShaderCache );
				executedUpdatesMask.set( shaderUpdateMask );
			}
		}

		if( _stateUpdateMask.is_set( E_GRAPHICS_STATE_UPDATE_FLAG_COMMON_VERTEX_STREAM_BIT ) )
		{
			const auto & currentVertexStreamDefinition = GetCurrentIAVertexStreamDefinition();
			ApplyIAVertexStreamState( currentVertexStreamDefinition );
			executedUpdatesMask.set( E_GRAPHICS_STATE_UPDATE_FLAG_COMMON_VERTEX_STREAM_BIT );
		}

		if( _stateUpdateMask.is_set( E_GRAPHICS_STATE_UPDATE_FLAG_COMMON_RENDER_TARGET_BINDING_BIT ) )
		{
			const auto & currentRenderTargetBinding = GetCurrentRenderTargetBinding();
			ApplyRenderTargetBinding( currentRenderTargetBinding );
			executedUpdatesMask.set( E_GRAPHICS_STATE_UPDATE_FLAG_COMMON_RENDER_TARGET_BINDING_BIT );
		}

		return !executedUpdatesMask.empty();
	}

	bool DX11GraphicsPipelineStateController::SetGraphicsPipelineStateObject( const GraphicsPipelineStateObject & pGraphicsPSO )
	{
		return GraphicsPipelineStateControllerSeparableShader::SetGraphicsPipelineStateObject( pGraphicsPSO );
	}

	bool DX11GraphicsPipelineStateController::ResetGraphicsPipelineStateObject()
	{
		return GraphicsPipelineStateControllerSeparableShader::ResetGraphicsPipelineStateObject();
	}

	bool DX11GraphicsPipelineStateController::SetIAVertexStreamState( const IAVertexStreamDynamicState & pIAVertexStreamState )
	{
		bool updateResult = GraphicsPipelineStateControllerSeparableShader::SetIAVertexStreamState( pIAVertexStreamState );

		if( updateResult )
		{
			auto dx11VertexStreamDefinition =
					SMU::TranslateDX11IAVertexStreamDefinition( pIAVertexStreamState.GetVertexStreamDefinition() );

			_dynamicIAVertexStreamDefinitionDX11 = dx11VertexStreamDefinition;
		}

		return updateResult;
	}

	bool DX11GraphicsPipelineStateController::SetIAVertexStreamState( const IAVertexStreamImmutableState & pIAVertexStreamState )
	{
		return GraphicsPipelineStateControllerSeparableShader::SetIAVertexStreamState( pIAVertexStreamState );
	}

	bool DX11GraphicsPipelineStateController::ResetIAVertexStreamState()
	{
		return GraphicsPipelineStateControllerSeparableShader::ResetIAVertexStreamState();
	}

	bool DX11GraphicsPipelineStateController::SetRenderTargetBindingState( const RenderTargetBindingDynamicState & pRenderTargetBindingState )
	{
		bool updateResult = GraphicsPipelineStateControllerSeparableShader::SetRenderTargetBindingState( pRenderTargetBindingState );

		if( updateResult )
		{
			auto dx11RenderTargetBinding =
					SMU::CreateRenderTargetBindingDataDX11( *mGpuDevice.QueryInterface<DX11GpuDevice>(), pRenderTargetBindingState.GetBindingDefinition() );

			_dynamicRenderTargetBindingDataDX11 = dx11RenderTargetBinding;
		}

		return updateResult;
	}

	bool DX11GraphicsPipelineStateController::SetRenderTargetBindingState( const RenderTargetBindingImmutableState & pRenderTargetBindingState )
	{
		return GraphicsPipelineStateControllerSeparableShader::SetRenderTargetBindingState( pRenderTargetBindingState );
	}

	bool DX11GraphicsPipelineStateController::ResetRenderTargetBindingState()
	{
		return GraphicsPipelineStateControllerSeparableShader::ResetRenderTargetBindingState();
	}

	bool DX11GraphicsPipelineStateController::SetViewport( const ViewportDesc & pViewportDesc )
	{
		D3D11_VIEWPORT viewportDesc;
		viewportDesc.TopLeftX = static_cast<float>( pViewportDesc.origin.x );
		viewportDesc.TopLeftY = static_cast<float>( pViewportDesc.origin.y );
		viewportDesc.Width = static_cast<float>( pViewportDesc.size.x );
		viewportDesc.Height = static_cast<float>( pViewportDesc.size.y );
		viewportDesc.MinDepth = pViewportDesc.depthRange.zNear;
		viewportDesc.MaxDepth = pViewportDesc.depthRange.zFar;

		mDX11CommandList->mD3D11DeviceContext1->RSSetViewports( 1, &viewportDesc );

		return true;
	}

	bool DX11GraphicsPipelineStateController::SetShaderConstant( shader_input_ref_id_t pParamRefID, const void * pData )
	{
		return false;
	}

	bool DX11GraphicsPipelineStateController::SetShaderConstantBuffer( shader_input_ref_id_t pParamRefID, GpuBuffer & pConstantBuffer )
	{
		const auto & shaderInputSignature = GetShaderInputSignature();
		const auto & descriptorInfo = shaderInputSignature.GetDescriptorInfo( pParamRefID );

		Ic3DebugAssert( descriptorInfo.dDescriptorType == EShaderInputDescriptorType::Resource );
		Ic3DebugAssert( descriptorInfo.uResourceInfo.resourceType == EShaderInputResourceType::CBVConstantBuffer );

		auto updatedStagesMask = make_bitmask<EShaderStageFlags>();

		if( descriptorInfo.dShaderVisibilityMask != 0 )
		{
			auto * dx11Buffer = pConstantBuffer.QueryInterface<DX11GpuBuffer>();
			auto * d3d11Buffer = dx11Buffer->mD3D11Buffer.Get();

			if( descriptorInfo.dShaderVisibilityMask.is_set( E_SHADER_STAGE_FLAG_GRAPHICS_VERTEX_BIT ) )
			{
				mD3D11DeviceContext1->VSSetConstantBuffers( descriptorInfo.uResourceInfo.resourceBaseRegisterIndex, 1, &d3d11Buffer );
				updatedStagesMask.set( E_SHADER_STAGE_FLAG_GRAPHICS_VERTEX_BIT );
			}

			if( descriptorInfo.dShaderVisibilityMask.is_set( E_SHADER_STAGE_FLAG_GRAPHICS_HULL_BIT) )
			{
				mD3D11DeviceContext1->HSSetConstantBuffers( descriptorInfo.uResourceInfo.resourceBaseRegisterIndex, 1, &d3d11Buffer );
				updatedStagesMask.set( E_SHADER_STAGE_FLAG_GRAPHICS_HULL_BIT );
			}

			if( descriptorInfo.dShaderVisibilityMask.is_set( E_SHADER_STAGE_FLAG_GRAPHICS_DOMAIN_BIT ) )
			{
				mD3D11DeviceContext1->DSSetConstantBuffers( descriptorInfo.uResourceInfo.resourceBaseRegisterIndex, 1, &d3d11Buffer );
				updatedStagesMask.set( E_SHADER_STAGE_FLAG_GRAPHICS_DOMAIN_BIT );
			}

			if( descriptorInfo.dShaderVisibilityMask.is_set( E_SHADER_STAGE_FLAG_GRAPHICS_GEOMETRY_BIT ) )
			{
				mD3D11DeviceContext1->GSSetConstantBuffers( descriptorInfo.uResourceInfo.resourceBaseRegisterIndex, 1, &d3d11Buffer );
				updatedStagesMask.set( E_SHADER_STAGE_FLAG_GRAPHICS_GEOMETRY_BIT );
			}

			if( descriptorInfo.dShaderVisibilityMask.is_set( E_SHADER_STAGE_FLAG_GRAPHICS_PIXEL_BIT ) )
			{
				mD3D11DeviceContext1->PSSetConstantBuffers( descriptorInfo.uResourceInfo.resourceBaseRegisterIndex, 1, &d3d11Buffer );
				updatedStagesMask.set( E_SHADER_STAGE_FLAG_GRAPHICS_PIXEL_BIT );
			}
		}

		return !updatedStagesMask.empty();
	}

	bool DX11GraphicsPipelineStateController::SetShaderTextureImage( shader_input_ref_id_t pParamRefID, Texture & pTexture )
	{
		const auto & shaderInputSignature = GetShaderInputSignature();
		const auto & descriptorInfo = shaderInputSignature.GetDescriptorInfo( pParamRefID );

		Ic3DebugAssert( descriptorInfo.dDescriptorType == EShaderInputDescriptorType::Resource );
		Ic3DebugAssert( descriptorInfo.uResourceInfo.resourceType == EShaderInputResourceType::SRVTextureImage );

		auto updatedStagesMask = make_bitmask<EShaderStageFlags>();

		if( descriptorInfo.dShaderVisibilityMask != 0 )
		{
			auto * dx11Texture = pTexture.QueryInterface<DX11Texture>();
			auto * d3d11TextureSRV = dx11Texture->mD3D11DefaultSRV.Get();

			if( descriptorInfo.dShaderVisibilityMask.is_set( E_SHADER_STAGE_FLAG_GRAPHICS_VERTEX_BIT ) )
			{
				mD3D11DeviceContext1->VSSetShaderResources( descriptorInfo.uResourceInfo.resourceBaseRegisterIndex, 1, &d3d11TextureSRV );
				updatedStagesMask.set( E_SHADER_STAGE_FLAG_GRAPHICS_VERTEX_BIT );
			}

			if( descriptorInfo.dShaderVisibilityMask.is_set( E_SHADER_STAGE_FLAG_GRAPHICS_HULL_BIT ) )
			{
				mD3D11DeviceContext1->HSSetShaderResources( descriptorInfo.uResourceInfo.resourceBaseRegisterIndex, 1, &d3d11TextureSRV );
				updatedStagesMask.set( E_SHADER_STAGE_FLAG_GRAPHICS_HULL_BIT );
			}

			if( descriptorInfo.dShaderVisibilityMask.is_set( E_SHADER_STAGE_FLAG_GRAPHICS_DOMAIN_BIT ) )
			{
				mD3D11DeviceContext1->DSSetShaderResources( descriptorInfo.uResourceInfo.resourceBaseRegisterIndex, 1, &d3d11TextureSRV );
				updatedStagesMask.set( E_SHADER_STAGE_FLAG_GRAPHICS_DOMAIN_BIT );
			}

			if( descriptorInfo.dShaderVisibilityMask.is_set( E_SHADER_STAGE_FLAG_GRAPHICS_GEOMETRY_BIT ) )
			{
				mD3D11DeviceContext1->GSSetShaderResources( descriptorInfo.uResourceInfo.resourceBaseRegisterIndex, 1, &d3d11TextureSRV );
				updatedStagesMask.set( E_SHADER_STAGE_FLAG_GRAPHICS_GEOMETRY_BIT );
			}

			if( descriptorInfo.dShaderVisibilityMask.is_set( E_SHADER_STAGE_FLAG_GRAPHICS_PIXEL_BIT ) )
			{
				mD3D11DeviceContext1->PSSetShaderResources( descriptorInfo.uResourceInfo.resourceBaseRegisterIndex, 1, &d3d11TextureSRV );
				updatedStagesMask.set( E_SHADER_STAGE_FLAG_GRAPHICS_PIXEL_BIT );
			}
		}

		return !updatedStagesMask.empty();
	}

	bool DX11GraphicsPipelineStateController::SetShaderTextureSampler( shader_input_ref_id_t pParamRefID, Sampler & pSampler )
	{
		const auto & shaderInputSignature = GetShaderInputSignature();
		const auto & descriptorInfo = shaderInputSignature.GetDescriptorInfo( pParamRefID );

		Ic3DebugAssert( descriptorInfo.dDescriptorType == EShaderInputDescriptorType::Sampler );

		auto updatedStagesMask = make_bitmask<EShaderStageFlags>();

		if( descriptorInfo.dShaderVisibilityMask != 0 )
		{
			auto * dx11Sampler = pSampler.QueryInterface<DX11Sampler>();
			auto * d3d11SamplerState = dx11Sampler->mD3D11SamplerState.Get();

			if( descriptorInfo.dShaderVisibilityMask.is_set( E_SHADER_STAGE_FLAG_GRAPHICS_VERTEX_BIT ) )
			{
				mD3D11DeviceContext1->VSSetSamplers( descriptorInfo.uSamplerInfo.samplerBindingIndex, 1, &d3d11SamplerState );
				updatedStagesMask.set( E_SHADER_STAGE_FLAG_GRAPHICS_VERTEX_BIT );
			}

			if( descriptorInfo.dShaderVisibilityMask.is_set( E_SHADER_STAGE_FLAG_GRAPHICS_HULL_BIT ) )
			{
				mD3D11DeviceContext1->HSSetSamplers( descriptorInfo.uSamplerInfo.samplerBindingIndex, 1, &d3d11SamplerState );
				updatedStagesMask.set( E_SHADER_STAGE_FLAG_GRAPHICS_HULL_BIT );
			}

			if( descriptorInfo.dShaderVisibilityMask.is_set( E_SHADER_STAGE_FLAG_GRAPHICS_DOMAIN_BIT ) )
			{
				mD3D11DeviceContext1->DSSetSamplers( descriptorInfo.uSamplerInfo.samplerBindingIndex, 1, &d3d11SamplerState );
				updatedStagesMask.set( E_SHADER_STAGE_FLAG_GRAPHICS_DOMAIN_BIT );
			}

			if( descriptorInfo.dShaderVisibilityMask.is_set( E_SHADER_STAGE_FLAG_GRAPHICS_GEOMETRY_BIT ) )
			{
				mD3D11DeviceContext1->GSSetSamplers( descriptorInfo.uSamplerInfo.samplerBindingIndex, 1, &d3d11SamplerState );
				updatedStagesMask.set( E_SHADER_STAGE_FLAG_GRAPHICS_GEOMETRY_BIT );
			}

			if( descriptorInfo.dShaderVisibilityMask.is_set( E_SHADER_STAGE_FLAG_GRAPHICS_PIXEL_BIT ) )
			{
				mD3D11DeviceContext1->PSSetSamplers( descriptorInfo.uSamplerInfo.samplerBindingIndex, 1, &d3d11SamplerState );
				updatedStagesMask.set( E_SHADER_STAGE_FLAG_GRAPHICS_PIXEL_BIT );
			}
		}

		return !updatedStagesMask.empty();
	}

	cppx::bitmask<uint32> DX11GraphicsPipelineStateController::ApplyCommonGraphicsConfigState( const DX11GraphicsPipelineStateObject & pGraphicsPSO )
	{
		cppx::bitmask<uint32> executedUpdatesMask = 0;

		if( _stateUpdateMask.is_set( eGraphicsStateUpdateFlagSeparableStateBLEND_BIT ) )
		{
			const auto & blendState = pGraphicsPSO.GetBlendState();
			const auto & dynamicState = GetRenderPassDynamicState();

			if( dynamicState.activeStateMask.is_set( E_GRAPHICS_PIPELINE_DYNAMIC_STATE_FLAG_BLEND_CONSTANT_COLOR_BIT ) )
			{
				mDX11CommandList->mD3D11DeviceContext1->OMSetBlendState(
						blendState.mD3D11BlendState.Get(),
						dynamicState.blendConstantColor.mRGBA,
						0xFFFFFFFF );
			}
			else
			{
				mDX11CommandList->mD3D11DeviceContext1->OMSetBlendState(
						blendState.mD3D11BlendState.Get(),
						nullptr,
						0xFFFFFFFF );
			}

			executedUpdatesMask.set( eGraphicsStateUpdateFlagSeparableStateBLEND_BIT );
		}

		if( _stateUpdateMask.is_set( eGraphicsStateUpdateFlagSeparableStateDEPTH_STENCIL_BIT ) )
		{
			const auto & depthStencilState = pGraphicsPSO.GetDepthStencilState();
			const auto & dynamicState = GetRenderPassDynamicState();

			if( dynamicState.activeStateMask.is_set( E_GRAPHICS_PIPELINE_DYNAMIC_STATE_FLAG_STENCIL_REF_VALUE_BIT ) )
			{
				mDX11CommandList->mD3D11DeviceContext1->OMSetDepthStencilState(
						depthStencilState.mD3D11DepthStencilState.Get(),
						dynamicState.stencilTestRefValue );
			}
			else
			{
				mDX11CommandList->mD3D11DeviceContext1->OMSetDepthStencilState(
						depthStencilState.mD3D11DepthStencilState.Get(),
						0x00 );
			}

			executedUpdatesMask.set( eGraphicsStateUpdateFlagSeparableStateDEPTH_STENCIL_BIT );
		}

		if( _stateUpdateMask.is_set( eGraphicsStateUpdateFlagSeparableStateRASTERIZER_BIT ) )
		{
			const auto & rasterizerState = pGraphicsPSO.GetRasterizerState();
			const auto & dynamicState = GetRenderPassDynamicState();

			mDX11CommandList->mD3D11DeviceContext1->RSSetState( rasterizerState.mD3D11RasterizerState.Get() );

			executedUpdatesMask.set( eGraphicsStateUpdateFlagSeparableStateRASTERIZER_BIT );
		}

		if( _stateUpdateMask.is_set( eGraphicsStateUpdateFlagSeparableStateIA_INPUT_LAYOUT_BIT ) )
		{
			const auto & iaInputLayoutState = pGraphicsPSO.GetIAInputLayoutState();
			const auto & dynamicState = GetRenderPassDynamicState();

			mDX11CommandList->mD3D11DeviceContext1->IASetPrimitiveTopology( iaInputLayoutState.mD3D11PrimitiveTopology );
			mDX11CommandList->mD3D11DeviceContext1->IASetInputLayout( iaInputLayoutState.mD3D11InputLayout.Get() );

			executedUpdatesMask.set( eGraphicsStateUpdateFlagSeparableStateIA_INPUT_LAYOUT_BIT );
		}

		return executedUpdatesMask;
	}

	cppx::bitmask<uint32> DX11GraphicsPipelineStateController::ApplyGraphicsShaderState( const SeparableShaderCache & pSeparableShaders )
	{
		cppx::bitmask<uint32> executedUpdatesMask = 0;

		if( _stateUpdateMask.is_set( E_GRAPHICS_STATE_UPDATE_FLAG_SEPARABLE_SHADER_VERTEX_BIT ) )
		{
			auto * dx11Shader = pSeparableShaders.vertexShader->QueryInterface<DX11Shader>();
			mDX11CommandList->mD3D11DeviceContext1->VSSetShader( dx11Shader->mD3D11VertexShader, nullptr, 0 );
			executedUpdatesMask.set( E_GRAPHICS_STATE_UPDATE_FLAG_SEPARABLE_SHADER_VERTEX_BIT );
		}

		if( _stateUpdateMask.is_set( E_GRAPHICS_STATE_UPDATE_FLAG_SEPARABLE_SHADER_HULL_BIT ) )
		{
			auto * dx11Shader = pSeparableShaders.hullShader->QueryInterface<DX11Shader>();
			mDX11CommandList->mD3D11DeviceContext1->HSSetShader( dx11Shader->mD3D11HullShader, nullptr, 0 );
			executedUpdatesMask.set( E_GRAPHICS_STATE_UPDATE_FLAG_SEPARABLE_SHADER_HULL_BIT );
		}

		if( _stateUpdateMask.is_set( E_GRAPHICS_STATE_UPDATE_FLAG_SEPARABLE_SHADER_DOMAIN_BIT ) )
		{
			auto * dx11Shader = pSeparableShaders.domainShader->QueryInterface<DX11Shader>();
			mDX11CommandList->mD3D11DeviceContext1->DSSetShader( dx11Shader->mD3D11DomainShader, nullptr, 0 );
			executedUpdatesMask.set( E_GRAPHICS_STATE_UPDATE_FLAG_SEPARABLE_SHADER_DOMAIN_BIT );
		}

		if( _stateUpdateMask.is_set( E_GRAPHICS_STATE_UPDATE_FLAG_SEPARABLE_SHADER_GEOMETRY_BIT ) )
		{
			auto * dx11Shader = pSeparableShaders.geometryShader->QueryInterface<DX11Shader>();
			mDX11CommandList->mD3D11DeviceContext1->GSSetShader( dx11Shader->mD3D11GeometryShader, nullptr, 0 );
			executedUpdatesMask.set( E_GRAPHICS_STATE_UPDATE_FLAG_SEPARABLE_SHADER_GEOMETRY_BIT );
		}

		if( _stateUpdateMask.is_set( E_GRAPHICS_STATE_UPDATE_FLAG_SEPARABLE_SHADER_PIXEL_BIT ) )
		{
			auto * dx11Shader = pSeparableShaders.pixelShader->QueryInterface<DX11Shader>();
			mDX11CommandList->mD3D11DeviceContext1->PSSetShader( dx11Shader->mD3D11PixelShader, nullptr, 0 );
			executedUpdatesMask.set( E_GRAPHICS_STATE_UPDATE_FLAG_SEPARABLE_SHADER_PIXEL_BIT );
		}

		return executedUpdatesMask;
	}

	void DX11GraphicsPipelineStateController::ApplyIAVertexStreamState( const DX11IAVertexStreamDefinition & pVertexStreamDefinition )
	{
		const auto & indexBufferBinding = pVertexStreamDefinition.indexBufferBinding;
		const auto & vertexBufferBindings = pVertexStreamDefinition.vertexBufferBindings;

		mDX11CommandList->mD3D11DeviceContext1->IASetIndexBuffer(
				indexBufferBinding.buffer,
				indexBufferBinding.format,
				indexBufferBinding.offset );

		for( const auto & vertexStreamRange : vertexBufferBindings.activeRanges )
		{
			mDX11CommandList->mD3D11DeviceContext1->IASetVertexBuffers(
					vertexStreamRange.firstIndex,
					vertexStreamRange.length,
					&( vertexBufferBindings.bindingData.bufferArray[vertexStreamRange.firstIndex] ),
					&( vertexBufferBindings.bindingData.strideArray[vertexStreamRange.firstIndex] ),
					&( vertexBufferBindings.bindingData.offsetArray[vertexStreamRange.firstIndex] ) );
		}
	}

	void DX11GraphicsPipelineStateController::ApplyRenderTargetBinding( const DX11RenderTargetBindingData & pRenderTargetBindingData )
	{
		if( !pRenderTargetBindingData.activeAttachmentsMask.is_set_any_of( E_RT_ATTACHMENT_MASK_COLOR_ALL ) )
		{
			mDX11CommandList->mD3D11DeviceContext1->OMSetRenderTargets(
				0u,
				nullptr,
				pRenderTargetBindingData.d3d11DepthStencilAttachmentDSView );
		}
		else
		{
			mDX11CommandList->mD3D11DeviceContext1->OMSetRenderTargets(
				GCM::RT_MAX_COLOR_ATTACHMENTS_NUM,
				pRenderTargetBindingData.d3d11ColorAttachmentRTViewArray,
				pRenderTargetBindingData.d3d11DepthStencilAttachmentDSView );
		}
	}

} // namespace Ic3::Graphics::GCI
