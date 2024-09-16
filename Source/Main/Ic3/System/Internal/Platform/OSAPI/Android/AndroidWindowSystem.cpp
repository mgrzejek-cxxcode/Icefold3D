
#include "AndroidWindowSystem.h"

#if( IC3_PCL_TARGET_SYSAPI == IC3_PCL_TARGET_SYSAPI_ANDROID )
namespace Ic3::System
{

	AndroidWindowManager::AndroidWindowManager( DisplayManagerHandle pDisplayManager )
	: WindowManager( std::move( pDisplayManager ) )
	{}

	AndroidWindowManager::~AndroidWindowManager() noexcept = default;

	WindowHandle AndroidWindowManager::_nativeCreateWindow( WindowCreateInfo pCreateInfo )
	{
		return nullptr;
	}


	AndroidWindow::AndroidWindow( AndroidWindowManagerHandle pWindowManager )
	: Window( std::move( pWindowManager ), &mNativeData )
	{}

	AndroidWindow::~AndroidWindow() noexcept = default;

	void AndroidWindow::_nativeResize( const FrameSize & pFrameSize, EFrameSizeMode pSizeMode )
	{}

	void AndroidWindow::_nativeSetFullscreenMode( bool pEnable )
	{}

	void AndroidWindow::_nativeSetTitle( const std::string & pTitle )
	{}

	void AndroidWindow::_nativeUpdateGeometry( const FrameGeometry & pFrameGeometry,
	                                           TBitmask<EFrameGeometryUpdateFlags> pUpdateFlags )
	{}

	FrameSize AndroidWindow::_nativeGetSize( EFrameSizeMode /* pSizeMode */ ) const
	{
		auto & aSessionData = Platform::androidGetASessionData( *this );
		return Platform::androidQueryNativeWindowSize( aSessionData.aNativeWindow );
	}


	namespace Platform
	{

		Math::Size2u androidQueryNativeWindowSize( ANativeWindow * pANativeWindow )
		{
			Math::Size2u resultSize{};

			auto width = ANativeWindow_getWidth( pANativeWindow );
			auto height = ANativeWindow_getHeight( pANativeWindow );

			resultSize.x = numeric_cast<decltype( resultSize.x )>( width );
			resultSize.y = numeric_cast<decltype( resultSize.x )>( height );

			return resultSize;
		}

	}

} // namespace Ic3::System
#endif // IC3_PCL_TARGET_SYSAPI_ANDROID
