
set( IC3_GRAPHICS_HW3D_VK10_SRC_FILES
    ${IC3_GRAPHICS_HW3D_VKCOMMON_SRC_FILES}

    "VK10Prerequisites.h"
    "VK10Prerequisites.cpp"

    "core/VK10UDriverAPI.h"
    "core/VK10UDriverAPI.cpp"
)

ic3AddCoreLib( Ic3.Graphics.HW3D.VK13 STATIC ${IC3_GRAPHICS_HW3D_VK10_SRC_FILES} )

target_include_directories( Ic3.Graphics.HW3D.VK13 PUBLIC
        ${IC3_GRAPHICS_HW3D_INCLUDE_DIR_INTERNAL} )

target_link_libraries( Ic3.Graphics.HW3D.VK13 PUBLIC
        Ic3.Graphics.GCI
        Ic3.System )
