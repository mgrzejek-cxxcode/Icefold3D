
#include "GLCommonGraphicsConfig.h"
#include <Ic3/Graphics/HW3D/GL/GLAPITranslationLayer.h>
#include <Ic3/Graphics/HW3D/GL/GLGPUDevice.h>

namespace Ic3::Graphics::GCI
{

	GLBlendImmutableState::GLBlendImmutableState(
			GLGPUDevice & pGPUDevice,
			cppx::bitmask<EBlendConfigFlags> pBlendFlags,
			const GLBlendConfig & pGLBlendConfig )
	: BlendImmutableState( pGPUDevice, pBlendFlags )
	, mGLBlendConfig( pGLBlendConfig )
	{}

	GLBlendImmutableState::~GLBlendImmutableState() = default;

	TGfxHandle<GLBlendImmutableState> GLBlendImmutableState::CreateInstance(
			GLGPUDevice & pGPUDevice,
			const BlendConfig & pBlendConfig )
	{
		const auto glcBlendConfig = SMU::TranslateBlendConfigGL( pBlendConfig );

		auto immutableState = CreateGfxObject<GLBlendImmutableState>(
				pGPUDevice,
				pBlendConfig.flags,
				glcBlendConfig );

		return immutableState;
	}


	GLDepthStencilImmutableState::GLDepthStencilImmutableState(
			GLGPUDevice & pGPUDevice,
			cppx::bitmask<EDepthStencilConfigFlags> pDepthStencilFlags,
			const GLDepthStencilConfig & pGLDepthStencilConfig  )
	: DepthStencilImmutableState( pGPUDevice, pDepthStencilFlags )
	, mGLDepthStencilConfig( pGLDepthStencilConfig )
	{}

	GLDepthStencilImmutableState::~GLDepthStencilImmutableState() = default;

	TGfxHandle<GLDepthStencilImmutableState> GLDepthStencilImmutableState::CreateInstance(
			GLGPUDevice & pGPUDevice,
			const DepthStencilConfig & pDepthStencilConfig  )
	{
		const auto glcDepthStencilConfig = SMU::TranslateDepthStencilConfigGL( pDepthStencilConfig );

		auto immutableState = CreateGfxObject<GLDepthStencilImmutableState>(
				pGPUDevice,
				pDepthStencilConfig.commonFlags,
				glcDepthStencilConfig );

		return immutableState;
	}


	GLRasterizerImmutableState::GLRasterizerImmutableState(
			GLGPUDevice & pGPUDevice,
			cppx::bitmask<ERasterizerConfigFlags> pRasterizerFlags,
			const GLRasterizerConfig & pGLRasterizerConfig )
	: RasterizerImmutableState( pGPUDevice, pRasterizerFlags )
	, mGLRasterizerConfig( pGLRasterizerConfig )
	{}

	GLRasterizerImmutableState::~GLRasterizerImmutableState() = default;

	TGfxHandle<GLRasterizerImmutableState> GLRasterizerImmutableState::CreateInstance(
			GLGPUDevice & pGPUDevice,
			const RasterizerConfig & pRasterizerConfig )
	{
		const auto glcRasterizerConfig = SMU::TranslateRasterizerConfigGL( pRasterizerConfig );

		auto immutableState = CreateGfxObject<GLRasterizerImmutableState>(
				pGPUDevice,
				pRasterizerConfig.flags,
				glcRasterizerConfig );

		return immutableState;
	}


	namespace SMU
	{

		GLBlendConfig TranslateBlendConfigGL( const BlendConfig & pConfig )
		{
			GLBlendConfig openglBlendConfig{};
			openglBlendConfig.attachmentsMask = pConfig.attachmentsMask;
			openglBlendConfig.flags = pConfig.flags;

			if( pConfig.attachmentsMask != 0 )
			{
				if( !pConfig.flags.is_set( eBlendConfigFlagEnableMRTIndependentBlendingBit ) )
				{
					openglBlendConfig.attachments[0] = SMU::TranslateRTColorAttachmentBlendSettingsGL( pConfig.attachments[0] );
				}
				else
				{
					ForEachRTAttachmentIndex( pConfig.attachmentsMask,
						[&]( native_uint pIndex, ERTAttachmentFlags pAttachmentBit )
						{
							auto & openglAttachmentSettings = openglBlendConfig.attachments[pIndex];
							openglAttachmentSettings = SMU::TranslateRTColorAttachmentBlendSettingsGL( pConfig.attachments[pIndex] );
							openglAttachmentSettings.blendActive = true;
							return true;
						} );
				}

				openglBlendConfig.constantColor = pConfig.constantColor;
			}

			return openglBlendConfig;
		}

		GLDepthStencilConfig TranslateDepthStencilConfigGL( const DepthStencilConfig & pConfig )
		{
			GLDepthStencilConfig openglDepthStencilConfig{};

			auto & depthSettings = pConfig.depthTestSettings;
			openglDepthStencilConfig.depthTestActive = pConfig.commonFlags.is_set(
					eDepthStencilConfigFlagEnableDepthTestBit );
			openglDepthStencilConfig.depthSettings.depthCompFunc = ATL::TranslateGLCompFunc( depthSettings.depthCompFunc );
			openglDepthStencilConfig.depthSettings.writeMask = (depthSettings.depthWriteMask == EDepthWriteMask::All ) ? GL_TRUE : GL_FALSE;

			openglDepthStencilConfig.stencilTestActive = pConfig.commonFlags.is_set(
					eDepthStencilConfigFlagEnableStencilTestBit );

			auto & stencilBackFaceDesc = pConfig.stencilTestSettings.backFace;
			openglDepthStencilConfig.stencilSettings.backFace.compFunc = ATL::TranslateGLCompFunc( stencilBackFaceDesc.compFunc );
			openglDepthStencilConfig.stencilSettings.backFace.opFail = ATL::TranslateGLStencilOp( stencilBackFaceDesc.opFail );
			openglDepthStencilConfig.stencilSettings.backFace.opPassDepthFail = ATL::TranslateGLStencilOp( stencilBackFaceDesc.opPassDepthFail );
			openglDepthStencilConfig.stencilSettings.backFace.opPassDepthPass = ATL::TranslateGLStencilOp( stencilBackFaceDesc.opPassDepthPass );
			openglDepthStencilConfig.stencilSettings.backFace.readMask = pConfig.stencilTestSettings.readMask;
			openglDepthStencilConfig.stencilSettings.backFace.writeMask = pConfig.stencilTestSettings.writeMask;

			auto & stencilFrontFaceDesc = pConfig.stencilTestSettings.frontFace;
			openglDepthStencilConfig.stencilSettings.frontFace.compFunc = ATL::TranslateGLCompFunc( stencilFrontFaceDesc.compFunc );
			openglDepthStencilConfig.stencilSettings.frontFace.opFail = ATL::TranslateGLStencilOp( stencilFrontFaceDesc.opFail );
			openglDepthStencilConfig.stencilSettings.frontFace.opPassDepthFail = ATL::TranslateGLStencilOp( stencilFrontFaceDesc.opPassDepthFail );
			openglDepthStencilConfig.stencilSettings.frontFace.opPassDepthPass = ATL::TranslateGLStencilOp( stencilFrontFaceDesc.opPassDepthPass );
			openglDepthStencilConfig.stencilSettings.frontFace.readMask = pConfig.stencilTestSettings.readMask;
			openglDepthStencilConfig.stencilSettings.frontFace.writeMask = pConfig.stencilTestSettings.writeMask;

			return openglDepthStencilConfig;
		}

		GLRasterizerConfig TranslateRasterizerConfigGL( const RasterizerConfig & pConfig )
		{
			GLRasterizerConfig rasterizerConfig{};

			rasterizerConfig.scissorTestActive = pConfig.flags.is_set( eRasterizerConfigFlagEnableScissorTestBit );
			rasterizerConfig.cullMode = ATL::TranslateGLCullMode( pConfig.cullMode );
			rasterizerConfig.frontFaceVerticesOrder = ATL::TranslateGLTriangleVerticesOrder( pConfig.frontFaceVerticesOrder );
		#if( IC3_GX_GL_FEATURE_SUPPORT_PRIMITIVE_FILL_MODE )
			rasterizerConfig.primitiveFillMode = ATL::TranslateGLPrimitiveFillMode( pConfig.primitiveFillMode );
		#endif

			return rasterizerConfig;
		}

		GLRTColorAttachmentBlendSettings TranslateRTColorAttachmentBlendSettingsGL( const RTColorAttachmentBlendSettings & pSettings )
		{
			GLRTColorAttachmentBlendSettings openglBlendSettings;
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
