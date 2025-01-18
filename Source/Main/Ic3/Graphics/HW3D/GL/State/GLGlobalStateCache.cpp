
#include "GLGlobalStateCache.h"

namespace Ic3::Graphics::GCI
{

	const GLGlobalState GLGlobalStateCache::sDefaultState = GLGlobalStateCache::GetDefaultGlobalState();

	GLGlobalStateCache::GLGlobalStateCache()
	{
		Reset();
	}

	void GLGlobalStateCache::Reset()
	{
		_cachedState = sDefaultState;
	}

	void GLGlobalStateCache::ApplyShaderPipelineBinding( GLuint pShaderPipelineHandle )
	{
		if( pShaderPipelineHandle != _cachedState.shaderPipelineBinding )
		{
			glBindProgramPipeline( pShaderPipelineHandle );
			Ic3OpenGLHandleLastError();
			_cachedState.shaderPipelineBinding = pShaderPipelineHandle;
		}
	}

	void GLGlobalStateCache::ApplyShaderProgramBinding( GLuint pShaderProgramHandle )
	{
		if( pShaderProgramHandle != _cachedState.shaderProgramBinding )
		{
			glUseProgram( pShaderProgramHandle );
			Ic3OpenGLHandleLastError();
			_cachedState.shaderProgramBinding = pShaderProgramHandle;
		}
	}

	void GLGlobalStateCache::ApplyIndexBufferBinding( GLuint pIndexBufferObjectHandle )
	{
		if( pIndexBufferObjectHandle != _cachedState.indexBufferBinding )
		{
			glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, pIndexBufferObjectHandle );
			Ic3OpenGLHandleLastError();
			_cachedState.indexBufferBinding = pIndexBufferObjectHandle;
		}
	}

	void GLGlobalStateCache::ApplyVertexArrayObjectBinding( GLuint pVertexArrayObjectHandle )
	{
		if( pVertexArrayObjectHandle != _cachedState.vertexArrayObjectBinding )
		{
			glBindVertexArray( pVertexArrayObjectHandle );
			Ic3OpenGLHandleLastError();
			_cachedState.vertexArrayObjectBinding = pVertexArrayObjectHandle;
			_cachedState.indexBufferBinding = 0;
		}
	}

	void GLGlobalStateCache::ApplyBlendState( const GLBlendConfig & pBlendConfig )
	{
		auto & cachedBlendConfig = _cachedState.blendConfig;

		if( !pBlendConfig.attachmentsMask.is_set_any_of( eRTAttachmentMaskColorAll ) )
		{
			if( cachedBlendConfig.blendActiveGlobal )
			{
				glDisable( GL_BLEND );
				Ic3OpenGLHandleLastError();

				cachedBlendConfig.blendActiveGlobal = false;
			}
		}
		else
		{
			const bool isSeparateBlendingEnabled = cachedBlendConfig.flags.is_set(
					eBlendConfigFlagEnableMRTIndependentBlendingBit );
			const bool isSeparateBlendingRequested = pBlendConfig.flags.is_set(
					eBlendConfigFlagEnableMRTIndependentBlendingBit );

			if( isSeparateBlendingRequested )
			{
				bool compareCurrentState = isSeparateBlendingEnabled;

				for( uint32 caIndex = 0; CxDef::IsRTColorAttachmentIndexValid( caIndex ); ++caIndex )
				{
					auto & cachedBlendProps = cachedBlendConfig.attachments[caIndex];
					const auto & blendProps = pBlendConfig.attachments[caIndex];

					if( !blendProps.blendActive )
					{
						if( !compareCurrentState || cachedBlendProps.blendActive )
						{
							glDisablei( GL_BLEND, caIndex );
							Ic3OpenGLHandleLastError();
							cachedBlendProps.blendActive = 0;
						}
					}
					else // Enable blending for this attachment
					{
						if( !compareCurrentState || !cachedBlendProps.blendActive )
						{
							glEnablei( GL_BLEND, caIndex );
							Ic3OpenGLHandleLastError();
							cachedBlendProps.blendActive = 1;
						}

						if( !compareCurrentState || cppx::mem_cmp_not_equal( blendProps.equation, cachedBlendProps.equation ) )
						{
							glBlendEquationSeparatei( caIndex, blendProps.equation.rgb, blendProps.equation.alpha );
							Ic3OpenGLHandleLastError();
							cachedBlendProps.equation = blendProps.equation;
						}

						if( !compareCurrentState || cppx::mem_cmp_not_equal( blendProps.factor, cachedBlendProps.factor ) )
						{
							glBlendFuncSeparatei( caIndex, blendProps.factor.rgbSrc, blendProps.factor.rgbDst, blendProps.factor.alphaSrc, blendProps.factor.alphaDst );
							Ic3OpenGLHandleLastError();
							cachedBlendProps.factor = blendProps.factor;
						}
					}
				}
			}
			else
			{
				bool compareCurrentState = !isSeparateBlendingEnabled;

				if( !compareCurrentState || !cachedBlendConfig.blendActiveGlobal  )
				{
					glEnable( GL_BLEND );
					Ic3OpenGLHandleLastError();
					cachedBlendConfig.blendActiveGlobal = true;
				}

				auto & cachedBlendProps = cachedBlendConfig.attachments[0];
				const auto & blendProps = pBlendConfig.attachments[0];

				if( !compareCurrentState || cppx::mem_cmp_not_equal( blendProps.equation, cachedBlendProps.equation ) )
				{
					glBlendEquationSeparate( blendProps.equation.rgb, blendProps.equation.alpha );
					Ic3OpenGLHandleLastError();
					cachedBlendProps.equation = blendProps.equation;
				}

				if( !compareCurrentState || cppx::mem_cmp_not_equal( blendProps.factor, cachedBlendProps.factor ) )
				{
					glBlendFuncSeparate( blendProps.factor.rgbSrc, blendProps.factor.rgbDst, blendProps.factor.alphaSrc, blendProps.factor.alphaDst );
					Ic3OpenGLHandleLastError();
					cachedBlendProps.factor = blendProps.factor;
				}
			}

			if( pBlendConfig.flags.is_set_any_of( eBlendConfigFlagSetFixedBlendConstantsBit ) )
			{
				const auto & blendConstantColor = pBlendConfig.constantColor;

				if( cppx::mem_cmp_not_equal( blendConstantColor, cachedBlendConfig.constantColor ) )
				{
					glBlendColor( blendConstantColor.fpRed, blendConstantColor.fpGreen, blendConstantColor.fpBlue, blendConstantColor.fpAlpha );
					Ic3OpenGLHandleLastError();
					cachedBlendConfig.constantColor = pBlendConfig.constantColor;
				}
			}
		}
	}

	void GLGlobalStateCache::ApplyDepthStencilState( const GLDepthStencilConfig & pDepthStencilConfig, uint8 pStencilRefValue )
	{
		auto & cachedDepthStencilConfig = _cachedState.depthStencilConfig;
		auto & cachedDepthSettings = cachedDepthStencilConfig.depthSettings;
		auto & cachedStencilSettings = cachedDepthStencilConfig.stencilSettings;

		if( !pDepthStencilConfig.depthTestActive )
		{
			if( cachedDepthStencilConfig.depthTestActive )
			{
				glDisable( GL_DEPTH_TEST );
				Ic3OpenGLHandleLastError();
				cachedDepthStencilConfig.depthTestActive = false;
			}
		}
		else
		{
			if( !cachedDepthStencilConfig.depthTestActive )
			{
				glEnable( GL_DEPTH_TEST );
				Ic3OpenGLHandleLastError();
				cachedDepthStencilConfig.depthTestActive = true;
			}

			if( cachedDepthSettings.depthCompFunc != pDepthStencilConfig.depthSettings.depthCompFunc )
			{
				glDepthFunc( pDepthStencilConfig.depthSettings.depthCompFunc );
				Ic3OpenGLHandleLastError();
				cachedDepthSettings.depthCompFunc = pDepthStencilConfig.depthSettings.depthCompFunc;
			}

			if( cachedDepthSettings.writeMask != pDepthStencilConfig.depthSettings.writeMask )
			{
				glDepthMask( pDepthStencilConfig.depthSettings.writeMask ? GL_TRUE : GL_FALSE );
				Ic3OpenGLHandleLastError();
				cachedDepthSettings.writeMask = pDepthStencilConfig.depthSettings.writeMask;
			}
		}

		if( !pDepthStencilConfig.stencilTestActive )
		{
			if( cachedDepthStencilConfig.stencilTestActive )
			{
				glDisable( GL_STENCIL_TEST );
				Ic3OpenGLHandleLastError();
				cachedDepthStencilConfig.stencilTestActive = false;
			}
		}
		else
		{
			if( !cachedDepthStencilConfig.stencilTestActive )
			{
				glEnable( GL_STENCIL_TEST );
				Ic3OpenGLHandleLastError();
				cachedDepthStencilConfig.stencilTestActive = true;
			}

			const auto & frontFace = pDepthStencilConfig.stencilSettings.frontFace;
			if( !cppx::mem_cmp_equal( frontFace, cachedStencilSettings.frontFace ) )
			{
				glStencilFuncSeparate( GL_FRONT, frontFace.compFunc, pStencilRefValue, frontFace.readMask );
				Ic3OpenGLHandleLastError();

				glStencilOpSeparate( GL_FRONT, frontFace.opFail, frontFace.opPassDepthFail, frontFace.opPassDepthPass );
				Ic3OpenGLHandleLastError();

				glStencilMaskSeparate( GL_FRONT, frontFace.writeMask );
				Ic3OpenGLHandleLastError();

				cppx::mem_copy( cachedStencilSettings.frontFace, frontFace );
			}

			const auto & backFace = pDepthStencilConfig.stencilSettings.backFace;
			if( !cppx::mem_cmp_equal( backFace, cachedStencilSettings.backFace ) )
			{
				glStencilFuncSeparate( GL_FRONT, backFace.compFunc, pStencilRefValue, backFace.readMask );
				Ic3OpenGLHandleLastError();

				glStencilOpSeparate( GL_FRONT, backFace.opFail, backFace.opPassDepthFail, backFace.opPassDepthPass );
				Ic3OpenGLHandleLastError();

				glStencilMaskSeparate( GL_FRONT, backFace.writeMask );
				Ic3OpenGLHandleLastError();

				cppx::mem_copy( cachedStencilSettings.backFace, backFace );
			}
		}
	}

	void GLGlobalStateCache::ApplyRasterizerState( const GLRasterizerConfig & pRasterizerConfig )
	{
		auto & cachedRasterizerConfig = _cachedState.rasterizerConfig;

		if( pRasterizerConfig.scissorTestActive != cachedRasterizerConfig.scissorTestActive )
		{
			if( pRasterizerConfig.scissorTestActive )
			{
				glEnable( GL_SCISSOR_TEST );
				Ic3OpenGLHandleLastError();
				cachedRasterizerConfig.scissorTestActive = true;
			}
			else
			{
				glDisable( GL_SCISSOR_TEST );
				Ic3OpenGLHandleLastError();
				cachedRasterizerConfig.scissorTestActive = false;
			}
		}

		if( pRasterizerConfig.cullMode != cachedRasterizerConfig.cullMode )
		{
			if( pRasterizerConfig.cullMode != GL_NONE )
			{
				glEnable( GL_CULL_FACE );
				Ic3OpenGLHandleLastError();

				glCullFace( pRasterizerConfig.cullMode );
				Ic3OpenGLHandleLastError();

				cachedRasterizerConfig.cullMode = pRasterizerConfig.cullMode;
			}
			else
			{
				glDisable( GL_CULL_FACE );
				Ic3OpenGLHandleLastError();

				cachedRasterizerConfig.cullMode = GL_NONE;
			}
		}

		if( pRasterizerConfig.frontFaceVerticesOrder != cachedRasterizerConfig.frontFaceVerticesOrder )
		{
			glFrontFace( pRasterizerConfig.frontFaceVerticesOrder );
			Ic3OpenGLHandleLastError();

			cachedRasterizerConfig.frontFaceVerticesOrder = pRasterizerConfig.frontFaceVerticesOrder;
		}

	#if( IC3_GX_GL_FEATURE_SUPPORT_PRIMITIVE_FILL_MODE )
		if( pRasterizerConfig.primitiveFillMode != cachedRasterizerConfig.primitiveFillMode )
		{
			glPolygonMode( GL_FRONT_AND_BACK, pRasterizerConfig.primitiveFillMode );
			Ic3OpenGLHandleLastError();

			cachedRasterizerConfig.frontFaceVerticesOrder = pRasterizerConfig.frontFaceVerticesOrder;
		}
	#endif
	}

	GLGlobalState GLGlobalStateCache::GetDefaultGlobalState()
	{
		GLGlobalState defaultGlobalState;

		defaultGlobalState.shaderPipelineBinding = 0;
		defaultGlobalState.shaderProgramBinding = 0;
		defaultGlobalState.vertexArrayObjectBinding = 0;

		// BlendConfig
		{
			defaultGlobalState.blendConfig.blendActiveGlobal = false;
			defaultGlobalState.blendConfig.flags = 0;
			defaultGlobalState.blendConfig.attachmentsMask = 0;
			defaultGlobalState.blendConfig.constantColor = { 0.0f, 0.0f, 0.0f, 0.0f };
			defaultGlobalState.blendConfig.attachments[0].blendActive = false;
			defaultGlobalState.blendConfig.attachments[0].equation.rgb = GL_FUNC_ADD;
			defaultGlobalState.blendConfig.attachments[0].equation.alpha = GL_FUNC_ADD;
			defaultGlobalState.blendConfig.attachments[0].factor.rgbSrc = GL_ONE;
			defaultGlobalState.blendConfig.attachments[0].factor.alphaSrc = GL_ONE;
			defaultGlobalState.blendConfig.attachments[0].factor.rgbDst = GL_ZERO;
			defaultGlobalState.blendConfig.attachments[0].factor.alphaDst = GL_ZERO;
			defaultGlobalState.blendConfig.attachments[1] = defaultGlobalState.blendConfig.attachments[0];
			defaultGlobalState.blendConfig.attachments[2] = defaultGlobalState.blendConfig.attachments[0];
			defaultGlobalState.blendConfig.attachments[3] = defaultGlobalState.blendConfig.attachments[0];
			defaultGlobalState.blendConfig.attachments[4] = defaultGlobalState.blendConfig.attachments[0];
			defaultGlobalState.blendConfig.attachments[5] = defaultGlobalState.blendConfig.attachments[0];
			defaultGlobalState.blendConfig.attachments[6] = defaultGlobalState.blendConfig.attachments[0];
			defaultGlobalState.blendConfig.attachments[7] = defaultGlobalState.blendConfig.attachments[0];
		}

		// DepthStencilConfig
		{
			defaultGlobalState.depthStencilConfig.depthTestActive = false;
			defaultGlobalState.depthStencilConfig.depthSettings.depthCompFunc = GL_LESS;
			defaultGlobalState.depthStencilConfig.depthSettings.writeMask = GL_TRUE;

			defaultGlobalState.depthStencilConfig.stencilTestActive = false;
			defaultGlobalState.depthStencilConfig.stencilSettings.frontFace.compFunc = GL_ALWAYS;
			defaultGlobalState.depthStencilConfig.stencilSettings.frontFace.writeMask = 0xFFFFFFFF;
			defaultGlobalState.depthStencilConfig.stencilSettings.frontFace.opFail = GL_KEEP;
			defaultGlobalState.depthStencilConfig.stencilSettings.frontFace.opPassDepthFail = GL_KEEP;
			defaultGlobalState.depthStencilConfig.stencilSettings.frontFace.opPassDepthPass = GL_KEEP;
			defaultGlobalState.depthStencilConfig.stencilSettings.frontFace = defaultGlobalState.depthStencilConfig.stencilSettings.backFace;
		}

		// RasterizerConfig
		{
			defaultGlobalState.rasterizerConfig.scissorTestActive = false;
			defaultGlobalState.rasterizerConfig.cullMode = GL_NONE; // This is initially GL_BACK, but culling itself is disabled!
			defaultGlobalState.rasterizerConfig.frontFaceVerticesOrder = GL_CCW;
		#if( IC3_GX_GL_FEATURE_SUPPORT_PRIMITIVE_FILL_MODE )
			defaultGlobalState.rasterizerConfig.primitiveFillMode = GL_FILL;
		#endif
		}

		cppx::mem_set_zero( defaultGlobalState.samplerBindings );
		cppx::mem_set_zero( defaultGlobalState.textureUnitBindings );

		return defaultGlobalState;
	}

}
