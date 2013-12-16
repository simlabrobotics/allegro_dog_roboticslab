
// rPanelAllegroDog.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "rPanelAllegroDog.h"
#include "MainFrm.h"

#include "rPanelAllegroDogDoc.h"
#include "rPanelAllegroDogView.h"

#include <fstream>
using namespace std;
#include "json/json.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CrPanelAllegroDogAppParam theAppParam;

// CrPanelAllegroDogApp

BEGIN_MESSAGE_MAP(CrPanelAllegroDogApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CrPanelAllegroDogApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
END_MESSAGE_MAP()


// CrPanelAllegroDogApp construction

CrPanelAllegroDogApp::CrPanelAllegroDogApp()
{

	m_bHiColorIcons = TRUE;

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance

	_stateData = getrPanelManipulatorCmdMemory();
}

// The one and only CrPanelAllegroDogApp object

CrPanelAllegroDogApp theApp;


// CrPanelAllegroDogApp initialization

BOOL CrPanelAllegroDogApp::InitInstance()
{
	LoadConstants();

	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("rPanelAllegroDogNRL"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)

	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CrPanelAllegroDogDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CrPanelAllegroDogView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand
	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();
	return TRUE;
}

void CrPanelAllegroDogApp::LoadConstants()
{
	Json::Value json_root;
	Json::Reader json_reader;
	ifstream json_config_file;
	json_config_file.open("rPanelAllegroDog.ini", ifstream::in);
	if (json_reader.parse(json_config_file, json_root))
	{
		theAppParam.strRtssProcessManagerPath = json_root.get("rtss_process_manager_path", "manager.exe").asString();
		theAppParam.strRTXPropertiesPath = json_root.get("rtx_properties_path", "RtxProperties.exe").asString();
		theAppParam.strRtssRunPath = json_root.get("rtss_run_path", "RTSSrun_GUI.exe").asString();
		theAppParam.nGraphUpdateMethod = json_root.get("graph_update_method" , 0).asInt();
		theAppParam.nGraphSamplingPeriod = json_root.get("graph_sampling_period" , 10).asInt();
		theAppParam.nGraphSamplingDuration = json_root.get("graph_sampling_duration" , 120000).asInt();
		theAppParam.dblGraphXmin = json_root.get("graph_x_min" , 0.0).asDouble();
		theAppParam.dblGraphXmax = json_root.get("graph_x_max" , 10.0).asDouble();
		theAppParam.dblGraphYmin = json_root.get("graph_y_min" , -1.0).asDouble();
		theAppParam.dblGraphYmax = json_root.get("graph_y_max" , 1.0).asDouble();
		theAppParam.bRPlotEnable = json_root.get("rplot_enable" , false).asBool();
		theAppParam.bRPlotAutoStart = json_root.get("rplot_auto_start" , false).asBool();
		theAppParam.nRPlotPort = json_root.get("rplot_port" , 5159).asUInt();
	}
	else
	{
		theAppParam.strRtssProcessManagerPath = "manager.exe";
		theAppParam.strRTXPropertiesPath = "RtxProperties.exe";
		theAppParam.strRtssRunPath = "RTSSrun_GUI.exe";
		theAppParam.nGraphUpdateMethod = 0;
		theAppParam.nGraphSamplingPeriod = 10;
		theAppParam.nGraphSamplingDuration = 120000;
		theAppParam.dblGraphXmin = 0.0;
		theAppParam.dblGraphXmax = 10.0;
		theAppParam.dblGraphYmin = -1.0;
		theAppParam.dblGraphYmax = 1.0;
		theAppParam.bRPlotEnable = false;
		theAppParam.bRPlotAutoStart = false;
		theAppParam.nRPlotPort = 5159;
	}
	json_config_file.close();


	/*Json::Value json_root;
	Json::Reader json_reader;
	ifstream json_config_file;
	json_config_file.open("conf.txt", ifstream::in);
	if (json_reader.parse(json_config_file, json_root))
	{
		theAppParam.d = json_root.get("d", 1.0).asDouble();
		Json::Value json_release_fraction_accident = json_root["release_fraction_accident"];
		for (int i=0; i<NUCLIDE_NUM; i++)
			theAppParam.release_fraction_accident[i] = json_release_fraction_accident[(Json::Value::UInt)i].asDouble();
		
		theAppParam.d_str = "d=" + json_root.get("d", 1.0).toStyledString();
		theAppParam.release_fraction_accident_str = "release_fraction_accident=" + json_root.get("release_fraction_accident", 1.0).toStyledString();
	}
	json_config_file.close();*/
}

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// App command to run the dialog
void CrPanelAllegroDogApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CrPanelAllegroDogApp customization load/save methods

void CrPanelAllegroDogApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void CrPanelAllegroDogApp::LoadCustomState()
{
}

void CrPanelAllegroDogApp::SaveCustomState()
{
}

// CrPanelAllegroDogApp message handlers



