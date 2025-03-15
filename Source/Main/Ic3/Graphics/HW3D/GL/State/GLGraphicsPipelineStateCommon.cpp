
#include "GLGraphicsPipelineStateCommon.h"
#include <Ic3/Graphics/HW3D/GL/GLAPITranslationLayer.h>
#include <Ic3/Graphics/HW3D/GL/GLGPUDevice.h>

namespace Ic3::Graphics::GCI
{

	GLBlendStateDescriptor::GLBlendStateDescriptor(
			GLGPUDevice & pGPUDevice,
			cppx::bitmask<EBlendConfigFlags> pBlendFlags,
			const GLBlendSettings & pGLBlendSettings )
	: HW3DPipelineStateDescriptor( pGPUDevice, pBlendFlags )
	, mGLBlendSettings( pGLBlendSettings )
	{}

	GLBlendStateDescriptor::~GLBlendStateDescriptor() = default;

	TGfxHandle<GLBlendStateDescriptor> GLBlendStateDescriptor::CreateInstance(
			GLGPUDevice & pGPUDevice,
			const BlendStateDescriptorCreateInfo & pCreateInfo )
	{
		const auto glcBlendSettings = Utilities::TranslateBlendSettingsGL( pCreateInfo.blendSettings );

		auto stateDescriptor = CreateGfxObject<GLBlendStateDescriptor>(
				pGPUDevice,
				pCreateInfo.blendSettings.flags,
				glcBlendSettings );

		return stateDescriptor;
	}


	GLDepthStencilStateDescriptor::GLDepthStencilStateDescriptor(
			GLGPUDevice & pGPUDevice,
			cppx::bitmask<EDepthStencilConfigFlags> pDepthStencilFlags,
			const GLDepthStencilSettings & pGLDepthStencilSettings  )
	: HW3DPipelineStateDescriptor( pGPUDevice, pDepthStencilFlags )
	, mGLDepthStencilSettings( pGLDepthStencilSettings )
	{}

	GLDepthStencilStateDescriptor::~GLDepthStencilStateDescriptor() = default;

	TGfxHandle<GLDepthStencilStateDescriptor> GLDepthStencilStateDescriptor::CreateInstance(
			GLGPUDevice & pGPUDevice,
			const DepthStencilStateDescriptorCreateInfo & pCreateInfo )
	{
		const auto glcDepthStencilSettings = Utilities::TranslateDepthStencilSettingsGL( pCreateInfo.depthStencilSettings );

		auto stateDescriptor = CreateGfxObject<GLDepthStencilStateDescriptor>(
				pGPUDevice,
				pCreateInfo.depthStencilSettings.commonFlags,
				glcDepthStencilSettings );

		return stateDescriptor;
	}


	GLRasterizerStateDescriptor::GLRasterizerStateDescriptor(
			GLGPUDevice & pGPUDevice,
			cppx::bitmask<ERasterizerConfigFlags> pRasterizerFlags,
			const GLRasterizerSettings & pGLRasterizerSettings )
	: HW3DPipelineStateDescriptor( pGPUDevice, pRasterizerFlags )
	, mGLRasterizerSettings( pGLRasterizerSettings )
	{}

	GLRasterizerStateDescriptor::~GLRasterizerStateDescriptor() = default;

	TGfxHandle<GLRasterizerStateDescriptor> GLRasterizerStateDescriptor::CreateInstance(
			GLGPUDevice & pGPUDevice,
			const RasterizerStateDescriptorCreateInfo & pCreateInfo )
	{
		const auto glcRasterizerSettings = Utilities::TranslateRasterizerSettingsGL( pCreateInfo.rasterizerSettings );

		auto stateDescriptor = CreateGfxObject<GLRasterizerStateDescriptor>(
				pGPUDevice,
				pCreateInfo.rasterizerSettings.flags,
				glcRasterizerSettings );

		return stateDescriptor;
	}


	namespace Utilities
	{

		GLBlendSettings TranslateBlendSettingsGL( const BlendSettings & pBlendSettings )
		{
			GLBlendSettings openglBlendSettings{};
			openglBlendSettings.attachmentsMask = pBlendSettings.attachmentsMask;
			openglBlendSettings.flags = pBlendSettings.flags;

			if( pBlendSettings.attachmentsMask != 0 )
			{
				if( !pBlendSettings.flags.is_set( eBlendConfigFlagEnableMRTIndependentBlendingBit ) )
				{
					openglBlendSettings.attachments[0] = Utilities::TranslateRenderTargetColorAttachmentBlendSettingsGL( pBlendSettings.attachments[0] );
				}
				else
				{
					ForEachRTAttachmentIndex( pBlendSettings.attachmentsMask,
						[&]( native_uint pIndex, ERTAttachmentFlags pAttachmentBit )
						{
							auto & openglAttachmentSettings = openglBlendSettings.attachments[pIndex];
							openglAttachmentSettings = Utilities::TranslateRenderTargetColorAttachmentBlendSettingsGL( pBlendSettings.attachments[pIndex] );
							openglAttachmentSettings.blendActive = true;
							return true;
						} );
				}

				openglBlendSettings.constantColor = pBlendSettings.constantColor;
			}

			return openglBlendSettings;
		}

		GLDepthStencilSettings TranslateDepthStencilSettingsGL( const DepthStencilSettings & pDepthStencilSettings )
		{
			GLDepthStencilSettings openglDepthStencilSettings{};

			auto & depthSettings = pDepthStencilSettings.depthTest;
			openglDepthStencilSettings.depthTestActive = pDepthStencilSettings.commonFlags.is_set(
					eDepthStencilConfigFlagEnableDepthTestBit );
			openglDepthStencilSettings.depthSettings.depthCompFunc = ATL::GLTranslateCompFunc( depthSettings.depthCompFunc );
			openglDepthStencilSettings.depthSettings.writeMask = (depthSettings.depthWriteMask == EDepthWriteMask::All ) ? GL_TRUE : GL_FALSE;

			openglDepthStencilSettings.stencilTestActive = pDepthStencilSettings.commonFlags.is_set(
					eDepthStencilConfigFlagEnableStencilTestBit );

			auto & stencilBackFaceDesc = pDepthStencilSettings.stencilTest.backFace;
			openglDepthStencilSettings.stencilSettings.backFace.compFunc = ATL::GLTranslateCompFunc( stencilBackFaceDesc.compFunc );
			openglDepthStencilSettings.stencilSettings.backFace.opFail = ATL::GLTranslateStencilOp( stencilBackFaceDesc.opFail );
			openglDepthStencilSettings.stencilSettings.backFace.opPassDepthFail = ATL::GLTranslateStencilOp( stencilBackFaceDesc.opPassDepthFail );
			openglDepthStencilSettings.stencilSettings.backFace.opPassDepthPass = ATL::GLTranslateStencilOp( stencilBackFaceDesc.opPassDepthPass );
			openglDepthStencilSettings.stencilSettings.backFace.readMask = pDepthStencilSettings.stencilTest.readMask;
			openglDepthStencilSettings.stencilSettings.backFace.writeMask = pDepthStencilSettings.stencilTest.writeMask;

			auto & stencilFrontFaceDesc = pDepthStencilSettings.stencilTest.frontFace;
			openglDepthStencilSettings.stencilSettings.frontFace.compFunc = ATL::GLTranslateCompFunc( stencilFrontFaceDesc.compFunc );
			openglDepthStencilSettings.stencilSettings.frontFace.opFail = ATL::GLTranslateStencilOp( stencilFrontFaceDesc.opFail );
			openglDepthStencilSettings.stencilSettings.frontFace.opPassDepthFail = ATL::GLTranslateStencilOp( stencilFrontFaceDesc.opPassDepthFail );
			openglDepthStencilSettings.stencilSettings.frontFace.opPassDepthPass = ATL::GLTranslateStencilOp( stencilFrontFaceDesc.opPassDepthPass );
			openglDepthStencilSettings.stencilSettings.frontFace.readMask = pDepthStencilSettings.stencilTest.readMask;
			openglDepthStencilSettings.stencilSettings.frontFace.writeMask = pDepthStencilSettings.stencilTest.writeMask;

			return openglDepthStencilSettings;
		}

		GLRasterizerSettings TranslateRasterizerSettingsGL( const RasterizerSettings & pRasterizerSettings )
		{
			GLRasterizerSettings rasterizerSettings{};

			rasterizerSettings.scissorTestActive = pRasterizerSettings.flags.is_set( eRasterizerConfigFlagEnableScissorTestBit );
			rasterizerSettings.cullMode = ATL::GLTranslateCullMode( pRasterizerSettings.cullMode );
			rasterizerSettings.frontFaceVerticesOrder = ATL::GLTranslateTriangleVerticesOrder( pRasterizerSettings.frontFaceVerticesOrder );
		#if( IC3_GX_GL_FEATURE_SUPPORT_PRIMITIVE_FILL_MODE )
			rasterizerSettings.primitiveFillMode = ATL::GLTranslatePrimitiveFillMode( pRasterizerSettings.primitiveFillMode );
		#endif

			return rasterizerSettings;
		}

		GLRenderTargetColorAttachmentBlendSettings TranslateRenderTargetColorAttachmentBlendSettingsGL(
				const RenderTargetColorAttachmentBlendSettings & pColorAttachmentBlendSettings )
		{
			GLRenderTargetColorAttachmentBlendSettings openglBlendSettings;
			openglBlendSettings.equation.rgb = ATL::GLTranslateBlendOp( pColorAttachmentBlendSettings.opColor );
			openglBlendSettings.equation.alpha = ATL::GLTranslateBlendOp( pColorAttachmentBlendSettings.opAlpha );
			openglBlendSettings.factor.rgbSrc = ATL::GLTranslateBlendFactor( pColorAttachmentBlendSettings.factorSrcColor );
			openglBlendSettings.factor.rgbDst = ATL::GLTranslateBlendFactor( pColorAttachmentBlendSettings.factorDstColor );
			openglBlendSettings.factor.alphaSrc = ATL::GLTranslateBlendFactor( pColorAttachmentBlendSettings.factorSrcAlpha );
			openglBlendSettings.factor.alphaDst = ATL::GLTranslateBlendFactor( pColorAttachmentBlendSettings.factorDstAlpha );

			return openglBlendSettings;
		}

	}

}
