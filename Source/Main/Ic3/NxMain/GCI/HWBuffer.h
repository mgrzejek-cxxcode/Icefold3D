
#ifndef __IC3_NXMAIN_HARDWARE_BUFFER_H__
#define __IC3_NXMAIN_HARDWARE_BUFFER_H__

#include "HWBufferCommon.h"
#include <Ic3/CoreLib/signals/signalEmitter.h>

namespace Ic3
{

	struct HWBufferEventProxy
	{
		template <typename TEvent>
		using EventEmitterType = EventEmitter<HWBuffer, TEvent>;

		using EvtLocked = Event<E_EVT_HWB_LOCKED>;
		using EvtUnlocked = Event<E_EVT_HWB_UNLOCKED>;

		EventEmitterType<EvtLocked> eLocked;
		EventEmitterType<EvtUnlocked> eUnlocked;

		explicit HWBufferEventProxy( HWBuffer & pHWBufferRef ) noexcept
		: eLocked( pHWBufferRef )
		, eUnlocked( pHWBufferRef )
		{}
	};

	class IC3_NXMAIN_API HWBuffer : public DynamicInterface
	{
	public:
		const GPUBufferRef & mGPUBufferRef;
		const HWBufferEventProxy & mEventProxy;

		HWBuffer();
		virtual ~HWBuffer();

	private:
		GPUBufferRef _gpuBufferRef;
		HWBufferEventProxy _eventProxy;
	};

} // namespace Ic3

#endif // __IC3_NXMAIN_HARDWARE_BUFFER_H__
