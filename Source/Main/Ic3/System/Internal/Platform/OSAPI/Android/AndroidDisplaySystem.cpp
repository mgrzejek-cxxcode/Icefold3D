
#include "AndroidDisplaySystem.h"
#include "AndroidWindowSystem.h"
#include <cppx/stdHelperAlgo.h>
#include <map>

#if( PCL_TARGET_SYSAPI == PCL_TARGET_SYSAPI_ANDROID )
namespace Ic3::System
{

	namespace Platform
	{

		EColorFormat _androidTranslateAHWBufferFormatToColorFormat( AHardwareBuffer_Format pAHWBufferFormat );

	}


	AndroidDisplayAdapter::AndroidDisplayAdapter( AndroidDisplayDriver & pDisplayDriver )
	: AndroidNativeObject( pDisplayDriver )
	{}

	AndroidDisplayAdapter::~AndroidDisplayAdapter() noexcept = default;


	AndroidDisplayOutput::AndroidDisplayOutput( AndroidDisplayAdapter & pDisplayAdapter )
	: AndroidNativeObject( pDisplayAdapter )
	{}

	AndroidDisplayOutput::~AndroidDisplayOutput() noexcept = default;


	AndroidDisplayVideoMode::AndroidDisplayVideoMode( AndroidDisplayOutput & pDisplayOutput )
	: AndroidNativeObject( pDisplayOutput )
	{}

	AndroidDisplayVideoMode::~AndroidDisplayVideoMode() noexcept = default;


	AndroidDisplayManager::AndroidDisplayManager( SysContextHandle pSysContext )
	: AndroidNativeObject( std::move( pSysContext ) )
	{}

	AndroidDisplayManager::~AndroidDisplayManager() noexcept = default;

	DisplayDriverHandle AndroidDisplayManager::_NativeCreateDisplayDriver()
	{
		return CreateSysObject<AndroidDisplayDriver>( GetHandle<AndroidDisplayManager>() );
	}

	void AndroidDisplayManager::_NativeQueryDefaultDisplaySize( DisplaySize & pOutSize ) const
	{
		auto & aSessionData = Platform::AndroidGetASessionData( *this );
		pOutSize = Platform::AndroidQueryNativeWindowSize( aSessionData.aNativeWindow );
	}

	void AndroidDisplayManager::_NativeQueryMinWindowSize( DisplaySize & pOutSize ) const
	{
		auto & aSessionData = Platform::AndroidGetASessionData( *this );
		pOutSize = Platform::AndroidQueryNativeWindowSize( aSessionData.aNativeWindow );
	}


	AndroidDisplayDriver::AndroidDisplayDriver( AndroidDisplayManagerHandle pDisplayManager )
	: AndroidNativeObject( std::move( pDisplayManager ), EDisplayDriverType::Generic )
	{}

	AndroidDisplayDriver::~AndroidDisplayDriver() noexcept = default;

	void AndroidDisplayDriver::_NativeEnumDisplayDevices()
	{
		auto adapterObject = CreateAdapter<AndroidDisplayAdapter>( *this );
		auto & adapterDesc = adapterObject->getAdapterDescInternal();
		adapterDesc.name = "ANDROID_DEFAULT_ADAPTER";
		adapterDesc.vendorID = EDisplayAdapterVendorID::Google;
		adapterDesc.flags.set( E_DISPLAY_ADAPTER_FLAG_ACTIVE_BIT );
		adapterDesc.flags.set( E_DISPLAY_ADAPTER_FLAG_PRIMARY_BIT );

		auto outputObject = adapterObject->createOutput<AndroidDisplayOutput>( *adapterObject );
		auto & outputDesc = outputObject->GetOutputDescInternal();
		outputDesc.name = "ANDROID_DEFAULT_DISPLAY";
		outputDesc.screenRect.offset.x = 0;
		outputDesc.screenRect.offset.y = 0;
		outputDesc.screenRect.size = mDisplayManager->queryDefaultDisplaySize();
	}

	void AndroidDisplayDriver::_NativeEnumVideoModes( DisplayOutput & pOutput, EColorFormat pColorFormat )
	{
		auto & aSessionData = Platform::AndroidGetASessionData( *this );

		auto aWindowFormat = ANativeWindow_getFormat( aSessionData.aNativeWindow );
		auto aHWBufferFormat = static_cast<AHardwareBuffer_Format>( aWindowFormat );
		auto colorFormat = Platform::_androidTranslateAHWBufferFormatToColorFormat( aHWBufferFormat );

		if( pColorFormat != colorFormat )
		{
			return;
		}

		auto * androidDisplayOutput = pOutput.QueryInterface<AndroidDisplayOutput>();
		auto videoModeObject = androidDisplayOutput->createVideoMode<AndroidDisplayVideoMode>( *androidDisplayOutput, pColorFormat );

		auto & videoModeDesc = videoModeObject->GetModeDescInternal();
		videoModeDesc.settings.resolution = pOutput.getOutputDesc().screenRect.size;
		videoModeDesc.settings.refreshRate = 60;
		videoModeDesc.settings.flags.set( E_DISPLAY_VIDEO_SETTINGS_FLAG_SCAN_PROGRESSIVE_BIT );
		videoModeDesc.settingsHash = DSMComputeVideoSettingsHash( pColorFormat, videoModeDesc.settings );
	}

	EColorFormat AndroidDisplayDriver::_NativeQueryDefaultSystemColorFormat() const
	{
		auto & aSessionData = Platform::AndroidGetASessionData( *this );

		auto aWindowFormat = ANativeWindow_getFormat( aSessionData.aNativeWindow );
		auto aHWBufferFormat = static_cast<AHardwareBuffer_Format>( aWindowFormat );
		auto colorFormat = Platform::_androidTranslateAHWBufferFormatToColorFormat( aHWBufferFormat );

		return colorFormat;
	}

	namespace Platform
	{

		EColorFormat _androidTranslateAHWBufferFormatToColorFormat( AHardwareBuffer_Format pAHWBufferFormat )
		{
			static const std::map<AHardwareBuffer_Format, EColorFormat> colorDescMap =
			{
				{ AHARDWAREBUFFER_FORMAT_R8G8B8A8_UNORM,    EColorFormat::R8G8B8A8 },
				{ AHARDWAREBUFFER_FORMAT_R8G8B8X8_UNORM,    EColorFormat::R8G8B8X8 },
				{ AHARDWAREBUFFER_FORMAT_R10G10B10A2_UNORM, EColorFormat::R10G10B10A2 }
			};
			return cppx::get_map_value_ref_or_default( colorDescMap, pAHWBufferFormat, EColorFormat::Unknown );
		}

	}

} // namespace Ic3::System
#endif // PCL_TARGET_SYSAPI_ANDROID
