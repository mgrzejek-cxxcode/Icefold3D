
#include "Application.h"
#include "UI/MainWindow.h"

namespace Ic3::Tools::IceLogViewer
{

	bool Application::OnInit()
	{
		if( !wxApp::OnInit() )
		{
			return false;
		}

		auto * mainWindow = new MainWindow( "IceLogViewer v0.1-alpha" );
		mainWindow->Show( true );

		return true;
	}

}
