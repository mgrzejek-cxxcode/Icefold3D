
#include "DX11GraphicsPipelineStateController.h"
#include "DX11GraphicsPipelineStateCommon.h"
#include "DX11GraphicsPipelineStateIA.h"
#include "DX11GraphicsPipelineStateObject.h"
#include "DX11GraphicsPipelineStateRTO.h"
#include <Ic3/Graphics/HW3D/DX11/DX11GPUDevice.h>
#include <Ic3/Graphics/HW3D/DX11/DX11CommandList.h>
#include <Ic3/Graphics/HW3D/DX11/Resources/DX11GPUBuffer.h>
#include <Ic3/Graphics/HW3D/DX11/Resources/DX11Sampler.h>
#include <Ic3/Graphics/HW3D/DX11/Resources/DX11Shader.h>
#include <Ic3/Graphics/HW3D/DX11/Resources/DX11Texture.h>
#include <Ic3/Graphics/GCI/State/GraphicsPipelineStateDescriptorIADynamic.h>
#include <Ic3/Graphics/GCI/State/GraphicsPipelineStateDescriptorRTODynamic.h>

namespace Ic3::Graphics::GCI
{

	DX11GraphicsPipelineStateController::DX11GraphicsPipelineStateController( DX11CommandList & pDX11CommandList )
	: GraphicsPipelineStateControllerRenderPassGeneric()
	, mGPUDevice( pDX11CommandList.mGPUDevice )
	, mDX11CommandList( &pDX11CommandList )
	, mD3D11DeviceContext1( pDX11CommandList.mD3D11DeviceContext1.Get() )
	{}

	DX11GraphicsPipelineStateController::~DX11GraphicsPipelineStateController() = default;

	bool DX11GraphicsPipelineStateController::ApplyStateChanges()
	{
		cppx::bitmask<uint32> executedUpdatesMask = 0;

		if( _stateUpdateMask.is_set( eGraphicsStateUpdateFlagCommonPipelineStateObjectBit ) )
		{
			if( _stateUpdateMask.is_set_any_of( eGraphicsStateUpdateMaskSeparableStatesAll ) )
			{
				const auto * dx11GraphicsPipelineStateObject = GetCurrentGraphicsPipelineStateObject<DX11GraphicsPipelineStateObject>();
				const auto commonPSOUpdateMask = BindCommonConfigDescriptors( *dx11GraphicsPipelineStateObject );
				executedUpdatesMask.set( commonPSOUpdateMask );
			}

			if( _stateUpdateMask.is_set( eGraphicsStateUpdateFlagSeparableStateShaderLinkageBit ) )
			{
				const auto & separableShadersCache = GetSeparableShaders();
				const auto shaderUpdateMask = BindSeparableGraphicsShaders( separableShadersCache );
				executedUpdatesMask.set( shaderUpdateMask );
			}
		}

		if( _stateUpdateMask.is_set( eGraphicsStateUpdateFlagCommonVertexSourceBindingDescriptorBit ) )
		{
			const auto & dx11VertexSourceBinding = GetDX11VertexSourceBinding();
			SetDX11VertexSourceBinding( dx11VertexSourceBinding );
			executedUpdatesMask.set( eGraphicsStateUpdateFlagCommonVertexSourceBindingDescriptorBit );
		}

		if( _stateUpdateMask.is_set( eGraphicsStateUpdateFlagCommonRenderTargetDescriptorBit ) )
		{
			const auto & dx11RenderTargetBinding = GetDX11RenderTargetBinding();
			SetDX11RenderTargetBinding( dx11RenderTargetBinding );
			executedUpdatesMask.set( eGraphicsStateUpdateFlagCommonRenderTargetDescriptorBit );
		}

		return !executedUpdatesMask.empty();
	}

	bool DX11GraphicsPipelineStateController::SetGraphicsPipelineStateObject( const GraphicsPipelineStateObject & pGraphicsPipelineStateObject )
	{
		return BaseStateControllerType::SetGraphicsPipelineStateObject( pGraphicsPipelineStateObject );
	}

	bool DX11GraphicsPipelineStateController::ResetGraphicsPipelineStateObject()
	{
		return BaseStateControllerType::ResetGraphicsPipelineStateObject();
	}

	bool DX11GraphicsPipelineStateController::SetRenderPassDescriptor( const RenderPassDescriptor & pRenderPassDescriptor )
	{
		return BaseStateControllerType::SetRenderPassDescriptor( pRenderPassDescriptor );
	}

	bool DX11GraphicsPipelineStateController::SetRenderPassDescriptorDynamic( RenderPassDescriptorDynamic & pRenderPassDescriptor )
	{
		return BaseStateControllerType::SetRenderPassDescriptorDynamic( pRenderPassDescriptor );
	}

	void DX11GraphicsPipelineStateController::ResetRenderPassDescriptor()
	{
		BaseStateControllerType::ResetRenderPassDescriptor();
	}

	bool DX11GraphicsPipelineStateController::SetRenderTargetDescriptor( const RenderTargetDescriptor & pRenderTargetDescriptor )
	{
		const bool updateResult = BaseStateControllerType::SetRenderTargetDescriptor( pRenderTargetDescriptor );

		if( updateResult )
		{
			const auto * dx11RenderTargetDescriptor = pRenderTargetDescriptor.QueryInterface<DX11RenderTargetDescriptor>();
			_dx11CurrentPipelineBindings.renderTargetBinding = &( dx11RenderTargetDescriptor->mDX11RenderTargetBinding );
		}

		return updateResult;
	}

	bool DX11GraphicsPipelineStateController::SetRenderTargetDescriptorDynamic( RenderTargetDescriptorDynamic & pRenderTargetDescriptor )
	{
		const bool updateResult = BaseStateControllerType::SetRenderTargetDescriptorDynamic( pRenderTargetDescriptor );

		if( updateResult )
		{
			if( !pRenderTargetDescriptor.IsDynamicDriverStateInitialized() )
			{
				InitializeDynamicDriverStateForDescriptor<DX11RenderTargetBinding>( pRenderTargetDescriptor );
			}
			auto * dx11RenderTargetDriverState = pRenderTargetDescriptor.GetDynamicDriverStateAs<DX11RenderTargetBinding>();

			auto & dx11RenderTargetBinding = dx11RenderTargetDriverState->mDriverData;
			if( &dx11RenderTargetBinding != _dx11CurrentPipelineBindings.renderTargetBinding )
			{
				if( IsDynamicDescriptorConfigurationModified( pRenderTargetDescriptor ) )
				{
					GCU::RTOUpdateRenderTargetBindingDX11(
						*( mGPUDevice.QueryInterface<DX11GPUDevice>() ),
						pRenderTargetDescriptor.GetRenderTargetArrayConfiguration(),
						dx11RenderTargetBinding );

					ResetDynamicDescriptorConfigurationModifiedState( pRenderTargetDescriptor );
				}

				_dx11CurrentPipelineBindings.renderTargetBinding = &dx11RenderTargetBinding;
			}
		}

		return updateResult;
	}

	bool DX11GraphicsPipelineStateController::ResetRenderTargetDescriptor()
	{
		const bool updateResult = BaseStateControllerType::ResetRenderTargetDescriptor();

		if( updateResult )
		{
			_dx11CurrentPipelineBindings.renderTargetBinding = nullptr;
		}

		return updateResult;
	}

	bool DX11GraphicsPipelineStateController::SetVertexSourceBindingDescriptor( const VertexSourceBindingDescriptor & pVertexSourceBindingDescriptor )
	{
		const bool updateResult = BaseStateControllerType::SetVertexSourceBindingDescriptor( pVertexSourceBindingDescriptor );

		if( updateResult )
		{
			const auto * dx11VertexSourceBindingDescriptor = pVertexSourceBindingDescriptor.QueryInterface<DX11VertexSourceBindingDescriptor>();
			_dx11CurrentPipelineBindings.vertexSourceBinding = &( dx11VertexSourceBindingDescriptor->mDX11VertexSourceBinding );
		}

		return updateResult;
	}

	bool DX11GraphicsPipelineStateController::SetVertexSourceBindingDescriptorDynamic( VertexSourceBindingDescriptorDynamic & pVertexSourceBindingDescriptor )
	{
		const bool updateResult = BaseStateControllerType::SetVertexSourceBindingDescriptorDynamic( pVertexSourceBindingDescriptor );

		if( updateResult )
		{
			if( !pVertexSourceBindingDescriptor.IsDynamicDriverStateInitialized() )
			{
				InitializeDynamicDriverStateForDescriptor<DX11IAVertexSourceBinding>( pVertexSourceBindingDescriptor );
			}
			auto * dx11VertexSourceBindingDriverState = pVertexSourceBindingDescriptor.GetDynamicDriverStateAs<DX11IAVertexSourceBinding>();


			auto & dx11VertexSourceBinding = dx11VertexSourceBindingDriverState->mDriverData;
			if( &dx11VertexSourceBinding != _dx11CurrentPipelineBindings.vertexSourceBinding )
			{
				if( IsDynamicDescriptorConfigurationModified( pVertexSourceBindingDescriptor ) )
				{
					GCU::DX11IAUpdateVertexSourceBindingDefinition(
						pVertexSourceBindingDescriptor.GetVertexSourceBindingDefinition(),
						dx11VertexSourceBinding );

					ResetDynamicDescriptorConfigurationModifiedState( pVertexSourceBindingDescriptor );
				}

				_dx11CurrentPipelineBindings.vertexSourceBinding = &dx11VertexSourceBinding;
			}

		}

		return updateResult;
	}

	bool DX11GraphicsPipelineStateController::ResetVertexSourceBindingDescriptor()
	{
		const bool updateResult = BaseStateControllerType::ResetVertexSourceBindingDescriptor();

		if( updateResult )
		{
			_dx11CurrentPipelineBindings.vertexSourceBinding = nullptr;
		}

		return updateResult;
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

	bool DX11GraphicsPipelineStateController::SetShaderConstantBuffer( shader_input_ref_id_t pParamRefID, GPUBuffer & pConstantBuffer )
	{
		bool baseResult = BaseStateControllerType::SetShaderConstantBuffer( pParamRefID, pConstantBuffer );

		auto updatedStagesMask = cppx::make_bitmask_tp<EShaderStageFlags>();

		if( baseResult )
		{
			const auto & rootSignatureDescriptor = GetCurrentPSORootSignatureDescriptor<DX11RootSignatureDescriptor>();
			Ic3DebugAssert( rootSignatureDescriptor.mRootSignature );

			const auto & descriptorInfo = rootSignatureDescriptor.mRootSignature.GetDescriptorInfo( pParamRefID );
			Ic3DebugAssert( descriptorInfo.dDescriptorType == EShaderInputDescriptorType::Resource );
			Ic3DebugAssert( descriptorInfo.uResourceInfo.resourceType == EShaderInputResourceType::CBVConstantBuffer );

			auto updatedStagesMask = cppx::make_bitmask_tp<EShaderStageFlags>();

			if( descriptorInfo.dShaderVisibilityMask != 0 )
			{
				auto * dx11Buffer = pConstantBuffer.QueryInterface<DX11GPUBuffer>();
				auto * d3d11Buffer = dx11Buffer->mD3D11Buffer.Get();

				if( descriptorInfo.dShaderVisibilityMask.is_set( eShaderStageFlagGraphicsVertexBit ) )
				{
					mD3D11DeviceContext1->VSSetConstantBuffers( descriptorInfo.uResourceInfo.resourceBaseRegisterIndex, 1, &d3d11Buffer );
					updatedStagesMask.set( eShaderStageFlagGraphicsVertexBit );
				}

				if( descriptorInfo.dShaderVisibilityMask.is_set( eShaderStageFlagGraphicsTessHullBit) )
				{
					mD3D11DeviceContext1->HSSetConstantBuffers( descriptorInfo.uResourceInfo.resourceBaseRegisterIndex, 1, &d3d11Buffer );
					updatedStagesMask.set( eShaderStageFlagGraphicsTessHullBit );
				}

				if( descriptorInfo.dShaderVisibilityMask.is_set( eShaderStageFlagGraphicsTessDomainBit ) )
				{
					mD3D11DeviceContext1->DSSetConstantBuffers( descriptorInfo.uResourceInfo.resourceBaseRegisterIndex, 1, &d3d11Buffer );
					updatedStagesMask.set( eShaderStageFlagGraphicsTessDomainBit );
				}

				if( descriptorInfo.dShaderVisibilityMask.is_set( eShaderStageFlagGraphicsGeometryBit ) )
				{
					mD3D11DeviceContext1->GSSetConstantBuffers( descriptorInfo.uResourceInfo.resourceBaseRegisterIndex, 1, &d3d11Buffer );
					updatedStagesMask.set( eShaderStageFlagGraphicsGeometryBit );
				}

				if( descriptorInfo.dShaderVisibilityMask.is_set( eShaderStageFlagGraphicsPixelBit ) )
				{
					mD3D11DeviceContext1->PSSetConstantBuffers( descriptorInfo.uResourceInfo.resourceBaseRegisterIndex, 1, &d3d11Buffer );
					updatedStagesMask.set( eShaderStageFlagGraphicsPixelBit );
				}
			}
		}

		return !updatedStagesMask.empty();
	}

	bool DX11GraphicsPipelineStateController::SetShaderTextureImage( shader_input_ref_id_t pParamRefID, Texture & pTexture )
	{
		bool baseResult = BaseStateControllerType::SetShaderTextureImage( pParamRefID, pTexture );

		auto updatedStagesMask = cppx::make_bitmask_tp<EShaderStageFlags>();

		if( baseResult )
		{
			const auto & rootSignatureDescriptor = GetCurrentPSORootSignatureDescriptor<DX11RootSignatureDescriptor>();
			Ic3DebugAssert( rootSignatureDescriptor.mRootSignature );

			const auto & descriptorInfo = rootSignatureDescriptor.mRootSignature.GetDescriptorInfo( pParamRefID );
			Ic3DebugAssert( descriptorInfo.dDescriptorType == EShaderInputDescriptorType::Resource );
			Ic3DebugAssert( descriptorInfo.uResourceInfo.resourceType == EShaderInputResourceType::SRVTextureImage );

			auto updatedStagesMask = cppx::make_bitmask_tp<EShaderStageFlags>();

			if( descriptorInfo.dShaderVisibilityMask != 0 )
			{
				auto * dx11Texture = pTexture.QueryInterface<DX11Texture>();
				auto * d3d11TextureSRV = dx11Texture->mD3D11DefaultSRV.Get();

				if( descriptorInfo.dShaderVisibilityMask.is_set( eShaderStageFlagGraphicsVertexBit ) )
				{
					mD3D11DeviceContext1->VSSetShaderResources( descriptorInfo.uResourceInfo.resourceBaseRegisterIndex, 1, &d3d11TextureSRV );
					updatedStagesMask.set( eShaderStageFlagGraphicsVertexBit );
				}

				if( descriptorInfo.dShaderVisibilityMask.is_set( eShaderStageFlagGraphicsTessHullBit ) )
				{
					mD3D11DeviceContext1->HSSetShaderResources( descriptorInfo.uResourceInfo.resourceBaseRegisterIndex, 1, &d3d11TextureSRV );
					updatedStagesMask.set( eShaderStageFlagGraphicsTessHullBit );
				}

				if( descriptorInfo.dShaderVisibilityMask.is_set( eShaderStageFlagGraphicsTessDomainBit ) )
				{
					mD3D11DeviceContext1->DSSetShaderResources( descriptorInfo.uResourceInfo.resourceBaseRegisterIndex, 1, &d3d11TextureSRV );
					updatedStagesMask.set( eShaderStageFlagGraphicsTessDomainBit );
				}

				if( descriptorInfo.dShaderVisibilityMask.is_set( eShaderStageFlagGraphicsGeometryBit ) )
				{
					mD3D11DeviceContext1->GSSetShaderResources( descriptorInfo.uResourceInfo.resourceBaseRegisterIndex, 1, &d3d11TextureSRV );
					updatedStagesMask.set( eShaderStageFlagGraphicsGeometryBit );
				}

				if( descriptorInfo.dShaderVisibilityMask.is_set( eShaderStageFlagGraphicsPixelBit ) )
				{
					mD3D11DeviceContext1->PSSetShaderResources( descriptorInfo.uResourceInfo.resourceBaseRegisterIndex, 1, &d3d11TextureSRV );
					updatedStagesMask.set( eShaderStageFlagGraphicsPixelBit );
				}
			}
		}

		return !updatedStagesMask.empty();
	}

	bool DX11GraphicsPipelineStateController::SetShaderTextureSampler( shader_input_ref_id_t pParamRefID, Sampler & pSampler )
	{
		bool baseResult = BaseStateControllerType::SetShaderTextureSampler( pParamRefID, pSampler );

		auto updatedStagesMask = cppx::make_bitmask_tp<EShaderStageFlags>();

		if( baseResult )
		{
			const auto & rootSignatureDescriptor = GetCurrentPSORootSignatureDescriptor<DX11RootSignatureDescriptor>();
			Ic3DebugAssert( rootSignatureDescriptor.mRootSignature );

			const auto & descriptorInfo = rootSignatureDescriptor.mRootSignature.GetDescriptorInfo( pParamRefID );
			Ic3DebugAssert( descriptorInfo.dDescriptorType == EShaderInputDescriptorType::Resource );
			Ic3DebugAssert( descriptorInfo.uResourceInfo.resourceType == EShaderInputResourceType::SRVTextureImage );

			if( descriptorInfo.dShaderVisibilityMask != 0 )
			{
				auto * dx11Sampler = pSampler.QueryInterface<DX11Sampler>();
				auto * d3d11SamplerState = dx11Sampler->mD3D11SamplerState.Get();

				if( descriptorInfo.dShaderVisibilityMask.is_set( eShaderStageFlagGraphicsVertexBit ) )
				{
					mD3D11DeviceContext1->VSSetSamplers( descriptorInfo.uSamplerInfo.samplerBindingIndex, 1, &d3d11SamplerState );
					updatedStagesMask.set( eShaderStageFlagGraphicsVertexBit );
				}

				if( descriptorInfo.dShaderVisibilityMask.is_set( eShaderStageFlagGraphicsTessHullBit ) )
				{
					mD3D11DeviceContext1->HSSetSamplers( descriptorInfo.uSamplerInfo.samplerBindingIndex, 1, &d3d11SamplerState );
					updatedStagesMask.set( eShaderStageFlagGraphicsTessHullBit );
				}

				if( descriptorInfo.dShaderVisibilityMask.is_set( eShaderStageFlagGraphicsTessDomainBit ) )
				{
					mD3D11DeviceContext1->DSSetSamplers( descriptorInfo.uSamplerInfo.samplerBindingIndex, 1, &d3d11SamplerState );
					updatedStagesMask.set( eShaderStageFlagGraphicsTessDomainBit );
				}

				if( descriptorInfo.dShaderVisibilityMask.is_set( eShaderStageFlagGraphicsGeometryBit ) )
				{
					mD3D11DeviceContext1->GSSetSamplers( descriptorInfo.uSamplerInfo.samplerBindingIndex, 1, &d3d11SamplerState );
					updatedStagesMask.set( eShaderStageFlagGraphicsGeometryBit );
				}

				if( descriptorInfo.dShaderVisibilityMask.is_set( eShaderStageFlagGraphicsPixelBit ) )
				{
					mD3D11DeviceContext1->PSSetSamplers( descriptorInfo.uSamplerInfo.samplerBindingIndex, 1, &d3d11SamplerState );
					updatedStagesMask.set( eShaderStageFlagGraphicsPixelBit );
				}
			}
		}

		return !updatedStagesMask.empty();
	}

	const DX11RenderTargetBinding & DX11GraphicsPipelineStateController::GetDX11RenderTargetBinding() const noexcept
	{
		return *( _dx11CurrentPipelineBindings.renderTargetBinding );
	}

	const DX11IAVertexSourceBinding & DX11GraphicsPipelineStateController::GetDX11VertexSourceBinding() const noexcept
	{
		return *( _dx11CurrentPipelineBindings.vertexSourceBinding );
	}

	cppx::bitmask<uint32> DX11GraphicsPipelineStateController::BindCommonConfigDescriptors(
			const DX11GraphicsPipelineStateObject & pDX11GraphicsPipelineStateObject )
	{
		cppx::bitmask<uint32> executedUpdatesMask = 0;

		if( _stateUpdateMask.is_set( eGraphicsStateUpdateFlagSeparableStateBlendBit ) )
		{
			const auto & dx11BlendStateDescriptor = pDX11GraphicsPipelineStateObject.GetDX11BlendStateDescriptor();
			const auto & dynamicPipelineConfig = GetPipelineDynamicConfig();
			const auto * blendConstantFactor = SelectBlendConstantFactor( dx11BlendStateDescriptor, dynamicPipelineConfig );

			mDX11CommandList->mD3D11DeviceContext1->OMSetBlendState(
					dx11BlendStateDescriptor.mD3D11BlendState.Get(),
					blendConstantFactor->uv_rgba,
					0xFFFFFFFF );

			executedUpdatesMask.set( eGraphicsStateUpdateFlagSeparableStateBlendBit );
		}

		if( _stateUpdateMask.is_set( eGraphicsStateUpdateFlagSeparableStateDepthStencilBit ) )
		{
			const auto & dx11DepthStencilStateDescriptor = pDX11GraphicsPipelineStateObject.GetDX11DepthStencilStateDescriptor();
			const auto & dynamicPipelineConfig = GetPipelineDynamicConfig();
			const auto stencilTestRefValue = SelectStencilTestRefValue( dx11DepthStencilStateDescriptor, dynamicPipelineConfig );

			mDX11CommandList->mD3D11DeviceContext1->OMSetDepthStencilState(
					dx11DepthStencilStateDescriptor.mD3D11DepthStencilState.Get(),
					stencilTestRefValue );

			executedUpdatesMask.set( eGraphicsStateUpdateFlagSeparableStateDepthStencilBit );
		}

		if( _stateUpdateMask.is_set( eGraphicsStateUpdateFlagSeparableStateRasterizerBit ) )
		{
			const auto & dx11RasterizerStateDescriptor = pDX11GraphicsPipelineStateObject.GetDX11RasterizerStateDescriptor();

			mDX11CommandList->mD3D11DeviceContext1->RSSetState(
					dx11RasterizerStateDescriptor.mD3D11RasterizerState.Get() );

			executedUpdatesMask.set( eGraphicsStateUpdateFlagSeparableStateRasterizerBit );
		}

		if( _stateUpdateMask.is_set( eGraphicsStateUpdateFlagSeparableStateVertexAttributeLayoutBit ) )
		{
			const auto & dx11VertexAttributeLayoutDescriptor = pDX11GraphicsPipelineStateObject.GetDX11VertexAttributeLayoutDescriptor();

			mDX11CommandList->mD3D11DeviceContext1->IASetPrimitiveTopology( dx11VertexAttributeLayoutDescriptor.mD3D11PrimitiveTopology );
			mDX11CommandList->mD3D11DeviceContext1->IASetInputLayout( dx11VertexAttributeLayoutDescriptor.mD3D11InputLayout.Get() );

			executedUpdatesMask.set( eGraphicsStateUpdateFlagSeparableStateVertexAttributeLayoutBit );
		}

		return executedUpdatesMask;
	}

	cppx::bitmask<uint32> DX11GraphicsPipelineStateController::BindSeparableGraphicsShaders( const SeparableShaderCache & pSeparableShaders )
	{
		cppx::bitmask<uint32> executedUpdatesMask = 0;

		if( _stateUpdateMask.is_set( eGraphicsStateUpdateFlagSeparableShaderVertexBit ) )
		{
			const auto * dx11VertxShader = pSeparableShaders.vertexShader->QueryInterface<DX11Shader>();
			mDX11CommandList->mD3D11DeviceContext1->VSSetShader( dx11VertxShader->mD3D11VertexShader, nullptr, 0 );
			executedUpdatesMask.set( eGraphicsStateUpdateFlagSeparableShaderVertexBit );
		}

		if( _stateUpdateMask.is_set( eGraphicsStateUpdateFlagSeparableShaderHullBit ) )
		{
			const auto * dx11TessHullShader = pSeparableShaders.tessHullShader->QueryInterface<DX11Shader>();
			mDX11CommandList->mD3D11DeviceContext1->HSSetShader( dx11TessHullShader->mD3D11HullShader, nullptr, 0 );
			executedUpdatesMask.set( eGraphicsStateUpdateFlagSeparableShaderHullBit );
		}

		if( _stateUpdateMask.is_set( eGraphicsStateUpdateFlagSeparableShaderDomainBit ) )
		{
			const auto * dx11TessDomainShader = pSeparableShaders.tessDomainShader->QueryInterface<DX11Shader>();
			mDX11CommandList->mD3D11DeviceContext1->DSSetShader( dx11TessDomainShader->mD3D11DomainShader, nullptr, 0 );
			executedUpdatesMask.set( eGraphicsStateUpdateFlagSeparableShaderDomainBit );
		}

		if( _stateUpdateMask.is_set( eGraphicsStateUpdateFlagSeparableShaderGeometryBit ) )
		{
			const auto * dx11GeomtryShader = pSeparableShaders.geometryShader->QueryInterface<DX11Shader>();
			mDX11CommandList->mD3D11DeviceContext1->GSSetShader( dx11GeomtryShader->mD3D11GeometryShader, nullptr, 0 );
			executedUpdatesMask.set( eGraphicsStateUpdateFlagSeparableShaderGeometryBit );
		}

		if( _stateUpdateMask.is_set( eGraphicsStateUpdateFlagSeparableShaderPixelBit ) )
		{
			const auto * dx11PixelShader = pSeparableShaders.pixelShader->QueryInterface<DX11Shader>();
			mDX11CommandList->mD3D11DeviceContext1->PSSetShader( dx11PixelShader->mD3D11PixelShader, nullptr, 0 );
			executedUpdatesMask.set( eGraphicsStateUpdateFlagSeparableShaderPixelBit );
		}

		return executedUpdatesMask;
	}

	void DX11GraphicsPipelineStateController::SetDX11RenderTargetBinding( const DX11RenderTargetBinding & pDX11RenderTargetBinding )
	{
		ID3D11RenderTargetView * const * d3d11RenderTargetViewArrayPtr = nullptr;
		ID3D11DepthStencilView * d3d11DepthStencilView = nullptr;

		UINT renderTargetColorAttachmentsNum = 0;

		if( !pDX11RenderTargetBinding.IsEmpty() )
		{
			d3d11RenderTargetViewArrayPtr = &( pDX11RenderTargetBinding.d3d11ColorAttachmentRTViewArray[0] );
			renderTargetColorAttachmentsNum = pDX11RenderTargetBinding.GetActiveColorAttachmentsNum();
		}
		
		if( pDX11RenderTargetBinding.IsDepthStencilAttachmentActive() )
		{
			d3d11DepthStencilView = pDX11RenderTargetBinding.d3d11DepthStencilAttachmentDSView;
		}

		mDX11CommandList->mD3D11DeviceContext1->OMSetRenderTargets(
				renderTargetColorAttachmentsNum,
				d3d11RenderTargetViewArrayPtr,
				d3d11DepthStencilView );
	}

	void DX11GraphicsPipelineStateController::SetDX11VertexSourceBinding( const DX11IAVertexSourceBinding & pDX11VertexSourceBinding )
	{
		const auto & vertexBufferArrayBindings = pDX11VertexSourceBinding.vertexBufferBindings;
		const auto & indexBufferBinding = pDX11VertexSourceBinding.indexBufferBinding;

		for( const auto & activeVertexStreamRange : vertexBufferArrayBindings.activeRanges )
		{
			mDX11CommandList->mD3D11DeviceContext1->IASetVertexBuffers(
				activeVertexStreamRange.firstIndex,
				activeVertexStreamRange.length,
				&( vertexBufferArrayBindings.bindingData.bufferArray[activeVertexStreamRange.firstIndex] ),
				&( vertexBufferArrayBindings.bindingData.strideArray[activeVertexStreamRange.firstIndex] ),
				&( vertexBufferArrayBindings.bindingData.offsetArray[activeVertexStreamRange.firstIndex] ) );
		}

		mDX11CommandList->mD3D11DeviceContext1->IASetIndexBuffer(
			indexBufferBinding.buffer,
			indexBufferBinding.format,
			indexBufferBinding.offset );
	}

	const cxm::rgba_color_r32_norm * DX11GraphicsPipelineStateController::SelectBlendConstantFactor(
		const DX11BlendStateDescriptor & pDX11BlendStateDescriptor,
		const GraphicsPipelineDynamicConfig & pDynamicConfig ) noexcept
	{
		if( pDynamicConfig.activeStateMask.is_set( eGraphicsPipelineDynamicConfigFlagBlendConstantColorBit ) )
		{
			return &( pDynamicConfig.blendConstantColor );
		}
		else if( pDX11BlendStateDescriptor.mBlendFlags.is_set( eBlendConfigFlagSetFixedBlendConstantsBit ) )
		{
			return &( pDX11BlendStateDescriptor.mConstantFactor );
		}

		return nullptr;
	}

	uint8 DX11GraphicsPipelineStateController::SelectStencilTestRefValue(
		const DX11DepthStencilStateDescriptor & pDX11DepthStencilStateDescriptor,
		const GraphicsPipelineDynamicConfig & pDynamicConfig ) noexcept
	{
		if( pDynamicConfig.activeStateMask.is_set( eGraphicsPipelineDynamicConfigFlagStencilRefValueBit ) )
		{
			return pDynamicConfig.stencilTestRefValue;
		}

		return 0x00;
	}

} // namespace Ic3::Graphics::GCI
