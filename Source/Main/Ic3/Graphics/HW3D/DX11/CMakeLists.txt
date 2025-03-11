
set( IC3_GRAPHICS_HW3D_DX11_SRC_FILES
	"DX11Prerequisites.h"
	"DX11Prerequisites.cpp"

	"DX11CommandList.h"
	"DX11CommandList.cpp"
	"DX11CommandSystem.h"
	"DX11CommandSystem.cpp"
	"DX11APITranslationLayer.h"
	"DX11APITranslationLayer.cpp"
	"DX11GPUDevice.h"
	"DX11GPUDevice.cpp"
	"DX11GPUDriver.h"
	"DX11GPUDriver.cpp"
	"DX11GPUDriverAPI.h"
	"DX11GPUDriverAPI.cpp"
	"DX11PresentationLayer.h"
	"DX11PresentationLayer.cpp"


	"Resources/DX11GPUBuffer.h"
	"Resources/DX11GPUBuffer.cpp"
	"Resources/DX11Sampler.h"
	"Resources/DX11Sampler.cpp"
	"Resources/DX11Shader.h"
	"Resources/DX11Shader.cpp"
	"Resources/DX11Texture.h"
	"Resources/DX11Texture.cpp"

	"State/DX11GraphicsPipelineStateCommon.h"
	"State/DX11GraphicsPipelineStateCommon.cpp"
	"State/DX11GraphicsPipelineStateRTO.h"
	"State/DX11GraphicsPipelineStateRTO.cpp"

	"State/DX11PipelineStateController.h"
	"State/DX11PipelineStateController.cpp"
	"State/DX11PipelineStateObject.h"
	"State/DX11PipelineStateObject.cpp"

	"State/DX11GraphicsShaderState.h"

	"State/DX11InputAssembler.cpp"
	"State/DX11InputAssembler.h"
	"State/DX11PipelineImmutableStateFactory.cpp"
	"State/DX11PipelineImmutableStateFactory.h"
)

ic3AddCoreLib( Ic3.Graphics.HW3D.DX11 ${IC3_GRAPHICS_HW3D_DX11_SRC_FILES} )

target_include_directories( Ic3.Graphics.HW3D.DX11 PUBLIC
		${IC3_GRAPHICS_HW3D_INCLUDE_DIR_INTERNAL} )

target_link_libraries( Ic3.Graphics.HW3D.DX11 PUBLIC
		Ic3.Graphics.GCI
		Ic3.System
		D3D11.lib
		D3DCompiler.lib )
