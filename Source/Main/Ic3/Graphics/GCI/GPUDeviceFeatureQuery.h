
#pragma once

#ifndef __IC3_GRAPHICS_GCI_GPU_DEVICE_FEATURE_QUERY_H__
#define __IC3_GRAPHICS_GCI_GPU_DEVICE_FEATURE_QUERY_H__

#include "State/CommonGraphicsConfig.h"

namespace Ic3::Graphics::GCI
{

	using MultiSamplingSettingsList = std::vector<MultiSamplingSettings>;

	class IC3_GRAPHICS_GCI_CLASS GPUDeviceFeatureQuery
	{
	public:
		GPUDeviceFeatureQuery() = default;
		virtual ~GPUDeviceFeatureQuery() = default;

		/**
		 * 
		 * @param pFormat 
		 * @return 
		 */
		virtual MultiSamplingSettingsList EnumSupportedMultisamplingConfigs( ETextureFormat pFormat ) const noexcept = 0;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_GPU_DEVICE_FEATURE_QUERY_H__

