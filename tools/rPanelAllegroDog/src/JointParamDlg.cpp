// JointParamDlg.cpp : implementation file
//

#include "stdafx.h"
#include "rPanelManipulatorCmdUtil.h"
#include "rPanelManipulatorDef.h"
#include "rPanelAllegroDog.h"
#include "JointParamDlg.h"


// CJointParamDlg dialog

IMPLEMENT_DYNAMIC(CJointParamDlg, CScrollDlg)

CJointParamDlg::CJointParamDlg(CWnd* pParent /*=NULL*/)
	: CScrollDlg(CJointParamDlg::IDD, pParent)
{
}

CJointParamDlg::~CJointParamDlg()
{
}

void CJointParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CScrollDlg::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_GAIN_KP1, m_strGainKp[0]);
	DDX_Text(pDX, IDC_EDIT_GAIN_KP2, m_strGainKp[1]);
	DDX_Text(pDX, IDC_EDIT_GAIN_KP3, m_strGainKp[2]);
	DDX_Text(pDX, IDC_EDIT_GAIN_KP4, m_strGainKp[3]);
	DDX_Text(pDX, IDC_EDIT_GAIN_KP5, m_strGainKp[4]);
	DDX_Text(pDX, IDC_EDIT_GAIN_KP6, m_strGainKp[5]);
	DDX_Text(pDX, IDC_EDIT_GAIN_KP7, m_strGainKp[6]);
	DDX_Text(pDX, IDC_EDIT_GAIN_KP8, m_strGainKp[7]);
	DDX_Text(pDX, IDC_EDIT_GAIN_KP9, m_strGainKp[8]);
	DDX_Text(pDX, IDC_EDIT_GAIN_KP10, m_strGainKp[9]);
	DDX_Text(pDX, IDC_EDIT_GAIN_KP11, m_strGainKp[10]);
	DDX_Text(pDX, IDC_EDIT_GAIN_KP12, m_strGainKp[11]);
	DDX_Text(pDX, IDC_EDIT_GAIN_KP13, m_strGainKp[12]);
	DDX_Text(pDX, IDC_EDIT_GAIN_KP14, m_strGainKp[13]);
	DDX_Text(pDX, IDC_EDIT_GAIN_KP15, m_strGainKp[14]);
	DDX_Text(pDX, IDC_EDIT_GAIN_KP16, m_strGainKp[15]);
	DDX_Text(pDX, IDC_EDIT_GAIN_KV1, m_strGainKv[0]);
	DDX_Text(pDX, IDC_EDIT_GAIN_KV2, m_strGainKv[1]);
	DDX_Text(pDX, IDC_EDIT_GAIN_KV3, m_strGainKv[2]);
	DDX_Text(pDX, IDC_EDIT_GAIN_KV4, m_strGainKv[3]);
	DDX_Text(pDX, IDC_EDIT_GAIN_KV5, m_strGainKv[4]);
	DDX_Text(pDX, IDC_EDIT_GAIN_KV6, m_strGainKv[5]);
	DDX_Text(pDX, IDC_EDIT_GAIN_KV7, m_strGainKv[6]);
	DDX_Text(pDX, IDC_EDIT_GAIN_KV8, m_strGainKv[7]);
	DDX_Text(pDX, IDC_EDIT_GAIN_KV9, m_strGainKv[8]);
	DDX_Text(pDX, IDC_EDIT_GAIN_KV10, m_strGainKv[9]);
	DDX_Text(pDX, IDC_EDIT_GAIN_KV11, m_strGainKv[10]);
	DDX_Text(pDX, IDC_EDIT_GAIN_KV12, m_strGainKv[11]);
	DDX_Text(pDX, IDC_EDIT_GAIN_KV13, m_strGainKv[12]);
	DDX_Text(pDX, IDC_EDIT_GAIN_KV14, m_strGainKv[13]);
	DDX_Text(pDX, IDC_EDIT_GAIN_KV15, m_strGainKv[14]);
	DDX_Text(pDX, IDC_EDIT_GAIN_KV16, m_strGainKv[15]);
	DDX_Text(pDX, IDC_EDIT_GAIN_KI1, m_strGainKi[0]);
	DDX_Text(pDX, IDC_EDIT_GAIN_KI2, m_strGainKi[1]);
	DDX_Text(pDX, IDC_EDIT_GAIN_KI3, m_strGainKi[2]);
	DDX_Text(pDX, IDC_EDIT_GAIN_KI4, m_strGainKi[3]);
	DDX_Text(pDX, IDC_EDIT_GAIN_KI5, m_strGainKi[4]);
	DDX_Text(pDX, IDC_EDIT_GAIN_KI6, m_strGainKi[5]);
	DDX_Text(pDX, IDC_EDIT_GAIN_KI7, m_strGainKi[6]);
	DDX_Text(pDX, IDC_EDIT_GAIN_KI8, m_strGainKi[7]);
	DDX_Text(pDX, IDC_EDIT_GAIN_KI9, m_strGainKi[8]);
	DDX_Text(pDX, IDC_EDIT_GAIN_KI10, m_strGainKi[9]);
	DDX_Text(pDX, IDC_EDIT_GAIN_KI11, m_strGainKi[10]);
	DDX_Text(pDX, IDC_EDIT_GAIN_KI12, m_strGainKi[11]);
	DDX_Text(pDX, IDC_EDIT_GAIN_KI13, m_strGainKi[12]);
	DDX_Text(pDX, IDC_EDIT_GAIN_KI14, m_strGainKi[13]);
	DDX_Text(pDX, IDC_EDIT_GAIN_KI15, m_strGainKi[14]);
	DDX_Text(pDX, IDC_EDIT_GAIN_KI16, m_strGainKi[15]);
	DDX_Text(pDX, IDC_EDIT_GAIN_INVL2_1, m_strGainInvL2[0]);
	DDX_Text(pDX, IDC_EDIT_GAIN_INVL2_2, m_strGainInvL2[1]);
	DDX_Text(pDX, IDC_EDIT_GAIN_INVL2_3, m_strGainInvL2[2]);
	DDX_Text(pDX, IDC_EDIT_GAIN_INVL2_4, m_strGainInvL2[3]);
	DDX_Text(pDX, IDC_EDIT_GAIN_INVL2_5, m_strGainInvL2[4]);
	DDX_Text(pDX, IDC_EDIT_GAIN_INVL2_6, m_strGainInvL2[5]);
	DDX_Text(pDX, IDC_EDIT_GAIN_INVL2_7, m_strGainInvL2[6]);
	DDX_Text(pDX, IDC_EDIT_GAIN_INVL2_8, m_strGainInvL2[7]);
	DDX_Text(pDX, IDC_EDIT_GAIN_INVL2_9, m_strGainInvL2[8]);
	DDX_Text(pDX, IDC_EDIT_GAIN_INVL2_10, m_strGainInvL2[9]);
	DDX_Text(pDX, IDC_EDIT_GAIN_INVL2_11, m_strGainInvL2[10]);
	DDX_Text(pDX, IDC_EDIT_GAIN_INVL2_12, m_strGainInvL2[11]);
	DDX_Text(pDX, IDC_EDIT_GAIN_INVL2_13, m_strGainInvL2[12]);
	DDX_Text(pDX, IDC_EDIT_GAIN_INVL2_14, m_strGainInvL2[13]);
	DDX_Text(pDX, IDC_EDIT_GAIN_INVL2_15, m_strGainInvL2[14]);
	DDX_Text(pDX, IDC_EDIT_GAIN_INVL2_16, m_strGainInvL2[15]);
	DDX_Text(pDX, IDC_EDIT_POS_UPPER1, m_strPosUpper[0]);
	DDX_Text(pDX, IDC_EDIT_POS_UPPER2, m_strPosUpper[1]);
	DDX_Text(pDX, IDC_EDIT_POS_UPPER3, m_strPosUpper[2]);
	DDX_Text(pDX, IDC_EDIT_POS_UPPER4, m_strPosUpper[3]);
	DDX_Text(pDX, IDC_EDIT_POS_UPPER5, m_strPosUpper[4]);
	DDX_Text(pDX, IDC_EDIT_POS_UPPER6, m_strPosUpper[5]);
	DDX_Text(pDX, IDC_EDIT_POS_UPPER7, m_strPosUpper[6]);
	DDX_Text(pDX, IDC_EDIT_POS_UPPER8, m_strPosUpper[7]);
	DDX_Text(pDX, IDC_EDIT_POS_UPPER9, m_strPosUpper[8]);
	DDX_Text(pDX, IDC_EDIT_POS_UPPER10, m_strPosUpper[9]);
	DDX_Text(pDX, IDC_EDIT_POS_UPPER11, m_strPosUpper[10]);
	DDX_Text(pDX, IDC_EDIT_POS_UPPER12, m_strPosUpper[11]);
	DDX_Text(pDX, IDC_EDIT_POS_UPPER13, m_strPosUpper[12]);
	DDX_Text(pDX, IDC_EDIT_POS_UPPER14, m_strPosUpper[13]);
	DDX_Text(pDX, IDC_EDIT_POS_UPPER15, m_strPosUpper[14]);
	DDX_Text(pDX, IDC_EDIT_POS_UPPER16, m_strPosUpper[15]);
	DDX_Text(pDX, IDC_EDIT_POS_LOWER1, m_strPosLower[0]);
	DDX_Text(pDX, IDC_EDIT_POS_LOWER2, m_strPosLower[1]);
	DDX_Text(pDX, IDC_EDIT_POS_LOWER3, m_strPosLower[2]);
	DDX_Text(pDX, IDC_EDIT_POS_LOWER4, m_strPosLower[3]);
	DDX_Text(pDX, IDC_EDIT_POS_LOWER5, m_strPosLower[4]);
	DDX_Text(pDX, IDC_EDIT_POS_LOWER6, m_strPosLower[5]);
	DDX_Text(pDX, IDC_EDIT_POS_LOWER7, m_strPosLower[6]);
	DDX_Text(pDX, IDC_EDIT_POS_LOWER8, m_strPosLower[7]);
	DDX_Text(pDX, IDC_EDIT_POS_LOWER9, m_strPosLower[8]);
	DDX_Text(pDX, IDC_EDIT_POS_LOWER10, m_strPosLower[9]);
	DDX_Text(pDX, IDC_EDIT_POS_LOWER11, m_strPosLower[10]);
	DDX_Text(pDX, IDC_EDIT_POS_LOWER12, m_strPosLower[11]);
	DDX_Text(pDX, IDC_EDIT_POS_LOWER13, m_strPosLower[12]);
	DDX_Text(pDX, IDC_EDIT_POS_LOWER14, m_strPosLower[13]);
	DDX_Text(pDX, IDC_EDIT_POS_LOWER15, m_strPosLower[14]);
	DDX_Text(pDX, IDC_EDIT_POS_LOWER16, m_strPosLower[15]);
	DDX_Text(pDX, IDC_EDIT_VEL_UPPER1, m_strVelUpper[0]);
	DDX_Text(pDX, IDC_EDIT_VEL_UPPER2, m_strVelUpper[1]);
	DDX_Text(pDX, IDC_EDIT_VEL_UPPER3, m_strVelUpper[2]);
	DDX_Text(pDX, IDC_EDIT_VEL_UPPER4, m_strVelUpper[3]);
	DDX_Text(pDX, IDC_EDIT_VEL_UPPER5, m_strVelUpper[4]);
	DDX_Text(pDX, IDC_EDIT_VEL_UPPER6, m_strVelUpper[5]);
	DDX_Text(pDX, IDC_EDIT_VEL_UPPER7, m_strVelUpper[6]);
	DDX_Text(pDX, IDC_EDIT_VEL_UPPER8, m_strVelUpper[7]);
	DDX_Text(pDX, IDC_EDIT_VEL_UPPER9, m_strVelUpper[8]);
	DDX_Text(pDX, IDC_EDIT_VEL_UPPER10, m_strVelUpper[9]);
	DDX_Text(pDX, IDC_EDIT_VEL_UPPER11, m_strVelUpper[10]);
	DDX_Text(pDX, IDC_EDIT_VEL_UPPER12, m_strVelUpper[11]);
	DDX_Text(pDX, IDC_EDIT_VEL_UPPER13, m_strVelUpper[12]);
	DDX_Text(pDX, IDC_EDIT_VEL_UPPER14, m_strVelUpper[13]);
	DDX_Text(pDX, IDC_EDIT_VEL_UPPER15, m_strVelUpper[14]);
	DDX_Text(pDX, IDC_EDIT_VEL_UPPER16, m_strVelUpper[15]);
	DDX_Text(pDX, IDC_EDIT_ACC_UPPER1, m_strAccUpper[0]);
	DDX_Text(pDX, IDC_EDIT_ACC_UPPER2, m_strAccUpper[1]);
	DDX_Text(pDX, IDC_EDIT_ACC_UPPER3, m_strAccUpper[2]);
	DDX_Text(pDX, IDC_EDIT_ACC_UPPER4, m_strAccUpper[3]);
	DDX_Text(pDX, IDC_EDIT_ACC_UPPER5, m_strAccUpper[4]);
	DDX_Text(pDX, IDC_EDIT_ACC_UPPER6, m_strAccUpper[5]);
	DDX_Text(pDX, IDC_EDIT_ACC_UPPER7, m_strAccUpper[6]);
	DDX_Text(pDX, IDC_EDIT_ACC_UPPER8, m_strAccUpper[7]);
	DDX_Text(pDX, IDC_EDIT_ACC_UPPER9, m_strAccUpper[8]);
	DDX_Text(pDX, IDC_EDIT_ACC_UPPER10, m_strAccUpper[9]);
	DDX_Text(pDX, IDC_EDIT_ACC_UPPER11, m_strAccUpper[10]);
	DDX_Text(pDX, IDC_EDIT_ACC_UPPER12, m_strAccUpper[11]);
	DDX_Text(pDX, IDC_EDIT_ACC_UPPER13, m_strAccUpper[12]);
	DDX_Text(pDX, IDC_EDIT_ACC_UPPER14, m_strAccUpper[13]);
	DDX_Text(pDX, IDC_EDIT_ACC_UPPER15, m_strAccUpper[14]);
	DDX_Text(pDX, IDC_EDIT_ACC_UPPER16, m_strAccUpper[15]);
}

BEGIN_MESSAGE_MAP(CJointParamDlg, CScrollDlg)
	ON_BN_CLICKED(IDCANCEL, &CJointParamDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CJointParamDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CJointParamDlg message handlers

void CJointParamDlg::OnBnClickedCancel()
{
	rPanelManipulatorData_t* pData = getrPanelManipulatorCmdMemory();
	if (!pData) return;

	for (int i=0; i<16; i++)
	{
		m_strGainKp[i].Format(_T("%.2f"), pData->param.kp[i]);
		m_strGainKv[i].Format(_T("%.2f"), pData->param.kv[i]);
		m_strGainKi[i].Format(_T("%.2f"), pData->param.ki[i]);
		m_strGainInvL2[i].Format(_T("%.2f"), pData->param.invL2[i]);
		m_strVelUpper[i].Format(_T("%.1f"), R2D(pData->param.vel_ubound[i]));
		m_strAccUpper[i].Format(_T("%.1f"), R2D(pData->param.acc_ubound[i]));
		m_strPosLower[i].Format(_T("%.1f"), R2D(pData->param.pos_lbound[i]));
		m_strPosUpper[i].Format(_T("%.1f"), R2D(pData->param.pos_ubound[i]));
	}

	UpdateData(FALSE);
}

void CJointParamDlg::OnBnClickedOk()
{
	rPanelManipulatorData_t* pData = getrPanelManipulatorCmdMemory();
	if (!pData) return;

	UpdateData(TRUE);

	for (int i=0; i<16; i++)
	{
		pData->param.kp[i] = _tstof(m_strGainKp[i]);
		pData->param.kv[i] = _tstof(m_strGainKv[i]);
		pData->param.ki[i] = _tstof(m_strGainKi[i]);
		pData->param.invL2[i] = _tstof(m_strGainInvL2[i]);
		pData->param.vel_ubound[i] = D2R(_tstof(m_strVelUpper[i]));
		pData->param.acc_ubound[i] = D2R(_tstof(m_strAccUpper[i]));
		pData->param.pos_lbound[i] = D2R(_tstof(m_strPosLower[i]));
		pData->param.pos_ubound[i] = D2R(_tstof(m_strPosUpper[i]));
	}
}

BOOL CJointParamDlg::OnInitDialog()
{
	CScrollDlg::OnInitDialog();

	OnBnClickedCancel();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
