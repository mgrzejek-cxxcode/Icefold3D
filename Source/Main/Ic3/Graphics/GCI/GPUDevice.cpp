
#include "CommandSystem.h"
#include "GpuDeviceNull.h"
#include "GpuDriver.h"
#include "PresentationLayer.h"
#include "Resources/Texture.h"
#include "Resources/RenderTargetTexture.h"
#include "State/GraphicsShaderLinkageImmutableState.h"
#include "State/PipelineStateObject.h"
#include "State/PipelineImmutableStateCache.h"

namespace Ic3::Graphics::GCI
{
	
	static const Math::RGBAColorU8 sDefaultClearColorDriver0     { 0x11, 0x66, 0xCC, 0xFF };
	static const Math::RGBAColorU8 sDefaultClearColorDriverDX11  { 0x77, 0xAA, 0x5F, 0xFF };
	static const Math::RGBAColorU8 sDefaultClearColorDriverDX12  { 0x22, 0x88, 0x3F, 0xFF };
	static const Math::RGBAColorU8 sDefaultClearColorDriverMTL2  { 0xFF, 0x99, 0x66, 0xFF };
	static const Math::RGBAColorU8 sDefaultClearColorDriverGL4   { 0x55, 0x88, 0xAA, 0xFF };
	static const Math::RGBAColorU8 sDefaultClearColorDriverGLES3 { 0x7A, 0x00, 0x4D, 0xFF };
	static const Math::RGBAColorU8 sDefaultClearColorDriverVK1   { 0x8F, 0x0F, 0x1F, 0xFF };

	enum EGpuDeviceInternalStateFlags : uint32
	{
		E_GPU_DEVICE_INTERNAL_STATE_FLAG_DEBUG_DEVICE_BIT = 0x0001,
		E_GPU_DEVICE_INTERNAL_STATE_FLAG_MULTI_THREAD_ACCESS_BIT = 0x0002,
		E_GPU_DEVICE_INTERNAL_STATE_FLAG_ENABLE_RESOURCE_ACTIVE_REFS_TRACKING_BIT = 0x0008
	};

	
	GpuDevice::GpuDevice( GpuDriver & pDriver )
	: GpuDriverChildObject( pDriver )
	, mGpuDriverID( pDriver.QueryGpuDriverID() )
	, mSysContext( pDriver.mSysContext )
	{
		if( pDriver.IsDebugFunctionalityRequested() )
		{
			_internalStateFlags.set( E_GPU_DEVICE_INTERNAL_STATE_FLAG_DEBUG_DEVICE_BIT );
		}
	}

	GpuDevice::~GpuDevice() = default;

	bool GpuDevice::IsNullDevice() const noexcept
	{
		return false;
	}

	bool GpuDevice::IsDebugDevice() const noexcept
	{
		return _internalStateFlags.is_set( E_GPU_DEVICE_INTERNAL_STATE_FLAG_DEBUG_DEVICE_BIT );
	}

	bool GpuDevice::IsMultiThreadAccessSupported() const noexcept
	{
		return _internalStateFlags.is_set( E_GPU_DEVICE_INTERNAL_STATE_FLAG_MULTI_THREAD_ACCESS_BIT );
	}

	bool GpuDevice::IsResourceActiveRefsTrackingEnabled() const noexcept
	{
		return _internalStateFlags.is_set( E_GPU_DEVICE_INTERNAL_STATE_FLAG_ENABLE_RESOURCE_ACTIVE_REFS_TRACKING_BIT );
	}

	CommandSystem & GpuDevice::GetCommandSystem() const noexcept
	{
		return *_commandSystem;
	}

	PipelineImmutableStateFactory & GpuDevice::GetPipelineStateFactory() const noexcept
	{
		return *_immutableStateFactoryBase;
	}

	PresentationLayer * GpuDevice::GetPresentationLayer() const noexcept
	{
		return _presentationLayer.get();
	}

	const Math::RGBAColorU8 & GpuDevice::GetDefaultClearColor() const noexcept
	{
		switch( mGpuDriverID )
		{
			case EGpuDriverID::GDIDirectX11:
				return sDefaultClearColorDriverDX11;

			case EGpuDriverID::GDIDirectX12:
				return sDefaultClearColorDriverDX12;

			case EGpuDriverID::GDIMetal1:
				return sDefaultClearColorDriverMTL2;

			case EGpuDriverID::GDIOpenGLDesktop4:
				return sDefaultClearColorDriverGL4;

			case EGpuDriverID::GDIOpenGLES3:
				return sDefaultClearColorDriverGLES3;

			case EGpuDriverID::GDIVulkan10:
				return sDefaultClearColorDriverVK1;

			default:
				break;
		}
		return sDefaultClearColorDriver0;
	}

	const RenderTargetAttachmentClearConfig & GpuDevice::GetDefaultClearConfig() const noexcept
	{
		static const RenderTargetAttachmentClearConfig sDefaultClearConfig =
		{
			GetDefaultClearColor(),
			1.0f,
			0
		};
		return sDefaultClearConfig;
	}

	GpuBufferHandle GpuDevice::CreateGpuBuffer( const GpuBufferCreateInfo & pCreateInfo )
	{
	    return _DrvCreateGpuBuffer( pCreateInfo );
	}

	SamplerHandle GpuDevice::CreateSampler( const SamplerCreateInfo & pCreateInfo )
	{
	    return _DrvCreateSampler( pCreateInfo );
	}

	ShaderHandle GpuDevice::CreateShader( const ShaderCreateInfo & pCreateInfo )
	{
	    return _DrvCreateShader( pCreateInfo );
	}

	TextureHandle GpuDevice::CreateTexture( const TextureCreateInfo & pCreateInfo )
	{
	    return _DrvCreateTexture( pCreateInfo );
	}

	RenderTargetTextureHandle GpuDevice::CreateRenderTargetTexture( const RenderTargetTextureCreateInfo & pCreateInfo )
	{
		if( !pCreateInfo.bindFlags.is_set_any_of(
				eGpuResourceUsageFlagRenderTargetColorBit | eGpuResourceUsageMaskRenderTargetDepthStencil ) )
		{
			ic3DebugOutput( "No RT attachment bind flags specified for the RT texture (E_GPU_RESOURCE_USAGE_xxx_RENDER_TARGET_yyy)." );
			return nullptr;
		}

		if( pCreateInfo.targetTexture )
		{
			const auto & targetTextureResourceFlags = pCreateInfo.targetTexture->mTextureProperties.resourceFlags;
			if( !targetTextureResourceFlags.is_set( pCreateInfo.bindFlags & eGpuResourceUsageMaskAll ) )
			{
				ic3DebugOutput( "Target texture for Render Target is not compatible with specified bind flags (E_GPU_RESOURCE_USAGE_xxx)." );
				return nullptr;
			}
		}

		return _DrvCreateRenderTargetTexture( pCreateInfo );
	}

	GraphicsPipelineStateObjectHandle GpuDevice::CreateGraphicsPipelineStateObject( const GraphicsPipelineStateObjectCreateInfo & pCreateInfo )
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

	BlendImmutableStateHandle GpuDevice::CreateBlendImmutableState( const BlendConfig & pConfig )
	{
		ic3DebugAssert( _immutableStateFactoryBase );
		return _immutableStateFactoryBase->CreateBlendState( pConfig );
	}

	DepthStencilImmutableStateHandle GpuDevice::CreateDepthStencilImmutableState( const DepthStencilConfig & pConfig )
	{
		ic3DebugAssert( _immutableStateFactoryBase );
		return _immutableStateFactoryBase->CreateDepthStencilState( pConfig );
	}

	GraphicsShaderLinkageImmutableStateHandle GpuDevice::CreateGraphicsShaderLinkageImmutableState( const GraphicsShaderSet & pShaderSet )
	{
		ic3DebugAssert( _immutableStateFactoryBase );
		return _immutableStateFactoryBase->CreateGraphicsShaderLinkageState( pShaderSet );
	}

	IAInputLayoutImmutableStateHandle GpuDevice::CreateIAInputLayoutImmutableState( const IAInputLayoutDefinition & pDefinition, Shader & pVertexShaderWithBinary )
	{
		ic3DebugAssert( _immutableStateFactoryBase );
		return _immutableStateFactoryBase->CreateIAInputLayoutState( pDefinition, pVertexShaderWithBinary );
	}

	IAVertexStreamImmutableStateHandle GpuDevice::CreateIAVertexStreamImmutableState( const IAVertexStreamDefinition & pDefinition )
	{
		ic3DebugAssert( _immutableStateFactoryBase );
		return _immutableStateFactoryBase->CreateIAVertexStreamState( pDefinition );
	}

	RasterizerImmutableStateHandle GpuDevice::CreateRasterizerImmutableState( const RasterizerConfig & pConfig )
	{
		ic3DebugAssert( _immutableStateFactoryBase );
		return _immutableStateFactoryBase->CreateRasterizerState( pConfig );
	}

	RenderTargetBindingImmutableStateHandle GpuDevice::CreateRenderTargetBindingImmutableState( const RenderTargetBindingDefinition & pDefinition )
	{
		ic3DebugAssert( _immutableStateFactoryBase );
		return _immutableStateFactoryBase->CreateRenderTargetBindingState( pDefinition );
	}

	RenderPassConfigurationImmutableStateHandle GpuDevice::CreateRenderPassConfigurationImmutableState( const RenderPassConfiguration & pConfiguration )
	{
		ic3DebugAssert( _immutableStateFactoryBase );
		return _immutableStateFactoryBase->CreateRenderPassState( pConfiguration.GetValidated() );
	}

	void GpuDevice::ResetImmutableStateCache( cppx::bitmask<EPipelineImmutableStateTypeFlags> pResetMask )
	{
		ic3DebugAssert( _immutableStateCachePtr );
		_immutableStateCachePtr->Reset( pResetMask );
	}

	void GpuDevice::SetPresentationLayer( PresentationLayerHandle pPresentationLayer )
	{
		if( !_DrvOnSetPresentationLayer( pPresentationLayer ) )
		{
			return;
		}
		_presentationLayer = pPresentationLayer;
	}

	GpuDevice & GpuDevice::nullDevice()
	{
		static const GpuDeviceHandle sNullDeviceInstance = CreateGfxObject<GpuDeviceNull>( GpuDriver::GetNullDriver() );
		return *sNullDeviceInstance;
	}

	bool GpuDevice::OnGpuResourceActiveRefsZero( GpuResource & pGpuResource )
	{
		return true;
	}

	void GpuDevice::SetImmutableStateCache( PipelineImmutableStateCache & pStateCache )
	{
		_immutableStateCachePtr = &pStateCache;
		_immutableStateFactoryBase = &( pStateCache.mStateFactory );
	}

	bool GpuDevice::_DrvOnSetPresentationLayer( PresentationLayerHandle pPresentationLayer )
	{
		return true;
	}

	GpuBufferHandle GpuDevice::_DrvCreateGpuBuffer( const GpuBufferCreateInfo & pCreateInfo )
	{
		return nullptr;
	}

	SamplerHandle GpuDevice::_DrvCreateSampler( const SamplerCreateInfo & pCreateInfo )
	{
		return nullptr;
	}

	ShaderHandle GpuDevice::_DrvCreateShader( const ShaderCreateInfo & pCreateInfo )
	{
		return nullptr;
	}

	TextureHandle GpuDevice::_DrvCreateTexture( const TextureCreateInfo & pCreateInfo )
	{
		return nullptr;
	}

	RenderTargetTextureHandle GpuDevice::_DrvCreateRenderTargetTexture( const RenderTargetTextureCreateInfo & pCreateInfo )
	{
		return nullptr;
	}

	GraphicsPipelineStateObjectHandle GpuDevice::_DrvCreateGraphicsPipelineStateObject( const GraphicsPipelineStateObjectCreateInfo & pCreateInfo )
	{
		return nullptr;
	}

} // namespace Ic3::Graphics::GCI
