
#pragma once

#ifndef __IC3_NXMAIN_EXCEPTION_H__
#define __IC3_NXMAIN_EXCEPTION_H__

#include "Prerequisites.h"
#include <Ic3/CoreLib/Exception.h>

namespace Ic3
{

	enum : exception_category_value_t
	{
		E_EXCEPTION_CATEGORY_ENGINE_SUBMODULE_MAIN =
			CXU::DeclareExceptionCategory( EExceptionBaseType::EngineSubmodule, Ic3ExcCategoryIID( 0x01 ) ),
	};

	class EngineMainSubModuleException : public EngineSubModuleException
	{
	public:
		explicit EngineMainSubModuleException( ExceptionInfo pExceptionInfo )
		: EngineSubModuleException( std::move( pExceptionInfo ) )
		{}
	};

	Ic3SetExceptionCategoryType( E_EXCEPTION_CATEGORY_ENGINE_SUBMODULE_MAIN, EngineMainSubModuleException );

	enum : exception_code_value_t
	{
		E_EXC_ESM_MAIN_SCF_ERROR =
			CXU::DeclareExceptionCode( eExceptionCategoryEngineSubmodule, Ic3ExcCodeIID( 0x71 ) ),
	};

} // namespace Ic3

#endif // __IC3_NXMAIN_EXCEPTION_H__
