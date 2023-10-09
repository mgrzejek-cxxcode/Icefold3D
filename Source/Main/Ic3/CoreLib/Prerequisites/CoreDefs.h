
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

	template <typename TEnum>
	class EnumTypeInfo;

	template <typename TValue, typename TAllocator = std::allocator<TValue>>
	using Array = std::vector<TValue, TAllocator>;

	template <typename TResult, typename TSrc>
	IC3_ATTR_NO_DISCARD inline TResult dynamic_cast_dbg( TSrc * pSource )
	{
	#if( IC3_DEBUG )
		auto * targetPtr = dynamic_cast<TResult>( pSource );
		ic3DebugAssert( targetPtr );
	#else
		auto * targetPtr = static_cast<TResult>( pSource );
	#endif
		return targetPtr;
	}

	template <typename TResult, typename TSrc>
	IC3_ATTR_NO_DISCARD inline TResult dynamic_cast_check( TSrc * pSource )
	{
		if( auto * targetPtr = dynamic_cast<TResult>( pSource ) )
		{
			return targetPtr;
		}
		IC3_PCL_DEBUG_BREAK();
		return nullptr;
	}

	template <typename TResult, typename TSrc>
	IC3_ATTR_NO_DISCARD inline TResult dynamic_cast_throw( TSrc * pSource )
	{
		if( auto * targetPtr = dynamic_cast<TResult>( pSource ) )
		{
			return targetPtr;
		}
		throw std::bad_cast();
	}

	template <typename TResult, typename TSrc>
	IC3_ATTR_NO_DISCARD inline std::shared_ptr<TResult> dynamic_ptr_cast_check( std::shared_ptr<TSrc> pSource )
	{
		if( auto targetPtr = std::dynamic_pointer_cast<TResult>( std::move( pSource ) ) )
		{
			return targetPtr;
		}
		IC3_PCL_DEBUG_BREAK();
		return nullptr;
	}

	template <typename TResult, typename TSrc>
	IC3_ATTR_NO_DISCARD inline std::shared_ptr<TResult> dynamic_ptr_cast_throw( std::shared_ptr<TSrc> pSource )
	{
		if( auto targetPtr = std::dynamic_pointer_cast<TResult>( std::move( pSource ) ) )
		{
			return targetPtr;
		}
		throw std::bad_cast();
	}

}

#endif // __IC3_CORELIB_CORE_DEFS_H__
