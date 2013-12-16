#pragma once

#include "ScrollDlg.h"

#define MAX_SLAVE_COUNT_UI 16

// CUnitTestDlg dialog

class CUnitTestDlg : public CScrollDlg
{
	DECLARE_DYNAMIC(CUnitTestDlg)

public:
	CUnitTestDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CUnitTestDlg();

// Dialog Data
	enum { IDD = IDD_UNITTESTDLG };

	rPanelManipulatorData_t	*_pSHM;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	BOOL m_bEnableJoint[MAX_SLAVE_COUNT_UI];
	double m_dblCmdPosition[MAX_SLAVE_COUNT_UI];
	double m_dblCmdVelocity[MAX_SLAVE_COUNT_UI];
	double m_dblCmdCurrent[MAX_SLAVE_COUNT_UI];
	afx_msg void OnBnClickedBtnCmdPosition();
	afx_msg void OnBnClickedBtnCmdVelocity();
	afx_msg void OnBnClickedBtnCmdCurrent();
	afx_msg void OnBnClickedChkEnableJoint1();
	afx_msg void OnBnClickedChkEnableJoint2();
	afx_msg void OnBnClickedChkEnableJoint3();
	afx_msg void OnBnClickedChkEnableJoint4();
	afx_msg void OnBnClickedChkEnableJoint5();
	afx_msg void OnBnClickedChkEnableJoint6();
	afx_msg void OnBnClickedChkEnableJoint7();
	afx_msg void OnBnClickedChkEnableJoint8();
	afx_msg void OnBnClickedChkEnableJoint9();
	afx_msg void OnBnClickedChkEnableJoint10();
	afx_msg void OnBnClickedChkEnableJoint11();
	afx_msg void OnBnClickedChkEnableJoint12();
	afx_msg void OnBnClickedChkEnableJoint13();
	afx_msg void OnBnClickedChkEnableJoint14();
	afx_msg void OnBnClickedChkEnableJoint15();
	afx_msg void OnBnClickedChkEnableJoint16();
};
