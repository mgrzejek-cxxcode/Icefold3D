
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
		cppx::bitmask<EMetalSurfaceCreateFlags> flags = 0;
	};

	class MetalDevice : public SysObject
	{
	public:
		std::unique_ptr<MetalDeviceData> const mDeviceData;

	public:
		MetalDevice( SysContextHandle pSysContext );
		~MetalDevice();

		static MetalDeviceHandle CreateDefault( SysContextHandle pSysContext );
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
		CPPX_ATTR_NO_DISCARD MetalDevice & InitializeDefaultDevice();

		/// @brief
		CPPX_ATTR_NO_DISCARD MetalDevice & GetDefaultDevice() noexcept;

		/// @brief
		CPPX_ATTR_NO_DISCARD MetalDevice & GetDefaultDevice() const;

		/// @brief
		CPPX_ATTR_NO_DISCARD MetalDisplaySurfaceHandle CreateDisplaySurface(
				MetalDevice & pMetalDevice,
				const MetalDisplaySurfaceCreateInfo & pCreateInfo );
		
	private:
		virtual MetalDisplaySurfaceHandle _NativeCreateDisplaySurface(
				MetalDevice & pMetalDevice,
				const MetalDisplaySurfaceCreateInfo & pCreateInfo ) = 0;

		virtual void _NativeDestroyDisplaySurface( MetalDisplaySurface & pDisplaySurface ) = 0;

	private:
		MetalDeviceHandle _defaultMetalDevice;
	};

	class MetalDisplaySurface : public Frame
	{
		friend class MetalSystemDriver;

	public:
		std::unique_ptr<MetalDisplaySurfaceData> const surfaceData;
		MetalSystemDriverHandle const mMetalDriver;
		MetalDeviceHandle const mMetalDevice;

	public:
		explicit MetalDisplaySurface( MetalSystemDriverHandle pMTLSystemDriver, void * pNativeData );
		virtual ~MetalDisplaySurface() noexcept;

		void ClearColorBuffer();

		/// @copybrief Frame::ResizeClientArea
		virtual void ResizeClientArea( const FrameSize & pSize ) override final;

		/// @copybrief Frame::ResizeFrame
		virtual void ResizeFrame( const FrameSize & pSize ) override final;

		/// @copybrief Frame::SetFullscreenMode
		virtual void SetFullscreenMode( bool pEnable ) override final;

		/// @copybrief Frame::SetTitle
		virtual void SetTitle( const std::string & pTitleText ) override final;

		/// @copybrief Frame::UpdateGeometry
		virtual void UpdateGeometry(
				const FrameGeometry & pFrameGeometry,
				cppx::bitmask<EFrameGeometryUpdateFlags> pUpdateFlags ) override final;

		/// @copybrief Frame::GetClientAreaSize
		CPPX_ATTR_NO_DISCARD virtual FrameSize GetClientAreaSize() const override final;

		/// @copybrief Frame::GetSize
		CPPX_ATTR_NO_DISCARD virtual FrameSize GetFrameSize() const override final;

		/// @copybrief Frame::IsFullscreen
		CPPX_ATTR_NO_DISCARD virtual bool IsFullscreen() const override final;
		
	private:
		virtual FrameSize _NativeQueryRenderAreaSize() const = 0;

		virtual bool _NativeSysValidate() const = 0;

		virtual void _NativeResize( const FrameSize & pFrameSize, EFrameSizeMode pSizeMode ) = 0;

		virtual void _NativeSetFullscreenMode( bool pEnable ) = 0;

		virtual void _NativeSetTitle( const std::string & pTitle ) = 0;

		virtual void _NativeUpdateGeometry(
				const FrameGeometry & pFrameGeometry,
				cppx::bitmask<EFrameGeometryUpdateFlags> pUpdateFlags ) = 0;

		virtual FrameSize _NativeGetSize( EFrameSizeMode pSizeMode ) const = 0;

		virtual bool _NativeIsFullscreen() const = 0;

	private:
		struct MTLDisplaySurfacePrivateData;
		//std::unique_ptr<MTLDisplaySurfacePrivateData> _privateData;
	};

}

#endif // __IC3_SYSTEM_METAL_DRIVER_H__
