
set( IC3_GRAPHICS_HW3D_INCLUDE_DIR
        "${IC3_INCLUDE_DIR}" )
		
set( IC3_GRAPHICS_HW3D_INCLUDE_DIR_INTERNAL
        ${IC3_GRAPHICS_HW3D_INCLUDE_DIR}
        "${IC3_BASE_DIR}/Source/Main" )

if( "${CM_EBS_SYSCONF_TARGET_SYSTEM}" STREQUAL "Android" )
    set( _IC3_GRAPHICS_HW3D_TARGET_LIST
            "GL"
            "GLES3"
            )
elseif( "${CM_EBS_SYSCONF_TARGET_SYSTEM}" STREQUAL "Linux" )
    set( _IC3_GRAPHICS_HW3D_TARGET_LIST
            "GL"
            "GL4"
            #"VK"
            #"VK11"
            )
elseif( "${CM_EBS_SYSCONF_TARGET_SYSTEM}" STREQUAL "macOS" )
    set( _IC3_GRAPHICS_HW3D_TARGET_LIST
            "GL"
            "GL4"
            #"MTL"
            #"MTL2"
            )
elseif( "${CM_EBS_SYSCONF_TARGET_SYSTEM}" STREQUAL "iOS" )
    set( _IC3_GRAPHICS_HW3D_TARGET_LIST
            # "MTL"
            # "MTL2"
            )
elseif( "${CM_EBS_SYSCONF_TARGET_SYSTEM}" STREQUAL "WinDesktop" )
    set( _IC3_GRAPHICS_HW3D_TARGET_LIST
            "GL"
            "GL4"
            "DX"
            #"DX11"
            #"DX12"
            )
elseif( "${CM_EBS_SYSCONF_TARGET_FLAG_OSINFO_WFA}" )
    set( _IC3_GRAPHICS_HW3D_TARGET_LIST
            "DX"
            "DX11"
            #"DX12"
            )
endif()

ebsSetInternal( CM_EBS_IC3_GRAPHICS_HW3D_TARGET_LIST "${_IC3_GRAPHICS_HW3D_TARGET_LIST}" )

foreach( target ${_IC3_GRAPHICS_HW3D_TARGET_LIST} )
    add_subdirectory( "${target}" )
    if( TARGET "Ic3.Graphics.HW3D.${target}" )
        ebsAppendInternal( IC3_GRAPHICS_HW3D_ALL_AVAILABLE_TARGETS "Ic3.Graphics.HW3D.${target}" )
    endif()
endforeach()
