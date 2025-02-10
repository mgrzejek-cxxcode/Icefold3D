
#pragma once

#ifndef __IC3_GRAPHICS_GCI_GPU_DEVICE_H__
#define __IC3_GRAPHICS_GCI_GPU_DEVICE_H__

#include "CommonCommandDefs.h"
#include "DisplayCommon.h"
#include "Resources/TextureCommon.h"
#include "State/GraphicsShaderDefs.h"
#include "State/InputAssemblerCommon.h"
#include "State/RenderPassCommon.h"
#include "State/PipelineStateDescriptorCacheUnit.h"

namespace Ic3::Graphics::GCI
{

	struct PresentationLayerCreateInfo;
	struct RenderTargetTextureCreateInfo;

	class PipelineStateDescriptorManager;

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

		CPPX_ATTR_NO_DISCARD PresentationLayer * GetPresentationLayer() const noexcept;

		CPPX_ATTR_NO_DISCARD const Math::RGBAColorU8 & GetDefaultClearColor() const noexcept;

		CPPX_ATTR_NO_DISCARD const RenderTargetAttachmentClearConfig & GetDefaultClearConfig() const noexcept;

		template <typename TPStateDescriptor>
		CPPX_ATTR_NO_DISCARD TGfxHandle<TPStateDescriptor> GetCachedStateDescriptor( pipeline_state_descriptor_id_t pDescriptorID ) const noexcept;

		CPPX_ATTR_NO_DISCARD TGfxHandle<PipelineStateDescriptor> GetCachedStateDescriptor(
				EPipelineStateDescriptorType pDescriptorType,
				pipeline_state_descriptor_id_t pDescriptorID ) const noexcept;

		template <typename TPStateDescriptor>
		CPPX_ATTR_NO_DISCARD TGfxHandle<TPStateDescriptor> GetCachedStateDescriptor( const GfxObjectName & pStateObjectName ) const noexcept;

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
		CPPX_ATTR_NO_DISCARD GraphicsPipelineStateObjectHandle CreateGraphicsPipelineStateObject(
				const GraphicsPipelineStateObjectCreateInfo & pCreateInfo );

		CPPX_ATTR_NO_DISCARD BlendStateDescriptorHandle CreateBlendStateDescriptor(
				const BlendStateDescriptorCreateInfo & pCreateInfo );

		CPPX_ATTR_NO_DISCARD DepthStencilStateDescriptorHandle CreateDepthStencilStateDescriptor(
				const DepthStencilStateDescriptorCreateInfo & pCreateInfo );

		CPPX_ATTR_NO_DISCARD RasterizerStateDescriptorHandle CreateRasterizerStateDescriptor(
				const RasterizerStateDescriptorCreateInfo & pCreateInfo );

		CPPX_ATTR_NO_DISCARD GraphicsShaderLinkageStateDescriptorHandle CreateGraphicsShaderLinkageStateDescriptor(
				const GraphicsShaderLinkageStateDescriptorCreateInfo & pCreateInfo );

		CPPX_ATTR_NO_DISCARD IAVertexAttributeLayoutStateDescriptorHandle CreateIAVertexAttributeLayoutStateDescriptor(
				const IAVertexAttributeLayoutStateDescriptorCreateInfo & pCreateInfo );

		CPPX_ATTR_NO_DISCARD ShaderRootSignatureStateDescriptorHandle CreateShaderRootSignatureStateDescriptor(
				const ShaderRootSignatureStateDescriptorCreateInfo & pCreateInfo );

		CPPX_ATTR_NO_DISCARD IAVertexStreamBindingStateDescriptorHandle CreateIAVertexStreamBindingStateDescriptor(
				const IAVertexStreamBindingStateDescriptorCreateInfo & pCreateInfo );

		CPPX_ATTR_NO_DISCARD RenderPassConfigurationStateDescriptorHandle CreateRenderPassConfigurationStateDescriptor(
				const RenderPassConfigurationStateDescriptorCreateInfo & pCreateInfo );

		template <typename TPStateDescriptor, typename TPCreateInfo>
		TGfxHandle<TPStateDescriptor> CreateCachedStateDescriptor(
				const TPCreateInfo & pCreateInfo,
				cppx::immutable_string pOptionalName = {} );

		void ResetStateDescriptorCache( cppx::bitmask<EPipelineStateDescriptorTypeFlags> pResetMask = ePipelineStateDescriptorTypeMaskAll );

		void SetPresentationLayer( PresentationLayerHandle pPresentationLayer );

		virtual void WaitForCommandSync( CommandSync & pCommandSync ) = 0;

		CPPX_ATTR_NO_DISCARD static GPUDevice & GetNullDevice();

	protected:
		virtual bool OnGPUResourceActiveRefsZero( GPUResource & pGPUResource );

		bool InitializePipelineStateDescriptorManager( PipelineStateDescriptorFactory & pDescriptorFactory );

	private:
		/// @brief API-level initialization of the command system. Called by the parent driver when a device is created.
		virtual void InitializeCommandSystem() = 0;

	_Ic3DriverAPI( private ):
		virtual bool _DrvOnSetPresentationLayer( PresentationLayerHandle pPresentationLayer );

		virtual GPUBufferHandle _DrvCreateGPUBuffer( const GPUBufferCreateInfo & pCreateInfo );

		virtual SamplerHandle _DrvCreateSampler( const SamplerCreateInfo & pCreateInfo );

		virtual ShaderHandle _DrvCreateShader( const ShaderCreateInfo & pCreateInfo );

		virtual TextureHandle _DrvCreateTexture( const TextureCreateInfo & pCreateInfo );

		virtual RenderTargetTextureHandle _DrvCreateRenderTargetTexture(
				const RenderTargetTextureCreateInfo & pCreateInfo );

		virtual GraphicsPipelineStateObjectHandle _DrvCreateGraphicsPipelineStateObject(
				const GraphicsPipelineStateObjectCreateInfo & pCreateInfo );

	protected:
		///
		CommandSystemHandle _commandSystem;

		///
		PresentationLayerHandle _presentationLayer;

		/// Factory used to Create immutable states. Set by the actual driver class during initialization.
		/// This decouples the state creation from the GPUDevice class so it's easier to manage and extend.
		PipelineStateDescriptorFactory * _pipelineStateDescriptorFactoryBase = nullptr;

		///
		std::unique_ptr<PipelineStateDescriptorManager> _pipelineStateDescriptorManager;

		cppx::bitmask<uint32> _internalStateFlags;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_GPU_DEVICE_H__
