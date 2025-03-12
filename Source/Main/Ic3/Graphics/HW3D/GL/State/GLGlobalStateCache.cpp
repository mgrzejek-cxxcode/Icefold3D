
#include "GLGlobalStateCache.h"

namespace Ic3::Graphics::GCI
{

	const GLGlobalState GLGlobalStateCache::sDefaultState = GLGlobalStateCache::GetDefaultGlobalState();

	GLGlobalStateCache::GLGlobalStateCache()
	{
		Reset();
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

	void GLGlobalStateCache::ApplyBlendSettings( const GLBlendSettings & pBlendSettings, bool pSetConstantColor )
	{
		auto & cachedBlendSettings = _cachedState.blendSettings;

		if( !pBlendSettings.attachmentsMask.is_set_any_of( eRTAttachmentMaskColorAll ) )
		{
			if( cachedBlendSettings.blendActiveGlobal )
			{
				glDisable( GL_BLEND );
				Ic3OpenGLHandleLastError();

				cachedBlendSettings.blendActiveGlobal = false;
			}
		}
		else
		{
			const bool isSeparateBlendingEnabled = cachedBlendSettings.flags.is_set(
					eBlendConfigFlagEnableMRTIndependentBlendingBit );
			const bool isSeparateBlendingRequested = pBlendSettings.flags.is_set(
					eBlendConfigFlagEnableMRTIndependentBlendingBit );

			if( isSeparateBlendingRequested )
			{
				bool compareCurrentState = isSeparateBlendingEnabled;

				for( uint32 colorAttachmentIndex = 0; CXU::RTOIsAttachmentIndexValid( colorAttachmentIndex ); ++colorAttachmentIndex )
				{
					auto & cachedBlendProps = cachedBlendSettings.attachments[colorAttachmentIndex];
					const auto & blendProps = pBlendSettings.attachments[colorAttachmentIndex];

					if( !blendProps.blendActive )
					{
						if( !compareCurrentState || cachedBlendProps.blendActive )
						{
							glDisablei( GL_BLEND, colorAttachmentIndex );
							Ic3OpenGLHandleLastError();
							cachedBlendProps.blendActive = 0;
						}
					}
					else // Enable blending for this attachment
					{
						if( !compareCurrentState || !cachedBlendProps.blendActive )
						{
							glEnablei( GL_BLEND, colorAttachmentIndex );
							Ic3OpenGLHandleLastError();
							cachedBlendProps.blendActive = 1;
						}

						if( !compareCurrentState || cppx::mem_cmp_not_equal( blendProps.equation, cachedBlendProps.equation ) )
						{
							glBlendEquationSeparatei( colorAttachmentIndex, blendProps.equation.rgb, blendProps.equation.alpha );
							Ic3OpenGLHandleLastError();
							cachedBlendProps.equation = blendProps.equation;
						}

						if( !compareCurrentState || cppx::mem_cmp_not_equal( blendProps.factor, cachedBlendProps.factor ) )
						{
							glBlendFuncSeparatei(
								colorAttachmentIndex,
								blendProps.factor.rgbSrc,
								blendProps.factor.rgbDst,
								blendProps.factor.alphaSrc,
								blendProps.factor.alphaDst );
							Ic3OpenGLHandleLastError();
							cachedBlendProps.factor = blendProps.factor;
						}
					}
				}
			}
			else
			{
				bool compareCurrentState = !isSeparateBlendingEnabled;

				if( !compareCurrentState || !cachedBlendSettings.blendActiveGlobal  )
				{
					glEnable( GL_BLEND );
					Ic3OpenGLHandleLastError();
					cachedBlendSettings.blendActiveGlobal = true;
				}

				auto & cachedBlendProps = cachedBlendSettings.attachments[0];
				const auto & blendProps = pBlendSettings.attachments[0];

				if( !compareCurrentState || cppx::mem_cmp_not_equal( blendProps.equation, cachedBlendProps.equation ) )
				{
					glBlendEquationSeparate( blendProps.equation.rgb, blendProps.equation.alpha );
					Ic3OpenGLHandleLastError();
					cachedBlendProps.equation = blendProps.equation;
				}

				if( !compareCurrentState || cppx::mem_cmp_not_equal( blendProps.factor, cachedBlendProps.factor ) )
				{
					glBlendFuncSeparate(
						blendProps.factor.rgbSrc,
						blendProps.factor.rgbDst,
						blendProps.factor.alphaSrc,
						blendProps.factor.alphaDst );
					Ic3OpenGLHandleLastError();
					cachedBlendProps.factor = blendProps.factor;
				}
			}

			if( pSetConstantColor )
			{
				const auto & blendConstantColor = pBlendSettings.constantColor;

				if( cppx::mem_cmp_not_equal( blendConstantColor, cachedBlendSettings.constantColor ) )
				{
					glBlendColor(
						blendConstantColor.ufp_red,
						blendConstantColor.ufp_green,
						blendConstantColor.ufp_blue,
						blendConstantColor.ufp_alpha );
					Ic3OpenGLHandleLastError();
					cachedBlendSettings.constantColor = pBlendSettings.constantColor;
				}
			}
		}
	}

	void GLGlobalStateCache::ApplyDepthStencilSettings( const GLDepthStencilSettings & pDepthStencilSettings, uint8 pStencilTestRefValue )
	{
		auto & cachedDepthStencilSettings = _cachedState.depthStencilSettings;
		auto & cachedDepthSettings = cachedDepthStencilSettings.depthSettings;
		auto & cachedStencilSettings = cachedDepthStencilSettings.stencilSettings;

		if( !pDepthStencilSettings.depthTestActive )
		{
			if( cachedDepthStencilSettings.depthTestActive )
			{
				glDisable( GL_DEPTH_TEST );
				Ic3OpenGLHandleLastError();
				cachedDepthStencilSettings.depthTestActive = false;
			}
		}
		else
		{
			if( !cachedDepthStencilSettings.depthTestActive )
			{
				glEnable( GL_DEPTH_TEST );
				Ic3OpenGLHandleLastError();
				cachedDepthStencilSettings.depthTestActive = true;
			}

			if( cachedDepthSettings.depthCompFunc != pDepthStencilSettings.depthSettings.depthCompFunc )
			{
				glDepthFunc( pDepthStencilSettings.depthSettings.depthCompFunc );
				Ic3OpenGLHandleLastError();
				cachedDepthSettings.depthCompFunc = pDepthStencilSettings.depthSettings.depthCompFunc;
			}

			if( cachedDepthSettings.writeMask != pDepthStencilSettings.depthSettings.writeMask )
			{
				glDepthMask( pDepthStencilSettings.depthSettings.writeMask ? GL_TRUE : GL_FALSE );
				Ic3OpenGLHandleLastError();
				cachedDepthSettings.writeMask = pDepthStencilSettings.depthSettings.writeMask;
			}
		}

		if( !pDepthStencilSettings.stencilTestActive )
		{
			if( cachedDepthStencilSettings.stencilTestActive )
			{
				glDisable( GL_STENCIL_TEST );
				Ic3OpenGLHandleLastError();
				cachedDepthStencilSettings.stencilTestActive = false;
			}
		}
		else
		{
			if( !cachedDepthStencilSettings.stencilTestActive )
			{
				glEnable( GL_STENCIL_TEST );
				Ic3OpenGLHandleLastError();
				cachedDepthStencilSettings.stencilTestActive = true;
			}

			const auto & frontFace = pDepthStencilSettings.stencilSettings.frontFace;
			if( !cppx::mem_cmp_equal( frontFace, cachedStencilSettings.frontFace ) )
			{
				glStencilFuncSeparate( GL_FRONT, frontFace.compFunc, pStencilTestRefValue, frontFace.readMask );
				Ic3OpenGLHandleLastError();

				glStencilOpSeparate( GL_FRONT, frontFace.opFail, frontFace.opPassDepthFail, frontFace.opPassDepthPass );
				Ic3OpenGLHandleLastError();

				glStencilMaskSeparate( GL_FRONT, frontFace.writeMask );
				Ic3OpenGLHandleLastError();

				cppx::mem_copy( cachedStencilSettings.frontFace, frontFace );
			}

			const auto & backFace = pDepthStencilSettings.stencilSettings.backFace;
			if( !cppx::mem_cmp_equal( backFace, cachedStencilSettings.backFace ) )
			{
				glStencilFuncSeparate( GL_FRONT, backFace.compFunc, pStencilTestRefValue, backFace.readMask );
				Ic3OpenGLHandleLastError();

				glStencilOpSeparate( GL_FRONT, backFace.opFail, backFace.opPassDepthFail, backFace.opPassDepthPass );
				Ic3OpenGLHandleLastError();

				glStencilMaskSeparate( GL_FRONT, backFace.writeMask );
				Ic3OpenGLHandleLastError();

				cppx::mem_copy( cachedStencilSettings.backFace, backFace );
			}
		}
	}

	void GLGlobalStateCache::ApplyRasterizerSettings( const GLRasterizerSettings & pRasterizerSettings )
	{
		auto & cachedRasterizerSettings = _cachedState.rasterizerSettings;

		if( pRasterizerSettings.scissorTestActive != cachedRasterizerSettings.scissorTestActive )
		{
			if( pRasterizerSettings.scissorTestActive )
			{
				glEnable( GL_SCISSOR_TEST );
				Ic3OpenGLHandleLastError();
				cachedRasterizerSettings.scissorTestActive = true;
			}
			else
			{
				glDisable( GL_SCISSOR_TEST );
				Ic3OpenGLHandleLastError();
				cachedRasterizerSettings.scissorTestActive = false;
			}
		}

		if( pRasterizerSettings.cullMode != cachedRasterizerSettings.cullMode )
		{
			if( pRasterizerSettings.cullMode != GL_NONE )
			{
				glEnable( GL_CULL_FACE );
				Ic3OpenGLHandleLastError();

				glCullFace( pRasterizerSettings.cullMode );
				Ic3OpenGLHandleLastError();

				cachedRasterizerSettings.cullMode = pRasterizerSettings.cullMode;
			}
			else
			{
				glDisable( GL_CULL_FACE );
				Ic3OpenGLHandleLastError();

				cachedRasterizerSettings.cullMode = GL_NONE;
			}
		}

		if( pRasterizerSettings.frontFaceVerticesOrder != cachedRasterizerSettings.frontFaceVerticesOrder )
		{
			glFrontFace( pRasterizerSettings.frontFaceVerticesOrder );
			Ic3OpenGLHandleLastError();

			cachedRasterizerSettings.frontFaceVerticesOrder = pRasterizerSettings.frontFaceVerticesOrder;
		}

	#if( IC3_GX_GL_FEATURE_SUPPORT_PRIMITIVE_FILL_MODE )
		if( pRasterizerSettings.primitiveFillMode != cachedRasterizerSettings.primitiveFillMode )
		{
			glPolygonMode( GL_FRONT_AND_BACK, pRasterizerSettings.primitiveFillMode );
			Ic3OpenGLHandleLastError();

			cachedRasterizerSettings.frontFaceVerticesOrder = pRasterizerSettings.frontFaceVerticesOrder;
		}
	#endif
	}

	void GLGlobalStateCache::SetBlendConstantColor( const cxm::rgba_color_r32_norm & pConstantColor )
	{
		auto & cachedBlendSettings = _cachedState.blendSettings;
		if( cppx::mem_cmp_not_equal( pConstantColor, cachedBlendSettings.constantColor ) )
		{
			glBlendColor(
					pConstantColor.ufp_red,
					pConstantColor.ufp_green,
					pConstantColor.ufp_blue,
					pConstantColor.ufp_alpha );
			Ic3OpenGLHandleLastError();
			cachedBlendSettings.constantColor = pConstantColor;
		}
	}

	void GLGlobalStateCache::Reset()
	{
		_cachedState = sDefaultState;
	}

	GLGlobalState GLGlobalStateCache::GetDefaultGlobalState()
	{
		GLGlobalState defaultGlobalState;

		defaultGlobalState.indexBufferBinding = 0;
		defaultGlobalState.shaderPipelineBinding = 0;
		defaultGlobalState.shaderProgramBinding = 0;
		defaultGlobalState.vertexArrayObjectBinding = 0;

		// BlendSettings
		{
			defaultGlobalState.blendSettings.blendActiveGlobal = false;
			defaultGlobalState.blendSettings.flags = 0;
			defaultGlobalState.blendSettings.attachmentsMask = 0;
			defaultGlobalState.blendSettings.constantColor = { 0.0f, 0.0f, 0.0f, 0.0f };
			defaultGlobalState.blendSettings.attachments[0].blendActive = false;
			defaultGlobalState.blendSettings.attachments[0].equation.rgb = GL_FUNC_ADD;
			defaultGlobalState.blendSettings.attachments[0].equation.alpha = GL_FUNC_ADD;
			defaultGlobalState.blendSettings.attachments[0].factor.rgbSrc = GL_ONE;
			defaultGlobalState.blendSettings.attachments[0].factor.alphaSrc = GL_ONE;
			defaultGlobalState.blendSettings.attachments[0].factor.rgbDst = GL_ZERO;
			defaultGlobalState.blendSettings.attachments[0].factor.alphaDst = GL_ZERO;
			defaultGlobalState.blendSettings.attachments[1] = defaultGlobalState.blendSettings.attachments[0];
			defaultGlobalState.blendSettings.attachments[2] = defaultGlobalState.blendSettings.attachments[0];
			defaultGlobalState.blendSettings.attachments[3] = defaultGlobalState.blendSettings.attachments[0];
			defaultGlobalState.blendSettings.attachments[4] = defaultGlobalState.blendSettings.attachments[0];
			defaultGlobalState.blendSettings.attachments[5] = defaultGlobalState.blendSettings.attachments[0];
			defaultGlobalState.blendSettings.attachments[6] = defaultGlobalState.blendSettings.attachments[0];
			defaultGlobalState.blendSettings.attachments[7] = defaultGlobalState.blendSettings.attachments[0];
		}

		// DepthStencilSettings
		{
			defaultGlobalState.depthStencilSettings.depthTestActive = false;
			defaultGlobalState.depthStencilSettings.depthSettings.depthCompFunc = GL_LESS;
			defaultGlobalState.depthStencilSettings.depthSettings.writeMask = GL_TRUE;

			defaultGlobalState.depthStencilSettings.stencilTestActive = false;
			defaultGlobalState.depthStencilSettings.stencilSettings.frontFace.compFunc = GL_ALWAYS;
			defaultGlobalState.depthStencilSettings.stencilSettings.frontFace.writeMask = 0xFFFFFFFF;
			defaultGlobalState.depthStencilSettings.stencilSettings.frontFace.opFail = GL_KEEP;
			defaultGlobalState.depthStencilSettings.stencilSettings.frontFace.opPassDepthFail = GL_KEEP;
			defaultGlobalState.depthStencilSettings.stencilSettings.frontFace.opPassDepthPass = GL_KEEP;
			defaultGlobalState.depthStencilSettings.stencilSettings.frontFace = defaultGlobalState.depthStencilSettings.stencilSettings.backFace;
		}

		// RasterizerSettings
		{
			defaultGlobalState.rasterizerSettings.scissorTestActive = false;
			defaultGlobalState.rasterizerSettings.cullMode = GL_NONE; // This is initially GL_BACK, but culling itself is disabled!
			defaultGlobalState.rasterizerSettings.frontFaceVerticesOrder = GL_CCW;
		#if( IC3_GX_GL_FEATURE_SUPPORT_PRIMITIVE_FILL_MODE )
			defaultGlobalState.rasterizerSettings.primitiveFillMode = GL_FILL;
		#endif
		}

		cppx::mem_set_zero( defaultGlobalState.samplerBindings );
		cppx::mem_set_zero( defaultGlobalState.textureUnitBindings );

		return defaultGlobalState;
	}

}
