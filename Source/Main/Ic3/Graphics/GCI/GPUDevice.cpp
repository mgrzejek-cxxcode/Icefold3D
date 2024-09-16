
#include "CommandSystem.h"
#include "GPUDeviceNull.h"
#include "GPUDriver.h"
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

	enum EGPUDeviceInternalStateFlags : uint32
	{
		E_GPU_DEVICE_INTERNAL_STATE_FLAG_DEBUG_DEVICEBit = 0x0001,
		E_GPU_DEVICE_INTERNAL_STATE_FLAG_MULTI_THREAD_ACCESSBit = 0x0002,
		E_GPU_DEVICE_INTERNAL_STATE_FLAG_ENABLE_RESOURCE_ACTIVE_REFS_TRACKINGBit = 0x0008
	};

	
	GPUDevice::GPUDevice( GPUDriver & pDriver )
	: GPUDriverChildObject( pDriver )
	, mGPUDriverID( pDriver.queryGPUDriverID() )
	, mSysContext( pDriver.mSysContext )
	{
		if( pDriver.isDebugFunctionalityRequested() )
		{
			_internalStateFlags.set( E_GPU_DEVICE_INTERNAL_STATE_FLAG_DEBUG_DEVICEBit );
		}
	}

	GPUDevice::~GPUDevice() = default;

	bool GPUDevice::isNullDevice() const noexcept
	{
		return false;
	}

	bool GPUDevice::isDebugDevice() const noexcept
	{
		return _internalStateFlags.isSet( E_GPU_DEVICE_INTERNAL_STATE_FLAG_DEBUG_DEVICEBit );
	}

	bool GPUDevice::isMultiThreadAccessSupported() const noexcept
	{
		return _internalStateFlags.isSet( E_GPU_DEVICE_INTERNAL_STATE_FLAG_MULTI_THREAD_ACCESSBit );
	}

	bool GPUDevice::isResourceActiveRefsTrackingEnabled() const noexcept
	{
		return _internalStateFlags.isSet( E_GPU_DEVICE_INTERNAL_STATE_FLAG_ENABLE_RESOURCE_ACTIVE_REFS_TRACKINGBit );
	}

	CommandSystem & GPUDevice::getCommandSystem() const noexcept
	{
		return *_commandSystem;
	}

	PipelineImmutableStateFactory & GPUDevice::getPipelineStateFactory() const noexcept
	{
		return *_immutableStateFactoryBase;
	}

	PresentationLayer * GPUDevice::getPresentationLayer() const noexcept
	{
		return _presentationLayer.get();
	}

	const Math::RGBAColorU8 & GPUDevice::getDefaultClearColor() const noexcept
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

	const RenderTargetAttachmentClearConfig & GPUDevice::getDefaultClearConfig() const noexcept
	{
		static const RenderTargetAttachmentClearConfig sDefaultClearConfig =
		{
			getDefaultClearColor(),
			1.0f,
			0
		};
		return sDefaultClearConfig;
	}

	GPUBufferHandle GPUDevice::createGPUBuffer( const GPUBufferCreateInfo & pCreateInfo )
	{
	    return _drvCreateGPUBuffer( pCreateInfo );
	}

	SamplerHandle GPUDevice::createSampler( const SamplerCreateInfo & pCreateInfo )
	{
	    return _drvCreateSampler( pCreateInfo );
	}

	ShaderHandle GPUDevice::createShader( const ShaderCreateInfo & pCreateInfo )
	{
	    return _drvCreateShader( pCreateInfo );
	}

	TextureHandle GPUDevice::createTexture( const TextureCreateInfo & pCreateInfo )
	{
	    return _drvCreateTexture( pCreateInfo );
	}

	RenderTargetTextureHandle GPUDevice::createRenderTargetTexture( const RenderTargetTextureCreateInfo & pCreateInfo )
	{
		if( !pCreateInfo.mBindFlags.isSetAnyOf( eGPUResourceUsageFlagRenderTargetColorBit | eGPUResourceUsageMaskRenderTargetDepthStencil ) )
		{
			ic3DebugOutput( "No RT attachment bind flags specified for the RT texture (E_GPU_RESOURCE_USAGE_xxx_RENDER_TARGET_yyy)." );
			return nullptr;
		}

		if( pCreateInfo.mTargetTexture )
		{
			const auto & targetTextureResourceFlags = pCreateInfo.mTargetTexture->mTextureProperties.mResourceFlags;
			if( !targetTextureResourceFlags.isSet( pCreateInfo.mBindFlags & eGPUResourceUsageMaskAll ) )
			{
				ic3DebugOutput( "Target texture for Render Target is not compatible with specified bind flags (E_GPU_RESOURCE_USAGE_xxx)." );
				return nullptr;
			}
		}

		return _drvCreateRenderTargetTexture( pCreateInfo );
	}

	GraphicsPipelineStateObjectHandle GPUDevice::createGraphicsPipelineStateObject( const GraphicsPipelineStateObjectCreateInfo & pCreateInfo )
	{
		if( pCreateInfo.renderTargetLayout.empty() )
		{
			return nullptr;
		}

		if( !pCreateInfo.shaderInputSignature )
		{
			pCreateInfo.shaderInputSignature = SMU::createShaderInputSignature( pCreateInfo.shaderInputSignatureDesc );
		}

		if( !pCreateInfo.blendState )
		{
			pCreateInfo.blendState = _immutableStateFactoryBase->createBlendState( pCreateInfo.blendConfig );
		}

		if( !pCreateInfo.depthStencilState )
		{
			pCreateInfo.depthStencilState = _immutableStateFactoryBase->createDepthStencilState( pCreateInfo.depthStencilConfig );
		}

		if( !pCreateInfo.rasterizerState )
		{
			pCreateInfo.rasterizerState = _immutableStateFactoryBase->createRasterizerState( pCreateInfo.rasterizerConfig );
		}

		if( !pCreateInfo.shaderLinkageState )
		{
			pCreateInfo.shaderLinkageState = _immutableStateFactoryBase->createGraphicsShaderLinkageState( pCreateInfo.shaderSet );
		}

		if( !pCreateInfo.inputLayoutState )
		{
			auto * vertexShader = pCreateInfo.shaderLinkageState->getShader( EShaderType::GSVertex );
			pCreateInfo.inputLayoutState = _immutableStateFactoryBase->createIAInputLayoutState( pCreateInfo.inputLayoutDefinition, *vertexShader );
		}

		return _drvCreateGraphicsPipelineStateObject( pCreateInfo );
	}

	BlendImmutableStateHandle GPUDevice::createBlendImmutableState( const BlendConfig & pConfig )
	{
		ic3DebugAssert( _immutableStateFactoryBase );
		return _immutableStateFactoryBase->createBlendState( pConfig );
	}

	DepthStencilImmutableStateHandle GPUDevice::createDepthStencilImmutableState( const DepthStencilConfig & pConfig )
	{
		ic3DebugAssert( _immutableStateFactoryBase );
		return _immutableStateFactoryBase->createDepthStencilState( pConfig );
	}

	GraphicsShaderLinkageImmutableStateHandle GPUDevice::createGraphicsShaderLinkageImmutableState( const GraphicsShaderSet & pShaderSet )
	{
		ic3DebugAssert( _immutableStateFactoryBase );
		return _immutableStateFactoryBase->createGraphicsShaderLinkageState( pShaderSet );
	}

	IAInputLayoutImmutableStateHandle GPUDevice::createIAInputLayoutImmutableState( const IAInputLayoutDefinition & pDefinition, Shader & pVertexShaderWithBinary )
	{
		ic3DebugAssert( _immutableStateFactoryBase );
		return _immutableStateFactoryBase->createIAInputLayoutState( pDefinition, pVertexShaderWithBinary );
	}

	IAVertexStreamImmutableStateHandle GPUDevice::createIAVertexStreamImmutableState( const IAVertexStreamDefinition & pDefinition )
	{
		ic3DebugAssert( _immutableStateFactoryBase );
		return _immutableStateFactoryBase->createIAVertexStreamState( pDefinition );
	}

	RasterizerImmutableStateHandle GPUDevice::createRasterizerImmutableState( const RasterizerConfig & pConfig )
	{
		ic3DebugAssert( _immutableStateFactoryBase );
		return _immutableStateFactoryBase->createRasterizerState( pConfig );
	}

	RenderTargetBindingImmutableStateHandle GPUDevice::createRenderTargetBindingImmutableState( const RenderTargetBindingDefinition & pDefinition )
	{
		ic3DebugAssert( _immutableStateFactoryBase );
		return _immutableStateFactoryBase->createRenderTargetBindingState( pDefinition );
	}

	RenderPassConfigurationImmutableStateHandle GPUDevice::createRenderPassConfigurationImmutableState( const RenderPassConfiguration & pConfiguration )
	{
		ic3DebugAssert( _immutableStateFactoryBase );
		return _immutableStateFactoryBase->createRenderPassState( pConfiguration.getValidated() );
	}

	void GPUDevice::resetImmutableStateCache( TBitmask<EPipelineImmutableStateTypeFlags> pResetMask )
	{
		ic3DebugAssert( _immutableStateCachePtr );
		_immutableStateCachePtr->reset( pResetMask );
	}

	void GPUDevice::setPresentationLayer( PresentationLayerHandle pPresentationLayer )
	{
		if( !_drvOnSetPresentationLayer( pPresentationLayer ) )
		{
			return;
		}
		_presentationLayer = pPresentationLayer;
	}

	GPUDevice & GPUDevice::nullDevice()
	{
		static const GPUDeviceHandle sNullDeviceInstance = createGPUAPIObject<GPUDeviceNull>( GPUDriver::nullDriver() );
		return *sNullDeviceInstance;
	}

	bool GPUDevice::onGPUResourceActiveRefsZero( GPUResource & pGPUResource )
	{
		return true;
	}

	void GPUDevice::setImmutableStateCache( PipelineImmutableStateCache & pStateCache )
	{
		_immutableStateCachePtr = &pStateCache;
		_immutableStateFactoryBase = &( pStateCache.mStateFactory );
	}

	bool GPUDevice::_drvOnSetPresentationLayer( PresentationLayerHandle pPresentationLayer )
	{
		return true;
	}

	GPUBufferHandle GPUDevice::_drvCreateGPUBuffer( const GPUBufferCreateInfo & pCreateInfo )
	{
		return nullptr;
	}

	SamplerHandle GPUDevice::_drvCreateSampler( const SamplerCreateInfo & pCreateInfo )
	{
		return nullptr;
	}

	ShaderHandle GPUDevice::_drvCreateShader( const ShaderCreateInfo & pCreateInfo )
	{
		return nullptr;
	}

	TextureHandle GPUDevice::_drvCreateTexture( const TextureCreateInfo & pCreateInfo )
	{
		return nullptr;
	}

	RenderTargetTextureHandle GPUDevice::_drvCreateRenderTargetTexture( const RenderTargetTextureCreateInfo & pCreateInfo )
	{
		return nullptr;
	}

	GraphicsPipelineStateObjectHandle GPUDevice::_drvCreateGraphicsPipelineStateObject( const GraphicsPipelineStateObjectCreateInfo & pCreateInfo )
	{
		return nullptr;
	}

} // namespace Ic3::Graphics::GCI
