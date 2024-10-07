
#ifndef __IC3_NXMAIN_HARDWARE_BUFFER_H__
#define __IC3_NXMAIN_HARDWARE_BUFFER_H__

#include "HardwareBufferCommon.h"
#include <Ic3/CoreLib/Signals/SignalEmitter.h>

namespace Ic3
{

	class HardwareBuffer;

	struct HardwareBufferEventProxy
	{
		template <typename TPEvent>
		using EventEmitterType = EventEmitter<HardwareBuffer, TPEvent>;

		using EvtStateChangeLocked = Event<eEvtCodeHardwareBufferStateChangeLocked>;
		using EvtStateChangeUnlocked = Event<eEvtCodeHardwareBufferStateChangeUnlocked>;
		using EvtTransferComplete = Event<eEvtCodeHardwareBufferStateChangeUnlocked, RHardwareBufferTransferUID>;

		EventEmitterType<EvtStateChangeLocked> evtStateChangeLocked;
		EventEmitterType<EvtStateChangeUnlocked> evtStateChangeUnlocked;
		EventEmitterType<EvtTransferComplete> evtTransferComplete;

		explicit HardwareBufferEventProxy( HardwareBuffer & pHardwareBuffer ) noexcept
		: evtStateChangeLocked( pHardwareBuffer )
		, evtStateChangeUnlocked( pHardwareBuffer )
		, evtTransferComplete( pHardwareBuffer )
		{}
	};

	class IC3_NXMAIN_API HardwareBuffer : public IDynamicObject
	{
	public:
		const GCI::GpuBufferReference & mGpuBufferRef;

		const HardwareBufferEventProxy & mEventProxy;

		HardwareBuffer()
		: mGpuBufferRef( _gpuBufferRef )
		, mEventProxy( _eventProxy )
		, _eventProxy( *this )
		{
			mEventProxy.evtStateChangeLocked.connect( this, &HardwareBuffer::onBufferLocked );
		}

		virtual ~HardwareBuffer();

		void lock();
		void unlock();

		void onBufferLocked( HardwareBuffer & pBuffer );

	private:
		GCI::GpuBufferReference _gpuBufferRef;
		HardwareBufferEventProxy _eventProxy;
	};

} // namespace Ic3

#endif // __IC3_NXMAIN_HARDWARE_BUFFER_H__
