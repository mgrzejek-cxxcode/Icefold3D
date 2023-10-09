
#include <Ic3/System/DisplaySystem.h>
#include <Ic3/System/SysContext.h>
#include <Ic3/System/EventCore.h>
#include <Ic3/System/EventObject.h>
#include <Ic3/System/OpenGLDriver.h>
#include <Ic3/System/AssetSystemNative.h>
#include <Ic3/System/PerfCounter.h>
#include <Ic3/System/SysContextNative.h>

#include <Ic3/Graphics/GCI/GPUDevice.h>
#include <Ic3/Graphics/GCI/GPUDriver.h>
#include <Ic3/Graphics/GCI/GPUDriverAPI.h>
#include <Ic3/Graphics/GCI/CommandContext.h>
#include <Ic3/Graphics/GCI/CommandSystem.h>
#include <Ic3/Graphics/GCI/PresentationLayer.h>
#include <Ic3/Graphics/GCI/GPUUtils.h>
#include <Ic3/Graphics/GCI/Resources/GPUBuffer.h>
#include <Ic3/Graphics/GCI/Resources/Texture.h>
#include <Ic3/Graphics/GCI/Resources/RenderTargetTexture.h>
#include <Ic3/Graphics/GCI/State/Sampler.h>
#include <Ic3/Graphics/GCI/State/ShaderInputSignature.h>
#include <Ic3/Graphics/GCI/State/PipelineStateObject.h>
#include <Ic3/Graphics/GCI/State/RenderTargetDynamicStates.h>
#include <Ic3/Graphics/GCI/State/RenderTargetImmutableStates.h>
#include <Ic3/Graphics/GCI/State/InputAssemblerDynamicStates.h>
#include <Ic3/Graphics/GCI/State/InputAssemblerImmutableStates.h>

#include <Ic3/NxMain/Camera/CameraController.h>
#include <Ic3/NxMain/Res/image/BitmapCommon.h>
#include <Ic3/NxMain/Geometry/MeshGroup.h>
#include <Ic3/NxMain/Geometry/MeshLoader.h>

#include <Ic3/NxMain/Renderer/ShaderLibrary.h>
#include <Ic3/NxMain/Renderer/ShaderLoader.h>
#include <Ic3/NxMain/Renderer/Effects/shadowRenderer.h>
#include <Ic3/NxMain/Geometry/GeometryManager.h>
#include <Ic3/NxMain/Geometry/GeometryStorage.h>
#include <Ic3/NxMain/Geometry/GeometryDataTransfer.h>

#include <chrono>
#include <thread>

#include "MeshDefs.h"

using namespace Ic3;
using namespace GCI;
using namespace Ic3::System;
namespace math = Ic3::Math;

std::string sGxDriverName;

struct GraphicsDriverState
{
	std::string driverID;
	std::unique_ptr<GPUDriverInterface> driverInterface;
	GPUDriverHandle driver;
	GPUDeviceHandle device;
	PresentationLayerHandle  presentationLayer;
	std::unique_ptr<CommandContextDirectGraphics> cmdContext;
	bool pauseAnimation = false;
};

struct AppSharedResources
{
	ImageData txROG512Data;
};

void initializeGraphicsDriver( SysContextHandle pSysContext, GraphicsDriverState & pGxDriverState );

struct CB0Data
{
	math::Mat4x4f modelMatrix;
	math::Mat4x4f viewMatrix;
	math::Mat4x4f projectionMatrix;
};

DynamicMemoryBuffer loadShaderSourceDefault( AssetLoader & pAssetLoader, const std::string & pShaderFile )
{
	auto psAsset = pAssetLoader.openSubAsset(
			"shaders/" + sGxDriverName + "/" + pShaderFile,
			E_ASSET_OPEN_FLAG_NO_EXTENSION_BIT );

	DynamicMemoryBuffer resultBuffer;
	const auto sourceLength = psAsset->readAll( resultBuffer, 1 );

	resultBuffer[sourceLength] = 0;

	return resultBuffer;
}

DynamicMemoryBuffer loadFileDefault( AssetLoader & pAssetLoader, const std::string & pFilename )
{
	auto psAsset = pAssetLoader.openSubAsset( pFilename, 0 );

	DynamicMemoryBuffer resultBuffer;
	const auto sourceLength = psAsset->readAll( resultBuffer );

	return resultBuffer;
}

std::function<DynamicMemoryBuffer()> bindShaderSourceLoadCallbackDefault( AssetLoader & pAssetLoader, const std::string & pShaderFile )
{
	return std::bind( loadShaderSourceDefault, std::ref( pAssetLoader ), std::ref( pShaderFile ) );
}

#if( IC3_PCL_TARGET_SYSAPI == IC3_PCL_TARGET_SYSAPI_ANDROID )

#include <ic3/GPUapiGLES3/GLES3_gpuDriverAPI.h>
#include "../../engine/components/Ic3/Graphics/GCI/GPUDriver.h"

int ic3AndroidAppMain( int argc, char ** argv, AndroidAppState * pAppState )
{
    sGxDriverName = "GLES3";

    SysContextCreateInfo sysContextCreateInfo {};
    sysContextCreateInfo.nativeParams.aCommonAppState = pAppState;
    auto sysContext = createSysContext( sysContextCreateInfo );

    AssetLoaderCreateInfo aslCreateInfo;
    auto assetLoader = sysContext->createAssetLoader( aslCreateInfo );

    GraphicsDriverState gxDriverState;
    gxDriverState.driverID = "GLES3";
    gxDriverState.driverInterface = std::make_unique<GLES3GPUDriverInterface>();

#elif( IC3_PCL_TARGET_SYSAPI == IC3_PCL_TARGET_SYSAPI_WIN32 )

#define ENABLE_IC3DRV_GL4 1
#define ENABLE_IC3DRV_D3D11 1

#if( ENABLE_IC3DRV_GL4 )
# include <ic3/GPUapiGL4/GL4_gpuDriverAPI.h>
#endif
#if( ENABLE_IC3DRV_D3D11 )
#  include <ic3/GPUapiDX11/DX11_gpuDriverAPI.h>
#endif

int main( int pArgc, const char ** pArgv )
{
    sGxDriverName = "GL4";

	SysContextCreateInfo sysContextCreateInfo;
	platform::SysContextCreateInfoNativeParams sysContextCreateInfoNP;
	sysContextCreateInfoNP.appExecModuleHandle = ::GetModuleHandleA( nullptr );
	sysContextCreateInfo.nativeParams = &sysContextCreateInfoNP;
	auto sysContext = platform::createSysContext( sysContextCreateInfo );

	platform::AssetLoaderCreateInfoNativeParams aslCreateInfoNP;
	aslCreateInfoNP.relativeAssetRootDir = "assets";
	AssetLoaderCreateInfo aslCreateInfo;
	aslCreateInfo.nativeParams = &aslCreateInfoNP;
	auto assetLoader = sysContext->createAssetLoader( aslCreateInfo );

    GraphicsDriverState gxDriverState;
    gxDriverState.driverID = sGxDriverName;

#if( ENABLE_IC3DRV_GL4 )
    if( sGxDriverName == "GL4" )
    {
        gxDriverState.driverInterface = std::make_unique<GL4GPUDriverInterface>();
    }
#endif
#if( ENABLE_IC3DRV_D3D11 )
    if( sGxDriverName == "DX11" )
    {
        gxDriverState.driverInterface = std::make_unique<DX11GPUDriverInterface>();
    }
#endif

    ic3DebugAssert( gxDriverState.driverInterface );

#elif( IC3_PCL_TARGET_SYSAPI == IC3_PCL_TARGET_SYSAPI_X11 )

#include <ic3/GPUapiGL4/GL4_gpuDriverAPI.h>

int main( int pArgc, const char ** pArgv )
{
    sGxDriverName = "GL4";

    SysContextCreateInfo sysContextCreateInfo;
    auto sysContext = platform::createSysContext( sysContextCreateInfo );

    platform::AssetLoaderCreateInfoNativeParams aslCreateInfoNP;
    aslCreateInfoNP.relativeAssetRootDir = "assets";
    AssetLoaderCreateInfo aslCreateInfo;
    aslCreateInfo.nativeParams = &aslCreateInfoNP;
    auto assetLoader = sysContext->createAssetLoader( aslCreateInfo );

    GraphicsDriverState gxDriverState;
    gxDriverState.driverID = "GL4";
    gxDriverState.driverInterface = std::make_unique<GL4GPUDriverInterface>();

#elif( IC3_PCL_TARGET_SYSAPI == IC3_PCL_TARGET_SYSAPI_OSX )

#include <Ic3/Graphics/HW3D/GL4/GL4GPUDriverAPI.h>

int main( int pArgc, const char ** pArgv )
{
	sGxDriverName = "GL4";

	SysContextCreateInfo sysContextCreateInfo;
	auto sysContext = platform::createSysContext( sysContextCreateInfo );

	platform::AssetLoaderCreateInfoNativeParams aslCreateInfoNP;
	aslCreateInfoNP.relativeAssetRootDir = "assets";
	AssetLoaderCreateInfo aslCreateInfo;
	aslCreateInfo.nativeParams = &aslCreateInfoNP;
	auto assetLoader = sysContext->createAssetLoader( aslCreateInfo );

	GraphicsDriverState gxDriverState;
	gxDriverState.driverID = "GL4";
	gxDriverState.driverInterface = std::make_unique<GL4GPUDriverInterface>();

#endif

	srand( time( nullptr ) );

	bool runApp = true;

	auto evtController = sysContext->createEventController();
	auto evtDispatcher = evtController->createEventDispatcher();
	evtController->setActiveEventDispatcher( *evtDispatcher );

#if( IC3_PCL_TARGET_OS == IC3_PCL_TARGET_OS_ANDROID )
	bool waitForDisplay = true;

    evtDispatcher->setEventHandler(
            EEventCodeIndex::AppActivityDisplayInit,
            [&waitForDisplay,&sysContext](const EventObject & pEvt) -> bool {
                waitForDisplay = false;
                return true;
            });

    while( waitForDisplay )
    {
        evtController->dispatchPendingEventsAuto();
    }

    evtDispatcher->setEventHandler(
            EEventCodeIndex::AppActivityDisplayInit,
            [&gfxState](const EventObject & pEvt) -> bool {
                initializeGraphicsGL( gfxState );
                gfxState.glContext->bindForCurrentThread( *(gfxState.glSurface) );
                gfxState.pauseAnimation = false;
                return true;
            });

    evtDispatcher->setEventHandler(
            EEventCodeIndex::AppActivityDisplayTerm,
            [&gfxState](const EventObject & pEvt) -> bool {
                //gfxState.glSystemDriver->invalidate();
                gfxState.glContext = nullptr;
                gfxState.glSurface = nullptr;
                gfxState.glSystemDriver = nullptr;
                gfxState.pauseAnimation = true;
                return true;
            });
#endif

	bool isFullscreen = true;
	bool updateLightPosition = true;

	CameraController cameraController;

	evtDispatcher->setEventHandler(
			EEventCodeIndex::AppActivityQuit,
			[&runApp](const EventObject & pEvt) -> bool {
				runApp = false;
				return true;
			});
	evtDispatcher->setEventHandler(
			EEventCodeIndex::WindowUpdateDestroy,
			[evtDispatcher,&gxDriverState](const EventObject & pEvt) -> bool {
				if( pEvt.eWindowUpdateDestroy.checkEventSource( gxDriverState.presentationLayer->getInternalSystemEventSource() ) )
				{
					evtDispatcher->postEventAppQuit();
				}
				return true;
			});
	evtDispatcher->setEventHandler(
			EEventCodeIndex::InputKeyboard,
			[&](const EventObject & pEvt) -> bool {
				auto & keyMap = pEvt.eInputKeyboard.inputKeyboardState->keyStateMap;
				if( pEvt.eInputKeyboard.keyCode == EKeyCode::Escape )
				{
					evtDispatcher->postEventAppQuit();
				}
				else if( keyMap[EKeyCode::Enter] )
				{
					isFullscreen = !isFullscreen;
					ic3DebugInterrupt();
					// gxDriverState.presentationLayer->setFullscreenMode( isFullscreen );
				}
				if( keyMap[EKeyCode::CtrlLeft] )
				{
					ic3DebugOutput("CTRL_LEFT");
				}
				if( keyMap[EKeyCode::CharW] )
				{
					cameraController.move( 0.075f );
				}
				if( keyMap[EKeyCode::CharS] )
				{
					cameraController.move( -0.075f );
				}
				if( keyMap[EKeyCode::CharD] )
				{
					cameraController.moveSide( 0.075f );
				}
				if( keyMap[EKeyCode::CharA] )
				{
					cameraController.moveSide( -0.075f );
				}
				if( keyMap[EKeyCode::Space] )
				{
					cameraController.moveUpDown( 0.075f );
				}
				if( keyMap[EKeyCode::CharC] )
				{
					cameraController.moveUpDown( -0.075f );
				}
				if( keyMap[EKeyCode::CharL] )
				{
					updateLightPosition = true;
				}
				if( keyMap[EKeyCode::CharK] )
				{
					updateLightPosition = false;
				}
				return true;
			});

	initializeGraphicsDriver( sysContext, gxDriverState );

	if( auto * eventSource = gxDriverState.presentationLayer->getInternalSystemEventSource() )
	{
		evtController->registerEventSource( *eventSource );
	}

	CoreEngineState ces{ sysContext, gxDriverState.device };

	GeometryDataFormat gdf;
	gdf.configureFixedAttribute( EFixedVertexAttributeID::Position, 0 );
	gdf.configureFixedAttribute( EFixedVertexAttributeID::Normal, 0 );
	gdf.configureFixedAttribute( EFixedVertexAttributeID::FixedColor, 0 );
	gdf.configureFixedAttribute( EFixedVertexAttributeID::TexCoord0, 0 );
	gdf.setIndexDataFormat( GCI::EIndexDataFormat::Uint32 );
	gdf.setPrimitiveTopology( GCI::EPrimitiveTopology::TriangleList );

	MeshLoader meshLoader;
	GeometryManager geometryManager{ ces };

	std::unique_ptr<MeshGroup> meshGroup;

	{
		MeshImporterAssimp meshImporter;
		GeometryDataGpuTransferUpload geometryDataGpuTransfer{ ces, *gxDriverState.cmdContext };

		MeshImportContext mic;
		mic.geometryManager = &geometryManager;
		mic.geometryDataFormat = &gdf;
		mic.importer = &meshImporter;
		mic.geometryDataTransfer = &geometryDataGpuTransfer;

		meshGroup = meshLoader.importMeshGroup( mic, "MeshGroupID_Default", {
				{ "MeshID_Default_TreeBase", "assets/meshes/tree/Tree.obj" }
			} );
	}

	auto * meshTree = meshGroup->findMesh( "MeshID_Default_TreeBase" );

	const auto rtSize = gxDriverState.presentationLayer->queryRenderTargetSize();
	auto shaderLibrary = ShaderLoader::createShaderLibrary( ces, {
			{ "SID_DEFAULT_PASSTHROUGH_VS", GCI::EShaderType::GSVertex, bindShaderSourceLoadCallbackDefault( *assetLoader, "default_passthrough_vs" ) },
			{ "SID_DEFAULT_PASSTHROUGH_PS", GCI::EShaderType::GSPixel, bindShaderSourceLoadCallbackDefault( *assetLoader, "default_passthrough_ps" ) },
			{ "SID_SHADOW_0_PASS1_LIGHT_VS", GCI::EShaderType::GSVertex, bindShaderSourceLoadCallbackDefault( *assetLoader, "shadow_0_pass1_light_vs" ) },
			{ "SID_SHADOW_0_PASS1_LIGHT_PS", GCI::EShaderType::GSPixel, bindShaderSourceLoadCallbackDefault( *assetLoader, "shadow_0_pass1_light_ps" ) },
			{ "SID_SHADOW_0_PASS2_SHADOW_VS", GCI::EShaderType::GSVertex, bindShaderSourceLoadCallbackDefault( *assetLoader, "shadow_0_pass2_shadow_vs" ) },
			{ "SID_SHADOW_0_PASS2_SHADOW_PS", GCI::EShaderType::GSPixel, bindShaderSourceLoadCallbackDefault( *assetLoader, "shadow_0_pass2_shadow_ps" ) },
	} );

	auto vertexShader = shaderLibrary->getShader( "SID_DEFAULT_PASSTHROUGH_VS" );
	auto pixelShader = shaderLibrary->getShader( "SID_DEFAULT_PASSTHROUGH_PS" );

	auto * gpuDevicePtr = gxDriverState.device.get();

	GCI::GPUBufferHandle cbuffer0;
	{
		GCI::GPUBufferCreateInfo cbci;
		cbci.memoryFlags = GCI::E_GPU_MEMORY_ACCESS_FLAG_GPU_READ_BIT;

		cbci.resourceFlags = GCI::E_GPU_RESOURCE_CONTENT_FLAG_STATIC_BIT;
		cbci.resourceFlags |= GCI::E_GPU_BUFFER_BIND_FLAG_CONSTANT_BUFFER_BIT;

		cbci.bufferSize = sizeof( CB0Data );
		cbuffer0 = gxDriverState.device->createGPUBuffer( cbci );
	}

	GCI::TextureHandle texRTColor0;
	GCI::RenderTargetTextureHandle texRTColor0RT;
	{
		GCI::TextureCreateInfo texRTColor0CI;
		texRTColor0CI.texClass = GCI::ETextureClass::T2D;
		texRTColor0CI.dimensions.width = rtSize.x;
		texRTColor0CI.dimensions.height = rtSize.y;
		texRTColor0CI.memoryFlags = GCI::E_GPU_MEMORY_ACCESS_FLAG_GPU_READ_BIT;
		texRTColor0CI.resourceFlags =
				GCI::E_GPU_RESOURCE_USAGE_FLAG_RENDER_TARGET_COLOR_BIT |
				GCI::E_GPU_RESOURCE_USAGE_FLAG_SHADER_INPUT_BIT;
		texRTColor0CI.internalFormat = GCI::ETextureFormat::RGBA8UN;
		texRTColor0 = gpuDevicePtr->createTexture( texRTColor0CI );

		GCI::RenderTargetTextureCreateInfo texRTColor0RTCI;
		texRTColor0RTCI.targetTexture = texRTColor0;
		texRTColor0RTCI.bindFlags =
				E_TEXTURE_BIND_FLAG_RENDER_TARGET_COLOR_ATTACHMENT_BIT |
				E_TEXTURE_BIND_FLAG_SHADER_INPUT_SAMPLED_IMAGE_BIT;
		texRTColor0RT = gpuDevicePtr->createRenderTargetTexture( texRTColor0RTCI );
	}

	GCI::TextureHandle texRTDepthStencil;
	GCI::RenderTargetTextureHandle texRTDepthStencilRT;
	{
		GCI::TextureCreateInfo texRTDepthStencilCI;
		texRTDepthStencilCI.texClass = GCI::ETextureClass::T2D;
		texRTDepthStencilCI.dimensions.width = rtSize.x;
		texRTDepthStencilCI.dimensions.height = rtSize.y;
		texRTDepthStencilCI.memoryFlags = GCI::E_GPU_MEMORY_ACCESS_FLAG_GPU_READ_BIT;
		texRTDepthStencilCI.resourceFlags =
				GCI::E_GPU_RESOURCE_USAGE_MASK_RENDER_TARGET_DEPTH_STENCIL;
		texRTDepthStencilCI.internalFormat = GCI::ETextureFormat::D24UNS8U;
		texRTDepthStencil = gpuDevicePtr->createTexture( texRTDepthStencilCI );

		GCI::RenderTargetTextureCreateInfo texRTDepthStencilRTCI;
		texRTDepthStencilRTCI.targetTexture = texRTDepthStencil;
		texRTDepthStencilRTCI.bindFlags =
				E_TEXTURE_BIND_FLAG_RENDER_TARGET_DEPTH_STENCIL_ATTACHMENT_BIT;
		texRTDepthStencilRT = gpuDevicePtr->createRenderTargetTexture( texRTDepthStencilRTCI );
	}

	GCI::RenderPassConfigurationImmutableStateHandle fboRenderPassState;
	GCI::RenderPassConfigurationImmutableStateHandle scrRenderPassState;
	{
		RenderPassConfiguration rpConfig;
		rpConfig.activeAttachmentsMask = E_RT_ATTACHMENT_MASK_DEFAULT_C0_DS;
		rpConfig.attachmentsActionClearMask = E_RT_ATTACHMENT_MASK_DEFAULT_C0_DS;
		rpConfig.colorAttachments[0].renderPassLoadAction = ERenderPassAttachmentLoadAction::Clear;
		rpConfig.colorAttachments[0].renderPassStoreAction = ERenderPassAttachmentStoreAction::Keep;
		rpConfig.colorAttachments[0].clearConfig.colorValue = { 0.12f, 0.36f, 0.88f, 1.0f };
		rpConfig.depthStencilAttachment.renderPassLoadAction = ERenderPassAttachmentLoadAction::Clear;
		rpConfig.depthStencilAttachment.renderPassStoreAction = ERenderPassAttachmentStoreAction::Keep;
		rpConfig.depthStencilAttachment.clearConfig.depthValue = 1.0f;
		rpConfig.depthStencilAttachment.clearConfig.stencilValue = 0xFF;
		fboRenderPassState = gpuDevicePtr->createRenderPassConfigurationImmutableState( rpConfig );

		// rpConfig.colorAttachments[0].clearConfig.colorValue = { 0.68f, 0.92f, 0.78f, 1.0f };
		scrRenderPassState = gpuDevicePtr->createRenderPassConfigurationImmutableState( rpConfig );
	}

	GraphicsPipelineStateObjectHandle mainPSO;
	{
		RenderTargetLayout rtLayout;
		rtLayout.activeAttachmentsMask = E_RT_ATTACHMENT_MASK_DEFAULT_C0_DS;
		rtLayout.colorAttachments[0].format = ETextureFormat::BGRA8UN;
		rtLayout.depthStencilAttachment.format = ETextureFormat::D24UNS8U;

		GraphicsPipelineStateObjectCreateInfo psoci;
		psoci.renderTargetLayout.activeAttachmentsMask = E_RT_ATTACHMENT_MASK_DEFAULT_C0_DS;
		psoci.renderTargetLayout.colorAttachments[0].format = ETextureFormat::BGRA8UN;
		psoci.renderTargetLayout.depthStencilAttachment.format = ETextureFormat::D24UNS8U;
		psoci.blendConfig = defaults::cvPipelineBlendConfigDefault;
		psoci.depthStencilConfig = defaults::cvPipelineDepthStencilConfigEnableDepthTest;
		psoci.rasterizerConfig = defaults::cvPipelineRasterizerConfigDefault;
		psoci.rasterizerConfig.cullMode = GCI::ECullMode::Back;
		psoci.rasterizerConfig.primitiveFillMode = GCI::EPrimitiveFillMode::Wireframe;
		psoci.rasterizerConfig.frontFaceVerticesOrder = GCI::ETriangleVerticesOrder::Clockwise;
		psoci.renderTargetLayout = rtLayout;
		psoci.inputLayoutDefinition = gdf.generateGpaInputLayoutDefinition();
		psoci.shaderSet.addShader( vertexShader );
		psoci.shaderSet.addShader( pixelShader );
		psoci.shaderInputSignatureDesc.activeShaderStagesMask = GCI::E_SHADER_STAGE_FLAG_GRAPHICS_VERTEX_BIT | GCI::E_SHADER_STAGE_FLAG_GRAPHICS_PIXEL_BIT;
		psoci.shaderInputSignatureDesc.descriptorSetsNum = 1;
		psoci.shaderInputSignatureDesc.descriptorSetArray[0].descriptorType = GCI::EShaderInputDescriptorType::Resource;
		psoci.shaderInputSignatureDesc.descriptorSetArray[0].descriptorsNum = 1;
		psoci.shaderInputSignatureDesc.descriptorSetArray[0].descriptorList[0] = {0, GCI::EShaderInputDescriptorType::Resource, GCI::E_SHADER_STAGE_FLAG_GRAPHICS_VERTEX_BIT };
		psoci.shaderInputSignatureDesc.descriptorSetArray[0].descriptorList[0].uResourceDesc = { GCI::EShaderInputResourceType::CBVConstantBuffer, 0, 1 };

		mainPSO = gxDriverState.device->createGraphicsPipelineStateObject( psoci );
	}

	RenderTargetBindingDynamicState rtds;
	auto & c0 = rtds.setColorAttachmentBinding( 0 );
	c0.attachmentTexture = texRTColor0RT;
	auto & ds = rtds.setDepthStencilAttachmentBinding();
	ds.attachmentTexture = texRTDepthStencilRT;

	math::Vec3f cameraOriginPoint{ 0.0f, 2.0f,  -4.0f };
	math::Vec3f cameraTargetPoint{ 0.0f, 0.0f,  4.0f };
	cameraController.initialize( cameraOriginPoint, cameraTargetPoint, 60.0f );

	GCI::ViewportDesc vpDescScreen{};
	vpDescScreen.origin.x = 0;
	vpDescScreen.origin.y = 0;
	vpDescScreen.size.x = rtSize.x;
	vpDescScreen.size.y = rtSize.y;
	vpDescScreen.depthRange.zNear = 0.0f;
	vpDescScreen.depthRange.zFar = 1.0f;

	GCI::ViewportDesc vpDescTexture{};
	vpDescTexture.origin.x = 0;
	vpDescTexture.origin.y = 0;
	vpDescTexture.size.x = rtSize.x;
	vpDescTexture.size.y = rtSize.y;
	vpDescTexture.depthRange.zNear = 0.0f;
	vpDescTexture.depthRange.zFar = 1.0f;

	const auto ic3ViewTexture = lookAtLH(
			math::Vec3f{ 0.0f, 3.0f,  -1.0f },
			math::Vec3f{ 0.0f, 0.0f,  5.0f },
			math::Vec3f{ 0.0f, 1.0f,  0.0f } );

	const auto ic3ProjectionTexture = math::perspectiveAspectLH<float>(
			cameraController.getPerspectiveFOVAngle(), ( float )rtSize.x / ( float )rtSize.y, 0.1f, 1000.0f );

	const auto ic3CameraProjection = math::perspectiveAspectLH<float>(
			cameraController.getPerspectiveFOVAngle(), ( float )rtSize.x / ( float )rtSize.y, 1.0f, 1000.0f );

	CB0Data cb0DataBase =
	{
		math::identity4<float>(),
		math::identity4<float>(),
		math::identity4<float>(),
	};

	const uint32 VNUM = 36;

	System::perf_counter_value_t u1ts = System::PerfCounter::queryCurrentStamp();
	System::perf_counter_value_t u2ts = System::PerfCounter::queryCurrentStamp();

	const float update1ts = 25.0f;
	const float update2ts = 25.0f;

	float u1angle = 0.0f;
	float u2angle = 0.0f;

	GPUBufferDataUploadDesc cb0DataUploadDesc;
	cb0DataUploadDesc.inputDataDesc.pointer = &cb0DataBase;
	cb0DataUploadDesc.inputDataDesc.size = sizeof( CB0Data );

	bool rotate = false;

	evtDispatcher->setEventHandler(
			System::EEventCodeIndex::InputMouseButton,
			[&rotate]( const System::EventObject & pEvt ) -> bool {
				const auto & eButton = pEvt.eInputMouseButton;
				if( eButton.buttonAction == EMouseButtonActionType::Click )
				{
					rotate = true;
				}
				else if( eButton.buttonAction == EMouseButtonActionType::Release )
				{
					rotate = false;
				}
				return true;
			});
	evtDispatcher->setEventHandler(
			System::EEventCodeIndex::InputMouseMove,
			[&cameraController,&rotate]( const System::EventObject & pEvt ) -> bool {
				//if( rotate )
				{
					const auto & emove = pEvt.eInputMouseMove;
					if( emove.buttonStateMask.isSet( System::E_MOUSE_BUTTON_FLAG_LEFT_BIT ) )
					{
						cameraController.rotateAroundOrigin( emove.movementDelta.x, emove.movementDelta.y );
					}
					else if( emove.buttonStateMask.isSet( System::E_MOUSE_BUTTON_FLAG_RIGHT_BIT ) )
					{
						cameraController.rotateAroundTarget( emove.movementDelta.x, emove.movementDelta.y );
					}
				}
				return true;
			});

	srand( time( nullptr ) );

	const auto treesNum = 36;
	const auto treeModelTransBase = math::translation( 0.0f, 0.48f, 1.0f);
	const auto treeModelScale = math::scaling<float>( 0.85f, 0.85f, 0.85f );

	const auto treeRangeX = std::make_pair( -4.0f, 4.0f );
	const auto treeRangeZ = std::make_pair( 0.0f, 6.0f );

	math::Mat4f treeModelMats[treesNum];
	for( uint32 iTree = 0; iTree < treesNum; ++iTree )
	{
		const auto xRangeLength = ( uint32 )( ( treeRangeX.second - treeRangeX.first ) * 100.0f );
		const auto zRangeLength = ( uint32 )( ( treeRangeZ.second - treeRangeZ.first ) * 100.0f );
		const auto xMod = rand() % xRangeLength;
		const auto zMod = rand() % zRangeLength;
		const auto xOff = xMod * 0.01f + treeRangeX.first;
		const auto zOff = zMod * 0.01f + treeRangeZ.first;

		treeModelMats[iTree] = math::mul(
				math::mul( treeModelTransBase, math::translation( xOff, 0.0f, zOff ) ),
				math::mul( math::rotationAxisY( ( rand() % 360 )  * math::constants::cxFloatRad1Degree ), treeModelScale ) );
	}

	while( runApp )
	{
		if( gxDriverState.pauseAnimation )
		{
			continue;
		}

		try
		{
			evtController->dispatchPendingEventsAuto();

			gxDriverState.cmdContext->beginCommandSequence();

			const auto cameraViewMatrix = cameraController.computeViewMatrixLH();

			gxDriverState.cmdContext->setGraphicsPipelineStateObject( *mainPSO );
			gxDriverState.cmdContext->setIAVertexStreamState( *meshGroup->getGeometryStorage().getGpaVertexStreamState() );

			cb0DataBase.projectionMatrix = ic3CameraProjection;
			cb0DataBase.viewMatrix = cameraViewMatrix;

			gxDriverState.presentationLayer->bindRenderTarget( gxDriverState.cmdContext.get() );

			gxDriverState.cmdContext->beginRenderPass( *scrRenderPassState );
			{
				gxDriverState.cmdContext->cmdSetViewport( vpDescScreen );
				gxDriverState.cmdContext->cmdSetShaderConstantBuffer( 0, *cbuffer0 );

				for( uint32 iTree = 0; iTree < treesNum; ++iTree )
				{
					cb0DataBase.modelMatrix = treeModelMats[iTree];
					gxDriverState.cmdContext->updateBufferDataUpload( *cbuffer0, cb0DataBase );

					for( uint32 iMeshComponent = 0; iMeshComponent < meshTree->getComponentsNum(); ++iMeshComponent )
					{
						auto * meshComponent = meshTree->getSubComponent( iMeshComponent );
						auto * geometryRef = meshComponent->geometryDataRef();


						gxDriverState.cmdContext->cmdDrawDirectIndexed(
								geometryRef->dataReference.indexDataRegion.sizeInElementsNum,
								geometryRef->dataReference.indexDataRegion.offsetInElementsNum,
								geometryRef->dataReference.vertexDataOffsetInElementsNum() );
					}
				}

				gxDriverState.presentationLayer->invalidateRenderTarget( gxDriverState.cmdContext.get() );
			}
			gxDriverState.cmdContext->endRenderPass();

			gxDriverState.cmdContext->endCommandSequence();
			gxDriverState.cmdContext->submit();

			gxDriverState.presentationLayer->present();

			// std::this_thread::sleep_for( std::chrono::milliseconds( 8 ) );
		}
		catch( ... )
		{
		}
	}

	return 0;
}

void initializeGraphicsDriver( SysContextHandle pSysContext, GraphicsDriverState & pGxDriverState )
{
	GPUDriverCreateInfo gpuDriverCreateInfo;
	gpuDriverCreateInfo.sysContext = pSysContext;
	gpuDriverCreateInfo.configFlags = E_GPU_DRIVER_CONFIG_FLAG_ENABLE_SHADER_DEBUG_INFO_BIT;
	pGxDriverState.driver = pGxDriverState.driverInterface->createDriver( gpuDriverCreateInfo );

	GPUDeviceCreateInfo gpuDeviceCreateInfo;
	gpuDeviceCreateInfo.adapterID = 0;
	gpuDeviceCreateInfo.flags = E_GPU_DEVICE_CREATE_FLAGS_DEFAULT;
	pGxDriverState.device = pGxDriverState.driver->createDevice( gpuDeviceCreateInfo );

	PresentationLayerCreateInfo presentationLayerCreateInfo;
	presentationLayerCreateInfo.screenRect.size.x = 1920;
	presentationLayerCreateInfo.screenRect.size.y = 1080;
	presentationLayerCreateInfo.displayConfigFlags = 0;
	presentationLayerCreateInfo.displayConfigFlags = 0;//E_DISPLAY_CONFIGURATION_FLAG_FULLSCREEN_BIT;
	pGxDriverState.presentationLayer = pGxDriverState.driverInterface->createScreenPresentationLayer( *( pGxDriverState.device ), presentationLayerCreateInfo );

	pGxDriverState.device->setPresentationLayer( pGxDriverState.presentationLayer );
	pGxDriverState.cmdContext = pGxDriverState.device->getCommandSystem().acquireCommandContext<CommandContextDirectGraphics>();
}
