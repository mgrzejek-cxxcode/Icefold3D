
set( IC3_PLATFORM_SRC_FILES
		"Internal/Environment.h"
		"Internal/Include.h"
		"Internal/Types.h"
		"Internal/SIMDExt.h"

		"Internal/Environment/Arch/ARM.h"
		"Internal/Environment/Arch/ARM64.h"
		"Internal/Environment/Arch/X86.h"
		"Internal/Environment/Arch/X86-64.h"
		"Internal/Environment/Compiler/Clang.h"
		"Internal/Environment/Compiler/GCC.h"
		"Internal/Environment/Compiler/ICC.h"
		"Internal/Environment/Compiler/MSVC.h"
		"Internal/Environment/OS/Android.h"
		"Internal/Environment/OS/iOS.h"
		"Internal/Environment/OS/Linux.h"
		"Internal/Environment/OS/OSX.h"
		"Internal/Environment/OS/WinDesktop.h"
		"Internal/Environment/OS/WinUWP.h"

		"Platform.h"
		"Platform.cpp"
		"Debug.h"
		"Debug.cpp"
		"FileLocationInfo.h"
		"FileLocationInfo.cpp"
		"GDS.h"
		"SyncCommon.h"
		"SyncCommon.cpp"
		)

if( "${CM_EBS_SYSCONF_TARGET_SYSTEM}" STREQUAL "Android" )
	set( IC3_PLATFORM_SRC_FILES
			${IC3_PLATFORM_SRC_FILES}
			"Android/androidNativeAppProxy.h"
			"Android/androidNativeAppProxy.cpp"
			)
endif()

add_library( Ic3.Platform STATIC ${IC3_PLATFORM_SRC_FILES} )

target_compile_definitions( Ic3.Platform PRIVATE
		IC3_PCL_PROJECT_ROOT_PATH="${IC3_PCL_PROJECT_ROOT_PATH}"
		)

set_target_properties( Ic3.Platform PROPERTIES
		OUTPUT_NAME "Ic3.Platform"
		)

target_include_directories( Ic3.Platform PUBLIC
		"${IC3_INCLUDE_DIR}"
		)

target_link_libraries( Ic3.Platform PUBLIC
		)
