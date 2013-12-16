#pragma once

#include "rPanelManipulatorDef.h"
#include "rPanelManipulatorCmdUtil.h"
#include "ScrollDlg.h"

// CControlPanelDlg dialog

class CControlPanelDlg : public CScrollDlg
{
	DECLARE_DYNAMIC(CControlPanelDlg)

public:
	CControlPanelDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CControlPanelDlg();

	CrPanelAllegroDogDoc* GetDocument() const;

// Dialog Data
	enum { IDD = IDD_CONTROLPANELDLG };

protected:
	rPanelManipulatorData_t*	_data;
	UINT_PTR					_dataUpdateTimerID;
	CEdit						_editCurTime;
	CEdit						_editMasterALStatus;
	CEdit						_editMasterAPStatus;
	CEdit						_editSlaveCount;
	CEdit						_editErrCount;
	CEdit						_editErrCountCont;
	CEdit						_editFrameRateSend;
	CEdit						_editFrameRateRecv;
	CEdit						_editFramesSend;
	CEdit						_editFramesRecv;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCmdEStop();
	afx_msg void OnBnClickedCmdHome();
	afx_msg void OnBnClickedEnable();
	afx_msg void OnBnClickedCmdReady();
	afx_msg void OnBnClickedCmdHop();
	afx_msg void OnBnClickedCmdWalk();
	afx_msg void OnBnClickedCmdStop();
	afx_msg void OnBnClickedCmdTurnleft();
	afx_msg void OnBnClickedCmdTurnright();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCmdWalkbackward();
	afx_msg void OnBnClickedCmdBaseup();
	afx_msg void OnBnClickedCmdBasedown();
};
