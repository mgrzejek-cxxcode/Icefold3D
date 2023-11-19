
#pragma once

#ifndef __IC3_GRAPHICS_COMMON_GRAPHICS_CORE_METRICS_H__
#define __IC3_GRAPHICS_COMMON_GRAPHICS_CORE_METRICS_H__

#include <Ic3/Platform/Platform.h>

namespace Ic3::Graphics
{

	// Graphics Core Metrics
	namespace GCM
	{

		constexpr uint32 IA_MAX_VERTEX_ATTRIBUTES_NUM = 16;

		constexpr uint32 IA_MAX_VERTEX_ATTRIBUTE_COMPONENTS_NUM = 4;

		constexpr uint32 IA_MAX_VERTEX_BUFFER_BINDINGS_NUM = 16;

		constexpr uint32 IA_MAX_VERTEX_STREAMS_NUM = IA_MAX_VERTEX_BUFFER_BINDINGS_NUM;

		constexpr uint32 RT_MAX_COLOR_ATTACHMENTS_NUM = 8;

		constexpr uint32 RT_MAX_COMBINED_ATTACHMENTS_NUM = 9;

		constexpr uint32 IS_MAX_CONSTANT_GROUP_SIZE = 32;

		constexpr uint32 IS_MAX_DESCRIPTOR_SET_SIZE = 16;

		constexpr uint32 IS_MAX_DESCRIPTOR_SETS_NUM = 4;

		constexpr uint32 IS_MAX_DWORD_SIZE = 64;

		constexpr uint32 SHADER_COMBINED_STAGES_NUM = 6;

		constexpr uint32 SHADER_GRAPHICS_STAGES_NUM = 5;

		constexpr uint32 RES_MAX_CONSTANT_BUFFERS_NUM = 16;

		constexpr uint32 RES_MAX_TEXTURE_UNITS_NUM = 16;

		constexpr uint32 TEXTURE_MAX_MIP_LEVELS_NUM = 16;

	} // namespace GCM

} // namespace Ic3::Graphics

#endif // __IC3_GRAPHICS_COMMON_GRAPHICS_CORE_METRICS_H__
