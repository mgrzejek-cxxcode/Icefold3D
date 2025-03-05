
add_subdirectory( "zlib" )
add_subdirectory( "cppx" )
add_subdirectory( "freetype" )
add_subdirectory( "glew" )
add_subdirectory( "lua" )
add_subdirectory( "png" )
add_subdirectory( "assimp" )

set(wxBUILD_MONOLITHIC ON)
set(wxBUILD_SHARED OFF)
set(wxBUILD_DEMOS OFF)
set(wxBUILD_SAMPLES OFF)
set(wxBUILD_TESTS OFF)
set(wxUSE_UNICODE OFF)

add_subdirectory( "wx" )

target_include_directories( ext-assimp PUBLIC "${ASSIMP_INCLUDE_DIRS}/assimp/include" )
target_include_directories( ext-png PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/zlib" )
