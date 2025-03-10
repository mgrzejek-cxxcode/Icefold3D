
cmake_minimum_required( VERSION 3.10 )

set( CMAKE_PROJECT_ROOT_PATH "${CMAKE_CURRENT_SOURCE_DIR}" )
string( REPLACE "\\" "/" CMAKE_PROJECT_ROOT_PATH "${CMAKE_PROJECT_ROOT_PATH}" )
set( PCL_PROJECT_ROOT_PATH "${CMAKE_PROJECT_ROOT_PATH}" )

set( CMAKE_C_STANDARD 11 )
set( CMAKE_C_STANDARD_REQUIRED ON )
set( CMAKE_C_EXTENSIONS OFF )

set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_CXX_EXTENSIONS OFF )

project( Icefold )

message("[Version]: CMake ${CMAKE_VERSION}")
message("[Root Dir]: ${CMAKE_CURRENT_SOURCE_DIR}")

include( "CMake/CMakeLists.txt" )

if( "${CM_EBS_SYSCONF_HOST_COMPILER}" STREQUAL "GCC" )
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wall -Wextra -Wuninitialized -Winit-self -ansi -fms-extensions" )
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-missing-declarations" )
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-sign-compare" )
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-switch" )
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-enum-enum-conversion" )
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0" )
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -rdynamic" )
endif()

if( "${CM_EBS_SYSCONF_HOST_COMPILER}" STREQUAL "MSVC" )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP" )
endif()

ebsSetInternal( IC3_BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}" )
ebsSetInternal( IC3_COMPONENTS_BUILD_MODE STATIC )

ebsSetInternal( IC3_COMMON_MODULE_DEFINITIONS
	"IC3_BUILD_STATIC=1"
)

ebsSetInternal( IC3_BASE_DIR
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

ebsSetInternal( EXT_INCLUDE_DIR
		"${IC3_BASE_DIR}/External/Embedded/"
        "${IC3_BASE_DIR}/External/Prebuilt/Include/"
)

ebsSetInternal( IC3_INCLUDE_DIR
		"${IC3_BASE_DIR}/External/Embedded/"
        "${IC3_BASE_DIR}/External/Prebuilt/Include/"
        "${IC3_BASE_DIR}/Source/Main/"
)
    
ebsSetInternal( IC3_LINK_DIR
        "${IC3_BASE_DIR}/External/Prebuilt/Lib/Common"
        "${IC3_BASE_DIR}/External/Prebuilt/Lib/Common/${CM_EBS_SYSCONF_TARGET_ARCH}"
        "${IC3_BASE_DIR}/External/Prebuilt/Lib/Platform/${CM_EBS_SYSCONF_TARGET_LIBRARY_SUBDIR}"
        "${IC3_BASE_DIR}/External/Prebuilt/Lib/Platform/${CM_EBS_SYSCONF_TARGET_LIBRARY_SUBDIR}/${CM_EBS_SYSCONF_TARGET_ARCH}"
)

ebsSetInternal(IC3_MODULE_TARGET_LIST "")

function( ebsDisableQtSupportWithMessage pMessage )
	message( WARNING "Qt support will be disabled. Reason: ${pMessage}." )
	ebsSetInternal( IC3_CONFIG_QT_SUPPORT_FLAG FALSE )
endfunction()

function( ebsCheckQtConfigForWinDesktop )
	if( $ENV{QT_ROOT} STREQUAL "" )
		ebsDisableQtSupportWithMessage( "QT_ROOT environment variable is missing" )
		return()
	endif()
	if( $ENV{QT_VERSION} STREQUAL "" )
		ebsDisableQtSupportWithMessage( "QT_VERSION environment variable is missing" )
		return()
	endif()
endfunction()

function( ebsGenerateQtConfig )
	if( "${CM_EBS_SYSCONF_TARGET_SYSTEM}" MATCHES "WinDesktop" ) # More will be added here as we go
		# For desktop platfroms, we enable Qt support (used only for building desktop tools and utilities).
		ebsSetInternal( IC3_CONFIG_QT_SUPPORT_FLAG TRUE )

		if( "${CM_EBS_SYSCONF_TARGET_SYSTEM}" STREQUAL "WinDesktop" )
			# Validate Windows-specific Qt configuration. This will automatically clear the support flag in case of an error.
			ebsCheckQtConfigForWinDesktop()
			# If still set, everything is fine and we can proceed.
			if( IC3_CONFIG_QT_SUPPORT_FLAG )
				# Unify the slashes to a common format (we don't like the backslashing mess).
				string( REGEX REPLACE "\\\\" "/" IC3_CONFIG_QT_ROOT_TMP "$ENV{QT_ROOT}" )
				ebsSetInternal( IC3_CONFIG_QT_ROOT "${IC3_CONFIG_QT_ROOT_TMP}" )
				ebsSetInternal( IC3_CONFIG_QT_VERSION "$ENV{QT_VERSION}" )

				if( "${CM_EBS_SYSCONF_HOST_COMPILER}" MATCHES "MSVC" )
					if( "${CM_EBS_SYSCONF_HOST_TOOLSET_ARCH}" MATCHES "arm64" )
						ebsSetInternal( IC3_CONFIG_QT_BASE_DIR "${IC3_CONFIG_QT_ROOT}/${IC3_CONFIG_QT_VERSION}/msvc${CM_EBS_SYSCONF_HOST_COMPILER_VERSION}_arm64" )
					elseif( "${CM_EBS_SYSCONF_HOST_TOOLSET_ARCH}" MATCHES "x86-64" )
						ebsSetInternal( IC3_CONFIG_QT_BASE_DIR "${IC3_CONFIG_QT_ROOT}/${IC3_CONFIG_QT_VERSION}/msvc${CM_EBS_SYSCONF_HOST_COMPILER_VERSION}_64" )
					endif()
				else()
					# This might trigger in case of change of the Qt version or a new toolset version - adjust the logic above if necessary.
					ebsDisableQtSupportWithMessage( "The current compiler (${CM_EBS_SYSCONF_HOST_COMPILER}) is not yet supported for Qt on this platform" )
				endif()
			endif()
		endif()

		if( IC3_CONFIG_QT_BASE_DIR )
			message( STATUS "Qt support enabled. QT_BASE_DIR = ${IC3_CONFIG_QT_BASE_DIR}" )
		endif()
	else()
		# This will also trigger for platform we have not added support for.
		ebsDisableQtSupportWithMessage( "The current system (${CM_EBS_SYSCONF_TARGET_SYSTEM}) is not yet supported for Qt" )
	endif()
endfunction()

ebsGenerateQtConfig()

if( IC3_CONFIG_QT_SUPPORT_FLAG )
	find_package( Qt6 REQUIRED COMPONENTS Core Gui Widgets )
endif()

function( ebsAddTargetQtSupport pTargetName pQtLibs )
	target_include_directories( "${pTargetName}" PRIVATE :"${IC3_CONFIG_QT_BASE_DIR}/include" )
	target_link_directories( "${pTargetName}" PRIVATE :"${IC3_CONFIG_QT_BASE_DIR}/lib" )
endfunction()

add_subdirectory( "External/Embedded" )
add_subdirectory( "Source/Main" )
add_subdirectory( "Source/Samples" )

if( IC3_CONFIG_QT_SUPPORT_FLAG )
	add_subdirectory( "Source/Tools" )
endif()

message( STATUS "${IC3_MODULE_TARGET_LIST}")
foreach( iceModule ${IC3_MODULE_TARGET_LIST} )
	if( TARGET "${iceModule}" )
		set_property( TARGET "${iceModule}" PROPERTY CXX_STANDARD 17 )
		target_compile_features( "${iceModule}" PUBLIC cxx_std_17 )
	endif()
endforeach()

if( "${CM_EBS_SYSCONF_TARGET_PLATFORM}" STREQUAL "Android" )
	add_subdirectory( Source/Appx/Boot/Android/launcher/src )
endif()
