
#ifndef __IC3_NXMAIN_HARDWARE_BUFFER_H__
#define __IC3_NXMAIN_HARDWARE_BUFFER_H__

#include "GCRBufferCommon.h"
#include <Ic3/CoreLib/Signals/SignalEmitter.h>

namespace Ic3
{

	class IC3_NXMAIN_API GCRBuffer : public IDynamicObject
	{
	public:
		const GCI::GPUBufferReference & mGPUBufferRef;

		//const GCRBufferEventProxy & mEventProxy;

		GCRBuffer();

		virtual ~GCRBuffer();

		void Lock();
		void Unlock();

		void OnBufferLocked( GCRBuffer & pBuffer );

	private:
		GCI::GPUBufferReference _gpuBufferRef;
		//GCRBufferEventProxy _eventProxy;
	};

} // namespace Ic3

#endif // __IC3_NXMAIN_HARDWARE_BUFFER_H__
