
#ifndef __IC3_NXMAIN_GCR_RESOURCE_H__
#define __IC3_NXMAIN_GCR_RESOURCE_H__

#include "CommonGCIDefs.h"
#include <Ic3/CoreLib/Signals/SignalEmitter.h>

namespace Ic3
{

	Ic3DeclareClassHandle( GCRResource );
	Ic3DeclareClassHandle( GCRBuffer );
	Ic3DeclareClassHandle( GCRTexture );

	using RGCRBufferTransferUID = uint64;

	enum EGCRBufferEventCode : uint32 // event_code_value_t
	{
		eEvtCodeGCRResourceBase = 0x7F,
		eEvtCodeGCRResourceGPURefUpdated,
		eEvtCodeGCRResourceStateChangeLocked,
		eEvtCodeGCRResourceStateChangeUnlocked,
		eEvtCodeGCRResourceTransferBegin,
		eEvtCodeGCRResourceTransferEnd,
	};

	struct GCRResourceEventProxy
	{
		template <typename TPEvent>
		using EventEmitterType = EventEmitter<GCRResource, TPEvent>;

		using EvtGPURefUpdated = Event<eEvtCodeGCRResourceGPURefUpdated>;
		using EvtStateChangeLocked = Event<eEvtCodeGCRResourceStateChangeLocked>;
		using EvtStateChangeUnlocked = Event<eEvtCodeGCRResourceStateChangeUnlocked>;
		using EvtTransferBegin = Event<eEvtCodeGCRResourceTransferBegin, RGCRBufferTransferUID>;
		using EvtTransferEnd = Event<eEvtCodeGCRResourceTransferEnd, RGCRBufferTransferUID>;

		EventEmitterType<EvtGPURefUpdated> evtGPURefUpdated;
		EventEmitterType<EvtStateChangeLocked> evtStateChangeLocked;
		EventEmitterType<EvtStateChangeUnlocked> evtStateChangeUnlocked;
		EventEmitterType<EvtTransferBegin> evtTransferBegin;
		EventEmitterType<EvtTransferEnd> evtTransferEnd;

		explicit GCRResourceEventProxy( GCRResource & pGCRResource ) noexcept
		: evtGPURefUpdated( pGCRResource )
		, evtStateChangeLocked( pGCRResource )
		, evtStateChangeUnlocked( pGCRResource )
		, evtTransferBegin( pGCRResource )
		, evtTransferEnd( pGCRResource )
		{}
	};

	class IC3_NXMAIN_API GCRResource : public IDynamicObject
	{
	};

}

#endif // __IC3_NXMAIN_GCR_RESOURCE_H__
