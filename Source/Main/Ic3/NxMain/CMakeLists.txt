
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
		"GCI/ShaderInputSemantics.h"
		"GCI/ShaderInputSemantics.cpp"

		"GCI/VertexAttributeCommonDefs.h"
		"GCI/VertexPipelineDefs.h"
		"GCI/VertexPipelineDefs.cpp"
		"GCI/VertexPipelineLayouts.h"
		"GCI/VertexPipelineLayouts.cpp"
		"Geometry/VertexPipelineConfig.h"
		"Geometry/VertexPipelineConfig.cpp"
		"Geometry/VertexDataSystem.h"
		"Geometry/VertexDataSystem.cpp"

		"Renderer/CommonRendererDefs.h"
		"Renderer/Renderer.h"
		"Renderer/Renderer.cpp"
		"Renderer/ShaderLibrary.h"
		"Renderer/shaderLibrary.cpp"
		"Renderer/Effects/ShadowRenderer.h"
		"Renderer/Effects/ShadowRenderer.cpp"

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

		# "utility/scfCommon.h"
        # "utility/scfEntry.h"
        # "utility/scfEntry.cpp"
		# "utility/scfIndex.h"
		# "utility/scfIndex.cpp"
		# "utility/scfIndexBuilder.h"
		# "utility/scfIndexBuilder.cpp"
		# "utility/scfIOSupport.h"
		# "utility/scfIOSupport.cpp"
		# "utility/scfXMLCommon.h"
		# "utility/scfXMLCommon.cpp"
		# "utility/scfXMLReader.h"
		# "utility/scfXMLReader.cpp"
		# "utility/gridGenerator.cpp"
		# "utility/gridGenerator.h"

		"Geometry/IndexedMeshDataBufferBuilder.h"
		"Geometry/MeshData.h"
		"Geometry/MeshData.cpp"
		"Geometry/MeshGroup.h"
		#"renderer/MaterialCommon.h"
		#"renderer/gpaSharedStateLibrary.cpp"
		#"renderer/gpaSharedStateLibrary.h"
		#"renderer/renderingContext.cpp"
		#"renderer/renderingContext.h"
		#"renderer/shaderLoader.h"
		#"renderer/shaderLoader.cpp"

		#"gpuapi/constantBufferProxy.cpp"
		#"gpuapi/constantBufferProxy.h"
		"Geometry/DataTypesConv.h"
        "Geometry/GeometryStorageGPU.h"
		"Geometry/GeometryStorageGPU.cpp"
        "Geometry/GeometryStorage.h"
		"Geometry/GeometryStorage.cpp"
		"Geometry/GeometryCommonDefs.h"

        #"fx/Material.h"
		#"fx/Material.cpp"
		#"fx/MaterialCommon.h"
		#"fx/MaterialCommon.cpp"
		#"fx/MaterialLibrary.cpp"
		#"fx/MaterialLibrary.h"
		"Geometry/GeometrySystem.cpp"
		"Geometry/GeometrySystem.h"
		"Geometry/MeshLoader.cpp"
		"Geometry/MeshLoader.h"
		"Geometry/MeshImporterAssimp.cpp"
		"Geometry/Mesh.cpp"
		"Geometry/Mesh.h"
		"Geometry/GeometryDataTransfer.cpp"
		"Geometry/GeometryDataTransfer.h"
		"Geometry/GeometryCommonDefs.cpp"
		"Geometry/MeshGroup.cpp"
		"Geometry/GeometryContainer.cpp"
		"Geometry/GeometryContainer.h"
		"Geometry/StandardVertexFormats.h"
		"Geometry/MeshCommon.h"
		"Geometry/GeometryBuffer.h"
		"Geometry/GeometryDataSource.cpp"
		"Geometry/GeometryDataSource.h"
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
		ExtLib.FreeType
		ExtLib.Png
		Ic3.CoreLib
		Ic3.Graphics.GCI
		)
