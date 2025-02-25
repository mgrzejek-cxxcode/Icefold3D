
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_GLC_GPU_DEVICE_H__
#define __IC3_GRAPHICS_HW3D_GLC_GPU_DEVICE_H__

#include "GLAPITranslationLayer.h"
#include "State/GLPipelineStateDescriptorFactory.h"
#include <Ic3/Graphics/GCI/GPUDevice.h>

namespace Ic3::Graphics::GCI
{

	class GLCommandContext;
	class GLGraphicsPipelineStateController;
	class GLGraphicsPipelineStateObject;
	class GLVertexStreamStateObject;

	class GLGPUDeviceFeatureQuery : public GPUDeviceFeatureQuery
	{
	public:
		GLGPUDeviceFeatureQuery() = default;
		virtual ~GLGPUDeviceFeatureQuery() = default;

		virtual MultiSamplingSettingsList EnumSupportedMultisamplingConfigs( ETextureFormat pFormat ) const noexcept override final;
	};

	/// @brief
	class GLGPUDevice : public GPUDevice
	{
		friend class GLCommandContext;
		friend class GLCommandList;
		friend class GLGraphicsPipelineStateObject;

	public:
		System::OpenGLSystemDriverHandle const mSysGLDriver;

		System::OpenGLVersionSupportInfo const mSysGLSupportInfo;

		cppx::bitmask<EGLRuntimeSupportFlags> const mGLRuntimeSupportFlags;

	public:
		explicit GLGPUDevice( GLGPUDriver & pGPUDriver, GLPipelineStateDescriptorFactory & pCompiledDescriptorFactory );
		virtual ~GLGPUDevice();

		CPPX_ATTR_NO_DISCARD virtual bool IsCompatibilityDevice() const noexcept = 0;

		GLDebugOutput * GetDebugOutputInterface() const;

		virtual void WaitForCommandSync( CommandSync & pCommandSync ) override;

	protected:
		bool InitializeGLDebugOutput();

		virtual void InitializeCommandSystem() override;

	private:
	    virtual bool _DrvOnSetPresentationLayer( PresentationLayerHandle pPresentationLayer ) override;

		virtual GPUBufferHandle _DrvCreateGPUBuffer( const GPUBufferCreateInfo & pCreateInfo ) override final;

	    virtual SamplerHandle _DrvCreateSampler( const SamplerCreateInfo & pCreateInfo ) override final;

		virtual ShaderHandle _DrvCreateShader( const ShaderCreateInfo & pCreateInfo ) override final;

		virtual TextureHandle _DrvCreateTexture( const TextureCreateInfo & pCreateInfo ) override final;

		virtual RenderTargetTextureHandle _DrvCreateRenderTargetTexture(
				const RenderTargetTextureCreateInfo & pCreateInfo ) override final;

		virtual GraphicsPipelineStateObjectHandle _DrvCreateGraphicsPipelineStateObject(
				const GraphicsPipelineStateObjectCreateInfo & pCreateInfo ) override final;

	private:
		GLPipelineStateDescriptorFactory * _glcPipelineStateDescriptorFactory;
		GLGPUDeviceFeatureQuery _glcDeviceFeatureQueryInterface;
		std::unique_ptr<GLDebugOutput> _glcDebugOutput;
	};

	class GLGPUDeviceCore : public GLGPUDevice
	{
	public:
		explicit GLGPUDeviceCore( GLGPUDriver & pGPUDriver );
		virtual ~GLGPUDeviceCore();

		virtual bool IsCompatibilityDevice() const noexcept override final;

	private:
		GLPipelineStateDescriptorFactoryCore _immutableDescriptorFactoryCore;
	};

	class GLGPUDeviceCompat : public GLGPUDevice
	{
	public:
		explicit GLGPUDeviceCompat( GLGPUDriver & pGPUDriver );
		virtual ~GLGPUDeviceCompat();

		virtual bool IsCompatibilityDevice() const noexcept override final;

	private:
		GLPipelineStateDescriptorFactoryCompat _immutableDescriptorFactoryCompat;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_GLC_GPU_DEVICE_H__
