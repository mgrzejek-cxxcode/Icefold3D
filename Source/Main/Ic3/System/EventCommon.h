
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


	namespace CXU
	{

		///
		constexpr uint8 VBM_EVENT_CODE_CONTROL_KEY = 0xEC;

		/// @brief
		inline constexpr event_code_value_t DeclareEventCode( EEventBaseType pEventBaseType, EEventCategory pEventCategory, EEventCodeIndex pEventCodeIndex )
		{
			return ( ( ( uint32 )VBM_EVENT_CODE_CONTROL_KEY << 24 ) | ( ( uint32 )pEventBaseType << 16 ) | ( ( uint32 )pEventCategory << 8 ) | ( uint32 )pEventCodeIndex );
		}

		inline constexpr event_code_value_t DeclareEventCodeAppActivity( EEventCodeIndex pEventCodeIndex )
		{
			return DeclareEventCode( EEventBaseType::App, EEventCategory::AppActivity, pEventCodeIndex );
		}

		inline constexpr event_code_value_t DeclareEventCodeInputGamepad( EEventCodeIndex pEventCodeIndex )
		{
			return DeclareEventCode( EEventBaseType::Input, EEventCategory::InputGamepad, pEventCodeIndex );
		}

		inline constexpr event_code_value_t DeclareEventCodeInputKeyboard( EEventCodeIndex pEventCodeIndex )
		{
			return DeclareEventCode( EEventBaseType::Input, EEventCategory::InputKeyboard, pEventCodeIndex );
		}

		inline constexpr event_code_value_t DeclareEventCodeInputMouse( EEventCodeIndex pEventCodeIndex )
		{
			return DeclareEventCode( EEventBaseType::Input, EEventCategory::InputMouse, pEventCodeIndex );
		}

		inline constexpr event_code_value_t DeclareEventCodeInputTouch( EEventCodeIndex pEventCodeIndex )
		{
			return DeclareEventCode( EEventBaseType::Input, EEventCategory::InputTouch, pEventCodeIndex );
		}

		inline constexpr event_code_value_t DeclareEventCodeWindowUpdate( EEventCodeIndex pEventCodeIndex )
		{
			return DeclareEventCode( EEventBaseType::Window, EEventCategory::WindowUpdate, pEventCodeIndex );
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

		eEventCodeAppActivityDisplayInit = CXU::DeclareEventCodeAppActivity( EEventCodeIndex::AppActivityDisplayInit ),
		eEventCodeAppActivityDisplayTerm = CXU::DeclareEventCodeAppActivity( EEventCodeIndex::AppActivityDisplayTerm ),
		eEventCodeAppActivityFocusGained = CXU::DeclareEventCodeAppActivity( EEventCodeIndex::AppActivityFocusGained ),
		eEventCodeAppActivityFocusLost   = CXU::DeclareEventCodeAppActivity( EEventCodeIndex::AppActivityFocusLost ),
		eEventCodeAppActivityPause       = CXU::DeclareEventCodeAppActivity( EEventCodeIndex::AppActivityPause ),
		eEventCodeAppActivityResume      = CXU::DeclareEventCodeAppActivity( EEventCodeIndex::AppActivityResume ),
		eEventCodeAppActivityStart       = CXU::DeclareEventCodeAppActivity( EEventCodeIndex::AppActivityStart ),
		eEventCodeAppActivityStop        = CXU::DeclareEventCodeAppActivity( EEventCodeIndex::AppActivityStop ),
		eEventCodeAppActivityQuit        = CXU::DeclareEventCodeAppActivity( EEventCodeIndex::AppActivityQuit ),
		eEventCodeAppActivityTerminate   = CXU::DeclareEventCodeAppActivity( EEventCodeIndex::AppActivityTerminate ),

		eEventCodeInputGamepadAxis   = CXU::DeclareEventCodeInputGamepad( EEventCodeIndex::InputGamepadAxis ),
		eEventCodeInputGamepadButton = CXU::DeclareEventCodeInputGamepad( EEventCodeIndex::InputGamepadButton ),
		eEventCodeInputGamepadState  = CXU::DeclareEventCodeInputGamepad( EEventCodeIndex::InputGamepadState ),
		eEventCodeInputKeyboard      = CXU::DeclareEventCodeInputKeyboard( EEventCodeIndex::InputKeyboard ),
		eEventCodeInputMouseButton   = CXU::DeclareEventCodeInputMouse( EEventCodeIndex::InputMouseButton ),
		eEventCodeInputMouseMove     = CXU::DeclareEventCodeInputMouse( EEventCodeIndex::InputMouseMove ),
		eEventCodeInputMouseScroll   = CXU::DeclareEventCodeInputMouse( EEventCodeIndex::InputMouseScroll ),
		eEventCodeInputTouchDown     = CXU::DeclareEventCodeInputTouch( EEventCodeIndex::InputTouchDown ),
		eEventCodeInputTouchMove     = CXU::DeclareEventCodeInputTouch( EEventCodeIndex::InputTouchMove ),
		eEventCodeInputTouchUp       = CXU::DeclareEventCodeInputTouch( EEventCodeIndex::InputTouchUp ),

		eEventCodeWindowUpdateCreate     = CXU::DeclareEventCodeWindowUpdate( EEventCodeIndex::WindowUpdateCreate ),
		eEventCodeWindowUpdateDestroy    = CXU::DeclareEventCodeWindowUpdate( EEventCodeIndex::WindowUpdateDestroy ),
		eEventCodeWindowUpdateFullscreen = CXU::DeclareEventCodeWindowUpdate( EEventCodeIndex::WindowUpdateFullscreen ),
		eEventCodeWindowUpdateResize     = CXU::DeclareEventCodeWindowUpdate( EEventCodeIndex::WindowUpdateResize ),
		eEventCodeWindowUpdateVisibility = CXU::DeclareEventCodeWindowUpdate( EEventCodeIndex::WindowUpdateVisibility )
	};

} // namespace Ic3::System

#endif // __IC3_SYSTEM_EVENT_COMMON_H__
