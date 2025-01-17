
#ifndef __IC3_GRAPHICS_HW3D_GLCOMMON_COMMON_GRAPHICS_CONFIG_H__
#define __IC3_GRAPHICS_HW3D_GLCOMMON_COMMON_GRAPHICS_CONFIG_H__

#include "../GLPrerequisites.h"
#include <Ic3/Graphics/GCI/State/CommonGraphicsConfigImmutableStates.h>

namespace Ic3::Graphics::GCI
{

	struct GLRTColorAttachmentBlendSettings
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

	struct GLBlendConfig
	{
		cppx::bitmask<ERTAttachmentFlags> attachmentsMask;
		cppx::bitmask<EBlendConfigFlags> flags;
		GLRTColorAttachmentBlendSettings attachments[GCM::cxRTMaxColorAttachmentsNum];
		Math::RGBAColorR32Norm constantColor;
	};

	struct GLDepthStencilConfig
	{
		struct GLDepthSettings
		{
			GLenum depthCompFunc;
			GLuint writeMask;
		};

		struct GLStencilFaceDesc
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
			GLStencilFaceDesc frontFace;
			GLStencilFaceDesc backFace;
		};

		uint16 depthTestActive;
		uint16 stencilTestActive;
		GLDepthSettings depthSettings;
		GLStencilSettings stencilSettings;
	};

	struct GLRasterizerConfig
	{
		uint32 scissorTestActive;
		GLenum cullMode;
		GLenum frontFaceVerticesOrder;
	#if( IC3_GX_GL_FEATURE_SUPPORT_PRIMITIVE_FILL_MODE )
		GLenum primitiveFillMode;
	#endif
	};


	///
	class GLBlendImmutableState : public BlendImmutableState
	{
	public:
		GLBlendConfig const mGLBlendConfig;

	public:
		GLBlendImmutableState(
				GLGpuDevice & pGpuDevice,
				cppx::bitmask<EBlendConfigFlags> pBlendFlags,
				const GLBlendConfig & pGLBlendConfig );

		virtual ~GLBlendImmutableState();

		static TGfxHandle<GLBlendImmutableState> CreateInstance( GLGpuDevice & pGpuDevice, const BlendConfig & pBlendConfig );
	};

	///
	class GLDepthStencilImmutableState : public DepthStencilImmutableState
	{
	public:
		GLDepthStencilConfig const mGLDepthStencilConfig;

	public:
		GLDepthStencilImmutableState(
				GLGpuDevice & pGpuDevice,
				cppx::bitmask<EDepthStencilConfigFlags> pDepthStencilFlags,
				const GLDepthStencilConfig & pGLDepthStencilConfig );

		virtual ~GLDepthStencilImmutableState();

		static TGfxHandle<GLDepthStencilImmutableState> CreateInstance( GLGpuDevice & pGpuDevice, const DepthStencilConfig & pDepthStencilConfig );
	};

	///
	class GLRasterizerImmutableState : public RasterizerImmutableState
	{
	public:
		GLRasterizerConfig const mGLRasterizerConfig;

	public:
		GLRasterizerImmutableState(
				GLGpuDevice & pGpuDevice,
				cppx::bitmask<ERasterizerConfigFlags> pRasterizerFlags,
				const GLRasterizerConfig & pGLRasterizerConfig );

		virtual ~GLRasterizerImmutableState();

		static TGfxHandle<GLRasterizerImmutableState> CreateInstance( GLGpuDevice & pGpuDevice, const RasterizerConfig & pRasterizerConfig );
	};


	namespace SMU
	{

		CPPX_ATTR_NO_DISCARD GLBlendConfig TranslateBlendConfigGL( const BlendConfig & pConfig );

		CPPX_ATTR_NO_DISCARD GLDepthStencilConfig TranslateDepthStencilConfigGL( const DepthStencilConfig & pConfig );

		CPPX_ATTR_NO_DISCARD GLRasterizerConfig TranslateRasterizerConfigGL( const RasterizerConfig & pConfig );

		CPPX_ATTR_NO_DISCARD GLRTColorAttachmentBlendSettings TranslateRTColorAttachmentBlendSettingsGL(
				const RTColorAttachmentBlendSettings & pSettings );

	}

}

#endif // __IC3_GRAPHICS_HW3D_GLCOMMON_COMMON_GRAPHICS_CONFIG_H__
