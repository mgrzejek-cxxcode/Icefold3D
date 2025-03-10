
#ifndef __IC3_SAMPLES_GFX_TEST_MAIN_GFX_HXX__
#define __IC3_SAMPLES_GFX_TEST_MAIN_GFX_HXX__


#include <Ic3/System/Core/DisplaySystem.h>
#include <Ic3/System/SysContext.h>
#include <Ic3/System/Events/EventCore.h>
#include <Ic3/System/Events/EventObject.h>
#include <Ic3/System/Gfx/OpenGLDriver.h>
#include <Ic3/System/IO/AssetSystemNative.h>
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
#include <Ic3/Graphics/GCI/State/GraphicsPipelineStateDescriptorRTODynamic.h>
#include <Ic3/Graphics/GCI/State/GraphicsPipelineStateDescriptorIA.h>
#include <Ic3/Graphics/GCI/State/GraphicsPipelineStateDescriptorIADynamic.h>

#include <Ic3/NxMain/Camera/CameraController.h>
#include <Ic3/NxMain/GCI/VertexFormatSignature.h>
#include <Ic3/NxMain/GCI/VertexFormatUtils.h>
#include <Ic3/NxMain/Renderer/ShaderLibrary.h>
#include <Ic3/NxMain/Renderer/ShaderLoader.h>
#include <Ic3/NxMain/Renderer/SharedStateLibrary.h>

#include <chrono>
#include <thread>

#include "MeshDefs.h"

using namespace Ic3;
using namespace GCI;
using namespace Ic3::Math;
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

struct CB0Data
{
	Mat4x4f modelMatrix;
	Mat4x4f viewMatrix;
	Mat4x4f projectionMatrix;
};

#include <Ic3/Graphics/HW3D/GL4/GL4GPUDriverAPI.h>

int main( int pArgc, const char ** pArgv )
{
//    {
//		VertexFormatSignatureBuilder vfsBuilder{};
//		vfsBuilder.AddAttribute( 0, kVertexAttributeKeySysPosition );
//		vfsBuilder.AddAttribute( 0, kVertexAttributeKeySysNormal );
//		vfsBuilder.AddAttribute( 0, kVertexAttributeKeySysTangent );
//		vfsBuilder.AddAttribute( 0, kVertexAttributeKeySysBiTangent );
//		vfsBuilder.AddAttribute( 0, kVertexAttributeKeySysTexCoord2D0 );
//		vfsBuilder.AddAttribute( 0, 6, { "CustomPosRef" }, GCI::EVertexAttribFormat::Vec4F32 );
//		vfsBuilder.AddAttribute( 1, kVertexAttributeKeySysInstanceMatrix );
//		vfsBuilder.AddAttribute( 2, kVertexAttributeKeySysInstanceUserData );
//		auto vfd = vfsBuilder.CreateVertexFormatSignature( GCI::EPrimitiveTopology::TriangleList, GCI::EIndexDataFormat::Uint32 );
//
//		const auto sid1 = vfd->GenerateSerialString();
//		const auto defArray0 = vfd->GenerateAttributeDefinitionArray();
//		const auto defArray1 = GCIUtils::ParseVertexFormatSignatureSerialString( sid1 );
//
//		VertexFormatSignatureBuilder vfsBuilder0{};
//		vfsBuilder0.AddAttributeList( cppx::bind_array_view( defArray0 ) );
//		auto vfdCopy0 = vfsBuilder0.CreateVertexFormatSignature( GCI::EPrimitiveTopology::TriangleList, GCI::EIndexDataFormat::Uint32 );
//		const auto sidCopy0 = vfd->GenerateSerialString();
//
//		VertexFormatSignatureBuilder vfsBuilder1{};
//		vfsBuilder1.AddAttributeList( cppx::bind_array_view( defArray1 ) );
//		auto vfdCopy1 = vfsBuilder1.CreateVertexFormatSignature( GCI::EPrimitiveTopology::TriangleList, GCI::EIndexDataFormat::Uint32 );
//		const auto sidCopy1 = vfd->GenerateSerialString();
//
//		Ic3DebugOutput( sid1.c_str() );
//		Ic3DebugOutput( sidCopy0.c_str() );
//		Ic3DebugOutput( sidCopy1.c_str() );
//    }

	sGxDriverName = "GL4";

	const auto mm1 = std::chrono::microseconds( 200 );

	const auto ms2 = std::chrono::duration_cast<std::chrono::milliseconds>( mm1 );

	const auto mm3 = cppx::duration<cppx::duration_unit::second>(1).get_as<cppx::duration_unit::nanosecond>();
	const auto mm4 = cppx::duration_from_std( std::chrono::milliseconds( 450 ) ).get_as<cppx::duration_unit::second, double>();

	SysContextCreateInfo sysContextCreateInfo;
	auto sysContext = Platform::CreateSysContext( sysContextCreateInfo );

	Platform::AssetLoaderCreateInfoNativeParams aslCreateInfoNP;
	aslCreateInfoNP.relativeAssetRootDir = "Assets";
	AssetLoaderCreateInfo aslCreateInfo;
	aslCreateInfo.nativeParams = &aslCreateInfoNP;
	auto assetLoader = sysContext->CreateAssetLoader( aslCreateInfo );

	GraphicsDriverState gxDriverState;
	gxDriverState.driverID = "GL4";
	gxDriverState.driverInterface = std::make_unique<GL4GPUDriverInterface>();

	srand( time( nullptr ) );

	bool runApp = true;

	auto evtController = sysContext->CreateEventController();
	auto evtDispatcher = evtController->CreateEventDispatcher();
	evtController->SetActiveEventDispatcher( *evtDispatcher );

	bool isFullscreen = true;
	bool updateLightPosition = true;

	CameraController cameraController;

	evtDispatcher->SetEventHandler(
		EEventCodeIndex::AppActivityQuit,
		[&runApp]( const EventObject & pEvt ) -> bool {
		runApp = false;
		return true;
	} );
	evtDispatcher->SetEventHandler(
		EEventCodeIndex::WindowUpdateDestroy,
		[evtDispatcher, &gxDriverState]( const EventObject & pEvt ) -> bool {
		if( pEvt.uEvtWindowUpdateDestroy.CheckEventSource( gxDriverState.presentationLayer->GetInternalSystemEventSource() ) )
		{
			evtDispatcher->PostEventAppQuit();
		}
		return true;
	} );
	evtDispatcher->SetEventHandler(
		EEventCodeIndex::InputKeyboard,
		[&]( const EventObject & pEvt ) -> bool {
		auto & keyMap = pEvt.uEvtInputKeyboard.inputKeyboardState->keyStateMap;
		if( pEvt.uEvtInputKeyboard.keyCode == EKeyCode::Escape )
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
			Ic3DebugOutput( "CTRL_LEFT" );
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
	} );

	InitializeGraphicsDriver( sysContext, gxDriverState );

	if( auto * eventSource = gxDriverState.presentationLayer->GetInternalSystemEventSource() )
	{
		evtController->RegisterEventSource( *eventSource );
	}

	CoreEngineState ces{ sysContext, gxDriverState.device };

	auto shaderLibrary = ShaderLoaderFileBased::CreateLoader( ces, assetLoader, "shaders/" + sGxDriverName )->LoadShadersIntoNewLibrary( {
			ShaderLoadDescFile{ Ic3ShaderLoadDescVSAutoID( "SID_DEFAULT_PASSTHROUGH_VS" ), "default_passthrough_vs" },
			ShaderLoadDescFile{ Ic3ShaderLoadDescPSAutoID( "SID_DEFAULT_PASSTHROUGH_PS" ), "default_passthrough_ps" }
		} );

	auto shader = shaderLibrary->GetShader( "SID_DEFAULT_PASSTHROUGH_VS" );

	auto gciSharedStateLibrary = CreateDynamicObject<GCISharedStateLibrary>( *shaderLibrary );
	gciSharedStateLibrary->Initialize();

	auto * gpuDevicePtr = gxDriverState.device.get();
	const auto rtSize = gxDriverState.presentationLayer->QueryRenderTargetSize();

	GCI::GPUBufferHandle cbuffer0;
	{
		GCI::GPUBufferCreateInfo cbci;
		cbci.memoryFlags = GCI::eGPUMemoryAccessFlagGPUReadBit;
		cbci.resourceFlags = GCI::eGPUResourceContentFlagDynamicBit;
		cbci.resourceFlags |= GCI::eGPUBufferBindFlagConstantBufferBit;
		cbci.bufferSize = sizeof( CB0Data );
		cbuffer0 = gxDriverState.device->CreateGPUBuffer( cbci );
	}

	const auto vtx0ElementsNum = cppx::static_array_size( cvMeshTexUnitCubeVertexData );
	GCI::GPUBufferHandle vbuffer0;
	{
		const auto vtxDataSize = sizeof( cvMeshTexUnitCubeVertexData );
		GCI::GPUBufferCreateInfo vbci;
		vbci.memoryFlags = GCI::eGPUMemoryAccessFlagGPUReadBit;
		vbci.resourceFlags = GCI::eGPUResourceContentFlagStaticBit;
		vbci.resourceFlags |= GCI::eGPUBufferBindFlagVertexBufferBit;
		vbci.initDataDesc.pointer = cvMeshTexUnitCubeVertexData;
		vbci.initDataDesc.size = vtxDataSize;
		vbuffer0 = gxDriverState.device->CreateGPUBuffer( vbci );
	}

	const auto idx0ElementsNum = cppx::static_array_size( cvMeshTexUnitCubeIndexData );
	GCI::GPUBufferHandle ibuffer0;
	{
		const auto idxDataSize = sizeof( cvMeshTexUnitCubeIndexData );
		GCI::GPUBufferCreateInfo ibci;
		ibci.memoryFlags = GCI::eGPUMemoryAccessFlagGPUReadBit;
		ibci.resourceFlags = GCI::eGPUResourceContentFlagStaticBit;
		ibci.resourceFlags |= GCI::eGPUBufferBindFlagIndexBufferBit;
		ibci.initDataDesc.pointer = cvMeshTexUnitCubeIndexData;
		ibci.initDataDesc.size = idxDataSize;
		ibuffer0 = gxDriverState.device->CreateGPUBuffer( ibci );
	}

	auto vsbDescriptor = VertexSourceBindingDescriptorDynamic::CreateNew( *gxDriverState.device );
	IAVertexBufferReference vbRef{};
	vbRef.sourceBuffer = vbuffer0;
	vbRef.relativeOffset = 0;
	vbRef.refParams.vertexStride = sizeof( VertexPNT0 );
	IAIndexBufferReference ibRef{};
	ibRef.sourceBuffer = ibuffer0;
	ibRef.relativeOffset = 0;
	ibRef.refParams.indexFormat = EIndexDataFormat::Uint32;
	vsbDescriptor->SetVertexBufferReference( 0, vbRef );
	vsbDescriptor->SetIndexBufferReference( ibRef );

	GCI::TextureHandle texRTColor0;
	GCI::RenderTargetTextureHandle texRTColor0RT;
	{
		GCI::TextureCreateInfo texRTColor0CI;
		texRTColor0CI.texClass = GCI::ETextureClass::T2D;
		texRTColor0CI.dimensions.width = rtSize.x;
		texRTColor0CI.dimensions.height = rtSize.y;
		texRTColor0CI.memoryFlags = GCI::eGPUMemoryAccessFlagGPUReadBit;
		texRTColor0CI.resourceFlags = GCI::eGPUResourceUsageFlagRenderTargetColorBit | GCI::eGPUResourceUsageFlagShaderInputBit;
		texRTColor0CI.internalFormat = GCI::ETextureFormat::RGBA8UN;
		texRTColor0 = gpuDevicePtr->CreateTexture( texRTColor0CI );

		GCI::RenderTargetTextureCreateInfo texRTColor0RTCI;
		texRTColor0RTCI.targetTexture = texRTColor0;
		texRTColor0RTCI.bindFlags = GCI::eTextureBindFlagRenderTargetColorAttachmentBit | GCI::eTextureBindFlagShaderInputSampledImageBit;
		texRTColor0RT = gpuDevicePtr->CreateRenderTargetTexture( texRTColor0RTCI );
	}

	GCI::TextureHandle texRTDepthStencil;
	GCI::RenderTargetTextureHandle texRTDepthStencilRT;
	{
		GCI::TextureCreateInfo texRTDepthStencilCI;
		texRTDepthStencilCI.texClass = GCI::ETextureClass::T2D;
		texRTDepthStencilCI.dimensions.width = rtSize.x;
		texRTDepthStencilCI.dimensions.height = rtSize.y;
		texRTDepthStencilCI.memoryFlags = GCI::eGPUMemoryAccessFlagGPUReadBit;
		texRTDepthStencilCI.resourceFlags = GCI::eGPUResourceUsageMaskRenderTargetDepthStencil;
		texRTDepthStencilCI.internalFormat = GCI::ETextureFormat::D24UNS8U;
		texRTDepthStencil = gpuDevicePtr->CreateTexture( texRTDepthStencilCI );

		GCI::RenderTargetTextureCreateInfo texRTDepthStencilRTCI;
		texRTDepthStencilRTCI.targetTexture = texRTDepthStencil;
		texRTDepthStencilRTCI.bindFlags = GCI::eTextureBindFlagRenderTargetDepthStencilAttachmentBit;
		texRTDepthStencilRT = gpuDevicePtr->CreateRenderTargetTexture( texRTDepthStencilRTCI );
	}

	GCI::RenderPassDescriptorHandle scrRenderPassDescriptor;
	{
		RenderPassDescriptorCreateInfo rpdCreateInfo;
		auto & rpConfig = rpdCreateInfo.passConfiguration;
		rpConfig.activeAttachmentsMask = eRTAttachmentMaskDefaultC0DS;
		rpConfig.activeAttachmentsNum = 2;
		rpConfig.colorAttachments[0].loadAction = ERenderPassAttachmentLoadAction::Clear;
		rpConfig.colorAttachments[0].storeAction = ERenderPassAttachmentStoreAction::Keep;
		rpConfig.colorAttachments[0].loadParameters.opClear.clearConfig.colorValue = gpuDevicePtr->GetDefaultClearColor();
		rpConfig.colorAttachments[0].loadParameters.opClear.clearMask = eRenderTargetBufferFlagColorBit;
		rpConfig.depthStencilAttachment.loadAction = ERenderPassAttachmentLoadAction::Clear;
		rpConfig.depthStencilAttachment.storeAction = ERenderPassAttachmentStoreAction::Keep;
		rpConfig.depthStencilAttachment.loadParameters.opClear.clearConfig.depthValue = 1.0f;
		rpConfig.depthStencilAttachment.loadParameters.opClear.clearConfig.stencilValue = 0xFF;
		rpConfig.depthStencilAttachment.loadParameters.opClear.clearMask = eRenderTargetBufferMaskDepthStencil;

		scrRenderPassDescriptor = gpuDevicePtr->CreateRenderPassDescriptor( rpdCreateInfo );
	}

	GCI::RenderPassDescriptorHandle offScrRenderPassDescriptor;
	{
		RenderPassDescriptorCreateInfo rpdCreateInfo;
		auto & rpConfig = rpdCreateInfo.passConfiguration;
		rpConfig.activeAttachmentsMask = eRTAttachmentMaskDefaultC0DS;
		rpConfig.activeAttachmentsNum = 2;
		rpConfig.colorAttachments[0].loadAction = ERenderPassAttachmentLoadAction::Clear;
		rpConfig.colorAttachments[0].storeAction = ERenderPassAttachmentStoreAction::Keep;
		rpConfig.colorAttachments[0].loadParameters.opClear.clearConfig.colorValue = RGBAColorR32Norm{ 0.6f, 0.6f, 0.6, 1.0f };
		rpConfig.colorAttachments[0].loadParameters.opClear.clearMask = eRenderTargetBufferFlagColorBit;
		rpConfig.depthStencilAttachment.loadAction = ERenderPassAttachmentLoadAction::Clear;
		rpConfig.depthStencilAttachment.storeAction = ERenderPassAttachmentStoreAction::Keep;
		rpConfig.depthStencilAttachment.loadParameters.opClear.clearConfig.depthValue = 1.0f;
		rpConfig.depthStencilAttachment.loadParameters.opClear.clearConfig.stencilValue = 0xFF;
		rpConfig.depthStencilAttachment.loadParameters.opClear.clearMask = eRenderTargetBufferMaskDepthStencil;

		offScrRenderPassDescriptor = gpuDevicePtr->CreateRenderPassDescriptor( rpdCreateInfo );
	}

	const auto kSDIDRootSignatureDefault = GCI::CXU::DeclarePipelineStateDescriptorIDRootSignature( 0x001 );
	RootSignatureDescriptorHandle rsDescriptor;
	{
		RootSignatureDescriptorCreateInfo rsDescriptorCI;
		rsDescriptorCI.descriptorID = kSDIDRootSignatureDefault;
		rsDescriptorCI.rootSignatureDesc.activeShaderStagesMask = GCI::eShaderStageFlagGraphicsVertexBit | GCI::eShaderStageFlagGraphicsPixelBit;
		rsDescriptorCI.rootSignatureDesc.descriptorSetsNum = 1;
		rsDescriptorCI.rootSignatureDesc.descriptorSetArray[0].descriptorType = GCI::EShaderInputDescriptorType::Resource;
		rsDescriptorCI.rootSignatureDesc.descriptorSetArray[0].descriptorsNum = 1;
		rsDescriptorCI.rootSignatureDesc.descriptorSetArray[0].descriptorList[0] = { 0, GCI::EShaderInputDescriptorType::Resource, GCI::eShaderStageFlagGraphicsVertexBit };
		rsDescriptorCI.rootSignatureDesc.descriptorSetArray[0].descriptorList[0].uResourceDesc = { GCI::EShaderInputResourceType::CBVConstantBuffer, 0, 1 };
		rsDescriptor = gpuDevicePtr->CreateRootSignatureDescriptor( rsDescriptorCI );
	}

	const auto kSDIDGraphicsShaderLinkageDefault = GCI::CXU::DeclarePipelineStateDescriptorIDGraphicsShaderLinkage( 0x001 );
	GraphicsShaderLinkageDescriptorHandle gslDescriptor;
	{
		GraphicsShaderLinkageDescriptorCreateInfo gslDescriptorCI;
		gslDescriptorCI.descriptorID = kSDIDGraphicsShaderLinkageDefault;
		gslDescriptorCI.shaderBinding.AddShader( shaderLibrary->GetShader( "SID_DEFAULT_PASSTHROUGH_VS" ) );
		gslDescriptorCI.shaderBinding.AddShader( shaderLibrary->GetShader( "SID_DEFAULT_PASSTHROUGH_PS" ) );
		gslDescriptor = gpuDevicePtr->CreateGraphicsShaderLinkageDescriptor( gslDescriptorCI );
	}

	GraphicsPipelineStateObjectHandle mainPSO;
	{
		GraphicsPipelineStateObjectCreateInfo psoCI;
		psoCI.blendStateDescriptorID = GID::kGfxIDStateDescriptorBlendDefault;
		psoCI.depthStencilStateDescriptorID = GID::kGfxIDStateDescriptorDepthStencilDepthTestEnable;
		psoCI.rasterizerStateDescriptorID = GID::kGfxIDStateDescriptorRasterizerSolidCullBackCCW;
		psoCI.vertexAttributeLayoutDescriptorID = GID::kGfxIDStateDescriptorIAVertexAttributeLayoutDefault;

		psoCI.shaderLinkageStateDescriptor = gslDescriptor;
		psoCI.rootSignatureDescriptor = rsDescriptor;

		psoCI.renderTargetLayout.activeAttachmentsMask = eRTAttachmentMaskDefaultC0DS;
		psoCI.renderTargetLayout.activeAttachmentsNum = 2;
		psoCI.renderTargetLayout.colorAttachments[0].format = ETextureFormat::BGRA8UN;
		psoCI.renderTargetLayout.depthStencilAttachment.format = ETextureFormat::D24UNS8U;
		mainPSO = gpuDevicePtr->CreateGraphicsPipelineStateObject( psoCI );
	}

	Vec3f cameraOriginPoint{ 0.0f, 2.0f, -4.0f };
	Vec3f cameraTargetPoint{ 0.0f, 0.0f, 4.0f };
	cameraController.Initialize( cameraOriginPoint, cameraTargetPoint, 60.0f );

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

	const auto Ic3ViewTexture = lookAtLH(
		Vec3f{ 0.0f, 3.0f, -1.0f },
		Vec3f{ 0.0f, 0.0f, 5.0f },
		Vec3f{ 0.0f, 1.0f, 0.0f } );

	const auto Ic3ProjectionTexture = perspectiveAspectLH<float>(
		cameraController.GetPerspectiveFOVAngle(), ( float )rtSize.x / ( float )rtSize.y, 0.1f, 1000.0f );

	const auto Ic3CameraProjection = perspectiveAspectLH<float>(
		cameraController.GetPerspectiveFOVAngle(), ( float )rtSize.x / ( float )rtSize.y, 1.0f, 1000.0f );

	CB0Data cb0DataBase =
	{
		identity4<float>(),
		identity4<float>(),
		identity4<float>()
	};

	GPUBufferDataUploadDesc cb0DataUploadDesc;
	cb0DataUploadDesc.inputDataDesc.pointer = &cb0DataBase;
	cb0DataUploadDesc.inputDataDesc.size = sizeof( CB0Data );

	bool rotate = false;

	evtDispatcher->SetEventHandler(
		EEventCodeIndex::InputMouseButton,
		[&rotate]( const EventObject & pEvt ) -> bool {
		const auto & eButton = pEvt.uEvtInputMouseButton;
		if( eButton.buttonAction == EMouseButtonActionType::Click )
		{
			rotate = true;
		}
		else if( eButton.buttonAction == EMouseButtonActionType::Release )
		{
			rotate = false;
		}
		return true;
	} );
	evtDispatcher->SetEventHandler(
		EEventCodeIndex::InputMouseMove,
		[&cameraController, &rotate]( const EventObject & pEvt ) -> bool {
			//if( rotate )
			{
				const auto & emove = pEvt.uEvtInputMouseMove;
				if( emove.buttonStateMask.is_set( eMouseButtonFlagLeftBit ) )
				{
					cameraController.RotateAroundOrigin( emove.movementDelta.x, emove.movementDelta.y );
				}
				else if( emove.buttonStateMask.is_set( eMouseButtonFlagRightBit ) )
				{
					cameraController.RotateAroundTarget( emove.movementDelta.x, emove.movementDelta.y );
				}
			}
			return true;
	} );

	while( true )
	{
		if( gxDriverState.pauseAnimation )
		{
			continue;
		}

		try
		{
			evtController->DispatchPendingEventsAuto();
			if( !runApp )
			{
				break;
			}

			cb0DataBase.projectionMatrix = Ic3CameraProjection;
			cb0DataBase.viewMatrix = cameraController.ComputeViewMatrixLH();

			gxDriverState.presentationLayer->BindRenderTarget( *gxDriverState.cmdContext );

			gxDriverState.cmdContext->BeginCommandSequence();
			gxDriverState.cmdContext->SetVertexSourceBindingDescriptorDynamic( *vsbDescriptor );
			gxDriverState.cmdContext->SetGraphicsPipelineStateObject( *mainPSO );
			gxDriverState.cmdContext->BeginRenderPass( *scrRenderPassDescriptor );
			{
				gxDriverState.cmdContext->UpdateBufferDataUpload( *cbuffer0, cb0DataBase );


				gxDriverState.cmdContext->CmdSetViewport( vpDescScreen );
				gxDriverState.cmdContext->CmdSetShaderConstantBuffer( 0, *cbuffer0 );

				gxDriverState.cmdContext->CmdDrawDirectIndexed( 36, 0 );

			}
			gxDriverState.cmdContext->EndRenderPass();
			gxDriverState.cmdContext->EndCommandSequence();
			gxDriverState.cmdContext->Submit();

			gxDriverState.presentationLayer->InvalidateRenderTarget( *gxDriverState.cmdContext );
			gxDriverState.presentationLayer->Present();
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


#endif
