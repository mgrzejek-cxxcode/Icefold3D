
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
#include <Ic3/Graphics/GCI/State/RootSignature.h>
#include <Ic3/Graphics/GCI/State/GraphicsPipelineStateCommon.h>
#include <Ic3/Graphics/GCI/State/GraphicsPipelineStateDescriptorRTO.h>
#include <Ic3/Graphics/GCI/State/GraphicsPipelineStateDescriptorIA.h>

#include <Ic3/NxMain/Camera/CameraController.h>

#include <Ic3/NxMain/GCI/VertexFormatDescriptor.h>
#include <Ic3/NxMain/GCI/VertexFormatDescriptorUtils.h>

#include <chrono>
#include <thread>

using namespace Ic3;
using namespace GCI;
using namespace Ic3::System;

std::string sGxDriverName;

struct GraphicsDriverState
{
	std::string driverID;
	std::unique_ptr<GPUDriverInterface> driverInterface;
	GPUDriverHandle driver;
	GPUDeviceHandle device;
	PresentationLayerHandle presentationLayer;
	std::unique_ptr<CommandContextDirectGraphics> cmdContext;
	bool pauseAnimation = false;
};

void InitializeGraphicsDriver( SysContextHandle pSysContext, GraphicsDriverState & pGxDriverState );

#include <Ic3/Graphics/HW3D/GL4/GL4GPUDriverAPI.h>

int main( int pArgc, const char ** pArgv )
{
    {
		VertexFormatDescriptorBuilder vfdBuilder{};
		vfdBuilder.AddAttribute( 0, kStandardVertexAttributeKeyPosition );
		vfdBuilder.AddAttribute( 0, kStandardVertexAttributeKeyNormal );
		vfdBuilder.AddAttribute( 0, kStandardVertexAttributeKeyTangent );
		vfdBuilder.AddAttribute( 0, kStandardVertexAttributeKeyBiTangent );
		vfdBuilder.AddAttribute( 0, kStandardVertexAttributeKeyTexCoord0 );
		vfdBuilder.AddAttribute( 0, 6, { "CustomPosRef" }, GCI::EVertexAttribFormat::Vec4F32 );
		vfdBuilder.AddAttribute( 1, kStandardVertexAttributeKeyInstanceMatrix );
		vfdBuilder.AddAttribute( 2, kStandardVertexAttributeKeyInstanceUserData );
		auto vfd = vfdBuilder.CreateVertexFormatDescriptor( GCI::EPrimitiveTopology::TriangleList, GCI::EIndexDataFormat::Uint32 );

		const auto sid1 = vfd->GenerateVertexFormatStringID();
		const auto defArray0 = vfd->GenerateAttributeDefinitionArray();
		const auto defArray1 = VertexFormatDescriptorBuilder::ParseVertexFormatString( sid1 );

		VertexFormatDescriptorBuilder vfdBuilder0{};
		vfdBuilder0.AddAttributeList( cppx::bind_array_view( defArray0 ) );
		auto vfdCopy0 = vfdBuilder0.CreateVertexFormatDescriptor( GCI::EPrimitiveTopology::TriangleList, GCI::EIndexDataFormat::Uint32 );
		const auto sidCopy0 = vfd->GenerateVertexFormatStringID();

		VertexFormatDescriptorBuilder vfdBuilder1{};
		vfdBuilder1.AddAttributeList( cppx::bind_array_view( defArray1 ) );
		auto vfdCopy1 = vfdBuilder1.CreateVertexFormatDescriptor( GCI::EPrimitiveTopology::TriangleList, GCI::EIndexDataFormat::Uint32 );
		const auto sidCopy1 = vfd->GenerateVertexFormatStringID();

		Ic3DebugOutput( sid1.c_str() );
		Ic3DebugOutput( sidCopy0.c_str() );
		Ic3DebugOutput( sidCopy1.c_str() );
    }

	sGxDriverName = "GL4";

	SysContextCreateInfo sysContextCreateInfo;
	auto sysContext = Platform::CreateSysContext( sysContextCreateInfo );

	Platform::AssetLoaderCreateInfoNativeParams aslCreateInfoNP;
	aslCreateInfoNP.relativeAssetRootDir = "assets";
	AssetLoaderCreateInfo aslCreateInfo;
	aslCreateInfo.nativeParams = &aslCreateInfoNP;
	auto assetLoader = sysContext->CreateAssetLoader( aslCreateInfo );

	GraphicsDriverState gxDriverState;
	gxDriverState.driverID = "GL4";
	gxDriverState.driverInterface = std::make_unique<GL4GPUDriverInterface>();

	srand( time( nullptr ));

	bool runApp = true;

	auto evtController = sysContext->CreateEventController();
	auto evtDispatcher = evtController->CreateEventDispatcher();
	evtController->SetActiveEventDispatcher( *evtDispatcher );

	bool isFullscreen = true;
	bool updateLightPosition = true;

	CameraController cameraController;

	evtDispatcher->SetEventHandler(
			EEventCodeIndex::AppActivityQuit,
			[&runApp](const EventObject & pEvt) -> bool {
				runApp = false;
				return true;
			});
	evtDispatcher->SetEventHandler(
			EEventCodeIndex::WindowUpdateDestroy,
			[evtDispatcher,&gxDriverState](const EventObject & pEvt) -> bool {
				if( pEvt.uEvtWindowUpdateDestroy.CheckEventSource( gxDriverState.presentationLayer->GetInternalSystemEventSource() ) )
				{
					evtDispatcher->PostEventAppQuit();
				}
				return true;
			});
	evtDispatcher->SetEventHandler(
			EEventCodeIndex::InputKeyboard,
			[&](const EventObject & pEvt) -> bool {
				auto & keyMap = pEvt.uEvtInputKeyboard.mInputKeyboardState->mKeyStateMap;
				if( pEvt.uEvtInputKeyboard.mKeyCode == EKeyCode::Escape )
				{
					evtDispatcher->PostEventAppQuit();
				}
				else if( keyMap[EKeyCode::Enter] )
				{
					isFullscreen = !isFullscreen;
					gxDriverState.presentationLayer->SetFullscreenMode( isFullscreen );
				}
				if( keyMap[EKeyCode::CtrlLeft] )
				{
					Ic3DebugOutput("CTRL_LEFT");
				}
				if( keyMap[EKeyCode::CharW] )
				{
					cameraController.Move( 0.075f );
				}
				if( keyMap[EKeyCode::CharS] )
				{
					cameraController.Move( -0.075f );
				}
				if( keyMap[EKeyCode::CharD] )
				{
					cameraController.MoveSide( 0.075f );
				}
				if( keyMap[EKeyCode::CharA] )
				{
					cameraController.MoveSide( -0.075f );
				}
				if( keyMap[EKeyCode::Space] )
				{
					cameraController.MoveUpDown( 0.075f );
				}
				if( keyMap[EKeyCode::CharC] )
				{
					cameraController.MoveUpDown( -0.075f );
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

	InitializeGraphicsDriver( sysContext, gxDriverState );

	if( auto * eventSource = gxDriverState.presentationLayer->GetInternalSystemEventSource() )
	{
		evtController->RegisterEventSource( *eventSource );
	}

	CoreEngineState ces{sysContext, gxDriverState.device};

	auto * gpuDevicePtr = gxDriverState.device.get();

	GCI::RenderPassDescriptorHandle scrRenderPassDescriptor;
	{
		RenderPassDescriptorCreateInfo rpdCreateInfo;
		auto & rpConfig = rpdCreateInfo.passConfiguration;
		rpConfig.activeAttachmentsMask = eRTAttachmentMaskDefaultC0DS;
		rpConfig.activeAttachmentsNum = 2;
		rpConfig.colorAttachments[0].loadAction = ERenderPassAttachmentLoadAction::Clear;
		rpConfig.colorAttachments[0].storeAction = ERenderPassAttachmentStoreAction::Keep;
		rpConfig.colorAttachments[0].loadParameters.opClear.clearConfig.colorValue = {0.12f, 0.36f, 0.88f, 1.0f};
		rpConfig.colorAttachments[0].loadParameters.opClear.clearMask = eRenderTargetBufferFlagColorBit;
		rpConfig.depthStencilAttachment.loadAction = ERenderPassAttachmentLoadAction::Clear;
		rpConfig.depthStencilAttachment.storeAction = ERenderPassAttachmentStoreAction::Keep;
		rpConfig.depthStencilAttachment.loadParameters.opClear.clearConfig.depthValue = 1.0f;
		rpConfig.depthStencilAttachment.loadParameters.opClear.clearConfig.stencilValue = 0xFF;
		rpConfig.depthStencilAttachment.loadParameters.opClear.clearMask = eRenderTargetBufferMaskDepthStencil;

		scrRenderPassDescriptor = gpuDevicePtr->CreateRenderPassDescriptor( rpdCreateInfo );
	}

	while( runApp )
	{
		if( gxDriverState.pauseAnimation )
		{
			continue;
		}

		try
		{
			evtController->DispatchPendingEventsAuto();

			gxDriverState.cmdContext->BeginCommandSequence();
			gxDriverState.presentationLayer->BindRenderTarget( *gxDriverState.cmdContext );
			gxDriverState.cmdContext->BeginRenderPass( *scrRenderPassDescriptor );
			{
				gxDriverState.presentationLayer->InvalidateRenderTarget( *gxDriverState.cmdContext );
			}
			gxDriverState.cmdContext->EndRenderPass();
			gxDriverState.cmdContext->EndCommandSequence();
			gxDriverState.cmdContext->Submit();

			gxDriverState.presentationLayer->Present();

			// std::this_thread::sleep_for( std::chrono::milliseconds( 8 ) );
		}
		catch( ... )
		{
		}
	}

	return 0;
}

void InitializeGraphicsDriver( SysContextHandle pSysContext, GraphicsDriverState & pGxDriverState )
{
	GPUDriverCreateInfo gpuDriverCreateInfo;
	gpuDriverCreateInfo.sysContext = pSysContext;
	gpuDriverCreateInfo.configFlags = eGPUDriverConfigFlagEnableShaderDebugInfoBit;
	pGxDriverState.driver = pGxDriverState.driverInterface->CreateDriver( gpuDriverCreateInfo );

	GPUDeviceCreateInfo gpuDeviceCreateInfo;
	gpuDeviceCreateInfo.adapterID = 0;
	gpuDeviceCreateInfo.flags = eGPUDeviceCreateFlagsDefault;
	pGxDriverState.device = pGxDriverState.driver->CreateDevice( gpuDeviceCreateInfo );

	PresentationLayerCreateInfo presentationLayerCreateInfo;
	presentationLayerCreateInfo.screenRect.size.x = 1920;
	presentationLayerCreateInfo.screenRect.size.y = 1080;
	presentationLayerCreateInfo.displayConfigFlags = 0;
	// presentationLayerCreateInfo.displayConfigFlags = eOpenGLDisplaySurfaceCreateFlagFullscreenBit;/
	pGxDriverState.presentationLayer = pGxDriverState.driverInterface->CreateScreenPresentationLayer( *( pGxDriverState.device ), presentationLayerCreateInfo );

	pGxDriverState.device->SetPresentationLayer( pGxDriverState.presentationLayer );
	pGxDriverState.cmdContext = pGxDriverState.device->GetCommandSystem().AcquireCommandContext<CommandContextDirectGraphics>();
}
