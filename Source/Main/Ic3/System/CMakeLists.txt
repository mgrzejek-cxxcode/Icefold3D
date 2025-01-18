
set( IC3_SYSTEM_SRC_FILES
        "AssetCommon.h"
        "AssetSystem.h"
        "AssetSystem.cpp"
        "AssetSystemNative.h"
        "DisplayCommon.h"
        "DisplayCommon.cpp"
        "DisplayConfiguration.h"
        "DisplayConfiguration.cpp"
        "DisplaySystem.h"
        "DisplaySystem.cpp"
        "DisplaySystemNative.h"
        "DynamicLib.h"
        "DynamicLib.cpp"
        "EventCommon.h"
        "EventCore.h"
        "EventCore.cpp"
        "EventDefApp.h"
        "EventDefInputKeyboard.h"
        "EventDefInputMouse.h"
        "EventDefInputTouch.h"
        "EventObject.h"
        "EventSource.h"
        "EventSource.cpp"
        "EventCoreNative.h"
        "FileCommon.h"
        "FileCommon.cpp"
        "FileManager.h"
        "FileManager.cpp"
        "Frame.h"
        "Frame.cpp"
        "OpenGLCommon.h"
        "OpenGLCommon.cpp"
        "OpenGLDriver.h"
        "OpenGLDriver.cpp"
        "OpenGLNative.h"
        "Visual.h"
        "Visual.cpp"
        "PerfCounter.h"
        "PerfCounter.cpp"
        "Prerequisites.h"
        "SysContext.h"
        "SysContext.cpp"
        "SysObject.h"
        "SysObject.cpp"
        "WindowCommon.h"
        "WindowSystem.h"
        "WindowSystem.cpp"
        "WindowNative.h"

        "internal/displaySystemPrivate.h"
        "internal/EventCorePrivate.h"
        )

if( ${CM_EBS_SYSCONF_TARGET_FLAG_OSINFO_MSE} )
    set( IC3_SYSTEM_SRC_FILES
            ${IC3_SYSTEM_SRC_FILES}
            "Internal/Platform/Shared/DXGI/DXGIDisplaySystem.h"
            "Internal/Platform/Shared/DXGI/DXGIDisplaySystem.cpp"
            "Internal/Platform/Shared/MSE/MSECommon.h"
            "Internal/Platform/Shared/MSE/MSECommon.cpp"
            "Internal/Platform/Shared/MSE/MSEPerfCounter.h"
            "Internal/Platform/Shared/MSE/MSEPerfCounter.cpp"
            )
endif()

if( ${CM_EBS_SYSCONF_TARGET_FLAG_OSINFO_APPLE} )
    set( IC3_SYSTEM_SRC_FILES
            ${IC3_SYSTEM_SRC_FILES}
            "metalCommon.h"
            "metalCommon.mm"
            "metalDriver.h"
            "metalDriver.mm"
            "metalNative.h"
            "Internal/Platform/Shared/MTL/MTLCommon.h"
            "Internal/Platform/Shared/MTL/MTLCommon.mm"
            )
endif()

if( ${CM_EBS_SYSCONF_TARGET_FLAG_OSINFO_POSIX} )
    set( IC3_SYSTEM_SRC_FILES
            ${IC3_SYSTEM_SRC_FILES}
            "Internal/Platform/Shared/POSIX/POSIXCommon.h"
            "Internal/Platform/Shared/POSIX/POSIXCommon.cpp"
            "Internal/Platform/Shared/POSIX/POSIXPerfCounter.h"
            "Internal/Platform/Shared/POSIX/POSIXPerfCounter.cpp"
            "Internal/Platform/Shared/POSIX/POSIXFileManager.h"
            "Internal/Platform/Shared/POSIX/POSIXFileManager.cpp"
            )
endif()

if( "${CM_EBS_SYSCONF_TARGET_FLAG_OSINFO_GLAPI}" STREQUAL "EGL" )
    set( IC3_SYSTEM_SRC_FILES
            ${IC3_SYSTEM_SRC_FILES}
            "Internal/Platform/Shared/EGL/EGLCoreUtils.h"
            "Internal/Platform/Shared/EGL/EGLCoreUtils.cpp"
            "Internal/Platform/Shared/EGL/EGLOpenGL.h"
            "Internal/Platform/Shared/EGL/EGLOpenGL.cpp"
            )
endif()

if( "${CM_EBS_SYSCONF_TARGET_SYSTEM}" STREQUAL "Android" )
    set( IC3_SYSTEM_SRC_FILES
            ${IC3_SYSTEM_SRC_FILES}
            "Internal/Platform/OSAPI/Android/AndroidAssetSystem.h"
            "Internal/Platform/OSAPI/Android/AndroidAssetSystem.cpp"
            "Internal/Platform/OSAPI/Android/AndroidCommon.h"
            "Internal/Platform/OSAPI/Android/AndroidCommon.cpp"
            "Internal/Platform/OSAPI/Android/AndroidDisplaySystem.h"
            "Internal/Platform/OSAPI/Android/AndroidDisplaySystem.cpp"
            "Internal/Platform/OSAPI/Android/AndroidEventCore.h"
            "Internal/Platform/OSAPI/Android/AndroidEventCore.cpp"
            "Internal/Platform/OSAPI/Android/AndroidFileManager.h"
            "Internal/Platform/OSAPI/Android/AndroidOpenGLDriver.h"
            "Internal/Platform/OSAPI/Android/AndroidOpenGLDriver.cpp"
            "Internal/Platform/OSAPI/Android/AndroidSysContext.h"
            "Internal/Platform/OSAPI/Android/AndroidSysContext.cpp"
            "Internal/Platform/OSAPI/Android/AndroidWindowSystem.h"
            "Internal/Platform/OSAPI/Android/AndroidWindowSystem.cpp"
            "Internal/Platform/Shared/Java/JavaCommon.h"
            "Internal/Platform/Shared/Java/JavaNativeInterface.h"
            "Internal/Platform/Shared/Java/JavaNativeInterface.cpp"
            "Internal/Platform/Shared/Java/JavaVM.h"
            "Internal/Platform/Shared/Java/JavaVM.cpp"
            )
    set( CM_EBS_SYSTEM_ASSET_SYSTEM_SRC_FLAG TRUE )
endif()

if( "${CM_EBS_SYSCONF_TARGET_SYSTEM}" STREQUAL "WinDesktop" )
    set( IC3_SYSTEM_SRC_FILES
            ${IC3_SYSTEM_SRC_FILES}
            "Internal/Platform/OSAPI/Win32/Win32AssetSystem.h"
            "Internal/Platform/OSAPI/Win32/Win32Common.h"
            "Internal/Platform/OSAPI/Win32/Win32Common.cpp"
            "Internal/Platform/OSAPI/Win32/Win32DisplaySystem.h"
            "Internal/Platform/OSAPI/Win32/Win32DisplaySystem.cpp"
            "Internal/Platform/OSAPI/Win32/Win32EventCore.h"
            "Internal/Platform/OSAPI/Win32/Win32EventCore.cpp"
            "Internal/Platform/OSAPI/Win32/Win32FileAPI.h"
            "Internal/Platform/OSAPI/Win32/Win32FileAPI.cpp"
            "Internal/Platform/OSAPI/Win32/Win32FileManager.h"
            "Internal/Platform/OSAPI/Win32/Win32FileManager.cpp"
            "Internal/Platform/OSAPI/Win32/Win32OpenGLDriver.h"
            "Internal/Platform/OSAPI/Win32/Win32OpenGLDriver.cpp"
            "Internal/Platform/OSAPI/Win32/Win32SysContext.h"
            "Internal/Platform/OSAPI/Win32/Win32SysContext.cpp"
            "Internal/Platform/OSAPI/Win32/Win32WindowSystem.h"
            "Internal/Platform/OSAPI/Win32/Win32WindowSystem.cpp"
            )
    set( CM_EBS_SYSTEM_ASSET_SYSTEM_SRC_FLAG FALSE )
endif()

if( "${CM_EBS_SYSCONF_TARGET_SYSTEM}" STREQUAL "Linux" )
    set( IC3_SYSTEM_SRC_FILES
            ${IC3_SYSTEM_SRC_FILES}
            "Internal/Platform/OSAPI/X11/X11AssetSystem.h"
            "Internal/Platform/OSAPI/X11/X11Common.h"
            "Internal/Platform/OSAPI/X11/X11Common.cpp"
            "Internal/Platform/OSAPI/X11/X11DisplaySystem.h"
            "Internal/Platform/OSAPI/X11/X11DisplaySystem.cpp"
            "Internal/Platform/OSAPI/X11/X11EventCore.h"
            "Internal/Platform/OSAPI/X11/X11EventCore.cpp"
            "Internal/Platform/OSAPI/X11/X11FileManager.h"
            "Internal/Platform/OSAPI/X11/X11OpenGLDriver.h"
            "Internal/Platform/OSAPI/X11/X11OpenGLDriver.cpp"
            "Internal/Platform/OSAPI/X11/X11SysContext.h"
            "Internal/Platform/OSAPI/X11/X11SysContext.cpp"
            "Internal/Platform/OSAPI/X11/X11WindowSystem.h"
            "Internal/Platform/OSAPI/X11/X11WindowSystem.cpp"
            )
    set( CM_EBS_SYSTEM_ASSET_SYSTEM_SRC_FLAG FALSE )
endif()

if( "${CM_EBS_SYSCONF_TARGET_SYSTEM}" STREQUAL "macOS" )
    set( IC3_SYSTEM_SRC_FILES
            ${IC3_SYSTEM_SRC_FILES}
            "Internal/Platform/OSAPI/OSX/NSIApplicationProxy.h"
            "Internal/Platform/OSAPI/OSX/NSIApplicationProxy.mm"
            "Internal/Platform/OSAPI/OSX/NSIKeyboardSupport.h"
            "Internal/Platform/OSAPI/OSX/NSIKeyboardSupport.mm"
            "Internal/Platform/OSAPI/OSX/NSIEventListener.h"
            "Internal/Platform/OSAPI/OSX/NSIEventListener.mm"
            "Internal/Platform/OSAPI/OSX/NSIMetalSupport.h"
            "Internal/Platform/OSAPI/OSX/NSIMetalSupport.mm"
            "Internal/Platform/OSAPI/OSX/NSIOpenGL.h"
            "Internal/Platform/OSAPI/OSX/NSIOpenGL.mm"
            "Internal/Platform/OSAPI/OSX/NSIWindow.h"
            "Internal/Platform/OSAPI/OSX/NSIWindow.mm"
            "Internal/Platform/OSAPI/OSX/OSXAssetSystem.h"
            "Internal/Platform/OSAPI/OSX/OSXCommon.h"
            "Internal/Platform/OSAPI/OSX/OSXCommon.mm"
            "Internal/Platform/OSAPI/OSX/OSXDisplaySystem.h"
            "Internal/Platform/OSAPI/OSX/OSXDisplaySystem.mm"
            "Internal/Platform/OSAPI/OSX/OSXEventCore.h"
            "Internal/Platform/OSAPI/OSX/OSXEventCore.mm"
            "Internal/Platform/OSAPI/OSX/OSXMetalDriver.h"
            "Internal/Platform/OSAPI/OSX/OSXMetalDriver.mm"
            "Internal/Platform/OSAPI/OSX/OSXOpenGLDriver.h"
            "Internal/Platform/OSAPI/OSX/OSXOpenGLDriver.mm"
            "Internal/Platform/OSAPI/OSX/OSXSysContext.h"
            "Internal/Platform/OSAPI/OSX/OSXSysContext.mm"
            "Internal/Platform/OSAPI/OSX/OSXWindowSystem.h"
            "Internal/Platform/OSAPI/OSX/OSXWindowSystem.mm"
            #"Internal/Platform/OSAPI/OSX/OSXEventCore.h"
            #"Internal/Platform/OSAPI/OSX/OSXEventCore.cpp"
            #"Internal/Platform/OSAPI/OSX/OSXFileManager.h"
            #"Internal/Platform/OSAPI/OSX/OSXSysContext.h"
            #"Internal/Platform/OSAPI/OSX/OSXSysContext.cpp"
            )
    set( CM_EBS_SYSTEM_ASSET_SYSTEM_SRC_FLAG FALSE )
endif()

if( NOT "${CM_EBS_SYSTEM_ASSET_SYSTEM_SRC_FLAG}" )
    set( IC3_SYSTEM_SRC_FILES
            ${IC3_SYSTEM_SRC_FILES}
            "Internal/Platform/Common/FileBasedAssetSystem.h"
            "Internal/Platform/Common/FileBasedAssetSystem.cpp"
            )
endif()

if( "${IC3_COMPONENTS_BUILD_MODE}" STREQUAL "STATIC" )
    add_library( Ic3.System STATIC ${IC3_SYSTEM_SRC_FILES} )
    target_compile_definitions( Ic3.System PRIVATE
            "IC3_BUILD_STATIC=1" )
else()
    add_library( Ic3.System SHARED ${IC3_SYSTEM_SRC_FILES} )
endif()

set_target_properties( Ic3.System PROPERTIES
        OUTPUT_NAME "Ic3.System"
        )

target_include_directories( Ic3.System PUBLIC
        ${IC3_INCLUDE_DIR}
        )

target_compile_definitions( Ic3.System PUBLIC
        IC3_SYSTEM_BUILD=1
        GLEW_STATIC=1
        GLEW_NO_GLU=1
        )

target_link_libraries( Ic3.System PUBLIC
        Ic3.CoreLib
        )

if( "${CM_EBS_SYSCONF_TARGET_SYSTEM}" STREQUAL "WinDesktop" )
    target_link_libraries( Ic3.System PUBLIC
            ext-glew
            opengl32.lib
            DXGI.lib
            )
endif()

if( "${CM_EBS_SYSCONF_TARGET_SYSTEM}" STREQUAL "Android" )
    target_link_libraries( Ic3.System PUBLIC
            Android
            log
            EGL
            GLESv2
            )
endif()

if( "${CM_EBS_SYSCONF_TARGET_SYSTEM}" STREQUAL "Linux" )
    target_link_libraries( Ic3.System PUBLIC
            ext-glew
            X11
            Xrandr
            GL
            GLX
            )
endif()

find_library(FrmLib_AppKit AppKit)
find_library(FrmLib_CoreFoundation CoreFoundation)
find_library(FrmLib_Foundation Foundation)
find_library(FrmLib_Metal Metal)
find_library(FrmLib_OpenGL OpenGL)
find_library(FrmLib_QuartzCore QuartzCore)

if( "${CM_EBS_SYSCONF_TARGET_SYSTEM}" STREQUAL "macOS" )
    target_link_libraries( Ic3.System PUBLIC
            ext-glew
            ${FrmLib_AppKit}
            ${FrmLib_CoreFoundation}
            ${FrmLib_Foundation}
            ${FrmLib_Metal}
            ${FrmLib_OpenGL}
            ${FrmLib_QuartzCore}
            )
endif()

if( ${CM_EBS_SYSCONF_TARGET_FLAG_OSINFO_VULKAN} )
    target_link_libraries( Ic3.System PUBLIC
            #shaderc
            #shaderc_util
            #vulkan-1
            )
endif()
