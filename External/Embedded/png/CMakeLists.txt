
set( IC3_EXTLIB_png_SCR_FILES
        png.h
        png.c
        pngconf.h
        pngdebug.h
        pngerror.c
        pngget.c
        pnginfo.h
        pnglibconf.h
        pngmem.c
        pngpread.c
        pngpriv.h
        pngread.c
        pngrio.c
        pngrtran.c
        pngrutil.c
        pngset.c
        pngstruct.h
        # pngtest.c
        pngtrans.c
        pngwio.c
        pngwrite.c
        pngwtran.c
        pngwutil.c
        )

if( "${CM_EBS_SYSCONF_TARGET_ARCH}" MATCHES "arm64|arm" )
    set( IC3_EXTLIB_png_SCR_FILES
            ${IC3_EXTLIB_png_SCR_FILES}
            "arm/arm_init.c"
            "arm/filter_neon_intrinsics.c"
            "arm/palette_neon_intrinsics.c"
            )
endif()

add_library( ext-png STATIC
        ${IC3_EXTLIB_png_SCR_FILES} )

set_target_properties( ext-png PROPERTIES
        OUTPUT_NAME "png" )

target_include_directories( ext-png PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR} )

target_compile_definitions( ext-png PRIVATE
        _CRT_SECURE_NO_WARNINGS=1
        _CRT_SECURE_NO_DEPRECATED=1
)

if( "${CM_EBS_SYSCONF_HOST_COMPILER}" MATCHES "Clang|GCC" )
    target_compile_options( ext-png PRIVATE
            -Wno-implicit-function-declaration
    )
endif()
