
#pragma once

#ifndef __IC3_GRAPHICS_GCI_GPU_CAPABILITIES_H__
#define __IC3_GRAPHICS_GCI_GPU_CAPABILITIES_H__

#include "Prerequisites.h"

namespace Ic3::Graphics::GCI
{

	enum EGPUDeviceFeatureFlags : uint32
	{
		E_GPU_DEVICE_FEATURE_FLAG_MULTI_THREADED_COMMAND_EXECUTIONBit      = 1 << 0,
		E_GPU_DEVICE_FEATURE_FLAG_MULTI_THREADED_RESOURCE_CREATIONBit      = 1 << 1,
		E_GPU_DEVICE_FEATURE_FLAG_RESOURCE_GPU_MEMORY_SPARSE_BINDINGBit    = 1 << 2,
		E_GPU_DEVICE_FEATURE_FLAG_SAMPLER_CUSTOM_BORDER_COLORBit           = 1 << 3,
		E_GPU_DEVICE_FEATURE_FLAG_SHADER_VERTEX_BASED_STAGE_GEOMETRYBit    = 1 << 6,
		E_GPU_DEVICE_FEATURE_FLAG_SHADER_VERTEX_BASED_STAGE_TESSELATIONBit = 1 << 7,
		E_GPU_DEVICE_FEATURE_FLAG_SHADER_MESH_BASED_SUPPORTBit             = 1 << 8,
		E_GPU_DEVICE_FEATURE_FLAG_TEXTURE_ANISOTROPY_FILTERBit             = 1 << 4,
		E_GPU_DEVICE_FEATURE_FLAG_VIEWPORT_ARRAYBit                        = 1 << 5,
	};

	enum class EGraphicsCapFeatureID : std::underlying_type_t<EGPUDeviceFeatureFlags>
	{
		MultiThreadedCommandExecution,
		MultiThreadedResourceCreation,
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
