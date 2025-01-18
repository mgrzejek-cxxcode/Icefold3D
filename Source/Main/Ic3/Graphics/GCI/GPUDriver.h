
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
		cppx::bitmask<EGPUDriverConfigFlags> configFlags = eGPUDriverConfigMaskDefault;
	};

	class IC3_GRAPHICS_GCI_CLASS GPUDriver : public IDynamicObject
	{
	public:
		System::SysContextHandle const mSysContext;

	public:
		explicit GPUDriver( System::SysContextHandle pSysContext ) noexcept;
		virtual ~GPUDriver() noexcept;

		CPPX_ATTR_NO_DISCARD virtual EGPUDriverID QueryGPUDriverID() const noexcept = 0;

		CPPX_ATTR_NO_DISCARD virtual bool IsNullDriver() const noexcept;

		cppx::bitmask<EGPUDriverConfigFlags> GetConfigFlags() const;

		bool IsDebugFunctionalityRequested() const;

		DisplayManagerHandle CreateDefaultDisplayManager();

		GPUDeviceHandle CreateDevice( const GPUDeviceCreateInfo & pCreateInfo );

		static GPUDriver & GetNullDriver();

	protected:
		void SetConfigFlags( cppx::bitmask<EGPUDriverConfigFlags> pConfigFlags );

	private:
		virtual DisplayManagerHandle _DrvCreateDefaultDisplayManager() = 0;

		virtual GPUDeviceHandle _DrvCreateDevice( const GPUDeviceCreateInfo & pCreateInfo ) = 0;


	private:
		cppx::bitmask<EGPUDriverConfigFlags> _configFlags;
	};

	inline cppx::bitmask<EGPUDriverConfigFlags> GPUDriver::GetConfigFlags() const
	{
		return _configFlags;
	}

	inline bool GPUDriver::IsDebugFunctionalityRequested() const
	{
		return _configFlags.is_set( eGPUDriverConfigFlagEnableDebugLayerBit );
	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_GPU_DRIVER_H__
