
set( IC3_SAMPLES_SRC_Script01
        "Main.mm"
        )

add_executable( Sample.Script01
        ${IC3_SAMPLES_SRC_Script01}
        )

target_link_libraries( Sample.Script01 PUBLIC
        Ic3.CoreLib
        Ic3.Script
        Ic3.System
        Ic3.NxMain
        )

if( "${IC3_COMPONENTS_BUILD_MODE}" STREQUAL "STATIC" )
    target_compile_definitions( Sample.Script01 PRIVATE
            "IC3_BUILD_STATIC=1" )
endif()
