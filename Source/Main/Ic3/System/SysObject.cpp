
#include "SysObject.h"

namespace Ic3::System
{

	SysObject::SysObject( SysContextHandle pSysContext )
	: mSysContext( std::move( pSysContext ) )
	{}

	SysObject::~SysObject() = default;

	bool SysObject::destroySystemObject()
	{
		if( setDestroyRequestFlag() )
		{
			onDestroySystemObjectRequested();

			setStateFlags( true, eSysObjectStateFlagDestroyRequestProcessedBit );

			return true;
		}
		return false;
	}

	void SysObject::onDestroySystemObjectRequested()
	{}

} // namespace Ic3::System
