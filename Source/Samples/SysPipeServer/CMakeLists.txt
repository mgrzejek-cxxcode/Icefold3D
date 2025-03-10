
set( IC3_SAMPLES_SRC_SysPipeServer
        "Main.cpp"
        )

add_executable( Sample.SysPipeServer
        ${IC3_SAMPLES_SRC_SysPipeServer}
        )

target_link_libraries( Sample.SysPipeServer PUBLIC
        Ic3.System
        )

if( "${IC3_COMPONENTS_BUILD_MODE}" STREQUAL "STATIC" )
    target_compile_definitions( Sample.SysPipeServer PRIVATE
            "${IC3_COMMON_MODULE_DEFINITIONS}" )
endif()
