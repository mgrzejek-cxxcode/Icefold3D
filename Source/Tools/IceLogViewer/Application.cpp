
#include "Application.h"
#include "UI/MainWindow.h"

#include <QScreen>

namespace Ic3::Tools::IceLogViewer
{

	Application::Application( int pArgc, char ** pArgv )
	: QApplication( pArgc, pArgv )
	, _qtAppScreen( Application::primaryScreen())
	{}

	Application ::~Application() = default;

	QSize Application::GetDisplaySize() const noexcept
	{
		return _qtAppScreen->size();
	}

}
