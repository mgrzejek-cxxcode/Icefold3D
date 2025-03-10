
#include "GLGraphicsPipelineStateRTO.h"
#include <Ic3/Graphics/HW3D/GL/GLGPUDevice.h>
#include <Ic3/Graphics/HW3D/GL/Objects/GLFramebufferObject.h>
#include <Ic3/Graphics/HW3D/GL/Objects/GLRenderbufferObject.h>
#include <Ic3/Graphics/HW3D/GL/Resources/GLRenderBuffer.h>
#include <Ic3/Graphics/HW3D/GL/Resources/GLTexture.h>

namespace Ic3::Graphics::GCI
{

	GLRenderTargetDescriptor::GLRenderTargetDescriptor(
			GLGPUDevice & pGPUDevice,
			pipeline_state_descriptor_id_t pDescriptorID,
			GLRenderTargetBindingStatic pGLRenderTargetBinding )
	: PIM::RenderTargetDescriptorNative( pGPUDevice, pDescriptorID )
	, mGLRenderTargetBinding( std::move( pGLRenderTargetBinding ) )
	{}

	GLRenderTargetDescriptor::~GLRenderTargetDescriptor() = default;

	bool GLRenderTargetDescriptor::IsAttachmentActive( native_uint pAttachmentIndex ) const noexcept
	{
		Ic3DebugAssert( CXU::RTOIsAttachmentIndexValid( pAttachmentIndex ) );
		return mGLRenderTargetBinding.activeAttachmentsMask.is_set( CXU::RTOMakeAttachmentFlag( pAttachmentIndex ) );
	}

	TGfxHandle<GLRenderTargetDescriptor> GLRenderTargetDescriptor::CreateInstance(
			GLGPUDevice & pGPUDevice,
			pipeline_state_descriptor_id_t pDescriptorID,
			const RenderTargetDescriptorCreateInfo & pCreateInfo )
	{
		if( !GCU::RTOValidateRenderTargetBinding( pCreateInfo.rtArrayBinding ) )
		{
			return nullptr;
		}

		auto glcRenderTargetBindingStatic = GCU::RTOTranslateRenderTargetBindingForStaticDescriptorGL( pCreateInfo.rtArrayBinding );

		auto glcRenderTargetBindingDescriptor = CreateGfxObject<GLRenderTargetDescriptor>(
				pGPUDevice,
				pDescriptorID,
				std::move( glcRenderTargetBindingStatic ) );

		return glcRenderTargetBindingDescriptor;
	}

	TGfxHandle<GLRenderTargetDescriptor> GLRenderTargetDescriptor::CreateForScreen(
			GLGPUDevice & pGPUDevice,
			pipeline_state_descriptor_id_t pDescriptorID,
			const RenderTargetLayout & pRenderTargetLayout )
	{
		GLRenderTargetBindingStatic glcRenderTargetBindingStatic{};
		glcRenderTargetBindingStatic.renderTargetLayout = pRenderTargetLayout;
		glcRenderTargetBindingStatic.activeAttachmentsMask = pRenderTargetLayout.activeAttachmentsMask;
		glcRenderTargetBindingStatic.activeAttachmentsNum = pRenderTargetLayout.activeAttachmentsNum;
		glcRenderTargetBindingStatic.baseFramebuffer = GLFramebufferObject::CreateForDefaultFramebuffer();
		glcRenderTargetBindingStatic.resolveFramebuffer = nullptr;

		auto glcRenderTargetBindingDescriptor = CreateGfxObject<GLRenderTargetDescriptor>(
				pGPUDevice,
				pDescriptorID,
				std::move( glcRenderTargetBindingStatic ) );

		return glcRenderTargetBindingDescriptor;
	}


	namespace GCU
	{

		GLFramebufferObjectHandle RTOCreateFramebufferObject(
				const RenderTargetBinding & pRenderTargetBinding,
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

			GCU::ForEachRTColorAttachmentIndex(
				pAttachmentMask,
				[&pRenderTargetBinding, &framebufferObject]( native_uint pColorAttachmentIndex, ERTAttachmentFlags pAttachmentBit ) -> bool
				{
					auto * colorAttachmentBinding = pRenderTargetBinding.GetAttachment( pColorAttachmentIndex );
					auto & baseTextureReference = colorAttachmentBinding->baseTexture->mTargetTexture;
					auto * openglTexture = baseTextureReference.GetRefTexture()->QueryInterface<GLTexture>();

					framebufferObject->BindColorTexture(
							pColorAttachmentIndex,
							*( openglTexture->mGLTextureObject ),
							baseTextureReference.GetRefSubResource() );

					return true;
				});

			if( pAttachmentMask.is_set( eRTAttachmentFlagDepthStencilBit ) )
			{
				const auto * depthStencilAttachmentBinding = pRenderTargetBinding.GetAttachment( kRTOAttachmentIndexDepthStencil );
				if( depthStencilAttachmentBinding->baseTexture->IsDepthStencilRenderBuffer() )
				{
					auto * renderBuffer = depthStencilAttachmentBinding->baseTexture->GetInternalRenderBuffer<GLInternalRenderBuffer>();

					framebufferObject->BindDepthStencilRenderbuffer(
							*renderBuffer->mGLRenderbufferObject,
							depthStencilAttachmentBinding->baseTexture->mRTBufferMask );
				}
				else
				{
					auto & textureReference = depthStencilAttachmentBinding->baseTexture->mTargetTexture;
					auto * openglTexture = textureReference.GetRefTexture()->QueryInterface<GLTexture>();

					framebufferObject->BindDepthStencilTexture(
							*openglTexture->mGLTextureObject,
							textureReference.GetRefSubResource(),
							depthStencilAttachmentBinding->baseTexture->mRTBufferMask );
				}
			}

			if( !pAttachmentMask.is_set_any_of( eRTAttachmentMaskColorAll ) )
			{
				GLint currentFramebufferHandle = -1;
				glGetIntegerv( GL_FRAMEBUFFER_BINDING, &currentFramebufferHandle );
				Ic3OpenGLHandleLastError();

				glDrawBuffer( GL_NONE );
				Ic3OpenGLHandleLastError();

				glBindFramebuffer( GL_FRAMEBUFFER, currentFramebufferHandle );
				Ic3OpenGLHandleLastError();
			}

			if( !framebufferObject->CheckStatus() )
			{
				Ic3DebugInterrupt();
				return nullptr;
			}

			return framebufferObject;
		}

		GLRenderTargetBindingDynamic RTOTranslateRenderTargetBindingForDynamicDescriptorGL(
				const RenderTargetBinding & pRenderTargetBinding )
		{
			GLRenderTargetBindingDynamic glcRenderTargetBindingDynamicData{};
			RTOUpdateRenderTargetBindingForDynamicDescriptorGL( pRenderTargetBinding, glcRenderTargetBindingDynamicData );
			return glcRenderTargetBindingDynamicData;
		}

		bool RTOUpdateRenderTargetBindingForDynamicDescriptorGL(
				const RenderTargetBinding & pRenderTargetBinding,
				GLRenderTargetBindingDynamic & pOutGLRenderTargetBinding )
		{
			pOutGLRenderTargetBinding.activeAttachmentsMask = pRenderTargetBinding.activeAttachmentsMask;
			pOutGLRenderTargetBinding.activeAttachmentsNum = pRenderTargetBinding.activeAttachmentsNum;
			pOutGLRenderTargetBinding.renderTargetLayout = pRenderTargetBinding.GetRenderTargetLayout();

			pOutGLRenderTargetBinding.baseFramebuffer =
					RTOCreateFramebufferObject( pRenderTargetBinding, pRenderTargetBinding.activeAttachmentsMask );

			pOutGLRenderTargetBinding.resolveFramebuffer =
					RTOCreateFramebufferObject( pRenderTargetBinding, pRenderTargetBinding.GetResolveAttachmentsMask() );

			return true;
		}

		GLRenderTargetBindingStatic RTOTranslateRenderTargetBindingForStaticDescriptorGL(
				const RenderTargetBinding & pRenderTargetBinding )
		{
			GLRenderTargetBindingStatic glcRenderTargetBindingStatic{};
			RTOUpdateRenderTargetBindingForStaticDescriptorGL( pRenderTargetBinding, glcRenderTargetBindingStatic );
			return glcRenderTargetBindingStatic;
		}

		bool RTOUpdateRenderTargetBindingForStaticDescriptorGL(
				const RenderTargetBinding & pRenderTargetBinding,
				GLRenderTargetBindingStatic & pOutGLRenderTargetBinding )
		{
			pOutGLRenderTargetBinding.activeAttachmentsMask = pRenderTargetBinding.activeAttachmentsMask;
			pOutGLRenderTargetBinding.activeAttachmentsNum = pRenderTargetBinding.activeAttachmentsNum;
			pOutGLRenderTargetBinding.renderTargetLayout = pRenderTargetBinding.GetRenderTargetLayout();
			pOutGLRenderTargetBinding.gciBindings = pRenderTargetBinding.attachments;

			pOutGLRenderTargetBinding.baseFramebuffer =
					RTOCreateFramebufferObject( pRenderTargetBinding, pRenderTargetBinding.activeAttachmentsMask );

			pOutGLRenderTargetBinding.resolveFramebuffer =
					RTOCreateFramebufferObject( pRenderTargetBinding, pRenderTargetBinding.GetResolveAttachmentsMask() );

			return true;
		}

		cppx::bitmask<ERTAttachmentFlags> RTORenderPassExecuteOpLoadClearGL(
				const RenderPassConfiguration & pRenderPassConfiguration,
				const GraphicsPipelineDynamicConfig & pDynamicConfig )
		{
			// Dynamic configuration overwrites the values specified via descriptors.
			// The activeStateMask member tells us which state override is enabled.
			const bool clearConfigOverride = pDynamicConfig.activeStateMask.is_set(
					eGraphicsPipelineDynamicConfigFlagRenderTargetClearConfigBit );

			const auto attachmentsLoadClearMask =
					pRenderPassConfiguration.GetAttachmentsMaskWithLoadFlags( eRenderPassAttachmentActionFlagLoadClearBit );

			auto clearedAttachmentsMask = cppx::make_bitmask_tp<ERTAttachmentFlags>();

			if( !attachmentsLoadClearMask.empty() )
			{
				ForEachRTColorAttachmentIndex(
					attachmentsLoadClearMask,
					[&]( native_uint pColorAttachmentIndex, ERTAttachmentFlags pAttachmentBit ) -> bool {
						const auto * attachmentConfig = pRenderPassConfiguration.GetAttachment( pColorAttachmentIndex );
						const auto * clearConfig = &( attachmentConfig->loadParameters.opClear.clearConfig );

						if( clearConfigOverride )
						{
							clearConfig = &( pDynamicConfig.renderTargetClearConfig );
						}

						GLfloat clearColorArray[4] = {
							clearConfig->colorValue.fpRed,
							clearConfig->colorValue.fpGreen,
							clearConfig->colorValue.fpBlue,
							clearConfig->colorValue.fpAlpha
						};

						glClearBufferfv( GL_COLOR, pColorAttachmentIndex, clearColorArray );
						Ic3OpenGLHandleLastError();

						clearedAttachmentsMask.set( pAttachmentBit );

						return true;
					});

				if( attachmentsLoadClearMask.is_set( eRTAttachmentFlagDepthStencilBit ) )
				{
					const auto * attachmentConfig = pRenderPassConfiguration.GetAttachment( kRTOAttachmentIndexDepthStencil );
					const auto * clearConfig = &( attachmentConfig->loadParameters.opClear.clearConfig );

					if( clearConfigOverride )
					{
						clearConfig = &( pDynamicConfig.renderTargetClearConfig );
					}

					if( attachmentConfig->loadParameters.opClear.clearMask.is_set( eRenderTargetBufferFlagDepthBit ) )
					{
						const GLfloat depthValue = clearConfig->depthValue;
						glClearBufferfv( GL_DEPTH, 0, &depthValue );
						Ic3OpenGLHandleLastError();
					}

					if( attachmentConfig->loadParameters.opClear.clearMask.is_set( eRenderTargetBufferFlagStencilBit ) )
					{
						const GLint stencilValue = clearConfig->stencilValue;
						glClearBufferiv( GL_STENCIL, 0, &stencilValue );
						Ic3OpenGLHandleLastError();
					}

					clearedAttachmentsMask.set( eRTAttachmentFlagDepthStencilBit );
				}
			}

			return clearedAttachmentsMask;
		}

		cppx::bitmask<ERTAttachmentFlags> RTORenderPassExecuteOpStoreResolveGL(
				const RenderPassConfiguration & pRenderPassConfiguration,
				const GLRenderTargetBinding & pGLRenderTargetBinding )
		{
			const auto attachmentsStoreResolveMask =
					pRenderPassConfiguration.GetAttachmentsMaskWithLoadFlags( eRenderPassAttachmentActionFlagLoadClearBit );

			auto resolvedAttachmentsMask = cppx::make_bitmask_tp<ERTAttachmentFlags>();

			if( !attachmentsStoreResolveMask.empty() )
			{
				Ic3DebugAssert( pGLRenderTargetBinding.resolveFramebuffer );

				// This function works with the currently bound draw framebuffer object (it is called in the render pass
				// scope when the rendering ends - all render pass bindings are expected to be valid at this point).
				// Hence, we need to save the current FBO bindings and restore them afterwards.

				GLint drawFramebufferHandle = -1;
				glGetIntegerv( GL_DRAW_FRAMEBUFFER_BINDING, &drawFramebufferHandle );
				Ic3OpenGLHandleLastError();

				GLint readFramebufferHandle = -1;
				glGetIntegerv( GL_READ_FRAMEBUFFER_BINDING, &readFramebufferHandle );
				Ic3OpenGLHandleLastError();

				glBindFramebuffer( GL_READ_FRAMEBUFFER, drawFramebufferHandle );
				Ic3OpenGLHandleLastError();

				glBindFramebuffer( GL_DRAW_FRAMEBUFFER, pGLRenderTargetBinding.resolveFramebuffer->mGLHandle );
				Ic3OpenGLHandleLastError();

				const auto & fboImageSize = pGLRenderTargetBinding.renderTargetLayout.sharedImageSize;

				ForEachRTColorAttachmentIndex(
					attachmentsStoreResolveMask,
					[&]( native_uint pColorAttachmentIndex, ERTAttachmentFlags pAttachmentBit ) -> bool {
						// glReadBuffer() specifies the index of an attachment within the GL_READ_FRAMEBUFFER
						// that will be used for subsequent pixel operations. glDrawBuffer(), obviously, does the same
						// for the GL_DRAW_FRAMEBUFFER. Set those to the same attachment so we can resolve it.

						glReadBuffer( GL_COLOR_ATTACHMENT0 + pColorAttachmentIndex );
						Ic3OpenGLHandleLastError();

						glDrawBuffer( GL_COLOR_ATTACHMENT0 + pColorAttachmentIndex );
						Ic3OpenGLHandleLastError();

						// Framebuffer blit will do a copy from the READ to DRAW FBO, resolving the contents in the process.
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
						Ic3OpenGLHandleLastError();

						resolvedAttachmentsMask.set( pAttachmentBit );

						return true;
					});

				if( attachmentsStoreResolveMask.is_set( eRTAttachmentFlagDepthStencilBit ) )
				{
					glReadBuffer( GL_DEPTH_STENCIL_ATTACHMENT );
					Ic3OpenGLHandleLastError();

					glDrawBuffer( GL_DEPTH_STENCIL_ATTACHMENT );
					Ic3OpenGLHandleLastError();

					glBlitFramebuffer(
							0,
							0,
							fboImageSize.width,
							fboImageSize.height,
							0,
							0,
							fboImageSize.width,
							fboImageSize.height,
							GL_DEPTH_BUFFER_BIT,
							GL_NEAREST );
					Ic3OpenGLHandleLastError();

					resolvedAttachmentsMask.set( eRTAttachmentFlagDepthStencilBit );
				}

				glBindFramebuffer( GL_READ_FRAMEBUFFER, readFramebufferHandle );
				Ic3OpenGLHandleLastError();

				glBindFramebuffer( GL_DRAW_FRAMEBUFFER, drawFramebufferHandle );
				Ic3OpenGLHandleLastError();
			}

			return resolvedAttachmentsMask;
		}

	}

} // namespace Ic3::Graphics::GCI
