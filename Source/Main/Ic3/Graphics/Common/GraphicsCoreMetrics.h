
#pragma once

#ifndef __IC3_GRAPHICS_COMMON_GRAPHICS_CORE_METRICS_H__
#define __IC3_GRAPHICS_COMMON_GRAPHICS_CORE_METRICS_H__

#include <Ic3/Platform/Platform.h>

namespace Ic3::Graphics
{
	
	using gcm_value_t = uint32;

	// Graphics Core Metrics
	namespace GCM
	{

		constexpr gcm_value_t cxIAMaxVertexAttributesNum = 16;

		constexpr gcm_value_t cxIAMaxVertexAttributeComponentsNum = 4;

		constexpr gcm_value_t cxIAMaxVertexBufferBindingsNum = 16;

		constexpr gcm_value_t cxIAMaxVertexStreamsNum = cxIAMaxVertexBufferBindingsNum;

		constexpr gcm_value_t cxRTMaxColorAttachmentsNum = 8;

		constexpr gcm_value_t cxRTMaxCombinedAttachmentsNum = 9;

		constexpr gcm_value_t cxISMaxConstantGroupSize = 32;

		constexpr gcm_value_t cxISMaxDescriptorSetSize = 16;

		constexpr gcm_value_t cxISMaxDescriptorSetsNum = 4;

		constexpr gcm_value_t cxISMaxDwordSize = 64;

		constexpr gcm_value_t cxShaderCombinedStagesNum = 6;

		constexpr gcm_value_t cxShaderGraphicsStagesNum = 5;

		constexpr gcm_value_t cxResMaxConstantBuffersNum = 16;

		constexpr gcm_value_t cxResMaxTextureUnitsNum = 16;

		constexpr gcm_value_t cxTextureMaxMipLevelsNum = 16;

	} // namespace GCM

} // namespace Ic3::Graphics

#endif // __IC3_GRAPHICS_COMMON_GRAPHICS_CORE_METRICS_H__
