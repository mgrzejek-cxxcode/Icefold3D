
#include "ShadowRenderer.h"
#include "../shaderLibrary.h"

#include <Ic3/Graphics/GCI/CommandContext.h>
#include <Ic3/Graphics/GCI/GPUDevice.h>
#include <Ic3/Graphics/GCI/Resources/RenderTargetTexture.h>
#include <Ic3/Graphics/GCI/Resources/Texture.h>
#include <Ic3/Graphics/GCI/State/GraphicsPipelineStateCommon.h>
#include <Ic3/Graphics/GCI/State/Sampler.h>

namespace Ic3
{

	ShadowRenderer::ShadowRenderer( ShaderLibrary & pShaderLibrary, const ShadowConfig & pShadowConfig )
	: _gpuDevice( *pShaderLibrary.mCES.mGPUDevice )
	, _shaderLibrary( pShaderLibrary )
	, _shadowConfig( pShadowConfig )
	{
		setCSLightPosition( { -2.0f, 3.0f, -2.0f } );
		setCSLightTarget( { 0.0f, 0.0f,  5.0f } );
	}

	ShadowRenderer::~ShadowRenderer() = default;

	void ShadowRenderer::createRendererResources()
	{
		initializeResources();
		initializeRenderPassStates();
		initializePipelineStateObjects();
	}

	void ShadowRenderer::setCSLightPosition( Math::Vec3f pLightPosition )
	{
		_currentState.vLightPosition = pLightPosition;
	}

	void ShadowRenderer::setCSLightTarget( Math::Vec3f pLightTarget )
	{
		_currentState.vLightTarget = pLightTarget;
	}

	void ShadowRenderer::setCSProjectionMatrixLightOrthoDefault()
	{
		_currentState.mLightProjection = Math::orthoOffCenterLH( -32.0f, 32.0f, -32.0f, 32.0f, 1.0f, 64.0f );
	}

	void ShadowRenderer::setCSProjectionMatrixLightPerspectiveDefault()
	{
		_currentState.mLightProjection = Math::perspectiveAspectLH( Math::Constants::FLT_RAD_90DEG, 1.0f, 1.0f, 24.0f );
	}

	void ShadowRenderer::updateMatricesForLightPass( GCI::CommandContext & pCommandContext )
	{
		using namespace Ic3::Graphics::GCI;

		Ic3DebugAssert( pCommandContext.CheckFeatureSupport( ECommandObjectPropertyMaskContextFamilyDirectGraphics ) );
		auto * directGraphicsContext = pCommandContext.QueryInterface<CommandContextDirectGraphics>();

		_currentState.mLightView = Math::lookAtLH( _currentState.vLightPosition, _currentState.vLightTarget, { 0.0f, 1.0f, 0.0f } );
		_currentState.mLightSpace = Math::mul( _currentState.mLightProjection, _currentState.mLightView );

		CBShadowData cbShadowData;
		cbShadowData.m4fLightSpaceMatrix = _currentState.mLightSpace;
		cbShadowData.v4fShadowProperties.x = _shadowConfig.shadowMapSize.width;
		cbShadowData.v4fShadowProperties.y = _shadowConfig.shadowMapSize.height;
		cbShadowData.v4fShadowProperties.z = _shadowConfig.screenSize.width;
		cbShadowData.v4fShadowProperties.w = _shadowConfig.screenSize.height;

		directGraphicsContext->UpdateBufferDataUpload( *_resources.constantBuffer, cbShadowData );
	}

	void ShadowRenderer::updateMatricesForShadowPass( GCI::CommandContext & pCommandContext )
	{
		using namespace Ic3::Graphics::GCI;

		Ic3DebugAssert( pCommandContext.CheckFeatureSupport( ECommandObjectPropertyMaskContextFamilyDirectGraphics ) );
		auto * directGraphicsContext = pCommandContext.QueryInterface<CommandContextDirectGraphics>();

		CBShadowData cbShadowData;
		cbShadowData.m4fLightSpaceMatrix = _currentState.mLightSpace;
		cbShadowData.v4fShadowProperties.x = _shadowConfig.shadowMapSize.width;
		cbShadowData.v4fShadowProperties.y = _shadowConfig.shadowMapSize.height;
		cbShadowData.v4fShadowProperties.z = _shadowConfig.screenSize.width;
		cbShadowData.v4fShadowProperties.w = _shadowConfig.screenSize.height;

		directGraphicsContext->UpdateBufferDataUpload( *_resources.constantBuffer, cbShadowData );
	}

	void ShadowRenderer::beginRenderPass1Light( GCI::CommandContext & pCommandContext )
	{
		using namespace Ic3::Graphics::GCI;

		Ic3DebugAssert( pCommandContext.CheckFeatureSupport( ECommandObjectPropertyMaskContextFamilyDirectGraphics ) );
		auto * directGraphicsContext = pCommandContext.QueryInterface<CommandContextDirectGraphics>();

		GCI::ViewportDesc viewportDescLight;
		viewportDescLight.origin.x = 0;
		viewportDescLight.origin.y = 0;
		viewportDescLight.size.x = _shadowConfig.shadowMapSize.width;
		viewportDescLight.size.y = _shadowConfig.shadowMapSize.height;
		viewportDescLight.depthRange.zNear = 0.0f;
		viewportDescLight.depthRange.zFar = 1.0f;

		directGraphicsContext->SetRenderTargetBindingState( _gpuAPIState.rtBindingPass1Light );
		directGraphicsContext->SetGraphicsPipelineStateObject( *_gpuAPIState.psoPass1Light );

		directGraphicsContext->BeginRenderPass( *_gpuAPIState.renderPass1Light );

		directGraphicsContext->CmdSetViewport( viewportDescLight );
		directGraphicsContext->CmdSetShaderConstantBuffer( 17, *_resources.constantBuffer );
	}

	void ShadowRenderer::beginRenderPass2Shadow( GCI::CommandContext & pCommandContext )
	{
		using namespace Ic3::Graphics::GCI;

		Ic3DebugAssert( pCommandContext.CheckFeatureSupport( ECommandObjectPropertyMaskContextFamilyDirectGraphics ) );
		auto * directGraphicsContext = pCommandContext.QueryInterface<CommandContextDirectGraphics>();

		GCI::ViewportDesc viewportDescLight;
		viewportDescLight.origin.x = 0;
		viewportDescLight.origin.y = 0;
		viewportDescLight.size.x = _shadowConfig.screenSize.width;
		viewportDescLight.size.y = _shadowConfig.screenSize.height;
		viewportDescLight.depthRange.zNear = 0.0f;
		viewportDescLight.depthRange.zFar = 1.0f;

		directGraphicsContext->SetGraphicsPipelineStateObject( *_gpuAPIState.psoPass2Shadow );

		directGraphicsContext->BeginRenderPass( *_gpuAPIState.renderPass2Shadow );

		directGraphicsContext->CmdSetShaderConstantBuffer( 17, *_resources.constantBuffer );
		directGraphicsContext->CmdSetShaderTextureImage( 27, *_resources.shadowMapTexture );
		directGraphicsContext->CmdSetShaderTextureSampler( 77, *_gpuAPIState.samplerPass2Shadow );
	}

	void ShadowRenderer::endRenderPass( GCI::CommandContext & pCommandContext )
	{
		using namespace Ic3::Graphics::GCI;

		Ic3DebugAssert( pCommandContext.CheckFeatureSupport( ECommandObjectPropertyMaskContextFamilyDirectGraphics ) );
		auto * directGraphicsContext = pCommandContext.QueryInterface<CommandContextDirectGraphics>();

		directGraphicsContext->EndRenderPass();
	}

	void ShadowRenderer::initializeResources()
	{
		using namespace Ic3::Graphics::GCI;

		{
			Ic3::Graphics::GCI::GPUBufferCreateInfo constantBufferCreateInfo;
			constantBufferCreateInfo.memoryFlags = Ic3::Graphics::GCI::eGPUMemoryAccessFlagGPUReadBit;
			constantBufferCreateInfo.resourceFlags = Ic3::Graphics::GCI::eGPUResourceContentFlagStaticBit;
			constantBufferCreateInfo.resourceFlags |= Ic3::Graphics::GCI::eGPUBufferBindFlagConstantBufferBit;
			constantBufferCreateInfo.bufferSize = sizeof( CBShadowData );

			_resources.constantBuffer = _gpuDevice.CreateGPUBuffer( constantBufferCreateInfo );
		}

		{
			TextureCreateInfo shadowMapTextureCreateInfo;
			shadowMapTextureCreateInfo.texClass = ETextureClass::T2D;
			shadowMapTextureCreateInfo.dimensions.width = _shadowConfig.shadowMapSize.width;
			shadowMapTextureCreateInfo.dimensions.height = _shadowConfig.shadowMapSize.height;
			shadowMapTextureCreateInfo.memoryFlags = eGPUMemoryAccessFlagGPUReadBit;
			shadowMapTextureCreateInfo.resourceFlags =
					eGPUResourceUsageFlagRenderTargetDepthBit | eGPUResourceUsageFlagShaderInputBit;
			shadowMapTextureCreateInfo.internalFormat = ETextureFormat::D32F;

			_resources.shadowMapTexture = _gpuDevice.CreateTexture( shadowMapTextureCreateInfo );

			Ic3::Graphics::GCI::RenderTargetTextureCreateInfo shadowMapRTTCreateInfo;
			shadowMapRTTCreateInfo.targetTexture = _resources.shadowMapTexture;
			shadowMapRTTCreateInfo.bindFlags =
					eTextureBindFlagRenderTargetDepthAttachmentBit | eTextureBindFlagShaderInputSampledImageBit;

			_resources.shadowMapRTT = _gpuDevice.CreateRenderTargetTexture( shadowMapRTTCreateInfo );

			auto & depthStencilAttachment = _gpuAPIState.rtBindingPass1Light.SetDepthStencilAttachmentBinding();
			depthStencilAttachment.attachmentTexture = _resources.shadowMapRTT;
		}

		{
			GCI::SamplerCreateInfo samplerPass2ShadowCreateInfo;
			samplerPass2ShadowCreateInfo.samplerConfig = GCI::cvSamplerConfigDefault;
			samplerPass2ShadowCreateInfo.samplerConfig.addressModeConfig.coordU = ETextureAddressMode::ClampToColor;
			samplerPass2ShadowCreateInfo.samplerConfig.addressModeConfig.coordV = ETextureAddressMode::ClampToColor;
			samplerPass2ShadowCreateInfo.samplerConfig.addressModeConfig.coordW = ETextureAddressMode::ClampToColor;
			samplerPass2ShadowCreateInfo.samplerConfig.textureCompareMode = ETextureCompareMode::RefToTexture;
			samplerPass2ShadowCreateInfo.samplerConfig.textureCompareFunc = ECompFunc::LessEqual;
			samplerPass2ShadowCreateInfo.samplerConfig.borderColor = Math::RGBAColorR32Norm{ 1.0f, 0.0f, 0.0f, 0.0f };

			_gpuAPIState.samplerPass2Shadow = _gpuDevice.CreateSampler( samplerPass2ShadowCreateInfo );
		}
	}

	void ShadowRenderer::initializeRenderPassStates()
	{
		using namespace Ic3::Graphics::GCI;

		{
			RenderPassConfiguration renderPass1LightConfig;
			renderPass1LightConfig.activeAttachmentsMask = eRTAttachmentMaskDefaultDSOnly;
			renderPass1LightConfig.attachmentsActionClearMask = eRTAttachmentMaskDefaultDSOnly;
			renderPass1LightConfig.depthStencilAttachment.renderPassLoadAction = ERenderPassAttachmentLoadAction::Clear;
			renderPass1LightConfig.depthStencilAttachment.renderPassStoreAction = ERenderPassAttachmentStoreAction::Keep;
			renderPass1LightConfig.depthStencilAttachment.clearConfig.depthValue = 1.0f;
			renderPass1LightConfig.depthStencilAttachment.clearConfig.stencilValue = 0;

			_gpuAPIState.renderPass1Light = _gpuDevice.CreateRenderPassConfigurationCompiledState( renderPass1LightConfig );
		}

		{
			RenderPassConfiguration renderPass2ShadowConfig;
			renderPass2ShadowConfig.activeAttachmentsMask = eRTAttachmentMaskDefaultC0DS;
			renderPass2ShadowConfig.attachmentsActionClearMask = eRTAttachmentMaskDefaultC0DS;
			renderPass2ShadowConfig.colorAttachments[0].renderPassLoadAction = ERenderPassAttachmentLoadAction::Clear;
			renderPass2ShadowConfig.colorAttachments[0].renderPassStoreAction = ERenderPassAttachmentStoreAction::Keep;
			renderPass2ShadowConfig.colorAttachments[0].clearConfig.colorValue = {0.0f, 0.0f, 0.0f, 1.0f };
			renderPass2ShadowConfig.depthStencilAttachment.renderPassLoadAction = ERenderPassAttachmentLoadAction::Clear;
			renderPass2ShadowConfig.depthStencilAttachment.renderPassStoreAction = ERenderPassAttachmentStoreAction::Keep;
			renderPass2ShadowConfig.depthStencilAttachment.clearConfig.depthValue = 1.0f;
			renderPass2ShadowConfig.depthStencilAttachment.clearConfig.stencilValue = 0;

			_gpuAPIState.renderPass2Shadow = _gpuDevice.CreateRenderPassConfigurationCompiledState( renderPass2ShadowConfig );
		}
	}

	void ShadowRenderer::initializePipelineStateObjects()
	{
		using namespace Ic3::Graphics::GCI;

		{
			auto vertexShaderPass1 = _shaderLibrary.getShader( "SID_SHADOW_0_PASS1_LIGHT_VS" );
			auto pixelShaderPass1 = _shaderLibrary.getShader( "SID_SHADOW_0_PASS1_LIGHT_PS" );

			GCI::GraphicsPipelineStateObjectCreateInfo psoPass1LightCreateInfo;
			psoPass1LightCreateInfo.renderTargetLayout.activeAttachmentsMask = eRTAttachmentMaskDefaultDSOnly;
			psoPass1LightCreateInfo.renderTargetLayout.depthStencilAttachment.format = ETextureFormat::D24UNS8U;
			psoPass1LightCreateInfo.blendConfig = defaults::cvPipelineBlendConfigDefault;
			psoPass1LightCreateInfo.depthStencilConfig = defaults::cvPipelineDepthStencilConfigEnableDepthTest;
			psoPass1LightCreateInfo.rasterizerConfig = defaults::cvPipelineRasterizerConfigDefault;
			psoPass1LightCreateInfo.rasterizerConfig.cullMode = ECullMode::Front;
			psoPass1LightCreateInfo.rasterizerConfig.primitiveFillMode = EPrimitiveFillMode::Solid;
			psoPass1LightCreateInfo.rasterizerConfig.frontFaceVerticesOrder = ETriangleVerticesOrder::CounterClockwise;
			psoPass1LightCreateInfo.inputLayoutDefinition.primitiveTopology = EPrimitiveTopology::TriangleList;
			psoPass1LightCreateInfo.inputLayoutDefinition.activeAttributesMask =
				eIAVertexAttributeFlagAttr0Bit | eIAVertexAttributeFlagAttr1Bit | eIAVertexAttributeFlagAttr2Bit | eIAVertexAttributeFlagAttr3Bit;
			psoPass1LightCreateInfo.inputLayoutDefinition.attributeArray[0] = { { "POSITION", 0 }, GCI::EVertexAttribFormat::Vec3F32, 0, 0 };
			psoPass1LightCreateInfo.inputLayoutDefinition.attributeArray[1] = { { "COLOR", 0 }, GCI::EVertexAttribFormat::Vec4F32, 0, GCI::kIAVertexAttributeOffsetAppend };
			psoPass1LightCreateInfo.inputLayoutDefinition.attributeArray[2] = { { "NORMAL", 0 }, GCI::EVertexAttribFormat::Vec3F32, 0, GCI::kIAVertexAttributeOffsetAppend };
			psoPass1LightCreateInfo.inputLayoutDefinition.attributeArray[3] = { { "TEXCOORD", 0 }, GCI::EVertexAttribFormat::Vec2F32, 0, GCI::kIAVertexAttributeOffsetAppend };
			psoPass1LightCreateInfo.shaderSet.AddShader( vertexShaderPass1 );
			psoPass1LightCreateInfo.shaderSet.AddShader( pixelShaderPass1 );
			psoPass1LightCreateInfo.shaderInputSignatureDesc.activeShaderStagesMask = eShaderStageFlagGraphicsVertexBit;
			psoPass1LightCreateInfo.shaderInputSignatureDesc.descriptorSetsNum = 1;
			{
				psoPass1LightCreateInfo.shaderInputSignatureDesc.descriptorSetArray[0].descriptorType = EShaderInputDescriptorType::Resource;
				psoPass1LightCreateInfo.shaderInputSignatureDesc.descriptorSetArray[0].descriptorsNum = 2;

				psoPass1LightCreateInfo.shaderInputSignatureDesc.descriptorSetArray[0].descriptorList[0] = {
						10, EShaderInputDescriptorType::Resource, eShaderStageFlagGraphicsVertexBit
				};
				psoPass1LightCreateInfo.shaderInputSignatureDesc.descriptorSetArray[0].descriptorList[0].uResourceDesc = {
					EShaderInputResourceType::CBVConstantBuffer, 0, 1
				};
				psoPass1LightCreateInfo.shaderInputSignatureDesc.descriptorSetArray[0].descriptorList[1] = {
						17, EShaderInputDescriptorType::Resource, eShaderStageFlagGraphicsVertexBit
				};
				psoPass1LightCreateInfo.shaderInputSignatureDesc.descriptorSetArray[0].descriptorList[1].uResourceDesc = {
					EShaderInputResourceType::CBVConstantBuffer, 7, 1
				};
			}

			_gpuAPIState.psoPass1Light = _gpuDevice.CreateGraphicsPipelineStateObject( psoPass1LightCreateInfo );
		}

		{
			auto vertexShaderPass2 = _shaderLibrary.getShader( "SID_SHADOW_0_PASS2_SHADOW_VS" );
			auto pixelShaderPass2 = _shaderLibrary.getShader( "SID_SHADOW_0_PASS2_SHADOW_PS" );

			GCI::GraphicsPipelineStateObjectCreateInfo psoPass2ShadowCreateInfo;
			psoPass2ShadowCreateInfo.renderTargetLayout.activeAttachmentsMask = eRTAttachmentMaskDefaultC0DS;
			psoPass2ShadowCreateInfo.renderTargetLayout.colorAttachments[0].format = ETextureFormat::BGRA8UN;
			psoPass2ShadowCreateInfo.renderTargetLayout.depthStencilAttachment.format = ETextureFormat::D24UNS8U;
			psoPass2ShadowCreateInfo.blendConfig = defaults::cvPipelineBlendConfigDefault;
			psoPass2ShadowCreateInfo.depthStencilConfig = defaults::cvPipelineDepthStencilConfigEnableDepthTest;
			psoPass2ShadowCreateInfo.rasterizerConfig = defaults::cvPipelineRasterizerConfigDefault;
			psoPass2ShadowCreateInfo.rasterizerConfig.cullMode = ECullMode::Back;
			psoPass2ShadowCreateInfo.rasterizerConfig.primitiveFillMode = EPrimitiveFillMode::Solid;
			psoPass2ShadowCreateInfo.rasterizerConfig.frontFaceVerticesOrder = ETriangleVerticesOrder::CounterClockwise;
			psoPass2ShadowCreateInfo.inputLayoutDefinition.primitiveTopology = EPrimitiveTopology::TriangleList;
			psoPass2ShadowCreateInfo.inputLayoutDefinition.activeAttributesMask =
				eIAVertexAttributeFlagAttr0Bit | eIAVertexAttributeFlagAttr1Bit | eIAVertexAttributeFlagAttr2Bit | eIAVertexAttributeFlagAttr3Bit;
			psoPass2ShadowCreateInfo.inputLayoutDefinition.attributeArray[0] = { { "POSITION", 0 }, Ic3::Graphics::GCI::EVertexAttribFormat::Vec3F32, 0, 0 };
			psoPass2ShadowCreateInfo.inputLayoutDefinition.attributeArray[1] = { { "COLOR", 0 }, Ic3::Graphics::GCI::EVertexAttribFormat::Vec4F32, 0, Ic3::Graphics::GCI::kIAVertexAttributeOffsetAppend };
			psoPass2ShadowCreateInfo.inputLayoutDefinition.attributeArray[2] = { { "NORMAL", 0 }, Ic3::Graphics::GCI::EVertexAttribFormat::Vec3F32, 0, Ic3::Graphics::GCI::kIAVertexAttributeOffsetAppend };
			psoPass2ShadowCreateInfo.inputLayoutDefinition.attributeArray[3] = { { "TEXCOORD", 0 }, Ic3::Graphics::GCI::EVertexAttribFormat::Vec2F32, 0, Ic3::Graphics::GCI::kIAVertexAttributeOffsetAppend };
			psoPass2ShadowCreateInfo.shaderSet.AddShader( vertexShaderPass2 );
			psoPass2ShadowCreateInfo.shaderSet.AddShader( pixelShaderPass2 );
			psoPass2ShadowCreateInfo.shaderInputSignatureDesc.activeShaderStagesMask = eShaderStageMaskGraphicsVsPs;
			psoPass2ShadowCreateInfo.shaderInputSignatureDesc.descriptorSetsNum = 2;
			{
				psoPass2ShadowCreateInfo.shaderInputSignatureDesc.descriptorSetArray[0].descriptorType = EShaderInputDescriptorType::Resource;
				psoPass2ShadowCreateInfo.shaderInputSignatureDesc.descriptorSetArray[0].descriptorsNum = 4;

				psoPass2ShadowCreateInfo.shaderInputSignatureDesc.descriptorSetArray[0].descriptorList[0] = {
						10, EShaderInputDescriptorType::Resource, eShaderStageFlagGraphicsVertexBit
				};
				psoPass2ShadowCreateInfo.shaderInputSignatureDesc.descriptorSetArray[0].descriptorList[0].uResourceDesc = {
					EShaderInputResourceType::CBVConstantBuffer, 0, 1
				};
				psoPass2ShadowCreateInfo.shaderInputSignatureDesc.descriptorSetArray[0].descriptorList[1] = {
						17, EShaderInputDescriptorType::Resource, eShaderStageFlagGraphicsVertexBit
				};
				psoPass2ShadowCreateInfo.shaderInputSignatureDesc.descriptorSetArray[0].descriptorList[1].uResourceDesc = {
					EShaderInputResourceType::CBVConstantBuffer, 7, 1
				};
				psoPass2ShadowCreateInfo.shaderInputSignatureDesc.descriptorSetArray[0].descriptorList[2] = {
						20, EShaderInputDescriptorType::Resource, eShaderStageFlagGraphicsPixelBit
				};
				psoPass2ShadowCreateInfo.shaderInputSignatureDesc.descriptorSetArray[0].descriptorList[2].uResourceDesc = {
					EShaderInputResourceType::SRVTextureImage, 0, 1
				};
				psoPass2ShadowCreateInfo.shaderInputSignatureDesc.descriptorSetArray[0].descriptorList[3] = {
						27, EShaderInputDescriptorType::Resource, eShaderStageFlagGraphicsPixelBit
				};
				psoPass2ShadowCreateInfo.shaderInputSignatureDesc.descriptorSetArray[0].descriptorList[3].uResourceDesc = {
					EShaderInputResourceType::SRVTextureImage, 7, 1
				};
			}
			{
				psoPass2ShadowCreateInfo.shaderInputSignatureDesc.descriptorSetArray[1].descriptorType = EShaderInputDescriptorType::Sampler;
				psoPass2ShadowCreateInfo.shaderInputSignatureDesc.descriptorSetArray[1].descriptorsNum = 2;

				psoPass2ShadowCreateInfo.shaderInputSignatureDesc.descriptorSetArray[1].descriptorList[0] =
						{70, EShaderInputDescriptorType::Sampler, eShaderStageFlagGraphicsPixelBit };
				psoPass2ShadowCreateInfo.shaderInputSignatureDesc.descriptorSetArray[1].descriptorList[0].uSamplerConfig = { 0 };

				psoPass2ShadowCreateInfo.shaderInputSignatureDesc.descriptorSetArray[1].descriptorList[1] =
						{77, EShaderInputDescriptorType::Sampler, eShaderStageFlagGraphicsPixelBit };
				psoPass2ShadowCreateInfo.shaderInputSignatureDesc.descriptorSetArray[1].descriptorList[1].uSamplerConfig = { 7 };
			}

			_gpuAPIState.psoPass2Shadow = _gpuDevice.CreateGraphicsPipelineStateObject( psoPass2ShadowCreateInfo );
		}
	}

}
