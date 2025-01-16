
#ifndef __IC3_SYSTEM_PLATFORM_OSAPI_X11_DISPLAY_SYSTEM_H__
#define __IC3_SYSTEM_PLATFORM_OSAPI_X11_DISPLAY_SYSTEM_H__

#include "X11Common.h"
#include <Ic3/System/DisplayConfiguration.h>
#include <Ic3/System/DisplaySystem.h>
#include <X11/extensions/Xrandr.h>
#include <cppx/version.h>
#include <unordered_map>

namespace Ic3::System
{

	ic3SysDeclareHandle( X11DisplayManager );
	ic3SysDeclareHandle( X11DisplayDriver );

	namespace Platform
	{

		struct X11DisplayManagerNativeData : public X11NativeDataCommon
		{
			uint32 mScreenDepth = 0;
			cppx::version mXRRVersion = CX_VERSION_UNKNOWN;
			XRRMonitorInfo mXRRDefaultMonitorInfo;
		};

		struct X11DisplayDriverNativeData : public X11NativeDataCommon
		{
			uint32 mScreenDepth = 0;
			XRRScreenResources * mXRRScreenResources = nullptr;
			XRRMonitorInfo * mXRRMonitorList = nullptr;
			int mXRRMonitorsNum = 0;
			XRRMonitorInfo * mXRRDefaultMonitorInfo = nullptr;
			std::unordered_map<RRMode, XRRModeInfo *> mXRRModeInfoMap;
		};

		struct X11DisplayAdapterNativeData : public X11NativeDataCommon
		{
		};

		struct X11DisplayOutputNativeData : public X11NativeDataCommon
		{
			RROutput mXRROutputID = eXIDNone;
			RRCrtc mXRRCrtcID = eXIDNone;
		};

		struct X11DisplayVideoModeNativeData : public X11NativeDataCommon
		{
			RRMode mXRRModeID = eXIDNone;
			XRRModeInfo * mXRRModeInfo = nullptr;
		};

	}

	using X11DisplayAdapter = X11NativeObject<DisplayAdapter, Platform::X11DisplayAdapterNativeData>;
	using X11DisplayOutput = X11NativeObject<DisplayOutput, Platform::X11DisplayOutputNativeData>;
	using X11DisplayVideoMode = X11NativeObject<DisplayVideoMode, Platform::X11DisplayVideoModeNativeData>;

	/// @brief Implementation of DisplayManager for the X11 subsystem.
	class X11DisplayManager : public X11NativeObject<DisplayManager, Platform::X11DisplayManagerNativeData>
	{
	public:
		explicit X11DisplayManager( SysContextHandle pSysContext );
		virtual ~X11DisplayManager() noexcept;

	private:
		void _InitializeX11DisplayManagerState();
		void _ReleaseX11DisplayManagerState();

		virtual DisplayDriverHandle _NativeCreateDisplayDriver() override final;

		virtual void _NativeQueryDefaultDisplayOffset( DisplayOffset & pOutOffset ) const override final;

		virtual void _NativeQueryDefaultDisplaySize( DisplaySize & pOutSize ) const override final;

		virtual void _NativeQueryMinWindowSize( DisplaySize & pOutSize ) const override final;
	};

	/// @brief Implementation of DisplayDriver for the X11 subsystem.
	class X11DisplayDriver : public X11NativeObject<DisplayDriver, Platform::X11DisplayDriverNativeData>
	{
	public:
		explicit X11DisplayDriver( X11DisplayManagerHandle pDisplayManager );
		virtual ~X11DisplayDriver() noexcept;

	private:
		void _InitializeX11DisplayDriverState();
		void _ReleaseX11DisplayDriverState();

		virtual void _NativeEnumDisplayDevices() override final;

		virtual void _NativeEnumVideoModes( DisplayOutput & pOutput, EColorFormat pColorFormat ) override final;

		virtual EColorFormat _NativeQueryDefaultSystemColorFormat() const override final;
	};

} // namespace Ic3::System

#endif // __IC3_SYSTEM_PLATFORM_OSAPI_X11_DISPLAY_SYSTEM_H__
