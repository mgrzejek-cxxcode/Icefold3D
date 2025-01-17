
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_GLCOMMON_GPU_DEVICE_H__
#define __IC3_GRAPHICS_HW3D_GLCOMMON_GPU_DEVICE_H__

#include "GLApiTranslationLayer.h"
#include "State/GLPipelineImmutableStateFactory.h"
#include <Ic3/Graphics/GCI/GpuDevice.h>
#include <Ic3/Graphics/GCI/State/PipelineImmutableStateCache.h>

namespace Ic3::Graphics::GCI
{

	class GLCommandContext;
	class GLGraphicsPipelineStateController;
	class GLGraphicsPipelineStateObject;
	class GLVertexStreamStateObject;

	/// @brief
	class GLGpuDevice : public GpuDevice
	{
		friend class GLCommandContext;
		friend class GLCommandList;
		friend class GLGraphicsPipelineStateObject;

	public:
		System::OpenGLSystemDriverHandle const mSysGLDriver;

		System::OpenGLVersionSupportInfo const mSysGLSupportInfo;

		cppx::bitmask<EGLRuntimeSupportFlags> const mGLRuntimeSupportFlags;

	public:
		explicit GLGpuDevice( GLGpuDriver & pGpuDriver, GLPipelineImmutableStateFactory & pImmutableStateFactory );
		virtual ~GLGpuDevice();

		CPPX_ATTR_NO_DISCARD virtual bool IsCompatibilityDevice() const noexcept = 0;

		GLDebugOutput * GetDebugOutputInterface() const;

		virtual void WaitForCommandSync( CommandSync & pCommandSync ) override;

	protected:
		bool InitializeGLDebugOutput();

		virtual void InitializeCommandSystem() override;

	private:
	    virtual bool _DrvOnSetPresentationLayer( PresentationLayerHandle pPresentationLayer ) override;

		virtual GpuBufferHandle _DrvCreateGpuBuffer( const GpuBufferCreateInfo & pCreateInfo ) override final;
	    virtual SamplerHandle _DrvCreateSampler( const SamplerCreateInfo & pCreateInfo ) override final;
		virtual ShaderHandle _DrvCreateShader( const ShaderCreateInfo & pCreateInfo ) override final;
		virtual TextureHandle _DrvCreateTexture( const TextureCreateInfo & pCreateInfo ) override final;

		virtual RenderTargetTextureHandle _DrvCreateRenderTargetTexture(
				const RenderTargetTextureCreateInfo & pCreateInfo ) override final;

		virtual GraphicsPipelineStateObjectHandle _DrvCreateGraphicsPipelineStateObject(
				const GraphicsPipelineStateObjectCreateInfo & pCreateInfo ) override final;

	private:
		GLPipelineImmutableStateFactory * _immutableStateFactoryGL;
		PipelineImmutableStateCache _immutableStateCache;
		std::unique_ptr<GLDebugOutput> _glDebugOutput;
	};

	class GLGpuDeviceCore : public GLGpuDevice
	{
	public:
		explicit GLGpuDeviceCore( GLGpuDriver & pGpuDriver );
		virtual ~GLGpuDeviceCore();

		virtual bool IsCompatibilityDevice() const noexcept override final;

	private:
		GLPipelineImmutableStateFactoryCore _immutableStateFactoryCore;
	};

	class GLGpuDeviceCompat : public GLGpuDevice
	{
	public:
		explicit GLGpuDeviceCompat( GLGpuDriver & pGpuDriver );
		virtual ~GLGpuDeviceCompat();

		virtual bool IsCompatibilityDevice() const noexcept override final;

	private:
		GLPipelineImmutableStateFactoryCompat _immutableStateFactoryCompat;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_GLCOMMON_GPU_DEVICE_H__
