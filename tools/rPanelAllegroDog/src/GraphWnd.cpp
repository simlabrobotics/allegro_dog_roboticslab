
#include "stdafx.h"
#include "GraphWnd.h"
#include "Resource.h"
#include "MainFrm.h"
#include "rPanelAllegroDog.h"
#include "rPanelAllegroDogDoc.h"
#include "FileDialogEx.h"
#include "GraphOptionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

enum rPlotDataLineType
{
	rPlotDataLineType_Solid		= 0x00,	///< "Solid" line
	rPlotDataLineType_Dash,				///< "Dash"
	rPlotDataLineType_Dot,				///< "Dot" line
	rPlotDataLineType_DotDash,			///< "Dash-dot" line
	rPlotDataLineType_AltDash,			///< "Dash-short dash" line
};

rPlotDataLineType getElementLinetype(long type)
{
	switch (type)
	{
	case 0: return rPlotDataLineType_Solid;
	case 1: return rPlotDataLineType_Dash;
	case 2: return rPlotDataLineType_Dot;
	case 3: return rPlotDataLineType_DotDash;
	case 4: return rPlotDataLineType_AltDash;
	case 5: return rPlotDataLineType_Solid;
	case 6: return rPlotDataLineType_Solid;
	case 7: return rPlotDataLineType_Solid;
	case 8: return rPlotDataLineType_Solid;
	case 9: return rPlotDataLineType_Solid;
	default: return rPlotDataLineType_Solid;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CGraphChildWnd

IMPLEMENT_DYNAMIC(CGraphChildWnd, CDialog)

DWORD WINAPI CGraphChildWnd::DataUpdateThread(LPVOID lpParam)
{
	CGraphChildWnd* pWnd = (CGraphChildWnd*)lpParam;
	if (!pWnd) return -1;

	while (pWnd->_dataUpdateThreadRun)
	{
		if (WAIT_OBJECT_0 == waitrPanelManipulatorCmdUpdate(500/*INFINITE*/) &&
			pWnd->_dataUpdateThreadRun)
			pWnd->AddData();
		else
			Sleep(10);
	}
	return 0;
}

VOID WINAPI CGraphChildWnd::DataUpdateTimerCallback(PVOID lpParam, BOOLEAN)
{
	CGraphChildWnd* pWnd = (CGraphChildWnd*)lpParam;
	if (pWnd && pWnd->_dataUpdateThreadRun)
		pWnd->AddData();
}

CGraphChildWnd::CGraphChildWnd(CWnd* pParent /*=NULL*/)
: CDialog(CGraphChildWnd::IDD, pParent)
, _doc(NULL)
, _data(NULL)
, _dataQuitTimerID(0)
, _dataUpdateThread(NULL)
, _dataUpdateThreadID(-1)
, _dataUpdateThreadRun(FALSE)
, _dataUpdateTimerCallback(NULL)
, _plotServer(NULL)
, _plotEnable(false)
{
	_plotEnable = theAppParam.bRPlotEnable;
	if (_plotEnable)
		_plotServer = new CrPlotServer(theAppParam.nRPlotPort, theAppParam.bRPlotAutoStart);
}

CGraphChildWnd::~CGraphChildWnd()
{
	if (_plotServer)
		delete _plotServer;
}

BEGIN_MESSAGE_MAP(CGraphChildWnd, CDialog)
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDOK, &CGraphChildWnd::OnBnClickedOk)
	ON_COMMAND(ID_GRAPH_START, &CGraphChildWnd::OnGraphStart)
	ON_COMMAND(ID_GRAPH_STOP, &CGraphChildWnd::OnGraphStop)
	ON_COMMAND(ID_GRAPH_CLEAR, &CGraphChildWnd::OnGraphClear)
	ON_COMMAND(ID_GRAPH_SAVEAS, &CGraphChildWnd::OnGraphSaveas)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_GRAPH_AUTORANGE, &CGraphChildWnd::OnGraphAutorange)
	ON_WM_DESTROY()
	ON_COMMAND(ID_GRAPH_AUTOSCROLL, &CGraphChildWnd::OnGraphAutoscroll)
	ON_UPDATE_COMMAND_UI(ID_GRAPH_AUTOSCROLL, &CGraphChildWnd::OnUpdateGraphAutoscroll)
	ON_UPDATE_COMMAND_UI(ID_GRAPH_AUTORANGE, &CGraphChildWnd::OnUpdateGraphAutorange)
	ON_COMMAND(ID_GRAPH_OPTIONS, &CGraphChildWnd::OnGraphOptions)
END_MESSAGE_MAP()


void CGraphChildWnd::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDC_GRAPHCTRL, _graph);

	CDialog::DoDataExchange(pDX);
}

void CGraphChildWnd::InitGraph()
{
	_graphProp.FrameStyle = 1;
	_graphProp.PlotAreaColor = BLACK;
	_graphProp.YGridNumber = 10;
	_graphProp.Xmin = theAppParam.dblGraphXmin;
	_graphProp.Xmax = theAppParam.dblGraphXmax;
	_graphProp.Ymin = theAppParam.dblGraphYmin;
	_graphProp.Ymax = theAppParam.dblGraphYmax;
	_graphProp.ElementWidth = 1;
	_graphProp.LineColor = RED;
	_graphProp.LineType = 0;
	_graphProp.AutoScroll = TRUE;
	_graphProp.AutoRange = FALSE; // when it is enable, the CPU usage is getting higher.
	_graphProp.FlagReset = FALSE;

	_graph.SetFrameStyle(_graphProp.FrameStyle);
	_graph.SetPlotAreaColor(_graphProp.PlotAreaColor);
	_graph.SetShowGrid(TRUE);
	_graph.SetFormatAxisBottom(_T("%.1f"));
	_graph.SetYGridNumber(_graphProp.YGridNumber);
	_graph.SetRange (_graphProp.Xmin, _graphProp.Xmax, _graphProp.Ymin, _graphProp.Ymax);
	_graph.SetXLabel(_T("Time(s)"));
	_graph.SetYLabel(_T("Angle(radian)"));
	_graph.SetCaption(_T("Joint Angle"));
	//_graph.AddCursor();
	_graph.SetTrackMode(4);
}

void CGraphChildWnd::ResetGraph()
{
	_graph.ClearGraph();

	// 1st element
	TCHAR element_name[256];

	for (int i=0; i<JDOF; i++)
	{
		if (GetDocument()->_graphQ[i])
		{
			_stprintf_s(element_name, 256, _T("q[%d]"), i);
			_graph.AddElement();
			_graph.SetElementWidth(_graphProp.ElementWidth);
			_graph.SetElementLinetype(_graphProp.LineType);
			_graph.SetElementLineColor(GraphLineColorArr[i]);
			_graph.SetElementName(element_name);
		}
	}

	for (int i=0; i<JDOF; i++)
	{
		if (GetDocument()->_graphTau[i])
		{
			_stprintf_s(element_name, 256, _T("current[%d]"), i);
			_graph.AddElement();
			_graph.SetElementWidth(_graphProp.ElementWidth);
			_graph.SetElementLinetype(_graphProp.LineType);
			_graph.SetElementLineColor(GraphLineColorArr[i]);
			_graph.SetElementName(element_name);
		}
	}

	_lastDataGetTime = -1.0;
	_dataCount = 0;
}

void CGraphChildWnd::StartGraph()
{
	if (_dataUpdateThreadRun)
		return; 

	_dataUpdateThreadRun = TRUE;

	if (theAppParam.nGraphUpdateMethod == 1)
	{
		// event
		_dataUpdateThread = CreateThread(NULL, 0, DataUpdateThread, this, CREATE_SUSPENDED, &_dataUpdateThreadID);
		SetThreadPriority(_dataUpdateThread, THREAD_PRIORITY_HIGHEST);
		ResumeThread(_dataUpdateThread);
	}
	else
	{
		// timer
		CreateTimerQueueTimer(
			&_dataUpdateTimerCallback, NULL, 
			DataUpdateTimerCallback, 
			(PVOID)this, 
			GetDocument()->_graphSamplingPeriod,
			GetDocument()->_graphSamplingPeriod,
			WT_EXECUTEDEFAULT);
	}
}

void CGraphChildWnd::StopGraph()
{
	if (_dataUpdateThreadRun)
	{
		// IMPORTANT: Set _dataUpdateThreadRun as FALSE earlier than calling rPanelManipulatorCmdUpdate()
		//            It's because AddData() should not be called by calling rPanelManipulatorCmdUpdate().
		//            See DataUpdateThread() function for further understanding.
		_dataUpdateThreadRun = FALSE;

		if (theAppParam.nGraphUpdateMethod == 1)
		{
			setrPanelManipulatorCmdUpdate();
			WaitForSingleObject((HANDLE)_dataUpdateThread, INFINITE);
			CloseHandle((HANDLE)_dataUpdateThread);
			_dataUpdateThread = NULL;
		}
		else
			DeleteTimerQueueTimer(NULL, _dataUpdateTimerCallback, NULL);
	}
}

void CGraphChildWnd::AdjustRange()
{
	if (_data && _graphProp.AutoScroll == TRUE)
	{
		if (_graphProp.AutoRange == TRUE)
			_graph.AutoRange();
		else
			_graph.SetRange(_data->state.time-_graphProp.Xmax, _data->state.time, _graphProp.Ymin, _graphProp.Ymax);
	}
}

BOOL CGraphChildWnd::AddData()
{
	if (!_data || (_data->state.time <= _lastDataGetTime+1.0e-7))
		return FALSE;

	_lastDataGetTime = _data->state.time;

	double value;
	int elem_id = 0;

	for (int i=0; i<JDOF; i++)
	{
		if (GetDocument()->_graphQ[i])
		{
			elem_id++;
			//value = RTD(_data->state.slave_state[i].position);
			value = _data->state.slave_state[i].position;
			_graph.PlotXY(_data->state.time, value, elem_id);
			
			if (value >= _graphProp.Ymax)
				_graphProp.Ymax = value + 1.0;
			else if (value <= _graphProp.Ymin)
				_graphProp.Ymin = value - 1.0;
		}
	}

	for (int i=0; i<JDOF; i++)
	{
		if (GetDocument()->_graphTau[i])
		{
			elem_id++;
			value = _data->state.slave_state[i].torque;
			_graph.PlotXY(_data->state.time, value, elem_id);
			
			if (value >= _graphProp.Ymax)
				_graphProp.Ymax = value + 1.0;
			else if (value <= _graphProp.Ymin)
				_graphProp.Ymin = value - 1.0;
		}
	}

	_dataCount++;
	AdjustRange();

	if (_plotEnable && _plotServer)
	{
		double data[1] = {
			/*_data->state.slave_state[0].position,
			_data->state.slave_state[1].position,*/
			_data->state.slave_state[2].position
		};
		_plotServer->ntyPlotData(1, _lastDataGetTime, 1, data);
	}

	return TRUE;
}

BOOL CGraphChildWnd::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	_data = theApp.GetStateData();
	
	InitGraph();

	//SetTimer(1, 10, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CGraphChildWnd::OnDestroy()
{
	StopGraph();
	CDialog::OnDestroy();
}

void CGraphChildWnd::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	
	if (NULL != _graph.GetSafeHwnd())
		_graph.SetWindowPos (NULL, 0, 0, cx-20, cy-20, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
}

void CGraphChildWnd::OnTimer(UINT_PTR nIDEvent)
{
	if (_TIMER_ID_DATA_QUIT_GRAPH_WND == nIDEvent)
	{
		StopGraph();
		KillTimer(_dataQuitTimerID);
		_dataQuitTimerID = 0;
	}

	CDialog::OnTimer(nIDEvent);
}

void CGraphChildWnd::OnGraphStart()
{
	ResetGraph();
	StartGraph();
}

void CGraphChildWnd::OnGraphStop()
{
	StopGraph();
}

void CGraphChildWnd::OnGraphClear()
{
	if (_dataUpdateThreadRun)
		return;

	//InitGraph();
	ResetGraph();
}

void CGraphChildWnd::OnGraphSaveas()
{
	CFileDialogEx fileDlg(
		FALSE,
		_T("bmp"),
		_T("graph"),
		OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY | OFN_EXPLORER,
		/*_T("Bitmap file(*.bmp) | *.bmp||"),*/
		_T("Bitmap file(*.bmp)|*.bmp|Text file(*.txt)|*.txt|rPlot file(*.rplt)|*.rplt||"),
		NULL);
	if (IDOK != fileDlg.DoModal())
		return;

	// backup current directory and set the output path as the current directory
	TCHAR buf_path[MAX_PATH+1];
	GetCurrentDirectory(MAX_PATH, buf_path);
	SetCurrentDirectory(fileDlg.m_strFolderName);

	CString outputPath = fileDlg.GetPathName();
	CString outputFile = fileDlg.GetFileName();

#ifdef _UNICODE
	size_t converted;
	char filename_mbs[MAX_PATH];
	wcstombs_s(&converted, filename_mbs, MAX_PATH, outputFile, MAX_PATH);
#endif

	if (0 == fileDlg.GetFileExt().CompareNoCase(_T("bmp")))
		_graph.SaveAs((LPCTSTR)outputPath);
	else if (0 == fileDlg.GetFileExt().CompareNoCase(_T("txt")))
		SaveAsTextFile((LPCTSTR)outputPath);
	else if (0 == fileDlg.GetFileExt().CompareNoCase(_T("rplt")))
		SaveAsRpltFile((LPCTSTR)outputPath);
	
	// restore current directory
	SetCurrentDirectory(buf_path);
}

BOOL CGraphChildWnd::SaveAsTextFile(LPCTSTR lpszPathName)
{
	FILE* file;
	errno_t error;

	error = _tfopen_s(&file, lpszPathName, _T("w"));
	_ftprintf(file, _T("%% time"));

	short elem_count = _graph.GetElementCount();
	short elem_id = 0;

	for (elem_id=1; elem_id<elem_count; elem_id++)
	{
		_graph.SetElement(elem_id);
		CString elem_name = _graph.GetElementName();
		_ftprintf(file, _T("\t%s"), (LPCTSTR)elem_name);
	}
	_ftprintf(file, _T("\n"));
	
	int data_index;
	double elem_xval;
	double elem_yval;
	for (data_index=0; data_index<_dataCount; data_index++)
	{
		elem_xval = _graph.GetElementXValue(data_index, 1);
		_ftprintf(file, _T("%g\t"), elem_xval);
		for (elem_id=1; elem_id<elem_count; elem_id++)
		{
			elem_yval = _graph.GetElementYValue(data_index, elem_id);
			_ftprintf(file, _T("%g\t"), elem_yval);
		}
		_ftprintf(file, _T("\n"));
	}

	fclose(file);
	return TRUE;
}

BOOL CGraphChildWnd::SaveAsRpltFile(LPCTSTR lpszPathName)
{
	CFile file;
	file.Open(lpszPathName, CFile::modeCreate | CFile::modeWrite);
	CArchive ar(&file, CArchive::store);

	// document head
	ar.WriteString(_bstr_t("TIMELINECHART"));
	ar.WriteString(_T("\r\n"));
	ar.WriteString(_bstr_t("real-time chart"));
	ar.WriteString(_T("\r\n"));

	short elem_count = _graph.GetElementCount();
	short elem_id = 0;
	int data_index;
	double elem_xval;
	double elem_yval;
	CString elem_name;
	COLORREF elem_line_color;
	long elem_line_type;

	// number of elements
	ar << (int)(elem_count-1);

	// number of data
	ar << (int)_dataCount;

	// line color
	for (elem_id=1; elem_id<elem_count; elem_id++)
	{
		_graph.SetElement(elem_id);
		elem_line_color = _graph.GetElementLineColor();
		elem_line_color = RGB(GetBValue(elem_line_color), GetGValue(elem_line_color), GetRValue(elem_line_color)); 
		ar << elem_line_color;
	}

	// line type
	for (elem_id=1; elem_id<elem_count; elem_id++)
	{
		_graph.SetElement(elem_id);
		elem_line_type = _graph.GetElementLinetype();
		ar << elem_line_type;
	}

	// element name
	for (elem_id=1; elem_id<elem_count; elem_id++)
	{
		_graph.SetElement(elem_id);
		elem_name = _graph.GetElementName();
		ar.WriteString(_bstr_t((LPCTSTR)elem_name));
		ar.WriteString(_T("\r\n"));
	}

	// data
	for (elem_id=1; elem_id<elem_count; elem_id++)
	{
		for (data_index=0; data_index<_dataCount; data_index++)
		{
			elem_yval = _graph.GetElementYValue(data_index, elem_id);
			ar << elem_yval;
		}
	}

	// time element
	for (data_index=0; data_index<_dataCount; data_index++)
	{
		elem_xval = _graph.GetElementXValue(data_index, 1);
		ar << elem_xval;
	}
	
	//file.Close();

	return TRUE;
}

CrPanelAllegroDogDoc* CGraphChildWnd::GetDocument()
{
	if (!_doc)
		_doc = (CrPanelAllegroDogDoc*)((CMainFrame*)theApp.GetMainWnd())->GetActiveDocument();
	return _doc;
}

void CGraphChildWnd::OnGraphAutorange()
{
	_graphProp.AutoRange = (_graphProp.AutoRange == TRUE ? FALSE : TRUE);
	AdjustRange();
}

void CGraphChildWnd::OnGraphAutoscroll()
{
	_graphProp.AutoScroll = (_graphProp.AutoScroll == TRUE ? FALSE : TRUE);
}

void CGraphChildWnd::OnBnClickedOk()
{
	//OnOK();
}

void CGraphChildWnd::OnGraphOptions()
{
	CGraphOptionDlg dlg;
	CrPanelAllegroDogDoc* doc = GetDocument();
	
	dlg.m_nSamplingDuration = doc->_graphSamplingDuration;
	dlg.m_nSamplingPeriod = doc->_graphSamplingPeriod;
	for (int i=0; i<JDOF; i++)
	{
		dlg.m_bQ[i] = doc->_graphQ[i];
		dlg.m_bQdot[i] = doc->_graphQdot[i];
		dlg.m_bTau[i] = doc->_graphTau[i];
	}
	dlg.m_bErrCount = doc->_graphErrCount;

	if (IDOK == dlg.DoModal())
	{
		doc->_graphSamplingDuration = dlg.m_nSamplingDuration;
		doc->_graphSamplingPeriod = dlg.m_nSamplingPeriod;
		for (int i=0; i<JDOF; i++)
		{
			doc->_graphQ[i] = dlg.m_bQ[i];
			doc->_graphQdot[i] = dlg.m_bQdot[i];
			doc->_graphTau[i] = dlg.m_bTau[i];
		}
		doc->_graphErrCount = dlg.m_bErrCount;
	}
}

void CGraphChildWnd::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CMenu menu;
	menu.LoadMenu(IDR_GRAPH_MENU);
	CMenu *menu0 = menu.GetSubMenu(0);
	menu0->EnableMenuItem(ID_GRAPH_START, (_dataUpdateThreadRun == TRUE ? MF_GRAYED : MF_ENABLED));
	menu0->EnableMenuItem(ID_GRAPH_CLEAR, (_dataUpdateThreadRun == TRUE ? MF_GRAYED : MF_ENABLED));
	menu0->EnableMenuItem(ID_GRAPH_SAVEAS, (_dataUpdateThreadRun == TRUE ? MF_GRAYED : MF_ENABLED));
	menu0->EnableMenuItem(ID_GRAPH_OPTIONS, (_dataUpdateThreadRun == TRUE ? MF_GRAYED : MF_ENABLED));
	menu0->EnableMenuItem(ID_GRAPH_STOP, (_dataUpdateThreadRun == TRUE ? MF_ENABLED : MF_GRAYED));
	menu0->CheckMenuItem(ID_GRAPH_AUTOSCROLL, (_graphProp.AutoScroll ? MF_CHECKED : MF_UNCHECKED));
	menu0->CheckMenuItem(ID_GRAPH_AUTORANGE, (_graphProp.AutoRange ? MF_CHECKED : MF_UNCHECKED));
	menu0->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	menu0->DestroyMenu();
}

void CGraphChildWnd::OnUpdateGraphAutoscroll(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(_graphProp.AutoScroll);
}

void CGraphChildWnd::OnUpdateGraphAutorange(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(_graphProp.AutoRange);
}

/////////////////////////////////////////////////////////////////////////////
// CGraphWnd

CGraphWnd::CGraphWnd()
{
}

CGraphWnd::~CGraphWnd()
{
}

BEGIN_MESSAGE_MAP(CGraphWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

int CGraphWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	const DWORD dwStyle = WS_CHILD | WS_VISIBLE;

	// Create graph window
	if (!m_wndGraph.Create(IDD_GRAPH, this))
	{
		TRACE0("Failed to create graph window\n");
		return -1;
	}
	
	return 0;
}

void CGraphWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	
	m_wndGraph.SetWindowPos (NULL, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndGraph.ShowWindow(TRUE);
}
