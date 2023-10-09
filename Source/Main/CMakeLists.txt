
set( _IC3_SRC_MAIN_SUBDIR_LIST
        "Platform"
        "Cppx"
        "Math"
        "CoreLib"
        "System"
        "Graphics/CGI"
        "Script"
        "NxMain"
        )

add_subdirectory( "Ic3/Platform" )
add_subdirectory( "Ic3/Cppx" )
add_subdirectory( "Ic3/Math" )
add_subdirectory( "Ic3/CoreLib" )
add_subdirectory( "Ic3/System" )
add_subdirectory( "Ic3/Graphics/GCI" )
add_subdirectory( "Ic3/Script" )
add_subdirectory( "Ic3/NxMain" )

add_subdirectory( "Ic3/Graphics/HW3D" )
