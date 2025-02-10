
#include "SeparablePipelineState.h"
#include <Ic3/Graphics/GCI/State/RenderTargetCommon.h>

namespace Ic3::Graphics::GCI
{

	//!! GraphicsPipelineStateObjectSeparable

	GraphicsPipelineStateObjectSeparable::GraphicsPipelineStateObjectSeparable(
			GPUDevice & pGPUDevice,
			pipeline_state_object_id_t pStateObjectID,
			const SeparableGraphicsPSDSet & pSeparableDescriptors )
	: GraphicsPipelineStateObject( pGPUDevice, pStateObjectID )
	, mSeparableDescriptors( pSeparableDescriptors )
	{}

	GraphicsPipelineStateObjectSeparable::~GraphicsPipelineStateObjectSeparable() = default;


	//!! GraphicsPipelineStateObjectSeparableShader

	GraphicsPipelineStateObjectSeparableShader::GraphicsPipelineStateObjectSeparableShader(
			GPUDevice & pGPUDevice,
			pipeline_state_object_id_t pStateObjectID,
			const SeparableGraphicsPSDSet & pSeparableDescriptors,
			const GraphicsShaderBinding & pSeparableShaders )
	: GraphicsPipelineStateObjectSeparable( pGPUDevice, pStateObjectID, std::move( pSeparableDescriptors ) )
	, mSeparableShaders( pSeparableShaders )
	{}

	GraphicsPipelineStateObjectSeparableShader::~GraphicsPipelineStateObjectSeparableShader() = default;


	//!! GraphicsPipelineStateControllerSeparable

	GraphicsPipelineStateControllerSeparable::GraphicsPipelineStateControllerSeparable() = default;

	GraphicsPipelineStateControllerSeparable::~GraphicsPipelineStateControllerSeparable() = default;

	const SeparableGraphicsPSDCache & GraphicsPipelineStateControllerSeparable::GetSeparableDescriptors() const noexcept
	{
		return _separableDescriptorsCache;
	}

	bool GraphicsPipelineStateControllerSeparable::SetGraphicsPipelineStateObject(
			const GraphicsPipelineStateObject & pGraphicsPSO )
	{
		// Execute the base set method. THis ensures the PSO handle is saved inside the state controller.
		bool updateResult = GraphicsPipelineStateController::SetGraphicsPipelineStateObject( pGraphicsPSO );

		if( updateResult && _stateUpdateMask.is_set( eGraphicsStateUpdateFlagCommonPipelineStateObjectBit ) )
		{
			const auto * graphicsPSOSeparable = pGraphicsPSO.QueryInterface<GraphicsPipelineStateObjectSeparable>();

			// Update the individual descriptors. Returned mask is a combination of all eGraphicsStateUpdateFlagSeparableState*Bit
			// bits for states which has been updated. Zero means all states in the PSO match those currently bound.
			const auto stateUpdateMask = SetSeparableDescriptors( *graphicsPSOSeparable );

			if( stateUpdateMask.empty() )
			{
				// It is absolutely possible to have two different PSOs that ended up having the same descriptors:
				// for separable states we use cache to ensure each unique state combination is represented by only one
				// state descriptor - if the config is repeated, that cached object will be returned when a PSO is created.
				// When this happens, we skip setting anything and clear the state. Change result to 'false' to indicate this.
				// TODO: This is a rather unwanted scenario. Maybe some logging here would be accurate?
				updateResult = false;

				// Clear the bit for the graphics PSO itself. There is nothing to set.
				_stateUpdateMask.unset( eGraphicsStateUpdateFlagCommonPipelineStateObjectBit );
			}
		}

		return updateResult;
	}

	bool GraphicsPipelineStateControllerSeparable::ResetGraphicsPipelineStateObject()
	{
		return GraphicsPipelineStateController::ResetGraphicsPipelineStateObject();
	}

	cppx::bitmask<uint64> GraphicsPipelineStateControllerSeparable::SetSeparableDescriptors(
			const GraphicsPipelineStateObjectSeparable & pGraphicsPSOSeparable )
	{
		auto stateUpdateMask = cppx::make_bitmask<uint64>( 0 );

		auto * blendStateDescriptor = pGraphicsPSOSeparable.mSeparableDescriptors.blendStateDescriptor.get();
		if( blendStateDescriptor != _separableDescriptorsCache.blendStateDescriptor )
		{
			_separableDescriptorsCache.blendStateDescriptor = blendStateDescriptor;
			stateUpdateMask.set( eGraphicsStateUpdateFlagSeparableStateBlendBit );
		}

		auto * depthStencilStateDescriptor = pGraphicsPSOSeparable.mSeparableDescriptors.depthStencilStateDescriptor.get();
		if( depthStencilStateDescriptor != _separableDescriptorsCache.depthStencilStateDescriptor )
		{
			_separableDescriptorsCache.depthStencilStateDescriptor = depthStencilStateDescriptor;
			stateUpdateMask.set( eGraphicsStateUpdateFlagSeparableStateDepthStencilBit );
		}

		auto * rasterizerStateDescriptor = pGraphicsPSOSeparable.mSeparableDescriptors.rasterizerStateDescriptor.get();
		if( rasterizerStateDescriptor != _separableDescriptorsCache.rasterizerStateDescriptor )
		{
			_separableDescriptorsCache.rasterizerStateDescriptor = rasterizerStateDescriptor;
			stateUpdateMask.set( eGraphicsStateUpdateFlagSeparableStateRasterizerBit );
		}

		auto * iaVertexAttributeLayoutStateDescriptor = pGraphicsPSOSeparable.mSeparableDescriptors.iaVertexAttributeLayoutStateDescriptor.get();
		if( iaVertexAttributeLayoutStateDescriptor != _separableDescriptorsCache.iaVertexAttributeLayoutStateDescriptor )
		{
			_separableDescriptorsCache.iaVertexAttributeLayoutStateDescriptor = iaVertexAttributeLayoutStateDescriptor;
			stateUpdateMask.set( eGraphicsStateUpdateFlagSeparableStateIAVertexAttributeLayoutBit );
		}

		auto * shaderLinkageStateDescriptor = pGraphicsPSOSeparable.mSeparableDescriptors.shaderLinkageStateDescriptor.get();
		if( shaderLinkageStateDescriptor != _separableDescriptorsCache.shaderLinkageStateDescriptor )
		{
			_separableDescriptorsCache.shaderLinkageStateDescriptor = shaderLinkageStateDescriptor;
			stateUpdateMask.set( eGraphicsStateUpdateFlagSeparableStateShaderLinkageBit );
		}

		auto * shaderRootSignatureStateDescriptor = pGraphicsPSOSeparable.mSeparableDescriptors.shaderRootSignatureStateDescriptor.get();
		if( shaderRootSignatureStateDescriptor != _separableDescriptorsCache.shaderRootSignatureStateDescriptor )
		{
			_separableDescriptorsCache.shaderRootSignatureStateDescriptor = shaderRootSignatureStateDescriptor;
			stateUpdateMask.set( eGraphicsStateUpdateFlagSeparableStateShaderRootSignatureBit );
		}

		_stateUpdateMask.set( stateUpdateMask );

		return stateUpdateMask;
	}


	//!! GraphicsPipelineStateControllerSeparableShader

	GraphicsPipelineStateControllerSeparableShader::GraphicsPipelineStateControllerSeparableShader() = default;

	GraphicsPipelineStateControllerSeparableShader::~GraphicsPipelineStateControllerSeparableShader() = default;

	const SeparableShaderCache & GraphicsPipelineStateControllerSeparableShader::GetSeparableShaders() const noexcept
	{
		return _separableShadersCache;
	}

	bool GraphicsPipelineStateControllerSeparableShader::SetGraphicsPipelineStateObject(
			const GraphicsPipelineStateObject & pGraphicsPSO )
	{
		bool updateResult = GraphicsPipelineStateControllerSeparable::SetGraphicsPipelineStateObject( pGraphicsPSO );

		if( updateResult && _stateUpdateMask.is_set( eGraphicsStateUpdateFlagSeparableStateShaderLinkageBit ) )
		{
			const auto * graphicsPSOSeparableShader = pGraphicsPSO.QueryInterface<GraphicsPipelineStateObjectSeparableShader>();

			// Update the individual shaders. Returned mask is a combination of all eGraphicsStateUpdateFlagSeparableShader*Bit
			// bits for shaders which has been updated. Zero means all shaders in the PSO match those currently bound.
			const auto stateUpdateMask = SetSeparableShaders( *graphicsPSOSeparableShader );

			if( stateUpdateMask.empty() )
			{
				updateResult = false;

				// Clear all bits related to all separable shader stages.
				_stateUpdateMask.unset( eGraphicsStateUpdateMaskSeparableShadersAll );

				// Clear the bit for shader linkage state.
				_stateUpdateMask.unset( eGraphicsStateUpdateFlagSeparableStateShaderLinkageBit );
			}
		}

		return updateResult;
	}

	bool GraphicsPipelineStateControllerSeparableShader::ResetGraphicsPipelineStateObject()
	{
		return GraphicsPipelineStateControllerSeparable::ResetGraphicsPipelineStateObject();
	}

	cppx::bitmask<uint64> GraphicsPipelineStateControllerSeparableShader::SetSeparableShaders(
			const GraphicsPipelineStateObjectSeparableShader & pGraphicsPSOSeparableShader )
	{
		auto stateUpdateMask = cppx::make_bitmask<uint64>( 0 );

		auto * vertexShader = pGraphicsPSOSeparableShader.mSeparableShaders[EShaderType::GSVertex];
		if( vertexShader != _separableShadersCache.vertexShader )
		{
			_separableShadersCache.vertexShader = vertexShader;
			stateUpdateMask.set( eGraphicsStateUpdateFlagSeparableShaderVertexBit );
		}

		auto * tessHullShader = pGraphicsPSOSeparableShader.mSeparableShaders[EShaderType::GSTessHull];
		if( tessHullShader != _separableShadersCache.tessHullShader )
		{
			_separableShadersCache.tessHullShader = tessHullShader;
			stateUpdateMask.set( eGraphicsStateUpdateFlagSeparableShaderHullBit );
		}

		auto * tessDomainShader = pGraphicsPSOSeparableShader.mSeparableShaders[EShaderType::GSTessDomain];
		if( tessDomainShader != _separableShadersCache.tessDomainShader )
		{
			_separableShadersCache.tessDomainShader = tessDomainShader;
			stateUpdateMask.set( eGraphicsStateUpdateFlagSeparableShaderDomainBit );
		}

		auto * geometryShader = pGraphicsPSOSeparableShader.mSeparableShaders[EShaderType::GSGeometry];
		if( geometryShader != _separableShadersCache.geometryShader )
		{
			_separableShadersCache.geometryShader = geometryShader;
			stateUpdateMask.set( eGraphicsStateUpdateFlagSeparableShaderGeometryBit );
		}

		auto * pixelShader = pGraphicsPSOSeparableShader.mSeparableShaders[EShaderType::GSPixel];
		if( pixelShader != _separableShadersCache.pixelShader )
		{
			_separableShadersCache.pixelShader = pixelShader;
			stateUpdateMask.set( eGraphicsStateUpdateFlagSeparableShaderPixelBit );
		}

		_stateUpdateMask.set( stateUpdateMask );

		return stateUpdateMask;
	}

} // namespace Ic3::Graphics::GCI
