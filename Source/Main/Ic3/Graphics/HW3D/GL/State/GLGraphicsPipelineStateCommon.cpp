
#include "GLGraphicsPipelineStateCommon.h"
#include <Ic3/Graphics/HW3D/GL/GLAPITranslationLayer.h>
#include <Ic3/Graphics/HW3D/GL/GLGPUDevice.h>

namespace Ic3::Graphics::GCI
{

	GLBlendStateDescriptor::GLBlendStateDescriptor(
			GLGPUDevice & pGPUDevice,
			pipeline_state_descriptor_id_t pDescriptorID,
			cppx::bitmask<EBlendConfigFlags> pBlendFlags,
			const GLBlendSettings & pGLBlendSettings )
	: BlendStateDescriptor( pGPUDevice, pDescriptorID, pBlendFlags )
	, mGLBlendSettings( pGLBlendSettings )
	{}

	GLBlendStateDescriptor::~GLBlendStateDescriptor() = default;

	TGfxHandle<GLBlendStateDescriptor> GLBlendStateDescriptor::CreateInstance(
			GLGPUDevice & pGPUDevice,
			pipeline_state_descriptor_id_t pDescriptorID,
			const BlendStateDescriptorCreateInfo & pCreateInfo )
	{
		const auto glcBlendSettings = GCU::TranslateBlendSettingsGL( pCreateInfo.blendSettings );

		auto stateDescriptor = CreateGfxObject<GLBlendStateDescriptor>(
				pGPUDevice,
				pDescriptorID,
				pCreateInfo.blendSettings.flags,
				glcBlendSettings );

		return stateDescriptor;
	}


	GLDepthStencilStateDescriptor::GLDepthStencilStateDescriptor(
			GLGPUDevice & pGPUDevice,
			pipeline_state_descriptor_id_t pDescriptorID,
			cppx::bitmask<EDepthStencilConfigFlags> pDepthStencilFlags,
			const GLDepthStencilSettings & pGLDepthStencilSettings  )
	: DepthStencilStateDescriptor( pGPUDevice, pDescriptorID, pDepthStencilFlags )
	, mGLDepthStencilSettings( pGLDepthStencilSettings )
	{}

	GLDepthStencilStateDescriptor::~GLDepthStencilStateDescriptor() = default;

	TGfxHandle<GLDepthStencilStateDescriptor> GLDepthStencilStateDescriptor::CreateInstance(
			GLGPUDevice & pGPUDevice,
			pipeline_state_descriptor_id_t pDescriptorID,
			const DepthStencilStateDescriptorCreateInfo & pCreateInfo )
	{
		const auto glcDepthStencilSettings = GCU::TranslateDepthStencilSettingsGL( pCreateInfo.depthStencilSettings );

		auto stateDescriptor = CreateGfxObject<GLDepthStencilStateDescriptor>(
				pGPUDevice,
				pDescriptorID,
				pCreateInfo.depthStencilSettings.commonFlags,
				glcDepthStencilSettings );

		return stateDescriptor;
	}


	GLRasterizerStateDescriptor::GLRasterizerStateDescriptor(
			GLGPUDevice & pGPUDevice,
			pipeline_state_descriptor_id_t pDescriptorID,
			cppx::bitmask<ERasterizerConfigFlags> pRasterizerFlags,
			const GLRasterizerSettings & pGLRasterizerSettings )
	: RasterizerStateDescriptor( pGPUDevice, pDescriptorID, pRasterizerFlags )
	, mGLRasterizerSettings( pGLRasterizerSettings )
	{}

	GLRasterizerStateDescriptor::~GLRasterizerStateDescriptor() = default;

	TGfxHandle<GLRasterizerStateDescriptor> GLRasterizerStateDescriptor::CreateInstance(
			GLGPUDevice & pGPUDevice,
			pipeline_state_descriptor_id_t pDescriptorID,
			const RasterizerStateDescriptorCreateInfo & pCreateInfo )
	{
		const auto glcRasterizerSettings = GCU::TranslateRasterizerSettingsGL( pCreateInfo.rasterizerSettings );

		auto stateDescriptor = CreateGfxObject<GLRasterizerStateDescriptor>(
				pGPUDevice,
				pDescriptorID,
				pCreateInfo.rasterizerSettings.flags,
				glcRasterizerSettings );

		return stateDescriptor;
	}


	namespace GCU
	{

		GLBlendSettings TranslateBlendSettingsGL( const BlendSettings & pSettings )
		{
			GLBlendSettings openglBlendSettings{};
			openglBlendSettings.attachmentsMask = pSettings.attachmentsMask;
			openglBlendSettings.flags = pSettings.flags;

			if( pSettings.attachmentsMask != 0 )
			{
				if( !pSettings.flags.is_set( eBlendConfigFlagEnableMRTIndependentBlendingBit ) )
				{
					openglBlendSettings.attachments[0] = GCU::TranslateRenderTargetColorAttachmentBlendSettingsGL( pSettings.attachments[0] );
				}
				else
				{
					ForEachRTAttachmentIndex( pSettings.attachmentsMask,
						[&]( native_uint pIndex, ERTAttachmentFlags pAttachmentBit )
						{
							auto & openglAttachmentSettings = openglBlendSettings.attachments[pIndex];
							openglAttachmentSettings = GCU::TranslateRenderTargetColorAttachmentBlendSettingsGL( pSettings.attachments[pIndex] );
							openglAttachmentSettings.blendActive = true;
							return true;
						} );
				}

				openglBlendSettings.constantColor = pSettings.constantColor;
			}

			return openglBlendSettings;
		}

		GLDepthStencilSettings TranslateDepthStencilSettingsGL( const DepthStencilSettings & pSettings )
		{
			GLDepthStencilSettings openglDepthStencilSettings{};

			auto & depthSettings = pSettings.depthTest;
			openglDepthStencilSettings.depthTestActive = pSettings.commonFlags.is_set(
					eDepthStencilConfigFlagEnableDepthTestBit );
			openglDepthStencilSettings.depthSettings.depthCompFunc = ATL::TranslateGLCompFunc( depthSettings.depthCompFunc );
			openglDepthStencilSettings.depthSettings.writeMask = (depthSettings.depthWriteMask == EDepthWriteMask::All ) ? GL_TRUE : GL_FALSE;

			openglDepthStencilSettings.stencilTestActive = pSettings.commonFlags.is_set(
					eDepthStencilConfigFlagEnableStencilTestBit );

			auto & stencilBackFaceDesc = pSettings.stencilTest.backFace;
			openglDepthStencilSettings.stencilSettings.backFace.compFunc = ATL::TranslateGLCompFunc( stencilBackFaceDesc.compFunc );
			openglDepthStencilSettings.stencilSettings.backFace.opFail = ATL::TranslateGLStencilOp( stencilBackFaceDesc.opFail );
			openglDepthStencilSettings.stencilSettings.backFace.opPassDepthFail = ATL::TranslateGLStencilOp( stencilBackFaceDesc.opPassDepthFail );
			openglDepthStencilSettings.stencilSettings.backFace.opPassDepthPass = ATL::TranslateGLStencilOp( stencilBackFaceDesc.opPassDepthPass );
			openglDepthStencilSettings.stencilSettings.backFace.readMask = pSettings.stencilTest.readMask;
			openglDepthStencilSettings.stencilSettings.backFace.writeMask = pSettings.stencilTest.writeMask;

			auto & stencilFrontFaceDesc = pSettings.stencilTest.frontFace;
			openglDepthStencilSettings.stencilSettings.frontFace.compFunc = ATL::TranslateGLCompFunc( stencilFrontFaceDesc.compFunc );
			openglDepthStencilSettings.stencilSettings.frontFace.opFail = ATL::TranslateGLStencilOp( stencilFrontFaceDesc.opFail );
			openglDepthStencilSettings.stencilSettings.frontFace.opPassDepthFail = ATL::TranslateGLStencilOp( stencilFrontFaceDesc.opPassDepthFail );
			openglDepthStencilSettings.stencilSettings.frontFace.opPassDepthPass = ATL::TranslateGLStencilOp( stencilFrontFaceDesc.opPassDepthPass );
			openglDepthStencilSettings.stencilSettings.frontFace.readMask = pSettings.stencilTest.readMask;
			openglDepthStencilSettings.stencilSettings.frontFace.writeMask = pSettings.stencilTest.writeMask;

			return openglDepthStencilSettings;
		}

		GLRasterizerSettings TranslateRasterizerSettingsGL( const RasterizerSettings & pSettings )
		{
			GLRasterizerSettings rasterizerSettings{};

			rasterizerSettings.scissorTestActive = pSettings.flags.is_set( eRasterizerConfigFlagEnableScissorTestBit );
			rasterizerSettings.cullMode = ATL::TranslateGLCullMode( pSettings.cullMode );
			rasterizerSettings.frontFaceVerticesOrder = ATL::TranslateGLTriangleVerticesOrder( pSettings.frontFaceVerticesOrder );
		#if( IC3_GX_GL_FEATURE_SUPPORT_PRIMITIVE_FILL_MODE )
			rasterizerSettings.primitiveFillMode = ATL::TranslateGLPrimitiveFillMode( pSettings.primitiveFillMode );
		#endif

			return rasterizerSettings;
		}

		GLRenderTargetColorAttachmentBlendSettings TranslateRenderTargetColorAttachmentBlendSettingsGL( const RenderTargetColorAttachmentBlendSettings & pSettings )
		{
			GLRenderTargetColorAttachmentBlendSettings openglBlendSettings;
			openglBlendSettings.equation.rgb = ATL::TranslateGLBlendOp( pSettings.opColor );
			openglBlendSettings.equation.alpha = ATL::TranslateGLBlendOp( pSettings.opAlpha );
			openglBlendSettings.factor.rgbSrc = ATL::TranslateGLBlendFactor( pSettings.factorSrcColor );
			openglBlendSettings.factor.rgbDst = ATL::TranslateGLBlendFactor( pSettings.factorDstColor );
			openglBlendSettings.factor.alphaSrc = ATL::TranslateGLBlendFactor( pSettings.factorSrcAlpha );
			openglBlendSettings.factor.alphaDst = ATL::TranslateGLBlendFactor( pSettings.factorDstAlpha );

			return openglBlendSettings;
		}

	}

}
