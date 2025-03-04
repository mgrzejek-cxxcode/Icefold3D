
#pragma once

#ifndef __IC3_GRAPHICS_GCI_GPU_CAPABILITIES_H__
#define __IC3_GRAPHICS_GCI_GPU_CAPABILITIES_H__

#include "Prerequisites.h"

namespace Ic3::Graphics::GCI
{

	enum EGPUDeviceFeatureFlags : uint32
	{
		EGPUDeviceFeatureFlagMultiThreadedCommandExecutionBit     = 0x00000001,
		EGPUDeviceFeatureFlagMultiThreadedResourceCreationBit     = EGPUDeviceFeatureFlagMultiThreadedCommandExecutionBit << 1,
		EGPUDeviceFeatureFlagPipelineStateObjectBinaryCachingBit  = EGPUDeviceFeatureFlagMultiThreadedResourceCreationBit << 1,
		EGPUDeviceFeatureFlagResourceGPUMemorySparseBindingBit    = EGPUDeviceFeatureFlagPipelineStateObjectBinaryCachingBit << 1,
		EGPUDeviceFeatureFlagSamplerCustomBorderColorBit          = EGPUDeviceFeatureFlagResourceGPUMemorySparseBindingBit << 1,
		EGPUDeviceFeatureFlagShaderVertexBasedStageGeometryBit    = EGPUDeviceFeatureFlagSamplerCustomBorderColorBit << 1,
		EGPUDeviceFeatureFlagShaderVertexBasedStageTesselationBit = EGPUDeviceFeatureFlagShaderVertexBasedStageGeometryBit << 1,
		EGPUDeviceFeatureFlagShaderMeshBasedSupportBit            = EGPUDeviceFeatureFlagShaderVertexBasedStageTesselationBit << 1,
		EGPUDeviceFeatureFlagTextureAnisotropyFilterBit           = EGPUDeviceFeatureFlagShaderMeshBasedSupportBit << 1,
		EGPUDeviceFeatureFlagViewportArrayBit                     = EGPUDeviceFeatureFlagTextureAnisotropyFilterBit << 1,
	};

	enum class EGraphicsCapFeatureID : std::underlying_type_t<EGPUDeviceFeatureFlags>
	{
		MultiThreadedCommandExecution,
		MultiThreadedResourceCreation,
		PipelineStateObjectBinaryCaching,
		ResourceMemorySparseBinding,
		SamplerCustomBorderColor,
		ShaderVertexBasedStageGeometry,
		ShaderVertexBasedStageTesselation,
		ShaderMeshBasedSupport,
		TextureAnisotropyFilter,
		ViewportArray,
		Unknown
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_GPU_CAPABILITIES_H__
