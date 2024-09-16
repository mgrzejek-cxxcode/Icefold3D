
#ifndef __IC3_SYSTEM_EVENT_DEF_WINDOW_H__
#define __IC3_SYSTEM_EVENT_DEF_WINDOW_H__

#include "EventCommon.h"
#include "WindowCommon.h"

namespace Ic3::System
{

	enum class EWindowVisibilityState : uint32
	{
		Unknown,
		Hidden,
		Visible
	};

	struct EvtWindow : public EvtBase
	{
	public:
		EventSource * mEventSource = nullptr;

	public:
		template <typename TObject>
		bool checkEventSource( const TObject * pSource ) const
		{
			return static_cast<const void *>( pSource ) == mEventSource;
		}
	};

	struct EvtWindowUpdateCreate : public EvtWindow
	{
	};

	struct EvtWindowUpdateDestroy : public EvtWindow
	{
	};

	struct EvtWindowUpdateFullscreen : public EvtWindow
	{
		EActiveState mFullscreenState = EActiveState::UNKNOWN;
	};

	struct EvtWindowUpdateResize : public EvtWindow
	{
		FrameSize mNewSize;
	};

	struct EvtWindowUpdateVisibility : public EvtWindow
	{
		EWindowVisibilityState mNewVisibilityState = EWindowVisibilityState::Unknown;
	};

} // namespace Ic3::System

#endif // __IC3_SYSTEM_EVENT_DEF_WINDOW_H__
