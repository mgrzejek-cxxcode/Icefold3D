
#pragma once

#ifndef __IC3_GRAPHICS_GCI_GPU_DEVICE_H__
#define __IC3_GRAPHICS_GCI_GPU_DEVICE_H__

#include "CommonCommandDefs.h"
#include "DisplayCommon.h"
#include "Resources/TextureCommon.h"
#include "State/GraphicsShaderState.h"
#include "State/RenderPassCommon.h"
#include "State/PipelineImmutableStateCache.h"

namespace Ic3::Graphics::GCI
{

	struct PresentationLayerCreateInfo;
	struct RenderTargetTextureCreateInfo;

	enum EGPUDeviceCreateFlags : uint32
	{
		E_GPU_DEVICE_CREATE_FLAG_INIT_DEFAULT_PRESENT_QUEUE_BIT = 0x0004,
		E_GPU_DEVICE_CREATE_FLAGS_DEFAULT = E_GPU_DEVICE_CREATE_FLAG_INIT_DEFAULT_PRESENT_QUEUE_BIT
	};

	enum EGPUDeviceStateFlags : uint32
	{
	};

	struct GPUDeviceCreateInfo
	{
		// DisplayManager * displayManager = nullptr;
		// display_system_id_t adapterOutputID = cvDisplaySystemIDDefault;
		display_system_id_t adapterID = cvDisplaySystemIDDefault;
		cppx::bitmask<EGPUDeviceCreateFlags> flags = E_GPU_DEVICE_CREATE_FLAGS_DEFAULT;
	};

	struct DepthStencilState;
	struct RasterizerState;
	struct ICommandList;

	class ICommandSystem
	{
	public:
		virtual ICommandList * CreateCommandList( /*... */ ) const = 0;
		virtual void ReleaseCommandList( ICommandList * pList ) const = 0;
	};

	class IGPUDevice
	{
	public:
		CPPX_ATTR_NO_DISCARD virtual ICommandList * AcquireCommandList() const = 0;

		CPPX_ATTR_NO_DISCARD virtual bool IsNullDevice() const noexcept;

		CPPX_ATTR_NO_DISCARD bool IsDebugDevice() const noexcept;
		CPPX_ATTR_NO_DISCARD bool IsMultiThreadAccessSupported() const noexcept;

		CPPX_ATTR_NO_DISCARD GPUBuffer * CreateGPUBuffer( const GPUBufferCreateInfo & pCreateInfo );
		CPPX_ATTR_NO_DISCARD Sampler * CreateSampler( const SamplerCreateInfo & pCreateInfo );
		CPPX_ATTR_NO_DISCARD Shader * CreateShader( const ShaderCreateInfo & pCreateInfo );
		CPPX_ATTR_NO_DISCARD Texture * CreateTexture( const TextureCreateInfo & pCreateInfo );

		CPPX_ATTR_NO_DISCARD DepthStencilState * CreateDepthStencilState( const DepthStencilConfig & pConfig );
		CPPX_ATTR_NO_DISCARD RasterizerState * CreateRasterizerState( const RasterizerConfig & pConfig );
		/* and so on... */
	};


	class IC3_GRAPHICS_GCI_CLASS GPUDevice : public GPUDriverChildObject
	{
		friend class GPUDriver;
		friend class GPUResource;

	public:
		EGPUDriverID const mGPUDriverID;
		System::SysContextHandle const mSysContext;

		explicit GPUDevice( GPUDriver & pDriver );
		virtual ~GPUDevice();

		CPPX_ATTR_NO_DISCARD virtual bool IsNullDevice() const noexcept;

		CPPX_ATTR_NO_DISCARD bool IsDebugDevice() const noexcept;

		CPPX_ATTR_NO_DISCARD bool IsMultiThreadAccessSupported() const noexcept;

		CPPX_ATTR_NO_DISCARD bool IsResourceActiveRefsTrackingEnabled() const noexcept;

		CPPX_ATTR_NO_DISCARD CommandSystem & GetCommandSystem() const noexcept;

		CPPX_ATTR_NO_DISCARD PipelineImmutableStateFactory & GetPipelineStateFactory() const noexcept;

		CPPX_ATTR_NO_DISCARD PresentationLayer * GetPresentationLayer() const noexcept;

		CPPX_ATTR_NO_DISCARD const Math::RGBAColorU8 & GetDefaultClearColor() const noexcept;

		CPPX_ATTR_NO_DISCARD const RenderTargetAttachmentClearConfig & GetDefaultClearConfig() const noexcept;

		template <typename TPState>
		CPPX_ATTR_NO_DISCARD TGfxHandle<TPState> GetCachedImmutableState( GfxObjectID pUniqueID ) const noexcept;

		template <typename TPState>
		CPPX_ATTR_NO_DISCARD TGfxHandle<TPState> GetCachedImmutableState( const GfxObjectName & pStateObjectName ) const noexcept;

		CPPX_ATTR_NO_DISCARD GPUBufferHandle CreateGPUBuffer( const GPUBufferCreateInfo & pCreateInfo );
		CPPX_ATTR_NO_DISCARD SamplerHandle CreateSampler( const SamplerCreateInfo & pCreateInfo );
		CPPX_ATTR_NO_DISCARD ShaderHandle CreateShader( const ShaderCreateInfo & pCreateInfo );
		CPPX_ATTR_NO_DISCARD TextureHandle CreateTexture( const TextureCreateInfo & pCreateInfo );

		/// @brief Creates an RTT using the provided CIS.
		/// This function will automatically Create a required resource, depending on the specified layout and usage.
		/// It can either be an explicit texture object which can be retrieved later or an implicit render buffer
		/// (e.g. if the RTT is supposed to be only a depth/stencil attachment used for depth and/or stencil testing).
		CPPX_ATTR_NO_DISCARD RenderTargetTextureHandle CreateRenderTargetTexture( const RenderTargetTextureCreateInfo & pCreateInfo );

		/// @brief
		CPPX_ATTR_NO_DISCARD GraphicsPipelineStateObjectHandle CreateGraphicsPipelineStateObject( const GraphicsPipelineStateObjectCreateInfo & pCreateInfo );

		CPPX_ATTR_NO_DISCARD BlendImmutableStateHandle CreateBlendImmutableState( const BlendConfig & pConfig );
		CPPX_ATTR_NO_DISCARD DepthStencilImmutableStateHandle CreateDepthStencilImmutableState( const DepthStencilConfig & pConfig );
		CPPX_ATTR_NO_DISCARD GraphicsShaderLinkageImmutableStateHandle CreateGraphicsShaderLinkageImmutableState( const GraphicsShaderSet & pShaderSet );
		CPPX_ATTR_NO_DISCARD IAInputLayoutImmutableStateHandle CreateIAInputLayoutImmutableState( const IAInputLayoutDefinition & pDefinition, Shader & pVertexShaderWithBinary );
		CPPX_ATTR_NO_DISCARD IAVertexStreamImmutableStateHandle CreateIAVertexStreamImmutableState( const IAVertexStreamDefinition & pDefinition );
		CPPX_ATTR_NO_DISCARD RasterizerImmutableStateHandle CreateRasterizerImmutableState( const RasterizerConfig & pConfig );
		CPPX_ATTR_NO_DISCARD RenderTargetBindingImmutableStateHandle CreateRenderTargetBindingImmutableState( const RenderTargetBindingDefinition & pDefinition );
		CPPX_ATTR_NO_DISCARD RenderPassConfigurationImmutableStateHandle CreateRenderPassConfigurationImmutableState( const RenderPassConfiguration & pConfiguration );

		template <typename TPState, typename TPInputDesc, typename... TArgs>
		TGfxHandle<TPState> CreateCachedImmutableState( GfxObjectID pUniqueID, const TPInputDesc & pInputDesc, TArgs && ...pArgs );

		template <typename TPState, typename TPInputDesc, typename... TArgs>
		TGfxHandle<TPState> CreateCachedImmutableState( const GfxObjectName & pUniqueName, const TPInputDesc & pInputDesc, TArgs && ...pArgs );

		void ResetImmutableStateCache( cppx::bitmask<EPipelineImmutableStateTypeFlags> pResetMask = ePipelineImmutableStateTypeMaskAll );

		void SetPresentationLayer( PresentationLayerHandle pPresentationLayer );

		virtual void WaitForCommandSync( CommandSync & pCommandSync ) = 0;

		CPPX_ATTR_NO_DISCARD static GPUDevice & GetNullDevice();

	protected:
		virtual bool OnGPUResourceActiveRefsZero( GPUResource & pGPUResource );

		void SetImmutableStateCache( PipelineImmutableStateCache & pStateCache );

	private:
		/// @brief API-level initialization of the command system. Called by the parent driver when a device is created.
		virtual void InitializeCommandSystem() = 0;

	Ic3driverApi( private ):
		virtual bool _DrvOnSetPresentationLayer( PresentationLayerHandle pPresentationLayer );

		virtual GPUBufferHandle _DrvCreateGPUBuffer( const GPUBufferCreateInfo & pCreateInfo );
		virtual SamplerHandle _DrvCreateSampler( const SamplerCreateInfo & pCreateInfo );
		virtual ShaderHandle _DrvCreateShader( const ShaderCreateInfo & pCreateInfo );
		virtual TextureHandle _DrvCreateTexture( const TextureCreateInfo & pCreateInfo );
		virtual RenderTargetTextureHandle _DrvCreateRenderTargetTexture( const RenderTargetTextureCreateInfo & pCreateInfo );

		virtual GraphicsPipelineStateObjectHandle _DrvCreateGraphicsPipelineStateObject(
				const GraphicsPipelineStateObjectCreateInfo & pCreateInfo );

	protected:
		CommandSystemHandle _commandSystem;
		PresentationLayerHandle _presentationLayer;

		/// Factory used to Create immutable states. Set by the actual driver class during initialization.
		/// This decouples the state creation from the GPUDevice class so it's easier to manage and extend.
		PipelineImmutableStateFactory * _immutableStateFactoryBase = nullptr;

		/// Immutable state cache. Holds created states and enables re-using them across all APIs.
		/// Requires PipelineImmutableStateFactory to be specified when created.
		PipelineImmutableStateCache * _immutableStateCachePtr = nullptr;

		cppx::bitmask<uint32> _internalStateFlags;
	};

	template <typename TPState>
	inline TGfxHandle<TPState> GPUDevice::GetCachedImmutableState( GfxObjectID pUniqueID ) const noexcept
	{
		return _immutableStateCachePtr->template GetState<TPState>( pUniqueID );
	}

	template <typename TPState>
	inline TGfxHandle<TPState> GPUDevice::GetCachedImmutableState( const GfxObjectName & pUniqueName ) const noexcept
	{
		return _immutableStateCachePtr->template GetState<TPState>( pUniqueName );
	}

	template <typename TPState, typename TPInputDesc, typename... TArgs>
	inline TGfxHandle<TPState> GPUDevice::CreateCachedImmutableState( GfxObjectID pUniqueID, const TPInputDesc & pInputDesc, TArgs && ...pArgs )
	{
		return _immutableStateCachePtr->template CreateState<TPState>( pUniqueID, pInputDesc, std::forward<TArgs>( pArgs )... );
	}

	template <typename TPState, typename TPInputDesc, typename... TArgs>
	inline TGfxHandle<TPState> GPUDevice::CreateCachedImmutableState( const GfxObjectName & pUniqueName, const TPInputDesc & pInputDesc, TArgs && ...pArgs )
	{
		return _immutableStateCachePtr->template CreateState<TPState>( pUniqueName, pInputDesc, std::forward<TArgs>( pArgs )... );
	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_GPU_DEVICE_H__
