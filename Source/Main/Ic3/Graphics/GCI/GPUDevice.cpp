
#include "CommandSystem.h"
#include "GPUDeviceNull.h"
#include "GPUDriver.h"
#include "PresentationLayer.h"
#include "Resources/Shader.h"
#include "Resources/Texture.h"
#include "Resources/RenderTargetTexture.h"
#include "State/GraphicsPipelineStateDescriptorShader.h"
#include "State/GraphicsPipelineStateCommon.h"
#include "State/PipelineStateDescriptorManager.h"

namespace Ic3::Graphics::GCI
{
	
	static const Math::RGBAColorU8 kDefaultClearColorDriver0     { 0x11, 0x66, 0xCC, 0xFF };
	static const Math::RGBAColorU8 kDefaultClearColorDriverDX11  { 0x77, 0xAA, 0x5F, 0xFF };
	static const Math::RGBAColorU8 kDefaultClearColorDriverDX12  { 0x22, 0x88, 0x3F, 0xFF };
	static const Math::RGBAColorU8 kDefaultClearColorDriverMTL2  { 0xFF, 0x99, 0x66, 0xFF };
	static const Math::RGBAColorU8 kDefaultClearColorDriverGL4   { 0x55, 0x88, 0xAA, 0xFF };
	static const Math::RGBAColorU8 kDefaultClearColorDriverGLES3 { 0x7A, 0x00, 0x4D, 0xFF };
	static const Math::RGBAColorU8 kDefaultClearColorDriverVK1   { 0x8F, 0x0F, 0x1F, 0xFF };

	enum EGPUDeviceInternalStateFlags : uint32
	{
		eGPUDeviceInternalStateFlagDebugDeviceBit = 0x0001,
		eGPUDeviceInternalStateFlagMultiThreadAccessBit = 0x0002,
		eGPUDeviceInternalStateFlagEnableResourceActiveRefsTrackingBit = 0x0008
	};

	
	GPUDevice::GPUDevice( GPUDriver & pDriver, GPUDeviceFeatureQuery & pFeatureQueryInterface )
	: GPUDriverChildObject( pDriver )
	, mGPUDriverID( pDriver.QueryGPUDriverID() )
	, mSysContext( pDriver.mSysContext )
	, mFeatureQueryInterface( &pFeatureQueryInterface )
	{
		if( pDriver.IsDebugFunctionalityRequested() )
		{
			_internalStateFlags.set( eGPUDeviceInternalStateFlagDebugDeviceBit );
		}
	}

	GPUDevice::~GPUDevice() = default;

	bool GPUDevice::IsNullDevice() const noexcept
	{
		return false;
	}

	bool GPUDevice::IsDebugDevice() const noexcept
	{
		return _internalStateFlags.is_set( eGPUDeviceInternalStateFlagDebugDeviceBit );
	}

	bool GPUDevice::IsMultiThreadAccessSupported() const noexcept
	{
		return _internalStateFlags.is_set( eGPUDeviceInternalStateFlagMultiThreadAccessBit );
	}

	bool GPUDevice::IsResourceActiveRefsTrackingEnabled() const noexcept
	{
		return _internalStateFlags.is_set( eGPUDeviceInternalStateFlagEnableResourceActiveRefsTrackingBit );
	}

	CommandSystem & GPUDevice::GetCommandSystem() const noexcept
	{
		return *_commandSystem;
	}

	PipelineStateDescriptorManager & GPUDevice::GetDescriptorManager() const noexcept
	{
		return *_pipelineStateDescriptorManager;
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
				return kDefaultClearColorDriverDX11;

			case EGPUDriverID::GDIDirectX12:
				return kDefaultClearColorDriverDX12;

			case EGPUDriverID::GDIMetal1:
				return kDefaultClearColorDriverMTL2;

			case EGPUDriverID::GDIOpenGLDesktop4:
				return kDefaultClearColorDriverGL4;

			case EGPUDriverID::GDIOpenGLES3:
				return kDefaultClearColorDriverGLES3;

			case EGPUDriverID::GDIVulkan10:
				return kDefaultClearColorDriverVK1;

			default:
				break;
		}
		return kDefaultClearColorDriver0;
	}

	const RenderTargetAttachmentClearConfig & GPUDevice::GetDefaultClearConfig() const noexcept
	{
		static const RenderTargetAttachmentClearConfig kDefaultClearConfig =
		{
			GetDefaultClearColor(),
			1.0f,
			0
		};
		return kDefaultClearConfig;
	}

	const BlendSettings & GPUDevice::GetDefaultBlendSettings()
	{
		static const BlendSettings kDefaultBlendSettings =
		{
			eRTAttachmentMaskDefaultC0DS,
			eBlendConfigMaskDefault,
			Math::kColorBlackOpaque,
			{
				EBlendFactor::One,
				EBlendFactor::Zero,
				EBlendOp::Add,
				EBlendFactor::One,
				EBlendFactor::Zero,
				EBlendOp::Add,
				eBlendWriteMaskAll
			}
		};
		return kDefaultBlendSettings;
	}

	const DepthStencilSettings & GPUDevice::GetDefaultDepthStencilSettings()
	{
		static const DepthStencilSettings kDefaultDepthStencilSettings =
		{
			eDepthStencilConfigMaskNone,
			{
				ECompFunc::Less,
				EDepthWriteMask::All
			},
			{
				{
					ECompFunc::Always,
					EStencilOp::Keep,
					EStencilOp::Keep,
					EStencilOp::Keep
				},
				{
					ECompFunc::Always,
					EStencilOp::Keep,
					EStencilOp::Keep,
					EStencilOp::Keep
				},
				0,
				0
			}
		};
		return kDefaultDepthStencilSettings;
	}

	const DepthStencilSettings & GPUDevice::GetDefaultDepthStencilSettingsWithDepthTestEnabled()
	{
		static const DepthStencilSettings kDefaultDepthStencilSettingsWithDepthTestEnabled =
		{
			eDepthStencilConfigFlagEnableDepthTestBit,
			{
				ECompFunc::Less,
				EDepthWriteMask::All
			},
			{
				{
					ECompFunc::Always,
					EStencilOp::Keep,
					EStencilOp::Keep,
					EStencilOp::Keep
				},
				{
					ECompFunc::Always,
					EStencilOp::Keep,
					EStencilOp::Keep,
					EStencilOp::Keep
				},
				0,
				0
			}
		};
		return kDefaultDepthStencilSettingsWithDepthTestEnabled;
	}

	const RasterizerSettings & GPUDevice::GetDefaultRasterizerSettings()
	{
		static const RasterizerSettings kDefaultRasterizerSettings =
		{
			eRasterizerConfigMaskNone,
			ECullMode::Back,
			EPrimitiveFillMode::Solid,
			ETriangleVerticesOrder::CounterClockwise
		};
		return kDefaultRasterizerSettings;
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

	GraphicsPipelineStateObjectHandle GPUDevice::CreateGraphicsPipelineStateObject(
			const GraphicsPipelineStateObjectCreateInfo & pCreateInfo )
	{
		GraphicsPipelineStateObjectCreateInfo psoCreateInfo{};

		{
			if( pCreateInfo.renderTargetLayout.IsEmpty() )
			{
				return nullptr;
			}
			psoCreateInfo.renderTargetLayout = pCreateInfo.renderTargetLayout;
		}

		{
			auto blendStateDescriptor = pCreateInfo.blendStateDescriptor;
			if( !blendStateDescriptor && CXU::IsPipelineStateDescriptorIDValid( pCreateInfo.blendStateDescriptorID ) )
			{
				blendStateDescriptor =
					_pipelineStateDescriptorManager->GetCachedDescriptorByID<BlendStateDescriptor>(
						pCreateInfo.depthStencilStateDescriptorID );

				if( !blendStateDescriptor )
				{
					return nullptr;
				}
			}
			psoCreateInfo.blendStateDescriptor = blendStateDescriptor;
		}

		{
			auto depthStencilStateDescriptor = pCreateInfo.depthStencilStateDescriptor;
			if( !depthStencilStateDescriptor && CXU::IsPipelineStateDescriptorIDValid( pCreateInfo.depthStencilStateDescriptorID ) )
			{
				depthStencilStateDescriptor =
					_pipelineStateDescriptorManager->GetCachedDescriptorByID<DepthStencilStateDescriptor>(
						pCreateInfo.depthStencilStateDescriptorID );

				if( !depthStencilStateDescriptor )
				{
					return nullptr;
				}
			}
			psoCreateInfo.depthStencilStateDescriptor = depthStencilStateDescriptor;
		}

		{
			auto rasterizerStateDescriptor = pCreateInfo.rasterizerStateDescriptor;
			if( !rasterizerStateDescriptor && CXU::IsPipelineStateDescriptorIDValid( pCreateInfo.rasterizerStateDescriptorID ) )
			{
				rasterizerStateDescriptor =
					_pipelineStateDescriptorManager->GetCachedDescriptorByID<RasterizerStateDescriptor>(
						pCreateInfo.rasterizerStateDescriptorID );

				if( !rasterizerStateDescriptor )
				{
					return nullptr;
				}
			}
			psoCreateInfo.rasterizerStateDescriptor = rasterizerStateDescriptor;
		}

		{
			auto rootSignatureDescriptor = pCreateInfo.rootSignatureDescriptor;
			if( !rootSignatureDescriptor && CXU::IsPipelineStateDescriptorIDValid( pCreateInfo.rootSignatureDescriptorID ) )
			{
				rootSignatureDescriptor =
					_pipelineStateDescriptorManager->GetCachedDescriptorByID<RootSignatureDescriptor>(
						pCreateInfo.rootSignatureDescriptorID );

				if( !rootSignatureDescriptor )
				{
					return nullptr;
				}
			}
			psoCreateInfo.rootSignatureDescriptor = rootSignatureDescriptor;
		}

		{
			auto shaderLinkageStateDescriptor = pCreateInfo.shaderLinkageStateDescriptor;
			if( !shaderLinkageStateDescriptor && CXU::IsPipelineStateDescriptorIDValid( pCreateInfo.shaderLinkageStateDescriptorID ) )
			{
				shaderLinkageStateDescriptor =
					_pipelineStateDescriptorManager->GetCachedDescriptorByID<GraphicsShaderLinkageDescriptor>(
						pCreateInfo.shaderLinkageStateDescriptorID );

				if( !shaderLinkageStateDescriptor )
				{
					return nullptr;
				}
			}
			psoCreateInfo.shaderLinkageStateDescriptor = shaderLinkageStateDescriptor;
		}

		{
			auto vertexAttributeLayoutDescriptor = pCreateInfo.vertexAttributeLayoutDescriptor;
			if( !vertexAttributeLayoutDescriptor && CXU::IsPipelineStateDescriptorIDValid( pCreateInfo.vertexAttributeLayoutDescriptorID ) )
			{
				vertexAttributeLayoutDescriptor =
					_pipelineStateDescriptorManager->GetCachedDescriptorByID<VertexAttributeLayoutDescriptor>(
						pCreateInfo.vertexAttributeLayoutDescriptorID );

				if( !vertexAttributeLayoutDescriptor )
				{
					return nullptr;
				}
			}
			psoCreateInfo.vertexAttributeLayoutDescriptor = vertexAttributeLayoutDescriptor;
		}

		return _DrvCreateGraphicsPipelineStateObject( pCreateInfo );
	}
	
	BlendStateDescriptorHandle GPUDevice::CreateBlendStateDescriptor(
			const BlendStateDescriptorCreateInfo & pCreateInfo )
	{
		return _pipelineStateDescriptorManager->CreateBlendStateDescriptor( pCreateInfo );
	}

	DepthStencilStateDescriptorHandle GPUDevice::CreateDepthStencilStateDescriptor(
			const DepthStencilStateDescriptorCreateInfo & pCreateInfo )
	{
		return _pipelineStateDescriptorManager->CreateDepthStencilStateDescriptor( pCreateInfo );
	}

	RasterizerStateDescriptorHandle GPUDevice::CreateRasterizerStateDescriptor(
			const RasterizerStateDescriptorCreateInfo & pCreateInfo )
	{
		return _pipelineStateDescriptorManager->CreateRasterizerStateDescriptor( pCreateInfo );
	}

	GraphicsShaderLinkageDescriptorHandle GPUDevice::CreateGraphicsShaderLinkageDescriptor(
			const GraphicsShaderLinkageDescriptorCreateInfo & pCreateInfo )
	{
		return _pipelineStateDescriptorManager->CreateGraphicsShaderLinkageDescriptor( pCreateInfo );
	}

	VertexAttributeLayoutDescriptorHandle GPUDevice::CreateVertexAttributeLayoutDescriptor(
			const VertexAttributeLayoutDescriptorCreateInfo & pCreateInfo )
	{
		return _pipelineStateDescriptorManager->CreateVertexAttributeLayoutDescriptor( pCreateInfo );
	}

	RootSignatureDescriptorHandle GPUDevice::CreateRootSignatureDescriptor(
			const RootSignatureDescriptorCreateInfo & pCreateInfo )
	{
		return _pipelineStateDescriptorManager->CreateRootSignatureDescriptor( pCreateInfo );
	}

	RenderPassDescriptorHandle GPUDevice::CreateRenderPassDescriptor(
			const RenderPassDescriptorCreateInfo & pCreateInfo )
	{
		return _pipelineStateDescriptorManager->CreateRenderPassDescriptor( pCreateInfo );
	}

	RenderTargetDescriptorHandle GPUDevice::CreateRenderTargetDescriptor(
			const RenderTargetDescriptorCreateInfo & pCreateInfo )
	{
		return _pipelineStateDescriptorManager->CreateRenderTargetDescriptor( pCreateInfo );
	}

	VertexSourceBindingDescriptorHandle GPUDevice::CreateVertexSourceBindingDescriptor(
			const VertexSourceBindingDescriptorCreateInfo & pCreateInfo )
	{
		return _pipelineStateDescriptorManager->CreateVertexSourceBindingDescriptor( pCreateInfo );
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

	bool GPUDevice::InitializePipelineStateDescriptorManager( PipelineStateDescriptorFactory & pDescriptorFactory )
	{
		Ic3DebugAssert( !_pipelineStateDescriptorManager );
		_pipelineStateDescriptorManager = std::make_unique<PipelineStateDescriptorManager>( *this, pDescriptorFactory );
		return true;
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
