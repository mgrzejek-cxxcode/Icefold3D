
#include "HWBuffer.h"

namespace Ic3
{

	HWBuffer::HWBuffer()
	: mGPUBufferRef( _gpuBufferRef )
	, mEventProxy( _eventProxy )
	, _eventProxy( *this )
	{}

	HWBuffer::~HWBuffer() = default;

} // namespace Ic3
