
#include "IOStreamTypes.h"

namespace Ic3::System
{

	IOBaseStream::IOBaseStream(
		SysContextHandle pSysContext,
		const IOStreamProperties & pIOStreamProperties )
	: SysObject( std::move( pSysContext ) )
	, mAccessMode( pIOStreamProperties.accessMode )
	{}

	IOBaseStream::~IOBaseStream() = default;

} // namespace Ic3::System
