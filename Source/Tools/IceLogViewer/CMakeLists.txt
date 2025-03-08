
set( IC3_TOOLS_SRC_IceLogViewer
    "Application.h"
    "Application.cpp"
    "UI/MainWindow.h"
    "UI/MainWindow.cpp"
)

if( "${CM_EBS_SYSCONF_TARGET_SYSTEM}" STREQUAL "macOS" )
    set( IC3_TOOLS_SRC_IceLogViewer
        ${IC3_TOOLS_SRC_IceLogViewer}
        "Main.cpp"
    )
else()
    set( IC3_TOOLS_SRC_IceLogViewer
        ${IC3_TOOLS_SRC_IceLogViewer}
        "Main.cpp"
    )
endif()

add_executable( Tool.IceLogViewer WIN32 MACOSX_BUNDLE
    ${IC3_TOOLS_SRC_IceLogViewer}
)

target_link_libraries( Tool.IceLogViewer PRIVATE
    "${IC3_TOOLS_LINK_INPUT}"
    Ic3.System
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)

target_compile_definitions( Tool.IceLogViewer PRIVATE
    "${IC3_COMMON_MODULE_DEFINITIONS}"
)
