// GraphOptionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "rPanelAllegroDog.h"
#include "GraphOptionDlg.h"


// GraphOptionDlg dialog

IMPLEMENT_DYNAMIC(CGraphOptionDlg, CDialog)

CGraphOptionDlg::CGraphOptionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGraphOptionDlg::IDD, pParent)
{

}

CGraphOptionDlg::~CGraphOptionDlg()
{
	
}

void CGraphOptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRAPH_OPTION_SAMPLING_RATE, m_cmbSamplingPeriod);
	DDX_Control(pDX, IDC_GRAPH_OPTION_SAMPLING_DURATION, m_cmbSamplingDuration);
	DDX_Control(pDX, IDC_GRAPH_OPTION_Q1, m_chkQ[0]);
	DDX_Control(pDX, IDC_GRAPH_OPTION_Q2, m_chkQ[1]);
	DDX_Control(pDX, IDC_GRAPH_OPTION_Q3, m_chkQ[2]);
	DDX_Control(pDX, IDC_GRAPH_OPTION_Q4, m_chkQ[3]);
	DDX_Control(pDX, IDC_GRAPH_OPTION_Q5, m_chkQ[4]);
	DDX_Control(pDX, IDC_GRAPH_OPTION_Q6, m_chkQ[5]);
	DDX_Control(pDX, IDC_GRAPH_OPTION_Q7, m_chkQ[6]);
	DDX_Control(pDX, IDC_GRAPH_OPTION_Q8, m_chkQ[7]);
	DDX_Control(pDX, IDC_GRAPH_OPTION_Q9, m_chkQ[8]);
	DDX_Control(pDX, IDC_GRAPH_OPTION_Q10, m_chkQ[9]);
	DDX_Control(pDX, IDC_GRAPH_OPTION_Q11, m_chkQ[10]);
	DDX_Control(pDX, IDC_GRAPH_OPTION_Q12, m_chkQ[11]);
	DDX_Control(pDX, IDC_GRAPH_OPTION_QDOT1, m_chkQdot[0]);
	DDX_Control(pDX, IDC_GRAPH_OPTION_QDOT2, m_chkQdot[1]);
	DDX_Control(pDX, IDC_GRAPH_OPTION_QDOT3, m_chkQdot[2]);
	DDX_Control(pDX, IDC_GRAPH_OPTION_QDOT4, m_chkQdot[3]);
	DDX_Control(pDX, IDC_GRAPH_OPTION_QDOT5, m_chkQdot[4]);
	DDX_Control(pDX, IDC_GRAPH_OPTION_QDOT6, m_chkQdot[5]);
	DDX_Control(pDX, IDC_GRAPH_OPTION_QDOT7, m_chkQdot[6]);
	DDX_Control(pDX, IDC_GRAPH_OPTION_QDOT8, m_chkQdot[7]);
	DDX_Control(pDX, IDC_GRAPH_OPTION_QDOT9, m_chkQdot[8]);
	DDX_Control(pDX, IDC_GRAPH_OPTION_QDOT10, m_chkQdot[9]);
	DDX_Control(pDX, IDC_GRAPH_OPTION_QDOT11, m_chkQdot[10]);
	DDX_Control(pDX, IDC_GRAPH_OPTION_QDOT12, m_chkQdot[11]);
	DDX_Control(pDX, IDC_GRAPH_OPTION_TAU1, m_chkTau[0]);
	DDX_Control(pDX, IDC_GRAPH_OPTION_TAU2, m_chkTau[1]);
	DDX_Control(pDX, IDC_GRAPH_OPTION_TAU3, m_chkTau[2]);
	DDX_Control(pDX, IDC_GRAPH_OPTION_TAU4, m_chkTau[3]);
	DDX_Control(pDX, IDC_GRAPH_OPTION_TAU5, m_chkTau[4]);
	DDX_Control(pDX, IDC_GRAPH_OPTION_TAU6, m_chkTau[5]);
	DDX_Control(pDX, IDC_GRAPH_OPTION_TAU7, m_chkTau[6]);
	DDX_Control(pDX, IDC_GRAPH_OPTION_TAU8, m_chkTau[7]);
	DDX_Control(pDX, IDC_GRAPH_OPTION_TAU9, m_chkTau[8]);
	DDX_Control(pDX, IDC_GRAPH_OPTION_TAU10,m_chkTau[9]);
	DDX_Control(pDX, IDC_GRAPH_OPTION_TAU11, m_chkTau[10]);
	DDX_Control(pDX, IDC_GRAPH_OPTION_TAU12, m_chkTau[11]);
	DDX_Control(pDX, IDC_GRAPH_OPTION_ERR_COUNT, m_chkErrCount);
	
}


BEGIN_MESSAGE_MAP(CGraphOptionDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CGraphOptionDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CGraphOptionDlg message handlers

BOOL CGraphOptionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();


	int nSelect = 0;
	int nIndex = 0;

	nIndex = m_cmbSamplingPeriod.AddString(_T("2 msec"));
	m_cmbSamplingPeriod.SetItemData(nIndex, 2);
	nIndex = m_cmbSamplingPeriod.AddString(_T("5 msec"));
	m_cmbSamplingPeriod.SetItemData(nIndex, 5);
	nIndex = m_cmbSamplingPeriod.AddString(_T("10 msec"));
	m_cmbSamplingPeriod.SetItemData(nIndex, 10);
	nIndex = m_cmbSamplingPeriod.AddString(_T("50 msec"));
	m_cmbSamplingPeriod.SetItemData(nIndex, 50);
	nIndex = m_cmbSamplingPeriod.AddString(_T("100 msec"));
	m_cmbSamplingPeriod.SetItemData(nIndex, 100);
	nIndex = m_cmbSamplingPeriod.AddString(_T("1 sec"));
	m_cmbSamplingPeriod.SetItemData(nIndex, 100);

	nSelect = 0;
	for (int i=0; i<m_cmbSamplingPeriod.GetCount(); i++)
	{
		int itemData = (int)m_cmbSamplingPeriod.GetItemData(i);
		if (itemData == m_nSamplingPeriod)
		{
			nSelect = i;
			break;
		}
	}
	m_cmbSamplingPeriod.SetCurSel(nSelect);
	
	nIndex = m_cmbSamplingDuration.AddString(_T("10 seconds"));
	m_cmbSamplingDuration.SetItemData(nIndex, 10000);
	nIndex = m_cmbSamplingDuration.AddString(_T("30 seconds"));
	m_cmbSamplingDuration.SetItemData(nIndex, 30000);
	nIndex = m_cmbSamplingDuration.AddString(_T("1 minute"));
	m_cmbSamplingDuration.SetItemData(nIndex, 60000);
	nIndex = m_cmbSamplingDuration.AddString(_T("2 minutes"));
	m_cmbSamplingDuration.SetItemData(nIndex, 120000);

	nSelect = 0;
	for (int i=0; i<m_cmbSamplingDuration.GetCount(); i++)
	{
		int itemData = (int)m_cmbSamplingDuration.GetItemData(i);
		if (itemData == m_nSamplingDuration)
		{
			nSelect = i;
			break;
		}
	}
	m_cmbSamplingDuration.SetCurSel(nSelect);
	
	for (int i=0; i<JDOF; i++)
	{
		m_chkQ[i].SetCheck(m_bQ[i]);
		m_chkQdot[i].SetCheck(m_bQdot[i]);
		m_chkTau[i].SetCheck(m_bTau[i]);
	}
	m_chkErrCount.SetCheck(m_bErrCount);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CGraphOptionDlg::OnBnClickedOk()
{
	int nSelected;
	nSelected = m_cmbSamplingPeriod.GetCurSel();
	m_nSamplingPeriod = (int)m_cmbSamplingPeriod.GetItemData(nSelected);
	nSelected = m_cmbSamplingDuration.GetCurSel();
	m_nSamplingDuration = (int)m_cmbSamplingDuration.GetItemData(nSelected);
	for (int i=0; i<JDOF; i++)
	{
		m_bQ[i] = m_chkQ[i].GetCheck();
		m_bQdot[i] = m_chkQdot[i].GetCheck();
		m_bTau[i] =m_chkTau[i].GetCheck();
	}
	m_bErrCount = m_chkErrCount.GetCheck();

	OnOK();
}
