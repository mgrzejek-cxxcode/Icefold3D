
set( IC3_CPPX_SRC_FILES
        "Prerequisites.h"
        "Aligned.h"
        "ArrayIterator.h"
        "ArrayView.h"
        "Bitmask.h"
        "BitmaskAtomic.h"
        "ByteArray.h"
        "Chrono.h"
        "FlatMap.h"
        "FsUtils.h"
        "FsUtils.cpp"
        "Hash.h"
        "Hash.cpp"
        "IntrusivePtr.h"
        "Memory.h"
        "Memory.cpp"
        "MemoryBuffer.h"
        "PathNameIterator.h"
        "Range.h"
        "RefWrapper.h"
        "SortedArray.h"
        "StaticAlgo.h"
        "StaticLimits.h"
        "StaticVector.h"
        "StringExt.h"
        "StringUtils.h"
        "StringView.h"
        "TypeTraits.h"
        "Utilities.h"
        "Variant.h"
)

add_library( Ic3.Cppx STATIC
        ${IC3_CPPX_SRC_FILES}
)

ebsAppendInternal(IC3_MODULE_TARGET_LIST "Ic3.Cppx")

set_target_properties( Ic3.Cppx PROPERTIES
        OUTPUT_NAME "Ic3.Cppx"
        )

target_include_directories( Ic3.Cppx PUBLIC
        "${IC3_INCLUDE_DIR}"
        )

target_link_libraries( Ic3.Cppx PUBLIC
        ExtLib.Zlib
        Ic3.Platform
        )
        