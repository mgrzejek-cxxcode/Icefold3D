
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

message("VERSION: CMake ${CMAKE_VERSION}")
message("ROOT: ${CMAKE_CURRENT_SOURCE_DIR}")

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

ebsSetInternal( IC3_BASE_DIR
    "${CMAKE_CURRENT_SOURCE_DIR}" )

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

add_subdirectory( "External/Embedded" )
add_subdirectory( "Source/Main" )
add_subdirectory( "Source/Samples" )
#add_subdirectory( "Source/Tools" )

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
