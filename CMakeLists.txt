
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

if ("${CM_EBS_SYSCONF_TARGET_SYSTEM}" STREQUAL "macOS")
	set( CMAKE_OSX_SYSROOT
			"/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX15.2.sdk" )

	if( NOT CMAKE_C_COMPILER )
		set( CMAKE_C_COMPILER
				"/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang" )
	endif()

	if (NOT CMAKE_CXX_COMPILER)
		set( CMAKE_CXX_COMPILER
				"/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang++" )
	endif ()

	if (NOT CMAKE_OBJC_COMPILER)
		set( CMAKE_OBJC_COMPILER
				"/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang" )
	endif ()

	if (NOT CMAKE_OBJCXX_COMPILER)
		set( CMAKE_OBJCXX_COMPILER
				"/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang++" )
	endif ()

	# ebsSetInternal( IC3_EXTERNAL_WX_OSX_PREBUILT_SUFFIX_LIB_DIR "unicode-3.2" )
	# ebsSetInternal( IC3_EXTERNAL_WX_OSX_PREBUILT_SUFFIX_LIB_NAME "u-3.2" )
	# ebsSetInternal( IC3_EXTERNAL_WX_OSX_PREBUILT_LIB_DIR "${IC3_BASE_DIR}/External/Embedded/wx-prebuilt-osx/lib/Debug" )
	# ebsAppendInternal( IC3_EXTERNAL_WX_OSX_PREBUILT_INCLUDE_DIR "${IC3_BASE_DIR}/External/Embedded/wx/include" )

	ebsAppendInternal( IC3_EXTERNAL_WX_OSX_PREBUILT_INCLUDE_DIR
			"${IC3_BASE_DIR}/External/Embedded/wx-prebuilt-osx/lib/wx/include/osx_cocoa-${IC3_EXTERNAL_WX_OSX_PREBUILT_SUFFIX_LIB_DIR}" )
endif ()

if ("${CM_EBS_SYSCONF_TARGET_SYSTEM}" STREQUAL "macOS")
	ebsSetInternal( IC3_TARGET_INCLUDE_DIR_WX "${IC3_EXTERNAL_WX_OSX_PREBUILT_INCLUDE_DIR}" )
	ebsSetInternal( IC3_TARGET_LIB_DIR_WX "${IC3_EXTERNAL_WX_OSX_PREBUILT_LIB_DIR}" )
	ebsSetInternal( IC3_TARGET_LINK_LIB_WX "wx_osx_cocoa${IC3_EXTERNAL_WX_OSX_PREBUILT_SUFFIX_LIB_NAME}" )
else()
	ebsSetInternal( IC3_TARGET_INCLUDE_DIR_WX "" )
	ebsSetInternal( IC3_TARGET_LIB_DIR_WX "" )
	ebsSetInternal( IC3_TARGET_LINK_LIB_WX "wxmono" )
endif()

function( setTargetWxWidgetsIncludePath pTargetName pBuildConfigFlags )
	foreach( includeDirFlag ${pBuildConfigFlags} )
		if("${includeDirFlag}" MATCHES "^-I" )
			string( REGEX REPLACE "^-I" "" includeDir "${includeDirFlag}" )
			list( APPEND IC3_WX_CONFIG_INCDIR_LIST "${includeDir}" )
		endif()
	endforeach()
	target_include_directories( "${pTargetName}" PUBLIC "${IC3_WX_CONFIG_INCDIR_LIST}" )
endfunction()

function( setTargetWxWidgetsConfig pTargetName )
	set( WX_BUILD_BUILD_ARGS "--cxxflags;--static=yes" )
	set( WX_BUILD_LINK_ARGS "--libs;--static=yes" )
	if( CM_EBS_SYSCONF_TARGET_DEBUG_ENABLED )
		set( WX_BUILD_BUILD_ARGS "${WX_BUILD_BUILD_ARGS};--debug=yes" )
		set( WX_BUILD_LINK_ARGS "${WX_BUILD_LINK_ARGS};--debug=yes" )
	endif()
	execute_process( COMMAND wx-config ${WX_BUILD_BUILD_ARGS} OUTPUT_VARIABLE IC3_WX_CONFIG_BUILD_FLAGS )
	execute_process( COMMAND wx-config ${WX_BUILD_LINK_ARGS} OUTPUT_VARIABLE IC3_WX_CONFIG_LINK_FLAGS )

	string( REGEX REPLACE "[ ]+" " " IC3_WX_CONFIG_BUILD_FLAGS ${IC3_WX_CONFIG_BUILD_FLAGS} )
	string( REGEX REPLACE "^[ \n\r]+" "" IC3_WX_CONFIG_BUILD_FLAGS ${IC3_WX_CONFIG_BUILD_FLAGS} )
	string( REGEX REPLACE "[ \n\r]+$" "" IC3_WX_CONFIG_BUILD_FLAGS ${IC3_WX_CONFIG_BUILD_FLAGS} )

	string( REGEX REPLACE "[ ]+" " " IC3_WX_CONFIG_LINK_FLAGS ${IC3_WX_CONFIG_LINK_FLAGS} )
	string( REGEX REPLACE "^[ \n\r]+" "" IC3_WX_CONFIG_LINK_FLAGS ${IC3_WX_CONFIG_LINK_FLAGS})
	string( REGEX REPLACE "[ \n\r]+$" "" IC3_WX_CONFIG_LINK_FLAGS ${IC3_WX_CONFIG_LINK_FLAGS})

	string(REGEX REPLACE " " ";" IC3_WX_CONFIG_BUILD_FLAGS_LIST ${IC3_WX_CONFIG_BUILD_FLAGS})

	#message( STATUS "IC3_WX_CONFIG_BUILD_FLAGS: [${IC3_WX_CONFIG_BUILD_FLAGS}]" )
	#message( STATUS "IC3_WX_CONFIG_LINK_FLAGS: [${IC3_WX_CONFIG_LINK_FLAGS}]" )
	#message( STATUS "IC3_WX_CONFIG_BUILD_FLAGS_LIST: [${IC3_WX_CONFIG_BUILD_FLAGS_LIST}]" )

	target_compile_options( "${pTargetName}" PRIVATE "${IC3_WX_CONFIG_BUILD_FLAGS}" )
	target_link_libraries( "${pTargetName}" PUBLIC "${IC3_WX_CONFIG_LINK_FLAGS}" )

	setTargetWxWidgetsIncludePath( ${pTargetName} "${IC3_WX_CONFIG_BUILD_FLAGS_LIST}" )
endfunction()

add_subdirectory( "External/Embedded" )
add_subdirectory( "Source/Main" )
add_subdirectory( "Source/Samples" )
add_subdirectory( "Source/Tools" )

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
