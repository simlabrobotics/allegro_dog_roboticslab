#if !defined(AFX_SCROLLDLG_H__D46063D6_1480_11D3_8EEA_00104B30908A__INCLUDED_)
#define AFX_SCROLLDLG_H__D46063D6_1480_11D3_8EEA_00104B30908A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScrollDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScrollDlg dialog

class CScrollDlg : public CDialog
{
// Construction
public:
	CScrollDlg(UINT nIDD, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CScrollDlg)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScrollDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    void SetupScrollbars();
    void ResetScrollbars();
    int  m_nHorzInc, m_nVertInc,
         m_nVscrollMax, m_nHscrollMax,
         m_nVscrollPos, m_nHscrollPos;

    CRect m_ClientRect;
    BOOL m_bInitialized;
    
	// Generated message map functions
	//{{AFX_MSG(CScrollDlg)
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCROLLDLG_H__D46063D6_1480_11D3_8EEA_00104B30908A__INCLUDED_)
