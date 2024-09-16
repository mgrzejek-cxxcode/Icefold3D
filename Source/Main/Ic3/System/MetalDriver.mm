
#include "MetalNative.h"
#include "DisplaySystem.h"

#include <Metal/MTLRenderPass.h>

namespace Ic3::System
{

	MetalDevice::MetalDevice( SysContextHandle pSysContext )
	: SysObject( std::move( pSysContext ) )
	, mDeviceData( std::make_unique<MetalDeviceData>() )
	{}

	MetalDevice::~MetalDevice() = default;

	MetalDeviceHandle MetalDevice::createDefault( SysContextHandle pSysContext )
	{
	@autoreleasepool
	{
		id<MTLDevice> defaultMTLDevice = MTLCreateSystemDefaultDevice();
		if( defaultMTLDevice == nil )
		{
			ic3DebugInterrupt();
			return nullptr;
		}

		id<MTLCommandQueue> mainMTLCommandQueue = [defaultMTLDevice newCommandQueue];
		if( mainMTLCommandQueue == nil )
		{
			ic3DebugInterrupt();
			return nullptr;
		}

		auto metalDevice = createSysObject<MetalDevice>( std::move( pSysContext ) );
		metalDevice->mDeviceData->mMTLDevice = defaultMTLDevice;
		metalDevice->mDeviceData->mMTLMainCmdQueue = mainMTLCommandQueue;

		return metalDevice;
	}
	}


	MetalSystemDriver::MetalSystemDriver( DisplayManagerHandle pDisplayManager )
	: SysObject( pDisplayManager->mSysContext )
	, mDriverData( std::make_unique<MetalSystemDriverData>() )
	, mDisplayManager( std::move( pDisplayManager ) )
	{}

	MetalSystemDriver::~MetalSystemDriver() noexcept = default;

	MetalDevice & MetalSystemDriver::initializeDefaultDevice()
	{
		ic3DebugAssert( !_defaultMetalDevice );
		_defaultMetalDevice = MetalDevice::createDefault( mSysContext );
		return *_defaultMetalDevice;
	}

	MetalDevice & MetalSystemDriver::getDefaultDevice() noexcept
	{
		if( !_defaultMetalDevice )
		{
			_defaultMetalDevice = MetalDevice::createDefault( mSysContext );
		}

		return *_defaultMetalDevice;
	}

	MetalDevice & MetalSystemDriver::getDefaultDevice() const
	{
		if( !_defaultMetalDevice )
		{
			ic3Throw( 0 );
		}
		return *_defaultMetalDevice;
	}

	MetalDisplaySurfaceHandle MetalSystemDriver::createDisplaySurface(
			MetalDevice & pMetalDevice,
			const MetalDisplaySurfaceCreateInfo & pCreateInfo )
	{
		MetalDisplaySurfaceCreateInfo surfaceCreateInfo = pCreateInfo;

		if( pCreateInfo.mFlags.isSet( eMetalDisplaySurfaceCreateFlagFullscreenBit ) )
		{
			surfaceCreateInfo.mFrameGeometry.mSize = cxFrameSizeMax;
			surfaceCreateInfo.mFrameGeometry.mStyle = EFrameStyle::Overlay;
		}
		else
		{
			surfaceCreateInfo.mFrameGeometry.mPosition = pCreateInfo.mFrameGeometry.mPosition;
			surfaceCreateInfo.mFrameGeometry.mSize = pCreateInfo.mFrameGeometry.mSize;
			surfaceCreateInfo.mFrameGeometry.mStyle = pCreateInfo.mFrameGeometry.mStyle;
		}

		surfaceCreateInfo.mFrameGeometry = mDisplayManager->validateFrameGeometry( surfaceCreateInfo.mFrameGeometry );

		auto displaySurface = _nativeCreateDisplaySurface( pMetalDevice, surfaceCreateInfo );

		auto * caMetalLayer = displaySurface->mSurfaceData->mCAMetalLayer;
		ic3DebugAssert( caMetalLayer != nil );

		auto mtlDevice = pMetalDevice.mDeviceData->mMTLDevice;
		[caMetalLayer setDevice:mtlDevice];

		return displaySurface;
	}


	MetalDisplaySurface::MetalDisplaySurface( MetalSystemDriverHandle pMTLSystemDriver, void * pNativeData )
	: Frame( pMTLSystemDriver->mSysContext )
	, mSurfaceData( std::make_unique<MetalDisplaySurfaceData>() )
	, mMetalDriver( std::move( pMTLSystemDriver ) )
	{
		setEventSourceNativeData( pNativeData );
	}

	MetalDisplaySurface::~MetalDisplaySurface() noexcept
	{
		resetEventSourceNativeData();
	}

	void MetalDisplaySurface::clearColorBuffer()
	{
	@autoreleasepool
	{
		auto mtlDevice = mMetalDevice->mDeviceData->mMTLDevice;
		auto mtlCommandQueue = mMetalDevice->mDeviceData->mMTLMainCmdQueue;
		auto caMetalLayer = mSurfaceData->mCAMetalLayer;

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

	void MetalDisplaySurface::resizeClientArea( const FrameSize & pSize )
	{
		FrameGeometry newFrameGeometry{};
		newFrameGeometry.mPosition = cxFramePosAuto;
		newFrameGeometry.mSize = pSize;
		newFrameGeometry.mStyle = EFrameStyle::Unspecified;

		newFrameGeometry = mMetalDriver->mDisplayManager->validateFrameGeometry( newFrameGeometry );

		const auto updateFlags = eFrameGeometryUpdateFlagPositionBit | eFrameGeometryUpdateFlagSizeClientAreaBit;
		_nativeUpdateGeometry( newFrameGeometry, updateFlags );
	}

	void MetalDisplaySurface::resizeFrame( const FrameSize & pSize )
	{
		FrameGeometry newFrameGeometry{};
		newFrameGeometry.mPosition = cxFramePosAuto;
		newFrameGeometry.mSize = pSize;
		newFrameGeometry.mStyle = EFrameStyle::Unspecified;

		newFrameGeometry = mMetalDriver->mDisplayManager->validateFrameGeometry( newFrameGeometry );

		const auto updateFlags = eFrameGeometryUpdateFlagPositionBit | eFrameGeometryUpdateFlagSizeOuterRectBit;
		_nativeUpdateGeometry( newFrameGeometry, updateFlags );
	}

	void MetalDisplaySurface::setFullscreenMode( bool pEnable )
	{
		_nativeSetFullscreenMode( pEnable );
	}

	void MetalDisplaySurface::setTitle( const std::string & pTitleText )
	{
		_nativeSetTitle( pTitleText );
	}

	void MetalDisplaySurface::updateGeometry( const FrameGeometry & pFrameGeometry,
	                                          TBitmask<EFrameGeometryUpdateFlags> pUpdateFlags )
	{
		_nativeUpdateGeometry( pFrameGeometry, pUpdateFlags );
	}

	FrameSize MetalDisplaySurface::getClientAreaSize() const
	{
		return _nativeGetSize( EFrameSizeMode::ClientArea );
	}

	FrameSize MetalDisplaySurface::getFrameSize() const
	{
		return _nativeGetSize( EFrameSizeMode::OuterRect );
	}

	bool MetalDisplaySurface::isFullscreen() const
	{
		return _nativeIsFullscreen();
	}

}
