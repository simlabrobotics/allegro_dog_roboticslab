
#pragma once

#include "resource.h"
#include "ntgraph.h"
#include "rPanelManipulatorDef.h"
#include "rPanelManipulatorCmdUtil.h"
#include "rPlotServer.h"

class CrPanelAllegroDogDoc;

class CGraphChildWnd : public CDialog
{
	DECLARE_DYNAMIC(CGraphChildWnd)

public:
	CGraphChildWnd(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGraphChildWnd();

// Dialog Data
	enum { IDD = IDD_GRAPH };

protected:
	CrPanelAllegroDogDoc*		_doc;
	rPanelManipulatorData_t*	_data;

	CNTGraph					_graph;
	NTGraphProperty				_graphProp;
	double						_lastDataGetTime;
	int							_dataCount;
	UINT_PTR					_dataQuitTimerID;

	HANDLE						_dataUpdateThread;
	DWORD						_dataUpdateThreadID;
	BOOL						_dataUpdateThreadRun;

	HANDLE						_dataUpdateTimerCallback;

	CrPlotServer*				_plotServer;
	bool						_plotEnable;

	static DWORD WINAPI DataUpdateThread(LPVOID lpParam);
	static VOID WINAPI DataUpdateTimerCallback(PVOID, BOOLEAN);

protected:
	void InitGraph();
	void ResetGraph();
	void StartGraph();
	void StopGraph();
	void AdjustRange();
	BOOL AddData();
	BOOL SaveAsTextFile(LPCTSTR lpszPathName);
	BOOL SaveAsRpltFile(LPCTSTR lpszPathName); // export data to rPlot
	CrPanelAllegroDogDoc* GetDocument();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnGraphStart();
	afx_msg void OnGraphStop();
	afx_msg void OnGraphClear();
	afx_msg void OnGraphSaveas();
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnGraphAutorange();
	afx_msg void OnDestroy();
	afx_msg void OnGraphAutoscroll();
	afx_msg void OnUpdateGraphAutoscroll(CCmdUI *pCmdUI);
	afx_msg void OnUpdateGraphAutorange(CCmdUI *pCmdUI);
	afx_msg void OnGraphOptions();
};


class CGraphWnd : public CDockablePane
{
// Construction
public:
	CGraphWnd();

// Attributes
protected:
	CGraphChildWnd m_wndGraph;

// Implementation
public:
	virtual ~CGraphWnd();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
};
