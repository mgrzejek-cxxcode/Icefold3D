
set( CPPX_PLATFORM_SRC_FILES

		"Platform.h"
		"Platform.cpp"
		"Debug.h"
		"file_location_info.h"
		"file_location_info.cpp"
		"GDS.h"
		)

if( "${CM_EBS_SYSCONF_TARGET_SYSTEM}" STREQUAL "Android" )
	set( CPPX_PLATFORM_SRC_FILES
			${CPPX_PLATFORM_SRC_FILES}
			"Android/androidNativeAppProxy.h"
			"Android/androidNativeAppProxy.cpp"
			)
endif()

add_library( Ic3.Platform STATIC ${CPPX_PLATFORM_SRC_FILES} )

target_compile_definitions( Ic3.Platform PRIVATE
		PCL_PROJECT_ROOT_PATH="${PCL_PROJECT_ROOT_PATH}"
		)

set_target_properties( Ic3.Platform PROPERTIES
		OUTPUT_NAME "Ic3.Platform"
		)

target_include_directories( Ic3.Platform PUBLIC
		"${CPPX_INCLUDE_DIR}"
		)

target_link_libraries( Ic3.Platform PUBLIC
		)
