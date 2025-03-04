
#pragma once

#ifndef __IC3_GRAPHICS_GCI_GPU_DEVICE_NULL_H__
#define __IC3_GRAPHICS_GCI_GPU_DEVICE_NULL_H__

#include "GPUDevice.h"

namespace Ic3::Graphics::GCI
{

	class GPUDeviceFeatureQueryNull : public GPUDeviceFeatureQuery
	{
	public:
		virtual MultiSamplingSettingsList EnumSupportedMultisamplingConfigs( ETextureFormat ) const noexcept override final
		{
			return {};
		}
	};

	class GPUDeviceNull : public GPUDevice
	{
	public:
		explicit GPUDeviceNull( GPUDriver & pDriver )
		: GPUDevice( pDriver, nullptr, nullptr )
		{}

		virtual ~GPUDeviceNull() = default;

		CPPX_ATTR_NO_DISCARD virtual bool IsNullDevice() const noexcept override final
		{
			return true;
		}

		virtual void WaitForCommandSync( CommandSync & ) override final
		{}

	protected:
		virtual bool OnGPUResourceActiveRefsZero( GPUResource & ) override final
		{
			return false;
		}

	private:
		virtual void InitializeCommandSystem() override final
		{}
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_GPU_DEVICE_NULL_H__
