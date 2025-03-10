
set( IC3_SAMPLES_SRC_GfxTest
    "MeshDefs.h"
    "MainGfx.hxx"
)

if( "${CM_EBS_SYSCONF_TARGET_SYSTEM}" STREQUAL "macOS" )
    set( IC3_SAMPLES_SRC_GfxTest
        ${IC3_SAMPLES_SRC_GfxTest}
        "MainGfx.mm"
    )
else()
    set( IC3_SAMPLES_SRC_GfxTest
        ${IC3_SAMPLES_SRC_GfxTest}
        "MainGfx.cpp"
    )
endif()

#add_executable( Sample.GfxTest
#
#)

ic3AddCoreApp( Sample.GfxTest ${IC3_SAMPLES_SRC_GfxTest} )

target_link_libraries( Sample.GfxTest PUBLIC
    ${IC3_SAMPLES_LINK_INPUT}
)
