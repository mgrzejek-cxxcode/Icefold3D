
#ifndef __IC3_SYSTEM_DXGI_DISPLAY_SYSTEM_H__
#define __IC3_SYSTEM_DXGI_DISPLAY_SYSTEM_H__

#include <Ic3/System/DisplayConfiguration.h>
#include <Ic3/System/DisplaySystem.h>
#include "DXGICommon.h"

#if( IC3_SYSTEM_DSM_DRIVER_TYPE_SUPPORT_DXGI )
namespace Ic3::System
{

	namespace platform
	{

		struct DisplayDriverNativeDataDXGI
		{
			ComPtr<IDXGIFactory1> dxgiFactory;
		};

		struct DisplayAdapterNativeDataDXGI
		{
			ComPtr<IDXGIAdapter1> dxgiAdapter;
			DXGI_ADAPTER_DESC1  dxgiAdapterDesc;
		};

		struct DisplayOutputNativeDataDXGI
		{
			ComPtr<IDXGIOutput1> dxgiOutput;
			DXGI_OUTPUT_DESC  dxgiOutputDesc;
		};

		struct DisplayVideoModeNativeDataDXGI
		{
			DXGI_MODE_DESC dxgiModeDesc;
		};

	}

    using DisplayAdapterDXGI = NativeObject<DisplayAdapter, platform::DisplayAdapterNativeDataDXGI>;
    using DisplayOutputDXGI = NativeObject<DisplayOutput, platform::DisplayOutputNativeDataDXGI>;
    using DisplayVideoModeDXGI = NativeObject<DisplayVideoMode, platform::DisplayVideoModeNativeDataDXGI>;

	/// @brief
	class DisplayDriverDXGI : public NativeObject<DisplayDriver, platform::DisplayDriverNativeDataDXGI>
	{
	public:
		explicit DisplayDriverDXGI( DisplayManagerHandle pDisplayManager );
		virtual ~DisplayDriverDXGI() noexcept;

	private:
		void _initializeDXGIDriverState();
		void _releaseDXGIDriverState() noexcept;

		void _enumAdapterOutputs( DisplayAdapter & pAdapter );

		virtual void _nativeEnumDisplayDevices() override final;

		virtual void _nativeEnumVideoModes( DisplayOutput & pOutput, EColorFormat pColorFormat ) override final;

		virtual EColorFormat _nativeQueryDefaultSystemColorFormat() const override final;
	};

} // namespace Ic3::System
#endif // IC3_SYSTEM_DSM_DRIVER_TYPE_SUPPORT_DXGI

#endif // __IC3_SYSTEM_DXGI_DISPLAY_SYSTEM_H__
