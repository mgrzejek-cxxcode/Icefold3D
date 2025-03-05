
#include "X11WindowSystem.h"
#include "X11DisplaySystem.h"

#if( PCL_TARGET_SYSAPI == PCL_TARGET_SYSAPI_X11 )
namespace Ic3::System
{

	namespace Platform
	{

		// Returns an array of _NET_WM_ACTION_xxx atoms for the specified frame style.
		// This array is then used to set the ALLOWED_ACTIONS atom, which enables/disables certain window actions.
		std::vector<Atom> _X11QueryActionTableForFrameStyle( XDisplay pDisplay, EFrameStyle pFrameStyle );

		// Returns an XSizeHints structure, configured based on the specified frame style and geometry.
		// XSizeHints enables Window Manager to properly style the window and enable/disable geometry changes.
		XSizeHints _X11QuerySizeHintsForFrameStyle( XDisplay pDisplay, EFrameStyle pFrameStyle, const FrameGeometry & pFrameGeometry );

	}
	
	X11WindowManager::X11WindowManager( X11DisplayManagerHandle pDisplayManager )
	: X11NativeObject( std::move( pDisplayManager ) )
	{}

	X11WindowManager::~X11WindowManager() noexcept = default;

	WindowHandle X11WindowManager::_NativeCreateWindow( WindowCreateInfo pCreateInfo )
	{
		auto & xSessionData = Platform::X11GetXSessionData( *this );

		auto windowObject = CreateSysObject<X11Window>( GetHandle<X11WindowManager>() );

		Platform::X11WindowCreateInfo x11CreateInfo;
		x11CreateInfo.frameGeometry = pCreateInfo.frameGeometry;
		x11CreateInfo.title = std::move( pCreateInfo.title );
		x11CreateInfo.colorDepth = XDefaultDepth( xSessionData.displayHandle, xSessionData.screenIndex );
		x11CreateInfo.windowVisual = XDefaultVisual( xSessionData.displayHandle, xSessionData.screenIndex );

		Platform::X11CreateWindow( windowObject->mNativeData, x11CreateInfo );

		Platform::X11WindowPostCreateUpdate( windowObject->mNativeData, x11CreateInfo );

		return windowObject;
	}

	void X11WindowManager::_NativeDestroyWindow( Window & pWindow )
	{
		auto * x11Window = pWindow.QueryInterface<X11Window>();

		Platform::X11DestroyWindow( x11Window->mNativeData );
	}
	

	X11Window::X11Window( X11WindowManagerHandle pWindowManager )
	: X11NativeObject( std::move( pWindowManager ), &mNativeData )
	{}

	X11Window::~X11Window() noexcept = default;

	void X11Window::_NativeResize( const FrameSize & pFrameSize, EFrameSizeMode pSizeMode )
	{}

	void X11Window::_NativeSetFullscreenMode( bool pEnable )
	{
		Platform::X11SetWindowFullscreenState( mNativeData, pEnable );
	}

	void X11Window::_NativeSetTitle( const std::string & pTitle )
	{
		return Platform::X11SetFrameTitle( mNativeData, pTitle );
	}

	void X11Window::_NativeUpdateGeometry(
			const FrameGeometry & pFrameGeometry,
			cppx::bitmask<EFrameGeometryUpdateFlags> pUpdateFlags )
	{
		return Platform::X11UpdateFrameGeometry( mNativeData, pFrameGeometry, pUpdateFlags );
	}

	FrameSize X11Window::_NativeGetSize( EFrameSizeMode pSizeMode ) const
	{
		return Platform::X11GetFrameSize( mNativeData, pSizeMode );
	}


	namespace Platform
	{

		void X11CreateWindow( X11WindowNativeData & pWindowNativeData, const X11WindowCreateInfo & pCreateInfo )
		{
			auto & xSessionData = Platform::X11GetXSessionData( pWindowNativeData );

			// Colormap for our window.
			Colormap colormap = XCreateColormap( xSessionData.displayHandle,
			                                     xSessionData.rootWindowXID,
			                                     pCreateInfo.windowVisual,
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
			auto windowXID = XCreateWindow( xSessionData.displayHandle,
			                                xSessionData.rootWindowXID,
			                                0, 0,
			                                pCreateInfo.frameGeometry.size.x,
			                                pCreateInfo.frameGeometry.size.y,
			                                0u,
			                                pCreateInfo.colorDepth,
			                                InputOutput,
			                                pCreateInfo.windowVisual,
			                                windowSetAttributesMask,
			                                &windowSetAttributes );

			if( windowXID == eXIDNone )
			{
				Ic3Throw( eExcCodeDebugPlaceholder );
			}

			pWindowNativeData.mWindowXID = windowXID;
			pWindowNativeData.xColormap = colormap;

			// Window style requested for the window. There is no real "X11-equivalent" (like we have
			// on other OSes), because of the separation between X Server and the actual Window Manager.
			const auto windowStyle = pCreateInfo.frameGeometry.style;

			// First thing to enforce requested window behaviour: action table. It defines what kind of actions
			// (like moving, resizing, entering fullscreen) are allowed for a given window. Based on the style,
			// we query the table and set it for our newly created window.
			auto windowActionTable = Platform::_X11QueryActionTableForFrameStyle( xSessionData.displayHandle, windowStyle );

			// This could happen only if we added/changed something and forgot to adjust this code.
			Ic3DebugAssert( !windowActionTable.empty() );

			// The atom used to set this table is named "_NET_WM_ALLOWED_ACTIONS".
			Atom wmAllowedActions = XInternAtom( xSessionData.displayHandle, "_NET_WM_ALLOWED_ACTIONS", true );

			XChangeProperty( xSessionData.displayHandle,
			                 windowXID,
			                 wmAllowedActions,
			                 XA_ATOM,
			                 32,
			                 PropModeReplace,
			                 ( unsigned char * )windowActionTable.data(),
			                 ( int )windowActionTable.size() );

			auto windowSizeHints = Platform::_X11QuerySizeHintsForFrameStyle( xSessionData.displayHandle, windowStyle, pCreateInfo.frameGeometry );

			// Note, that this call must be done before the window is mapped to the screen. Otherwise, most WMs
			// will not recognize PMinSize/PMaxSize hints, making non-resizable windows very much resizable ones.
			XSetNormalHints( xSessionData.displayHandle, windowXID, &windowSizeHints );
		}

		void X11WindowPostCreateUpdate( X11WindowNativeData & pWindowNativeData, const X11WindowCreateInfo & pCreateInfo )
		{
			auto & xSessionData = Platform::X11GetXSessionData( pWindowNativeData );
			
			XMoveWindow( xSessionData.displayHandle,
			             pWindowNativeData.mWindowXID,
			             static_cast<int>( pCreateInfo.frameGeometry.position.x ),
			             static_cast<int>( pCreateInfo.frameGeometry.position.y ) );

			// Another observation: if this gets called before the initial XMoveWindow(), fullscreen windows
			// appear at a wrong position (and, thus, usually - on the wrong screen as well). Confirmed at least
			// on Kubuntu 20.04 and Mint. Non-fullscreen windows seem to work regardless of the call order.
			XMapWindow( xSessionData.displayHandle, pWindowNativeData.mWindowXID );

			if( pCreateInfo.fullscreenMode )
			{
				// This works by sending a _NET_WM_STATE update event to the window.
				// It strictly requires the window to be mapped already, so make sure
				// this always called after XMapWindow().
				X11SetWindowFullscreenState( pWindowNativeData, true );
			}

			Atom registeredWMProtocolArray[] =
			{
				xSessionData.atomCache.wmProtocolDelete,
				xSessionData.atomCache.wmProtocolDestroy
			};
			const auto registeredWMProtocolsNum = cppx::static_array_size( registeredWMProtocolArray );

			XSetWMProtocols( xSessionData.displayHandle,
			                 pWindowNativeData.mWindowXID,
			                 &( registeredWMProtocolArray[0] ),
			                 cppx::numeric_cast<int>( registeredWMProtocolsNum ) );

			XStoreName( xSessionData.displayHandle, pWindowNativeData.mWindowXID, pCreateInfo.title.c_str() );
			XFlush( xSessionData.displayHandle );
		}

		void X11DestroyWindow( X11WindowNativeData & pWindowNativeData )
		{
			auto & xSessionData = Platform::X11GetXSessionData( pWindowNativeData );

			XUnmapWindow( xSessionData.displayHandle, pWindowNativeData.mWindowXID );

			XDestroyWindow( xSessionData.displayHandle, pWindowNativeData.mWindowXID );
			pWindowNativeData.mWindowXID = eXIDNone;

			XFreeColormap( xSessionData.displayHandle, pWindowNativeData.xColormap );
			pWindowNativeData.xColormap = eXIDNone;

			XFlush( xSessionData.displayHandle );
		}

		void X11SetWindowFullscreenState( X11WindowNativeData & pWindowNativeData, bool pSetFullscreen )
		{
			const auto IsWindowFullscreen = X11IsFullscreenWindow( pWindowNativeData );

			if( IsWindowFullscreen == pSetFullscreen )
			{
				return;
			}

			auto & xSessionData = Platform::X11GetXSessionData( pWindowNativeData );

			XEvent wmEvent;
			memset( &wmEvent, 0, sizeof( wmEvent ) );
			wmEvent.type = ClientMessage;
			wmEvent.xclient.window = pWindowNativeData.mWindowXID;
			wmEvent.xclient.message_type = xSessionData.atomCache.wmState;
			wmEvent.xclient.format = 32;
			wmEvent.xclient.data.l[0] = pSetFullscreen ? 1 : 0;
			wmEvent.xclient.data.l[1] = cppx::numeric_cast<long>( xSessionData.atomCache.wmStateFullscreen );
			wmEvent.xclient.data.l[2] = 0;

			XSendEvent( xSessionData.displayHandle,
			            xSessionData.rootWindowXID,
			            False,
			            SubstructureRedirectMask | SubstructureNotifyMask,
			            &wmEvent );

			XFlush( xSessionData.displayHandle );
		}

		void X11SetFrameTitle( const X11WindowNativeData & pWindowNativeData, const std::string & pTitle )
		{
			auto & xSessionData = Platform::X11GetXSessionData( pWindowNativeData );

			XStoreName( xSessionData.displayHandle, pWindowNativeData.mWindowXID, pTitle.c_str() );

			XFlush( xSessionData.displayHandle );
		}

		void X11UpdateFrameGeometry( const X11WindowNativeData & pWindowNativeData,
		                             const FrameGeometry & pFrameGeometry,
		                             cppx::bitmask<EFrameGeometryUpdateFlags> pUpdateFlags )
		{
			auto & xSessionData = Platform::X11GetXSessionData( pWindowNativeData );

			if( pUpdateFlags.is_set( eFrameGeometryUpdateFlagPositionBit ) )
			{
				XMoveWindow( xSessionData.displayHandle,
				             pWindowNativeData.mWindowXID,
				             pFrameGeometry.position.x,
				             pFrameGeometry.position.y );
			}

			if( pUpdateFlags.is_set( eFrameGeometryUpdateFlagSizeBit ) )
			{
				XResizeWindow( xSessionData.displayHandle,
				               pWindowNativeData.mWindowXID,
				               pFrameGeometry.size.x,
				               pFrameGeometry.size.y );
			}

			XFlush( xSessionData.displayHandle );
		}

		FrameSize X11GetFrameSize( const X11WindowNativeData & pWindowNativeData, EFrameSizeMode pSizeMode )
		{
			auto & xSessionData = Platform::X11GetXSessionData( pWindowNativeData );

			XWindowAttributes windowAttributes;
			XGetWindowAttributes( xSessionData.displayHandle,
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

		bool X11IsFullscreenWindow( XDisplay pDisplay, XWindow pWindow )
		{
			return X11CheckWindowPropertyValueSet( pDisplay, pWindow, "_NET_WM_STATE", "_NET_WM_STATE_FULLSCREEN" );
		}

		bool X11IsFullscreenWindow( const X11WindowNativeData & pWindowNativeData )
		{
			auto & xSessionData = X11GetXSessionData( pWindowNativeData );
			return X11IsFullscreenWindow( xSessionData.displayHandle, pWindowNativeData.mWindowXID );
		}

		std::vector<Atom> X11QueryWindowPropertyValueArray( XDisplay pDisplay, XWindow pWindow, const char * pPropertyName )
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

			const long wmPropertyValuesNum = cppx::numeric_cast<long>( wmPropertyValueBytesNum ) / 4;

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

		bool X11CheckWindowPropertyValueSet( XDisplay pDisplay, XWindow pWindow, const char * pPropertyName, const char * pValueID )
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

			const long wmPropertyValuesNum = cppx::numeric_cast<long>( wmPropertyValueBytesNum ) / 4;

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

		std::vector<Atom> _X11QueryActionTableForFrameStyle( XDisplay pDisplay, EFrameStyle pFrameStyle )
		{
			const char ** windowActionTablePtr = nullptr;
			size_t windowActionTableSize = 0u;

			switch( pFrameStyle )
			{
				case EFrameStyle::Caption:
					windowActionTablePtr = &( sX11WindowActionTableCaption[0] );
					windowActionTableSize = cppx::static_array_size( sX11WindowActionTableCaption );
					break;

				case EFrameStyle::Fixed:
					windowActionTablePtr = &( sX11WindowActionTableFixed[0] );
					windowActionTableSize = cppx::static_array_size( sX11WindowActionTableFixed );
					break;

				case EFrameStyle::Overlay:
					windowActionTablePtr = &( sX11WindowActionTableOverlay[0] );
					windowActionTableSize = cppx::static_array_size( sX11WindowActionTableOverlay );
					break;

				case EFrameStyle::Resizeable:
					windowActionTablePtr = &( sX11WindowActionTableResizeable[0] );
					windowActionTableSize = cppx::static_array_size( sX11WindowActionTableResizeable );
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

		XSizeHints _X11QuerySizeHintsForFrameStyle( XDisplay pDisplay, EFrameStyle pFrameStyle, const FrameGeometry & pFrameGeometry )
		{
			XSizeHints windowSizeHints;

			windowSizeHints.flags = PPosition | PSize;
			windowSizeHints.x = static_cast<int>( pFrameGeometry.position.x );
			windowSizeHints.y = static_cast<int>( pFrameGeometry.position.y );
			windowSizeHints.width = static_cast<int>( pFrameGeometry.size.x );
			windowSizeHints.height = static_cast<int>( pFrameGeometry.size.y );

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
#endif // PCL_TARGET_SYSAPI_X11
