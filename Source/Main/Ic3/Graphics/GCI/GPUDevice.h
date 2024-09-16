
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
		E_GPU_DEVICE_CREATE_FLAG_INIT_DEFAULT_PRESENT_QUEUEBit = 0x0004,
		E_GPU_DEVICE_CREATE_FLAGS_DEFAULT = E_GPU_DEVICE_CREATE_FLAG_INIT_DEFAULT_PRESENT_QUEUEBit
	};

	enum EGPUDeviceStateFlags : uint32
	{
	};

	struct GPUDeviceCreateInfo
	{
		// DisplayManager * displayManager = nullptr;
		// display_system_id_t adapterOutputID = cvDisplaySystemIDDefault;
		display_system_id_t adapterID = cvDisplaySystemIDDefault;
		TBitmask<EGPUDeviceCreateFlags> flags = E_GPU_DEVICE_CREATE_FLAGS_DEFAULT;
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

		IC3_ATTR_NO_DISCARD virtual bool isNullDevice() const noexcept;

		IC3_ATTR_NO_DISCARD bool isDebugDevice() const noexcept;

		IC3_ATTR_NO_DISCARD bool isMultiThreadAccessSupported() const noexcept;

		IC3_ATTR_NO_DISCARD bool isResourceActiveRefsTrackingEnabled() const noexcept;

		IC3_ATTR_NO_DISCARD CommandSystem & getCommandSystem() const noexcept;

		IC3_ATTR_NO_DISCARD PipelineImmutableStateFactory & getPipelineStateFactory() const noexcept;

		IC3_ATTR_NO_DISCARD PresentationLayer * getPresentationLayer() const noexcept;

		IC3_ATTR_NO_DISCARD const Math::RGBAColorU8 & getDefaultClearColor() const noexcept;

		IC3_ATTR_NO_DISCARD const RenderTargetAttachmentClearConfig & getDefaultClearConfig() const noexcept;

		template <typename TState>
		IC3_ATTR_NO_DISCARD TGPAHandle<TState> getCachedImmutableState( UniqueGPUObjectID pUniqueID ) const noexcept;

		template <typename TState>
		IC3_ATTR_NO_DISCARD TGPAHandle<TState> getCachedImmutableState( const UniqueGPUObjectName & pStateObjectName ) const noexcept;

		IC3_ATTR_NO_DISCARD GPUBufferHandle createGPUBuffer( const GPUBufferCreateInfo & pCreateInfo );
		IC3_ATTR_NO_DISCARD SamplerHandle createSampler( const SamplerCreateInfo & pCreateInfo );
		IC3_ATTR_NO_DISCARD ShaderHandle createShader( const ShaderCreateInfo & pCreateInfo );
		IC3_ATTR_NO_DISCARD TextureHandle createTexture( const TextureCreateInfo & pCreateInfo );

		/// @brief Creates an RTT using the provided CIS.
		/// This function will automatically create a required resource, depending on the specified layout and usage.
		/// It can either be an explicit texture object which can be retrieved later or an implicit render buffer
		/// (e.g. if the RTT is supposed to be only a depth/stencil attachment used for depth and/or stencil testing).
		IC3_ATTR_NO_DISCARD RenderTargetTextureHandle createRenderTargetTexture( const RenderTargetTextureCreateInfo & pCreateInfo );

		/// @brief
		IC3_ATTR_NO_DISCARD GraphicsPipelineStateObjectHandle createGraphicsPipelineStateObject( const GraphicsPipelineStateObjectCreateInfo & pCreateInfo );

		IC3_ATTR_NO_DISCARD BlendImmutableStateHandle createBlendImmutableState( const BlendConfig & pConfig );
		IC3_ATTR_NO_DISCARD DepthStencilImmutableStateHandle createDepthStencilImmutableState( const DepthStencilConfig & pConfig );
		IC3_ATTR_NO_DISCARD GraphicsShaderLinkageImmutableStateHandle createGraphicsShaderLinkageImmutableState( const GraphicsShaderSet & pShaderSet );
		IC3_ATTR_NO_DISCARD IAInputLayoutImmutableStateHandle createIAInputLayoutImmutableState( const IAInputLayoutDefinition & pDefinition, Shader & pVertexShaderWithBinary );
		IC3_ATTR_NO_DISCARD IAVertexStreamImmutableStateHandle createIAVertexStreamImmutableState( const IAVertexStreamDefinition & pDefinition );
		IC3_ATTR_NO_DISCARD RasterizerImmutableStateHandle createRasterizerImmutableState( const RasterizerConfig & pConfig );
		IC3_ATTR_NO_DISCARD RenderTargetBindingImmutableStateHandle createRenderTargetBindingImmutableState( const RenderTargetBindingDefinition & pDefinition );
		IC3_ATTR_NO_DISCARD RenderPassConfigurationImmutableStateHandle createRenderPassConfigurationImmutableState( const RenderPassConfiguration & pConfiguration );

		template <typename TState, typename TPInputDesc, typename... TArgs>
		TGPAHandle<TState> createCachedImmutableState( UniqueGPUObjectID pUniqueID, const TPInputDesc & pInputDesc, TArgs && ...pArgs );

		template <typename TState, typename TPInputDesc, typename... TArgs>
		TGPAHandle<TState> createCachedImmutableState( const UniqueGPUObjectName & pUniqueName, const TPInputDesc & pInputDesc, TArgs && ...pArgs );

		void resetImmutableStateCache( TBitmask<EPipelineImmutableStateTypeFlags> pResetMask = ePipelineImmutableStateTypeMaskAll );

		void setPresentationLayer( PresentationLayerHandle pPresentationLayer );

		virtual void waitForCommandSync( CommandSync & pCommandSync ) = 0;

		IC3_ATTR_NO_DISCARD static GPUDevice & nullDevice();

	protected:
		virtual bool onGPUResourceActiveRefsZero( GPUResource & pGPUResource );

		void setImmutableStateCache( PipelineImmutableStateCache & pStateCache );

	private:
		/// @brief API-level initialization of the command system. Called by the parent driver when a device is created.
		virtual void initializeCommandSystem() = 0;

	ic3driverApi( private ):
		virtual bool _drvOnSetPresentationLayer( PresentationLayerHandle pPresentationLayer );

		virtual GPUBufferHandle _drvCreateGPUBuffer( const GPUBufferCreateInfo & pCreateInfo );
		virtual SamplerHandle _drvCreateSampler( const SamplerCreateInfo & pCreateInfo );
		virtual ShaderHandle _drvCreateShader( const ShaderCreateInfo & pCreateInfo );
		virtual TextureHandle _drvCreateTexture( const TextureCreateInfo & pCreateInfo );
		virtual RenderTargetTextureHandle _drvCreateRenderTargetTexture( const RenderTargetTextureCreateInfo & pCreateInfo );

		virtual GraphicsPipelineStateObjectHandle _drvCreateGraphicsPipelineStateObject(
				const GraphicsPipelineStateObjectCreateInfo & pCreateInfo );

	protected:
		CommandSystemHandle _commandSystem;
		PresentationLayerHandle _presentationLayer;

		/// Factory used to create immutable states. Set by the actual driver class during initialization.
		/// This decouples the state creation from the GPUDevice class so it's easier to manage and extend.
		PipelineImmutableStateFactory * _immutableStateFactoryBase = nullptr;

		/// Immutable state cache. Holds created states and enables re-using them across all APIs.
		/// Requires PipelineImmutableStateFactory to be specified when created.
		PipelineImmutableStateCache * _immutableStateCachePtr = nullptr;

		TBitmask<uint32> _internalStateFlags;
	};

	template <typename TState>
	inline TGPAHandle<TState> GPUDevice::getCachedImmutableState( UniqueGPUObjectID pUniqueID ) const noexcept
	{
		return _immutableStateCachePtr->template getState<TState>( pUniqueID );
	}

	template <typename TState>
	inline TGPAHandle<TState> GPUDevice::getCachedImmutableState( const UniqueGPUObjectName & pUniqueName ) const noexcept
	{
		return _immutableStateCachePtr->template getState<TState>( pUniqueName );
	}

	template <typename TState, typename TPInputDesc, typename... TArgs>
	inline TGPAHandle<TState> GPUDevice::createCachedImmutableState( UniqueGPUObjectID pUniqueID, const TPInputDesc & pInputDesc, TArgs && ...pArgs )
	{
		return _immutableStateCachePtr->template createState<TState>( pUniqueID, pInputDesc, std::forward<TArgs>( pArgs )... );
	}

	template <typename TState, typename TPInputDesc, typename... TArgs>
	inline TGPAHandle<TState> GPUDevice::createCachedImmutableState( const UniqueGPUObjectName & pUniqueName, const TPInputDesc & pInputDesc, TArgs && ...pArgs )
	{
		return _immutableStateCachePtr->template createState<TState>( pUniqueName, pInputDesc, std::forward<TArgs>( pArgs )... );
	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_GPU_DEVICE_H__
