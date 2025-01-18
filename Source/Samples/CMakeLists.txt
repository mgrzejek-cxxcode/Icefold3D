
set( IC3_SAMPLES_LINK_INPUT
        Ic3.CoreLib
        Ic3.NxMain
        Ic3.Graphics.GCI
)

if( TARGET Ic3.Graphics.HW3D.DX11 )
    set( IC3_SAMPLES_LINK_INPUT ${IC3_SAMPLES_LINK_INPUT} "Ic3.Graphics.HW3D.DX11" )
endif()

if( TARGET Ic3.Graphics.HW3D.DX12 )
    set( IC3_SAMPLES_LINK_INPUT ${IC3_SAMPLES_LINK_INPUT} "Ic3.Graphics.HW3D.DX12" )
endif()

if( TARGET Ic3.Graphics.HW3D.GL4 )
    set( IC3_SAMPLES_LINK_INPUT ${IC3_SAMPLES_LINK_INPUT} "Ic3.Graphics.HW3D.GL4" )
endif()

if( TARGET Ic3.Graphics.HW3D.GLES3 )
    set( IC3_SAMPLES_LINK_INPUT ${IC3_SAMPLES_LINK_INPUT} "Ic3.Graphics.HW3D.GLES3" )
endif()

message("/Samples: ${CMAKE_CURRENT_SOURCE_DIR}")

add_subdirectory( "Render01" )
add_subdirectory( "Script01" )
