
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
        // Exception codes prefix: eExcDebug
        Debug,

        // Dedicated exception type for top-level engine components (graphics, scripting, etc).
        // Usually each component provides its own base class which derives from EngineSubModuleException class.
        // Exception codes prefix: eExcSm%SubmoduleName%
        EngineSubmodule,

        // For all normal, error-like exceptions, intended to be used by lower-level components of the engine
        // (hence the 'framework' keyword). Defined primarily in the common ::Core component of the project.
        // Exception codes prefix: eExcCore
        FrameworkCore,

        // Internal, implementation-details exception used by the framework. Used primarily for signalling errors
        // between parts of the framework, without exposing them to the user. Those exceptions should never leave
        // the "implementation detail" level of the project.
        // Exception codes prefix: eExcInternal
        Internal,

        // For exceptions used as an interrupts (for example in thread proc).
        // Exception codes prefix: eExcInterrupt
        Interrupt,

        // Defined primarily within the ::Math component.
        // Exception codes prefix: eExcMath
        Math,

        // Dedicated type for exceptions carrying ResultInfo as an error indicator.
        // Exception codes prefix: eExcResult
        Result,

        // System-specific exceptions, extended in platform-specific manner. Defined within the ::System component.
        // Exception codes prefix: eExcSystem
        System,

        // All exceptions defined by the client libraries and frameworks.
        // Exception codes prefix: eExcUserExt
        UserExternal,

        // Reserved value, used to count all enumerators. Must always be defined as the last one.
        Reserved
    };
	Ic3TypeInfoEnumDeclareCoreLib( EExceptionBaseType );

	namespace CXU
	{

		/// Number of total enum values. Used for validation.
		inline constexpr auto kEnumExceptionBaseTypeCount = static_cast<exception_base_type_value_t>( EExceptionBaseType::Reserved );

		/// Exception code: control key for validation.
		inline constexpr auto kVbmExceptionCodeControlKey = static_cast<exception_code_value_t>( 0xE7000000 );

		/// Exception code: mask for type component (EExceptionBaseType, 8-bit integer).
		inline constexpr auto kVbmExceptionCodeBaseTypeMask = static_cast<exception_code_value_t>( 0x00FF0000 );

		/// Exception code: mask for category component (16-bit integer). Includes base type.
		inline constexpr auto kVbmExceptionCodeCategoryMask = static_cast<exception_code_value_t>( 0x00FFFF00 );

		/// Exception code: mask for IID (internal ID) component (8-bit integer).
		inline constexpr auto kVbmExceptionCodeIIDMask = static_cast<exception_code_value_t>( 0x000000FF );

		/// Exception category: mask for base type component (8-bit integer).
		inline constexpr auto kVbmExceptionCategoryBaseTypeMask = static_cast<exception_category_value_t>( 0xFF00 );

	    /// @brief Checks whether the specified value is a valid EExceptionBaseType.
	    /// Used primarily by the library in the template-based mappings.
	    inline constexpr bool IsExceptionBaseTypeValid( EExceptionBaseType pBaseType )
	    {
			using value_type = exception_base_type_value_t;
		    return ( ( value_type )( pBaseType ) > 0u ) && ( ( value_type )( pBaseType ) < kEnumExceptionBaseTypeCount );
	    }

		/// @brief
	    inline constexpr exception_category_value_t DeclareExceptionCategory( EExceptionBaseType pBaseType, uint8 pCategoryIID )
	    {
		    return ( ( ( exception_base_type_value_t )( pBaseType ) << 8 ) | pCategoryIID );
	    }

		/// @brief
	    inline constexpr exception_code_value_t DeclareExceptionCode( exception_category_value_t pCategory, uint8 pCodeIID )
	    {
		    return ( kVbmExceptionCodeControlKey | ( ( exception_code_value_t )( pCategory ) << 8 ) | pCodeIID );
	    }

		/// @brief
	    inline constexpr EExceptionBaseType GetExceptionCategoryBaseType( exception_category_value_t pCategory )
	    {
		    return ( EExceptionBaseType )( ( pCategory & 0xFF00 ) >> 8 );
	    }

		/// @brief
	    inline constexpr EExceptionBaseType GetExceptionCodeBaseType( exception_code_value_t pCode )
	    {
		    return ( EExceptionBaseType )( ( pCode & kVbmExceptionCodeBaseTypeMask ) >> 16 );
	    }

		/// @brief
	    inline constexpr exception_category_value_t GetExceptionCodeCategory( exception_code_value_t pCode )
	    {
		    return ( exception_category_value_t )( ( pCode & kVbmExceptionCodeCategoryMask ) >> 8 );
	    }

		/// @brief
	    inline constexpr bool ValidateExceptionCode( exception_code_value_t pCode )
	    {
		    return ( pCode & kVbmExceptionCodeControlKey ) == kVbmExceptionCodeControlKey;
	    }

	}

} // namespace Ic3

#endif // __IC3_CORELIB_EXCEPTION_COMMON_H__
