
#include "AndroidWindowSystem.h"

#if( PCL_TARGET_SYSAPI == PCL_TARGET_SYSAPI_ANDROID )
namespace Ic3::System
{

	AndroidWindowManager::AndroidWindowManager( DisplayManagerHandle pDisplayManager )
	: WindowManager( std::move( pDisplayManager ) )
	{}

	AndroidWindowManager::~AndroidWindowManager() noexcept = default;

	WindowHandle AndroidWindowManager::_NativeCreateWindow( WindowCreateInfo pCreateInfo )
	{
		return nullptr;
	}


	AndroidWindow::AndroidWindow( AndroidWindowManagerHandle pWindowManager )
	: Window( std::move( pWindowManager ), &mNativeData )
	{}

	AndroidWindow::~AndroidWindow() noexcept = default;

	void AndroidWindow::_NativeResize( const FrameSize & pFrameSize, EFrameSizeMode pSizeMode )
	{}

	void AndroidWindow::_NativeSetFullscreenMode( bool pEnable )
	{}

	void AndroidWindow::_NativeSetTitle( const std::string & pTitle )
	{}

	void AndroidWindow::_NativeUpdateGeometry(
			const FrameGeometry & pFrameGeometry,
			cppx::bitmask<EFrameGeometryUpdateFlags> pUpdateFlags )
	{}

	FrameSize AndroidWindow::_NativeGetSize( EFrameSizeMode /* pSizeMode */ ) const
	{
		auto & aSessionData = Platform::AndroidGetASessionData( *this );
		return Platform::AndroidQueryNativeWindowSize( aSessionData.aNativeWindow );
	}


	namespace Platform
	{

		cxm::size2u AndroidQueryNativeWindowSize( ANativeWindow * pANativeWindow )
		{
			cxm::size2u resultSize{};

			auto width = ANativeWindow_getWidth( pANativeWindow );
			auto height = ANativeWindow_getHeight( pANativeWindow );

			resultSize.x = cppx::numeric_cast<decltype( resultSize.x )>( width );
			resultSize.y = cppx::numeric_cast<decltype( resultSize.x )>( height );

			return resultSize;
		}

	}

} // namespace Ic3::System
#endif // PCL_TARGET_SYSAPI_ANDROID
