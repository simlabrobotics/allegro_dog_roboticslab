// ControlPanelDlg.cpp : implementation file
//

#include "stdafx.h"
#include "rPanelAllegroDog.h"
#include "rPanelAllegroDogDoc.h"
#include "rPanelAllegroDogLog.h"
#include "ControlPanelDlg.h"


// CControlPanelDlg dialog

IMPLEMENT_DYNAMIC(CControlPanelDlg, CScrollDlg)

CControlPanelDlg::CControlPanelDlg(CWnd* pParent /*=NULL*/)
	: CScrollDlg(CControlPanelDlg::IDD, pParent)
	, _data(NULL)
	, _dataUpdateTimerID(0)
{

}

CControlPanelDlg::~CControlPanelDlg()
{
}

void CControlPanelDlg::DoDataExchange(CDataExchange* pDX)
{
	CScrollDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CUR_TIME, _editCurTime);
	DDX_Control(pDX, IDC_MASTER_AL_STATUS, _editMasterALStatus);
	DDX_Control(pDX, IDC_MASTER_AP_STATUS, _editMasterAPStatus);
	DDX_Control(pDX, IDC_SLAVE_COUNT, _editSlaveCount);
	DDX_Control(pDX, IDC_ERR_COUNT, _editErrCount);
	DDX_Control(pDX, IDC_ERR_COUNT_CONTINUOUS, _editErrCountCont);
	DDX_Control(pDX, IDC_FRAME_RATE_RECV, _editFrameRateRecv);
	DDX_Control(pDX, IDC_FRAME_RATE_SEND, _editFrameRateSend);
	DDX_Control(pDX, IDC_FRAMES_RECV, _editFramesRecv);
	DDX_Control(pDX, IDC_FRAMES_SEND, _editFramesSend);
}


BEGIN_MESSAGE_MAP(CControlPanelDlg, CScrollDlg)
	ON_BN_CLICKED(IDC_CMD_ESTOP, &CControlPanelDlg::OnBnClickedCmdEStop)
	ON_BN_CLICKED(IDC_CMD_HOME, &CControlPanelDlg::OnBnClickedCmdHome)
	ON_BN_CLICKED(IDC_ENABLE, &CControlPanelDlg::OnBnClickedEnable)
	ON_BN_CLICKED(IDC_CMD_READY, &CControlPanelDlg::OnBnClickedCmdReady)
	ON_BN_CLICKED(IDC_CMD_HOP, &CControlPanelDlg::OnBnClickedCmdHop)
	ON_BN_CLICKED(IDC_CMD_WALK, &CControlPanelDlg::OnBnClickedCmdWalk)
	ON_BN_CLICKED(IDC_CMD_STOP, &CControlPanelDlg::OnBnClickedCmdStop)
	ON_BN_CLICKED(IDC_CMD_TURNLEFT, &CControlPanelDlg::OnBnClickedCmdTurnleft)
	ON_BN_CLICKED(IDC_CMD_TURNRIGHT, &CControlPanelDlg::OnBnClickedCmdTurnright)
	ON_BN_CLICKED(IDOK, &CControlPanelDlg::OnBnClickedOk)
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_CMD_WALKBACKWARD, &CControlPanelDlg::OnBnClickedCmdWalkbackward)
	ON_BN_CLICKED(IDC_CMD_BASEUP, &CControlPanelDlg::OnBnClickedCmdBaseup)
	ON_BN_CLICKED(IDC_CMD_BASEDOWN, &CControlPanelDlg::OnBnClickedCmdBasedown)
END_MESSAGE_MAP()


// CControlPanelDlg message handlers

BOOL CControlPanelDlg::OnInitDialog()
{
	CScrollDlg::OnInitDialog();

	_data = theApp.GetStateData();
	_editCurTime.SetWindowText(_T("0.0"));
	_editMasterALStatus.SetWindowText(_T(""));
	_editMasterAPStatus.SetWindowText(_T(""));
	_editSlaveCount.SetWindowText(_T("0"));
	_editErrCount.SetWindowText(_T("0"));
	_editErrCountCont.SetWindowText(_T("0"));
	_editFrameRateSend.SetWindowText(_T("0"));
	_editFrameRateRecv.SetWindowText(_T("0"));
	_editFramesSend.SetWindowText(_T("0"));
	_editFramesRecv.SetWindowText(_T("0"));
	_dataUpdateTimerID = _TIMER_ID_DATA_UPDATE_CONTROL_PANEL;
	SetTimer(_dataUpdateTimerID, _TIMER_PERIOD_DATA_UPDATE_CONTROL_PANEL, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CControlPanelDlg::OnBnClickedCmdEStop()
{
	WriteLog(eLOG_TARGET_OUTPUT, eLOG_INFO, false, false, _T("Emergency STOP"));
	_data->cmd.command = CMD_DISABLE;
}

void CControlPanelDlg::OnBnClickedCmdHome()
{
	WriteLog(eLOG_TARGET_OUTPUT, eLOG_INFO, false, false, _T("HOME"));
	_data->cmd.command = CMD_CMD_1;
}

void CControlPanelDlg::OnBnClickedEnable()
{
	WriteLog(eLOG_TARGET_OUTPUT, eLOG_INFO, false, false, _T("ENABLE"));
	_data->cmd.command = CMD_ENABLE;
}

void CControlPanelDlg::OnBnClickedCmdReady()
{
	WriteLog(eLOG_TARGET_OUTPUT, eLOG_INFO, false, false, _T("READY"));
	_data->cmd.command = CMD_CMD_2;
}

void CControlPanelDlg::OnBnClickedCmdHop()
{
	WriteLog(eLOG_TARGET_OUTPUT, eLOG_INFO, false, false, _T("HOP"));
	_data->cmd.command = CMD_CMD_3;
}

void CControlPanelDlg::OnBnClickedCmdWalk()
{
	WriteLog(eLOG_TARGET_OUTPUT, eLOG_INFO, false, false, _T("WALK"));
	_data->cmd.command = CMD_CMD_4;
}

void CControlPanelDlg::OnBnClickedCmdWalkbackward()
{
	WriteLog(eLOG_TARGET_OUTPUT, eLOG_INFO, false, false, _T("WALK BACKWARD"));
	_data->cmd.command = CMD_CMD_5;
}

void CControlPanelDlg::OnBnClickedCmdStop()
{
	WriteLog(eLOG_TARGET_OUTPUT, eLOG_INFO, false, false, _T("STOP"));
	_data->cmd.command = CMD_CMD_6;
}

void CControlPanelDlg::OnBnClickedCmdTurnleft()
{
	WriteLog(eLOG_TARGET_OUTPUT, eLOG_INFO, false, false, _T("TURN LEFT"));
	_data->cmd.command = CMD_CMD_7;
}

void CControlPanelDlg::OnBnClickedCmdTurnright()
{
	WriteLog(eLOG_TARGET_OUTPUT, eLOG_INFO, false, false, _T("TURN RIGHT"));
	_data->cmd.command = CMD_CMD_8;
}

void CControlPanelDlg::OnBnClickedCmdBaseup()
{
	WriteLog(eLOG_TARGET_OUTPUT, eLOG_INFO, false, false, _T("BASE UP"));
	_data->cmd.command = CMD_CMD_9;
}

void CControlPanelDlg::OnBnClickedCmdBasedown()
{
	WriteLog(eLOG_TARGET_OUTPUT, eLOG_INFO, false, false, _T("BASE DOWN"));
	_data->cmd.command = CMD_CMD_10;
}

void CControlPanelDlg::OnSize(UINT nType, int cx, int cy)
{
	CScrollDlg::OnSize(nType, cx, cy);

	
	if (NULL != GetDlgItem(IDC_STATIC_COPYRIGHT)->GetSafeHwnd())
	{
		GetDlgItem(IDC_STATIC_COPYRIGHT)->SetWindowPos(NULL, 0, 0, max(80, cx-20), 20, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
	}
}

void CControlPanelDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case _TIMER_ID_DATA_UPDATE_CONTROL_PANEL:
		{
			CString strTime;
			strTime.Format(_T("%.4f"), _data->state.time);
			_editCurTime.SetWindowText(strTime);
		}
		break;
	}

	CScrollDlg::OnTimer(nIDEvent);
}

void CControlPanelDlg::OnDestroy()
{
	CScrollDlg::OnDestroy();

	KillTimer(_dataUpdateTimerID);
	_dataUpdateTimerID = 0;
}

void CControlPanelDlg::OnBnClickedOk()
{
	//OnOK();
}

