
#include "AndroidEventCore.h"
#include "AndroidSysContext.h"

#include <Ic3/System/Internal/EventCorePrivate.h>
#include <cxm/vectorOps.h>

#include <android/input.h>
#include <android/keycodes.h>

#if( PCL_TARGET_SYSAPI == PCL_TARGET_SYSAPI_ANDROID )
namespace Ic3::System
{

	namespace Platform
	{

		void _androidOnAppCommand( AndroidAppState * pAppState, int32_t pCommand );

		int32_t _androidOnInputEvent( AndroidAppState * pAppState, AInputEvent * pInputEvent );

		bool _androidTranslateAppCommand( AndroidAppState * pAppState, int32_t pCommand, EventObject & pOutEvent );

		bool _androidTranslateInputEvent( AndroidAppState * pAppState, AInputEvent * pInputEvent, EventObject & pOutEvent );

		bool _androidTranslateInputEventKey( AndroidAppState * pAppState, AInputEvent * pInputEvent, EventObject & pOutEvent );

		bool _androidTranslateInputEventTouch( AndroidAppState * pAppState, AInputEvent * pInputEvent, EventObject & pOutEvent );

	}


	AndroidEventController::AndroidEventController( SysContextHandle pSysContext )
	: AndroidNativeObject( std::move( pSysContext ) )
	{
		auto & aSessionData = Platform::AndroidGetASessionData( *this );
		aSessionData.aCommonAppState->onAppCmd = Platform::_androidOnAppCommand;
		aSessionData.aCommonAppState->onInputEvent = Platform::_androidOnInputEvent;
		aSessionData.aCommonAppState->Ic3SetUserData( Platform::eAndroidAppStateUserDataIndexEventController, this );
	}

	AndroidEventController::~AndroidEventController() noexcept
	{
		auto & aSessionData = Platform::AndroidGetASessionData( *this );
		aSessionData.aCommonAppState->onAppCmd = nullptr;
		aSessionData.aCommonAppState->onInputEvent = nullptr;
		aSessionData.aCommonAppState->Ic3SetUserData( Platform::eAndroidAppStateUserDataIndexEventController, nullptr );
	}

	void AndroidEventController::_NativeRegisterEventSource( EventSource & pEventSource )
	{}

	void AndroidEventController::_NativeUnRegisterEventSource( EventSource & pEventSource )
	{}

	bool AndroidEventController::_NativeDispatchPendingEvents()
	{
		// Note for Android event dispatching:
		// We use a modified native_app_glue code from the NDK to handle the Activity <--> C++ flow.
		// AndroidAppState (modified android_app struct) is the basic entity used to store any internal
		// data (to avoid introducing our custom types into the low-level android stuff).

		auto & aSessionData = Platform::AndroidGetASessionData( *this );

		int events = 0;
		AndroidPollSource * source = nullptr;

		// Poll single event without any wait ("peek mode"). We will get the source in case of success.
		auto pollResult = ALooper_pollOnce( Platform::cxAndroidEventLooperWaitTimeoutImmediate,
		                                    nullptr,
		                                    &events,
		                                    reinterpret_cast<void**>( &source ) );

		if( pollResult >= 0 )
		{
			if( source != nullptr )
			{
				// Process the dispatched event. This will be handled by the internal Android layer.
				// It uses two separate callbacks - one for general app commands and one for input
				// events (see nativeInitializeEventController() above, where these are set).
				source->process( aSessionData.aCommonAppState, source );

				if( aSessionData.aCommonAppState->destroyRequested != 0 )
				{
					// A bit of a hack, but we want to support a destroy request explicitly. There is no
					// dedicated event for that, but is signaled through the 'destroyRequested' variable.
					// So we use our own additional eAndroidEvtCmdUserDestroyRequested event code
					// and artificially emit it when the destroyRequested is set to true (the value changes
					// *after* process() is executed on a source).
					_androidOnAppCommand( aSessionData.aCommonAppState, Platform::eAndroidEvtCmdUserDestroyRequested );
				}

				return true;
			}
		}

		return false;
	}

	bool AndroidEventController::_NativeDispatchPendingEventsWait()
	{
		auto & aSessionData = Platform::AndroidGetASessionData( *this );

		int events = 0;
		AndroidPollSource * source = nullptr;

		// Same thing as inside nativeFetchNextEvent, but here we block if there is no event in the queue.
		auto pollResult = ALooper_pollOnce( Platform::cxAndroidEventLooperWaitTimeoutInfinity,
		                                    nullptr,
		                                    &events,
		                                    reinterpret_cast<void**>( &source ) );

		if( pollResult >= 0 )
		{
			if( source != nullptr )
			{
				source->process( aSessionData.aCommonAppState, source );

				if( aSessionData.aCommonAppState->destroyRequested != 0 )
				{
					_androidOnAppCommand( aSessionData.aCommonAppState,
					                      Platform::eAndroidEvtCmdUserDestroyRequested );
				}

				return true;
			}
		}

		return false;
	}


	namespace Platform
	{

		void _androidOnAppCommand( AndroidAppState * pAppState, int32_t pCommand )
		{
			AndroidNativeEvent androidEvent {};
			androidEvent.type = AndroidNativeEventType::AppCommand;
			androidEvent.uEvtAppCommand = pCommand;

			// Event controller is always accessible through the user data within AndroidAppState.
			// We set it inside nativeInitializeEventController(). Events may be emitted after the
			// framework state is released, so the null-check is necessary.
			if( auto * eventController = pAppState->Ic3GetUserDataAs<EventController>( eAndroidAppStateUserDataIndexEventController ) )
			{
				NativeEventDispatch( *eventController, androidEvent );
			}
		}

		int32_t _androidOnInputEvent( AndroidAppState * pAppState, AInputEvent * pInputEvent )
		{
			AndroidNativeEvent androidEvent {};
			androidEvent.type = AndroidNativeEventType::Input;
			androidEvent.eInputEvent = pInputEvent;

			if( auto * eventController = pAppState->Ic3GetUserDataAs<EventController>( eAndroidAppStateUserDataIndexEventController ) )
			{
				if( NativeEventDispatch( *eventController, androidEvent ) )
				{
					return 1;
				}
			}

			return 0;
		}

		bool NativeEventTranslate( EventController & pEventController, const NativeEventType & pNativeEvent, EventObject & pOutEvent )
		{
			auto * androidEventController = pEventController.QueryInterface<AndroidEventController>();
			auto & aSessionData = Platform::AndroidGetASessionData( *androidEventController );

			pOutEvent.commonData.eventCode != E_EVENT_CODE_UNDEFINED;

			if( pNativeEvent.type == AndroidNativeEventType::AppCommand )
			{
				if( _androidTranslateAppCommand( aSessionData.aCommonAppState, pNativeEvent.uEvtAppCommand, pOutEvent ) )
				{
					return true;
				}
			}
			else if( pNativeEvent.type == AndroidNativeEventType::Input )
			{
				if( _androidTranslateInputEvent( aSessionData.aCommonAppState, pNativeEvent.eInputEvent, pOutEvent ) )
				{
					return true;
				}
			}

			return false;
		}

		bool _androidTranslateAppCommand( AndroidAppState * pAppState, int32_t pCommand, EventObject & pOutEvent )
		{
			switch( pCommand )
			{
				case APP_CMD_INPUT_CHANGED:
				{
					break;
				}
				case APP_CMD_INIT_WINDOW:
				{
					if( pAppState->window != nullptr )
					{
						auto * sysContext = pAppState->Ic3GetUserDataAs<AndroidSysContext>( eAndroidAppStateUserDataIndexSysContext );
						sysContext->UpdateANativeWindowReference( pAppState->window );
						sysContext->mSysThreadJNIObject->NASetRequestedOrientation(
							pAppState->activity->clazz, eAndroidScreenOrientationLandscape );

						pOutEvent.code = E_EVENT_CODE_APP_ACTIVITY_DISPLAY_INIT;

						// JavaVM * javaVM = pAppState->activity->vm;
						// JNIEnv * jni;
						// javaVM->AttachCurrentThread( &jni, nullptr );
						// auto * jActivityClass = jni->GetObjectClass( pAppState->activity->clazz );
						// auto * jActivityMethod = jni->GetMethodID( jActivityClass, "setRequestedOrientation", "(I)V" );
						// jni->CallVoidMethod( pAppState->activity->clazz, jActivityMethod, 0 );
						// javaVM->DetachCurrentThread();
					}
					break;
				}
				case APP_CMD_TERM_WINDOW:
				{
					pOutEvent.code = E_EVENT_CODE_APP_ACTIVITY_DISPLAY_TERM;
					break;
				}
				case APP_CMD_WINDOW_RESIZED:
				{
					pOutEvent.code = E_EVENT_CODE_WINDOW_UPDATE_RESIZE;
					break;
				}
				case APP_CMD_WINDOW_REDRAW_NEEDED:
				case APP_CMD_CONTENT_RECT_CHANGED:
				{
					break;
				}
				case APP_CMD_GAINED_FOCUS:
				{
					pOutEvent.code = E_EVENT_CODE_APP_ACTIVITY_FOCUS_GAINED;
					break;
				}
				case APP_CMD_LOST_FOCUS:
				{
					pOutEvent.code = E_EVENT_CODE_APP_ACTIVITY_FOCUS_LOST;
					break;
				}
				case APP_CMD_CONFIG_CHANGED:
				case APP_CMD_LOW_MEMORY:
				{
					break;
				}
				case APP_CMD_START:
				{
					pOutEvent.code = E_EVENT_CODE_APP_ACTIVITY_START;
					break;
				}
				case APP_CMD_RESUME:
				{
					pOutEvent.code = E_EVENT_CODE_APP_ACTIVITY_RESUME;
					break;
				}
				case APP_CMD_SAVE_STATE:
				{
					break;
				}
				case APP_CMD_PAUSE:
				{
					pOutEvent.code = E_EVENT_CODE_APP_ACTIVITY_PAUSE;
					break;
				}
				case APP_CMD_STOP:
				{
					pOutEvent.code = E_EVENT_CODE_APP_ACTIVITY_STOP;
					break;
				}
				case APP_CMD_DESTROY:
				{
					pOutEvent.code = E_EVENT_CODE_APP_ACTIVITY_TERMINATE;
					break;
				}
				case eAndroidEvtCmdUserDestroyRequested:
				{
					pOutEvent.code = E_EVENT_CODE_APP_ACTIVITY_QUIT;
					break;
				}
				default:
				{
					break;
				}
			}

			return pOutEvent.code != E_EVENT_CODE_UNDEFINED;
		}

		bool _androidTranslateInputEvent( AndroidAppState * pAppState, AInputEvent * pInputEvent, EventObject & pOutEvent )
		{
			auto eventType = AInputEvent_getType( pInputEvent );

			if( eventType == AINPUT_EVENT_TYPE_KEY )
			{
				return _androidTranslateInputEventKey( pAppState, pInputEvent, pOutEvent );
			}
			else if( eventType == AINPUT_EVENT_TYPE_MOTION )
			{
				return _androidTranslateInputEventTouch( pAppState, pInputEvent, pOutEvent );
			}

			return false;
		}

		bool _androidTranslateInputEventKey( AndroidAppState * pAppState, AInputEvent * pInputEvent, EventObject & pOutEvent )
		{
			return false;
		}

		bool _androidTranslateInputEventTouch( AndroidAppState * pAppState, AInputEvent * pInputEvent, EventObject & pOutEvent )
		{
			auto * eventController = pAppState->Ic3GetUserDataAs<AndroidEventController>( eAndroidAppStateUserDataIndexEventController );

			int32_t eventType = AInputEvent_getType( pInputEvent );
			switch( eventType )
			{
				case AINPUT_EVENT_TYPE_MOTION:
				{
					auto & inputMouseState = eventController->getEventDispatcherInputState().inputMouseState;

					decltype( inputMouseState.lastCursorPos ) cursorPos;
					cursorPos.x = AMotionEvent_getX( pInputEvent, 0u );
					cursorPos.y = AMotionEvent_getY( pInputEvent, 0u );

					if( inputMouseState.lastCursorPos == CX_EVENT_MOUSE_POS_INVALID )
					{
						inputMouseState.lastCursorPos = cursorPos;
					}

					int32_t eventSource = AInputEvent_getSource( pInputEvent );
					if( eventSource == AINPUT_SOURCE_TOUCHSCREEN )
					{
						int action = AKeyEvent_getAction( pInputEvent );
						action &= AMOTION_EVENT_ACTION_MASK;

						switch( action )
						{
							case AMOTION_EVENT_ACTION_DOWN:
							{
								auto & eInputMouseButton = pOutEvent.eInputMouseButton;
								eInputMouseButton.eventCode = E_EVENT_CODE_INPUT_MOUSE_BUTTON;
								eInputMouseButton.cursorPos = cursorPos;
								eInputMouseButton.buttonAction = EMouseButtonActionType::Click;
								eInputMouseButton.buttonID = EMouseButtonID::Left;
								inputMouseState.lastCursorPos = cursorPos;
								break;
							}
							case AMOTION_EVENT_ACTION_MOVE:
							{
								auto & eInputMouseMove = pOutEvent.eInputMouseMove;
								eInputMouseMove.eventCode = E_EVENT_CODE_INPUT_MOUSE_MOVE;
								eInputMouseMove.cursorPos = cursorPos;
								eInputMouseMove.movementDelta = ( cursorPos - inputMouseState.lastCursorPos ) * 0.2f;
								inputMouseState.lastCursorPos = cursorPos;
								break;
							}
							case AMOTION_EVENT_ACTION_UP:
							{
								auto & eInputMouseButton = pOutEvent.eInputMouseButton;
								eInputMouseButton.eventCode = E_EVENT_CODE_INPUT_MOUSE_BUTTON;
								eInputMouseButton.cursorPos = cursorPos;
								eInputMouseButton.buttonAction = EMouseButtonActionType::Release;
								eInputMouseButton.buttonID = EMouseButtonID::Left;
								inputMouseState.lastCursorPos = cursorPos;
								break;
							}
						}
					}
				}
			}

			return pOutEvent.commonData.eventCode != E_EVENT_CODE_UNDEFINED;
		}

	}
	
}
#endif // PCL_TARGET_SYSAPI_ANDROID
