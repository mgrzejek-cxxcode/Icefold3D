
#include "GLPipelineStateController.h"
#include "GLCommonGraphicsConfig.h"
#include "GLInputAssembler.h"
#include "GLGraphicsShaderState.h"
#include "GLRenderTarget.h"
#include "GLPipelineStateObject.h"
#include "GLVertexArrayObjectCache.h"
#include "../Objects/GLFramebufferObject.h"
#include "../Objects/GLShaderPipelineObject.h"
#include "../Objects/GLVertexArrayObject.h"
#include "../Resources/GLGPUBuffer.h"
#include "../Resources/GLSampler.h"
#include "../Resources/GLShader.h"
#include "../Resources/GLTexture.h"

#include <Ic3/Graphics/GCI/State/InputAssemblerDynamicStates.h>
#include <Ic3/Graphics/GCI/State/RenderTargetDynamicStates.h>

namespace Ic3::Graphics::GCI
{

	GLGraphicsPipelineStateController::GLGraphicsPipelineStateController()
	: GraphicsPipelineStateControllerSeparable()
	{}

	GLGraphicsPipelineStateController::~GLGraphicsPipelineStateController() = default;

	const GLDrawTopologyProperties & GLGraphicsPipelineStateController::GetGLDrawTopologyProperties() const noexcept
	{
		return _currentDrawTopologyProperties;
	}

	const GLIAVertexStreamDefinition & GLGraphicsPipelineStateController::GetCurrentIAVertexStreamDefinition() const noexcept
	{
		if( IsIAVertexStreamStateDynamic() )
		{
			return _dynamicIAVertexStreamDefinition;
		}
		else
		{
			const auto * glcVertexStreamState = _currentCommonState.iaVertexStreamState->QueryInterface<GLIAVertexStreamImmutableState>();
			return glcVertexStreamState->mGLVertexStreamDefinition;
		}
	}

	GLRenderTargetBindingInfo GLGraphicsPipelineStateController::GetCurrentRenderTargetBindingInfo() const noexcept
	{
		GLRenderTargetBindingInfo glcRTBindingInfo{};
		if( _currentCommonState.renderTargetBindingState )
		{
			if( IsRenderTargetStateDynamic()  )
			{
				return SMU::GetGLRenderTargetBindingInfo( _dynamicRenderTargetBindingDefinition );
			}
			else
			{
				const auto * glcRTBindingState = _currentCommonState.renderTargetBindingState->GetInterface<GLRenderTargetBindingImmutableState>();
				return glcRTBindingState->GetGLRenderTargetBindingInfo();
			}
		}
		else
		{
			return GLRenderTargetBindingInfo{};
		}
		return glcRTBindingInfo;
	}

	bool GLGraphicsPipelineStateController::ApplyStateChanges()
	{
		cppx::bitmask<uint32> executedUpdatesMask = 0;

		if( _stateUpdateMask.is_set( eGraphicsStateUpdateFlagCommonPSOBit ) )
		{
			if( _stateUpdateMask.is_set_any_of( eGraphicsStateUpdateMaskSeparableStatesAll ) )
			{
				const auto * glcGraphicsPSO = _currentCommonState.graphicsPSO->QueryInterface<GLGraphicsPipelineStateObject>();
				const auto commonPSOUpdateMask = ApplyCommonGraphicsConfigState( *glcGraphicsPSO );
				executedUpdatesMask.set( commonPSOUpdateMask );
			}
		}

		if( _stateUpdateMask.is_set( eGraphicsStateUpdateFlagCommonRenderTargetBindingBit ) )
		{
			const auto & currentRenderTargetBindingInfo = GetCurrentRenderTargetBindingInfo();
			ApplyGLRenderTargetBinding( currentRenderTargetBindingInfo );
			executedUpdatesMask.set( eGraphicsStateUpdateFlagCommonRenderTargetBindingBit );
		}

		ApplyGraphicsPipelineDynamicState( GetRenderPassDynamicState() );

		return !executedUpdatesMask.empty();
	}

	bool GLGraphicsPipelineStateController::SetGraphicsPipelineStateObject( const GraphicsPipelineStateObject & pGraphicsPSO )
	{
		return GraphicsPipelineStateControllerSeparable::SetGraphicsPipelineStateObject( pGraphicsPSO );
	}

	bool GLGraphicsPipelineStateController::ResetGraphicsPipelineStateObject()
	{
		return GraphicsPipelineStateControllerSeparable::ResetGraphicsPipelineStateObject();
	}

	bool GLGraphicsPipelineStateController::SetIAVertexStreamState( const IAVertexStreamDynamicState & pIAVertexStreamState )
	{
		bool updateResult = GraphicsPipelineStateController::SetIAVertexStreamState( pIAVertexStreamState );

		if( updateResult )
		{
			auto glcVertexStreamDefinition =
					SMU::TranslateIAVertexStreamDefinitionGL( pIAVertexStreamState.GetVertexStreamDefinition() );

			_dynamicIAVertexStreamDefinition = glcVertexStreamDefinition;
		}

		return updateResult;
	}

	bool GLGraphicsPipelineStateController::SetIAVertexStreamState( const IAVertexStreamImmutableState & pIAVertexStreamState )
	{
		return GraphicsPipelineStateControllerSeparable::SetIAVertexStreamState( pIAVertexStreamState );
	}

	bool GLGraphicsPipelineStateController::ResetIAVertexStreamState()
	{
		return GraphicsPipelineStateControllerSeparable::ResetIAVertexStreamState();
	}

	bool GLGraphicsPipelineStateController::SetRenderTargetBindingState( const RenderTargetBindingDynamicState & pRenderTargetBindingState )
	{
		bool updateResult = GraphicsPipelineStateController::SetRenderTargetBindingState( pRenderTargetBindingState );

		if( updateResult )
		{
			auto glcRenderTargetBindingDefinition =
					SMU::TranslateRenderTargetBindingDefinition( pRenderTargetBindingState.GetBindingDefinition() );

			_dynamicRenderTargetBindingDefinition = std::move( glcRenderTargetBindingDefinition );

		}

		return updateResult;
	}

	bool GLGraphicsPipelineStateController::SetRenderTargetBindingState( const RenderTargetBindingImmutableState & pRenderTargetBindingState )
	{
		return GraphicsPipelineStateControllerSeparable::SetRenderTargetBindingState( pRenderTargetBindingState );
	}

	bool GLGraphicsPipelineStateController::ResetRenderTargetBindingState()
	{
		return GraphicsPipelineStateControllerSeparable::ResetRenderTargetBindingState();
	}

	bool GLGraphicsPipelineStateController::SetViewport( const ViewportDesc & pViewportDesc )
	{
		bool baseResult = GraphicsPipelineStateController::SetViewport( pViewportDesc );

		if( baseResult )
		{
			glViewport(
				cppx::numeric_cast<GLsizei>( pViewportDesc.origin.x ),
		        cppx::numeric_cast<GLsizei>( pViewportDesc.origin.y ),
	        cppx::numeric_cast<GLsizei>( pViewportDesc.size.x ),
	        cppx::numeric_cast<GLsizei>( pViewportDesc.size.y ) );
			Ic3OpenGLHandleLastError();

			glDepthRangef( pViewportDesc.depthRange.zNear, pViewportDesc.depthRange.zFar );
			Ic3OpenGLHandleLastError();
		}

		return baseResult;
	}

	bool GLGraphicsPipelineStateController::SetShaderConstant( shader_input_ref_id_t pParamRefID, const void * pData )
	{
		bool baseResult = GraphicsPipelineStateController::SetShaderConstant( pParamRefID, pData );

		if( baseResult )
		{
			const auto & glcGraphicsPSO = GetCurrentGraphicsPSORef<GLGraphicsPipelineStateObject>();
			if( const auto & inputSignature = glcGraphicsPSO.mShaderInputSignature )
			{
				const auto & constantInfo = inputSignature.GetConstantInfo( pParamRefID );
				if( constantInfo.iVisibilityMask != 0 )
				{
					const auto * shaderLinkageState = GetCurrentSeparableStates().shaderLinkageState;
					const auto * glcShaderLinkageState = shaderLinkageState->QueryInterface<GLGraphicsShaderLinkageImmutableState>();

					UpdateShaderInputInlineConstantData( *glcShaderLinkageState, constantInfo, pData );

					return true;
				}
			}
			return false;
		}

		return baseResult;
	}

	bool GLGraphicsPipelineStateController::SetShaderConstantBuffer( shader_input_ref_id_t pParamRefID, GPUBuffer & pConstantBuffer )
	{
		bool baseResult = GraphicsPipelineStateController::SetShaderConstantBuffer( pParamRefID, pConstantBuffer );

		if( baseResult )
		{
			const auto & glcGraphicsPSO = GetCurrentGraphicsPSORef<GLGraphicsPipelineStateObject>();
			if( const auto & inputSignature = glcGraphicsPSO.mShaderInputSignature )
			{
				const auto & descriptorInfo = inputSignature.GetDescriptorInfo( pParamRefID );
				Ic3DebugAssert( descriptorInfo.dDescriptorType == EShaderInputDescriptorType::Resource );
				Ic3DebugAssert( descriptorInfo.uResourceInfo.resourceType == EShaderInputResourceType::CBVConstantBuffer );

				if( descriptorInfo.dShaderVisibilityMask != 0 )
				{
					auto * openglBuffer = pConstantBuffer.QueryInterface<GLGPUBuffer>();

					glBindBufferBase( GL_UNIFORM_BUFFER, descriptorInfo.uResourceInfo.resourceBaseRegisterIndex, openglBuffer->mGLBufferObject->mGLHandle );
					Ic3OpenGLHandleLastError();

					return true;
				}
			}
		}

		return baseResult;
	}

	bool GLGraphicsPipelineStateController::SetShaderTextureImage( shader_input_ref_id_t pParamRefID, Texture & pTexture )
	{
		bool baseResult = GraphicsPipelineStateController::SetShaderTextureImage( pParamRefID, pTexture );

		if( baseResult )
		{
			const auto & glcGraphicsPSO = GetCurrentGraphicsPSORef<GLGraphicsPipelineStateObject>();
			if( const auto & inputSignature = glcGraphicsPSO.mShaderInputSignature )
			{
				const auto & descriptorInfo = inputSignature.GetDescriptorInfo( pParamRefID );
				Ic3DebugAssert( descriptorInfo.dDescriptorType == EShaderInputDescriptorType::Resource );
				Ic3DebugAssert( descriptorInfo.uResourceInfo.resourceType == EShaderInputResourceType::SRVTextureImage );

				auto * openglTexture = pTexture.QueryInterface<GLTexture>();

				glActiveTexture( GL_TEXTURE0 + descriptorInfo.uResourceInfo.resourceBaseRegisterIndex );
				Ic3OpenGLHandleLastError();

				glBindTexture( openglTexture->mGLTextureObject->mGLTextureBindTarget, openglTexture->mGLTextureObject->mGLHandle );
				Ic3OpenGLHandleLastError();

				return true;
			}
		}

		return baseResult;
	}

	bool GLGraphicsPipelineStateController::SetShaderTextureSampler( shader_input_ref_id_t pParamRefID, Sampler & pSampler )
	{
		bool baseResult = GraphicsPipelineStateController::SetShaderTextureSampler( pParamRefID, pSampler );

		if( baseResult )
		{
			const auto & glcGraphicsPSO = GetCurrentGraphicsPSORef<GLGraphicsPipelineStateObject>();
			if( const auto & inputSignature = glcGraphicsPSO.mShaderInputSignature )
			{
				const auto & descriptorInfo = inputSignature.GetDescriptorInfo( pParamRefID );
				Ic3DebugAssert( descriptorInfo.dDescriptorType == EShaderInputDescriptorType::Sampler );

				if( descriptorInfo.dShaderVisibilityMask != 0 )
				{
					auto * openglSampler = pSampler.QueryInterface<GLSampler>();

					glBindSampler( descriptorInfo.uSamplerInfo.samplerBindingIndex, openglSampler->mGLSamplerObject->mGLHandle );
					Ic3OpenGLHandleLastError();

					return true;
				}
			}
		}

		return baseResult;
	}

	cppx::bitmask<uint32> GLGraphicsPipelineStateController::ApplyCommonGraphicsConfigState( const GLGraphicsPipelineStateObject & pGraphicsPSO )
	{
		cppx::bitmask<uint32> executedUpdatesMask = 0;

		if( _stateUpdateMask.is_set( eGraphicsStateUpdateFlagSeparableStateBlendBit ) )
		{
			const auto & blendState = pGraphicsPSO.GetBlendState();
			_globalStateCache.ApplyBlendState( blendState.mGLBlendConfig );
			executedUpdatesMask.set( eGraphicsStateUpdateFlagSeparableStateBlendBit );
		}

		if( _stateUpdateMask.is_set( eGraphicsStateUpdateFlagSeparableStateDepthStencilBit ) )
		{
			const auto & depthStencilState = pGraphicsPSO.GetDepthStencilState();
			const auto & dynamicState = GetRenderPassDynamicState();

			if( dynamicState.activeStateMask.is_set( eGraphicsPipelineDynamicStateFlagBlendConstantColorBit ) )
			{
				_globalStateCache.ApplyDepthStencilState( depthStencilState.mGLDepthStencilConfig, dynamicState.stencilTestRefValue );
			}
			else
			{
				_globalStateCache.ApplyDepthStencilState( depthStencilState.mGLDepthStencilConfig, 0x00 );
			}

			executedUpdatesMask.set( eGraphicsStateUpdateFlagSeparableStateDepthStencilBit );
		}

		if( _stateUpdateMask.is_set( eGraphicsStateUpdateFlagSeparableStateRasterizerBit ) )
		{
			const auto & rasterizerState = pGraphicsPSO.GetRasterizerState();
			_globalStateCache.ApplyRasterizerState( rasterizerState.mGLRasterizerConfig );
			executedUpdatesMask.set( eGraphicsStateUpdateFlagSeparableStateRasterizerBit );
		}

		return executedUpdatesMask;
	}

	void GLGraphicsPipelineStateController::ApplyGraphicsPipelineDynamicState( const GraphicsPipelineDynamicState & pDynamicState )
	{
		if( pDynamicState.activeStateMask.is_set( eGraphicsPipelineDynamicStateFlagBlendConstantColorBit ) )
		{

			glBlendColor(
					pDynamicState.blendConstantColor.fpRed,
					pDynamicState.blendConstantColor.fpGreen,
					pDynamicState.blendConstantColor.fpBlue,
					pDynamicState.blendConstantColor.fpAlpha );
			Ic3OpenGLHandleLastError();
		}
	}

	void GLGraphicsPipelineStateController::ApplyGLRenderTargetBinding( const GLRenderTargetBindingInfo & pGLRenderTargetBinding )
	{
		glBindFramebuffer( GL_FRAMEBUFFER, pGLRenderTargetBinding.renderFBO->mGLHandle );
		Ic3OpenGLHandleLastError();
	}



	bool GLGraphicsPipelineStateControllerCore::ApplyStateChanges()
	{
		cppx::bitmask<uint32> executedUpdatesMask = 0;

		if( GLGraphicsPipelineStateController::ApplyStateChanges() )
		{
			const auto * glcGraphicsPSO = _currentCommonState.graphicsPSO->QueryInterface<GLGraphicsPipelineStateObject>();

			if( _stateUpdateMask.is_set_any_of( eGraphicsStateUpdateFlagSeparableStateShaderLinkageBit ) )
			{
				const auto * shaderLinkageState = glcGraphicsPSO->GetGraphicsShaderLinkageState().QueryInterface<GLGraphicsShaderLinkageImmutableStateCore>();
				_globalStateCache.ApplyShaderPipelineBinding( shaderLinkageState->mGLShaderPipelineObject->mGLHandle );
				executedUpdatesMask.set( eGraphicsStateUpdateFlagSeparableStateShaderLinkageBit );
			}

			if( _stateUpdateMask.is_set( eGraphicsStateUpdateFlagSeparableStateIAInputLayoutBit ) )
			{
				const auto * inputLayoutState = glcGraphicsPSO->GetIAInputLayoutState().QueryInterface<GLIAInputLayoutImmutableStateCore>();
				_globalStateCache.ApplyVertexArrayObjectBinding( inputLayoutState->mVertexArrayObject->mGLHandle );
				_currentDrawTopologyProperties.primitiveTopology = inputLayoutState->mGLPrimitiveTopology;
				executedUpdatesMask.set( eGraphicsStateUpdateFlagSeparableStateIAInputLayoutBit );
			}

			if( _stateUpdateMask.is_set_any_of( eGraphicsStateUpdateMaskCombinedInputAssembler ) )
			{
				const auto & currentIAVertexStreamDefinition = GetCurrentIAVertexStreamDefinition();
				ApplyGLIAVertexBufferBindings( currentIAVertexStreamDefinition.vertexBufferBindings );
				_globalStateCache.ApplyIndexBufferBinding( currentIAVertexStreamDefinition.indexBufferBinding.handle );
				_currentDrawTopologyProperties.indexBufferBaseOffset = currentIAVertexStreamDefinition.indexBufferBinding.offset;
				_currentDrawTopologyProperties.indexBufferDataType = currentIAVertexStreamDefinition.indexBufferBinding.format;
				_currentDrawTopologyProperties.indexBufferElementByteSize = currentIAVertexStreamDefinition.indexBufferBinding.elementByteSize;
				executedUpdatesMask.set( eGraphicsStateUpdateMaskCombinedInputAssembler );
			}
		}

		_stateUpdateMask.unset( eGraphicsStateUpdateMaskCommonAll | eGraphicsStateUpdateMaskSeparableAll );

		return !executedUpdatesMask.empty();
	}

	void GLGraphicsPipelineStateControllerCore::UpdateShaderInputInlineConstantData(
			const GLGraphicsShaderLinkageImmutableState & pShaderState,
			const ShaderInputParameterConstant & pConstantInfo,
			const void * pConstantData )
	{
		auto * shaderStateCore = pShaderState.QueryInterface<GLGraphicsShaderLinkageImmutableStateCore>();

		auto constantBaseType = CxDef::GetVertexAttribFormatBaseDataType( pConstantInfo.iFormat );
		auto constantLength = CxDef::GetVertexAttribFormatComponentsNum( pConstantInfo.iFormat );

		SMU::UpdateUniformDataCurrentGL(
				*shaderStateCore->mGLShaderPipelineObject,
				pConstantInfo.iStageIndex,
				constantBaseType,
				constantLength,
				pConstantData );
	}

	void GLGraphicsPipelineStateControllerCore::ApplyGLIAVertexBufferBindings( const GLIAVertexBuffersBindings & pVertexBufferBindings )
	{
	#if( IC3_GX_GL_PLATFORM_TYPE == IC3_GX_GL_PLATFORM_TYPE_ES )
		for( const auto & activeVBRange : pVertexBufferBindings.activeRanges )
		{
			for( uint32 entryOffset = 0; entryOffset < activeVBRange.length; ++entryOffset )
			{
				const auto & vbBinding = pVertexBufferBindings.interleavedBindings[activeVBRange.firstIndex + entryOffset];

				glBindVertexBuffer( streamIndex, vbBinding.handle, vbBinding.offset, vbBinding.stride );
				Ic3OpenGLHandleLastError();
			}
		}
	#else
		// Bind all vertex buffers at once using multi-bind. Note, that this also updates the unused bind slots,
		// to properly have buffer object '0' set as the source VBO (deactivating it). Doing that makes profiling
		// and debugging a lot easier (unused buffers from previous passes did some confusion in few cases).
		// glBindVertexBuffers(
		// 	0u,
		// 	GCM::IA_MAX_VERTEX_BUFFER_BINDINGS_NUM,
		// 	&( pVertexBufferBindings.separateBindings.handleArray[0] ),
		// 	&( pVertexBufferBindings.separateBindings.offsetArray[0] ),
		// 	&( pVertexBufferBindings.separateBindings.strideArray[0] ) );
		// Ic3OpenGLHandleLastError();

		for( const auto & activeVBRange : pVertexBufferBindings.activeRanges )
		{
			glBindVertexBuffers(
				activeVBRange.firstIndex,
				activeVBRange.length,
				&( pVertexBufferBindings.separateBindings.handleArray[activeVBRange.firstIndex] ),
				&( pVertexBufferBindings.separateBindings.offsetArray[activeVBRange.firstIndex] ),
				&( pVertexBufferBindings.separateBindings.strideArray[activeVBRange.firstIndex] ) );
			Ic3OpenGLHandleLastError();
		}
	#endif
	}


	bool GLGraphicsPipelineStateControllerCompat::ApplyStateChanges()
	{
		cppx::bitmask<uint32> executedUpdatesMask = 0;

		if( GLGraphicsPipelineStateController::ApplyStateChanges() )
		{
			const auto * glcGraphicsPSO = _currentCommonState.graphicsPSO->QueryInterface<GLGraphicsPipelineStateObject>();

			if( _stateUpdateMask.is_set_any_of( eGraphicsStateUpdateFlagSeparableStateShaderLinkageBit ) )
			{
				const auto * shaderLinkageState = glcGraphicsPSO->GetGraphicsShaderLinkageState().QueryInterface<GLGraphicsShaderLinkageImmutableStateCompat>();
				_globalStateCache.ApplyShaderProgramBinding( shaderLinkageState->mGLShaderProgramObject->mGLHandle );
				executedUpdatesMask.set( eGraphicsStateUpdateFlagSeparableStateShaderLinkageBit );
			}

			if( _stateUpdateMask.is_set_any_of( eGraphicsStateUpdateMaskCombinedInputAssembler ) )
			{
				if( !IsIAVertexStreamStateDynamic() )
				{
					const auto * inputLayoutState = glcGraphicsPSO->GetIAInputLayoutState().QueryInterface<GLIAInputLayoutImmutableStateCompat>();
					const auto * vertexStreamState = _currentCommonState.iaVertexStreamState->QueryInterface<GLIAVertexStreamImmutableState>();

					const auto & vertexArrayObject = GetCachedVertexArrayObject( *inputLayoutState, *vertexStreamState );

					_globalStateCache.ApplyVertexArrayObjectBinding( vertexArrayObject.mGLHandle );
					_currentDrawTopologyProperties.primitiveTopology = inputLayoutState->mGLInputLayoutDefinition.primitiveTopology;

					_globalStateCache.ApplyIndexBufferBinding( vertexStreamState->mGLVertexStreamDefinition.indexBufferBinding.handle );
					_currentDrawTopologyProperties.indexBufferBaseOffset = vertexStreamState->mGLVertexStreamDefinition.indexBufferBinding.offset;
					_currentDrawTopologyProperties.indexBufferDataType = vertexStreamState->mGLVertexStreamDefinition.indexBufferBinding.format;
					_currentDrawTopologyProperties.indexBufferElementByteSize = vertexStreamState->mGLVertexStreamDefinition.indexBufferBinding.elementByteSize;

				}
				else
				{
					const auto * inputLayoutState = glcGraphicsPSO->GetIAInputLayoutState().QueryInterface<GLIAInputLayoutImmutableStateCompat>();
					const auto & currentVertexStreamDefinition = _dynamicIAVertexStreamDefinition;

					const auto & vertexArrayObject = GetCachedVertexArrayObject( inputLayoutState->mGLInputLayoutDefinition, currentVertexStreamDefinition );

					_globalStateCache.ApplyVertexArrayObjectBinding( vertexArrayObject.mGLHandle );
					_currentDrawTopologyProperties.primitiveTopology = inputLayoutState->mGLInputLayoutDefinition.primitiveTopology;

					_globalStateCache.ApplyIndexBufferBinding( currentVertexStreamDefinition.indexBufferBinding.handle );
					_currentDrawTopologyProperties.indexBufferBaseOffset = currentVertexStreamDefinition.indexBufferBinding.offset;
					_currentDrawTopologyProperties.indexBufferDataType = currentVertexStreamDefinition.indexBufferBinding.format;
					_currentDrawTopologyProperties.indexBufferElementByteSize = currentVertexStreamDefinition.indexBufferBinding.elementByteSize;
				}

				executedUpdatesMask.set( eGraphicsStateUpdateMaskCombinedInputAssembler );
			}

		}

		_stateUpdateMask.unset( eGraphicsStateUpdateMaskCommonAll | eGraphicsStateUpdateMaskSeparableAll );

		return !executedUpdatesMask.empty();
	}

	void GLGraphicsPipelineStateControllerCompat::UpdateShaderInputInlineConstantData(
			const GLGraphicsShaderLinkageImmutableState & pShaderState,
			const ShaderInputParameterConstant & pConstantInfo,
			const void * pConstantData )
	{
		auto * shaderStateCompat = pShaderState.QueryInterface<GLGraphicsShaderLinkageImmutableStateCompat>();

		auto constantBaseType = CxDef::GetVertexAttribFormatBaseDataType( pConstantInfo.iFormat );
		auto constantLength = CxDef::GetVertexAttribFormatComponentsNum( pConstantInfo.iFormat );

		SMU::UpdateUniformDataExplicitGL(
				*shaderStateCompat->mGLShaderProgramObject,
				pConstantInfo.iStageIndex,
				constantBaseType,
				constantLength,
				pConstantData );
	}

	const GLVertexArrayObject & GLGraphicsPipelineStateControllerCompat::GetCachedVertexArrayObject(
			const GLIAInputLayoutImmutableStateCompat & pInputLayoutState,
			const GLIAVertexStreamImmutableState & pVertexStreamState )
	{
		return _vaoCache.GetOrCreate( pInputLayoutState, pVertexStreamState );
	}

	const GLVertexArrayObject & GLGraphicsPipelineStateControllerCompat::GetCachedVertexArrayObject(
			const GLIAInputLayoutDefinition & pInputLayoutDefinition,
			const GLIAVertexStreamDefinition & pVertexStreamDefinition )
	{
		return _vaoCache.GetOrCreate( pInputLayoutDefinition, pVertexStreamDefinition );
	}

} // namespace Ic3::Graphics::GCI
