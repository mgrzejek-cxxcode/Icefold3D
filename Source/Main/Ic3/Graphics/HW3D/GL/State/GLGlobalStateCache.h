
#ifndef __IC3_GRAPHICS_HW3D_GLCOMMON_GLOBAL_STATE_CACHE_H__
#define __IC3_GRAPHICS_HW3D_GLCOMMON_GLOBAL_STATE_CACHE_H__

#include "GLCommonGraphicsConfig.h"
#include "GLInputAssembler.h"

namespace Ic3::Graphics::GCI
{

	struct GLGlobalState
	{
		struct BlendConfig : public GLBlendConfig
		{
			uint32 blendActiveGlobal;
		};

		struct TextureUnitBinding
		{
			GLenum textureType;
			GLuint textureHandle;
		};

		using SamplerBindings = std::array<GLuint, GCM::RES_MAX_TEXTURE_UNITS_NUM>;
		using TextureUnitBindings = std::array<TextureUnitBinding, GCM::RES_MAX_TEXTURE_UNITS_NUM>;

		GLuint shaderPipelineBinding;
		GLuint shaderProgramBinding;
		GLuint vertexArrayObjectBinding;
		GLuint indexBufferBinding;

		BlendConfig blendConfig;
		GLDepthStencilConfig depthStencilConfig;
		GLRasterizerConfig rasterizerConfig;
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

		void reset();

		void applyShaderPipelineBinding( GLuint pShaderPipelineHandle );
		void applyShaderProgramBinding( GLuint pShaderProgramHandle );
		void applyIndexBufferBinding( GLuint pIndexBufferObjectHandle );
		void applyVertexArrayObjectBinding( GLuint pVertexArrayObjectHandle );

		void applyBlendState( const GLBlendConfig & pBlendConfig );
		void applyDepthStencilState( const GLDepthStencilConfig & pDepthStencilConfig, uint8 pStencilRefValue );
		void applyRasterizerState( const GLRasterizerConfig & pRasterizerConfig );

		static GLGlobalState getDefaultGlobalState();

	private:
		GLGlobalState _cachedState;
	};

}

#endif // __IC3_GRAPHICS_HW3D_GLCOMMON_GLOBAL_STATE_CACHE_H__
