
#pragma once

#ifndef __IC3_TOOLS_ICE_LOG_VIEWER_MAIN_WINDOW_H__
#define __IC3_TOOLS_ICE_LOG_VIEWER_MAIN_WINDOW_H__

#include <wx/wx.h>

namespace Ic3::Tools::IceLogViewer
{

    class MainWindow : public wxFrame
    {
    public:
        MainWindow( const wxString & pWindowTitle );
        virtual ~MainWindow();

        void OnQuit( wxCommandEvent & pEvent );

        void OnAbout( wxCommandEvent & pEvent );

	protected:
		virtual bool Layout() override final;

    private:
        void InitializeBaseContent();

    private:
        wxDECLARE_EVENT_TABLE();
    };

}

#endif // __IC3_TOOLS_ICE_LOG_VIEWER_MAIN_WINDOW_H__
