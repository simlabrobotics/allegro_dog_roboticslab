// UnitTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "rPanelManipulatorCmdUtil.h"
#include "rPanelManipulatorDef.h"
#include "rPanelAllegroDog.h"
#include "UnitTestDlg.h"

// CUnitTestDlg dialog

IMPLEMENT_DYNAMIC(CUnitTestDlg, CScrollDlg)

CUnitTestDlg::CUnitTestDlg(CWnd* pParent /*=NULL*/)
	: CScrollDlg(CUnitTestDlg::IDD, pParent)
{
	_pSHM = getrPanelManipulatorCmdMemory();
	memset(m_bEnableJoint, 0, sizeof(m_bEnableJoint[0])*MAX_SLAVE_COUNT_UI);
	memset(m_dblCmdPosition, 0, sizeof(m_dblCmdPosition[0])*MAX_SLAVE_COUNT_UI);
	memset(m_dblCmdVelocity, 0, sizeof(m_dblCmdVelocity[0])*MAX_SLAVE_COUNT_UI);
	memset(m_dblCmdCurrent, 0, sizeof(m_dblCmdCurrent[0])*MAX_SLAVE_COUNT_UI);
}

CUnitTestDlg::~CUnitTestDlg()
{
}

void CUnitTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CScrollDlg::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHK_ENABLE_JOINT1, m_bEnableJoint[0]);
	DDX_Check(pDX, IDC_CHK_ENABLE_JOINT2, m_bEnableJoint[1]);
	DDX_Check(pDX, IDC_CHK_ENABLE_JOINT3, m_bEnableJoint[2]);
	DDX_Check(pDX, IDC_CHK_ENABLE_JOINT4, m_bEnableJoint[3]);
	DDX_Check(pDX, IDC_CHK_ENABLE_JOINT5, m_bEnableJoint[4]);
	DDX_Check(pDX, IDC_CHK_ENABLE_JOINT6, m_bEnableJoint[5]);
	DDX_Check(pDX, IDC_CHK_ENABLE_JOINT7, m_bEnableJoint[6]);
	DDX_Check(pDX, IDC_CHK_ENABLE_JOINT8, m_bEnableJoint[7]);
	DDX_Check(pDX, IDC_CHK_ENABLE_JOINT9, m_bEnableJoint[8]);
	DDX_Check(pDX, IDC_CHK_ENABLE_JOINT10, m_bEnableJoint[9]);
	DDX_Check(pDX, IDC_CHK_ENABLE_JOINT11, m_bEnableJoint[10]);
	DDX_Check(pDX, IDC_CHK_ENABLE_JOINT12, m_bEnableJoint[11]);
	DDX_Check(pDX, IDC_CHK_ENABLE_JOINT13, m_bEnableJoint[12]);
	DDX_Check(pDX, IDC_CHK_ENABLE_JOINT14, m_bEnableJoint[13]);
	DDX_Check(pDX, IDC_CHK_ENABLE_JOINT15, m_bEnableJoint[14]);
	DDX_Check(pDX, IDC_CHK_ENABLE_JOINT16, m_bEnableJoint[15]);
	DDX_Text(pDX, IDC_EDIT_CMD_POSITION_JOINT1, m_dblCmdPosition[0]);
	DDX_Text(pDX, IDC_EDIT_CMD_POSITION_JOINT2, m_dblCmdPosition[1]);
	DDX_Text(pDX, IDC_EDIT_CMD_POSITION_JOINT3, m_dblCmdPosition[2]);
	DDX_Text(pDX, IDC_EDIT_CMD_POSITION_JOINT4, m_dblCmdPosition[3]);
	DDX_Text(pDX, IDC_EDIT_CMD_POSITION_JOINT5, m_dblCmdPosition[4]);
	DDX_Text(pDX, IDC_EDIT_CMD_POSITION_JOINT6, m_dblCmdPosition[5]);
	DDX_Text(pDX, IDC_EDIT_CMD_POSITION_JOINT7, m_dblCmdPosition[6]);
	DDX_Text(pDX, IDC_EDIT_CMD_POSITION_JOINT8, m_dblCmdPosition[7]);
	DDX_Text(pDX, IDC_EDIT_CMD_POSITION_JOINT9, m_dblCmdPosition[8]);
	DDX_Text(pDX, IDC_EDIT_CMD_POSITION_JOINT10, m_dblCmdPosition[9]);
	DDX_Text(pDX, IDC_EDIT_CMD_POSITION_JOINT11, m_dblCmdPosition[10]);
	DDX_Text(pDX, IDC_EDIT_CMD_POSITION_JOINT12, m_dblCmdPosition[11]);
	DDX_Text(pDX, IDC_EDIT_CMD_POSITION_JOINT13, m_dblCmdPosition[12]);
	DDX_Text(pDX, IDC_EDIT_CMD_POSITION_JOINT14, m_dblCmdPosition[13]);
	DDX_Text(pDX, IDC_EDIT_CMD_POSITION_JOINT15, m_dblCmdPosition[14]);
	DDX_Text(pDX, IDC_EDIT_CMD_POSITION_JOINT16, m_dblCmdPosition[15]);
	DDX_Text(pDX, IDC_EDIT_CMD_VELOCITY_JOINT1, m_dblCmdVelocity[0]);
	DDX_Text(pDX, IDC_EDIT_CMD_VELOCITY_JOINT2, m_dblCmdVelocity[1]);
	DDX_Text(pDX, IDC_EDIT_CMD_VELOCITY_JOINT3, m_dblCmdVelocity[2]);
	DDX_Text(pDX, IDC_EDIT_CMD_VELOCITY_JOINT4, m_dblCmdVelocity[3]);
	DDX_Text(pDX, IDC_EDIT_CMD_VELOCITY_JOINT5, m_dblCmdVelocity[4]);
	DDX_Text(pDX, IDC_EDIT_CMD_VELOCITY_JOINT6, m_dblCmdVelocity[5]);
	DDX_Text(pDX, IDC_EDIT_CMD_VELOCITY_JOINT7, m_dblCmdVelocity[6]);
	DDX_Text(pDX, IDC_EDIT_CMD_VELOCITY_JOINT8, m_dblCmdVelocity[7]);
	DDX_Text(pDX, IDC_EDIT_CMD_VELOCITY_JOINT9, m_dblCmdVelocity[8]);
	DDX_Text(pDX, IDC_EDIT_CMD_VELOCITY_JOINT10, m_dblCmdVelocity[9]);
	DDX_Text(pDX, IDC_EDIT_CMD_VELOCITY_JOINT11, m_dblCmdVelocity[10]);
	DDX_Text(pDX, IDC_EDIT_CMD_VELOCITY_JOINT12, m_dblCmdVelocity[11]);
	DDX_Text(pDX, IDC_EDIT_CMD_VELOCITY_JOINT13, m_dblCmdVelocity[12]);
	DDX_Text(pDX, IDC_EDIT_CMD_VELOCITY_JOINT14, m_dblCmdVelocity[13]);
	DDX_Text(pDX, IDC_EDIT_CMD_VELOCITY_JOINT15, m_dblCmdVelocity[14]);
	DDX_Text(pDX, IDC_EDIT_CMD_VELOCITY_JOINT16, m_dblCmdVelocity[15]);
	DDX_Text(pDX, IDC_EDIT_CMD_CURRENT_JOINT1, m_dblCmdCurrent[0]);
	DDX_Text(pDX, IDC_EDIT_CMD_CURRENT_JOINT2, m_dblCmdCurrent[1]);
	DDX_Text(pDX, IDC_EDIT_CMD_CURRENT_JOINT3, m_dblCmdCurrent[2]);
	DDX_Text(pDX, IDC_EDIT_CMD_CURRENT_JOINT4, m_dblCmdCurrent[3]);
	DDX_Text(pDX, IDC_EDIT_CMD_CURRENT_JOINT5, m_dblCmdCurrent[4]);
	DDX_Text(pDX, IDC_EDIT_CMD_CURRENT_JOINT6, m_dblCmdCurrent[5]);
	DDX_Text(pDX, IDC_EDIT_CMD_CURRENT_JOINT7, m_dblCmdCurrent[6]);
	DDX_Text(pDX, IDC_EDIT_CMD_CURRENT_JOINT8, m_dblCmdCurrent[7]);
	DDX_Text(pDX, IDC_EDIT_CMD_CURRENT_JOINT9, m_dblCmdCurrent[8]);
	DDX_Text(pDX, IDC_EDIT_CMD_CURRENT_JOINT10, m_dblCmdCurrent[9]);
	DDX_Text(pDX, IDC_EDIT_CMD_CURRENT_JOINT11, m_dblCmdCurrent[10]);
	DDX_Text(pDX, IDC_EDIT_CMD_CURRENT_JOINT12, m_dblCmdCurrent[11]);
	DDX_Text(pDX, IDC_EDIT_CMD_CURRENT_JOINT13, m_dblCmdCurrent[12]);
	DDX_Text(pDX, IDC_EDIT_CMD_CURRENT_JOINT14, m_dblCmdCurrent[13]);
	DDX_Text(pDX, IDC_EDIT_CMD_CURRENT_JOINT15, m_dblCmdCurrent[14]);
	DDX_Text(pDX, IDC_EDIT_CMD_CURRENT_JOINT16, m_dblCmdCurrent[15]);
}


BEGIN_MESSAGE_MAP(CUnitTestDlg, CScrollDlg)
	ON_BN_CLICKED(IDOK, &CUnitTestDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CUnitTestDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTN_CMD_POSITION, &CUnitTestDlg::OnBnClickedBtnCmdPosition)
	ON_BN_CLICKED(IDC_BTN_CMD_VELOCITY, &CUnitTestDlg::OnBnClickedBtnCmdVelocity)
	ON_BN_CLICKED(IDC_BTN_CMD_CURRENT, &CUnitTestDlg::OnBnClickedBtnCmdCurrent)
	ON_BN_CLICKED(IDC_CHK_ENABLE_JOINT1, &CUnitTestDlg::OnBnClickedChkEnableJoint1)
	ON_BN_CLICKED(IDC_CHK_ENABLE_JOINT2, &CUnitTestDlg::OnBnClickedChkEnableJoint2)
	ON_BN_CLICKED(IDC_CHK_ENABLE_JOINT3, &CUnitTestDlg::OnBnClickedChkEnableJoint3)
	ON_BN_CLICKED(IDC_CHK_ENABLE_JOINT4, &CUnitTestDlg::OnBnClickedChkEnableJoint4)
	ON_BN_CLICKED(IDC_CHK_ENABLE_JOINT5, &CUnitTestDlg::OnBnClickedChkEnableJoint5)
	ON_BN_CLICKED(IDC_CHK_ENABLE_JOINT6, &CUnitTestDlg::OnBnClickedChkEnableJoint6)
	ON_BN_CLICKED(IDC_CHK_ENABLE_JOINT7, &CUnitTestDlg::OnBnClickedChkEnableJoint7)
	ON_BN_CLICKED(IDC_CHK_ENABLE_JOINT8, &CUnitTestDlg::OnBnClickedChkEnableJoint8)
	ON_BN_CLICKED(IDC_CHK_ENABLE_JOINT9, &CUnitTestDlg::OnBnClickedChkEnableJoint9)
	ON_BN_CLICKED(IDC_CHK_ENABLE_JOINT10, &CUnitTestDlg::OnBnClickedChkEnableJoint10)
	ON_BN_CLICKED(IDC_CHK_ENABLE_JOINT11, &CUnitTestDlg::OnBnClickedChkEnableJoint11)
	ON_BN_CLICKED(IDC_CHK_ENABLE_JOINT12, &CUnitTestDlg::OnBnClickedChkEnableJoint12)
	ON_BN_CLICKED(IDC_CHK_ENABLE_JOINT13, &CUnitTestDlg::OnBnClickedChkEnableJoint13)
	ON_BN_CLICKED(IDC_CHK_ENABLE_JOINT14, &CUnitTestDlg::OnBnClickedChkEnableJoint14)
	ON_BN_CLICKED(IDC_CHK_ENABLE_JOINT15, &CUnitTestDlg::OnBnClickedChkEnableJoint15)
	ON_BN_CLICKED(IDC_CHK_ENABLE_JOINT16, &CUnitTestDlg::OnBnClickedChkEnableJoint16)
END_MESSAGE_MAP()


// CUnitTestDlg message handlers

void CUnitTestDlg::OnBnClickedOk()
{
	//OnOK();
}

void CUnitTestDlg::OnBnClickedCancel()
{
	//OnCancel();
}

void CUnitTestDlg::OnBnClickedBtnCmdPosition()
{
	if (!_pSHM) return;

	UpdateData(TRUE);
	for (int i=0; i<MAX_SLAVE_COUNT_UI; i++)
	{
		_pSHM->cmd.slave_command[i].OP_mode = eOpMode_POSITION;
		_pSHM->cmd.slave_command[i].position = 
			(TRUE == m_bEnableJoint[i] ? D2R(m_dblCmdPosition[i]) : _pSHM->state.slave_state[i].position);
	}
	_pSHM->cmd.reserved_1 = 1;
}

void CUnitTestDlg::OnBnClickedBtnCmdVelocity()
{
	if (!_pSHM) return;

	UpdateData(TRUE);
	for (int i=0; i<MAX_SLAVE_COUNT_UI; i++)
	{
		_pSHM->cmd.slave_command[i].OP_mode = eOpMode_VELOCITY;
		_pSHM->cmd.slave_command[i].velocity = 
			(TRUE == m_bEnableJoint[i] ? D2R(m_dblCmdVelocity[i]) : 0);
	}
	_pSHM->cmd.reserved_1 = 1;
}

void CUnitTestDlg::OnBnClickedBtnCmdCurrent()
{
	if (!_pSHM) return;
	
	UpdateData(TRUE);
	for (int i=0; i<MAX_SLAVE_COUNT_UI; i++)
	{
		_pSHM->cmd.slave_command[i].OP_mode = eOpMode_TORQUE;
		_pSHM->cmd.slave_command[i].torque = 
			(TRUE == m_bEnableJoint[i] ? m_dblCmdCurrent[i] : 0);
	}
	_pSHM->cmd.reserved_1 = 1;
}

void CUnitTestDlg::OnBnClickedChkEnableJoint1()
{
	m_bEnableJoint[0] = ((CButton*)GetDlgItem(IDC_CHK_ENABLE_JOINT1))->GetCheck();
}

void CUnitTestDlg::OnBnClickedChkEnableJoint2()
{
	m_bEnableJoint[1] = ((CButton*)GetDlgItem(IDC_CHK_ENABLE_JOINT2))->GetCheck();
}

void CUnitTestDlg::OnBnClickedChkEnableJoint3()
{
	m_bEnableJoint[2] = ((CButton*)GetDlgItem(IDC_CHK_ENABLE_JOINT3))->GetCheck();
}

void CUnitTestDlg::OnBnClickedChkEnableJoint4()
{
	m_bEnableJoint[3] = ((CButton*)GetDlgItem(IDC_CHK_ENABLE_JOINT4))->GetCheck();
}

void CUnitTestDlg::OnBnClickedChkEnableJoint5()
{
	m_bEnableJoint[4] = ((CButton*)GetDlgItem(IDC_CHK_ENABLE_JOINT5))->GetCheck();
}

void CUnitTestDlg::OnBnClickedChkEnableJoint6()
{
	m_bEnableJoint[5] = ((CButton*)GetDlgItem(IDC_CHK_ENABLE_JOINT6))->GetCheck();
}

void CUnitTestDlg::OnBnClickedChkEnableJoint7()
{
	m_bEnableJoint[6] = ((CButton*)GetDlgItem(IDC_CHK_ENABLE_JOINT7))->GetCheck();
}

void CUnitTestDlg::OnBnClickedChkEnableJoint8()
{
	m_bEnableJoint[7] = ((CButton*)GetDlgItem(IDC_CHK_ENABLE_JOINT8))->GetCheck();
}

void CUnitTestDlg::OnBnClickedChkEnableJoint9()
{
	m_bEnableJoint[8] = ((CButton*)GetDlgItem(IDC_CHK_ENABLE_JOINT9))->GetCheck();
}

void CUnitTestDlg::OnBnClickedChkEnableJoint10()
{
	m_bEnableJoint[9] = ((CButton*)GetDlgItem(IDC_CHK_ENABLE_JOINT10))->GetCheck();
}

void CUnitTestDlg::OnBnClickedChkEnableJoint11()
{
	m_bEnableJoint[10] = ((CButton*)GetDlgItem(IDC_CHK_ENABLE_JOINT11))->GetCheck();
}

void CUnitTestDlg::OnBnClickedChkEnableJoint12()
{
	m_bEnableJoint[11] = ((CButton*)GetDlgItem(IDC_CHK_ENABLE_JOINT12))->GetCheck();
}

void CUnitTestDlg::OnBnClickedChkEnableJoint13()
{
	m_bEnableJoint[12] = ((CButton*)GetDlgItem(IDC_CHK_ENABLE_JOINT13))->GetCheck();
}

void CUnitTestDlg::OnBnClickedChkEnableJoint14()
{
	m_bEnableJoint[13] = ((CButton*)GetDlgItem(IDC_CHK_ENABLE_JOINT14))->GetCheck();
}

void CUnitTestDlg::OnBnClickedChkEnableJoint15()
{
	m_bEnableJoint[14] = ((CButton*)GetDlgItem(IDC_CHK_ENABLE_JOINT15))->GetCheck();
}

void CUnitTestDlg::OnBnClickedChkEnableJoint16()
{
	m_bEnableJoint[15] = ((CButton*)GetDlgItem(IDC_CHK_ENABLE_JOINT16))->GetCheck();
}
