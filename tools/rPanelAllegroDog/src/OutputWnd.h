
#pragma once

#include "rPanelAllegroDogLog.h"

/////////////////////////////////////////////////////////////////////////////
// COutputList window

class COutputList : public CListBox
{
// Construction
public:
	COutputList();

// Implementation
public:
	virtual ~COutputList();

protected:
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnEditCopy();
	afx_msg void OnEditClear();
	afx_msg void OnViewOutput();

	DECLARE_MESSAGE_MAP()
};

class COutputWnd : public CDockablePane
{
// Construction
public:
	COutputWnd();

// Attributes
protected:
	CFont m_Font;

	CMFCTabCtrl	m_wndTabs;

	COutputList m_wndOutputGeneral;
	COutputList m_wndOutputDebug;
	COutputList m_wndOutputFind;

protected:
	void AdjustHorzScroll(CListBox& wndListBox);

// Implementation
public:
	virtual ~COutputWnd();

	void WriteLog(eLogTarget etarget, eLogType etype, bool bringFrontWnd, bool mb, LPCTSTR lpszLog);
	void ClearLog(eLogTarget etarget);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
};

