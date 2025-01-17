
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

		using SamplerBindings = std::array<GLuint, GCM::cxResMaxTextureUnitsNum>;
		using TextureUnitBindings = std::array<TextureUnitBinding, GCM::cxResMaxTextureUnitsNum>;

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

		void Reset();

		void ApplyShaderPipelineBinding( GLuint pShaderPipelineHandle );
		void ApplyShaderProgramBinding( GLuint pShaderProgramHandle );
		void ApplyIndexBufferBinding( GLuint pIndexBufferObjectHandle );
		void ApplyVertexArrayObjectBinding( GLuint pVertexArrayObjectHandle );

		void ApplyBlendState( const GLBlendConfig & pBlendConfig );
		void ApplyDepthStencilState( const GLDepthStencilConfig & pDepthStencilConfig, uint8 pStencilRefValue );
		void ApplyRasterizerState( const GLRasterizerConfig & pRasterizerConfig );

		static GLGlobalState GetDefaultGlobalState();

	private:
		GLGlobalState _cachedState;
	};

}

#endif // __IC3_GRAPHICS_HW3D_GLCOMMON_GLOBAL_STATE_CACHE_H__
