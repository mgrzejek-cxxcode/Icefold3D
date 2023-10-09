
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
			EEventCode                 code;
			EvtBase                    commonData;
			EvtAppActivityDisplayInit  eAppActivityDisplayInit;
			EvtAppActivityDisplayTerm  eAppActivityDisplayTerm;
			EvtAppActivityFocusGained  eAppActivityFocusGained;
			EvtAppActivityFocusLost    eAppActivityFocusLost;
			EvtAppActivityPause        eAppActivityPause;
			EvtAppActivityResume       eAppActivityResume;
			EvtAppActivityStart        eAppActivityStart;
			EvtAppActivityStop         eAppActivityStop;
			EvtAppActivityQuit         eAppActivityQuit;
			EvtAppActivityTerminate    eAppActivityTerminate;
			EvtInputGamepadAxis        eInputGamepadAxis;
			EvtInputGamepadButton      eInputGamepadButton;
			EvtInputGamepadState       eInputGamepadState;
			EvtInputKeyboard           eInputKeyboard;
			EvtInputMouse              eInputMouse;
			EvtInputMouseButton        eInputMouseButton;
			EvtInputMouseMove          eInputMouseMove;
			EvtInputMouseScroll        eInputMouseScroll;
			EvtInputTouchDown          eInputTouchDown;
			EvtInputTouchMove          eInputTouchMove;
			EvtInputTouchUp            eInputTouchUp;
			EvtWindowUpdateCreate      eWindowUpdateCreate;
			EvtWindowUpdateDestroy     eWindowUpdateDestroy;
			EvtWindowUpdateFullscreen  eWindowUpdateFullscreen;
			EvtWindowUpdateResize      eWindowUpdateResize;
			EvtWindowUpdateVisibility  eWindowUpdateVisibility;
		};

		EventSystemSharedState * eventSystemSharedState = nullptr;

		constexpr EventObject()
		: code{ E_EVENT_CODE_UNDEFINED }
		{}

		constexpr explicit EventObject( EEventCode pEventCode )
		: code{ pEventCode }
		{}

		constexpr explicit EventObject( event_code_value_t pEventCode )
		: code{ static_cast<EEventCode>( pEventCode ) }
		{}

		constexpr explicit operator bool() const
		{
			return ( code != E_EVENT_CODE_UNDEFINED ) && CxDefs::validateEventCode( code );
		}

		 IC3_ATTR_NO_DISCARD constexpr EEventCategory category() const
		{
			return CxDefs::getEventCodeCategory( code );
		}
	};

} // namespace Ic3::System

#endif // __IC3_SYSTEM_EVENT_OBJECT_H__
