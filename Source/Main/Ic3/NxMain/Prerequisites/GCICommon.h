
#ifndef __IC3_NXMAIN_GCI_COMMON_H__
#define __IC3_NXMAIN_GCI_COMMON_H__

#include <Ic3/Graphics/GCI/Memory/commonGPUMemoryDefs.h>

namespace Ic3
{

	class GpaContext
	{
	public:
		GCI::GPUDevice * const mGPUDevice = nullptr;

	public:
		GpaContext( GCI::GPUDevice & pGPUDevice )
		: mGPUDevice( &pGPUDevice )
		{}
	};

}

#endif // __IC3_NXMAIN_GCI_COMMON_H__
