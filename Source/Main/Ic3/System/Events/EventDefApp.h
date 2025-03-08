
#ifndef __IC3_SYSTEM_EVENT_DEF_APP_H__
#define __IC3_SYSTEM_EVENT_DEF_APP_H__

#include "EventCommon.h"

namespace Ic3::System
{

	struct EvtApp : public EvtBase
	{
	};

	struct EvtAppActivityDisplayInit : public EvtApp
	{
	};

	struct EvtAppActivityDisplayTerm : public EvtApp
	{
	};

	struct EvtAppActivityFocusGained : public EvtApp
	{
	};

	struct EvtAppActivityFocusLost : public EvtApp
	{
	};

	struct EvtAppActivityPause : public EvtApp
	{
	};

	struct EvtAppActivityResume : public EvtApp
	{
	};

	struct EvtAppActivityStart : public EvtApp
	{
	};

	struct EvtAppActivityStop : public EvtApp
	{
	};

	struct EvtAppActivityQuit : public EvtApp
	{
	};

	struct EvtAppActivityTerminate : public EvtApp
	{
	};

} // namespace Ic3::System

#endif // __IC3_SYSTEM_EVENT_DEF_APP_H__
