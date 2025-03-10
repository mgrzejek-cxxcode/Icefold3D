
#pragma once

#ifndef __IC3_NXMAIN_SHADOW_RENDERER_H__
#define __IC3_NXMAIN_SHADOW_RENDERER_H__

#include "../commonRendererDefs.h"

#include <Ic3/Graphics/GCI/State/GraphicsPipelineStateDescriptorRTO.h>

namespace Ic3
{

	struct CBShadowData
	{
		cxm::mat4f m4fLightSpaceMatrix;
		cxm::vec4f v4fShadowProperties;
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
			cxm::vec3f vLightPosition;
			cxm::vec3f vLightTarget;
			cxm::mat4f mLightView;
			cxm::mat4f mLightProjection;
			cxm::mat4f mLightSpace;
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

		const cxm::mat4f & getLightProjectionMatrix() const
		{
			return _currentState.mLightProjection;
		}

		const cxm::mat4f & getLightViewMatrix() const
		{
			return _currentState.mLightView;
		}

		const cxm::mat4f & getLightSpaceMatrix() const
		{
			return _currentState.mLightSpace;
		}

		const cxm::vec3f & getLightPosition() const
		{
			return _currentState.vLightPosition;
		}

		void setCSLightPosition( cxm::vec3f pLightPosition );
		void setCSLightTarget( cxm::vec3f pLightTarget );

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
