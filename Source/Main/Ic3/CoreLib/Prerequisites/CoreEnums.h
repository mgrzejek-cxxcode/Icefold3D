
#pragma once

#ifndef __IC3_CORELIB_CORE_ENUMS_H__
#define __IC3_CORELIB_CORE_ENUMS_H__

namespace Ic3
{

	enum class EActiveState : uint16
	{
		Disabled = 0,
		Enabled = 1,
		Unknown = 2
	};

	enum EAccessModeFlags : uint32
	{
		E_ACCESS_MODE_FLAG_READ_BIT = 0x0001,
		E_ACCESS_MODE_FLAG_WRITE_BIT = 0x0002,
		E_ACCESS_MODE_FLAGS_FULL_ACCESS = E_ACCESS_MODE_FLAG_READ_BIT | E_ACCESS_MODE_FLAG_WRITE_BIT,
		E_ACCESS_MODE_FLAGS_NONE = 0
	};

	/// @brief Declares a namespace-scope function template for querying Ic3::EnumTypeInfo for enum types.
	#define ic3EnableEnumTypeInfoSupport() \
        /* Template function used to retrieve an EnumTypeInfo object for an enum type. */ \
        /* Not implemented, specialized for every enum using ic3TypeInfoEnumDeclare. */  \
        template <typename TEnum> const ::Ic3::EnumTypeInfo<TEnum> & queryEnumTypeInfo();

	/// @brief Declares an enum for which an Ic3::EnumTypeInfo object can be queried.
	/// Extra arguments are used as a function specifier. You can pass e.g. declspec(dllexport/import) if the
	/// client library which provides the implementation is a dynamic library.
	/// Usage: call it once after an enum definition (in a header file), passing enum type as an argument.
	/// Example:
	/// enum class Color { Red, Green, Blue, Unknown };
	/// ic3TypeInfoEnumDeclare( Color );
	#define ic3TypeInfoEnumDeclare( TEnum, ... ) \
        namespace _typeinfo {                                                                                \
            /* Forward declaration of an enum-specific query function, e.g. queryEnumTypeInfoColor() */      \
            ::Ic3::EnumTypeInfo<TEnum> & queryEnumTypeInfo##TEnum();                                       \
            const std::string & toString##TEnum( TEnum );                                                  \
        } /* namespace _typeinfo */                                                                          \
        /* Implementation of queryEnumTypeInfo<TEnum> template function specialization. */                  \
        template <> inline const ::Ic3::EnumTypeInfo<TEnum> & queryEnumTypeInfo<TEnum>()                   \
        {                                                                                                    \
            /* Just call the enum-specific function. This allows moving the definition code to .cpp. */      \
            return _typeinfo::queryEnumTypeInfo##TEnum();                                                   \
        }                                                                                                    \
        IC3_ATTR_NO_DISCARD inline const std::string & toString( TEnum pValue )                                                 \
        {                                                                                                    \
        	/* Just call the enum-specific function. This allows moving the definition code to .cpp. */      \
        	return _typeinfo::toString##TEnum( pValue );                                                    \
        }

	// Enable support for enum type info for the whole Ic3:: namespace.
	ic3EnableEnumTypeInfoSupport();

	ic3TypeInfoEnumDeclare( EActiveState );
	ic3TypeInfoEnumDeclare( EAccessModeFlags );

}

#endif // __IC3_CORELIB_CORE_ENUMS_H__
