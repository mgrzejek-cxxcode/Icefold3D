
#include "Frame.h"

namespace Ic3::System
{

	Frame::Frame( SysContextHandle pSysContext )
	: EventSource( std::move( pSysContext ) )
	{}

	Frame::~Frame() noexcept = default;

} // namespace Ic3::System
