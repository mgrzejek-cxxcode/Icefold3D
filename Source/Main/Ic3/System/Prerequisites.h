
#ifndef __IC3_SYSTEM_PREREQUISITES_H__
#define __IC3_SYSTEM_PREREQUISITES_H__

#include <Ic3/CoreLib/Exception.h>
#include <Ic3/Cppx/Bitmask.h>
#include <Ic3/Cppx/Utilities.h>
#include <Ic3/Cppx/Version.h>

#include <memory>
#include <string>
#include <utility>

#if( IC3_BUILD_STATIC )
#  define IC3_SYSTEM_API
#  define IC3_SYSTEM_CLASS
#  define IC3_SYSTEM_OBJ extern __cdecl
#else
#  if( IC3_SYSTEM_BUILD )
#	define IC3_SYSTEM_API   IC3_PCL_ATTR_DLL_EXPORT
#	define IC3_SYSTEM_CLASS IC3_PCL_ATTR_DLL_EXPORT
#	define IC3_SYSTEM_OBJ   IC3_PCL_ATTR_DLL_EXPORT
#  else
#	define IC3_SYSTEM_API   IC3_PCL_ATTR_DLL_IMPORT
#	define IC3_SYSTEM_CLASS IC3_PCL_ATTR_DLL_IMPORT
#	define IC3_SYSTEM_OBJ   IC3_PCL_ATTR_DLL_IMPORT
#  endif
#endif

#define IC3_SYSTEM_API_NODISCARD IC3_SYSTEM_API IC3_ATTR_NO_DISCARD

namespace Ic3::System
{

	ic3EnableCustomExceptionSupport();
	ic3EnableEnumTypeInfoSupport();

	template <typename TObject>
	using SysHandle = ::Ic3::SharedHandle<TObject>;

	/// @brief Declares the handle type for a given system class.
	/// The handle's type name is created by appending 'Handle' suffix to a given class name.
	/// For example: ic3SysDeclareHandle( MyType ) will produce a declaration for 'MyTypeHandle'.
	#define ic3SysDeclareHandle( pType ) \
		class pType; using pType##Handle = SysHandle<::Ic3::System::pType>

	// These two types need to be visible everywhere.
	ic3SysDeclareHandle( SysContext );
	ic3SysDeclareHandle( SysObject );

	enum : exception_category_value_t
	{
		E_EXCEPTION_CATEGORY_SYSTEM_CORE    = CxDefs::declareExceptionCategory( ExceptionBaseType::System, 0x01 ),
		E_EXCEPTION_CATEGORY_SYSTEM_DISPLAY = CxDefs::declareExceptionCategory( ExceptionBaseType::System, 0x02 ),
		E_EXCEPTION_CATEGORY_SYSTEM_EVENT   = CxDefs::declareExceptionCategory( ExceptionBaseType::System, 0x03 ),
		E_EXCEPTION_CATEGORY_SYSTEM_FILE    = CxDefs::declareExceptionCategory( ExceptionBaseType::System, 0x04 ),
		E_EXCEPTION_CATEGORY_SYSTEM_METAL   = CxDefs::declareExceptionCategory( ExceptionBaseType::System, 0x05 ),
		E_EXCEPTION_CATEGORY_SYSTEM_OPENGL  = CxDefs::declareExceptionCategory( ExceptionBaseType::System, 0x07 ),
		E_EXCEPTION_CATEGORY_SYSTEM_WINDOW  = CxDefs::declareExceptionCategory( ExceptionBaseType::System, 0x09 ),
	};

	enum : exception_code_value_t
	{
		E_EXC_SYSTEM_INTERFACE_NOT_SUPPORTED = CxDefs::declareExceptionCode( E_EXCEPTION_CATEGORY_SYSTEM_CORE, 0x04 ),
	};

	/// @brief Helper proxy-like base class for platform-specific types.
	/// @tparam TBaseType Base class to derive from.
	/// @tparam TNativeData Native data type with platform-specific state.
	///
	/// Public classes in the System component (like File, Window, GLDriver etc.) are implemented at the platform
	/// level (Win32File, Win32Window, Win32GLDriver). These classes obviously carry some platform-specific data.
	/// This class serves as a helper which adds public mNativeData member of a specified type and inherits from
	/// a given base class. It enables much compact definition of platform-specific types - especially those, that
	/// only require native data (like Adapter/Output/VideoMode from the display system) - they can be defined as a
	/// simple typedef.
	template <typename TBaseType, typename TNativeData>
	class NativeObject : public TBaseType
	{
	public:
		TNativeData mNativeData;

	public:
		template <typename... TBaseArgs>
		explicit NativeObject( TBaseArgs && ...pBaseArgs )
		: TBaseType( std::forward<TBaseArgs>( pBaseArgs )... )
		{}

		virtual ~NativeObject() = default;
	};

	template <typename TBaseType>
	class NativeObject<TBaseType, void> : public TBaseType
	{
	public:
		template <typename... TBaseArgs>
		explicit NativeObject( TBaseArgs && ...pBaseArgs )
		: TBaseType( std::forward<TBaseArgs>( pBaseArgs )... )
		{}

		virtual ~NativeObject() = default;
	};

} // namespace Ic3::System

#endif // __IC3_SYSTEM_PREREQUISITES_H__
