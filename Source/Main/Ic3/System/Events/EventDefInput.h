
#ifndef __IC3_SYSTEM_EVENT_DEF_INPUT_H__
#define __IC3_SYSTEM_EVENT_DEF_INPUT_H__

#include "EventCommon.h"

namespace Ic3::System
{

	struct EvtInput : public EvtBase
	{
	};

	struct EvtInputGamepadAxis : public EvtInput
	{
	};

	struct EvtInputGamepadButton : public EvtInput
	{
	};

	struct EvtInputGamepadState : public EvtInput
	{
	};

} // namespace Ic3::System

#endif // __IC3_SYSTEM_EVENT_DEF_INPUT_H__
