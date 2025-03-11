
#include "GraphicsPipelineStateImplSeparableState.h"
#include <Ic3/Graphics/GCI/State/GraphicsPipelineStateDescriptorCommon.h>
#include <Ic3/Graphics/GCI/State/GraphicsPipelineStateDescriptorIA.h>
#include <Ic3/Graphics/GCI/State/GraphicsPipelineStateDescriptorRTO.h>
#include <Ic3/Graphics/GCI/State/GraphicsPipelineStateDescriptorShader.h>
#include <Ic3/Graphics/GCI/State/PipelineStateDescriptorRootSignature.h>

namespace Ic3::Graphics::GCI
{

	// GraphicsPipelineStateObjectSeparable

	GraphicsPipelineStateObjectSeparable::GraphicsPipelineStateObjectSeparable(
			GPUDevice & pGPUDevice,
			pipeline_state_object_id_t pStateObjectID,
			const RenderTargetLayout & pRenderTargetLayout,
			const SeparableGraphicsPSDSet & pSeparableDescriptors )
	: GraphicsPipelineStateObject(
		pGPUDevice,
		pStateObjectID,
		pRenderTargetLayout,
		GCU::GetDescriptorsIDSetForSeparablePSO( pSeparableDescriptors ) )
	, mSeparableDescriptors( pSeparableDescriptors )
	{}

	GraphicsPipelineStateObjectSeparable::~GraphicsPipelineStateObjectSeparable() = default;


	// GraphicsPipelineStateObjectSeparableShader

	GraphicsPipelineStateObjectSeparableShader::GraphicsPipelineStateObjectSeparableShader(
			GPUDevice & pGPUDevice,
			pipeline_state_object_id_t pStateObjectID,
			const RenderTargetLayout & pRenderTargetLayout,
			const SeparableGraphicsPSDSet & pSeparableDescriptors,
			const GraphicsShaderBinding & pBindingConfiguration )
	: GraphicsPipelineStateObjectSeparable(
		pGPUDevice,
		pStateObjectID,
		pRenderTargetLayout,
		pSeparableDescriptors )
	, mBindingConfiguration( pBindingConfiguration )
	{}

	GraphicsPipelineStateObjectSeparableShader::~GraphicsPipelineStateObjectSeparableShader() = default;


	//GraphicsPipelineStateControllerSeparable

	GraphicsPipelineStateControllerSeparable::GraphicsPipelineStateControllerSeparable() = default;

	GraphicsPipelineStateControllerSeparable::~GraphicsPipelineStateControllerSeparable() = default;

	bool GraphicsPipelineStateControllerSeparable::SetGraphicsPipelineStateObject(
			const GraphicsPipelineStateObject & pGraphicsPipelineStateObject )
	{
		// Execute the base set method. THis ensures the PSO handle is saved inside the state controller.
		bool updateResult = GraphicsPipelineStateController::SetGraphicsPipelineStateObject( pGraphicsPipelineStateObject );

		if( updateResult && _stateUpdateMask.is_set( eGraphicsStateUpdateFlagCommonPipelineStateObjectBit ) )
		{
			const auto * separableGraphicsPSO = pGraphicsPipelineStateObject.QueryInterface<GraphicsPipelineStateObjectSeparable>();

			// Update the individual descriptors. Returned mask is a combination of all eGraphicsStateUpdateFlagSeparableState*Bit
			// bits for states which has been updated. Zero means all states in the PSO match those currently bound.
			const auto stateUpdateMask = SetSeparableDescriptors( *separableGraphicsPSO );

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
			const GraphicsPipelineStateObjectSeparable & pSeparableGraphicsPSO )
	{
		auto stateUpdateMask = cppx::make_bitmask<uint64>( 0 );

		auto * blendStateDescriptor = pSeparableGraphicsPSO.mSeparableDescriptors.blendStateDescriptor.get();
		if( blendStateDescriptor != _separableDescriptorsCache.blendStateDescriptor )
		{
			_separableDescriptorsCache.blendStateDescriptor = blendStateDescriptor;
			stateUpdateMask.set( eGraphicsStateUpdateFlagSeparableStateBlendBit );
		}

		auto * depthStencilStateDescriptor = pSeparableGraphicsPSO.mSeparableDescriptors.depthStencilStateDescriptor.get();
		if( depthStencilStateDescriptor != _separableDescriptorsCache.depthStencilStateDescriptor )
		{
			_separableDescriptorsCache.depthStencilStateDescriptor = depthStencilStateDescriptor;
			stateUpdateMask.set( eGraphicsStateUpdateFlagSeparableStateDepthStencilBit );
		}

		auto * rasterizerStateDescriptor = pSeparableGraphicsPSO.mSeparableDescriptors.rasterizerStateDescriptor.get();
		if( rasterizerStateDescriptor != _separableDescriptorsCache.rasterizerStateDescriptor )
		{
			_separableDescriptorsCache.rasterizerStateDescriptor = rasterizerStateDescriptor;
			stateUpdateMask.set( eGraphicsStateUpdateFlagSeparableStateRasterizerBit );
		}

		auto * rootSignatureDescriptor = pSeparableGraphicsPSO.mSeparableDescriptors.rootSignatureDescriptor.get();
		if( rootSignatureDescriptor != _separableDescriptorsCache.rootSignatureDescriptor )
		{
			_separableDescriptorsCache.rootSignatureDescriptor = rootSignatureDescriptor;
			stateUpdateMask.set( eGraphicsStateUpdateFlagSeparableStateRootSignatureBit );
		}

		auto * shaderLinkageStateDescriptor = pSeparableGraphicsPSO.mSeparableDescriptors.shaderLinkageStateDescriptor.get();
		if( shaderLinkageStateDescriptor != _separableDescriptorsCache.shaderLinkageStateDescriptor )
		{
			_separableDescriptorsCache.shaderLinkageStateDescriptor = shaderLinkageStateDescriptor;
			stateUpdateMask.set( eGraphicsStateUpdateFlagSeparableStateShaderLinkageBit );
		}

		auto * vertexAttributeLayoutDescriptor = pSeparableGraphicsPSO.mSeparableDescriptors.vertexAttributeLayoutDescriptor.get();
		if( vertexAttributeLayoutDescriptor != _separableDescriptorsCache.vertexAttributeLayoutDescriptor )
		{
			_separableDescriptorsCache.vertexAttributeLayoutDescriptor = vertexAttributeLayoutDescriptor;
			stateUpdateMask.set( eGraphicsStateUpdateFlagSeparableStateVertexAttributeLayoutBit );
		}

		_stateUpdateMask.set( stateUpdateMask );

		return stateUpdateMask;
	}


	// GraphicsPipelineStateControllerSeparableShader

	GraphicsPipelineStateControllerSeparableShader::GraphicsPipelineStateControllerSeparableShader() = default;

	GraphicsPipelineStateControllerSeparableShader::~GraphicsPipelineStateControllerSeparableShader() = default;

	bool GraphicsPipelineStateControllerSeparableShader::SetGraphicsPipelineStateObject(
			const GraphicsPipelineStateObject & pGraphicsPipelineStateObject )
	{
		bool updateResult = GraphicsPipelineStateControllerSeparable::SetGraphicsPipelineStateObject( pGraphicsPipelineStateObject );

		if( updateResult && _stateUpdateMask.is_set( eGraphicsStateUpdateFlagSeparableStateShaderLinkageBit ) )
		{
			const auto * separableShaderGraphicsPSO = pGraphicsPipelineStateObject.QueryInterface<GraphicsPipelineStateObjectSeparableShader>();

			// Update the individual shaders. Returned mask is a combination of all eGraphicsStateUpdateFlagSeparableShader*Bit
			// bits for shaders which has been updated. Zero means all shaders in the PSO match those currently bound.
			const auto stateUpdateMask = SetSeparableShaders( *separableShaderGraphicsPSO );

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

	const SeparableShaderCache & GraphicsPipelineStateControllerSeparableShader::GetSeparableShaders() const noexcept
	{
		return _separableShadersCache;
	}

	cppx::bitmask<uint64> GraphicsPipelineStateControllerSeparableShader::SetSeparableShaders(
			const GraphicsPipelineStateObjectSeparableShader & pSeparableShaderGraphicsPSO )
	{
		auto stateUpdateMask = cppx::make_bitmask<uint64>( 0 );

		auto * vertexShader = pSeparableShaderGraphicsPSO.mBindingConfiguration[EShaderType::GSVertex];
		if( vertexShader != _separableShadersCache.vertexShader )
		{
			_separableShadersCache.vertexShader = vertexShader;
			stateUpdateMask.set( eGraphicsStateUpdateFlagSeparableShaderVertexBit );
		}

		auto * tessHullShader = pSeparableShaderGraphicsPSO.mBindingConfiguration[EShaderType::GSTessHull];
		if( tessHullShader != _separableShadersCache.tessHullShader )
		{
			_separableShadersCache.tessHullShader = tessHullShader;
			stateUpdateMask.set( eGraphicsStateUpdateFlagSeparableShaderHullBit );
		}

		auto * tessDomainShader = pSeparableShaderGraphicsPSO.mBindingConfiguration[EShaderType::GSTessDomain];
		if( tessDomainShader != _separableShadersCache.tessDomainShader )
		{
			_separableShadersCache.tessDomainShader = tessDomainShader;
			stateUpdateMask.set( eGraphicsStateUpdateFlagSeparableShaderDomainBit );
		}

		auto * geometryShader = pSeparableShaderGraphicsPSO.mBindingConfiguration[EShaderType::GSGeometry];
		if( geometryShader != _separableShadersCache.geometryShader )
		{
			_separableShadersCache.geometryShader = geometryShader;
			stateUpdateMask.set( eGraphicsStateUpdateFlagSeparableShaderGeometryBit );
		}

		auto * pixelShader = pSeparableShaderGraphicsPSO.mBindingConfiguration[EShaderType::GSPixel];
		if( pixelShader != _separableShadersCache.pixelShader )
		{
			_separableShadersCache.pixelShader = pixelShader;
			stateUpdateMask.set( eGraphicsStateUpdateFlagSeparableShaderPixelBit );
		}

		_stateUpdateMask.set( stateUpdateMask );

		return stateUpdateMask;
	}


	namespace GCU
	{

		GraphicsPipelineStateObjectDescriptorIDSet GetDescriptorsIDSetForSeparablePSO(
		const SeparableGraphicsPSDSet & pSeparableGraphicsDescriptors ) noexcept
		{
			GraphicsPipelineStateObjectDescriptorIDSet descriptorsIDSet{};
			descriptorsIDSet.descIDBlend = pSeparableGraphicsDescriptors.blendStateDescriptor->GetDescriptorID();
			descriptorsIDSet.descIDDepthStencil = pSeparableGraphicsDescriptors.depthStencilStateDescriptor->GetDescriptorID();
			descriptorsIDSet.descIDRasterizer = pSeparableGraphicsDescriptors.rasterizerStateDescriptor->GetDescriptorID();
			descriptorsIDSet.descIDRootSignature = pSeparableGraphicsDescriptors.rootSignatureDescriptor->GetDescriptorID();
			descriptorsIDSet.descIDShaderLinkage = pSeparableGraphicsDescriptors.shaderLinkageStateDescriptor->GetDescriptorID();
			descriptorsIDSet.descIDVertexAttributeLayout = pSeparableGraphicsDescriptors.vertexAttributeLayoutDescriptor->GetDescriptorID();
			return descriptorsIDSet;

		}

	} // namespace GCU

} // namespace Ic3::Graphics::GCI
