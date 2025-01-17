
#pragma once

#ifndef __IC3_GRAPHICS_GCI_GPU_CAPABILITIES_H__
#define __IC3_GRAPHICS_GCI_GPU_CAPABILITIES_H__

#include "Prerequisites.h"

namespace Ic3::Graphics::GCI
{

	enum EGpuDeviceFeatureFlags : uint32
	{
		E_GPU_DEVICE_FEATURE_FLAG_MULTI_THREADED_COMMAND_EXECUTION_BIT      = 1 << 0,
		E_GPU_DEVICE_FEATURE_FLAG_MULTI_THREADED_RESOURCE_CREATION_BIT      = 1 << 1,
		E_GPU_DEVICE_FEATURE_FLAG_RESOURCE_GPU_MEMORY_SPARSE_BINDING_BIT    = 1 << 2,
		E_GPU_DEVICE_FEATURE_FLAG_SAMPLER_CUSTOM_BORDER_COLOR_BIT           = 1 << 3,
		E_GPU_DEVICE_FEATURE_FLAG_SHADER_VERTEX_BASED_STAGE_GEOMETRY_BIT    = 1 << 6,
		E_GPU_DEVICE_FEATURE_FLAG_SHADER_VERTEX_BASED_STAGE_TESSELATION_BIT = 1 << 7,
		E_GPU_DEVICE_FEATURE_FLAG_SHADER_MESH_BASED_SUPPORT_BIT             = 1 << 8,
		E_GPU_DEVICE_FEATURE_FLAG_TEXTURE_ANISOTROPY_FILTER_BIT             = 1 << 4,
		E_GPU_DEVICE_FEATURE_FLAG_VIEWPORT_ARRAY_BIT                        = 1 << 5,
	};

	enum class EGraphicsCapFeatureID : std::underlying_type_t<EGpuDeviceFeatureFlags>
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
