
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
		EventSource * eventSource = nullptr;

	public:
		template <typename TObject>
		bool checkEventSource( const TObject * pSource ) const
		{
			return static_cast<const void *>( pSource ) == eventSource;
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
		EActiveState fullscreenState = EActiveState::Unknown;
	};

	struct EvtWindowUpdateResize : public EvtWindow
	{
		FrameSize newSize;
	};

	struct EvtWindowUpdateVisibility : public EvtWindow
	{
		EWindowVisibilityState newVisibilityState = EWindowVisibilityState::Unknown;
	};

} // namespace Ic3::System

#endif // __IC3_SYSTEM_EVENT_DEF_WINDOW_H__
