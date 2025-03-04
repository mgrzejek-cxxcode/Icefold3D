
set( IC3_SAMPLES_SRC_Render01
        "MeshDefs.h"
        "Main01.mm"
        )

add_executable( Sample.Render01
        ${IC3_SAMPLES_SRC_Render01}
        )

target_link_libraries( Sample.Render01 PUBLIC
        ${IC3_SAMPLES_LINK_INPUT} )

if( "${IC3_COMPONENTS_BUILD_MODE}" STREQUAL "STATIC" )
    target_compile_definitions( Sample.Render01 PRIVATE
            "IC3_BUILD_STATIC=1" )
endif()
