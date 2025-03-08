
#pragma once

#ifndef __IC3_TOOLS_ICE_LOG_VIEWER_MAIN_WINDOW_H__
#define __IC3_TOOLS_ICE_LOG_VIEWER_MAIN_WINDOW_H__

#include <QLabel>
#include <QMainWindow>

namespace Ic3::Tools::IceLogViewer
{

	class Application;

	class MainWindow : public QMainWindow
	{
	public:
		MainWindow( Application & pApplicationInstance );
		virtual ~MainWindow();

	private:
		void _Initialize();
		void _InitializeActions();
		void _InitializeMenus();

	private slots:
		void _ShAbout();

	private:
		Application * _applicationInstance;

		QLabel * _qtLabelInfo;

		QMenu * _qtMenuItemFile;
		QMenu * _qtMenuItemEdit;
		QMenu * _qtMenuItemTools;
		QMenu * _qtMenuItemHelp;

		QAction * _qtActFileConnect;
		QAction * _qtActFileSave;
		QAction * _qtActFileSaveAs;
		QAction * _qtActFileQuit;
		QAction * _qtActEditClear;
		QAction * _qtActEditFilters;
		QAction * _qtActToolsOptions;
		QAction * _qtActHelpAbout;
	};

}

#endif // __IC3_TOOLS_ICE_LOG_VIEWER_MAIN_WINDOW_H__
