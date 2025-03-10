
#include "IOStreamTypes.h"

namespace Ic3::System
{

#if( !IC3_BUILD_STATIC )
	template class PCL_ATTR_DLL_EXPORT IOReadStreamProxy<IOBaseStream>;
	template class PCL_ATTR_DLL_EXPORT IOWriteStreamProxy<IOReadOnlyStream>;
	template class PCL_ATTR_DLL_EXPORT IOWriteStreamProxy<IOBaseStream>;
#endif


	IOBaseStream::IOBaseStream(
		SysContextHandle pSysContext,
		const IOStreamProperties & pIOStreamProperties )
	: SysObject( std::move( pSysContext ) )
	, mAccessMode( pIOStreamProperties.accessMode )
	{}

	IOBaseStream::~IOBaseStream() = default;

} // namespace Ic3::System
