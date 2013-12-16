#pragma once
#include "afxwin.h"


// CGraphOptionDlg dialog

class CGraphOptionDlg : public CDialog
{
	DECLARE_DYNAMIC(CGraphOptionDlg)

public:
	CGraphOptionDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGraphOptionDlg();

// Dialog Data
	enum { IDD = IDD_GRAPH_OPTIONS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_cmbSamplingPeriod;
	CComboBox m_cmbSamplingDuration;
	CButton m_chkQ[JDOF];
	CButton m_chkQdot[JDOF];
	CButton m_chkTau[JDOF];
	CButton m_chkErrCount;
	int m_nSamplingPeriod;
	int m_nSamplingDuration;
	BOOL m_bQ[JDOF];
	BOOL m_bQdot[JDOF];
	BOOL m_bTau[JDOF];
	BOOL m_bErrCount;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
