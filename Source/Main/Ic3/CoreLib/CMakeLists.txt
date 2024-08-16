
set( IC3_CORELIB_SRC_FILES
		"Prerequisites/CoreDefs.h"
		"Prerequisites/CoreEnums.h"
		"Prerequisites/DynamicInterface.h"
        "Prerequisites/DynamicInterface.cpp"
        "Prerequisites/Result.h"
        "Prerequisites/SharedHandle.h"

        "Prerequisites.h"
        "Prerequisites.cpp"

        "Exception.h"
		"Exception.cpp"
		"ExceptionCommon.h"
		"ExceptionResult.h"
		"ExceptionUtils.h"
        "PixelCommon.h"
        "MathImports.h"

        "Memory/CommonMemoryDefs.h"

		"Reflection/EnumTypeInfo.h"
		"Reflection/EnumTypeInfo.cpp"

		"Sync/AtomicRWLock.h"
		"Sync/CommonSyncDefs.h"
		"Sync/Lockable.h"
		"Sync/MutexCommon.h"
		"Sync/MutexLockTypes.h"
		"Sync/SpinLock.h"
		"Sync/SyncInterface.h"

		"TypeInfo/TPIDefsCoreEnum.cpp"

        "Utility/GDSCore.h"
		"Utility/HFSIdentifier.h"
		"Utility/RectAllocator.h"
		"Utility/RectAllocator.cpp"
		"Utility/RXMLParser.h"
		"Utility/RXMLParser.cpp"
		"Utility/XMLTypes.h"
		"Utility/XMLTypes.cpp"
		)

if( "${IC3_COMPONENTS_BUILD_MODE}" STREQUAL "STATIC" )
    add_library( Ic3.CoreLib STATIC ${IC3_CORELIB_SRC_FILES} )
    target_compile_definitions( Ic3.CoreLib PRIVATE
			"IC3_BUILD_STATIC=1" )
else()
    add_library( Ic3.CoreLib SHARED ${IC3_CORELIB_SRC_FILES} )
endif()

ebsAppendInternal(IC3_MODULE_TARGET_LIST "Ic3.CoreLib")

set_target_properties( Ic3.CoreLib PROPERTIES
		OUTPUT_NAME "Ic3.CoreLib"
		)

target_compile_definitions( Ic3.CoreLib PRIVATE
		"IC3_CORELIB_BUILD=1"
		)

target_include_directories( Ic3.CoreLib PUBLIC
		"${IC3_INCLUDE_DIR}"
		)

target_link_libraries( Ic3.CoreLib PUBLIC
		Ic3.Platform
		Ic3.Cppx
		)
