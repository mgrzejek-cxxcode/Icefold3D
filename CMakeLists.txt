
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

if( CM_EBS_SYSCONF_TARGET_DEBUG_ENABLED )
	set( IC3_BUILD_TYPE_DEFINE "PCL_DEBUG=1" )
else()
	set( IC3_BUILD_TYPE_DEFINE "PCL_RELEASE=1" )
endif()

if( NOT IC3_BUILD_DYNAMIC_LIBS )
	ebsSetInternal( IC3_COMPONENTS_BUILD_MODE STATIC )
	ebsSetInternal( IC3_COMMON_MODULE_DEFINITIONS "IC3_BUILD_STATIC=1;${IC3_BUILD_TYPE_DEFINE}" )
else()
	ebsSetInternal( IC3_COMPONENTS_BUILD_MODE DYNAMIC )
	ebsSetInternal( IC3_COMMON_MODULE_DEFINITIONS "IC3_BUILD_STATIC=0;${IC3_BUILD_TYPE_DEFINE}" )
endif()

if( "${IC3_COMPONENTS_BUILD_MODE}" STREQUAL "DYNAMIC" )
	ebsSetInternal( IC3_COMPONENT_LIB_TYPE "LibDll" )
	ebsSetInternal( IC3_COMPONENT_LIB_DIR_SUFFIX "dll" )
else()
	ebsSetInternal( IC3_COMPONENT_LIB_TYPE "LibStatic" )
	ebsSetInternal( IC3_COMPONENT_LIB_DIR_SUFFIX "static" )
endif()

function( ic3SetTargetOutputDir pTargetName )
	if( TARGET "${pTargetName}" )
		string( TOLOWER "${CM_EBS_SYSCONF_HOST_COMPILER}" TID_COMPILER )
		string( TOLOWER "${CM_EBS_SYSCONF_TARGET_BUILDCONFIG}" TID_BUILDCONFIG )

		set( TID_SYSTEM "${CM_EBS_SYSCONF_TARGET_SYSTEM}")
		set( TID_ARCH "${CM_EBS_SYSCONF_TARGET_ARCH}" )
		set( TID_LIBSUFFIX "${IC3_COMPONENT_LIB_DIR_SUFFIX}" )

		string( REGEX REPLACE "(\\.[0-9]+)+" "" TID_COMPILER_VER "${CM_EBS_SYSCONF_HOST_COMPILER_VERSION}" )

		set_target_properties("${pTargetName}"
				PROPERTIES
				ARCHIVE_OUTPUT_DIRECTORY "${IC3_BASE_DIR}/Lib/${TID_SYSTEM}/${TID_COMPILER}${TID_COMPILER_VER}.${TID_ARCH}.${TID_LIBSUFFIX}.${TID_BUILDCONFIG}"
				LIBRARY_OUTPUT_DIRECTORY "${IC3_BASE_DIR}/Lib/${TID_SYSTEM}/${TID_COMPILER}${TID_COMPILER_VER}.${TID_ARCH}.${TID_LIBSUFFIX}.${TID_BUILDCONFIG}"
				RUNTIME_OUTPUT_DIRECTORY "${IC3_BASE_DIR}/Bin/${TID_SYSTEM}/${TID_COMPILER}${TID_COMPILER_VER}.${TID_ARCH}.${TID_LIBSUFFIX}.${TID_BUILDCONFIG}"
		)
	endif()
endfunction()

function( ic3SetCoreTargetCompileDefinitions pTargetName )
	# Target is the name of one of the main ("core") Ic3 targets, (e.g. Ic3.Graphics.HW3D.DX11).
	if( TARGET "${pTargetName}" )
		# Turn the target name into all-uppercase (e.g. IC3.GRAPHICS.HW3D.DX11).
		string( TOUPPER "${pTargetName}" TARGET_BUILD_STR_BASE )
		# Replace dots with underscores (e.g. IC3_GRAPHICS_HW3D_DX11).
		string( REPLACE "." "_" TARGET_BUILD_STR_BASE "${TARGET_BUILD_STR_BASE}")
		# Construct the project specific build flag (e.g. IC3_GRAPHICS_HW3D_DX11_BUILD=1).
		set( TARGET_BUILD_DEFINE "${TARGET_BUILD_STR_BASE}_BUILD=1" )
		message( "TARGET_BUILD_DEFINE: ${TARGET_BUILD_DEFINE}" )
		# Check for Graphics.HW3D targets where we have two build vars (e.g. GL4 and GL, DX11 and DX, etc).
		if( "${TARGET_BUILD_STR_BASE}" MATCHES "IC3_GRAPHICS_HW3D_([A-Za-z0-9]+)" )
			# IC3_GRAPHICS_HW3D_DX11 -> IC3_GRAPHICS_HW3D_DX
			string( REGEX REPLACE "((ES)?[0-9]+)$" "" TARGET_BUILD_DEFINE_HW3D_COMMON "${TARGET_BUILD_STR_BASE}" )
			set( TARGET_BUILD_DEFINE_HW3D_COMMON "${TARGET_BUILD_DEFINE_HW3D_COMMON}_BUILD=1" )
			message( "TARGET_BUILD_DEFINE_HW3D_COMMON: ${TARGET_BUILD_DEFINE_HW3D_COMMON}" )
		endif()
		# Add standard compile definitions required for all modules
		target_compile_definitions( ${pTargetName} PRIVATE
				# This contains build mode for libraries and other shared definitions.
				"${IC3_COMMON_MODULE_DEFINITIONS}"
				# This is the project specific build flag (e.g. IC3_GRAPHICS_GCI_BUILD=1).
				"${TARGET_BUILD_DEFINE}"
				# Also add HW3D-specific second build define (this will be empty for other targets)
				"${TARGET_BUILD_DEFINE_HW3D_COMMON}"
		)
	endif()
endfunction()

function( ic3AddTarget pTargetType pTargetName pSources )
	if( "${pTargetType}" STREQUAL "Application" )
		add_executable( "${pTargetName}" "${pSources};${ARGN}" )
	elseif( "${pTargetType}" STREQUAL "LibDll" )
		add_library( "${pTargetName}" SHARED "${pSources};${ARGN}" )
	elseif( "${pTargetType}" STREQUAL "LibStatic" )
		add_library( "${pTargetName}" STATIC "${pSources};${ARGN}" )
	endif()

	set_target_properties( ${pTargetName} PROPERTIES
			OUTPUT_NAME "${pTargetName}" )

	ebsSetInternal( IC3_TARGET_TYPE_${pTargetName} "${pTargetType}" )
	ebsAppendInternal( IC3_TARGET_LIST "${pTargetName}" )

	if( "${pTargetName}" MATCHES "Ic3.Graphics.HW3D.([A-Za-z0-9]+)" )
		ebsAppendInternal( IC3_TARGET_LIST_GRAPHICS_HW3D "${pTargetName}" )
	endif()

	ic3SetTargetOutputDir( "${pTargetName}" )
	ic3SetCoreTargetCompileDefinitions( "${pTargetName}" )
endfunction()

function( ic3AddCoreApp pTargetName pSources )
	ic3AddTarget( "Application" "${pTargetName}" "${pSources};${ARGN}" )
	target_include_directories( "${pTargetName}" PUBLIC "${IC3_INCLUDE_DIR}" )
endfunction()

function( ic3AddCoreLib pTargetName pSources )
	ic3AddTarget( "${IC3_COMPONENT_LIB_TYPE}" "${pTargetName}" "${pSources};${ARGN}" )
	target_include_directories( "${pTargetName}" PUBLIC "${IC3_INCLUDE_DIR}" )
	target_link_libraries( "${pTargetName}" PUBLIC ext-cppx ext-cxm )
endfunction()

function( ic3AddExternalLib pTargetName pSources )
	ic3AddTarget( "${IC3_COMPONENT_LIB_TYPE}" "${pTargetName}" "${pSources};${ARGN}" )
endfunction()

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
					if( "${CM_EBS_SYSCONF_TARGET_ARCH}" MATCHES "arm64" )
						ebsSetInternal( IC3_CONFIG_QT_BASE_DIR "${IC3_CONFIG_QT_ROOT}/${IC3_CONFIG_QT_VERSION}/msvc${CM_EBS_SYSCONF_HOST_COMPILER_VERSION}_arm64" )
					elseif( "${CM_EBS_SYSCONF_TARGET_ARCH}" MATCHES "x86_64" )
						ebsSetInternal( IC3_CONFIG_QT_BASE_DIR "${IC3_CONFIG_QT_ROOT}/${IC3_CONFIG_QT_VERSION}/msvc${CM_EBS_SYSCONF_HOST_COMPILER_VERSION}_64" )
					else()
						ebsDisableQtSupportWithMessage("The current arch (${CM_EBS_SYSCONF_TARGET_ARCH}) is not yet supported for Qt with this compiler (MSVC)")
					endif()
				elseif( "${CM_EBS_SYSCONF_HOST_COMPILER}" MATCHES "MinGW" )
					if( "${CM_EBS_SYSCONF_TARGET_ARCH}" MATCHES "x86_64" )
						ebsSetInternal(IC3_CONFIG_QT_BASE_DIR "${IC3_CONFIG_QT_ROOT}/${IC3_CONFIG_QT_VERSION}/mingw_64")
					else()
						ebsDisableQtSupportWithMessage("The current arch (${CM_EBS_SYSCONF_TARGET_ARCH}) is not yet supported for Qt with this compiler (MinGW)")
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

function( ic3AddTargetQtSupport pTargetName pQtLibs )
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
