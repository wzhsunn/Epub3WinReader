
// EPUB3Reader.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "EPUB3Reader.h"
#include "EPUB3ReaderDlg.h"
#include "MyCefApp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CEPUB3ReaderApp

BEGIN_MESSAGE_MAP(CEPUB3ReaderApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CEPUB3ReaderApp construction

CEPUB3ReaderApp::CEPUB3ReaderApp()
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CEPUB3ReaderApp object

CEPUB3ReaderApp theApp;


// CEPUB3ReaderApp initialization

BOOL CEPUB3ReaderApp::InitInstance()
{
	{
		CefMainArgs main_args(::GetModuleHandle(NULL));
		CefRefPtr<MyCefApp> app(new MyCefApp);

		// Execute the secondary process, if any.
		int exit_code = CefExecuteProcess(main_args, app.get(), nullptr);
		if (exit_code >= 0)
			return exit_code;
		CefSettings							settings;
		CefSettingsTraits::init(&settings);
		settings.command_line_args_disabled = false;
		settings.single_process = false;
		//中文支持
		// http://blog.sina.com.cn/s/blog_dad2c54101019dav.html
		CefString(&settings.locale).FromASCII("zh-cn");

		//settings.multi_threaded_message_loop = CommandLine->HasSwitch("multi-threaded-message-loop");
		settings.multi_threaded_message_loop = true;
		
		// Initialize CEF.
		CefInitialize(main_args, settings, app.get(), nullptr);

		// Init plugins, like Flash etc.
		//InitWebPlugins();
	}
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// Create the shell manager, in case the dialog contains
	// any shell tree view or shell list view controls.
	CShellManager *pShellManager = new CShellManager;

	// Activate "Windows Native" visual manager for enabling themes in MFC controls
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	httpServer.startMsHTTPServerThread();

	CEPUB3ReaderDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "Warning: dialog creation failed, so application is terminating unexpectedly.\n");
		TRACE(traceAppMsg, 0, "Warning: if you are using MFC controls on the dialog, you cannot #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS.\n");
	}

	// Delete the shell manager created above.
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	{
		// Run the CEF message loop. This function will block until the application
		// recieves a WM_QUIT message.
		//CefRunMessageLoop();

		CefShutdown();
	}

	SDKInitializeAndRelease::Instance().Teardown();
	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

