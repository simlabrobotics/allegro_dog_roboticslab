#pragma once


// CJointParamDlg dialog

#include "ScrollDlg.h"

class CJointParamDlg : public CScrollDlg
{
	DECLARE_DYNAMIC(CJointParamDlg)

public:
	CJointParamDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CJointParamDlg();

// Dialog Data
	enum { IDD = IDD_JOINTPARAMDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();

	CString m_strGainKp[16];
	CString m_strGainKv[16];
	CString m_strGainKi[16];
	CString m_strGainInvL2[16];
	CString m_strPosUpper[16];
	CString m_strPosLower[16];
	CString m_strVelUpper[16];
	CString m_strAccUpper[16];
};
