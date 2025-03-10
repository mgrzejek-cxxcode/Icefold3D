
#ifndef __IC3_SYSTEM_IO_COMMON_DEFS_H__
#define __IC3_SYSTEM_IO_COMMON_DEFS_H__

#include "../Prerequisites.h"
#include <cppx/chrono.h>

namespace Ic3::System
{

    class File;
    class ReadPipe;
    class WritePipe;

    Ic3SysDeclareHandle( File );
    Ic3SysDeclareHandle( FileManager );
    Ic3SysDeclareHandle( PipeFactory );
    Ic3SysDeclareHandle( ReadPipe );
    Ic3SysDeclareHandle( WritePipe );

    using file_char_t = char;
    using io_offset_t = native_int;
    using io_size_t = native_uint;

    using FileList = std::vector<FileHandle>;
    using FileNameList = std::vector<std::string>;

    inline constexpr auto kIOSizeMax = cppx::meta::limits<io_size_t>::max_value;

    enum : exception_code_value_t
    {
        ///
        eExcCodeSystemIOError = Ic3::CXU::DeclareExceptionCode( eExceptionCategorySystemIO, 1 ),
        ///
        eExcCodeSystemIOBadAccess = Ic3::CXU::DeclareExceptionCode( eExceptionCategorySystemIO, 2 ),
    };

    /**
     *
     */
    enum class EIOPointerRefPos : enum_default_value_t
    {
        CurrentPos,
        StreamBase,
        StreamEnd,
    };

    enum EIOAccessFlags : enum_default_value_t
    {
        eIOAccessFlagOpRead = 0x0001,
        eIOAccessFlagOpWrite = 0x0002,
        eIOAccessFlagWriteModeAppend = 0x0010,
        eIOAccessFlagWriteModeOverwrite = 0x0020,
    };

    /**
     *
     */
    enum class EIOAccessMode : enum_default_value_t
    {
        Undefined,

        // Open file for reading. If the specified file does not exist, an error is reported.
        // - File pointer is set to the <b>beginning</b>.
        // - Existing content is <b>preserved</b>.
        ReadOnly = eIOAccessFlagOpRead,

        /// Open file for reading and writing. If the specified file does not exist, a new one is created.
        /// - File pointer is set to the <b>beginning</b>.
        /// - Existing content is <b>preserved</b>.
        ReadWrite = eIOAccessFlagOpRead | eIOAccessFlagOpWrite,

        /// Open file for writing. If the specified file does not exist, a new one is created.
        /// - File pointer is set to the <b>end</b>.
        /// - Existing content is <b>preserved</b>.
        WriteAppend = eIOAccessFlagOpWrite | eIOAccessFlagWriteModeAppend,

        /// Open file for writing. If the specified file does not exist, a new one is created.
        /// - File pointer is set to the <b>beginning</b>.
        /// - Existing content is <b>erased</b>.
        WriteOverwrite = eIOAccessFlagOpWrite | eIOAccessFlagWriteModeOverwrite,
    };

    enum class EPipeDataMode : enum_default_value_t
    {
        ByteStream = 1,
        MessageStream
    };

    struct IOTimeoutSettings
    {
        cppx::milliseconds waitTimeout;

        cppx::milliseconds yieldTimeBetweenRetries;
    };

    inline constexpr auto kIODefaultTimeoutSettings = IOTimeoutSettings{ cppx::milliseconds( 500 ), cppx::milliseconds( 50 ) };

    /// @brief
    class SystemIOException : public SystemException
    {
    public:
        std::string mFilePath;

    public:
        explicit SystemIOException( ExceptionInfo pInfo, std::string pFilePath = {} )
        : SystemException( std::move( pInfo ) )
        , mFilePath( std::move( pFilePath ) )
        {}
    };

    Ic3SetExceptionCategoryType( eExceptionCategorySystemIO, SystemIOException );

} // namespace Ic3::System

#endif // __IC3_SYSTEM_IO_COMMON_DEFS_H__
