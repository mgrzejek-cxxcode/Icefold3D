
set( IC3_GRAPHICS_HWAPI_INCLUDE_DIR
        "${IC3_INCLUDE_DIR}" )
		
set( IC3_DRIVERS_INCLUDE_DIR_INTERNAL
        ${IC3_DRIVERS_INCLUDE_DIR}
        "${IC3_BASE_DIR}/source/engine/drivers" )

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
            #"DX"
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
#    if( TARGET "ts3drv-${target}" )
#        set_property( TARGET "ts3drv-${target}" PROPERTY CXX_STANDARD 17 )
#        target_compile_features( "ts3drv-${target}" PRIVATE cxx_std_17 )
#    endif()
endforeach()
