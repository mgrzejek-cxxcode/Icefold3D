
#include "MTLGPUDriver.h"

namespace Ic3::Graphics::GCI
{

	System::MetalSystemDriverHandle MetalGPUDriver::initializeSysMTLDriver( System::SysContextHandle pSysContext )
	{
		try
		{
			auto sysDisplayManager = pSysContext->createDisplayManager();
			auto sysMetalDriver = pSysContext->createMetalSystemDriver( sysDisplayManager, {} );

			sysMetalDriver->initializeDefaultDevice();

			return sysMetalDriver;
		}
		catch ( ... )
		{}

		return nullptr;
	}

}
