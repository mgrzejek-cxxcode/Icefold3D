
#pragma once

#ifndef __IC3_NXMAIN_CORE_ENGINE_STATE_H__
#define __IC3_NXMAIN_CORE_ENGINE_STATE_H__

#include <Ic3/System/SysContext.h>

namespace Ic3
{

	class CoreEngineState
	{
	public:
		System::SysContextHandle mSysContext;

		GCI::GPUDeviceHandle mGPUDevice;

	public:
		CoreEngineState( System::SysContextHandle pSysContext, GCI::GPUDeviceHandle pGPUDevice )
		: mSysContext( pSysContext )
		, mGPUDevice( pGPUDevice )
		{}
	};

} // namespace Ic3

#endif // __IC3_NXMAIN_CORE_ENGINE_STATE_H__
