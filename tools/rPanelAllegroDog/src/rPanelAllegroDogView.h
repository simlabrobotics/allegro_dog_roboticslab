
// rPanelAllegroDogView.h : interface of the CrPanelAllegroDogView class
//


#pragma once

#include "ControlPanelDlg.h"
#include "JointParamDlg.h"
#include "TrajectoryDlg.h"
#include "UnitTestDlg.h"

class CrPanelAllegroDogView : public CView
{
protected: // create from serialization only
	CrPanelAllegroDogView();
	DECLARE_DYNCREATE(CrPanelAllegroDogView)

// Attributes
public:
	CrPanelAllegroDogDoc* GetDocument() const;

	CMFCTabCtrl	m_wndTabs;
	CControlPanelDlg m_dlgControlPanel;
	CJointParamDlg m_dlgJointParam;
	CTrajectoryDlg m_dlgTrajectory;
	CUnitTestDlg m_dlgUnitTest;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CrPanelAllegroDogView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
protected:
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
};

#ifndef _DEBUG  // debug version in rPanelAllegroDogView.cpp
inline CrPanelAllegroDogDoc* CrPanelAllegroDogView::GetDocument() const
   { return reinterpret_cast<CrPanelAllegroDogDoc*>(m_pDocument); }
#endif
