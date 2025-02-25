
#pragma once

#ifndef __IC3_GRAPHICS_COMMON_GRAPHICS_CORE_METRICS_H__
#define __IC3_GRAPHICS_COMMON_GRAPHICS_CORE_METRICS_H__

#include "GraphicsCoreDefs.h"

namespace Ic3::Graphics
{
	
	using gcm_value_t = native_uint;

	// Graphics Core Metrics
	namespace GCM
	{

		constexpr gcm_value_t kIAMaxVertexAttributesNum = 16;

		constexpr gcm_value_t kIAMaxVertexAttributeComponentsNum = 4;

		constexpr gcm_value_t kIAMaxVertexAttributeSemanticGroupSize = 4;

		constexpr gcm_value_t kIAMaxDataStreamVertexBuffersNum = 16;

		constexpr gcm_value_t kIAMaxDataStreamCombinedBuffersNum = kIAMaxDataStreamVertexBuffersNum + 1;

		/// Maximum number of Render Target color attachments.
		constexpr gcm_value_t kRTOMaxColorAttachmentsNum = 8;

		/// Maximum number of all Render Target attachments (size of the RT attachment array). This equals to the
		/// maximum number of color attachments +1 for depth-stencil attachment.
		constexpr gcm_value_t kRTOMaxCombinedAttachmentsNum = kRTOMaxColorAttachmentsNum + 1;

		constexpr gcm_value_t kSRSMaxConstantGroupSize = 32;

		constexpr gcm_value_t kSRSMaxDescriptorSetSize = 16;

		constexpr gcm_value_t kSRSMaxDescriptorSetsNum = 4;

		constexpr gcm_value_t kSRSMaxDwordSize = 64;

		constexpr gcm_value_t kShaderCombinedStagesNum = 6;

		constexpr gcm_value_t kShaderGraphicsStagesNum = 5;

		constexpr gcm_value_t kResMaxConstantBuffersNum = 16;

		constexpr gcm_value_t kResMaxTextureUnitsNum = 16;

		constexpr gcm_value_t kTextureMaxMipLevelsNum = 16;

	} // namespace GCM

} // namespace Ic3::Graphics

#endif // __IC3_GRAPHICS_COMMON_GRAPHICS_CORE_METRICS_H__
