
#ifndef __IC3_SYSTEM_PLATFORM_SHARED_DXGI_DISPLAY_SYSTEM_H__
#define __IC3_SYSTEM_PLATFORM_SHARED_DXGI_DISPLAY_SYSTEM_H__

#include <Ic3/System/DisplayConfiguration.h>
#include <Ic3/System/DisplaySystem.h>
#include "DXGICommon.h"

#if( IC3_SYSTEM_DSM_DRIVER_TYPE_SUPPORT_DXGI )
namespace Ic3::System
{

	namespace Platform
	{

		struct DXGIDisplayDriverNativeData
		{
			ComPtr<IDXGIFactory1> dxgiFactory;
		};

		struct DXGIDisplayAdapterNativeData
		{
			ComPtr<IDXGIAdapter1> dxgiAdapter;
			DXGI_ADAPTER_DESC1  dxgiAdapterDesc;
		};

		struct DXGIDisplayOutputNativeData
		{
			ComPtr<IDXGIOutput1> dxgiOutput;
			DXGI_OUTPUT_DESC  dxgiOutputDesc;
		};

		struct DXGIDisplayVideoModeNativeData
		{
			DXGI_MODE_DESC dxgiModeDesc;
		};

	}

    using DXGIDisplayAdapter = NativeObject<DisplayAdapter, Platform::DXGIDisplayAdapterNativeData>;
    using DXGIDisplayOutput = NativeObject<DisplayOutput, Platform::DXGIDisplayOutputNativeData>;
    using DXGIDisplayVideoMode = NativeObject<DisplayVideoMode, Platform::DXGIDisplayVideoModeNativeData>;

	/// @brief
	class DXGIDisplayDriver : public NativeObject<DisplayDriver, Platform::DXGIDisplayDriverNativeData>
	{
	public:
		explicit DXGIDisplayDriver( DisplayManagerHandle pDisplayManager );
		virtual ~DXGIDisplayDriver() noexcept;

	private:
		void _InitializeDXGIDriverState();
		void _ReleaseDXGIDriverState() noexcept;

		void _EnumAdapterOutputs( DisplayAdapter & pAdapter );

		virtual void _NativeEnumDisplayDevices() override final;

		virtual void _NativeEnumVideoModes( DisplayOutput & pOutput, EColorFormat pColorFormat ) override final;

		virtual EColorFormat _NativeQueryDefaultSystemColorFormat() const override final;
	};

} // namespace Ic3::System
#endif // IC3_SYSTEM_DSM_DRIVER_TYPE_SUPPORT_DXGI

#endif // __IC3_SYSTEM_PLATFORM_SHARED_DXGI_DISPLAY_SYSTEM_H__
