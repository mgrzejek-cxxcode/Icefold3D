
#include "SeparablePipelineState.h"
#include <Ic3/Graphics/GCI/State/RenderTargetCommon.h>

namespace Ic3::Graphics::GCI
{

	GraphicsPipelineStateObjectSeparable::GraphicsPipelineStateObjectSeparable(
			GPUDevice & pGPUDevice,
			RenderTargetLayout pRenderTargetLayout,
			ShaderInputSignature pShaderInputSignature,
			const SeparablePSOStateSet & pSeparableStates )
	: GraphicsPipelineStateObject(
			pGPUDevice,
			std::move( pRenderTargetLayout ),
			std::move( pShaderInputSignature ) )
	, mSeparableStates( pSeparableStates )
	{}

	GraphicsPipelineStateObjectSeparable::~GraphicsPipelineStateObjectSeparable() = default;


	GraphicsPipelineStateObjectSeparableShader::GraphicsPipelineStateObjectSeparableShader(
			GPUDevice & pGPUDevice,
			RenderTargetLayout pRenderTargetLayout,
			ShaderInputSignature pShaderInputSignature,
			const SeparablePSOStateSet & pSeparableStates,
			const GraphicsShaderSet & pSeparableShaders )
	: GraphicsPipelineStateObjectSeparable(
			pGPUDevice,
			std::move( pRenderTargetLayout ),
			std::move( pShaderInputSignature ),
			pSeparableStates )
	, mSeparableShaders( pSeparableShaders )
	{}

	GraphicsPipelineStateObjectSeparableShader::~GraphicsPipelineStateObjectSeparableShader() = default;


	GraphicsPipelineStateControllerSeparable::GraphicsPipelineStateControllerSeparable() = default;

	GraphicsPipelineStateControllerSeparable::~GraphicsPipelineStateControllerSeparable() = default;

	const SeparablePSOStateCache & GraphicsPipelineStateControllerSeparable::GetCurrentSeparableStates() const noexcept
	{
		return _currentSeparableStates;
	}

	bool GraphicsPipelineStateControllerSeparable::SetGraphicsPipelineStateObject(
			const GraphicsPipelineStateObject & pGraphicsPSO )
	{
		bool updateResult = GraphicsPipelineStateController::SetGraphicsPipelineStateObject( pGraphicsPSO );

		if( updateResult && _stateUpdateMask.is_set( eGraphicsStateUpdateFlagCommonPSOBit ) )
		{
			const auto * graphicsPSOSeparable = pGraphicsPSO.QueryInterface<GraphicsPipelineStateObjectSeparable>();

			// Update the individual states. Returned mask is a combination of all eGraphicsStateUpdateFlagSeparableState*Bit
			// bits for states which has been updated. Zero means all states in the PSO match those currently bound.
			const auto stateUpdateMask = SetSeparablePSOStates( *graphicsPSOSeparable );

			if( stateUpdateMask.empty() )
			{
				// It is absolutely possible to have two different PSOs that ended up having the same shaders and pdesc:
				// for separable states we use cache to ensure each unique state combination is represented by only one
				// state descriptor - if the config is repeated, that cached object will be returned when a PSO is created.
				// When this happens, we skip setting anything and clear the state. Change result to 'false' to indicate this.
				// TODO: This is a rather unwanted scenario. Maybe some logging here would be accurate?
				updateResult = false;

				// Clear the bit for the graphics PSO itself. There is nothing to set.
				_stateUpdateMask.unset( eGraphicsStateUpdateFlagCommonPSOBit );
			}
		}

		return updateResult;
	}

	bool GraphicsPipelineStateControllerSeparable::ResetGraphicsPipelineStateObject()
	{
		return GraphicsPipelineStateController::ResetGraphicsPipelineStateObject();
	}

	cppx::bitmask<uint64> GraphicsPipelineStateControllerSeparable::SetSeparablePSOStates(
			const GraphicsPipelineStateObjectSeparable & pGraphicsPSOSeparable )
	{
		auto stateUpdateMask = cppx::make_bitmask<uint64>( 0 );

		auto * blendState = pGraphicsPSOSeparable.mSeparableStates.blendState.get();
		if( blendState != _currentSeparableStates.blendState )
		{
			_currentSeparableStates.blendState = blendState;
			stateUpdateMask.set( eGraphicsStateUpdateFlagSeparableStateBlendBit );
		}

		auto * depthStencilState = pGraphicsPSOSeparable.mSeparableStates.depthStencilState.get();
		if( depthStencilState != _currentSeparableStates.depthStencilState )
		{
			_currentSeparableStates.depthStencilState = depthStencilState;
			stateUpdateMask.set( eGraphicsStateUpdateFlagSeparableStateDepthStencilBit );
		}

		auto * rasterizerState = pGraphicsPSOSeparable.mSeparableStates.rasterizerState.get();
		if( rasterizerState != _currentSeparableStates.rasterizerState )
		{
			_currentSeparableStates.rasterizerState = rasterizerState;
			stateUpdateMask.set( eGraphicsStateUpdateFlagSeparableStateRasterizerBit );
		}

		auto * iaInputLayoutState = pGraphicsPSOSeparable.mSeparableStates.iaInputLayoutState.get();
		if( iaInputLayoutState != _currentSeparableStates.iaInputLayoutState )
		{
			_currentSeparableStates.iaInputLayoutState = iaInputLayoutState;
			stateUpdateMask.set( eGraphicsStateUpdateFlagSeparableStateIAInputLayoutBit );
		}

		auto * shaderLinkageState = pGraphicsPSOSeparable.mSeparableStates.shaderLinkageState.get();
		if( shaderLinkageState != _currentSeparableStates.shaderLinkageState )
		{
			_currentSeparableStates.shaderLinkageState = shaderLinkageState;
			stateUpdateMask.set( eGraphicsStateUpdateFlagSeparableStateShaderLinkageBit );
		}

		_stateUpdateMask.set( stateUpdateMask );

		return stateUpdateMask;
	}


	GraphicsPipelineStateControllerSeparableShader::GraphicsPipelineStateControllerSeparableShader() = default;

	GraphicsPipelineStateControllerSeparableShader::~GraphicsPipelineStateControllerSeparableShader() = default;

	const SeparableShaderCache & GraphicsPipelineStateControllerSeparableShader::GetCurrentSeparableShaders() const noexcept
	{
		return _currentSeparableShaders;
	}

	bool GraphicsPipelineStateControllerSeparableShader::SetGraphicsPipelineStateObject(
			const GraphicsPipelineStateObject & pGraphicsPSO )
	{
		bool updateResult = GraphicsPipelineStateControllerSeparable::SetGraphicsPipelineStateObject( pGraphicsPSO );

		if( updateResult && _stateUpdateMask.is_set( eGraphicsStateUpdateFlagSeparableStateShaderLinkageBit ) )
		{
			const auto * graphicsPSOSeparableShader = pGraphicsPSO.QueryInterface<GraphicsPipelineStateObjectSeparableShader>();

			// Update the individual states. Returned mask is a combination of all eGraphicsStateUpdateFlagSeparableState*Bit
			// bits for states which has been updated. Zero means all states in the PSO match those currently bound.
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
		if( vertexShader != _currentSeparableShaders.vertexShader )
		{
			_currentSeparableShaders.vertexShader = vertexShader;
			stateUpdateMask.set( eGraphicsStateUpdateFlagSeparableShaderVertexBit );
		}

		auto * hullShader = pGraphicsPSOSeparableShader.mSeparableShaders[EShaderType::GSHull];
		if( hullShader != _currentSeparableShaders.hullShader )
		{
			_currentSeparableShaders.hullShader = hullShader;
			stateUpdateMask.set( eGraphicsStateUpdateFlagSeparableShaderHullBit );
		}

		auto * domainShader = pGraphicsPSOSeparableShader.mSeparableShaders[EShaderType::GSDomain];
		if( domainShader != _currentSeparableShaders.domainShader )
		{
			_currentSeparableShaders.domainShader = domainShader;
			stateUpdateMask.set( eGraphicsStateUpdateFlagSeparableShaderDomainBit );
		}

		auto * geometryShader = pGraphicsPSOSeparableShader.mSeparableShaders[EShaderType::GSGeometry];
		if( geometryShader != _currentSeparableShaders.geometryShader )
		{
			_currentSeparableShaders.geometryShader = geometryShader;
			stateUpdateMask.set( eGraphicsStateUpdateFlagSeparableShaderGeometryBit );
		}

		auto * pixelShader = pGraphicsPSOSeparableShader.mSeparableShaders[EShaderType::GSPixel];
		if( pixelShader != _currentSeparableShaders.pixelShader )
		{
			_currentSeparableShaders.pixelShader = pixelShader;
			stateUpdateMask.set( eGraphicsStateUpdateFlagSeparableShaderPixelBit );
		}

		_stateUpdateMask.set( stateUpdateMask );

		return stateUpdateMask;
	}

} // namespace Ic3::Graphics::GCI
