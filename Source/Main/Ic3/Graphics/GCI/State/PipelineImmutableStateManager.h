
#pragma once

#ifndef __IC3_GRAPHICS_GCI_SHARED_IMMUTABLE_STATE_MANAGER_H__
#define __IC3_GRAPHICS_GCI_SHARED_IMMUTABLE_STATE_MANAGER_H__

#include "CommonGPUStateDefs.h"

namespace Ic3::Graphics::GCI
{

	using sm_separable_state_id_t = uint32;
	using sm_separable_state_combined_id_mask_t = uint64;

	using sm_blend_state_underlying_id_t = uint8;
	using sm_depth_stencil_state_underlying_id_t = uint8;
	using sm_input_layout_state_underlying_id_t = uint8;
	using sm_rasterizer_state_underlying_id_t = uint8;
	using sm_shader_binding_state_underlying_id_t = uint16;
	using sm_shader_input_signature_state_underlying_id_t = uint16;
	using sm_render_target_layout_state_underlying_id_t = uint8;

	struct PipelineStateObjectID
	{
		sm_separable_state_combined_id_mask_t immutableStateIDMask;
		sm_render_target_layout_state_underlying_id_t renderTargetLayoutID;
	};

	class PipelineSeparableStateCache
	{
	};

	class PipelineStateObjectCache
	{
	};

	class SharedImmutableStateManager
	{
	};

}

#endif // __IC3_GRAPHICS_GCI_SHARED_IMMUTABLE_STATE_MANAGER_H__
