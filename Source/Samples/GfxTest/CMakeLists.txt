
set( IC3_SAMPLES_SRC_GfxTest
        "MainGfx.mm"
        )

add_executable( Sample.GfxTest
        ${IC3_SAMPLES_SRC_GfxTest}
        )

target_link_libraries( Sample.GfxTest PUBLIC
        ${IC3_SAMPLES_LINK_INPUT} )

if( "${IC3_COMPONENTS_BUILD_MODE}" STREQUAL "STATIC" )
    target_compile_definitions( Sample.GfxTest PRIVATE
            "IC3_BUILD_STATIC=1" )
endif()
