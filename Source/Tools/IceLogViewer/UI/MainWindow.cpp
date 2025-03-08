
#include "MainWindow.h"
#include "../Application.h"

#include <QLayout>
#include <QMenuBar>
#include <QMessageBox>
#include <QScreen>
#include <QStatusBar>
#include <QProgressBar>
#include <QTextEdit>

namespace Ic3::Tools::IceLogViewer
{

	static QPoint GetWindowPositionForCenterOrientation( Application & pApplication, const QSize & pWindowSize )
	{
		const auto displaySize = pApplication.GetDisplaySize();
		return { ( displaySize.width() - pWindowSize.width() ) / 2, ( displaySize.height() - pWindowSize.height() ) / 2};
	}

	MainWindow::MainWindow( Application & pApplicationInstance )
	: QMainWindow( nullptr )
	, _applicationInstance( &pApplicationInstance )
	{
		_Initialize();
	}

	MainWindow::~MainWindow() = default;

	void MainWindow::_Initialize()
	{
		const auto kMinimumWindowWidth = 800;
		const auto kMinimumWindowHeight = 1000;
		const auto initialWindowSize = QSize( kMinimumWindowWidth, kMinimumWindowHeight );
		const auto initialWindowPos = GetWindowPositionForCenterOrientation( *_applicationInstance, initialWindowSize );

		resize( initialWindowSize );
		move( initialWindowPos );
		setWindowTitle( Application::applicationName() );
		setWindowFlags( Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint );
		setMinimumWidth( kMinimumWindowWidth );
		setMinimumHeight( kMinimumWindowHeight );

		_InitializeActions();
		_InitializeMenus();

		auto * qtMainWidget = new QWidget();
		qtMainWidget->resize( size() );
		setCentralWidget( qtMainWidget );

		const auto mainWidgetSize = qtMainWidget->size();

		const auto kFixedConfigLeftPaneWidth = 300;
		const auto kFixedConfigLeftPaneHeight = 800;

		auto * configPaneWidget = new QWidget( qtMainWidget  );
		configPaneWidget->move( QPoint( 0, 0 ) );
		configPaneWidget->setFixedSize( QSize( kFixedConfigLeftPaneWidth, kFixedConfigLeftPaneHeight ) );

		auto * mainTextControl = new QTextEdit( qtMainWidget );
		configPaneWidget->move( QPoint( kFixedConfigLeftPaneWidth, 0 ) );
		configPaneWidget->resize( QSize( mainWidgetSize.width() - kFixedConfigLeftPaneWidth, kMinimumWindowHeight ) );

		auto * statusBar = new QStatusBar( this );
		statusBar->showMessage( "SomeStatusMessage" );

		auto * statusLabel = new QLabel( this );
		statusLabel->setText( "Status Label" );

		auto * statusProgressBar = new QProgressBar( this );
		statusProgressBar->setTextVisible( false );
		statusProgressBar->setValue( 51 );
		statusProgressBar->setFixedWidth( 400 );

		statusBar->addPermanentWidget( statusLabel );
		statusBar->addPermanentWidget( statusProgressBar, 1 );

		setStatusBar( statusBar );
	}

	void MainWindow::_InitializeActions()
	{
		_qtActFileConnect = new QAction( QIcon::fromTheme( QIcon::ThemeIcon::NetworkWireless ), tr( "&Connect" ), this );
		_qtActFileSave = new QAction( QIcon::fromTheme( QIcon::ThemeIcon::DocumentSave ), tr( "&Save" ), this );
		_qtActFileSaveAs = new QAction( QIcon::fromTheme( QIcon::ThemeIcon::DocumentSaveAs ), tr( "Save As..." ), this );
		_qtActFileQuit = new QAction( QIcon::fromTheme( QIcon::ThemeIcon::ApplicationExit ), tr( "&Quit" ), this );

		_qtActEditClear = new QAction( QIcon::fromTheme( QIcon::ThemeIcon::EditClear ), tr( "&Clear" ), this );
		_qtActEditFilters = new QAction( QIcon::fromTheme( QIcon::ThemeIcon::ToolsCheckSpelling ), tr( "&Filters" ), this );

		_qtActToolsOptions = new QAction( QIcon::fromTheme( QIcon::ThemeIcon::DocumentProperties ), tr( "&Options" ), this );

		_qtActHelpAbout = new QAction( QIcon::fromTheme( QIcon::ThemeIcon::HelpAbout ), tr( "&About" ), this );
		connect( _qtActHelpAbout, &QAction::triggered, this, &MainWindow::_ShAbout );
	}

	void MainWindow::_InitializeMenus()
	{
		menuBar()->setContentsMargins( 1, 1, 1, 1 );

		_qtMenuItemFile = menuBar()->addMenu( tr( "&File" ) );
		_qtMenuItemFile->addAction( _qtActFileConnect );
		_qtMenuItemFile->addSeparator();
		_qtMenuItemFile->addAction( _qtActFileSave );
		_qtMenuItemFile->addAction( _qtActFileSaveAs );
		_qtMenuItemFile->addSeparator();
		_qtMenuItemFile->addAction( _qtActFileQuit );
		
		_qtMenuItemEdit = menuBar()->addMenu( tr( "&Edit" ) );
		_qtMenuItemEdit->addAction( _qtActEditClear );
		_qtMenuItemEdit->addSeparator();
		_qtMenuItemEdit->addAction( _qtActEditFilters );

		_qtMenuItemTools = menuBar()->addMenu( tr( "&Tools" ) );
		_qtMenuItemTools->addAction( _qtActToolsOptions );

		_qtMenuItemHelp = menuBar()->addMenu( tr( "&Help" ) );
		_qtMenuItemHelp->addAction( _qtActHelpAbout );
	}

	void MainWindow::_ShAbout()
	{
		_qtLabelInfo->setText( tr( "Invoked <b>Help|About</b>" ) );
		QMessageBox::about( this, tr( "About Menu" ),
			tr( "The <b>Menu</b> example shows how to create "
				"menu-bar menus and context menus." ) );
	}

}
