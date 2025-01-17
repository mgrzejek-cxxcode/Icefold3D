
#include "MTLGpuDriver.h"

namespace Ic3::Graphics::GCI
{

	System::MetalSystemDriverHandle MetalGpuDriver::initializeSysMTLDriver( System::SysContextHandle pSysContext )
	{
		try
		{
			auto sysDisplayManager = pSysContext->CreateDisplayManager();
			auto sysMetalDriver = pSysContext->createMetalSystemDriver( sysDisplayManager, {} );

			sysMetalDriver->initializeDefaultDevice();

			return sysMetalDriver;
		}
		catch ( ... )
		{}

		return nullptr;
	}

}
