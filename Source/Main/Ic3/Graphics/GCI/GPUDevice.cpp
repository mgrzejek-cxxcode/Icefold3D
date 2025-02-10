
#include "CommandSystem.h"
#include "GPUDeviceNull.h"
#include "GPUDriver.h"
#include "PresentationLayer.h"
#include "Resources/Texture.h"
#include "Resources/RenderTargetTexture.h"
#include "State/GraphicsPipelineStateDescriptorShader.h"
#include "State/GraphicsPipelineStateCommon.h"
#include "State/PipelineStateDescriptorManager.h"

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
		EGPUDeviceInternalStateFlagDebugDeviceBit = 0x0001,
		EGPUDeviceInternalStateFlagMultiThreadAccessBit = 0x0002,
		EGPUDeviceInternalStateFlagEnableResourceActiveRefsTrackingBit = 0x0008
	};

	
	GPUDevice::GPUDevice( GPUDriver & pDriver )
	: GPUDriverChildObject( pDriver )
	, mGPUDriverID( pDriver.QueryGPUDriverID() )
	, mSysContext( pDriver.mSysContext )
	{
		if( pDriver.IsDebugFunctionalityRequested() )
		{
			_internalStateFlags.set( EGPUDeviceInternalStateFlagDebugDeviceBit );
		}
	}

	GPUDevice::~GPUDevice() = default;

	bool GPUDevice::IsNullDevice() const noexcept
	{
		return false;
	}

	bool GPUDevice::IsDebugDevice() const noexcept
	{
		return _internalStateFlags.is_set( EGPUDeviceInternalStateFlagDebugDeviceBit );
	}

	bool GPUDevice::IsMultiThreadAccessSupported() const noexcept
	{
		return _internalStateFlags.is_set( EGPUDeviceInternalStateFlagMultiThreadAccessBit );
	}

	bool GPUDevice::IsResourceActiveRefsTrackingEnabled() const noexcept
	{
		return _internalStateFlags.is_set( EGPUDeviceInternalStateFlagEnableResourceActiveRefsTrackingBit );
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
			auto shaderLinkageStateDescriptor = pCreateInfo.shaderLinkageStateDescriptor;
			if( !shaderLinkageStateDescriptor && CXU::IsPipelineStateDescriptorIDValid( pCreateInfo.shaderLinkageStateDescriptorID ) )
			{
				shaderLinkageStateDescriptor =
					_pipelineStateDescriptorManager->GetCachedDescriptorByID<GraphicsShaderLinkageStateDescriptor>(
						pCreateInfo.shaderLinkageStateDescriptorID );

				if( !shaderLinkageStateDescriptor )
				{
					return nullptr;
				}
			}
			psoCreateInfo.shaderLinkageStateDescriptor = shaderLinkageStateDescriptor;
		}

		{
			auto vertexAttributeLayoutStateDescriptor = pCreateInfo.vertexAttributeLayoutStateDescriptor;
			if( !vertexAttributeLayoutStateDescriptor && CXU::IsPipelineStateDescriptorIDValid( pCreateInfo.vertexAttributeLayoutStateDescriptorID ) )
			{
				vertexAttributeLayoutStateDescriptor =
					_pipelineStateDescriptorManager->GetCachedDescriptorByID<IAVertexAttributeLayoutStateDescriptor>(
						pCreateInfo.vertexAttributeLayoutStateDescriptorID );

				if( !vertexAttributeLayoutStateDescriptor )
				{
					return nullptr;
				}
			}
			psoCreateInfo.vertexAttributeLayoutStateDescriptor = vertexAttributeLayoutStateDescriptor;
		}

		{
			auto shaderRootSignatureStateDescriptor = pCreateInfo.shaderRootSignatureStateDescriptor;
			if( !shaderRootSignatureStateDescriptor && CXU::IsPipelineStateDescriptorIDValid( pCreateInfo.shaderRootSignatureStateDescriptorID ) )
			{
				shaderRootSignatureStateDescriptor =
					_pipelineStateDescriptorManager->GetCachedDescriptorByID<ShaderRootSignatureStateDescriptor>(
						pCreateInfo.shaderRootSignatureStateDescriptorID );

				if( !shaderRootSignatureStateDescriptor )
				{
					return nullptr;
				}
			}
			psoCreateInfo.shaderRootSignatureStateDescriptor = shaderRootSignatureStateDescriptor;
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

	GraphicsShaderLinkageStateDescriptorHandle GPUDevice::CreateGraphicsShaderLinkageStateDescriptor(
			const GraphicsShaderLinkageStateDescriptorCreateInfo & pCreateInfo )
	{
		return _pipelineStateDescriptorManager->CreateGraphicsShaderLinkageStateDescriptor( pCreateInfo );
	}

	IAVertexAttributeLayoutStateDescriptorHandle GPUDevice::CreateIAVertexAttributeLayoutStateDescriptor(
			const IAVertexAttributeLayoutStateDescriptorCreateInfo & pCreateInfo )
	{
		return _pipelineStateDescriptorManager->CreateIAVertexAttributeLayoutStateDescriptor( pCreateInfo );
	}

	ShaderRootSignatureStateDescriptorHandle GPUDevice::CreateShaderRootSignatureStateDescriptor(
			const ShaderRootSignatureStateDescriptorCreateInfo & pCreateInfo )
	{
		return _pipelineStateDescriptorManager->CreateShaderRootSignatureStateDescriptor( pCreateInfo );
	}

	IAVertexStreamBindingStateDescriptorHandle GPUDevice::CreateIAVertexStreamBindingStateDescriptor(
			const IAVertexStreamBindingStateDescriptorCreateInfo & pCreateInfo )
	{
		return _pipelineStateDescriptorManager->CreateIAVertexStreamBindingStateDescriptor( pCreateInfo );
	}

	RenderPassConfigurationStateDescriptorHandle GPUDevice::CreateRenderPassConfigurationStateDescriptor(
			const RenderPassConfigurationStateDescriptorCreateInfo & pCreateInfo )
	{
		return _pipelineStateDescriptorManager->CreateRenderPassConfigurationStateDescriptor( pCreateInfo );
	}

	void GPUDevice::ResetStateDescriptorCache( cppx::bitmask<EPipelineStateDescriptorTypeFlags> pResetMask )
	{
		Ic3DebugAssert( _pipelineStateDescriptorManager );
		_pipelineStateDescriptorManager->Reset( pResetMask );
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
		if( _pipelineStateDescriptorManager->CreateDefaultStateDescriptors() )
		{
			return true;
		}

		_pipelineStateDescriptorManager.reset();

		return false;
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
