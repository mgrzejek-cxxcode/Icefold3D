
set( IC3_GRAPHICS_HW3D_DX12_SRC_FILES
    ${IC3_GRAPHICS_HW3D_DX_SRC_FILES}

    "Prerequisites/DX12_coreDefs.h"

    "DX12Prerequisites.h"
    "DX12Prerequisites.cpp"

    "DX12_coreAPIProxy.h"
    "DX12_coreAPIProxy.cpp"
    "DX12UCmdContext.h"
    "DX12UCmdContext.cpp"
    "DX12UCmdManager.h"
    "DX12UCmdManager.cpp"
    "DX12UDevice.h"
    "DX12UDevice.cpp"
    "DX12UDriver.h"
    "DX12UDriver.cpp"
    "DX12UDriverAPI.h"
    "DX12UDriverAPI.cpp"
    "DX12_presentationLayer.h"
    "DX12_presentationLayer.cpp"
)

ic3AddCoreLib( Ic3.Graphics.HW3D.DX12 ${IC3_GRAPHICS_HW3D_DX12_SRC_FILES} )

target_include_directories (Ic3.Graphics.HW3D.DX12 PUBLIC
        ${IC3_GRAPHICS_HW3D_INCLUDE_DIR_INTERNAL})

target_link_libraries( Ic3.Graphics.HW3D.DX12 PUBLIC
        Ic3.Graphics.GCI
        D3D12.lib )
