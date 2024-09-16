
#pragma once

#ifndef __IC3_GRAPHICS_GCI_GPU_DRIVER_H__
#define __IC3_GRAPHICS_GCI_GPU_DRIVER_H__

#include "Prerequisites.h"
#include <Ic3/System/SysContext.h>

namespace Ic3::Graphics::GCI
{

	struct GPUDeviceCreateInfo;

	struct GPUDriverCreateInfo
	{
		System::SysContextHandle sysContext = nullptr;
		System::SysContextCreateInfo sysContextCreateInfo;
		TBitmask<EGPUDriverConfigFlags> configFlags = eGPUDriverConfigMaskDefault;
	};

	class IC3_GRAPHICS_GCI_CLASS GPUDriver : public IDynamicObject
	{
	public:
		System::SysContextHandle const mSysContext;

	public:
		explicit GPUDriver( System::SysContextHandle pSysContext ) noexcept;
		virtual ~GPUDriver() noexcept;

		IC3_ATTR_NO_DISCARD virtual EGPUDriverID queryGPUDriverID() const noexcept = 0;

		IC3_ATTR_NO_DISCARD virtual bool isNullDriver() const noexcept;

		TBitmask<EGPUDriverConfigFlags> getConfigFlags() const;

		bool isDebugFunctionalityRequested() const;

		DisplayManagerHandle createDefaultDisplayManager();

		GPUDeviceHandle createDevice( const GPUDeviceCreateInfo & pCreateInfo );

		static GPUDriver & nullDriver();

	protected:
		void setConfigFlags( TBitmask<EGPUDriverConfigFlags> pConfigFlags );

	private:
		virtual DisplayManagerHandle _drvCreateDefaultDisplayManager() = 0;

		virtual GPUDeviceHandle _drvCreateDevice( const GPUDeviceCreateInfo & pCreateInfo ) = 0;


	private:
		TBitmask<EGPUDriverConfigFlags> _configFlags;
	};

	inline TBitmask<EGPUDriverConfigFlags> GPUDriver::getConfigFlags() const
	{
		return _configFlags;
	}

	inline bool GPUDriver::isDebugFunctionalityRequested() const
	{
		return _configFlags.isSet( eGPUDriverConfigFlagEnableDebugLayerBit );
	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_GPU_DRIVER_H__
