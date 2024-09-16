
#ifndef __IC3_SYSTEM_METAL_DRIVER_H__
#define __IC3_SYSTEM_METAL_DRIVER_H__

#include "MetalCommon.h"

namespace Ic3::System
{

	struct MetalDeviceData;
	struct MetalDisplaySurfaceData;
	struct MetalSystemDriverData;

	/// @brief
	struct MetalDisplaySurfaceCreateInfo : public WindowCreateInfo
	{
		/// Creation flags, describing additional surface properties.
		TBitmask<EMetalSurfaceCreateFlags> mFlags = 0;
	};

	class MetalDevice : public SysObject
	{
	public:
		std::unique_ptr<MetalDeviceData> const mDeviceData;

	public:
		MetalDevice( SysContextHandle pSysContext );
		~MetalDevice();

		static MetalDeviceHandle createDefault( SysContextHandle pSysContext );
	};

	class MetalSystemDriver : public SysObject
	{
	public:
		std::unique_ptr<MetalSystemDriverData> const mDriverData;
		DisplayManagerHandle const mDisplayManager;

	public:
		explicit MetalSystemDriver( DisplayManagerHandle pDisplayManager );
		virtual ~MetalSystemDriver() noexcept;
		
		/// @brief
		IC3_ATTR_NO_DISCARD MetalDevice & initializeDefaultDevice();

		/// @brief
		IC3_ATTR_NO_DISCARD MetalDevice & getDefaultDevice() noexcept;

		/// @brief
		IC3_ATTR_NO_DISCARD MetalDevice & getDefaultDevice() const;

		/// @brief
		IC3_ATTR_NO_DISCARD MetalDisplaySurfaceHandle createDisplaySurface(
				MetalDevice & pMetalDevice,
				const MetalDisplaySurfaceCreateInfo & pCreateInfo );
		
	private:
		virtual MetalDisplaySurfaceHandle _nativeCreateDisplaySurface(
				MetalDevice & pMetalDevice,
				const MetalDisplaySurfaceCreateInfo & pCreateInfo ) = 0;

		virtual void _nativeDestroyDisplaySurface( MetalDisplaySurface & pDisplaySurface ) = 0;

	private:
		MetalDeviceHandle _defaultMetalDevice;
	};

	class MetalDisplaySurface : public Frame
	{
		friend class MetalSystemDriver;

	public:
		std::unique_ptr<MetalDisplaySurfaceData> const mSurfaceData;
		MetalSystemDriverHandle const mMetalDriver;
		MetalDeviceHandle const mMetalDevice;

	public:
		explicit MetalDisplaySurface( MetalSystemDriverHandle pMTLSystemDriver, void * pNativeData );
		virtual ~MetalDisplaySurface() noexcept;

		void clearColorBuffer();

		/// @copybrief Frame::resizeClientArea
		virtual void resizeClientArea( const FrameSize & pSize ) override final;

		/// @copybrief Frame::resizeFrame
		virtual void resizeFrame( const FrameSize & pSize ) override final;

		/// @copybrief Frame::setFullscreenMode
		virtual void setFullscreenMode( bool pEnable ) override final;

		/// @copybrief Frame::setTitle
		virtual void setTitle( const std::string & pTitleText ) override final;

		/// @copybrief Frame::updateGeometry
		virtual void updateGeometry( const FrameGeometry & pFrameGeometry,
		                             TBitmask<EFrameGeometryUpdateFlags> pUpdateFlags ) override final;

		/// @copybrief Frame::getClientAreaSize
		IC3_ATTR_NO_DISCARD virtual FrameSize getClientAreaSize() const override final;

		/// @copybrief Frame::getSize
		IC3_ATTR_NO_DISCARD virtual FrameSize getFrameSize() const override final;

		/// @copybrief Frame::isFullscreen
		IC3_ATTR_NO_DISCARD virtual bool isFullscreen() const override final;
		
	private:
		virtual FrameSize _nativeQueryRenderAreaSize() const = 0;

		virtual bool _nativeSysValidate() const = 0;

		virtual void _nativeResize( const FrameSize & pFrameSize, EFrameSizeMode pSizeMode ) = 0;

		virtual void _nativeSetFullscreenMode( bool pEnable ) = 0;

		virtual void _nativeSetTitle( const std::string & pTitle ) = 0;

		virtual void _nativeUpdateGeometry( const FrameGeometry & pFrameGeometry,
		                                    TBitmask<EFrameGeometryUpdateFlags> pUpdateFlags ) = 0;

		virtual FrameSize _nativeGetSize( EFrameSizeMode pSizeMode ) const = 0;

		virtual bool _nativeIsFullscreen() const = 0;

	private:
		struct MTLDisplaySurfacePrivateData;
		//std::unique_ptr<MTLDisplaySurfacePrivateData> _privateData;
	};

}

#endif // __IC3_SYSTEM_METAL_DRIVER_H__
