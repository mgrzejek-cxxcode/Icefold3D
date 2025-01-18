
#include "CommandSystem.h"
#include "GPUDeviceNull.h"
#include "GPUDriver.h"
#include "PresentationLayer.h"
#include "Resources/Texture.h"
#include "Resources/RenderTargetTexture.h"
#include "State/GraphicsShaderLinkageImmutableState.h"
#include "State/PipelineStateObject.h"
#include "State/SharedImmutableStateCache.h"

namespace Ic3::Graphics::GCI
{
	
	static const Math::RGBAColorU8 sDefaultClearColorDriver0     { 0x11, 0x66, 0xCC, 0xFF };
	static const Math::RGBAColorU8 sDefaultClearColorDriverDX11  { 0x77, 0xAA, 0x5F, 0xFF };
	static const Math::RGBAColorU8 sDefaultClearColorDriverDX12  { 0x22, 0x88, 0x3F, 0xFF };
	static const Math::RGBAColorU8 sDefaultClearColorDriverMTL2  { 0xFF, 0x99, 0x66, 0xFF };
	static const Math::RGBAColorU8 sDefaultClearColorDriverGL4   { 0x55, 0x88, 0xAA, 0xFF };
	static const Math::RGBAColorU8 sDefaultClearColorDriverGLES3 { 0x7A, 0x00, 0x4D, 0xFF };
	static const Math::RGBAColorU8 sDefaultClearColorDriverVK1   { 0x8F, 0x0F, 0x1F, 0xFF };

	enum EGPUDeviceInternalStateFlags : uint32
	{
		E_GPU_DEVICE_INTERNAL_STATE_FLAG_DEBUG_DEVICE_BIT = 0x0001,
		E_GPU_DEVICE_INTERNAL_STATE_FLAG_MULTI_THREAD_ACCESS_BIT = 0x0002,
		E_GPU_DEVICE_INTERNAL_STATE_FLAG_ENABLE_RESOURCE_ACTIVE_REFS_TRACKING_BIT = 0x0008
	};

	
	GPUDevice::GPUDevice( GPUDriver & pDriver )
	: GPUDriverChildObject( pDriver )
	, mGPUDriverID( pDriver.QueryGPUDriverID() )
	, mSysContext( pDriver.mSysContext )
	{
		if( pDriver.IsDebugFunctionalityRequested() )
		{
			_internalStateFlags.set( E_GPU_DEVICE_INTERNAL_STATE_FLAG_DEBUG_DEVICE_BIT );
		}
	}

	GPUDevice::~GPUDevice() = default;

	bool GPUDevice::IsNullDevice() const noexcept
	{
		return false;
	}

	bool GPUDevice::IsDebugDevice() const noexcept
	{
		return _internalStateFlags.is_set( E_GPU_DEVICE_INTERNAL_STATE_FLAG_DEBUG_DEVICE_BIT );
	}

	bool GPUDevice::IsMultiThreadAccessSupported() const noexcept
	{
		return _internalStateFlags.is_set( E_GPU_DEVICE_INTERNAL_STATE_FLAG_MULTI_THREAD_ACCESS_BIT );
	}

	bool GPUDevice::IsResourceActiveRefsTrackingEnabled() const noexcept
	{
		return _internalStateFlags.is_set( E_GPU_DEVICE_INTERNAL_STATE_FLAG_ENABLE_RESOURCE_ACTIVE_REFS_TRACKING_BIT );
	}

	CommandSystem & GPUDevice::GetCommandSystem() const noexcept
	{
		return *_commandSystem;
	}

	PipelineImmutableStateFactory & GPUDevice::GetPipelineStateFactory() const noexcept
	{
		return *_immutableStateFactoryBase;
	}

	PresentationLayer * GPUDevice::GetPresentationLayer() const noexcept
	{
		return _presentationLayer.get();
	}

	const Math::RGBAColorU8 & GPUDevice::GetDefaultClearColor() const noexcept
	{
		switch( mGPUDriverID )
		{
			case EGPUDriverID::GDIDirectX11:
				return sDefaultClearColorDriverDX11;

			case EGPUDriverID::GDIDirectX12:
				return sDefaultClearColorDriverDX12;

			case EGPUDriverID::GDIMetal1:
				return sDefaultClearColorDriverMTL2;

			case EGPUDriverID::GDIOpenGLDesktop4:
				return sDefaultClearColorDriverGL4;

			case EGPUDriverID::GDIOpenGLES3:
				return sDefaultClearColorDriverGLES3;

			case EGPUDriverID::GDIVulkan10:
				return sDefaultClearColorDriverVK1;

			default:
				break;
		}
		return sDefaultClearColorDriver0;
	}

	const RenderTargetAttachmentClearConfig & GPUDevice::GetDefaultClearConfig() const noexcept
	{
		static const RenderTargetAttachmentClearConfig sDefaultClearConfig =
		{
			GetDefaultClearColor(),
			1.0f,
			0
		};
		return sDefaultClearConfig;
	}

	GPUBufferHandle GPUDevice::CreateGPUBuffer( const GPUBufferCreateInfo & pCreateInfo )
	{
	    return _DrvCreateGPUBuffer( pCreateInfo );
	}

	SamplerHandle GPUDevice::CreateSampler( const SamplerCreateInfo & pCreateInfo )
	{
	    return _DrvCreateSampler( pCreateInfo );
	}

	ShaderHandle GPUDevice::CreateShader( const ShaderCreateInfo & pCreateInfo )
	{
	    return _DrvCreateShader( pCreateInfo );
	}

	TextureHandle GPUDevice::CreateTexture( const TextureCreateInfo & pCreateInfo )
	{
	    return _DrvCreateTexture( pCreateInfo );
	}

	RenderTargetTextureHandle GPUDevice::CreateRenderTargetTexture( const RenderTargetTextureCreateInfo & pCreateInfo )
	{
		if( !pCreateInfo.bindFlags.is_set_any_of(
				eGPUResourceUsageFlagRenderTargetColorBit | eGPUResourceUsageMaskRenderTargetDepthStencil ) )
		{
			Ic3DebugOutput( "No RT attachment bind flags specified for the RT texture (E_GPU_RESOURCE_USAGE_xxx_RENDER_TARGET_yyy)." );
			return nullptr;
		}

		if( pCreateInfo.targetTexture )
		{
			const auto & targetTextureResourceFlags = pCreateInfo.targetTexture->mTextureProperties.resourceFlags;
			if( !targetTextureResourceFlags.is_set( pCreateInfo.bindFlags & eGPUResourceUsageMaskAll ) )
			{
				Ic3DebugOutput( "Target texture for Render Target is not compatible with specified bind flags (E_GPU_RESOURCE_USAGE_xxx)." );
				return nullptr;
			}
		}

		return _DrvCreateRenderTargetTexture( pCreateInfo );
	}

	GraphicsPipelineStateObjectHandle GPUDevice::CreateGraphicsPipelineStateObject( const GraphicsPipelineStateObjectCreateInfo & pCreateInfo )
	{
		if( pCreateInfo.renderTargetLayout.IsEmpty() )
		{
			return nullptr;
		}

		if( !pCreateInfo.shaderInputSignature )
		{
			pCreateInfo.shaderInputSignature = SMU::CreateShaderInputSignature( pCreateInfo.shaderInputSignatureDesc );
		}

		if( !pCreateInfo.blendState )
		{
			pCreateInfo.blendState = _immutableStateFactoryBase->CreateBlendState( pCreateInfo.blendConfig );
		}

		if( !pCreateInfo.depthStencilState )
		{
			pCreateInfo.depthStencilState = _immutableStateFactoryBase->CreateDepthStencilState( pCreateInfo.depthStencilConfig );
		}

		if( !pCreateInfo.rasterizerState )
		{
			pCreateInfo.rasterizerState = _immutableStateFactoryBase->CreateRasterizerState( pCreateInfo.rasterizerConfig );
		}

		if( !pCreateInfo.shaderLinkageState )
		{
			pCreateInfo.shaderLinkageState = _immutableStateFactoryBase->CreateGraphicsShaderLinkageState( pCreateInfo.shaderSet );
		}

		if( !pCreateInfo.inputLayoutState )
		{
			auto * vertexShader = pCreateInfo.shaderLinkageState->GetShader( EShaderType::GSVertex );
			pCreateInfo.inputLayoutState = _immutableStateFactoryBase->CreateIAInputLayoutState( pCreateInfo.inputLayoutDefinition, *vertexShader );
		}

		return _DrvCreateGraphicsPipelineStateObject( pCreateInfo );
	}

	BlendImmutableStateHandle GPUDevice::CreateBlendImmutableState( const BlendConfig & pConfig )
	{
		Ic3DebugAssert( _immutableStateFactoryBase );
		return _immutableStateFactoryBase->CreateBlendState( pConfig );
	}

	DepthStencilImmutableStateHandle GPUDevice::CreateDepthStencilImmutableState( const DepthStencilConfig & pConfig )
	{
		Ic3DebugAssert( _immutableStateFactoryBase );
		return _immutableStateFactoryBase->CreateDepthStencilState( pConfig );
	}

	GraphicsShaderLinkageImmutableStateHandle GPUDevice::CreateGraphicsShaderLinkageImmutableState( const GraphicsShaderSet & pShaderSet )
	{
		Ic3DebugAssert( _immutableStateFactoryBase );
		return _immutableStateFactoryBase->CreateGraphicsShaderLinkageState( pShaderSet );
	}

	IAInputLayoutImmutableStateHandle GPUDevice::CreateIAInputLayoutImmutableState( const IAInputLayoutDefinition & pDefinition, Shader & pVertexShaderWithBinary )
	{
		Ic3DebugAssert( _immutableStateFactoryBase );
		return _immutableStateFactoryBase->CreateIAInputLayoutState( pDefinition, pVertexShaderWithBinary );
	}

	IAVertexStreamImmutableStateHandle GPUDevice::CreateIAVertexStreamImmutableState( const IAVertexStreamDefinition & pDefinition )
	{
		Ic3DebugAssert( _immutableStateFactoryBase );
		return _immutableStateFactoryBase->CreateIAVertexStreamState( pDefinition );
	}

	RasterizerImmutableStateHandle GPUDevice::CreateRasterizerImmutableState( const RasterizerConfig & pConfig )
	{
		Ic3DebugAssert( _immutableStateFactoryBase );
		return _immutableStateFactoryBase->CreateRasterizerState( pConfig );
	}

	RenderTargetBindingImmutableStateHandle GPUDevice::CreateRenderTargetBindingImmutableState( const RenderTargetBindingDefinition & pDefinition )
	{
		Ic3DebugAssert( _immutableStateFactoryBase );
		return _immutableStateFactoryBase->CreateRenderTargetBindingState( pDefinition );
	}

	RenderPassConfigurationImmutableStateHandle GPUDevice::CreateRenderPassConfigurationImmutableState( const RenderPassConfiguration & pConfiguration )
	{
		Ic3DebugAssert( _immutableStateFactoryBase );
		return _immutableStateFactoryBase->CreateRenderPassState( pConfiguration.GetValidated() );
	}

	void GPUDevice::ResetImmutableStateCache( cppx::bitmask<EPipelineImmutableStateTypeFlags> pResetMask )
	{
		Ic3DebugAssert( _immutableStateCachePtr );
		_immutableStateCachePtr->Reset( pResetMask );
	}

	void GPUDevice::SetPresentationLayer( PresentationLayerHandle pPresentationLayer )
	{
		if( !_DrvOnSetPresentationLayer( pPresentationLayer ) )
		{
			return;
		}
		_presentationLayer = pPresentationLayer;
	}

	GPUDevice & GPUDevice::GetNullDevice()
	{
		static const GPUDeviceHandle sNullDeviceInstance = CreateGfxObject<GPUDeviceNull>( GPUDriver::GetNullDriver() );
		return *sNullDeviceInstance;
	}

	bool GPUDevice::OnGPUResourceActiveRefsZero( GPUResource & pGPUResource )
	{
		return true;
	}

	void GPUDevice::SetImmutableStateCache( SharedImmutableStateCache & pStateCache )
	{
		_immutableStateCachePtr = &pStateCache;
		_immutableStateFactoryBase = &( pStateCache.mStateFactory );
	}

	bool GPUDevice::_DrvOnSetPresentationLayer( PresentationLayerHandle pPresentationLayer )
	{
		return true;
	}

	GPUBufferHandle GPUDevice::_DrvCreateGPUBuffer( const GPUBufferCreateInfo & pCreateInfo )
	{
		return nullptr;
	}

	SamplerHandle GPUDevice::_DrvCreateSampler( const SamplerCreateInfo & pCreateInfo )
	{
		return nullptr;
	}

	ShaderHandle GPUDevice::_DrvCreateShader( const ShaderCreateInfo & pCreateInfo )
	{
		return nullptr;
	}

	TextureHandle GPUDevice::_DrvCreateTexture( const TextureCreateInfo & pCreateInfo )
	{
		return nullptr;
	}

	RenderTargetTextureHandle GPUDevice::_DrvCreateRenderTargetTexture( const RenderTargetTextureCreateInfo & pCreateInfo )
	{
		return nullptr;
	}

	GraphicsPipelineStateObjectHandle GPUDevice::_DrvCreateGraphicsPipelineStateObject( const GraphicsPipelineStateObjectCreateInfo & pCreateInfo )
	{
		return nullptr;
	}

} // namespace Ic3::Graphics::GCI
