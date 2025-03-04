
#include "CommandContext.h"
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

	
	GPUDevice::GPUDevice(
			GPUDriver & pDriver,
			GPUDeviceFeatureQuery * pFeatureQueryInterface,
			PipelineStateDescriptorManager * pPipelineStateDescriptorManager )
	: GPUDriverChildObject( pDriver )
	, mGPUDriverID( pDriver.QueryGPUDriverID() )
	, mSysContext( pDriver.mSysContext )
	, mFeatureQueryInterface( pFeatureQueryInterface )
	, mPipelineStateDescriptorManager( pPipelineStateDescriptorManager )
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
					mPipelineStateDescriptorManager->GetCachedDescriptorByID<BlendStateDescriptor>(
						pCreateInfo.blendStateDescriptorID );

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
					mPipelineStateDescriptorManager->GetCachedDescriptorByID<DepthStencilStateDescriptor>(
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
					mPipelineStateDescriptorManager->GetCachedDescriptorByID<RasterizerStateDescriptor>(
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
					mPipelineStateDescriptorManager->GetCachedDescriptorByID<RootSignatureDescriptor>(
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
					mPipelineStateDescriptorManager->GetCachedDescriptorByID<GraphicsShaderLinkageDescriptor>(
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
					mPipelineStateDescriptorManager->GetCachedDescriptorByID<VertexAttributeLayoutDescriptor>(
						pCreateInfo.vertexAttributeLayoutDescriptorID );

				if( !vertexAttributeLayoutDescriptor )
				{
					return nullptr;
				}
			}
			psoCreateInfo.vertexAttributeLayoutDescriptor = vertexAttributeLayoutDescriptor;
		}

		return _DrvCreateGraphicsPipelineStateObject( psoCreateInfo );
	}
	
	BlendStateDescriptorHandle GPUDevice::CreateBlendStateDescriptor(
			const BlendStateDescriptorCreateInfo & pCreateInfo,
			const cppx::immutable_string & pOptionalDescriptorName )
	{
		return mPipelineStateDescriptorManager->CreateBlendStateDescriptor( pCreateInfo, pOptionalDescriptorName );
	}

	DepthStencilStateDescriptorHandle GPUDevice::CreateDepthStencilStateDescriptor(
			const DepthStencilStateDescriptorCreateInfo & pCreateInfo,
			const cppx::immutable_string & pOptionalDescriptorName )
	{
		return mPipelineStateDescriptorManager->CreateDepthStencilStateDescriptor( pCreateInfo, pOptionalDescriptorName );
	}

	RasterizerStateDescriptorHandle GPUDevice::CreateRasterizerStateDescriptor(
			const RasterizerStateDescriptorCreateInfo & pCreateInfo,
			const cppx::immutable_string & pOptionalDescriptorName )
	{
		return mPipelineStateDescriptorManager->CreateRasterizerStateDescriptor( pCreateInfo, pOptionalDescriptorName );
	}

	GraphicsShaderLinkageDescriptorHandle GPUDevice::CreateGraphicsShaderLinkageDescriptor(
			const GraphicsShaderLinkageDescriptorCreateInfo & pCreateInfo,
			const cppx::immutable_string & pOptionalDescriptorName )
	{
		return mPipelineStateDescriptorManager->CreateGraphicsShaderLinkageDescriptor( pCreateInfo, pOptionalDescriptorName );
	}

	VertexAttributeLayoutDescriptorHandle GPUDevice::CreateVertexAttributeLayoutDescriptor(
			const VertexAttributeLayoutDescriptorCreateInfo & pCreateInfo,
			const cppx::immutable_string & pOptionalDescriptorName )
	{
		return mPipelineStateDescriptorManager->CreateVertexAttributeLayoutDescriptor( pCreateInfo, pOptionalDescriptorName );
	}

	RootSignatureDescriptorHandle GPUDevice::CreateRootSignatureDescriptor(
			const RootSignatureDescriptorCreateInfo & pCreateInfo,
			const cppx::immutable_string & pOptionalDescriptorName )
	{
		return mPipelineStateDescriptorManager->CreateRootSignatureDescriptor( pCreateInfo, pOptionalDescriptorName );
	}

	RenderPassDescriptorHandle GPUDevice::CreateRenderPassDescriptor(
			const RenderPassDescriptorCreateInfo & pCreateInfo )
	{
		return mPipelineStateDescriptorManager->CreateRenderPassDescriptor( pCreateInfo );
	}

	RenderTargetDescriptorHandle GPUDevice::CreateRenderTargetDescriptor(
			const RenderTargetDescriptorCreateInfo & pCreateInfo )
	{
		return mPipelineStateDescriptorManager->CreateRenderTargetDescriptor( pCreateInfo );
	}

	VertexSourceBindingDescriptorHandle GPUDevice::CreateVertexSourceBindingDescriptor(
			const VertexSourceBindingDescriptorCreateInfo & pCreateInfo )
	{
		return mPipelineStateDescriptorManager->CreateVertexSourceBindingDescriptor( pCreateInfo );
	}

	std::unique_ptr<CommandContext> GPUDevice::AcquireCommandContext( ECommandContextType pContextType )
	{
		return _commandSystem->AcquireCommandContext( pContextType );
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
		static const GPUDeviceHandle kNullDeviceInstance = CreateGfxObject<GPUDeviceNull>( GPUDriver::GetNullDriver() );
		return *kNullDeviceInstance;
	}

	bool GPUDevice::OnGPUResourceActiveRefsZero( GPUResource & pGPUResource )
	{
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
