
set( IC3_CORELIB_SRC_FILES
	"Prerequisites/CoreDefs.h"
	"Prerequisites/CoreEnums.h"
	"Prerequisites/DebugLayer.h"
	"Prerequisites/DynamicInterface.h"
	"Prerequisites/DynamicInterface.cpp"
	"Prerequisites/ResultCode.h"
	"Prerequisites/SharedHandle.h"

	"Prerequisites.h"
	"Prerequisites.cpp"

	"Exception.h"
	"Exception.cpp"
	"ExceptionCommon.h"
	"ReturnValueObject.h"
	"ExceptionUtils.h"
	"PixelCommon.h"
	"MathImports.h"

	"Memory/CommonMemoryDefs.h"

	"Reflection/EnumTypeInfo.h"
	"Reflection/EnumTypeInfo.cpp"

	"Threading/CommonSyncDefs.h"
	"Threading/Lockable.h"
	"Threading/MutexCommon.h"

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

ic3AddCoreLib( Ic3.CoreLib ${IC3_CORELIB_SRC_FILES} )
