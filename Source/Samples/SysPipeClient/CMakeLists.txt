
set( IC3_SAMPLES_SRC_SysPipeClient
        "Main.cpp"
        )

add_executable( Sample.SysPipeClient
        ${IC3_SAMPLES_SRC_SysPipeClient}
        )

target_link_libraries( Sample.SysPipeClient PUBLIC
        Ic3.System
        )

if( "${IC3_COMPONENTS_BUILD_MODE}" STREQUAL "STATIC" )
    target_compile_definitions( Sample.SysPipeClient PRIVATE
            "${IC3_COMMON_MODULE_DEFINITIONS}" )
endif()
