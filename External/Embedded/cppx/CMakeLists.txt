
set( CPPX_SRC_FILES
        "platform/internal/environment.h"
        "platform/internal/include.h"
        "platform/internal/types.h"
        "platform/internal/simdExt.h"

        "platform/internal/environment/arch/arm.h"
        "platform/internal/environment/arch/arm64.h"
        "platform/internal/environment/arch/x86.h"
        "platform/internal/environment/arch/x86-64.h"
        "platform/internal/environment/compiler/clang.h"
        "platform/internal/environment/compiler/gcc.h"
        "platform/internal/environment/compiler/icc.h"
        "platform/internal/environment/compiler/msvc.h"
        "platform/internal/environment/os/android.h"
        "platform/internal/environment/os/ios.h"
        "platform/internal/environment/os/linux.h"
        "platform/internal/environment/os/osx.h"
        "platform/internal/environment/os/winDesktop.h"
        "platform/internal/environment/os/winUWP.h"

        "platform/platform.h"
        "platform/platform.cpp"
        "platform/debug.h"
        "platform/debug.cpp"
        "platform/fileLocationInfo.h"
        "platform/fileLocationInfo.cpp"

        "prerequisites.h"

        "aligned.h"
        "arrayIterator.h"
        "arrayView.h"
        "bitmask.h"
        "bitmaskAtomic.h"
        "byteArray.h"
        "chrono.h"
        "flatMap.h"
        "fsUtils.h"
        "fsUtils.cpp"
        "hash.h"
        "hash.cpp"
        "immutableString.h"
        "intrusivePtr.h"
        "memory.h"
        "memory.cpp"
        "memoryBuffer.h"
        "namedObjectArray.h"
        "pathNameIterator.h"
        "range.h"
        "refCounter.h"
        "refWrapper.h"
        "sortedArray.h"
        "stdHelperAlgo.h"
        "staticAlgo.h"
        "staticVector.h"
        "stringExt.h"
        "stringUtils.h"
        "stringView.h"
        "typeLimits.h"
        "typeTraits.h"
        "utilities.h"
        "valueLimits.h"
        "variant.h"

        "sync/atomicRWFlag.h"
        "sync/atomicRWLock.h"
        "sync/commonSyncDefs.h"
        "sync/commonSyncDefs.cpp"
        "sync/spinLock.h"
)

if( "${CM_EBS_SYSCONF_TARGET_SYSTEM}" STREQUAL "Android" )
        set( CPPX_SRC_FILES
                ${CPPX_SRC_FILES}
                "platform/android/androidNativeAppProxy.h"
                "platform/android/androidNativeAppProxy.cpp"
        )
endif()

add_library( ext-cppx STATIC
        ${CPPX_SRC_FILES}
)

set_property( TARGET ext-cppx PROPERTY CXX_STANDARD 17 )
target_compile_features( ext-cppx PUBLIC cxx_std_17 )

set_target_properties( ext-cppx PROPERTIES
        OUTPUT_NAME ext-cppx
)

target_include_directories( ext-cppx PUBLIC
        "${EXT_INCLUDE_DIR}"
)

target_link_libraries( ext-cppx PUBLIC
        ext-zlib
)
        