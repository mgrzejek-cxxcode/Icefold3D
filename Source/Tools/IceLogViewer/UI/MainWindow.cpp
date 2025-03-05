
#include "MainWindow.h"

namespace Ic3::Tools::IceLogViewer
{

    enum EMainWindowControlID
    {
        eMainWindowCIDMenuFileQuit = wxID_EXIT,
        eMainWindowCIDMenuHelpAbout = wxID_ABOUT,
    };

	wxPoint GetWindowPositionForCenterOrientation( const wxSize & pSize )
	{
		int screenWidth = 0;
		int screenHeight = 0;
		wxDisplaySize( &screenWidth, &screenHeight );

		return { ( screenWidth - pSize.x ) / 2, ( screenHeight - pSize.y ) / 2 };
	}

    MainWindow::MainWindow( const wxString & pWindowTitle )
    : wxFrame(
		nullptr,
		wxID_ANY,
		pWindowTitle,
		GetWindowPositionForCenterOrientation( wxSize( 1200, 900 ) ),
		wxSize( 1200, 900 ) )
    {
        InitializeBaseContent();
    }

    MainWindow::~MainWindow() = default;

    void MainWindow::OnQuit( wxCommandEvent & pEvent )
    {
        Close( true );
    }

    void MainWindow::OnAbout( wxCommandEvent & pEvent )
    {
        wxMessageBox(
            wxString::Format(
                "IceLogViewer\n"
				"Logging client for apps running on Icefold3D Engine."
                "\n\n"
                "wxWidgets version: %s\n"
                "System info: %s.",
                wxGetLibraryVersionInfo().GetVersionString(),
                wxGetOsDescription() ),
            "IceLogViewer: about info",
            wxOK | wxICON_INFORMATION,
            this );
    }

	bool MainWindow::Layout()
	{
		const auto baseResult = wxFrame::Layout();
		return baseResult;
	}

    void MainWindow::InitializeBaseContent()
    {
		SetAutoLayout( true );
		SetMinClientSize( wxSize( 800, 600 ) );

        wxMenu * fileMenu = new wxMenu();
        fileMenu->Append( eMainWindowCIDMenuFileQuit, "E&xit\tAlt-X", "Quit the app" );

        wxMenu * helpMenu = new wxMenu();
        helpMenu->Append( eMainWindowCIDMenuHelpAbout, "&About\tF1", "Show 'about' info" );

        wxMenuBar * menuBar = new wxMenuBar();
        menuBar->Append( fileMenu, "&File" );
        menuBar->Append( helpMenu, "&Help" );

        SetMenuBar( menuBar );

        CreateStatusBar( 2 );
        SetStatusText( "IceLogViewer: ready for connection" );

		auto * basePanel = new wxPanel( this, wxID_ANY, wxPoint( 0, 0 ), GetClientSize() );
		basePanel->SetAutoLayout( true );
		basePanel->SetBackgroundColour( wxColor( 0xEE, 0xEE, 0xEE, 0xFF ) );

		const auto basePanelSize = basePanel->GetSize();
		auto * textCtrl = new wxTextCtrl(
			basePanel,
			wxID_ANY,
			wxEmptyString,
			wxPoint( 25, 25 ),
			wxSize( basePanelSize.x - 50, basePanelSize.y - 50 ),
			wxTE_MULTILINE | wxTE_READONLY | wxTE_RICH2 | wxHSCROLL | wxTE_LEFT );
    }

    wxBEGIN_EVENT_TABLE( MainWindow, wxFrame )
    EVT_MENU( eMainWindowCIDMenuFileQuit, MainWindow::OnQuit )
    EVT_MENU( eMainWindowCIDMenuHelpAbout, MainWindow::OnAbout )
    wxEND_EVENT_TABLE()

}
