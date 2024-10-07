
#include "HardwareBuffer.h"

namespace Ic3
{

	HardwareBuffer::HardwareBuffer()
	: mGpuBufferRef( _gpuBufferRef )
	, mEventProxy( _eventProxy )
	, _eventProxy( *this )
	{}

	HardwareBuffer::~HardwareBuffer() = default;

} // namespace Ic3
