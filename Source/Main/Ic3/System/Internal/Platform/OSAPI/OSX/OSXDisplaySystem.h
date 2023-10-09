
#ifndef __IC3_SYSTEM_PLATFORM_OSAPI_OSX_DISPLAY_SYSTEM_H__
#define __IC3_SYSTEM_PLATFORM_OSAPI_OSX_DISPLAY_SYSTEM_H__

#include "OSXSysContext.h"
#include <Ic3/System/DisplayConfiguration.h>
#include <Ic3/System/DisplaySystem.h>

#import <CoreGraphics/CGDirectDisplay.h>

namespace Ic3::System
{

	ic3SysDeclareHandle( OSXDisplayManager );
	ic3SysDeclareHandle( OSXDisplayDriver );

	namespace platform
	{

		struct OSXDisplayManagerNativeData : public OSXNativeDataCommon
		{
			std::unique_ptr<CGDirectDisplayID[]> cgActiveDisplayList;
			CGDisplayCount cgActiveDisplaysNum = 0;
			CGDirectDisplayID cgMainDisplayID = kCGNullDirectDisplay;
		};

		struct OSXDisplayDriverNativeData : public OSXNativeDataCommon
		{
		};

		struct OSXDisplayAdapterNativeData : public OSXNativeDataCommon
		{
		};

		struct OSXDisplayOutputNativeData : public OSXNativeDataCommon
		{
		};

		struct OSXDisplayVideoModeNativeData : public OSXNativeDataCommon
		{
		};

		IC3_SYSTEM_API_NODISCARD ScreenRect osxQueryDisplayRect( CGDirectDisplayID pCGDisplayID );

		IC3_SYSTEM_API_NODISCARD DisplaySize osxQueryDisplaySize( CGDirectDisplayID pCGDisplayID );

	}

	using OSXDisplayAdapter = OSXNativeObject<DisplayAdapter, platform::OSXDisplayAdapterNativeData>;
	using OSXDisplayOutput = OSXNativeObject<DisplayOutput, platform::OSXDisplayOutputNativeData>;
	using OSXDisplayVideoMode = OSXNativeObject<DisplayVideoMode, platform::OSXDisplayVideoModeNativeData>;

	/// @brief
	class OSXDisplayManager : public OSXNativeObject<DisplayManager, platform::OSXDisplayManagerNativeData>
	{
	public:
		explicit OSXDisplayManager( SysContextHandle pSysContext );
		virtual ~OSXDisplayManager() noexcept;

	private:
		void _initializeOSXDisplayManagerState();
		void _releaseOSXDisplayManagerState();
		
		virtual DisplayDriverHandle _nativeCreateDisplayDriver() override final;

		virtual void _nativeQueryDefaultDisplayOffset( DisplayOffset & pOutOffset ) const override final;

		virtual void _nativeQueryDefaultDisplaySize( DisplaySize & pOutSize ) const override final;

		virtual void _nativeQueryMinWindowSize( DisplaySize & pOutSize ) const override final;
	};

	/// @brief
	class OSXDisplayDriver : public OSXNativeObject<DisplayDriver, platform::OSXDisplayDriverNativeData>
	{
	public:
		explicit OSXDisplayDriver( OSXDisplayManagerHandle pDisplayManager );
		virtual ~OSXDisplayDriver() noexcept;

	private:
		virtual void _nativeEnumDisplayDevices() override final;

		virtual void _nativeEnumVideoModes( DisplayOutput & pOutput, EColorFormat pColorFormat ) override final;

		virtual EColorFormat _nativeQueryDefaultSystemColorFormat() const override final;
	};

} // namespace Ic3::System

#endif // __IC3_SYSTEM_PLATFORM_OSAPI_OSX_DISPLAY_SYSTEM_H__
