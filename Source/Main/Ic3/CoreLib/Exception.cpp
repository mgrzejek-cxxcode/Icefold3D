
#include "Exception.h"
#include <Ic3/CoreLib/Reflection/EnumTypeInfo.h>
#include <sstream>

namespace Ic3
{

    std::string ExceptionInfo::toString() const
	{
		std::ostringstream strStream;
		strStream << "[Exception]\n";
		strStream << "- Code: 0x" << std::hex << mCode << "\n";
		strStream << "- Desc: " << mDescription << "\n";
		strStream << "- Source: " << mFileLocationInfo.toString();

		return strStream.str();
	}

	ic3TypeInfoEnumDefine( EExceptionBaseType )
	{
		ic3TypeInfoEnumBegin( EExceptionBaseType );
		ic3TypeInfoEnumRegisterClassConstant( EExceptionBaseType::UNKNOWN );
		ic3TypeInfoEnumRegisterClassConstant( EExceptionBaseType::DEBUG ) ;
		ic3TypeInfoEnumRegisterClassConstant( EExceptionBaseType::ENGINE_SUBMODULE  );
		ic3TypeInfoEnumRegisterClassConstant( EExceptionBaseType::FRAMEWORK_CORE  );
		ic3TypeInfoEnumRegisterClassConstant( EExceptionBaseType::INTERNAL  );
		ic3TypeInfoEnumRegisterClassConstant( EExceptionBaseType::INTERRUPT  );
		ic3TypeInfoEnumRegisterClassConstant( EExceptionBaseType::MATH  );
		ic3TypeInfoEnumRegisterClassConstant( EExceptionBaseType::RESULT  );
		ic3TypeInfoEnumRegisterClassConstant( EExceptionBaseType::SYSTEM  );
		ic3TypeInfoEnumRegisterClassConstant( EExceptionBaseType::USER_EXTERNAL  );
	}

}
