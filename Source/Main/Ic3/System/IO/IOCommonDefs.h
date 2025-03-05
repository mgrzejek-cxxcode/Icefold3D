
#ifndef __IC3_SYSTEM_IO_COMMON_DEFS_H__
#define __IC3_SYSTEM_IO_COMMON_DEFS_H__

#include "../Prerequisites.h"

namespace Ic3::System
{

    using io_offset_t = native_int;
    using io_size_t = native_uint;

	inline constexpr auto kIOSizeMax = cppx::meta::limits<io_size_t>::max_value;

    /// @brief
    enum class EIOPointerRefPos : enum_default_value_t
    {
        CurrentPos,
        StreamBase,
        StreamEnd,
    };

    /// @brief
    enum class EIOAccessMode : enum_default_value_t
    {
        // Open file for reading. If the specified file does not exist, an error is reported.
        // - File pointer is set to a BEGINNING.
        // - Existing content is PRESERVED.
        ReadOnly,

        // Open file for reading and writing. If the specified file does not exist, a new one is created.
        // - File pointer is set to a BEGINNING.
        // - Existing content is PRESERVED.
        ReadWrite,

        // Open file for writing. If the specified file does not exist, a new one is created.
        // - File pointer is set to an END.
        // - Existing content is PRESERVED.
        WriteAppend,

        // Open file for writing. If the specified file does not exist, a new one is created.
        // - File pointer is set to a BEGINNING.
        // - Existing content is ERASED.
        WriteOverwrite
    };

} // namespace Ic3::System

#endif // __IC3_SYSTEM_IO_COMMON_DEFS_H__
