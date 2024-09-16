
#include "X11WindowSystem.h"
#include "X11DisplaySystem.h"

#if( IC3_PCL_TARGET_SYSAPI == IC3_PCL_TARGET_SYSAPI_X11 )
namespace Ic3::System
{

	namespace Platform
	{

		// Returns an array of _NET_WM_ACTION_xxx atoms for the specified frame style.
		// This array is then used to set the ALLOWED_ACTIONS atom, which enables/disables certain window actions.
		std::vector<Atom> _x11QueryActionTableForFrameStyle( XDisplay pDisplay, EFrameStyle pFrameStyle );

		// Returns an XSizeHints structure, configured based on the specified frame style and geometry.
		// XSizeHints enables Window Manager to properly style the window and enable/disable geometry changes.
		XSizeHints _x11QuerySizeHintsForFrameStyle( XDisplay pDisplay, EFrameStyle pFrameStyle, const FrameGeometry & pFrameGeometry );

	}
	
	X11WindowManager::X11WindowManager( X11DisplayManagerHandle pDisplayManager )
	: X11NativeObject( std::move( pDisplayManager ) )
	{}

	X11WindowManager::~X11WindowManager() noexcept = default;

	WindowHandle X11WindowManager::_nativeCreateWindow( WindowCreateInfo pCreateInfo )
	{
		auto & xSessionData = Platform::x11GetXSessionData( *this );

		auto windowObject = createSysObject<X11Window>( getHandle<X11WindowManager>() );

		Platform::X11WindowCreateInfo x11CreateInfo;
		x11CreateInfo.mFrameGeometry = pCreateInfo.mFrameGeometry;
		x11CreateInfo.mTitle = std::move( pCreateInfo.mTitle );
		x11CreateInfo.mColorDepth = XDefaultDepth( xSessionData.mDisplay, xSessionData.mScreenIndex );
		x11CreateInfo.mWindowVisual = XDefaultVisual( xSessionData.mDisplay, xSessionData.mScreenIndex );

		Platform::x11CreateWindow( windowObject->mNativeData, x11CreateInfo );

		Platform::x11WindowPostCreateUpdate( windowObject->mNativeData, x11CreateInfo );

		return windowObject;
	}

	void X11WindowManager::_nativeDestroyWindow( Window & pWindow )
	{
		auto * x11Window = pWindow.queryInterface<X11Window>();

		Platform::x11DestroyWindow( x11Window->mNativeData );
	}
	

	X11Window::X11Window( X11WindowManagerHandle pWindowManager )
	: X11NativeObject( std::move( pWindowManager ), &mNativeData )
	{}

	X11Window::~X11Window() noexcept = default;

	void X11Window::_nativeResize( const FrameSize & pFrameSize, EFrameSizeMode pSizeMode )
	{}

	void X11Window::_nativeSetFullscreenMode( bool pEnable )
	{
		Platform::x11SetWindowFullscreenState( mNativeData, pEnable );
	}

	void X11Window::_nativeSetTitle( const std::string & pTitle )
	{
		return Platform::x11SetFrameTitle( mNativeData, pTitle );
	}

	void X11Window::_nativeUpdateGeometry( const FrameGeometry & pFrameGeometry,
	                                       TBitmask<EFrameGeometryUpdateFlags> pUpdateFlags )
	{
		return Platform::x11UpdateFrameGeometry( mNativeData, pFrameGeometry, pUpdateFlags );
	}

	FrameSize X11Window::_nativeGetSize( EFrameSizeMode pSizeMode ) const
	{
		return Platform::x11GetFrameSize( mNativeData, pSizeMode );
	}


	namespace Platform
	{

		void x11CreateWindow( X11WindowNativeData & pWindowNativeData, const X11WindowCreateInfo & pCreateInfo )
		{
			auto & xSessionData = Platform::x11GetXSessionData( pWindowNativeData );

			// Colormap for our window.
			Colormap colormap = XCreateColormap( xSessionData.mDisplay,
			                                     xSessionData.mRootWindowXID,
			                                     pCreateInfo.mWindowVisual,
			                                     AllocNone );

			// Mask of all event types we want to register for and handle.
			constexpr long windowEventMask = FocusChangeMask | PropertyChangeMask| ExposureMask |
			                                 StructureNotifyMask | KeyPressMask | KeyReleaseMask |
			                                 ButtonPressMask | ButtonReleaseMask | PointerMotionMask |
			                                 Button1MotionMask | Button2MotionMask | Button3MotionMask |
			                                 Button4MotionMask | Button5MotionMask | ButtonMotionMask;

			XSetWindowAttributes windowSetAttributes;
			windowSetAttributes.background_pixel = 0;
			windowSetAttributes.border_pixel = 0;
			windowSetAttributes.colormap = colormap;
			windowSetAttributes.event_mask = windowEventMask;
			// Mask for all attributes we want to specify (fields in XSetWindowAttributes filled above).
			const unsigned long windowSetAttributesMask = CWBackPixel | CWBorderPixel | CWColormap | CWEventMask;

			// We pass initial position as (0,0) instead of the specified one, because window managers tend to do some
			// weird stuff and this is ignored in most cases. XMoveWindow() together with XSetNormalHints() do the thing.
			auto windowXID = XCreateWindow( xSessionData.mDisplay,
			                                xSessionData.mRootWindowXID,
			                                0, 0,
			                                pCreateInfo.mFrameGeometry.mSize.x,
			                                pCreateInfo.mFrameGeometry.mSize.y,
			                                0u,
			                                pCreateInfo.mColorDepth,
			                                InputOutput,
			                                pCreateInfo.mWindowVisual,
			                                windowSetAttributesMask,
			                                &windowSetAttributes );

			if( windowXID == eXIDNone )
			{
				ic3Throw( E_EXC_DEBUG_PLACEHOLDER );
			}

			pWindowNativeData.mWindowXID = windowXID;
			pWindowNativeData.mXColormap = colormap;

			// Window style requested for the window. There is no real "X11-equivalent" (like we have
			// on other OSes), because of the separation between X Server and the actual Window Manager.
			const auto windowStyle = pCreateInfo.mFrameGeometry.mStyle;

			// First thing to enforce requested window behaviour: action table. It defines what kind of actions
			// (like moving, resizing, entering fullscreen) are allowed for a given window. Based on the style,
			// we query the table and set it for our newly created window.
			auto windowActionTable = Platform::_x11QueryActionTableForFrameStyle( xSessionData.mDisplay, windowStyle );

			// This could happen only if we added/changed something and forgot to adjust this code.
			ic3DebugAssert( !windowActionTable.empty() );

			// The atom used to set this table is named "_NET_WM_ALLOWED_ACTIONS".
			Atom wmAllowedActions = XInternAtom( xSessionData.mDisplay, "_NET_WM_ALLOWED_ACTIONS", true );

			XChangeProperty( xSessionData.mDisplay,
			                 windowXID,
			                 wmAllowedActions,
			                 XA_ATOM,
			                 32,
			                 PropModeReplace,
			                 ( unsigned char * )windowActionTable.data(),
			                 ( int )windowActionTable.size() );

			auto windowSizeHints = Platform::_x11QuerySizeHintsForFrameStyle( xSessionData.mDisplay, windowStyle, pCreateInfo.mFrameGeometry );

			// Note, that this call must be done before the window is mapped to the screen. Otherwise, most WMs
			// will not recognize PMinSize/PMaxSize hints, making non-resizable windows very much resizable ones.
			XSetNormalHints( xSessionData.mDisplay, windowXID, &windowSizeHints );
		}

		void x11WindowPostCreateUpdate( X11WindowNativeData & pWindowNativeData, const X11WindowCreateInfo & pCreateInfo )
		{
			auto & xSessionData = Platform::x11GetXSessionData( pWindowNativeData );
			
			XMoveWindow( xSessionData.mDisplay,
			             pWindowNativeData.mWindowXID,
			             static_cast<int>( pCreateInfo.mFrameGeometry.mPosition.x ),
			             static_cast<int>( pCreateInfo.mFrameGeometry.mPosition.y ) );

			// Another observation: if this gets called before the initial XMoveWindow(), fullscreen windows
			// appear at a wrong position (and, thus, usually - on the wrong screen as well). Confirmed at least
			// on Kubuntu 20.04 and Mint. Non-fullscreen windows seem to work regardless of the call order.
			XMapWindow( xSessionData.mDisplay, pWindowNativeData.mWindowXID );

			if( pCreateInfo.mFullscreenMode )
			{
				// This works by sending a _NET_WM_STATE update event to the window.
				// It strictly requires the window to be mapped already, so make sure
				// this always called after XMapWindow().
				x11SetWindowFullscreenState( pWindowNativeData, true );
			}

			Atom registeredWMProtocolArray[] =
			{
				xSessionData.mAtomCache.wmProtocolDelete,
				xSessionData.mAtomCache.wmProtocolDestroy
			};
			const auto registeredWMProtocolsNum = Cppx::staticArraySize( registeredWMProtocolArray );

			XSetWMProtocols( xSessionData.mDisplay,
			                 pWindowNativeData.mWindowXID,
			                 &( registeredWMProtocolArray[0] ),
			                 numeric_cast<int>( registeredWMProtocolsNum ) );

			XStoreName( xSessionData.mDisplay, pWindowNativeData.mWindowXID, pCreateInfo.mTitle.c_str() );
			XFlush( xSessionData.mDisplay );
		}

		void x11DestroyWindow( X11WindowNativeData & pWindowNativeData )
		{
			auto & xSessionData = Platform::x11GetXSessionData( pWindowNativeData );

			XUnmapWindow( xSessionData.mDisplay, pWindowNativeData.mWindowXID );

			XDestroyWindow( xSessionData.mDisplay, pWindowNativeData.mWindowXID );
			pWindowNativeData.mWindowXID = eXIDNone;

			XFreeColormap( xSessionData.mDisplay, pWindowNativeData.mXColormap );
			pWindowNativeData.mXColormap = eXIDNone;

			XFlush( xSessionData.mDisplay );
		}

		void x11SetWindowFullscreenState( X11WindowNativeData & pWindowNativeData, bool pSetFullscreen )
		{
			const auto isWindowFullscreen = x11IsFullscreenWindow( pWindowNativeData );

			if( isWindowFullscreen == pSetFullscreen )
			{
				return;
			}

			auto & xSessionData = Platform::x11GetXSessionData( pWindowNativeData );

			XEvent wmEvent;
			memset( &wmEvent, 0, sizeof( wmEvent ) );
			wmEvent.type = ClientMessage;
			wmEvent.xclient.window = pWindowNativeData.mWindowXID;
			wmEvent.xclient.message_type = xSessionData.mAtomCache.wmState;
			wmEvent.xclient.format = 32;
			wmEvent.xclient.data.l[0] = pSetFullscreen ? 1 : 0;
			wmEvent.xclient.data.l[1] = numeric_cast<long>( xSessionData.mAtomCache.wmStateFullscreen );
			wmEvent.xclient.data.l[2] = 0;

			XSendEvent( xSessionData.mDisplay,
			            xSessionData.mRootWindowXID,
			            False,
			            SubstructureRedirectMask | SubstructureNotifyMask,
			            &wmEvent );

			XFlush( xSessionData.mDisplay );
		}

		void x11SetFrameTitle( const X11WindowNativeData & pWindowNativeData, const std::string & pTitle )
		{
			auto & xSessionData = Platform::x11GetXSessionData( pWindowNativeData );

			XStoreName( xSessionData.mDisplay, pWindowNativeData.mWindowXID, pTitle.c_str() );

			XFlush( xSessionData.mDisplay );
		}

		void x11UpdateFrameGeometry( const X11WindowNativeData & pWindowNativeData,
		                             const FrameGeometry & pFrameGeometry,
		                             TBitmask<EFrameGeometryUpdateFlags> pUpdateFlags )
		{
			auto & xSessionData = Platform::x11GetXSessionData( pWindowNativeData );

			if( pUpdateFlags.isSet( eFrameGeometryUpdateFlagPositionBit ) )
			{
				XMoveWindow( xSessionData.mDisplay,
				             pWindowNativeData.mWindowXID,
				             pFrameGeometry.mPosition.x,
				             pFrameGeometry.mPosition.y );
			}

			if( pUpdateFlags.isSet( eFrameGeometryUpdateFlagSizeBit ) )
			{
				XResizeWindow( xSessionData.mDisplay,
				               pWindowNativeData.mWindowXID,
				               pFrameGeometry.mSize.x,
				               pFrameGeometry.mSize.y );
			}

			XFlush( xSessionData.mDisplay );
		}

		FrameSize x11GetFrameSize( const X11WindowNativeData & pWindowNativeData, EFrameSizeMode pSizeMode )
		{
			auto & xSessionData = Platform::x11GetXSessionData( pWindowNativeData );

			XWindowAttributes windowAttributes;
			XGetWindowAttributes( xSessionData.mDisplay,
			                      pWindowNativeData.mWindowXID,
			                      &windowAttributes );

			FrameSize resultFrameSize;
			if( pSizeMode == EFrameSizeMode::ClientArea )
			{
				resultFrameSize.x = windowAttributes.width - windowAttributes.border_width;
				resultFrameSize.y = windowAttributes.height - windowAttributes.border_width;
			}
			else
			{
				resultFrameSize.x = windowAttributes.width;
				resultFrameSize.y = windowAttributes.height;
			}

			return resultFrameSize;
		}

		bool x11IsFullscreenWindow( XDisplay pDisplay, XWindow pWindow )
		{
			return x11CheckWindowPropertyValueSet( pDisplay, pWindow, "_NET_WM_STATE", "_NET_WM_STATE_FULLSCREEN" );
		}

		bool x11IsFullscreenWindow( const X11WindowNativeData & pWindowNativeData )
		{
			auto & xSessionData = x11GetXSessionData( pWindowNativeData );
			return x11IsFullscreenWindow( xSessionData.mDisplay, pWindowNativeData.mWindowXID );
		}

		std::vector<Atom> x11QueryWindowPropertyValueArray( XDisplay pDisplay, XWindow pWindow, const char * pPropertyName )
		{
			std::vector<Atom> wmPropertyValueArray;

			Atom wmPropertyValueType = 0;
			int wmPropertyValueFormat = 0;
			unsigned long wmPropertyValueItemsNum = 0;
			unsigned long wmPropertyValueBytesNum = 0;
			unsigned char * wmPropertyValueData8 = nullptr;

			const Atom wmProperty = XInternAtom( pDisplay, pPropertyName, true );

			XGetWindowProperty( pDisplay,
			                    pWindow,
			                    wmProperty,
			                    0,
			                    0,
			                    False,
			                    AnyPropertyType,
			                    &wmPropertyValueType,
			                    &wmPropertyValueFormat,
			                    &wmPropertyValueItemsNum,
			                    &wmPropertyValueBytesNum,
			                    &wmPropertyValueData8 );

			const long wmPropertyValuesNum = numeric_cast<long>( wmPropertyValueBytesNum ) / 4;

			XGetWindowProperty( pDisplay,
			                    pWindow,
			                    wmProperty,
			                    0,
			                    wmPropertyValuesNum,
			                    False,
			                    XA_ATOM,
			                    &wmPropertyValueType,
			                    &wmPropertyValueFormat,
			                    &wmPropertyValueItemsNum,
			                    &wmPropertyValueBytesNum,
			                    &wmPropertyValueData8 );

			const auto * wmPropertyValueData32 = reinterpret_cast<unsigned long *>( wmPropertyValueData8 );

			wmPropertyValueArray.resize( wmPropertyValuesNum );

			for( size_t wmPropertyValueIndex = 0; wmPropertyValueIndex < wmPropertyValuesNum; ++wmPropertyValueIndex  )
			{
				wmPropertyValueArray[wmPropertyValueIndex] = wmPropertyValueData32[wmPropertyValueIndex];
			}

			XFree( wmPropertyValueData8 );

			return wmPropertyValueArray;
		}

		bool x11CheckWindowPropertyValueSet( XDisplay pDisplay, XWindow pWindow, const char * pPropertyName, const char * pValueID )
		{
			Atom wmPropertyValueType = 0;
			int wmPropertyValueFormat = 0;
			unsigned long wmPropertyValueItemsNum = 0;
			unsigned long wmPropertyValueBytesNum = 0;
			unsigned char * wmPropertyValueData8 = nullptr;

			const Atom wmProperty = XInternAtom( pDisplay, pPropertyName, true );

			XGetWindowProperty( pDisplay,
			                    pWindow,
			                    wmProperty,
			                    0,
			                    0,
			                    False,
			                    AnyPropertyType,
			                    &wmPropertyValueType,
			                    &wmPropertyValueFormat,
			                    &wmPropertyValueItemsNum,
			                    &wmPropertyValueBytesNum,
			                    &wmPropertyValueData8 );

			const long wmPropertyValuesNum = numeric_cast<long>( wmPropertyValueBytesNum ) / 4;

			XGetWindowProperty( pDisplay,
			                    pWindow,
			                    wmProperty,
			                    0,
			                    wmPropertyValuesNum,
			                    False,
			                    XA_ATOM,
			                    &wmPropertyValueType,
			                    &wmPropertyValueFormat,
			                    &wmPropertyValueItemsNum,
			                    &wmPropertyValueBytesNum,
			                    &wmPropertyValueData8 );

			const auto * wmPropertyValueData32 = reinterpret_cast<unsigned long *>( wmPropertyValueData8 );

			const Atom wmPropertyValue = XInternAtom( pDisplay, pValueID, true );

			bool propertyValueFound = false;

			for( size_t wmPropertyValueIndex = 0; wmPropertyValueIndex < wmPropertyValuesNum; ++wmPropertyValueIndex  )
			{
				if( wmPropertyValueData32[wmPropertyValueIndex] == wmPropertyValue )
				{
					propertyValueFound = true;
					break;
				}
			}

			XFree( wmPropertyValueData8 );

			return propertyValueFound;
		}

		// List of Xlib-defined window actions:
		// _NET_WM_ACTION_CHANGE_DESKTOP
		// _NET_WM_ACTION_FULLSCREEN
		// _NET_WM_ACTION_MAXIMIZE_HORZ
		// _NET_WM_ACTION_MAXIMIZE_VERT
		// _NET_WM_ACTION_MINIMIZE
		// _NET_WM_ACTION_RESIZE
		// _NET_WM_ACTION_MOVE
		// _NET_WM_ACTION_CLOSE
		// _NET_WM_ACTION_STICK
		// _NET_WM_ACTION_SHADE

		static const char * sX11WindowActionTableCaption[] =
		{
			"_NET_WM_ACTION_CHANGE_DESKTOP",
			"_NET_WM_ACTION_FULLSCREEN",
			"_NET_WM_ACTION_MOVE",
			"_NET_WM_ACTION_STICK",
			"_NET_WM_ACTION_SHADE"
		};

		static const char * sX11WindowActionTableFixed[] =
		{
			"_NET_WM_ACTION_CHANGE_DESKTOP",
			"_NET_WM_ACTION_FULLSCREEN",
			"_NET_WM_ACTION_MINIMIZE",
			"_NET_WM_ACTION_MOVE",
			"_NET_WM_ACTION_CLOSE",
			"_NET_WM_ACTION_STICK",
			"_NET_WM_ACTION_SHADE"
		};

		static const char * sX11WindowActionTableOverlay[] =
		{
			"_NET_WM_ACTION_CHANGE_DESKTOP",
			"_NET_WM_ACTION_FULLSCREEN",
			"_NET_WM_ACTION_STICK",
			"_NET_WM_ACTION_SHADE"
		};

		static const char * sX11WindowActionTableResizeable[] =
		{
			"_NET_WM_ACTION_CHANGE_DESKTOP",
			"_NET_WM_ACTION_FULLSCREEN",
			"_NET_WM_ACTION_MAXIMIZE_HORZ",
			"_NET_WM_ACTION_MAXIMIZE_VERT",
			"_NET_WM_ACTION_MINIMIZE",
			"_NET_WM_ACTION_RESIZE",
			"_NET_WM_ACTION_MOVE",
			"_NET_WM_ACTION_CLOSE",
			"_NET_WM_ACTION_STICK",
			"_NET_WM_ACTION_SHADE"
		};

		std::vector<Atom> _x11QueryActionTableForFrameStyle( XDisplay pDisplay, EFrameStyle pFrameStyle )
		{
			const char ** windowActionTablePtr = nullptr;
			size_t windowActionTableSize = 0u;

			switch( pFrameStyle )
			{
				case EFrameStyle::Caption:
					windowActionTablePtr = &( sX11WindowActionTableCaption[0] );
					windowActionTableSize = Cppx::staticArraySize( sX11WindowActionTableCaption );
					break;

				case EFrameStyle::Fixed:
					windowActionTablePtr = &( sX11WindowActionTableFixed[0] );
					windowActionTableSize = Cppx::staticArraySize( sX11WindowActionTableFixed );
					break;

				case EFrameStyle::Overlay:
					windowActionTablePtr = &( sX11WindowActionTableOverlay[0] );
					windowActionTableSize = Cppx::staticArraySize( sX11WindowActionTableOverlay );
					break;

				case EFrameStyle::Resizeable:
					windowActionTablePtr = &( sX11WindowActionTableResizeable[0] );
					windowActionTableSize = Cppx::staticArraySize( sX11WindowActionTableResizeable );
					break;
			}

			std::vector<Atom> resultAtomTable;

			if( windowActionTablePtr )
			{
				resultAtomTable.resize( windowActionTableSize, 0u );

				for( size_t atomIndex = 0; atomIndex < windowActionTableSize; ++atomIndex )
				{
					const auto * atomName = windowActionTablePtr[atomIndex];
					resultAtomTable[atomIndex] = XInternAtom( pDisplay, atomName, true );
				}
			}

			return resultAtomTable;
		}

		XSizeHints _x11QuerySizeHintsForFrameStyle( XDisplay pDisplay, EFrameStyle pFrameStyle, const FrameGeometry & pFrameGeometry )
		{
			XSizeHints windowSizeHints;

			windowSizeHints.flags = PPosition | PSize;
			windowSizeHints.x = static_cast<int>( pFrameGeometry.mPosition.x );
			windowSizeHints.y = static_cast<int>( pFrameGeometry.mPosition.y );
			windowSizeHints.width = static_cast<int>( pFrameGeometry.mSize.x );
			windowSizeHints.height = static_cast<int>( pFrameGeometry.mSize.y );

			// "Resizeable" style is the only style which has the dynamic resizing enabled.
			if( pFrameStyle != EFrameStyle::Resizeable )
			{
				// 'PMaxSize' is the maximum allowed size of the window frame. 'PMinSize' - well, that is rather obvious.
				// If both are set to the same value, resizing is blocked. This will cause following things:
				// - no maximize button
				// - no side border resizing
				// - no corner resizing

				windowSizeHints.flags |= PMaxSize;
				windowSizeHints.max_width = windowSizeHints.width;
				windowSizeHints.max_height = windowSizeHints.height;

				windowSizeHints.flags |= PMinSize;
				windowSizeHints.min_width = windowSizeHints.width;
				windowSizeHints.min_height = windowSizeHints.height;
			}

			return windowSizeHints;
		}

	}

} // namespace Ic3::System
#endif // IC3_PCL_TARGET_SYSAPI_X11
