
set( IC3_SCRIPT_SRC_FILES

	"Prerequisites/CommonDefs.h"
	"Prerequisites/CommonTypes.h"

	"Prerequisites.h"
	"ScriptSystem.h"
	"ScriptSystem.cpp"
	"ScriptModule.h"
	"ScriptModule.cpp"

	"Lua/LuaCommon.h"
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

ic3AddCoreLib( Ic3.Script ${IC3_SCRIPT_SRC_FILES} )

target_link_libraries( Ic3.Script PUBLIC
		ext-lua
		Ic3.CoreLib
		Ic3.System )
