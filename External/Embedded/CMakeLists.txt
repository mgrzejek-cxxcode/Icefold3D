
set( _EBS_DEPENDENCIES_TARGET_LIST
        "freetype"
        "glew"
        "lua"
        "png"
        "zlib"
        )

add_subdirectory( "zlib" )
add_subdirectory( "freetype" )
add_subdirectory( "glew" )
add_subdirectory( "lua" )
add_subdirectory( "png" )
add_subdirectory( "assimp" )

#foreach( target ${_EBS_DEPENDENCIES_TARGET_LIST} )
#    add_subdirectory( "${target}" )
#    set_property( TARGET "Lib.External.${target}" PROPERTY C_STANDARD 11 )
#    target_compile_features( "Lib.External.${target}" PRIVATE c_std_11 )
#endforeach()

target_include_directories( ExtLib.Assimp PUBLIC "${ASSIMP_INCLUDE_DIRS}/assimp/include" )
target_include_directories( ExtLib.Png PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/zlib" )
