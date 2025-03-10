
#ifndef __IC3_GRAPHICS_HW3D_GLC_GRAPHICS_PIPELINE_STATE_COMMON_H__
#define __IC3_GRAPHICS_HW3D_GLC_GRAPHICS_PIPELINE_STATE_COMMON_H__

#include "../GLPrerequisites.h"
#include <Ic3/Graphics/GCI/State/GraphicsPipelineStateDescriptorCommon.h>

namespace Ic3::Graphics::GCI
{

	struct GLRenderTargetColorAttachmentBlendSettings
	{
		struct Equation
		{
			GLenum rgb;
			GLenum alpha;
		};

		struct Factor
		{
			GLenum rgbSrc;
			GLenum rgbDst;
			GLenum alphaSrc;
			GLenum alphaDst;
		};

		uint32 blendActive;
		Equation equation;
		Factor factor;
	};

	struct GLBlendSettings
	{
		cppx::bitmask<ERTAttachmentFlags> attachmentsMask;
		cppx::bitmask<EBlendConfigFlags> flags;
		GLRenderTargetColorAttachmentBlendSettings attachments[GCM::kRTOMaxColorAttachmentsNum];
		cxm::rgba_color_r32_norm constantColor;
	};

	struct GLDepthStencilSettings
	{
		struct GLDepthSettings
		{
			GLenum depthCompFunc;
			GLuint writeMask;
		};

		struct GLStencilFaceOp
		{
			GLenum compFunc;
			GLuint readMask;
			GLenum opFail;
			GLenum opPassDepthFail;
			GLenum opPassDepthPass;
			GLuint writeMask;
		};

		struct GLStencilSettings
		{
			GLStencilFaceOp frontFace;
			GLStencilFaceOp backFace;
		};

		uint16 depthTestActive;
		uint16 stencilTestActive;
		GLDepthSettings depthSettings;
		GLStencilSettings stencilSettings;
	};

	struct GLRasterizerSettings
	{
		uint32 scissorTestActive;
		GLenum cullMode;
		GLenum frontFaceVerticesOrder;
	#if( IC3_GX_GL_FEATURE_SUPPORT_PRIMITIVE_FILL_MODE )
		GLenum primitiveFillMode;
	#endif
	};

	/**
	 *
	 */
	class GLBlendStateDescriptor : public BlendStateDescriptor
	{
	public:
		GLBlendSettings const mGLBlendSettings;

	public:
		GLBlendStateDescriptor(
				GLGPUDevice & pGPUDevice,
				pipeline_state_descriptor_id_t pDescriptorID,
				cppx::bitmask<EBlendConfigFlags> pBlendFlags,
				const GLBlendSettings & pGLBlendSettings );

		virtual ~GLBlendStateDescriptor();

		static TGfxHandle<GLBlendStateDescriptor> CreateInstance(
				GLGPUDevice & pGPUDevice,
				const BlendStateDescriptorCreateInfo & pCreateInfo );
	};

	/**
	 *
	 */
	class GLDepthStencilStateDescriptor : public DepthStencilStateDescriptor
	{
	public:
		GLDepthStencilSettings const mGLDepthStencilSettings;

	public:
		GLDepthStencilStateDescriptor(
				GLGPUDevice & pGPUDevice,
				pipeline_state_descriptor_id_t pDescriptorID,
				cppx::bitmask<EDepthStencilConfigFlags> pDepthStencilFlags,
				const GLDepthStencilSettings & pGLDepthStencilSettings );

		virtual ~GLDepthStencilStateDescriptor();

		static TGfxHandle<GLDepthStencilStateDescriptor> CreateInstance(
				GLGPUDevice & pGPUDevice,
				const DepthStencilStateDescriptorCreateInfo & pCreateInfo );
	};

	/**
	 *
	 */
	class GLRasterizerStateDescriptor : public RasterizerStateDescriptor
	{
	public:
		GLRasterizerSettings const mGLRasterizerSettings;

	public:
		GLRasterizerStateDescriptor(
				GLGPUDevice & pGPUDevice,
				pipeline_state_descriptor_id_t pDescriptorID,
				cppx::bitmask<ERasterizerConfigFlags> pRasterizerFlags,
				const GLRasterizerSettings & pGLRasterizerSettings );

		virtual ~GLRasterizerStateDescriptor();

		static TGfxHandle<GLRasterizerStateDescriptor> CreateInstance(
				GLGPUDevice & pGPUDevice,
				const RasterizerStateDescriptorCreateInfo & pCreateInfo );
	};


	namespace GCU
	{

		CPPX_ATTR_NO_DISCARD GLBlendSettings TranslateBlendSettingsGL( const BlendSettings & pSettings );

		CPPX_ATTR_NO_DISCARD GLDepthStencilSettings TranslateDepthStencilSettingsGL( const DepthStencilSettings & pSettings );

		CPPX_ATTR_NO_DISCARD GLRasterizerSettings TranslateRasterizerSettingsGL( const RasterizerSettings & pSettings );

		CPPX_ATTR_NO_DISCARD GLRenderTargetColorAttachmentBlendSettings TranslateRenderTargetColorAttachmentBlendSettingsGL(
				const RenderTargetColorAttachmentBlendSettings & pSettings );

	}

}

#endif // __IC3_GRAPHICS_HW3D_GLC_GRAPHICS_PIPELINE_STATE_COMMON_H__
