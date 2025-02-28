
set( IC3_ENGINE_SRC_FILES
		"Prerequisites/CoreEngineState.h"
		"Prerequisites/GCICommon.h"
		"Prerequisites/GeometryCommon.h"

        "Prerequisites.h"
        "Prerequisites.cpp"
		"Exception.h"
		"Exception.cpp"

        "Camera/CameraCommonDefs.h"
        "Camera/CameraController.h"
        "Camera/CameraController.cpp"

        "GCI/CommonGCIDefs.h"
        "GCI/ConstantBufferProxy.h"
		# "GCI/GCRBufferCommon.h"
		# "GCI/GCRBuffer.h"
		# "GCI/GCRBuffer.cpp"
		"GCI/IACommonDefs.h"
		"GCI/IAVertexStreamLayout.h"
		"GCI/IAVertexStreamLayout.inl"
		"GCI/IAVertexStreamLayout.cpp"
		"GCI/IAVertexAttribLayout.h"
		"GCI/IAVertexAttribLayout.inl"
		"GCI/IAVertexAttribLayout.cpp"
		"GCI/ShaderInputSemantics.h"
		"GCI/ShaderInputSemantics.cpp"
		"GCI/ShaderResourceDefs.h"
		"GCI/VertexAttributeCommonDefs.h"
        "GCI/VertexFormatDescriptor.h"
		"GCI/VertexFormatDescriptor.inl"
		"GCI/VertexFormatDescriptor.cpp"
		"GCI/VertexFormatDescriptorUtils.h"
		"GCI/VertexFormatDescriptorUtils.cpp"

		#"Geometry/VertexPipelineConfig.h"
		#"Geometry/VertexPipelineConfig.cpp"
		#"Geometry/VertexDataSystem.h"
		#"Geometry/VertexDataSystem.cpp"

		#"Renderer/CommonRendererDefs.h"
		#"Renderer/Renderer.h"
		#"Renderer/Renderer.cpp"
		"Renderer/ShaderLibrary.h"
		"Renderer/ShaderLibrary.cpp"
		"Renderer/ShaderLoader.h"
		"Renderer/ShaderLoader.cpp"
		#"Renderer/Effects/ShadowRenderer.h"
		#"Renderer/Effects/ShadowRenderer.cpp"

		"Res/Font.h"
		"Res/Font.cpp"
		"Res/FontCommon.h"
		"Res/FontMetrics.h"
		"Res/Font/FontTypeDynamic.h"
		"Res/Font/FontTypeDynamic.cpp"
		"Res/Font/FontTypeStatic.h"
		"Res/Font/FontTypeStatic.cpp"
		"Res/Font/FTDFreeTypeCommon.h"
		"Res/Font/FTDFreeTypeFontFace.h"
		"Res/Font/FTDFreeTypeFontFace.cpp"
		"Res/Font/FTDFreeTypeFontObject.h"
		"Res/Font/FTDFreeTypeFontObject.cpp"
		"Res/ImageCommon.h"
		"Res/ImageCommon.cpp"
		"Res/Image/BitmapCommon.h"
		"Res/Image/BitmapCommon.cpp"
		"Res/Image/PngCommon.cpp"
		"Res/Image/PngCommon.h"
		"Res/Resource.h"
		"Res/Resource.cpp"
		"Res/ResourceLoader.h"
		"Res/ResourceLoader.cpp"
		"Res/ResourceManager.h"
		"Res/ResourceManager.cpp"

        "TypeInfo/TPIEngineEnum.cpp"
		GCI/ShaderUtils.h
		GCI/ShaderUtils.cpp

		#"Geometry/IndexedMeshDataBufferBuilder.h"
		#"Geometry/MeshData.h"
		#"Geometry/MeshData.cpp"
		#"Geometry/MeshGroup.h"
		#
		#"Geometry/DataTypesConv.h"
        #"Geometry/GeometryStorageGPU.h"
		#"Geometry/GeometryStorageGPU.cpp"
        #"Geometry/GeometryStorage.h"
		#"Geometry/GeometryStorage.cpp"
		#"Geometry/GeometryCommonDefs.h"
#
		#"Geometry/GeometrySystem.cpp"
		#"Geometry/GeometrySystem.h"
		#"Geometry/MeshLoader.cpp"
		#"Geometry/MeshLoader.h"
		#"Geometry/MeshImporterAssimp.cpp"
		#"Geometry/Mesh.cpp"
		#"Geometry/Mesh.h"
		#"Geometry/GeometryDataTransfer.cpp"
		#"Geometry/GeometryDataTransfer.h"
		#"Geometry/GeometryCommonDefs.cpp"
		#"Geometry/MeshGroup.cpp"
		#"Geometry/GeometryContainer.cpp"
		#"Geometry/GeometryContainer.h"
		#"Geometry/StandardVertexFormats.h"
		#"Geometry/MeshCommon.h"
		#"Geometry/GeometryBuffer.h"
		#"Geometry/GeometryDataSource.cpp"
		#"Geometry/GeometryDataSource.h"
		)

if( "${IC3_COMPONENTS_BUILD_MODE}" STREQUAL "STATIC" )
    add_library( Ic3.NxMain STATIC ${IC3_ENGINE_SRC_FILES} )
    target_compile_definitions( Ic3.NxMain PRIVATE
            "IC3_BUILD_STATIC=1")
else()
    add_library( Ic3.NxMain SHARED ${IC3_ENGINE_SRC_FILES} )
endif()

set_target_properties( Ic3.NxMain PROPERTIES
        OUTPUT_NAME "Ic3.NxMain" )

target_compile_definitions( Ic3.NxMain PRIVATE
        "IC3_NXMAIN_BUILD=1"
		)

target_include_directories( Ic3.NxMain PUBLIC
		"${IC3_INCLUDE_DIR}"
		)

target_link_libraries( Ic3.NxMain PUBLIC
		${ASSIMP_TARGET_NAME}
		ext-freetype
		ext-png
		Ic3.CoreLib
		Ic3.Graphics.GCI
		)
