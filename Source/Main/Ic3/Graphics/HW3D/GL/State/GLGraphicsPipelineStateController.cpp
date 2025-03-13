
#include "GLGraphicsPipelineStateController.h"
#include "GLGraphicsPipelineStateCommon.h"
#include "GLGraphicsPipelineStateIA.h"
#include "GLGraphicsPipelineStateShader.h"
#include "GLGraphicsPipelineStateRTO.h"
#include "GLGraphicsPipelineStateObject.h"
#include "GLVertexArrayObjectCache.h"
#include <Ic3/Graphics/HW3D/GL/GLCommandList.h>
#include <Ic3/Graphics/HW3D/GL/GLGPUDevice.h>
#include <Ic3/Graphics/HW3D/GL/Objects/GLFramebufferObject.h>
#include <Ic3/Graphics/HW3D/GL/Objects/GLShaderPipelineObject.h>
#include <Ic3/Graphics/HW3D/GL/Objects/GLVertexArrayObject.h>
#include <Ic3/Graphics/HW3D/GL/Resources/GLGPUBuffer.h>
#include <Ic3/Graphics/HW3D/GL/Resources/GLSampler.h>
#include <Ic3/Graphics/HW3D/GL/Resources/GLShader.h>
#include <Ic3/Graphics/HW3D/GL/Resources/GLTexture.h>

#include <Ic3/Graphics/GCI/State/GraphicsPipelineStateDescriptorIADynamic.h>
#include <Ic3/Graphics/GCI/State/GraphicsPipelineStateDescriptorRTODynamic.h>

#include "Ic3/Graphics/GCI/State/PipelineStateDescriptorRootSignature.h"

namespace Ic3::Graphics::GCI
{

	GLGraphicsPipelineStateController::GLGraphicsPipelineStateController( GLCommandList & pGLCommandList )
	: BaseStateControllerType()
	, mGLGPUDevice( pGLCommandList.mGPUDevice.GetRef<GLGPUDevice>() )
	{}

	GLGraphicsPipelineStateController::~GLGraphicsPipelineStateController() = default;

	const GLDrawTopologyProperties & GLGraphicsPipelineStateController::GetCurrentDrawTopologyProperties() const noexcept
	{
		return _glcCurrentDrawTopologyProperties;
	}

	bool GLGraphicsPipelineStateController::ApplyStateChanges()
	{
		cppx::bitmask<uint32> executedUpdatesMask = 0;

		if( _stateUpdateMask.is_set( eGraphicsStateUpdateFlagCommonPipelineStateObjectBit ) )
		{
			if( _stateUpdateMask.is_set_any_of( eGraphicsStateUpdateMaskSeparableStatesAll ) )
			{
				const auto * glcGraphicsPipelineStateObject = GetCurrentGraphicsPipelineStateObject<GLGraphicsPipelineStateObject>();
				const auto commonConfigDescriptorsUpdateMask = BindCommonConfigDescriptors( *glcGraphicsPipelineStateObject );
				executedUpdatesMask.set( commonConfigDescriptorsUpdateMask );
			}
		}

		if( _stateUpdateMask.is_set( eGraphicsStateUpdateFlagCommonRenderTargetDescriptorBit ) )
		{
			ApplyGLRenderTargetBinding( GetGLRenderTargetBinding() );
			executedUpdatesMask.set( eGraphicsStateUpdateFlagCommonRenderTargetDescriptorBit );
		}

		ApplyGraphicsPipelineDynamicConfig( GetPipelineDynamicConfig() );

		return !executedUpdatesMask.empty();
	}

	bool GLGraphicsPipelineStateController::SetGraphicsPipelineStateObject( const GraphicsPipelineStateObject & pGraphicsPipelineStateObject )
	{
		return BaseStateControllerType::SetGraphicsPipelineStateObject( pGraphicsPipelineStateObject );
	}

	bool GLGraphicsPipelineStateController::ResetGraphicsPipelineStateObject()
	{
		return BaseStateControllerType::ResetGraphicsPipelineStateObject();
	}

	bool GLGraphicsPipelineStateController::SetRenderPassDescriptor( const RenderPassDescriptor & pRenderPassDescriptor )
	{
		return BaseStateControllerType::SetRenderPassDescriptor( pRenderPassDescriptor );
	}

	bool GLGraphicsPipelineStateController::SetRenderPassDescriptorDynamic( RenderPassDescriptorDynamic & pRenderPassDescriptor )
	{
		return BaseStateControllerType::SetRenderPassDescriptorDynamic( pRenderPassDescriptor );
	}

	void GLGraphicsPipelineStateController::ResetRenderPassDescriptor()
	{
		BaseStateControllerType::ResetRenderPassDescriptor();
	}

	bool GLGraphicsPipelineStateController::SetRenderTargetDescriptor(
			const RenderTargetDescriptor & pRenderTargetDescriptor )
	{
		const bool updateResult = BaseStateControllerType::SetRenderTargetDescriptor( pRenderTargetDescriptor );

		if( updateResult )
		{
			const auto * glcRenderTargetDescriptor = pRenderTargetDescriptor.QueryInterface<GLRenderTargetDescriptor>();
			_glcCurrentPipelineBindings.renderTargetBinding = &( glcRenderTargetDescriptor->mGLRenderTargetBinding );
		}

		return updateResult;
	}

	bool GLGraphicsPipelineStateController::SetRenderTargetDescriptorDynamic(
			RenderTargetDescriptorDynamic & pRenderTargetDescriptor )
	{
		const bool updateResult = BaseStateControllerType::SetRenderTargetDescriptorDynamic(
				pRenderTargetDescriptor );

		if( updateResult )
		{
			if( !pRenderTargetDescriptor.IsDynamicDriverStateInitialized() )
			{
				InitializeDynamicDriverStateForDescriptor<GLRenderTargetBindingDynamic>( pRenderTargetDescriptor );
			}
			auto * glcRenderTargetDriverState = pRenderTargetDescriptor.GetDynamicDriverStateAs<GLRenderTargetBindingDynamic>();

			auto & glcRenderTargetBinding = glcRenderTargetDriverState->mDriverData;
			if( &glcRenderTargetBinding != _glcCurrentPipelineBindings.renderTargetBinding )
			{
				if( IsDynamicDescriptorConfigurationModified( pRenderTargetDescriptor ) )
				{
					GCU::RTOUpdateRenderTargetBindingForDynamicDescriptorGL(
							pRenderTargetDescriptor.GetRenderTargetArrayConfiguration(),
							glcRenderTargetBinding );

					ResetDynamicDescriptorConfigurationModifiedState( pRenderTargetDescriptor );
				}

				_glcCurrentPipelineBindings.renderTargetBinding = &glcRenderTargetBinding;
			}
		}

		return updateResult;
	}

	bool GLGraphicsPipelineStateController::ResetRenderTargetDescriptor()
	{
		const bool updateResult = BaseStateControllerType::ResetRenderTargetDescriptor();

		if( updateResult )
		{
			_glcCurrentPipelineBindings.renderTargetBinding = nullptr;
		}

		return updateResult;
	}

	bool GLGraphicsPipelineStateController::SetVertexSourceBindingDescriptor(
			const VertexSourceBindingDescriptor & pVertexSourceBindingDescriptor)
	{
		const bool updateResult = BaseStateControllerType::SetVertexSourceBindingDescriptor( pVertexSourceBindingDescriptor );

		if( updateResult )
		{
			const auto * glcVertexSourceBindingDescriptor = pVertexSourceBindingDescriptor.QueryInterface<GLVertexSourceBindingDescriptor>();
			_glcCurrentPipelineBindings.vertexSourceBinding = glcVertexSourceBindingDescriptor->mGLVertexSourceBinding.get();
		}

		return updateResult;
	}

	bool GLGraphicsPipelineStateController::SetVertexSourceBindingDescriptorDynamic(
			VertexSourceBindingDescriptorDynamic & pVertexSourceBindingDescriptor )
	{
		const bool updateResult = BaseStateControllerType::SetVertexSourceBindingDescriptorDynamic( pVertexSourceBindingDescriptor );

		if( updateResult )
		{
			if( !pVertexSourceBindingDescriptor.IsDynamicDriverStateInitialized() )
			{
				auto * glcVertexSourceBindingDriverState =
					InitializeDynamicDriverStateForDescriptor<GLIAVertexSourceBindingPtr>( pVertexSourceBindingDescriptor );

				const auto activeVertexBuffersNum = pVertexSourceBindingDescriptor.CountActiveVertexBuffers();
				glcVertexSourceBindingDriverState->mDriverData = MakeUniqueDynamicallySizedTemplate<GLIAVertexSourceBinding>( activeVertexBuffersNum );
			}

			auto * glcVertexSourceBindingDriverState = pVertexSourceBindingDescriptor.GetDynamicDriverStateAs<GLIAVertexSourceBindingPtr>();
			auto & glcVertexSourceBindingPtr = glcVertexSourceBindingDriverState->mDriverData;

			if( glcVertexSourceBindingPtr.get() != _glcCurrentPipelineBindings.vertexSourceBinding )
			{
				if( IsDynamicDescriptorConfigurationModified( pVertexSourceBindingDescriptor ) )
				{
					GCU::IAUpdateVertexSourceBindingDefinitionGL(
							pVertexSourceBindingDescriptor.GetVertexSourceBindingDefinition(),
							*glcVertexSourceBindingPtr );

					ResetDynamicDescriptorConfigurationModifiedState( pVertexSourceBindingDescriptor );
				}

				_glcCurrentPipelineBindings.vertexSourceBinding = glcVertexSourceBindingPtr.get();
			}

		}

		return updateResult;
	}

	bool GLGraphicsPipelineStateController::ResetVertexSourceBindingDescriptor()
	{
		const bool updateResult = BaseStateControllerType::ResetVertexSourceBindingDescriptor();

		if( updateResult )
		{
			_glcCurrentPipelineBindings.vertexSourceBinding = nullptr;
		}

		return updateResult;
	}

	bool GLGraphicsPipelineStateController::SetViewport( const ViewportDesc & pViewportDesc )
	{
		bool baseResult = BaseStateControllerType::SetViewport( pViewportDesc );

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
		bool baseResult = BaseStateControllerType::SetShaderConstant( pParamRefID, pData );

		if( baseResult )
		{
			const auto & rootSignatureDescriptor = GetCurrentPSORootSignatureDescriptor<RootSignatureDescriptorGeneric>();
			Ic3DebugAssert( rootSignatureDescriptor.mRootSignature );

			const auto & constantInfo = rootSignatureDescriptor.mRootSignature.GetConstantInfo( pParamRefID );
			if( constantInfo.iVisibilityMask != 0 )
			{
				const auto & glcShaderLinkageDescriptor = GetCurrentPSOShaderLinkageDescriptor<GLGraphicsShaderLinkageDescriptor>();
				UpdateShaderInputInlineConstantData( glcShaderLinkageDescriptor, constantInfo, pData );
			}
		}

		return baseResult;
	}

	bool GLGraphicsPipelineStateController::SetShaderConstantBuffer( shader_input_ref_id_t pParamRefID, GPUBuffer & pConstantBuffer )
	{
		bool baseResult = BaseStateControllerType::SetShaderConstantBuffer( pParamRefID, pConstantBuffer );

		if( baseResult )
		{
			const auto & rootSignatureDescriptor = GetCurrentPSORootSignatureDescriptor<RootSignatureDescriptorGeneric>();
			Ic3DebugAssert( rootSignatureDescriptor.mRootSignature );

			const auto & descriptorInfo = rootSignatureDescriptor.mRootSignature.GetDescriptorInfo( pParamRefID );
			Ic3DebugAssert( descriptorInfo.dDescriptorType == EShaderInputDescriptorType::Resource );
			Ic3DebugAssert( descriptorInfo.uResourceInfo.resourceType == EShaderInputResourceType::CBVConstantBuffer );

			if( descriptorInfo.dShaderVisibilityMask != 0 )
			{
				auto * glcGPUBuffer = pConstantBuffer.QueryInterface<GLGPUBuffer>();

				glBindBufferBase(
						GL_UNIFORM_BUFFER,
						descriptorInfo.uResourceInfo.resourceBaseRegisterIndex,
						glcGPUBuffer->mGLBufferObject->mGLHandle );
				Ic3OpenGLHandleLastError();
			}
		}

		return baseResult;
	}

	bool GLGraphicsPipelineStateController::SetShaderTextureImage( shader_input_ref_id_t pParamRefID, Texture & pTexture )
	{
		bool baseResult = BaseStateControllerType::SetShaderTextureImage( pParamRefID, pTexture );

		if( baseResult )
		{
			const auto & rootSignatureDescriptor = GetCurrentPSORootSignatureDescriptor<RootSignatureDescriptorGeneric>();
			Ic3DebugAssert( rootSignatureDescriptor.mRootSignature );

			const auto & descriptorInfo = rootSignatureDescriptor.mRootSignature.GetDescriptorInfo( pParamRefID );
			Ic3DebugAssert( descriptorInfo.dDescriptorType == EShaderInputDescriptorType::Resource );
			Ic3DebugAssert( descriptorInfo.uResourceInfo.resourceType == EShaderInputResourceType::SRVTextureImage );

			if( descriptorInfo.dShaderVisibilityMask != 0 )
			{
				auto * glcTexture = pTexture.QueryInterface<GLTexture>();

				glActiveTexture( GL_TEXTURE0 + descriptorInfo.uResourceInfo.resourceBaseRegisterIndex );
				Ic3OpenGLHandleLastError();

				glBindTexture(
						glcTexture->mGLTextureObject->mGLTextureBindTarget,
						glcTexture->mGLTextureObject->mGLHandle );
				Ic3OpenGLHandleLastError();
			}
		}

		return baseResult;
	}

	bool GLGraphicsPipelineStateController::SetShaderTextureSampler( shader_input_ref_id_t pParamRefID, Sampler & pSampler )
	{
		bool baseResult = BaseStateControllerType::SetShaderTextureSampler( pParamRefID, pSampler );

		if( baseResult )
		{
			const auto & rootSignatureDescriptor = GetCurrentPSORootSignatureDescriptor<RootSignatureDescriptorGeneric>();
			Ic3DebugAssert( rootSignatureDescriptor.mRootSignature );

			const auto & descriptorInfo = rootSignatureDescriptor.mRootSignature.GetDescriptorInfo( pParamRefID );
			Ic3DebugAssert( descriptorInfo.dDescriptorType == EShaderInputDescriptorType::Sampler );

			if( descriptorInfo.dShaderVisibilityMask != 0 )
			{
				auto * glcSampler = pSampler.QueryInterface<GLSampler>();

				glBindSampler(
						descriptorInfo.uSamplerInfo.samplerBindingIndex,
						glcSampler->mGLSamplerObject->mGLHandle );
				Ic3OpenGLHandleLastError();
			}
		}

		return baseResult;
	}

	const GLRenderTargetBinding & GLGraphicsPipelineStateController::GetGLRenderTargetBinding() const noexcept
	{
		return *( _glcCurrentPipelineBindings.renderTargetBinding );
	}

	const GLIAVertexSourceBindingBase & GLGraphicsPipelineStateController::GetGLVertexSourceBinding() const noexcept
	{
		return *( _glcCurrentPipelineBindings.vertexSourceBinding );
	}

	cppx::bitmask<uint32> GLGraphicsPipelineStateController::BindCommonConfigDescriptors(
		const GLGraphicsPipelineStateObject & pGLGraphicsPipelineStateObject )
	{
		cppx::bitmask<uint32> executedUpdatesMask = 0;

		if( _stateUpdateMask.is_set( eGraphicsStateUpdateFlagSeparableStateBlendBit ) )
		{
			const auto & glcBlendStateDescriptor = pGLGraphicsPipelineStateObject.GetGLBlendStateDescriptor();
			const auto & dynamicPipelineConfig = GetPipelineDynamicConfig();

			const bool setConstantColor =
				glcBlendStateDescriptor.mGLBlendSettings.flags.is_set( eBlendConfigFlagSetFixedBlendConstantsBit ) &&
				!dynamicPipelineConfig.activeStateMask.is_set( eGraphicsPipelineDynamicConfigFlagBlendConstantColorBit );

			_glcGlobalStateCache.ApplyBlendSettings( glcBlendStateDescriptor.mGLBlendSettings, setConstantColor );
			executedUpdatesMask.set( eGraphicsStateUpdateFlagSeparableStateBlendBit );
		}

		if( _stateUpdateMask.is_set( eGraphicsStateUpdateFlagSeparableStateDepthStencilBit ) )
		{
			const auto & glcDepthStencilStateDescriptor = pGLGraphicsPipelineStateObject.GetGLDepthStencilStateDescriptor();
			const auto & dynamicPipelineConfig = GetPipelineDynamicConfig();

			const auto setDefaultStencilRefValue =
				!dynamicPipelineConfig.activeStateMask.is_set( eGraphicsPipelineDynamicConfigFlagStencilRefValueBit );

			const auto stencilRefValue =
				setDefaultStencilRefValue ? kPipelineConfigDefaultStencilTestRefValue : dynamicPipelineConfig.stencilTestRefValue;

			_glcGlobalStateCache.ApplyDepthStencilSettings( glcDepthStencilStateDescriptor.mGLDepthStencilSettings, stencilRefValue );
			executedUpdatesMask.set( eGraphicsStateUpdateFlagSeparableStateDepthStencilBit );
		}

		if( _stateUpdateMask.is_set( eGraphicsStateUpdateFlagSeparableStateRasterizerBit ) )
		{
			const auto & glcRasterizerStateDescriptor = pGLGraphicsPipelineStateObject.GetGLRasterizerStateDescriptor();

			_glcGlobalStateCache.ApplyRasterizerSettings( glcRasterizerStateDescriptor.mGLRasterizerSettings );
			executedUpdatesMask.set( eGraphicsStateUpdateFlagSeparableStateRasterizerBit );
		}

		return executedUpdatesMask;
	}

	void GLGraphicsPipelineStateController::ApplyGLRenderTargetBinding( const GLRenderTargetBinding & pGLRenderTargetBinding )
	{
		glBindFramebuffer( GL_FRAMEBUFFER, pGLRenderTargetBinding.baseFramebuffer->mGLHandle );
		Ic3OpenGLHandleLastError();
	}

	void GLGraphicsPipelineStateController::ApplyGraphicsPipelineDynamicConfig( const GraphicsPipelineDynamicConfig & pDynamicPipelineConfig )
	{
		if( pDynamicPipelineConfig.activeStateMask.is_set( eGraphicsPipelineDynamicConfigFlagBlendConstantColorBit ) )
		{

			glBlendColor(
					pDynamicPipelineConfig.blendConstantColor.ufp_red,
					pDynamicPipelineConfig.blendConstantColor.ufp_green,
					pDynamicPipelineConfig.blendConstantColor.ufp_blue,
					pDynamicPipelineConfig.blendConstantColor.ufp_alpha );
			Ic3OpenGLHandleLastError();
		}
	}


	GLGraphicsPipelineStateControllerCore::GLGraphicsPipelineStateControllerCore( GLCommandList & pGLCommandList )
	: GLGraphicsPipelineStateController( pGLCommandList )
	{}

	GLGraphicsPipelineStateControllerCore::~GLGraphicsPipelineStateControllerCore() = default;

	bool GLGraphicsPipelineStateControllerCore::ApplyStateChanges()
	{
		cppx::bitmask<uint32> executedUpdatesMask = 0;

		if( GLGraphicsPipelineStateController::ApplyStateChanges() )
		{
			const auto & glcGraphicsPipelineStateObject = GetCurrentGraphicsPipelineStateObjectRef<GLGraphicsPipelineStateObject>();

			if( _stateUpdateMask.is_set_any_of( eGraphicsStateUpdateFlagSeparableStateShaderLinkageBit ) )
			{
				const auto & glcShaderLinkageDescriptor = GetCurrentPSOShaderLinkageDescriptor<GLGraphicsShaderLinkageDescriptorCore>();

				_glcGlobalStateCache.ApplyShaderPipelineBinding( glcShaderLinkageDescriptor.mGLShaderPipelineObject->mGLHandle );
				executedUpdatesMask.set( eGraphicsStateUpdateFlagSeparableStateShaderLinkageBit );
			}

			if( _stateUpdateMask.is_set( eGraphicsStateUpdateFlagSeparableStateVertexAttributeLayoutBit ) )
			{
				const auto & glcVertexAttributeLayoutDescriptor = GetCurrentPSOVertexAttributeLayoutDescriptor<GLVertexAttributeLayoutDescriptorCore>();

				_glcGlobalStateCache.ApplyVertexArrayObjectBinding( glcVertexAttributeLayoutDescriptor.mGLVertexArrayObject->mGLHandle );
				_glcCurrentDrawTopologyProperties.primitiveTopology = glcVertexAttributeLayoutDescriptor.mGLPrimitiveTopology;
				executedUpdatesMask.set( eGraphicsStateUpdateFlagSeparableStateVertexAttributeLayoutBit );
			}

			if( _stateUpdateMask.is_set_any_of( eGraphicsStateUpdateMaskCombinedInputAssembler ) )
			{
				const auto & glcVertexSourceBinding = GetGLVertexSourceBinding();

				// Vertex buffer bindings are executed directly by the state controller - no caching is done here..
				ApplyVertexBufferBindings( glcVertexSourceBinding );

				// Execute the binding for the index buffer.
				const auto & indexBufferBinding = glcVertexSourceBinding.indexBufferBinding;
				// Index buffer binding is propagated through the global state cache.
				_glcGlobalStateCache.ApplyIndexBufferBinding( indexBufferBinding.handle );

				_glcCurrentDrawTopologyProperties.indexBufferBaseOffset = indexBufferBinding.offset;
				_glcCurrentDrawTopologyProperties.indexBufferDataType = indexBufferBinding.format;
				_glcCurrentDrawTopologyProperties.indexBufferElementByteSize = indexBufferBinding.elementByteSize;

				executedUpdatesMask.set( eGraphicsStateUpdateMaskCombinedInputAssembler );
			}
		}

		_stateUpdateMask.unset( eGraphicsStateUpdateMaskCommonAll | eGraphicsStateUpdateMaskSeparableAll );

		return !executedUpdatesMask.empty();
	}

	void GLGraphicsPipelineStateControllerCore::UpdateShaderInputInlineConstantData(
			const GLGraphicsShaderLinkageDescriptor & pShaderLinkageDescriptor,
			const ShaderInputParameterConstant & pConstantInfo,
			const void * pConstantData )
	{
		auto * glcShaderLinkageDescriptorCore = pShaderLinkageDescriptor.QueryInterface<GLGraphicsShaderLinkageDescriptorCore>();

		auto constantBaseType = CXU::GetVertexAttribFormatBaseDataType( pConstantInfo.iFormat );
		auto constantLength = CXU::GetVertexAttribFormatComponentsNum( pConstantInfo.iFormat );

		GCU::UpdateUniformDataCurrentGL(
				*( glcShaderLinkageDescriptorCore->mGLShaderPipelineObject ),
				pConstantInfo.iStageIndex,
				constantBaseType,
				constantLength,
				pConstantData );
	}


	void GLGraphicsPipelineStateControllerCore::ApplyVertexBufferBindings( const GLIAVertexSourceBindingBase & pGLVertexSourceBinding )
	{
		for( const auto & vertexBufferBinding : pGLVertexSourceBinding.vertexBufferBindings )
		{
			glBindVertexBuffer(
				vertexBufferBinding.streamIndex,
				vertexBufferBinding.handle,
				vertexBufferBinding.offset,
				vertexBufferBinding.stride );
			Ic3OpenGLHandleLastError();
		}
	}


	GLGraphicsPipelineStateControllerCompat::GLGraphicsPipelineStateControllerCompat( GLCommandList & pGLCommandList )
	: GLGraphicsPipelineStateController( pGLCommandList )
	{}

	GLGraphicsPipelineStateControllerCompat::~GLGraphicsPipelineStateControllerCompat() = default;

	bool GLGraphicsPipelineStateControllerCompat::ApplyStateChanges()
	{
		cppx::bitmask<uint32> executedUpdatesMask = 0;

		if( GLGraphicsPipelineStateController::ApplyStateChanges() )
		{
			if( _stateUpdateMask.is_set_any_of( eGraphicsStateUpdateFlagSeparableStateShaderLinkageBit ) )
			{
				const auto & glcShaderLinkageState =
						GetCurrentPSOShaderLinkageDescriptor<GLGraphicsShaderLinkageDescriptorCompat>();

				_glcGlobalStateCache.ApplyShaderProgramBinding( glcShaderLinkageState.mGLShaderProgramObject->mGLHandle );

				executedUpdatesMask.set( eGraphicsStateUpdateFlagSeparableStateShaderLinkageBit );
			}

			if( _stateUpdateMask.is_set_any_of( eGraphicsStateUpdateMaskCombinedInputAssembler ) )
			{
				const auto & glcVertexAttributeLayoutDescriptor =
						GetCurrentPSOVertexAttributeLayoutDescriptor<GLVertexAttributeLayoutDescriptorCompat>();

				const auto & glcVertexSourceBinding = GetGLVertexSourceBinding();

				const auto & vertexArrayObject = GetCachedVertexArrayObject(
						glcVertexAttributeLayoutDescriptor.mGLVertexAttributeLayout,
						glcVertexSourceBinding );

				_glcGlobalStateCache.ApplyVertexArrayObjectBinding( vertexArrayObject.mGLHandle );
				_glcGlobalStateCache.ApplyIndexBufferBinding( glcVertexSourceBinding.indexBufferBinding.handle );

				_glcCurrentDrawTopologyProperties.indexBufferBaseOffset = glcVertexSourceBinding.indexBufferBinding.offset;
				_glcCurrentDrawTopologyProperties.indexBufferDataType = glcVertexSourceBinding.indexBufferBinding.format;
				_glcCurrentDrawTopologyProperties.indexBufferElementByteSize = glcVertexSourceBinding.indexBufferBinding.elementByteSize;

				_glcCurrentDrawTopologyProperties.primitiveTopology = glcVertexAttributeLayoutDescriptor.mGLVertexAttributeLayout.glcPrimitiveTopology;

				executedUpdatesMask.set( eGraphicsStateUpdateMaskCombinedInputAssembler );
			}

		}

		_stateUpdateMask.unset( eGraphicsStateUpdateMaskCommonAll | eGraphicsStateUpdateMaskSeparableAll );

		return !executedUpdatesMask.empty();
	}

	void GLGraphicsPipelineStateControllerCompat::UpdateShaderInputInlineConstantData(
			const GLGraphicsShaderLinkageDescriptor & pShaderLinkageDescriptor,
			const ShaderInputParameterConstant & pConstantInfo,
			const void * pConstantData )
	{
		auto * glcShaderLinkageDescriptorCompat = pShaderLinkageDescriptor.QueryInterface<GLGraphicsShaderLinkageDescriptorCompat>();

		auto constantBaseType = CXU::GetVertexAttribFormatBaseDataType( pConstantInfo.iFormat );
		auto constantLength = CXU::GetVertexAttribFormatComponentsNum( pConstantInfo.iFormat );

		GCU::UpdateUniformDataExplicitGL(
				*( glcShaderLinkageDescriptorCompat->mGLShaderProgramObject ),
				pConstantInfo.iStageIndex,
				constantBaseType,
				constantLength,
				pConstantData );
	}

	const GLVertexArrayObject & GLGraphicsPipelineStateControllerCompat::GetCachedVertexArrayObject(
			const GLIAVertexAttributeLayout & pGLAttributeLayoutDefinition,
			const GLIAVertexSourceBindingBase & pGLVertexSourceBinding )
	{
		auto * glcVertexArrayObjectCache = mGLGPUDevice.GetVertexArrayObjectCache();
		Ic3DebugAssert( glcVertexArrayObjectCache );
		return glcVertexArrayObjectCache->GetOrCreate( pGLAttributeLayoutDefinition, pGLVertexSourceBinding );
	}

} // namespace Ic3::Graphics::GCI
