
#include "MTLGPUDriver.h"

namespace Ic3::Graphics::GCI
{

	System::MetalSystemDriverHandle MetalGPUDriver::initializeSysMTLDriver( System::SysContextHandle pSysContext )
	{
		try
		{
			auto sysDisplayManager = pSysContext->CreateDisplayManager();
			auto sysMetalDriver = pSysContext->createMetalSystemDriver( sysDisplayManager, {} );

			sysMetalDriver->InitializeDefaultDevice();

			return sysMetalDriver;
		}
		catch ( ... )
		{}

		return nullptr;
	}

}
