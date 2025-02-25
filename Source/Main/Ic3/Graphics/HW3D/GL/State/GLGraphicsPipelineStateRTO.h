
#pragma once

#ifndef __IC3_GRAPHICS_HW3D_GLC_GRAPHICS_PIPELINE_STATE_RTO_H__
#define __IC3_GRAPHICS_HW3D_GLC_GRAPHICS_PIPELINE_STATE_RTO_H__

#include "../GLPrerequisites.h"
#include <Ic3/Graphics/GCI/State/GraphicsPipelineStateDescriptorRTO.h>

namespace Ic3::Graphics::GCI
{

	Ic3GLDeclareOpenGLObjectHandle( GLFramebufferObject );
	Ic3GLDeclareOpenGLObjectHandle( GLRenderbufferObject );

	struct GLRenderPassAttachmentConfig
	{
		RenderPassActionLoadParameters loadParameters;
		RenderPassActionStoreParameters storeParameters;
	};

	struct GLRenderTargetFramebufferData
	{
		GLFramebufferObjectHandle baseFramebuffer;
		GLFramebufferObjectHandle resolveFramebuffer;
	};

	struct GLRenderTargetBinding : public RenderTargetArrayCommonConfig, public GLRenderTargetFramebufferData
	{
		RenderTargetLayout renderTargetLayout;
	};

	/**
	 * GL API-level data for render target binding defined via dynamic RT descriptors.
	 * Dynamic RT descriptors store GCI::RenderTargetBinding, so they contain active
	 * texture references on their own - on the GL level, we only need FBOs.
	 */
	struct GLRenderTargetBindingDynamic : public GLRenderTargetBinding
	{
		// Dynamic RT descriptors store GCI::RenderTargetBinding, so they contain active
		// texture references on their own - on the GL level, we only need FBOs.
	};

	/**
	 * GL API-level data for render target binding defined via static (translated to GL) RT descriptors.
	 * Static RT descriptors do not have any state at the GCI level, so we need to save bound resources,
	 * so their references are retained and textures kept alive as long as their parent descriptor is valid.
	 */
	struct GLRenderTargetBindingStatic : public GLRenderTargetBinding
	{
		// An array of RenderTargetAttachmentBinding objects, holding
		// active references to the textures/buffers used by a descriptor.
		TRenderTargetAttachmentPropertyArray<RenderTargetAttachmentBinding> gciBindings;
	};


	class GLRenderTargetDescriptor : public PIM::RenderTargetDescriptorNative
	{
	public:
		GLRenderTargetBindingStatic const mGLRenderTargetBinding;

	public:
		GLRenderTargetDescriptor(
				GLGPUDevice & pGPUDevice,
				pipeline_state_descriptor_id_t pDescriptorID,
				GLRenderTargetBindingStatic pGLRenderTargetBinding );

		virtual ~GLRenderTargetDescriptor();

		CPPX_ATTR_NO_DISCARD virtual bool IsAttachmentActive( uint32 pAttachmentIndex ) const noexcept override final;

		static TGfxHandle<GLRenderTargetDescriptor> CreateInstance(
				GLGPUDevice & pGPUDevice,
				const RenderTargetDescriptorCreateInfo & pCreateInfo );

		static TGfxHandle<GLRenderTargetDescriptor> CreateForScreen(
				GLGPUDevice & pGPUDevice,
				pipeline_state_descriptor_id_t pDescriptorID,
				const RenderTargetLayout & pRenderTargetLayout );
	};


	namespace GCU
	{

		CPPX_ATTR_NO_DISCARD GLFramebufferObjectHandle RTOCreateFramebufferObjectGL(
				const RenderTargetBinding & pRenderTargetBinding,
				cppx::bitmask<ERTAttachmentFlags> pAttachmentMask );

		CPPX_ATTR_NO_DISCARD GLRenderTargetBindingDynamic RTOTranslateRenderTargetBindingForDynamicDescriptorGL(
				const RenderTargetBinding & pRenderTargetBinding );

		bool RTOUpdateRenderTargetBindingForDynamicDescriptorGL(
				const RenderTargetBinding & pRenderTargetBinding,
				GLRenderTargetBindingDynamic & pOutGLRenderTargetBinding );

		CPPX_ATTR_NO_DISCARD GLRenderTargetBindingStatic RTOTranslateRenderTargetBindingForStaticDescriptorGL(
				const RenderTargetBinding & pRenderTargetBinding );

		bool RTOUpdateRenderTargetBindingForStaticDescriptorGL(
				const RenderTargetBinding & pRenderTargetBinding,
				GLRenderTargetBindingStatic & pOutGLRenderTargetBinding );

		cppx::bitmask<ERTAttachmentFlags> RTORenderPassExecuteOpLoadClearGL(
				const RenderPassConfiguration & pRenderPassConfiguration,
				const GraphicsPipelineDynamicConfig & pDynamicConfig );

		cppx::bitmask<ERTAttachmentFlags> RTORenderPassExecuteOpStoreResolveGL(
				const RenderPassConfiguration & pRenderPassConfiguration,
				const GLRenderTargetBinding & pGLRenderTargetBinding );

	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_HW3D_GLC_GRAPHICS_PIPELINE_STATE_RTO_H__
