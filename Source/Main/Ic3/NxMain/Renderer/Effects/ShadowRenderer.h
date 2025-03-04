
#pragma once

#ifndef __IC3_NXMAIN_SHADOW_RENDERER_H__
#define __IC3_NXMAIN_SHADOW_RENDERER_H__

#include "../commonRendererDefs.h"

#include <Ic3/Graphics/GCI/State/GraphicsPipelineStateDescriptorRTO.h>

namespace Ic3
{

	struct CBShadowData
	{
		Math::Mat4f m4fLightSpaceMatrix;
		Math::Vec4f v4fShadowProperties;
	};

	struct ShadowConfig
	{
		GCI::TextureSize2D screenSize;
		GCI::TextureSize2D shadowMapSize;
	};

	class ShadowRenderer
	{
	public:
		struct CurrentState
		{
			Math::Vec3f vLightPosition;
			Math::Vec3f vLightTarget;
			Math::Mat4f mLightView;
			Math::Mat4f mLightProjection;
			Math::Mat4f mLightSpace;
		};

		struct GPUAPIState
		{
			GCI::RenderTargetBindingDynamicState rtBindingPass1Light;
			GCI::GraphicsPipelineStateObjectHandle psoPass1Light;
			GCI::GraphicsPipelineStateObjectHandle psoPass2Shadow;
			GCI::RenderPassConfigurationCompiledStateHandle renderPass1Light;
			GCI::RenderPassConfigurationCompiledStateHandle renderPass2Shadow;
			GCI::SamplerHandle samplerPass2Shadow;
		};

		struct Resources
		{
			GCI::GPUBufferHandle constantBuffer;
			GCI::TextureHandle shadowMapTexture;
			GCI::RenderTargetTextureHandle shadowMapRTT;
		};

	public:
		ShadowRenderer( ShaderLibrary & pShaderLibrary, const ShadowConfig & pShadowConfig );
		virtual ~ShadowRenderer();

		virtual void createRendererResources();

		const Math::Mat4f & getLightProjectionMatrix() const
		{
			return _currentState.mLightProjection;
		}

		const Math::Mat4f & getLightViewMatrix() const
		{
			return _currentState.mLightView;
		}

		const Math::Mat4f & getLightSpaceMatrix() const
		{
			return _currentState.mLightSpace;
		}

		const Math::Vec3f & getLightPosition() const
		{
			return _currentState.vLightPosition;
		}

		void setCSLightPosition( Math::Vec3f pLightPosition );
		void setCSLightTarget( Math::Vec3f pLightTarget );

		void setCSProjectionMatrixLightOrthoDefault();
		void setCSProjectionMatrixLightPerspectiveDefault();

		void updateMatricesForLightPass( GCI::CommandContext & pCommandContext );

		void updateMatricesForShadowPass( GCI::CommandContext & pCommandContext );

		void beginRenderPass1Light( GCI::CommandContext & pCommandContext );

		void beginRenderPass2Shadow( GCI::CommandContext & pCommandContext );

		void endRenderPass( GCI::CommandContext & pCommandContext );

	private:
		void initializeResources();

		void initializeRenderPassStates();

		void initializePipelineStateObjects();

	protected:
		GCI::GPUDevice & _gpuDevice;
		ShaderLibrary & _shaderLibrary;
		ShadowConfig _shadowConfig;
		GPUAPIState _gpuAPIState;
		Resources _resources;
		CurrentState _currentState;
	};

//	class ShadowRenderer0 : public ShadowRenderer
//	{
//	public:
//		ShadowRenderer0( ShaderLibraryHandle pShaderLibrary );
//		virtual ~ShadowRenderer0();
//
//	};


} // namespace Ic3

#endif // __IC3_NXMAIN_SHADOW_RENDERER_H__
