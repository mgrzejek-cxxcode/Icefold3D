
#include <Ic3/CoreLib/Reflection/EnumTypeInfo.h>

namespace Ic3
{

	ic3TypeInfoEnumDefine( EActiveState )
	{
		ic3TypeInfoEnumBegin( EActiveState );
		ic3TypeInfoEnumRegisterClassConstant( EActiveState::DISABLED );
		ic3TypeInfoEnumRegisterClassConstant( EActiveState::ENABLED ) ;
		ic3TypeInfoEnumRegisterClassConstant( EActiveState::UNKNOWN  );
	}

	ic3TypeInfoEnumDefine( EAccessModeFlags )
	{
		ic3TypeInfoEnumBegin( EAccessModeFlags );
		ic3TypeInfoEnumRegisterUnscopedConstant( E_ACCESS_MODE_FLAG_READBit );
		ic3TypeInfoEnumRegisterUnscopedConstant( E_ACCESS_MODE_FLAG_WRITEBit ) ;
		ic3TypeInfoEnumRegisterUnscopedConstant( E_ACCESS_MODE_FLAGS_FULL_ACCESS );
		ic3TypeInfoEnumRegisterUnscopedConstant( E_ACCESS_MODE_FLAGS_NONE );
	}

}
