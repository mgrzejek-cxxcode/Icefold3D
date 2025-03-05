
if( "${CM_EBS_SYSCONF_TARGET_SYSTEM}" STREQUAL "macOS" )
set( IC3_SAMPLES_SRC_GfxTest
        "MeshDefs.h"
        "MainGfx.hxx"
        "MainGfx.mm"
        )
else()
set( IC3_SAMPLES_SRC_GfxTest
        "MeshDefs.h"
        "MainGfx.hxx"
        "MainGfx.cpp"
        )
endif()

add_executable( Sample.GfxTest
        ${IC3_SAMPLES_SRC_GfxTest}
        )

target_link_libraries( Sample.GfxTest PUBLIC
        ${IC3_SAMPLES_LINK_INPUT} )

if( "${IC3_COMPONENTS_BUILD_MODE}" STREQUAL "STATIC" )
    target_compile_definitions( Sample.GfxTest PRIVATE
            "IC3_BUILD_STATIC=1" )
endif()

set_target_properties( Sample.GfxTest PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}" )
