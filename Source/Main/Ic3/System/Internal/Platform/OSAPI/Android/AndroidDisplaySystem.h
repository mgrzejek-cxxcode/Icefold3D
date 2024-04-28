
#ifndef __IC3_SYSTEM_PLATFORM_OSAPI_ANDROID_DISPLAY_H__
#define __IC3_SYSTEM_PLATFORM_OSAPI_ANDROID_DISPLAY_H__

#include "AndroidCommon.h"
#include <Ic3/System/DisplayConfiguration.h>
#include <Ic3/System/DisplaySystem.h>

namespace Ic3::System
{

	ic3SysDeclareHandle( AndroidDisplayManager );
	ic3SysDeclareHandle( AndroidDisplayDriver );

	namespace Platform
	{

		using AndroidDisplayManagerNativeData = AndroidNativeDataCommon;
		using AndroidDisplayDriverNativeData = AndroidNativeDataCommon;
		using AndroidDisplayAdapterNativeData = AndroidNativeDataCommon;
		using AndroidDisplayOutputNativeData = AndroidNativeDataCommon;
		using AndroidDisplayVideoModeNativeData = AndroidNativeDataCommon;

	}

	/// @brief
	class AndroidDisplayAdapter : public AndroidNativeObject<DisplayAdapter, Platform::AndroidDisplayAdapterNativeData>
	{
		friend class AndroidDisplayDriver;

	public:
		explicit AndroidDisplayAdapter( AndroidDisplayDriver & pDisplayDriver );
		virtual ~AndroidDisplayAdapter() noexcept;
	};

	/// @brief
	class AndroidDisplayOutput : public AndroidNativeObject<DisplayOutput, Platform::AndroidDisplayOutputNativeData>
	{
		friend class AndroidDisplayDriver;

	public:
		explicit AndroidDisplayOutput( AndroidDisplayAdapter & pDisplayAdapter );
		virtual ~AndroidDisplayOutput() noexcept;
	};

	/// @brief
	class AndroidDisplayVideoMode : public AndroidNativeObject<DisplayVideoMode, Platform::AndroidDisplayVideoModeNativeData>
	{
		friend class AndroidDisplayDriver;

	public:
		explicit AndroidDisplayVideoMode( AndroidDisplayOutput & pDisplayOutput );
		virtual ~AndroidDisplayVideoMode() noexcept;
	};

	/// @brief
	class AndroidDisplayManager : public AndroidNativeObject<DisplayManager, Platform::AndroidDisplayManagerNativeData>
	{
	public:
		explicit AndroidDisplayManager( SysContextHandle pSysContext );
		virtual ~AndroidDisplayManager() noexcept;

	private:
		virtual DisplayDriverHandle _nativeCreateDisplayDriver() override final;

		virtual void _nativeQueryDefaultDisplaySize( DisplaySize & pOutSize ) const override final;

		virtual void _nativeQueryMinWindowSize( DisplaySize & pOutSize ) const override final;
	};

	/// @brief
	class AndroidDisplayDriver : public AndroidNativeObject<DisplayDriver, Platform::AndroidDisplayDriverNativeData>
	{
	public:
		explicit AndroidDisplayDriver( AndroidDisplayManagerHandle pDisplayManager );
		virtual ~AndroidDisplayDriver() noexcept;

	private:
		virtual void _nativeEnumDisplayDevices() override final;

		virtual void _nativeEnumVideoModes( DisplayOutput & pOutput, EColorFormat pColorFormat ) override final;

		virtual EColorFormat _nativeQueryDefaultSystemColorFormat() const override final;
	};

} // namespace Ic3::System

#endif // __IC3_SYSTEM_PLATFORM_OSAPI_ANDROID_DISPLAY_H__
