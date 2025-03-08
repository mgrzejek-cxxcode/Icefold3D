
#ifndef __IC3_SYSTEM_EVENT_OBJECT_H__
#define __IC3_SYSTEM_EVENT_OBJECT_H__

#include "EventDefApp.h"
#include "EventDefInputKeyboard.h"
#include "EventDefInputMouse.h"
#include "EventDefInputTouch.h"
#include "EventDefWindow.h"

namespace Ic3::System
{

	struct EventSystemSharedState;

	struct EventObject
	{
		union
		{
			EEventCode                 uCode;
			EvtBase                    uCommonData;
			EvtAppActivityDisplayInit  uEvtAppActivityDisplayInit;
			EvtAppActivityDisplayTerm  uEvtAppActivityDisplayTerm;
			EvtAppActivityFocusGained  uEvtAppActivityFocusGained;
			EvtAppActivityFocusLost    uEvtAppActivityFocusLost;
			EvtAppActivityPause        uEvtAppActivityPause;
			EvtAppActivityResume       uEvtAppActivityResume;
			EvtAppActivityStart        uEvtAppActivityStart;
			EvtAppActivityStop         uEvtAppActivityStop;
			EvtAppActivityQuit         uEvtAppActivityQuit;
			EvtAppActivityTerminate    uEvtAppActivityTerminate;
			EvtInputGamepadAxis        uEvtInputGamepadAxis;
			EvtInputGamepadButton      uEvtInputGamepadButton;
			EvtInputGamepadState       uEvtInputGamepadState;
			EvtInputKeyboard           uEvtInputKeyboard;
			EvtInputMouse              uEvtInputMouse;
			EvtInputMouseButton        uEvtInputMouseButton;
			EvtInputMouseMove          uEvtInputMouseMove;
			EvtInputMouseScroll        uEvtInputMouseScroll;
			EvtInputTouchDown          uEvtInputTouchDown;
			EvtInputTouchMove          uEvtInputTouchMove;
			EvtInputTouchUp            uEvtInputTouchUp;
			EvtWindowUpdateCreate      uEvtWindowUpdateCreate;
			EvtWindowUpdateDestroy     uEvtWindowUpdateDestroy;
			EvtWindowUpdateFullscreen  uEvtWindowUpdateFullscreen;
			EvtWindowUpdateResize      uEvtWindowUpdateResize;
			EvtWindowUpdateVisibility  uEvtWindowUpdateVisibility;
		};

		EventSystemSharedState * eventSystemSharedState = nullptr;

		constexpr EventObject()
		: uCode{ eEventCodeUndefined }
		{}

		constexpr explicit EventObject( EEventCode pEventCode )
		: uCode{ pEventCode }
		{}

		constexpr explicit EventObject( event_code_value_t pEventCode )
		: uCode{ static_cast<EEventCode>( pEventCode ) }
		{}

		constexpr explicit operator bool() const
		{
			return ( uCode != eEventCodeUndefined ) && CXU::validateEventCode( uCode );
		}

		CPPX_ATTR_NO_DISCARD constexpr EEventCategory GetCategory() const
		{
			return CXU::GetEventCodeCategory( uCode );
		}
	};

} // namespace Ic3::System

#endif // __IC3_SYSTEM_EVENT_OBJECT_H__
