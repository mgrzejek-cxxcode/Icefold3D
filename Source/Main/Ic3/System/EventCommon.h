
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
		_Reserved
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
		_Reserved
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
		_Reserved
	};

	///
	constexpr auto cxEnumEventBaseTypeCount = static_cast< size_t >( EEventBaseType::_Reserved );

	///
	constexpr auto cxEnumEventCategoryCount = static_cast< size_t >( EEventCategory::_Reserved );

	///
	constexpr auto cxEnumEventCodeIndexCount = static_cast< size_t >( EEventCodeIndex::_Reserved );

	/// @brief
	struct EvtBase
	{
		//
		event_code_value_t eventCode;

		//
		perf_counter_value_t timeStamp;
	};


	namespace CxDef
	{

		///
		constexpr uint8 VBM_EVENT_CODE_CONTROL_KEY = 0xEC;

		/// @brief
		inline constexpr event_code_value_t declareEventCode( EEventBaseType pEventBaseType, EEventCategory pEventCategory, EEventCodeIndex pEventCodeIndex )
		{
			return ( ( ( uint32 )VBM_EVENT_CODE_CONTROL_KEY << 24 ) | ( ( uint32 )pEventBaseType << 16 ) | ( ( uint32 )pEventCategory << 8 ) | ( uint32 )pEventCodeIndex );
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

		inline constexpr uint8 GetEventCodeControlKey( event_code_value_t pEventCode )
		{
			return static_cast<uint8>( ( pEventCode >> 24 ) & 0xFF );
		}

		inline constexpr EEventBaseType GetEventCodeBaseType( event_code_value_t pEventCode )
		{
			return static_cast<EEventBaseType>( ( pEventCode >> 16 ) & 0xFF );
		}

		inline constexpr EEventCategory GetEventCodeCategory( event_code_value_t pEventCode )
		{
			return static_cast<EEventCategory>( ( pEventCode >> 8 ) & 0xFF );
		}

		inline constexpr EEventCodeIndex GetEventCodeCodeIndex( event_code_value_t pEventCode )
		{
			return static_cast<EEventCodeIndex>( pEventCode & 0xFF );
		}

		inline constexpr bool validateEventCode( event_code_value_t pEventCode )
		{
			return ( GetEventCodeControlKey( pEventCode ) == VBM_EVENT_CODE_CONTROL_KEY ) &&
			       ( GetEventCodeBaseType( pEventCode ) < EEventBaseType::_Reserved ) &&
			       ( GetEventCodeCategory( pEventCode ) < EEventCategory::_Reserved ) &&
			       ( GetEventCodeCodeIndex( pEventCode ) < EEventCodeIndex::_Reserved );
		}

	}

	enum EEventCode : event_code_value_t
	{
		eEventCodeUndefined = 0,

		eEventCodeAppActivityDisplayInit = CxDef::declareEventCodeAppActivity( EEventCodeIndex::AppActivityDisplayInit ),
		eEventCodeAppActivityDisplayTerm = CxDef::declareEventCodeAppActivity( EEventCodeIndex::AppActivityDisplayTerm ),
		eEventCodeAppActivityFocusGained = CxDef::declareEventCodeAppActivity( EEventCodeIndex::AppActivityFocusGained ),
		eEventCodeAppActivityFocusLost   = CxDef::declareEventCodeAppActivity( EEventCodeIndex::AppActivityFocusLost ),
		eEventCodeAppActivityPause       = CxDef::declareEventCodeAppActivity( EEventCodeIndex::AppActivityPause ),
		eEventCodeAppActivityResume      = CxDef::declareEventCodeAppActivity( EEventCodeIndex::AppActivityResume ),
		eEventCodeAppActivityStart       = CxDef::declareEventCodeAppActivity( EEventCodeIndex::AppActivityStart ),
		eEventCodeAppActivityStop        = CxDef::declareEventCodeAppActivity( EEventCodeIndex::AppActivityStop ),
		eEventCodeAppActivityQuit        = CxDef::declareEventCodeAppActivity( EEventCodeIndex::AppActivityQuit ),
		eEventCodeAppActivityTerminate   = CxDef::declareEventCodeAppActivity( EEventCodeIndex::AppActivityTerminate ),

		eEventCodeInputGamepadAxis   = CxDef::declareEventCodeInputGamepad( EEventCodeIndex::InputGamepadAxis ),
		eEventCodeInputGamepadButton = CxDef::declareEventCodeInputGamepad( EEventCodeIndex::InputGamepadButton ),
		eEventCodeInputGamepadState  = CxDef::declareEventCodeInputGamepad( EEventCodeIndex::InputGamepadState ),
		eEventCodeInputKeyboard      = CxDef::declareEventCodeInputKeyboard( EEventCodeIndex::InputKeyboard ),
		eEventCodeInputMouseButton   = CxDef::declareEventCodeInputMouse( EEventCodeIndex::InputMouseButton ),
		eEventCodeInputMouseMove     = CxDef::declareEventCodeInputMouse( EEventCodeIndex::InputMouseMove ),
		eEventCodeInputMouseScroll   = CxDef::declareEventCodeInputMouse( EEventCodeIndex::InputMouseScroll ),
		eEventCodeInputTouchDown     = CxDef::declareEventCodeInputTouch( EEventCodeIndex::InputTouchDown ),
		eEventCodeInputTouchMove     = CxDef::declareEventCodeInputTouch( EEventCodeIndex::InputTouchMove ),
		eEventCodeInputTouchUp       = CxDef::declareEventCodeInputTouch( EEventCodeIndex::InputTouchUp ),

		eEventCodeWindowUpdateCreate     = CxDef::declareEventCodeWindowUpdate( EEventCodeIndex::WindowUpdateCreate ),
		eEventCodeWindowUpdateDestroy    = CxDef::declareEventCodeWindowUpdate( EEventCodeIndex::WindowUpdateDestroy ),
		eEventCodeWindowUpdateFullscreen = CxDef::declareEventCodeWindowUpdate( EEventCodeIndex::WindowUpdateFullscreen ),
		eEventCodeWindowUpdateResize     = CxDef::declareEventCodeWindowUpdate( EEventCodeIndex::WindowUpdateResize ),
		eEventCodeWindowUpdateVisibility = CxDef::declareEventCodeWindowUpdate( EEventCodeIndex::WindowUpdateVisibility )
	};

} // namespace Ic3::System

#endif // __IC3_SYSTEM_EVENT_COMMON_H__
