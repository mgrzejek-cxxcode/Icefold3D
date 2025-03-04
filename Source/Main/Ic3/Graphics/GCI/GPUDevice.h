
#pragma once

#ifndef __IC3_GRAPHICS_GCI_GPU_DEVICE_H__
#define __IC3_GRAPHICS_GCI_GPU_DEVICE_H__

#include "CommonCommandDefs.h"
#include "DisplayCommon.h"
#include "GPUDeviceFeatureQuery.h"
#include "Resources/TextureCommon.h"
#include "State/GraphicsShaderDefs.h"
#include "State/InputAssemblerCommon.h"
#include "State/RenderPassCommon.h"
#include "State/GraphicsPipelineStateCommon.h"
#include "State/PipelineStateDescriptorCacheUnit.h"

namespace Ic3::Graphics::GCI
{

	struct PresentationLayerCreateInfo;
	struct RenderTargetTextureCreateInfo;

	class PipelineStateDescriptorManager;

	enum EGPUDeviceCreateFlags : uint32
	{
		eGPUDeviceCreateFlagInitDefaultPresentQueueBit = 0x0004,
		eGPUDeviceCreateFlagsDefault = eGPUDeviceCreateFlagInitDefaultPresentQueueBit
	};

	enum EGPUDeviceStateFlags : uint32
	{
	};

	struct GPUDeviceCreateInfo
	{
		// DisplayManager * displayManager = nullptr;
		// display_system_id_t adapterOutputID = cvDisplaySystemIDDefault;
		display_system_id_t adapterID = cvDisplaySystemIDDefault;
		cppx::bitmask<EGPUDeviceCreateFlags> flags = eGPUDeviceCreateFlagsDefault;
	};

	class IC3_GRAPHICS_GCI_CLASS GPUDevice : public GPUDriverChildObject
	{
		friend class GPUDriver;
		friend class GPUResource;

	public:
		/// Saved ID of the parent driver, enables easy checking what driver runtime this device is created for.
		EGPUDriverID const mGPUDriverID;

		/// Handle to a system context, enabling interaction with the system layer.
		System::SysContextHandle const mSysContext;

		/// A pointer to the feature query interface, executing queries about the device runtime capabilities and metrics.
		GPUDeviceFeatureQuery * const mFeatureQueryInterface;

		/// A pointer to the descriptor state manager, responsible for creating and caching all descriptors.
		PipelineStateDescriptorManager * const mPipelineStateDescriptorManager;

		GPUDevice(
				GPUDriver & pDriver,
				GPUDeviceFeatureQuery * pFeatureQueryInterface,
				PipelineStateDescriptorManager * pPipelineStateDescriptorManager );

		virtual ~GPUDevice();

		virtual void WaitForCommandSync( CommandSync & pCommandSync ) = 0;

		CPPX_ATTR_NO_DISCARD virtual bool IsNullDevice() const noexcept;

		CPPX_ATTR_NO_DISCARD bool IsDebugDevice() const noexcept;

		CPPX_ATTR_NO_DISCARD bool IsMultiThreadAccessSupported() const noexcept;

		CPPX_ATTR_NO_DISCARD bool IsResourceActiveRefsTrackingEnabled() const noexcept;

		CPPX_ATTR_NO_DISCARD CommandSystem & GetCommandSystem() const noexcept;

		CPPX_ATTR_NO_DISCARD PresentationLayer * GetPresentationLayer() const noexcept;

		CPPX_ATTR_NO_DISCARD const Math::RGBAColorU8 & GetDefaultClearColor() const noexcept;

		CPPX_ATTR_NO_DISCARD const RenderTargetAttachmentClearConfig & GetDefaultClearConfig() const noexcept;

		CPPX_ATTR_NO_DISCARD const BlendSettings & GetDefaultBlendSettings();
		CPPX_ATTR_NO_DISCARD const DepthStencilSettings & GetDefaultDepthStencilSettings();
		CPPX_ATTR_NO_DISCARD const DepthStencilSettings & GetDefaultDepthStencilSettingsWithDepthTestEnabled();
		CPPX_ATTR_NO_DISCARD const RasterizerSettings & GetDefaultRasterizerSettings();

		template <typename TPStateDescriptor>
		CPPX_ATTR_NO_DISCARD TGfxHandle<TPStateDescriptor> GetCachedStateDescriptor(
				pipeline_state_descriptor_id_t pDescriptorID ) const noexcept;

		template <typename TPStateDescriptor>
		CPPX_ATTR_NO_DISCARD TGfxHandle<TPStateDescriptor> GetCachedStateDescriptor(
				const GfxObjectName & pStateObjectName ) const noexcept;

		CPPX_ATTR_NO_DISCARD TGfxHandle<PipelineStateDescriptor> GetCachedStateDescriptor(
				EPipelineStateDescriptorType pDescriptorType,
				pipeline_state_descriptor_id_t pDescriptorID ) const noexcept;

		CPPX_ATTR_NO_DISCARD TGfxHandle<PipelineStateDescriptor> GetCachedStateDescriptor(
				EPipelineStateDescriptorType pDescriptorType,
				const GfxObjectName & pStateObjectName ) const noexcept;

		/**
		 *
		 * @param pCreateInfo
		 * @return
		 */
		CPPX_ATTR_NO_DISCARD GPUBufferHandle CreateGPUBuffer( const GPUBufferCreateInfo & pCreateInfo );

		/**
		 *
		 * @param pCreateInfo
		 * @return
		 */
		CPPX_ATTR_NO_DISCARD SamplerHandle CreateSampler( const SamplerCreateInfo & pCreateInfo );

		/**
		 *
		 * @param pCreateInfo
		 * @return
		 */
		CPPX_ATTR_NO_DISCARD ShaderHandle CreateShader( const ShaderCreateInfo & pCreateInfo );

		/**
		 *
		 * @param pCreateInfo
		 * @return
		 */
		CPPX_ATTR_NO_DISCARD TextureHandle CreateTexture( const TextureCreateInfo & pCreateInfo );

		/**
		 * @brief Creates an RTT using the provided CreateInfo object.
		 * This function will automatically Create a required resource, depending on the specified layout and usage.
		 * It can either be an explicit texture object which can be retrieved later or an implicit render buffer
		 * (e.g. if the RTT is supposed to be only a depth/stencil attachment used for depth and/or stencil testing).
		 * @param pCreateInfo
		 * @return
		 */
		CPPX_ATTR_NO_DISCARD RenderTargetTextureHandle CreateRenderTargetTexture( const RenderTargetTextureCreateInfo & pCreateInfo );

		/**
		 *
		 * @param pCreateInfo
		 * @return
		 */
		CPPX_ATTR_NO_DISCARD GraphicsPipelineStateObjectHandle CreateGraphicsPipelineStateObject(
				const GraphicsPipelineStateObjectCreateInfo & pCreateInfo );

		/**
		 *
		 * @param pCreateInfo
		 * @param pOptionalDescriptorName
		 * @return
		 */
		CPPX_ATTR_NO_DISCARD BlendStateDescriptorHandle CreateBlendStateDescriptor(
				const BlendStateDescriptorCreateInfo & pCreateInfo,
				const cppx::immutable_string & pOptionalDescriptorName = {} );

		/**
		 *
		 * @param pCreateInfo
		 * @param pOptionalDescriptorName
		 * @return
		 */
		CPPX_ATTR_NO_DISCARD DepthStencilStateDescriptorHandle CreateDepthStencilStateDescriptor(
				const DepthStencilStateDescriptorCreateInfo & pCreateInfo,
				const cppx::immutable_string & pOptionalDescriptorName = {} );

		/**
		 *
		 * @param pCreateInfo
		 * @param pOptionalDescriptorName
		 * @return
		 */
		CPPX_ATTR_NO_DISCARD RasterizerStateDescriptorHandle CreateRasterizerStateDescriptor(
				const RasterizerStateDescriptorCreateInfo & pCreateInfo,
				const cppx::immutable_string & pOptionalDescriptorName = {} );

		/**
		 *
		 * @param pCreateInfo
		 * @param pOptionalDescriptorName
		 * @return
		 */
		CPPX_ATTR_NO_DISCARD GraphicsShaderLinkageDescriptorHandle CreateGraphicsShaderLinkageDescriptor(
				const GraphicsShaderLinkageDescriptorCreateInfo & pCreateInfo,
				const cppx::immutable_string & pOptionalDescriptorName = {} );

		/**
		 *
		 * @param pCreateInfo
		 * @param pOptionalDescriptorName
		 * @return
		 */
		CPPX_ATTR_NO_DISCARD VertexAttributeLayoutDescriptorHandle CreateVertexAttributeLayoutDescriptor(
				const VertexAttributeLayoutDescriptorCreateInfo & pCreateInfo,
				const cppx::immutable_string & pOptionalDescriptorName = {} );

		/**
		 *
		 * @param pCreateInfo
		 * @param pOptionalDescriptorName
		 * @return
		 */
		CPPX_ATTR_NO_DISCARD RootSignatureDescriptorHandle CreateRootSignatureDescriptor(
				const RootSignatureDescriptorCreateInfo & pCreateInfo,
				const cppx::immutable_string & pOptionalDescriptorName = {} );

		/**
		 *
		 * @param pCreateInfo
		 * @return
		 */
		CPPX_ATTR_NO_DISCARD RenderPassDescriptorHandle CreateRenderPassDescriptor(
				const RenderPassDescriptorCreateInfo & pCreateInfo );

		/**
		 *
		 * @param pCreateInfo
		 * @return
		 */
		CPPX_ATTR_NO_DISCARD RenderTargetDescriptorHandle CreateRenderTargetDescriptor(
				const RenderTargetDescriptorCreateInfo & pCreateInfo );

		/**
		 *
		 * @param pCreateInfo
		 * @return
		 */
		CPPX_ATTR_NO_DISCARD VertexSourceBindingDescriptorHandle CreateVertexSourceBindingDescriptor(
				const VertexSourceBindingDescriptorCreateInfo & pCreateInfo );

		/**
		 *
		 * @tparam TContext
		 * @return
		 */
		template <typename TContext>
		std::unique_ptr<TContext> AcquireCommandContext();

		/**
		 *
		 * @param pContextType
		 * @return
		 */
		std::unique_ptr<CommandContext> AcquireCommandContext( ECommandContextType pContextType );

		/**
		 *
		 * @param pPresentationLayer
		 */
		void SetPresentationLayer( PresentationLayerHandle pPresentationLayer );

		/**
		 *
		 * @return
		 */
		CPPX_ATTR_NO_DISCARD static GPUDevice & GetNullDevice();

	protected:
		virtual bool OnGPUResourceActiveRefsZero( GPUResource & pGPUResource );

	private:
		/**
		 * @brief API-level initialization of the command system. Called by the parent driver when a device is created.
		 */
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

		///
		cppx::bitmask<uint32> _internalStateFlags;
	};

	template <typename TContext>
	inline std::unique_ptr<TContext> GPUDevice::AcquireCommandContext()
	{
		return MoveInterfaceUniquePtr<TContext>( AcquireCommandContext( TContext::sContextType ) );
	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_GPU_DEVICE_H__
