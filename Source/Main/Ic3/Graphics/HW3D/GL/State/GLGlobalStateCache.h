
#ifndef __IC3_GRAPHICS_HW3D_GLC_GLOBAL_STATE_CACHE_H__
#define __IC3_GRAPHICS_HW3D_GLC_GLOBAL_STATE_CACHE_H__

#include "GLGraphicsPipelineStateCommon.h"
#include "GLGraphicsPipelineStateIA.h"

namespace Ic3::Graphics::GCI
{

	struct GLGlobalState
	{
		struct BlendSettings : public GLBlendSettings
		{
			uint32 blendActiveGlobal;
		};

		struct TextureUnitBinding
		{
			GLenum textureType;
			GLuint textureHandle;
		};

		using SamplerBindings = std::array<GLuint, GCM::kResMaxTextureUnitsNum>;
		using TextureUnitBindings = std::array<TextureUnitBinding, GCM::kResMaxTextureUnitsNum>;

		GLuint shaderPipelineBinding;
		GLuint shaderProgramBinding;
		GLuint vertexArrayObjectBinding;
		GLuint indexBufferBinding;

		BlendSettings blendSettings;
		GLDepthStencilSettings depthStencilSettings;
		GLRasterizerSettings rasterizerSettings;
		SamplerBindings samplerBindings;
		TextureUnitBindings textureUnitBindings;
	};

	class GLGlobalStateCache
	{
	public:
		static const GLGlobalState sDefaultState;

	public:
		GLGlobalStateCache();
		~GLGlobalStateCache() = default;

		void ApplyShaderPipelineBinding( GLuint pShaderPipelineHandle );
		void ApplyShaderProgramBinding( GLuint pShaderProgramHandle );
		void ApplyIndexBufferBinding( GLuint pIndexBufferObjectHandle );
		void ApplyVertexArrayObjectBinding( GLuint pVertexArrayObjectHandle );

		void ApplyBlendSettings( const GLBlendSettings & pBlendSettings, bool pSetConstantColor );
		void ApplyDepthStencilSettings( const GLDepthStencilSettings & pDepthStencilSettings, uint8 pStencilTestRefValueO );
		void ApplyRasterizerSettings( const GLRasterizerSettings & pRasterizerSettings );

		void SetBlendConstantColor( const cxm::rgba_color_r32_norm & pConstantColor );

		void Reset();

		static GLGlobalState GetDefaultGlobalState();

	private:
		GLGlobalState _cachedState;
	};

}

#endif // __IC3_GRAPHICS_HW3D_GLC_GLOBAL_STATE_CACHE_H__
