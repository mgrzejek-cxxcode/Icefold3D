
#ifndef __IC3_SYSTEM_EVENT_COMMON_H__
#define __IC3_SYSTEM_EVENT_COMMON_H__

#include "PerfCounter.h"
#include <Ic3/Math/VectorOps.h>
#include <functional>

namespace Ic3::System
{

	struct EventObject;

	class EventController;
	class EventDispatcher;
	class EventSource;

	/// @brief
	using EventHandler = std::function<bool( EventObject & )>;

	/// @brief
	using event_code_value_t = uint32;

	/// @brief
	enum class EEventBaseType : uint8
	{
		Undefined,
		App,
		Input,
		Window,
		_ReservedMax
	};

	/// @brief
	enum class EEventCategory : uint8
	{
		Undefined,
		AppActivity,
		InputGamepad,
		InputKeyboard,
		InputMouse,
		InputTouch,
		WindowUpdate,
		_ReservedMax
	};

	/// @brief
	enum class EEventCodeIndex : uint8
	{
		Undefined,
		AppActivityDisplayInit,
		AppActivityDisplayTerm,
		AppActivityFocusGained,
		AppActivityFocusLost,
		AppActivityPause,
		AppActivityResume,
		AppActivityStart,
		AppActivityStop,
		AppActivityQuit,
		AppActivityTerminate,
		InputGamepadAxis,
		InputGamepadButton,
		InputGamepadState,
		InputKeyboard,
		InputMouseButton,
		InputMouseMove,
		InputMouseScroll,
		InputTouchDown,
		InputTouchMove,
		InputTouchUp,
		WindowUpdateCreate,
		WindowUpdateDestroy,
		WindowUpdateFullscreen,
		WindowUpdateResize,
		WindowUpdateVisibility,
		_ReservedMax
	};

	constexpr uint8 CX_EVENT_CODE_CONTROL_KEY = 0xEC;

	constexpr auto CX_ENUM_EVENT_BASE_TYPE_COUNT = static_cast< size_t >( EEventBaseType::_ReservedMax );

	constexpr auto CX_ENUM_EVENT_CATEGORY_COUNT = static_cast< size_t >( EEventCategory::_ReservedMax );

	constexpr auto CX_ENUM_EVENT_CODE_INDEX_COUNT = static_cast< size_t >( EEventCodeIndex::_ReservedMax );

	/// @brief
	struct EvtBase
	{
		//
		event_code_value_t eventCode;

		//
		perf_counter_value_t timeStamp;
	};


	namespace CxDefs
	{

		inline constexpr event_code_value_t declareEventCode( EEventBaseType pEventBaseType, EEventCategory pEventCategory, EEventCodeIndex pEventCodeIndex )
		{
			return ( ( (uint32)CX_EVENT_CODE_CONTROL_KEY << 24 ) | ( (uint32)pEventBaseType << 16 ) | ( (uint32)pEventCategory << 8 ) | (uint32)pEventCodeIndex );
		}

		inline constexpr event_code_value_t declareEventCodeAppActivity( EEventCodeIndex pEventCodeIndex )
		{
			return declareEventCode( EEventBaseType::App, EEventCategory::AppActivity, pEventCodeIndex );
		}

		inline constexpr event_code_value_t declareEventCodeInputGamepad( EEventCodeIndex pEventCodeIndex )
		{
			return declareEventCode( EEventBaseType::Input, EEventCategory::InputGamepad, pEventCodeIndex );
		}

		inline constexpr event_code_value_t declareEventCodeInputKeyboard( EEventCodeIndex pEventCodeIndex )
		{
			return declareEventCode( EEventBaseType::Input, EEventCategory::InputKeyboard, pEventCodeIndex );
		}

		inline constexpr event_code_value_t declareEventCodeInputMouse( EEventCodeIndex pEventCodeIndex )
		{
			return declareEventCode( EEventBaseType::Input, EEventCategory::InputMouse, pEventCodeIndex );
		}

		inline constexpr event_code_value_t declareEventCodeInputTouch( EEventCodeIndex pEventCodeIndex )
		{
			return declareEventCode( EEventBaseType::Input, EEventCategory::InputTouch, pEventCodeIndex );
		}

		inline constexpr event_code_value_t declareEventCodeWindowUpdate( EEventCodeIndex pEventCodeIndex )
		{
			return declareEventCode( EEventBaseType::Window, EEventCategory::WindowUpdate, pEventCodeIndex );
		}

		inline constexpr uint8 getEventCodeControlKey( event_code_value_t pEventCode )
		{
			return static_cast<uint8>( ( pEventCode >> 24 ) & 0xFF );
		}

		inline constexpr EEventBaseType getEventCodeBaseType( event_code_value_t pEventCode )
		{
			return static_cast<EEventBaseType>( ( pEventCode >> 16 ) & 0xFF );
		}

		inline constexpr EEventCategory getEventCodeCategory( event_code_value_t pEventCode )
		{
			return static_cast<EEventCategory>( ( pEventCode >> 8 ) & 0xFF );
		}

		inline constexpr EEventCodeIndex getEventCodeCodeIndex( event_code_value_t pEventCode )
		{
			return static_cast<EEventCodeIndex>( pEventCode & 0xFF );
		}

		inline constexpr bool validateEventCode( event_code_value_t pEventCode )
		{
			return ( getEventCodeControlKey( pEventCode ) == CX_EVENT_CODE_CONTROL_KEY ) &&
			       ( getEventCodeBaseType( pEventCode ) < EEventBaseType::_ReservedMax ) &&
			       ( getEventCodeCategory( pEventCode ) < EEventCategory::_ReservedMax ) &&
			       ( getEventCodeCodeIndex( pEventCode ) < EEventCodeIndex::_ReservedMax );
		}

	}

	enum EEventCode : event_code_value_t
	{
		E_EVENT_CODE_UNDEFINED = 0,
		E_EVENT_CODE_APP_ACTIVITY_DISPLAY_INIT  = CxDefs::declareEventCodeAppActivity( EEventCodeIndex::AppActivityDisplayInit ),
		E_EVENT_CODE_APP_ACTIVITY_DISPLAY_TERM  = CxDefs::declareEventCodeAppActivity( EEventCodeIndex::AppActivityDisplayTerm ),
		E_EVENT_CODE_APP_ACTIVITY_FOCUS_GAINED  = CxDefs::declareEventCodeAppActivity( EEventCodeIndex::AppActivityFocusGained ),
		E_EVENT_CODE_APP_ACTIVITY_FOCUS_LOST    = CxDefs::declareEventCodeAppActivity( EEventCodeIndex::AppActivityFocusLost ),
		E_EVENT_CODE_APP_ACTIVITY_PAUSE         = CxDefs::declareEventCodeAppActivity( EEventCodeIndex::AppActivityPause ),
		E_EVENT_CODE_APP_ACTIVITY_RESUME        = CxDefs::declareEventCodeAppActivity( EEventCodeIndex::AppActivityResume ),
		E_EVENT_CODE_APP_ACTIVITY_START         = CxDefs::declareEventCodeAppActivity( EEventCodeIndex::AppActivityStart ),
		E_EVENT_CODE_APP_ACTIVITY_STOP          = CxDefs::declareEventCodeAppActivity( EEventCodeIndex::AppActivityStop ),
		E_EVENT_CODE_APP_ACTIVITY_QUIT          = CxDefs::declareEventCodeAppActivity( EEventCodeIndex::AppActivityQuit ),
		E_EVENT_CODE_APP_ACTIVITY_TERMINATE     = CxDefs::declareEventCodeAppActivity( EEventCodeIndex::AppActivityTerminate ),

		E_EVENT_CODE_INPUT_GAMEPAD_AXIS    = CxDefs::declareEventCodeInputGamepad( EEventCodeIndex::InputGamepadAxis ),
		E_EVENT_CODE_INPUT_GAMEPAD_BUTTON  = CxDefs::declareEventCodeInputGamepad( EEventCodeIndex::InputGamepadButton ),
		E_EVENT_CODE_INPUT_GAMEPAD_STATE   = CxDefs::declareEventCodeInputGamepad( EEventCodeIndex::InputGamepadState ),

		E_EVENT_CODE_INPUT_KEYBOARD  = CxDefs::declareEventCodeInputKeyboard( EEventCodeIndex::InputKeyboard ),

		E_EVENT_CODE_INPUT_MOUSE_BUTTON  = CxDefs::declareEventCodeInputMouse( EEventCodeIndex::InputMouseButton ),
		E_EVENT_CODE_INPUT_MOUSE_MOVE    = CxDefs::declareEventCodeInputMouse( EEventCodeIndex::InputMouseMove ),
		E_EVENT_CODE_INPUT_MOUSE_SCROLL  = CxDefs::declareEventCodeInputMouse( EEventCodeIndex::InputMouseScroll ),

		E_EVENT_CODE_INPUT_TOUCH_DOWN  = CxDefs::declareEventCodeInputTouch( EEventCodeIndex::InputTouchDown ),
		E_EVENT_CODE_INPUT_TOUCH_MOVE  = CxDefs::declareEventCodeInputTouch( EEventCodeIndex::InputTouchMove ),
		E_EVENT_CODE_INPUT_TOUCH_UP    = CxDefs::declareEventCodeInputTouch( EEventCodeIndex::InputTouchUp ),

		E_EVENT_CODE_WINDOW_UPDATE_CREATE     = CxDefs::declareEventCodeWindowUpdate( EEventCodeIndex::WindowUpdateCreate ),
		E_EVENT_CODE_WINDOW_UPDATE_DESTROY    = CxDefs::declareEventCodeWindowUpdate( EEventCodeIndex::WindowUpdateDestroy ),
		E_EVENT_CODE_WINDOW_UPDATE_FULLSCREEN = CxDefs::declareEventCodeWindowUpdate( EEventCodeIndex::WindowUpdateFullscreen ),
		E_EVENT_CODE_WINDOW_UPDATE_RESIZE     = CxDefs::declareEventCodeWindowUpdate( EEventCodeIndex::WindowUpdateResize ),
		E_EVENT_CODE_WINDOW_UPDATE_VISIBILITY = CxDefs::declareEventCodeWindowUpdate( EEventCodeIndex::WindowUpdateVisibility )
	};

} // namespace Ic3::System

#endif // __IC3_SYSTEM_EVENT_COMMON_H__
