
#pragma once

#ifndef __IC3_GRAPHICS_GCI_GPU_DRIVER_H__
#define __IC3_GRAPHICS_GCI_GPU_DRIVER_H__

#include "Prerequisites.h"
#include <Ic3/System/SysContext.h>

namespace Ic3::Graphics::GCI
{

	struct GpuDeviceCreateInfo;

	struct GpuDriverCreateInfo
	{
		System::SysContextHandle sysContext = nullptr;
		System::SysContextCreateInfo sysContextCreateInfo;
		cppx::bitmask<EGpuDriverConfigFlags> configFlags = eGpuDriverConfigMaskDefault;
	};

	class IC3_GRAPHICS_GCI_CLASS GpuDriver : public IDynamicObject
	{
	public:
		System::SysContextHandle const mSysContext;

	public:
		explicit GpuDriver( System::SysContextHandle pSysContext ) noexcept;
		virtual ~GpuDriver() noexcept;

		CPPX_ATTR_NO_DISCARD virtual EGpuDriverID QueryGpuDriverID() const noexcept = 0;

		CPPX_ATTR_NO_DISCARD virtual bool IsNullDriver() const noexcept;

		cppx::bitmask<EGpuDriverConfigFlags> GetConfigFlags() const;

		bool IsDebugFunctionalityRequested() const;

		DisplayManagerHandle CreateDefaultDisplayManager();

		GpuDeviceHandle CreateDevice( const GpuDeviceCreateInfo & pCreateInfo );

		static GpuDriver & GetNullDriver();

	protected:
		void SetConfigFlags( cppx::bitmask<EGpuDriverConfigFlags> pConfigFlags );

	private:
		virtual DisplayManagerHandle _DrvCreateDefaultDisplayManager() = 0;

		virtual GpuDeviceHandle _DrvCreateDevice( const GpuDeviceCreateInfo & pCreateInfo ) = 0;


	private:
		cppx::bitmask<EGpuDriverConfigFlags> _configFlags;
	};

	inline cppx::bitmask<EGpuDriverConfigFlags> GpuDriver::GetConfigFlags() const
	{
		return _configFlags;
	}

	inline bool GpuDriver::IsDebugFunctionalityRequested() const
	{
		return _configFlags.is_set( eGpuDriverConfigFlagEnableDebugLayerBit );
	}

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_GPU_DRIVER_H__
