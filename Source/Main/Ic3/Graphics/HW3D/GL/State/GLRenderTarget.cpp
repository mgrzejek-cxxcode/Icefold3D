
#include "GLRenderTarget.h"
#include <Ic3/Graphics/HW3D/GL/GLGpuDevice.h>
#include <Ic3/Graphics/HW3D/GL/Objects/GLFramebufferObject.h>
#include <Ic3/Graphics/HW3D/GL/Objects/GLRenderbufferObject.h>
#include <Ic3/Graphics/HW3D/GL/Resources/GLRenderBuffer.h>
#include <Ic3/Graphics/HW3D/GL/Resources/GLTexture.h>

namespace Ic3::Graphics::GCI
{

	GLRenderTargetBindingImmutableState::GLRenderTargetBindingImmutableState(
			GLGpuDevice & pGpuDevice,
			const RenderTargetLayout & pRenderTargetLayout,
			GLRenderTargetBindingFBOData pGLFBOData )
	: RenderTargetBindingImmutableState( pGpuDevice, pRenderTargetLayout )
	, mGLFBOData( std::move( pGLFBOData ) )
	{}

	GLRenderTargetBindingImmutableState::~GLRenderTargetBindingImmutableState() = default;

	GLRenderTargetBindingInfo GLRenderTargetBindingImmutableState::GetGLRenderTargetBindingInfo() const
	{
		GLRenderTargetBindingInfo glcRTBindingInfo{};
		glcRTBindingInfo.renderFBO = mGLFBOData.renderFBO.get();
		glcRTBindingInfo.resolveFBO = mGLFBOData.resolveFBO.get();
		glcRTBindingInfo.rtLayout = &mRenderTargetLayout;
		return glcRTBindingInfo;
	}

	TGfxHandle<GLRenderTargetBindingImmutableState> GLRenderTargetBindingImmutableState::CreateInstance(
			GLGpuDevice & pGpuDevice,
			const RenderTargetBindingDefinition & pBindingDefinition )
	{
		if( !SMU::ValidateRenderTargetBindingDefinition( pBindingDefinition ) )
		{
			return nullptr;
		}

		auto glcRenderTargetBindingDefinition = SMU::TranslateRenderTargetBindingDefinition( pBindingDefinition );

		auto immutableState = CreateGfxObject<GLRenderTargetBindingImmutableState>(
				pGpuDevice,
				glcRenderTargetBindingDefinition.rtLayout,
				std::move( glcRenderTargetBindingDefinition.fboData ) );

		return immutableState;
	}

	TGfxHandle<GLRenderTargetBindingImmutableState> GLRenderTargetBindingImmutableState::CreateForScreen(
			GLGpuDevice & pGpuDevice,
			const RenderTargetLayout & pRenderTargetLayout )
	{
		GLRenderTargetBindingDefinition glcRenderTargetBindingDefinition;
		glcRenderTargetBindingDefinition.fboData.renderFBO = GLFramebufferObject::CreateForDefaultFramebuffer();
		glcRenderTargetBindingDefinition.fboData.resolveFBO = nullptr;
		glcRenderTargetBindingDefinition.rtLayout = pRenderTargetLayout;

		auto immutableState = CreateGfxObject<GLRenderTargetBindingImmutableState>(
			pGpuDevice,
			glcRenderTargetBindingDefinition.rtLayout,
			std::move( glcRenderTargetBindingDefinition.fboData ) );

		return immutableState;
	}


	namespace SMU
	{

		GLRenderTargetBindingInfo GetGLRenderTargetBindingInfo(
				const GLRenderTargetBindingDefinition & pBindingDefinition )
		{
			GLRenderTargetBindingInfo glcRTBindingInfo{};
			glcRTBindingInfo.renderFBO = pBindingDefinition.fboData.renderFBO.get();
			glcRTBindingInfo.resolveFBO = pBindingDefinition.fboData.resolveFBO.get();
			glcRTBindingInfo.rtLayout = &( pBindingDefinition.rtLayout );
			return glcRTBindingInfo;
		}

		GLRenderTargetBindingDefinition TranslateRenderTargetBindingDefinition(
				const RenderTargetBindingDefinition & pBindingDefinition )
		{
			GLRenderTargetBindingDefinition glcRenderTargetBindingDefinition{};

			const auto renderTargetLayout = SMU::GetRenderTargetLayoutForBindingDefinition( pBindingDefinition );

			glcRenderTargetBindingDefinition.fboData.renderFBO =
					SMU::CreateFramebufferObject( pBindingDefinition, pBindingDefinition.activeAttachmentsMask );

			glcRenderTargetBindingDefinition.fboData.resolveFBO =
					SMU::CreateFramebufferObject( pBindingDefinition, pBindingDefinition.attachmentsActionResolveMask );

			glcRenderTargetBindingDefinition.rtLayout = renderTargetLayout;

			return glcRenderTargetBindingDefinition;
		}

		GLFramebufferObjectHandle CreateFramebufferObject(
				const RenderTargetBindingDefinition & pBindingDefinition,
				cppx::bitmask<ERTAttachmentFlags> pAttachmentMask )
		{
			if( pAttachmentMask.empty() )
			{
				return nullptr;
			}

			auto framebufferObject = GLFramebufferObject::Create();
			if( !framebufferObject )
			{
				return nullptr;
			}

			for( uint32 caIndex = 0; CxDef::IsRTColorAttachmentIndexValid( caIndex ); ++caIndex )
			{
				const auto attachmentBit = CxDef::makeRTAttachmentFlag( caIndex );
				if( pAttachmentMask.is_set( attachmentBit ) )
				{
					auto & colorAttachmentBinding = pBindingDefinition.colorAttachments[caIndex];
					auto & textureReference = colorAttachmentBinding.attachmentTexture->mTargetTexture;
					auto * openglTexture = textureReference.GetRefTexture()->QueryInterface<GLTexture>();

					framebufferObject->BindColorTexture(
							caIndex,
							*( openglTexture->mGLTextureObject ),
							textureReference.GetRefSubResource() );
				}
			}

			if( pAttachmentMask.is_set( eRtAttachmentFlagDepthStencilBit ) )
			{
				auto & depthStencilAttachmentBinding = pBindingDefinition.depthStencilAttachment;

				if( depthStencilAttachmentBinding.attachmentTexture->IsDepthStencilRenderBuffer() )
				{
					auto * renderBuffer = depthStencilAttachmentBinding.attachmentTexture->GetInternalRenderBuffer<GLInternalRenderBuffer>();

					framebufferObject->BindDepthStencilRenderbuffer(
							*renderBuffer->mGLRenderbufferObject,
							depthStencilAttachmentBinding.attachmentTexture->mRTBufferMask );
				}
				else
				{
					auto & textureReference = depthStencilAttachmentBinding.attachmentTexture->mTargetTexture;
					auto * openglTexture = textureReference.GetRefTexture()->QueryInterface<GLTexture>();

					framebufferObject->BindDepthStencilTexture(
							*openglTexture->mGLTextureObject,
							textureReference.GetRefSubResource(),
							depthStencilAttachmentBinding.attachmentTexture->mRTBufferMask );
				}
			}

			if( !pAttachmentMask.is_set_any_of( eRTAttachmentMaskColorAll ) )
			{
				glDrawBuffer( GL_NONE );
				ic3OpenGLHandleLastError();
			}

			if( !framebufferObject->CheckStatus() )
			{
				ic3DebugInterrupt();
				return nullptr;
			}

			return framebufferObject;
		}

		RenderTargetLayout TranslateSystemVisualConfigToRenderTargetLayout( const System::VisualConfig & pSysVisualConfig )
		{
			RenderTargetLayout renderTargetLayout;
			renderTargetLayout.activeAttachmentsMask = eRTAttachmentMaskDefaultC0DS;
			renderTargetLayout.colorAttachments[0].format = ETextureFormat::BGRA8UN;
			renderTargetLayout.depthStencilAttachment.format = ETextureFormat::D24UNS8U;
			return renderTargetLayout;
		}

		void ClearRenderPassFramebuffer(
				const RenderPassConfiguration & pRenderPassConfiguration,
				const GraphicsPipelineDynamicState & pDynamicState )
		{
			const bool clearConfigOverride = pDynamicState.activeStateMask.is_set(
					eGraphicsPipelineDynamicStateFlagCommonClearConfigBit );

			if( pRenderPassConfiguration.attachmentsActionClearMask != 0 )
			{
				auto colorAttachmentsClearMask = pRenderPassConfiguration.attachmentsActionClearMask & eRTAttachmentMaskColorAll;
				for( uint32 caIndex = 0; CxDef::IsRTColorAttachmentIndexValid( caIndex ); ++caIndex )
				{
					const auto attachmentBit = CxDef::makeRTAttachmentFlag( caIndex );
					if( colorAttachmentsClearMask.is_set( attachmentBit ) )
					{
						const auto & attachmentConfig = pRenderPassConfiguration.colorAttachments[caIndex];
						if( attachmentConfig.clearMask.is_set( E_RENDER_TARGET_BUFFER_FLAG_COLOR_BIT ) )
						{
							const auto * clearConfig = clearConfigOverride ? &pDynamicState.commonClearConfig : &attachmentConfig.clearConfig;

							GLfloat clearArray[4] = {
								clearConfig->colorValue.fpRed,
								clearConfig->colorValue.fpGreen,
								clearConfig->colorValue.fpBlue,
								clearConfig->colorValue.fpAlpha
							};

							glClearBufferfv( GL_COLOR, caIndex, clearArray );
							ic3OpenGLHandleLastError();
						}
						colorAttachmentsClearMask.unset( attachmentBit );
					}

					if( colorAttachmentsClearMask.empty() )
					{
						break;
					}
				}

				if( pRenderPassConfiguration.attachmentsActionClearMask.is_set( eRtAttachmentFlagDepthStencilBit ) )
				{
					const auto & attachmentConfig = pRenderPassConfiguration.depthStencilAttachment;
					const auto * clearConfig = clearConfigOverride ? &pDynamicState.commonClearConfig : &attachmentConfig.clearConfig;

					if( attachmentConfig.clearMask.is_set( E_RENDER_TARGET_BUFFER_FLAG_DEPTH_BIT ) )
					{
						const GLfloat depthValue = clearConfig->depthValue;
						glClearBufferfv( GL_DEPTH, 0, &depthValue );
						ic3OpenGLHandleLastError();
					}

					if( attachmentConfig.clearMask.is_set( E_RENDER_TARGET_BUFFER_FLAG_STENCIL_BIT ) )
					{
						const GLint stencilValue = clearConfig->stencilValue;
						glClearBufferiv( GL_STENCIL, 0, &stencilValue );
						ic3OpenGLHandleLastError();
					}
				}
			}
		}

		void ResolveRenderPassFramebuffer(
				const GLRenderTargetBindingInfo & pRTBindingInfo,
				const RenderPassConfiguration & pRenderPassConfiguration )
		{
			if( pRenderPassConfiguration.attachmentsActionResolveMask != 0 )
			{
				ic3DebugAssert( pRTBindingInfo.resolveFBO );

				GLint drawFramebufferHandle = -1;
				glGetIntegerv( GL_DRAW_FRAMEBUFFER_BINDING, &drawFramebufferHandle );
				ic3OpenGLHandleLastError();

				GLint readFramebufferHandle = -1;
				glGetIntegerv( GL_READ_FRAMEBUFFER_BINDING, &readFramebufferHandle );
				ic3OpenGLHandleLastError();

				const auto & fboImageSize = pRTBindingInfo.rtLayout->sharedImageRect;

				glBindFramebuffer( GL_READ_FRAMEBUFFER, drawFramebufferHandle );
				ic3OpenGLHandleLastError();

				glBindFramebuffer( GL_DRAW_FRAMEBUFFER, pRTBindingInfo.renderFBO->mGLHandle );
				ic3OpenGLHandleLastError();

				auto colorAttachmentsResolveMask = pRenderPassConfiguration.attachmentsActionResolveMask;
				for( uint32 caIndex = 0; CxDef::IsRTColorAttachmentIndexValid( caIndex ); ++caIndex )
				{
					const auto attachmentBit = CxDef::makeRTAttachmentFlag( caIndex );
					if( colorAttachmentsResolveMask.is_set( attachmentBit ) )
					{
						glReadBuffer( GL_COLOR_ATTACHMENT0 + caIndex );
						ic3OpenGLHandleLastError();

						glDrawBuffer( GL_COLOR_ATTACHMENT0 + caIndex );
						ic3OpenGLHandleLastError();

						glBlitFramebuffer(
								0,
								0,
								fboImageSize.width,
								fboImageSize.height,
								0,
								0,
								fboImageSize.width,
								fboImageSize.height,
								GL_COLOR_BUFFER_BIT,
								GL_LINEAR );
						ic3OpenGLHandleLastError();

						colorAttachmentsResolveMask.unset( attachmentBit );

						if( colorAttachmentsResolveMask.empty() )
						{
							break;
						}
					}

					glBindFramebuffer( GL_READ_FRAMEBUFFER, readFramebufferHandle );
					ic3OpenGLHandleLastError();

					glBindFramebuffer( GL_DRAW_FRAMEBUFFER, drawFramebufferHandle );
					ic3OpenGLHandleLastError();
				}
			}
		}

	}

} // namespace Ic3::Graphics::GCI
