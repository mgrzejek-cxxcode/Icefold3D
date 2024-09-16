
#pragma once

#ifndef __IC3_CORELIB_CORE_DEFS_H__
#define __IC3_CORELIB_CORE_DEFS_H__

#include <memory>
#include <vector>

#define _ic3MakeStr2( pValue ) #pValue

#define ic3MakeStr( pValue ) _ic3MakeStr2( pValue )

#define ic3CaseReturn( pCaseValue, pReturnValue ) \
	case pCaseValue: return pReturnValue

#define ic3CaseReturnStr( pCaseValue ) \
	case pCaseValue: return #pCaseValue

#define ic3CaseDefaultBreak() \
	default: break

#define ic3CaseDefaultReturn( pReturnValue ) \
	default: return pReturnValue

namespace Ic3
{

	template <typename TPEnum>
	class EnumTypeInfo;

	template <typename TPResult, typename TPSrc>
	IC3_ATTR_NO_DISCARD inline TPResult dynamic_cast_dbg( TPSrc * pSource )
	{
	#if( IC3_DEBUG )
		auto * targetPtr = dynamic_cast<TPResult>( pSource );
		ic3DebugAssert( targetPtr );
	#else
		auto * targetPtr = static_cast<TPResult>( pSource );
	#endif
		return targetPtr;
	}

	template <typename TPResult, typename TPSrc>
	IC3_ATTR_NO_DISCARD inline TPResult dynamic_cast_check( TPSrc * pSource )
	{
		if( auto * targetPtr = dynamic_cast<TPResult>( pSource ) )
		{
			return targetPtr;
		}
		IC3_PCL_DEBUG_BREAK();
		return nullptr;
	}

	template <typename TPResult, typename TPSrc>
	IC3_ATTR_NO_DISCARD inline TPResult dynamic_cast_throw( TPSrc * pSource )
	{
		if( auto * targetPtr = dynamic_cast<TPResult>( pSource ) )
		{
			return targetPtr;
		}
		throw std::bad_cast();
	}

	template <typename TPResult, typename TPSrc>
	IC3_ATTR_NO_DISCARD inline std::shared_ptr<TPResult> dynamic_ptr_cast_check( std::shared_ptr<TPSrc> pSource )
	{
		if( auto targetPtr = std::dynamic_pointer_cast<TPResult>( std::move( pSource ) ) )
		{
			return targetPtr;
		}
		IC3_PCL_DEBUG_BREAK();
		return nullptr;
	}

	template <typename TPResult, typename TPSrc>
	IC3_ATTR_NO_DISCARD inline std::shared_ptr<TPResult> dynamic_ptr_cast_throw( std::shared_ptr<TPSrc> pSource )
	{
		if( auto targetPtr = std::dynamic_pointer_cast<TPResult>( std::move( pSource ) ) )
		{
			return targetPtr;
		}
		throw std::bad_cast();
	}

}

#endif // __IC3_CORELIB_CORE_DEFS_H__
