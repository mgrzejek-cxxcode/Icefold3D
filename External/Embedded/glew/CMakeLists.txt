
set( IC3_EXTLIB_GLEW_SCR_FILES
		glew.c )

add_library( ExtLib.GLEW STATIC
        ${IC3_EXTLIB_GLEW_SCR_FILES} )

set_target_properties( ExtLib.GLEW PROPERTIES
        OUTPUT_NAME "glew" )

target_include_directories( ExtLib.GLEW PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
		${IC3_BASE_DIR}/external/include )

target_compile_definitions( ExtLib.GLEW PUBLIC
		GLEW_BUILD=1
		GLEW_STATIC=1
		_LIB=1
		WIN32_LEAN_AND_MEAN )

if( "${CM_EBS_SYSCONF_HOST_COMPILER}" MATCHES "Clang|GCC" )
	target_compile_options( ExtLib.GLEW PRIVATE
			-Wno-implicit-function-declaration
			)
endif()
