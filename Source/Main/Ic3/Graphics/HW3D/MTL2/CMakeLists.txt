
set( IC3_GRAPHICS_HW3D_MTL2_SRC_FILES
        ${IC3_GRAPHICS_HW3D_MTLCOMMON_SRC_FILES}

        "MTL2Prerequisites.h"
        "MTL2GPUDevice.h"
        "MTL2GPUDevice.mm"
        "MTL2GPUDriver.h"
        "MTL2GPUDriver.mm"
        "MTL2GPUDriverAPI.h"
        "MTL2GPUDriverAPI.mm"
        )

ic3AddCoreLib( Ic3.Graphics.HW3D.MTL2 STATIC ${IC3_GRAPHICS_HW3D_MTL2_SRC_FILES} )

target_include_directories( Ic3.Graphics.HW3D.MTL2 PUBLIC
        ${IC3_GRAPHICS_HW3D_INCLUDE_DIR_INTERNAL} )

target_link_libraries( Ic3.Graphics.HW3D.MTL2 PUBLIC
        Ic3.Graphics.GCI
        Ic3.System )
