
// rPanelAllegroDog.h : main header file for the rPanelAllegroDog application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols

#include "rPanelManipulatorCmdUtil.h"

// CrPanelAllegroDogApp:
// See rPanelAllegroDog.cpp for the implementation of this class
//

class CrPanelAllegroDogApp : public CWinAppEx
{
public:
	CrPanelAllegroDogApp();

	rPanelManipulatorData_t* GetStateData() { return _stateData; }

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	void LoadConstants();

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

	rPanelManipulatorData_t* _stateData;
};

extern CrPanelAllegroDogApp theApp;
extern CrPanelAllegroDogAppParam theAppParam;

#define _TIMER_ID_DATA_UPDATE_CONTROL_PANEL		10
#define _TIMER_PERIOD_DATA_UPDATE_CONTROL_PANEL	100
#define _TIMER_ID_DATA_UPDATE_PROPERTY_WND		11
#define _TIMER_PERIOD_DATA_UPDATE_PROPERTY_WND	1000
#define _TIMER_ID_DATA_QUIT_GRAPH_WND			12