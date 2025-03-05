
#include "GCRBuffer.h"

namespace Ic3
{

	GCRBuffer::GCRBuffer()
	: mGPUBufferRef( _gpuBufferRef )
	//, mEventProxy( _eventProxy )
	//, _eventProxy( *this )
	{
		//mEventProxy.evtStateChangeLocked.Connect( this, &GCRBuffer::OnBufferLocked );
	}

	GCRBuffer::~GCRBuffer() = default;

} // namespace Ic3
