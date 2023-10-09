
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

		static constexpr auto sEquationDataSize = sizeof( Equation );
		static constexpr auto sFactorDataSize = sizeof( Factor );
	};

	struct GLBlendConfig
	{
		Bitmask<ERTAttachmentFlags> attachmentsMask;
		Bitmask<EBlendConfigFlags> flags;
		GLRTColorAttachmentBlendSettings attachments[gpm::RT_MAX_COLOR_ATTACHMENTS_NUM];
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
	#if( ICFGX_GL_FEATURE_SUPPORT_PRIMITIVE_FILL_MODE )
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
				GLGPUDevice & pGPUDevice,
				Bitmask<EBlendConfigFlags> pBlendFlags,
				const GLBlendConfig & pGLBlendConfig );

		virtual ~GLBlendImmutableState();

		static GpaHandle<GLBlendImmutableState> createInstance( GLGPUDevice & pGPUDevice, const BlendConfig & pBlendConfig );
	};

	///
	class GLDepthStencilImmutableState : public DepthStencilImmutableState
	{
	public:
		GLDepthStencilConfig const mGLDepthStencilConfig;

	public:
		GLDepthStencilImmutableState(
				GLGPUDevice & pGPUDevice,
				Bitmask<EDepthStencilConfigFlags> pDepthStencilFlags,
				const GLDepthStencilConfig & pGLDepthStencilConfig );

		virtual ~GLDepthStencilImmutableState();

		static GpaHandle<GLDepthStencilImmutableState> createInstance( GLGPUDevice & pGPUDevice, const DepthStencilConfig & pDepthStencilConfig );
	};

	///
	class GLRasterizerImmutableState : public RasterizerImmutableState
	{
	public:
		GLRasterizerConfig const mGLRasterizerConfig;

	public:
		GLRasterizerImmutableState(
				GLGPUDevice & pGPUDevice,
				Bitmask<ERasterizerConfigFlags> pRasterizerFlags,
				const GLRasterizerConfig & pGLRasterizerConfig );

		virtual ~GLRasterizerImmutableState();

		static GpaHandle<GLRasterizerImmutableState> createInstance( GLGPUDevice & pGPUDevice, const RasterizerConfig & pRasterizerConfig );
	};


	namespace smutil
	{

		IC3_ATTR_NO_DISCARD GLBlendConfig translateBlendConfigGL( const BlendConfig & pConfig );

		IC3_ATTR_NO_DISCARD GLDepthStencilConfig translateDepthStencilConfigGL( const DepthStencilConfig & pConfig );

		IC3_ATTR_NO_DISCARD GLRasterizerConfig translateRasterizerConfigGL( const RasterizerConfig & pConfig );

		IC3_ATTR_NO_DISCARD GLRTColorAttachmentBlendSettings translateRTColorAttachmentBlendSettingsGL(
				const RTColorAttachmentBlendSettings & pSettings );

	}

}

#endif // __IC3_GRAPHICS_HW3D_GLCOMMON_COMMON_GRAPHICS_CONFIG_H__
