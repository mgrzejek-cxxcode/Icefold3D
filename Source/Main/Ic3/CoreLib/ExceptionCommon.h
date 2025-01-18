
#pragma once

#ifndef __IC3_CORELIB_EXCEPTION_COMMON_H__
#define __IC3_CORELIB_EXCEPTION_COMMON_H__

#include "Prerequisites.h"

#define Ic3ExcCategoryIID( pIID ) ( pIID )
#define Ic3ExcCodeIID( pIID ) ( pIID )

namespace Ic3
{

    /// @brief Type used to represent exception codes.
    using exception_base_type_value_t = uint8;

    /// @brief Type used to represent exception categories.
    using exception_category_value_t = uint16;

    /// @brief Type used to represent exception codes.
    using exception_code_value_t = uint32;

    /// @brief Enum with possible base types for exception objects.
    ///
    /// EExceptionBaseType unambiguously identifies one of supported exception types defined in the framework.
    /// Base types are defined/provided by the library and there is no possibility of adding a custom one.
    /// Use a custom category (subtype) of an 'External' type for extending the system.
    enum class EExceptionBaseType : exception_base_type_value_t
    {
        // Unknown type. Not used directly by any exception class.
        Unknown,

        // For debug-specific errors and messages, debug-only assertions etc.
        // Exception codes prefix: E_EXC_DEBUG
        Debug,

        // Dedicated exception type for top-level engine components (graphics, scripting, etc).
        // Usually each component provides its own base class which derives from EngineSubModuleException class.
        // Exception codes prefix: E_EXC_ESM_%SubmoduleName%
        EngineSubmodule,

        // For all normal, error-like exceptions, intended to be used by lower-level components of the engine
        // (hence the 'framework' keyword). Defined primarily in the common ::Core component of the project.
        // Exception codes prefix: E_EXC_CORE
        FrameworkCore,

        // Internal, implementation-details exception used by the framework. Used primarily for signalling errors
        // between parts of the framework, without exposing them to the user. Those exceptions should never leave
        // the "implementation detail" level of the project.
        // Exception codes prefix: E_EXC_INTERNAL
        Internal,

        // For exceptions used as an interrupts (for example in thread proc).
        // Exception codes prefix: E_EXC_INTERRUPT
        Interrupt,

        // Defined primarily within the ::Math component.
        // Exception codes prefix: E_EXC_MATH
        Math,

        // Dedicated type for exceptions carrying ResultInfo as an error indicator.
        // Exception codes prefix: E_EXC_RESULT
        Result,

        // System-specific exceptions, extended in platform-specific manner. Defined within the ::System component.
        // Exception codes prefix: E_EXC_SYSTEM
        System,

        // All exceptions defined by the client libraries and frameworks.
        // Exception codes prefix: E_EXC_USREXT
        UserExternal,

        // Reserved value, used to count all enumerators. Must always be defined as the last one.
        Reserved
    };

	Ic3TypeInfoEnumDeclare( EExceptionBaseType );

	namespace CxDef
	{

		/// Number of total enum values. Used for validation.
		constexpr auto ENUM_EXCEPTION_BASE_TYPE_COUNT = static_cast<exception_base_type_value_t>( EExceptionBaseType::Reserved );

		/// Exception code: control key for validation.
		constexpr auto VBM_EXCEPTION_CODE_CONTROL_KEY = static_cast<exception_code_value_t>( 0xE7000000u );

		/// Exception code: mask for type component (EExceptionBaseType, 8-bit integer).
		constexpr auto VBM_EXCEPTION_CODE_BASE_TYPE_MASK = static_cast<exception_code_value_t>( 0x00FF0000u );

		/// Exception code: mask for category component (16-bit integer). Includes base type.
		constexpr auto VBM_EXCEPTION_CODE_CATEGORY_MASK = static_cast<exception_code_value_t>( 0x00FFFF00u );

		/// Exception code: mask for IID (internal ID) component (8-bit integer).
		constexpr auto VBM_EXCEPTION_CODE_IID_MASK = static_cast<exception_code_value_t>( 0x000000FFu );

		/// Exception category: mask for base type component (8-bit integer).
		constexpr auto VBM_EXCEPTION_CATEGORY_BASE_TYPE_MASK = static_cast<exception_category_value_t>( 0xFF00u );

	    /// @brief Checks whether the specified value is a valid EExceptionBaseType.
	    /// Used primarily by the library in the template-based mappings.
	    inline constexpr bool isExceptionBaseTypeValid( EExceptionBaseType pBaseType )
	    {
			using value_type = exception_base_type_value_t;
		    return ( ( value_type )( pBaseType ) > 0u ) && ( ( value_type )( pBaseType ) < ENUM_EXCEPTION_BASE_TYPE_COUNT );
	    }

		/// @brief
	    inline constexpr exception_category_value_t declareExceptionCategory( EExceptionBaseType pBaseType, uint8 pCategoryIID )
	    {
		    return ( ( ( exception_base_type_value_t )( pBaseType ) << 8 ) | pCategoryIID );
	    }

		/// @brief
	    inline constexpr exception_code_value_t declareExceptionCode( exception_category_value_t pCategory, uint8 pCodeIID )
	    {
		    return ( VBM_EXCEPTION_CODE_CONTROL_KEY | ( ( exception_code_value_t )( pCategory ) << 8 ) | pCodeIID );
	    }

		/// @brief
	    inline constexpr EExceptionBaseType getExceptionCategoryBaseType( exception_category_value_t pCategory )
	    {
		    return ( EExceptionBaseType )( ( pCategory & 0xFF00 ) >> 8 );
	    }

		/// @brief
	    inline constexpr EExceptionBaseType getExceptionCodeBaseType( exception_code_value_t pCode )
	    {
		    return ( EExceptionBaseType )( ( pCode & VBM_EXCEPTION_CODE_BASE_TYPE_MASK ) >> 16 );
	    }

		/// @brief
	    inline constexpr exception_category_value_t getExceptionCodeCategory( exception_code_value_t pCode )
	    {
		    return ( exception_category_value_t )( ( pCode & VBM_EXCEPTION_CODE_CATEGORY_MASK ) >> 8 );
	    }

		/// @brief
	    inline constexpr bool validateExceptionCode( exception_code_value_t pCode )
	    {
		    return ( pCode & VBM_EXCEPTION_CODE_CONTROL_KEY ) == VBM_EXCEPTION_CODE_CONTROL_KEY;
	    }

	}

} // namespace Ic3

#endif // __IC3_CORELIB_EXCEPTION_COMMON_H__
