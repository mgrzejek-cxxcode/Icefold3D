
set( IC3_SCRIPT_SRC_FILES

	"Prerequisites/CommonDefs.h"
	"Prerequisites/CommonTypes.h"

	"Prerequisites.h"
	"ScriptSystem.h"
	"ScriptSystem.cpp"

	"Lua/LuaCoreAPI.h"
    "Lua/LuaCoreAPI.cpp"
	"Lua/LuaScriptSystem.h"
	"Lua/LuaScriptSystem.cpp"
	"Lua/RefData.h"
	"Lua/RefData.cpp"

	"Lua/Interop/LuaClass.h"
	"Lua/Interop/LuaFunctions.h"
	"Lua/Interop/LuaVariables.h"
)

if( "${IC3_COMPONENTS_BUILD_MODE}" STREQUAL "STATIC" )
    add_library( Ic3.Script STATIC ${IC3_SCRIPT_SRC_FILES} )
    target_compile_definitions( Ic3.Script PRIVATE
			"IC3_BUILD_STATIC=1")
else()
    add_library( Ic3.Script SHARED ${IC3_SCRIPT_SRC_FILES} )
endif()

set_target_properties( Ic3.Script PROPERTIES
		OUTPUT_NAME "Ic3.Script"
		)

target_compile_definitions( Ic3.Script PRIVATE
		"IC3_SCRIPT_BUILD=1"
		)

target_include_directories( Ic3.Graphics.GCI PUBLIC
		"${IC3_INCLUDE_DIR}"
		)

target_link_libraries( Ic3.Script PUBLIC
		ExtLib.Lua
		Ic3.CoreLib
		Ic3.Cppx
		Ic3.System
		)
