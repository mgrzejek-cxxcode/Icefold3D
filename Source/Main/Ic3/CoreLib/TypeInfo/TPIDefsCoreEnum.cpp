
#include <Ic3/CoreLib/reflection/enumTypeInfo.h>

namespace Ic3
{

	ic3TypeInfoEnumDefine( EActiveState )
	{
		ic3TypeInfoEnumBegin( EActiveState );
		ic3TypeInfoEnumRegisterClassConstant( EActiveState::Disabled );
		ic3TypeInfoEnumRegisterClassConstant( EActiveState::Enabled ) ;
		ic3TypeInfoEnumRegisterClassConstant( EActiveState::Unknown  );
	}

	ic3TypeInfoEnumDefine( EAccessModeFlags )
	{
		ic3TypeInfoEnumBegin( EAccessModeFlags );
		ic3TypeInfoEnumRegisterUnscopedConstant( E_ACCESS_MODE_FLAG_READ_BIT );
		ic3TypeInfoEnumRegisterUnscopedConstant( E_ACCESS_MODE_FLAG_WRITE_BIT ) ;
		ic3TypeInfoEnumRegisterUnscopedConstant( E_ACCESS_MODE_FLAGS_FULL_ACCESS );
		ic3TypeInfoEnumRegisterUnscopedConstant( E_ACCESS_MODE_FLAGS_NONE );
	}

}
