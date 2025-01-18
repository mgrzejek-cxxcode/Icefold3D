
#include "SysObject.h"

namespace Ic3::System
{

	SysObject::SysObject( SysContextHandle pSysContext )
	: mSysContext( std::move( pSysContext ) )
	{}

	SysObject::~SysObject() = default;

	bool SysObject::DestroySystemObject()
	{
		if( SetDestroyRequestFlag() )
		{
			OnDestroySystemObjectRequested();

			SetStateFlags( true, eSysObjectStateFlagDestroyRequestProcessedBit );

			return true;
		}
		return false;
	}

	void SysObject::OnDestroySystemObjectRequested()
	{}

} // namespace Ic3::System
