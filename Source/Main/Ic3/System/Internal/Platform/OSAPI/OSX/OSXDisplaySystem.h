
#ifndef __IC3_SYSTEM_PLATFORM_OSAPI_OSX_DISPLAY_SYSTEM_H__
#define __IC3_SYSTEM_PLATFORM_OSAPI_OSX_DISPLAY_SYSTEM_H__

#include "OSXSysContext.h"
#include <Ic3/System/Core/DisplayConfiguration.h>
#include <Ic3/System/Core/DisplaySystem.h>

#import <CoreGraphics/CGDirectDisplay.h>

namespace Ic3::System
{

	Ic3SysDeclareHandle( OSXDisplayManager );
	Ic3SysDeclareHandle( OSXDisplayDriver );

	namespace Platform
	{

		struct OSXDisplayManagerNativeData : public OSXNativeDataCommon
		{
			std::unique_ptr<CGDirectDisplayID[]> mCGActiveDisplayList;

			CGDisplayCount mCGActiveDisplaysNum = 0;

			CGDirectDisplayID mCGMainDisplayID = kCGNullDirectDisplay;
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

		IC3_SYSTEM_API_NODISCARD ScreenRect OSXQueryDisplayRect( CGDirectDisplayID pCGDisplayID );

		IC3_SYSTEM_API_NODISCARD DisplaySize OSXQueryDisplaySize( CGDirectDisplayID pCGDisplayID );

	}

	using OSXDisplayAdapter = OSXNativeObject<DisplayAdapter, Platform::OSXDisplayAdapterNativeData>;
	using OSXDisplayOutput = OSXNativeObject<DisplayOutput, Platform::OSXDisplayOutputNativeData>;
	using OSXDisplayVideoMode = OSXNativeObject<DisplayVideoMode, Platform::OSXDisplayVideoModeNativeData>;

	/// @brief
	class OSXDisplayManager : public OSXNativeObject<DisplayManager, Platform::OSXDisplayManagerNativeData>
	{
	public:
		explicit OSXDisplayManager( SysContextHandle pSysContext );
		virtual ~OSXDisplayManager() noexcept;

	private:
		void _InitializeOSXDisplayManagerState();
		void _ReleaseOSXDisplayManagerState();
		
		virtual DisplayDriverHandle _NativeCreateDisplayDriver() override final;

		virtual void _NativeQueryDefaultDisplayOffset( DisplayOffset & pOutOffset ) const override final;

		virtual void _NativeQueryDefaultDisplaySize( DisplaySize & pOutSize ) const override final;

		virtual void _NativeQueryMinWindowSize( DisplaySize & pOutSize ) const override final;
	};

	/// @brief
	class OSXDisplayDriver : public OSXNativeObject<DisplayDriver, Platform::OSXDisplayDriverNativeData>
	{
	public:
		explicit OSXDisplayDriver( OSXDisplayManagerHandle pDisplayManager );
		virtual ~OSXDisplayDriver() noexcept;

	private:
		virtual void _NativeEnumDisplayDevices() override final;

		virtual void _NativeEnumVideoModes( DisplayOutput & pOutput, EColorFormat pColorFormat ) override final;

		virtual EColorFormat _NativeQueryDefaultSystemColorFormat() const override final;
	};

} // namespace Ic3::System

#endif // __IC3_SYSTEM_PLATFORM_OSAPI_OSX_DISPLAY_SYSTEM_H__
