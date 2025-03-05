
#pragma once

#ifndef __IC3_TOOLS_ICE_LOG_VIEWER_APPLICATION_H__
#define __IC3_TOOLS_ICE_LOG_VIEWER_APPLICATION_H__

#include <wx/wx.h>

namespace Ic3::Tools::IceLogViewer
{

	class Application : public wxApp
	{
	public:
		virtual bool OnInit() override final;
	};

}

#endif // __IC3_TOOLS_ICE_LOG_VIEWER_APPLICATION_H__
