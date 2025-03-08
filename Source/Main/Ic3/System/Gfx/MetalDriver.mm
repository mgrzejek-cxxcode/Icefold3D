
#include "MetalNative.h"
#include "../Core/DisplaySystem.h"

#include <Metal/MTLRenderPass.h>

namespace Ic3::System
{

	MetalDevice::MetalDevice( SysContextHandle pSysContext )
	: SysObject( std::move( pSysContext ) )
	, mDeviceData( std::make_unique<MetalDeviceData>() )
	{}

	MetalDevice::~MetalDevice() = default;

	MetalDeviceHandle MetalDevice::CreateDefault( SysContextHandle pSysContext )
	{
	@autoreleasepool
	{
		id<MTLDevice> defaultMTLDevice = MTLCreateSystemDefaultDevice();
		if( defaultMTLDevice == nil )
		{
			Ic3DebugInterrupt();
			return nullptr;
		}

		id<MTLCommandQueue> mainMTLCommandQueue = [defaultMTLDevice newCommandQueue];
		if( mainMTLCommandQueue == nil )
		{
			Ic3DebugInterrupt();
			return nullptr;
		}

		auto metalDevice = CreateSysObject<MetalDevice>( std::move( pSysContext ) );
		metalDevice->mDeviceData->mtlDevice = defaultMTLDevice;
		metalDevice->mDeviceData->mtlMainCmdQueue = mainMTLCommandQueue;

		return metalDevice;
	}
	}


	MetalSystemDriver::MetalSystemDriver( DisplayManagerHandle pDisplayManager )
	: SysObject( pDisplayManager->mSysContext )
	, mDriverData( std::make_unique<MetalSystemDriverData>() )
	, mDisplayManager( std::move( pDisplayManager ) )
	{}

	MetalSystemDriver::~MetalSystemDriver() noexcept = default;

	MetalDevice & MetalSystemDriver::InitializeDefaultDevice()
	{
		Ic3DebugAssert( !_defaultMetalDevice );
		_defaultMetalDevice = MetalDevice::CreateDefault( mSysContext );
		return *_defaultMetalDevice;
	}

	MetalDevice & MetalSystemDriver::GetDefaultDevice() noexcept
	{
		if( !_defaultMetalDevice )
		{
			_defaultMetalDevice = MetalDevice::CreateDefault( mSysContext );
		}

		return *_defaultMetalDevice;
	}

	MetalDevice & MetalSystemDriver::GetDefaultDevice() const
	{
		if( !_defaultMetalDevice )
		{
			Ic3Throw( 0 );
		}
		return *_defaultMetalDevice;
	}

	MetalDisplaySurfaceHandle MetalSystemDriver::CreateDisplaySurface(
			MetalDevice & pMetalDevice,
			const MetalDisplaySurfaceCreateInfo & pCreateInfo )
	{
		MetalDisplaySurfaceCreateInfo surfaceCreateInfo = pCreateInfo;

		if( pCreateInfo.flags.is_set( eMetalDisplaySurfaceCreateFlagFullscreenBit ) )
		{
			surfaceCreateInfo.frameGeometry.size = cxFrameSizeMax;
			surfaceCreateInfo.frameGeometry.style = EFrameStyle::Overlay;
		}
		else
		{
			surfaceCreateInfo.frameGeometry.position = pCreateInfo.frameGeometry.position;
			surfaceCreateInfo.frameGeometry.size = pCreateInfo.frameGeometry.size;
			surfaceCreateInfo.frameGeometry.style = pCreateInfo.frameGeometry.style;
		}

		surfaceCreateInfo.frameGeometry = mDisplayManager->ValidateFrameGeometry( surfaceCreateInfo.frameGeometry );

		auto displaySurface = _NativeCreateDisplaySurface( pMetalDevice, surfaceCreateInfo );

		auto * caMetalLayer = displaySurface->surfaceData->caMetalLayer;
		Ic3DebugAssert( caMetalLayer != nil );

		auto mtlDevice = pMetalDevice.mDeviceData->mtlDevice;
		[caMetalLayer setDevice:mtlDevice];

		return displaySurface;
	}


	MetalDisplaySurface::MetalDisplaySurface( MetalSystemDriverHandle pMTLSystemDriver, void * pNativeData )
	: Frame( pMTLSystemDriver->mSysContext )
	, surfaceData( std::make_unique<MetalDisplaySurfaceData>() )
	, mMetalDriver( std::move( pMTLSystemDriver ) )
	{
		SetEventSourceNativeData( pNativeData );
	}

	MetalDisplaySurface::~MetalDisplaySurface() noexcept
	{
		ResetEventSourceNativeData();
	}

	void MetalDisplaySurface::ClearColorBuffer()
	{
	@autoreleasepool
	{
		auto mtlDevice = mMetalDevice->mDeviceData->mtlDevice;
		auto mtlCommandQueue = mMetalDevice->mDeviceData->mtlMainCmdQueue;
		auto caMetalLayer = surfaceData->caMetalLayer;

		id<CAMetalDrawable> currentDrawable = [caMetalLayer nextDrawable];
		id<MTLTexture> texture = currentDrawable.texture;

		MTLRenderPassDescriptor * clearRPDescriptor = [MTLRenderPassDescriptor new];
		clearRPDescriptor.colorAttachments[0].texture = texture;
		clearRPDescriptor.colorAttachments[0].loadAction = MTLLoadActionClear;
		clearRPDescriptor.colorAttachments[0].storeAction = MTLStoreActionStore;
		clearRPDescriptor.colorAttachments[0].clearColor = MTLClearColorMake(0.12, 0.42, 0.92, 1.0);

		id<MTLCommandBuffer> commandBuffer = [mtlCommandQueue commandBuffer];
		id<MTLRenderCommandEncoder> commandEncoder = [commandBuffer renderCommandEncoderWithDescriptor:clearRPDescriptor];

		[commandEncoder endEncoding];
		[commandBuffer presentDrawable:currentDrawable];
		[commandBuffer commit];
	}
	}

	void MetalDisplaySurface::ResizeClientArea( const FrameSize & pSize )
	{
		FrameGeometry newFrameGeometry{};
		newFrameGeometry.position = cxFramePosAuto;
		newFrameGeometry.size = pSize;
		newFrameGeometry.style = EFrameStyle::Unspecified;

		newFrameGeometry = mMetalDriver->mDisplayManager->ValidateFrameGeometry( newFrameGeometry );

		const auto updateFlags = eFrameGeometryUpdateFlagPositionBit | eFrameGeometryUpdateFlagSizeClientAreaBit;
		_NativeUpdateGeometry( newFrameGeometry, updateFlags );
	}

	void MetalDisplaySurface::ResizeFrame( const FrameSize & pSize )
	{
		FrameGeometry newFrameGeometry{};
		newFrameGeometry.position = cxFramePosAuto;
		newFrameGeometry.size = pSize;
		newFrameGeometry.style = EFrameStyle::Unspecified;

		newFrameGeometry = mMetalDriver->mDisplayManager->ValidateFrameGeometry( newFrameGeometry );

		const auto updateFlags = eFrameGeometryUpdateFlagPositionBit | eFrameGeometryUpdateFlagSizeOuterRectBit;
		_NativeUpdateGeometry( newFrameGeometry, updateFlags );
	}

	void MetalDisplaySurface::SetFullscreenMode( bool pEnable )
	{
		_NativeSetFullscreenMode( pEnable );
	}

	void MetalDisplaySurface::SetTitle( const std::string & pTitleText )
	{
		_NativeSetTitle( pTitleText );
	}

	void MetalDisplaySurface::UpdateGeometry(
			const FrameGeometry & pFrameGeometry,
			cppx::bitmask<EFrameGeometryUpdateFlags> pUpdateFlags )
	{
		_NativeUpdateGeometry( pFrameGeometry, pUpdateFlags );
	}

	FrameSize MetalDisplaySurface::GetClientAreaSize() const
	{
		return _NativeGetSize( EFrameSizeMode::ClientArea );
	}

	FrameSize MetalDisplaySurface::GetFrameSize() const
	{
		return _NativeGetSize( EFrameSizeMode::OuterRect );
	}

	bool MetalDisplaySurface::IsFullscreen() const
	{
		return _NativeIsFullscreen();
	}

}
