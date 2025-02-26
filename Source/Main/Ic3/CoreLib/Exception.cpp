
#include "Exception.h"
#include <Ic3/CoreLib/Reflection/EnumTypeInfo.h>
#include <sstream>

namespace Ic3
{

    std::string ExceptionInfo::ToString() const
	{
		std::ostringstream strStream;
		strStream << "[Exception]\n";
		strStream << "- Code: 0x" << std::hex << code << "\n";
		strStream << "- Desc: " << description << "\n";
		strStream << "- Source: " << fileLocationInfo.to_string();

		return strStream.str();
	}

	Ic3TypeInfoEnumDefine( EExceptionBaseType )
	{
		Ic3TypeInfoEnumBegin( EExceptionBaseType );
		Ic3TypeInfoEnumRegisterClassConstant( EExceptionBaseType::Unknown );
		Ic3TypeInfoEnumRegisterClassConstant( EExceptionBaseType::Debug ) ;
		Ic3TypeInfoEnumRegisterClassConstant( EExceptionBaseType::EngineSubmodule  );
		Ic3TypeInfoEnumRegisterClassConstant( EExceptionBaseType::FrameworkCore  );
		Ic3TypeInfoEnumRegisterClassConstant( EExceptionBaseType::Internal  );
		Ic3TypeInfoEnumRegisterClassConstant( EExceptionBaseType::Interrupt  );
		Ic3TypeInfoEnumRegisterClassConstant( EExceptionBaseType::Math  );
		Ic3TypeInfoEnumRegisterClassConstant( EExceptionBaseType::Result  );
		Ic3TypeInfoEnumRegisterClassConstant( EExceptionBaseType::System  );
		Ic3TypeInfoEnumRegisterClassConstant( EExceptionBaseType::UserExternal  );
	}

}
