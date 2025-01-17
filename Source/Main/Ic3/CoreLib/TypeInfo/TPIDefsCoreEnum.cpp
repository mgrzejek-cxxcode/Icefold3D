
#include <Ic3/CoreLib/Reflection/EnumTypeInfo.h>

namespace Ic3
{

	Ic3TypeInfoEnumDefine( EActiveState )
	{
		Ic3TypeInfoEnumBegin( EActiveState );
		Ic3TypeInfoEnumRegisterClassConstant( EActiveState::Disabled );
		Ic3TypeInfoEnumRegisterClassConstant( EActiveState::Enabled ) ;
		Ic3TypeInfoEnumRegisterClassConstant( EActiveState::Unknown  );
	}

	Ic3TypeInfoEnumDefine( EAccessModeFlags )
	{
		Ic3TypeInfoEnumBegin( EAccessModeFlags );
		Ic3TypeInfoEnumRegisterUnscopedConstant( eAccessModeFlagReadBit );
		Ic3TypeInfoEnumRegisterUnscopedConstant( eAccessModeFlagWriteBit ) ;
		Ic3TypeInfoEnumRegisterUnscopedConstant( eAccessModeFlagsFullAccess );
		Ic3TypeInfoEnumRegisterUnscopedConstant( eAccessModeFlagsNone );
	}

}
