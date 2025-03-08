
#include "Application.h"
#include "UI/MainWindow.h"

int main( int pArgc, char ** pArgv )
{
	Ic3::Tools::IceLogViewer::Application appInstance{ pArgc, pArgv };
	Ic3::Tools::IceLogViewer::MainWindow mainWindow{ appInstance };

	mainWindow.show();

	return appInstance.exec();
}

