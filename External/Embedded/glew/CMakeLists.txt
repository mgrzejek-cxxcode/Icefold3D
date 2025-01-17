
set( IC3_EXTLIB_GLEW_SCR_FILES
		glew.c )

add_library( ext-glew STATIC
        ${IC3_EXTLIB_GLEW_SCR_FILES} )

set_target_properties( ext-glew PROPERTIES
        OUTPUT_NAME "glew" )

target_include_directories( ext-glew PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
		${IC3_BASE_DIR}/External/Prebuilt/Include )

target_compile_definitions( ext-glew PUBLIC
		GLEW_BUILD=1
		GLEW_STATIC=1
		_LIB=1
		WIN32_LEAN_AND_MEAN )

if( "${CM_EBS_SYSCONF_HOST_COMPILER}" MATCHES "Clang|GCC" )
	target_compile_options( ext-glew PRIVATE
			-Wno-implicit-function-declaration
			)
endif()
