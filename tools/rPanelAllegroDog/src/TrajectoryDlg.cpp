// TrajectoryDlg.cpp : implementation file
//

#include "stdafx.h"
#include "rPanelManipulatorCmdUtil.h"
#include "rPanelManipulatorDef.h"
#include "rPanelAllegroDog.h"
#include "TrajectoryDlg.h"


BEGIN_MESSAGE_MAP(CListCtrlKeyDown, CListCtrl)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

void CListCtrlKeyDown::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_DELETE)
		((CTrajectoryDlg*)GetParent())->OnDeleteKeyPressed(this);

	CListCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}

// CTrajectoryDlg dialog

BOOL CTrajectoryDlg::ms_bEnableJointLimit = TRUE;

IMPLEMENT_DYNAMIC(CTrajectoryDlg, CScrollDlg)

CTrajectoryDlg::CTrajectoryDlg(CWnd* pParent /*=NULL*/)
	: CScrollDlg(CTrajectoryDlg::IDD, pParent)
	, m_trajType(eTrajType_TASK)
	, m_strTaskX(_T("0"))
	, m_strTaskY(_T("0"))
	, m_strTaskZ(_T("0"))
	, m_strTaskXg(_T("0"))
	, m_strTaskYg(_T("0"))
	, m_strTaskZg(_T("0"))
	, m_strTaskThg(_T("0"))
	, m_strTaskRoll(_T("0"))
	, m_strTaskPitch(_T("0"))
	, m_strTaskYaw(_T("0"))
	, m_strTaskDO(_T("0"))
	, m_strTaskDuration(_T("0"))
	, m_strJoint1(_T("0"))
	, m_strJoint2(_T("0"))
	, m_strJoint3(_T("0"))
	, m_strJoint4(_T("0"))
	, m_strJoint5(_T("0"))
	, m_strJoint6(_T("0"))
	, m_strJoint7(_T("0"))
	, m_strJoint8(_T("0"))
	, m_strJoint9(_T("0"))
	, m_strJoint10(_T("0"))
	, m_strJoint11(_T("0"))
	, m_strJoint12(_T("0"))
	, m_strJoint13(_T("0"))
	, m_strJoint14(_T("0"))
	, m_strJoint15(_T("0"))
	, m_strJoint16(_T("0"))
	, m_strJointDO(_T("0"))
	, m_strJointDuration(_T("0"))
	, _isAdd(false)
{
	_pSHM = getrPanelManipulatorCmdMemory();

	_strJointArray[0] = &m_strJoint1;
	_strJointArray[1] = &m_strJoint2;
	_strJointArray[2] = &m_strJoint3;
	_strJointArray[3] = &m_strJoint4;
	_strJointArray[4] = &m_strJoint5;
	_strJointArray[5] = &m_strJoint6;
	_strJointArray[6] = &m_strJoint7;
	_strJointArray[7] = &m_strJoint8;
	_strJointArray[8] = &m_strJoint9;
	_strJointArray[9] = &m_strJoint10;
	_strJointArray[10] = &m_strJoint11;
	_strJointArray[11] = &m_strJoint12;
	_strJointArray[12] = &m_strJoint13;
	_strJointArray[13] = &m_strJoint14;
	_strJointArray[14] = &m_strJoint15;
	_strJointArray[15] = &m_strJoint16;
	_strJointArray[16] = &m_strJointDO;
	_strJointArray[17] = &m_strJointDuration;

	_strTaskArray[0] = &m_strTaskX;
	_strTaskArray[1] = &m_strTaskY;
	_strTaskArray[2] = &m_strTaskZ;
	_strTaskArray[3] = &m_strTaskRoll;
	_strTaskArray[4] = &m_strTaskPitch;
	_strTaskArray[5] = &m_strTaskYaw;
	_strTaskArray[6] = &m_strTaskXg;
	_strTaskArray[7] = &m_strTaskYg;
	_strTaskArray[8] = &m_strTaskZg;
	_strTaskArray[9] = &m_strTaskThg;
	_strTaskArray[10] = &m_strTaskDO;
	_strTaskArray[11] = &m_strTaskDuration;
}

CTrajectoryDlg::~CTrajectoryDlg()
{
}

void CTrajectoryDlg::DoDataExchange(CDataExchange* pDX)
{
	CScrollDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_TRAJ_TASK, m_listCtrlTrajTask);
	DDX_Control(pDX, IDC_LIST_TRAJ_JOINT, m_listCtrlTrajJoint);
	DDX_Text(pDX, IDC_EDIT_TRAJ_TASK_XG, m_strTaskXg);
	DDX_Text(pDX, IDC_EDIT_TRAJ_TASK_YG, m_strTaskYg);
	DDX_Text(pDX, IDC_EDIT_TRAJ_TASK_ZG, m_strTaskZg);
	DDX_Text(pDX, IDC_EDIT_TRAJ_TASK_THG, m_strTaskThg);
	DDX_Text(pDX, IDC_EDIT_TRAJ_TASK_X, m_strTaskX);
	DDX_Text(pDX, IDC_EDIT_TRAJ_TASK_Y, m_strTaskY);
	DDX_Text(pDX, IDC_EDIT_TRAJ_TASK_Z, m_strTaskZ);
	DDX_Text(pDX, IDC_EDIT_TRAJ_TASK_ROLL, m_strTaskRoll);
	DDX_Text(pDX, IDC_EDIT_TRAJ_TASK_PITCH, m_strTaskPitch);
	DDX_Text(pDX, IDC_EDIT_TRAJ_TASK_YAW, m_strTaskYaw);
	DDX_Text(pDX, IDC_EDIT_TRAJ_TASK_DO, m_strTaskDO);
	DDX_Text(pDX, IDC_EDIT_TRAJ_TASK_DURATION, m_strTaskDuration);
	DDX_Text(pDX, IDC_EDIT_TRAJ_JOINT1, m_strJoint1);
	DDX_Text(pDX, IDC_EDIT_TRAJ_JOINT2, m_strJoint2);
	DDX_Text(pDX, IDC_EDIT_TRAJ_JOINT3, m_strJoint3);
	DDX_Text(pDX, IDC_EDIT_TRAJ_JOINT4, m_strJoint4);
	DDX_Text(pDX, IDC_EDIT_TRAJ_JOINT5, m_strJoint5);
	DDX_Text(pDX, IDC_EDIT_TRAJ_JOINT6, m_strJoint6);
	DDX_Text(pDX, IDC_EDIT_TRAJ_JOINT7, m_strJoint7);
	DDX_Text(pDX, IDC_EDIT_TRAJ_JOINT8, m_strJoint8);
	DDX_Text(pDX, IDC_EDIT_TRAJ_JOINT9, m_strJoint9);
	DDX_Text(pDX, IDC_EDIT_TRAJ_JOINT10, m_strJoint10);
	DDX_Text(pDX, IDC_EDIT_TRAJ_JOINT11, m_strJoint11);
	DDX_Text(pDX, IDC_EDIT_TRAJ_JOINT12, m_strJoint12);
	DDX_Text(pDX, IDC_EDIT_TRAJ_JOINT13, m_strJoint13);
	DDX_Text(pDX, IDC_EDIT_TRAJ_JOINT14, m_strJoint14);
	DDX_Text(pDX, IDC_EDIT_TRAJ_JOINT15, m_strJoint15);
	DDX_Text(pDX, IDC_EDIT_TRAJ_JOINT16, m_strJoint16);
	DDX_Text(pDX, IDC_EDIT_TRAJ_JOINT_DO, m_strJointDO);
	DDX_Text(pDX, IDC_EDIT_TRAJ_JOINT_DURATION, m_strJointDuration);
	DDX_Control(pDX, IDC_CHECK_LIMIT_JOINT, m_chkEnableJointLimit);
}


BEGIN_MESSAGE_MAP(CTrajectoryDlg, CScrollDlg)
	ON_BN_CLICKED(IDC_CHK_TRAJ_TASK, &CTrajectoryDlg::OnBnClickedChkTrajTask)
	ON_BN_CLICKED(IDC_CHK_TRAJ_JOINT, &CTrajectoryDlg::OnBnClickedChkTrajJoint)
	ON_BN_CLICKED(IDC_BTN_TRAJ_ADD_TASK, &CTrajectoryDlg::OnBnClickedBtnTrajAddTask)
	ON_BN_CLICKED(IDC_BTN_TRAJ_ADD_JOINT, &CTrajectoryDlg::OnBnClickedBtnTrajAddJoint)
	ON_BN_CLICKED(IDOK, &CTrajectoryDlg::OnBnClickedOk)
	ON_COMMAND(ID_TRAJECTORYDLG_FILE_NEW, &CTrajectoryDlg::OnTrajectoryDialogFileNew)
	ON_COMMAND(ID_TRAJECTORYDLG_FILE_OPEN, &CTrajectoryDlg::OnTrajectoryDialogFileOpen)
	ON_COMMAND(ID_TRAJECTORYDLG_FILE_SAVE, &CTrajectoryDlg::OnTrajectoryDialogFileSave)
	ON_COMMAND(ID_TRAJECTORYDLG_FILE_SAVEAS, &CTrajectoryDlg::OnTrajectoryDialogFileSaveas)
	ON_BN_CLICKED(IDC_BTN_TRAJ_CLEAR_TASK, &CTrajectoryDlg::OnBnClickedBtnTrajClearTask)
	ON_BN_CLICKED(IDC_BTN_TRAJ_DELETE_TASK, &CTrajectoryDlg::OnBnClickedBtnTrajDeleteTask)
	ON_BN_CLICKED(IDC_BTN_TRAJ_CLEAR_JOINT, &CTrajectoryDlg::OnBnClickedBtnTrajClearJoint)
	ON_BN_CLICKED(IDC_BTN_TRAJ_DELETE_JOINT, &CTrajectoryDlg::OnBnClickedBtnTrajDeleteJoint)
	ON_BN_CLICKED(IDC_BTN_TRAJ_APPLY_JOINT, &CTrajectoryDlg::OnBnClickedBtnTrajApplyJoint)
	ON_BN_CLICKED(IDC_BTN_TRAJ_UP_JOINT, &CTrajectoryDlg::OnBnClickedBtnTrajUpJoint)
	ON_BN_CLICKED(IDC_BTN_TRAJ_DOWN_JOINT, &CTrajectoryDlg::OnBnClickedBtnTrajDownJoint)

	ON_BN_CLICKED(IDC_BTN_TRAJ_APPLY_TASK, &CTrajectoryDlg::OnBnClickedBtnTrajApplyTask)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_TRAJ_TASK, &CTrajectoryDlg::OnLvnItemchangedListTrajTask)
	ON_BN_CLICKED(IDC_BTN_TRAJ_UP_TASK, &CTrajectoryDlg::OnBnClickedBtnTrajUpTask)
	ON_BN_CLICKED(IDC_BTN_TRAJ_DOWN_TASK, &CTrajectoryDlg::OnBnClickedBtnTrajDownTask)
	ON_NOTIFY(NM_CLICK, IDC_LIST_TRAJ_JOINT, &CTrajectoryDlg::OnNMClickListTrajJoint)
	ON_BN_CLICKED(IDC_CHECK_LIMIT_JOINT, &CTrajectoryDlg::OnBnClickedCheckLimitJoint)
	ON_BN_CLICKED(IDC_BTN_TRAJ_TASK_GO, &CTrajectoryDlg::OnBnClickedBtnTrajTaskGo)
	ON_BN_CLICKED(IDC_BTN_TRAJ_TASK_SET_POSTURE, &CTrajectoryDlg::OnBnClickedBtnTrajTaskSetPosture)
	ON_BN_CLICKED(IDC_BTN_TRAJ_JOINT_GO, &CTrajectoryDlg::OnBnClickedBtnTrajJointGo)
	ON_BN_CLICKED(IDC_BTN_TRAJ_JOINT_SET_POSTURE, &CTrajectoryDlg::OnBnClickedBtnTrajJointSetPosture)
	ON_BN_CLICKED(IDC_BTN_TRAJ_RUN_TASK, &CTrajectoryDlg::OnBnClickedBtnTrajRunTask)
	ON_BN_CLICKED(IDC_BTN_TRAJ_RUN_JOINT, &CTrajectoryDlg::OnBnClickedBtnTrajRunJoint)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDCANCEL, &CTrajectoryDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CTrajectoryDlg message handlers

void CTrajectoryDlg::OnBnClickedChkTrajTask()
{
	if (TRUE == ((CButton*)GetDlgItem(IDC_CHK_TRAJ_TASK))->GetCheck())
		SetTrajType(eTrajType_TASK);
	else
		SetTrajType(eTrajType_JOINT);
}

void CTrajectoryDlg::OnBnClickedChkTrajJoint()
{
	if (TRUE == ((CButton*)GetDlgItem(IDC_CHK_TRAJ_JOINT))->GetCheck())
		SetTrajType(eTrajType_JOINT);
	else
		SetTrajType(eTrajType_TASK);
}

BOOL CTrajectoryDlg::OnInitDialog()
{
	CScrollDlg::OnInitDialog();

	m_chkEnableJointLimit.SetCheck(ms_bEnableJointLimit);

	SetTrajType(m_trajType);

	int nCol = 0;
	m_listCtrlTrajTask.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_listCtrlTrajTask.InsertColumn(nCol++, _T("x(mm)"), LVCFMT_LEFT, 55, -1);
	m_listCtrlTrajTask.InsertColumn(nCol++, _T("y(mm)"), LVCFMT_LEFT, 55, -1);
	m_listCtrlTrajTask.InsertColumn(nCol++, _T("z(mm)"), LVCFMT_LEFT, 55, -1);
	m_listCtrlTrajTask.InsertColumn(nCol++, _T("Roll(deg)"), LVCFMT_LEFT, 65, -1);
	m_listCtrlTrajTask.InsertColumn(nCol++, _T("Pitch(deg)"), LVCFMT_LEFT, 65, -1);
	m_listCtrlTrajTask.InsertColumn(nCol++, _T("Yaw(deg)"), LVCFMT_LEFT, 65, -1);
	m_listCtrlTrajTask.InsertColumn(nCol++, _T("xg(mm)"), LVCFMT_LEFT, 55, -1);
	m_listCtrlTrajTask.InsertColumn(nCol++, _T("yg(mm)"), LVCFMT_LEFT, 55, -1);
	m_listCtrlTrajTask.InsertColumn(nCol++, _T("zg(mm)"), LVCFMT_LEFT, 55, -1);
	m_listCtrlTrajTask.InsertColumn(nCol++, _T("thg(deg)"), LVCFMT_LEFT, 55, -1);
	m_listCtrlTrajTask.InsertColumn(nCol++, _T("DO"), LVCFMT_LEFT, 40, -1);
	m_listCtrlTrajTask.InsertColumn(nCol++, _T("Duration(sec)"), LVCFMT_LEFT, 80, -1);

	//m_listCtrlTrajTask.SetReadOnlyColumns(0);
	//m_listCtrlTrajTask.SetComboColumns(1);
	//CString strValidChars = _T("0123456789.-");
	//m_listCtrlTrajTask.SetValidEditCtrlCharacters(strValidChars);
	//m_listCtrlTrajTask.EnableVScroll(); 
	//m_listCtrlTrajTask.SetExtendedStyle(LVS_EX_FULLROWSELECT);


	nCol = 0;
	m_listCtrlTrajJoint.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_listCtrlTrajJoint.InsertColumn(nCol++, _T("Joint1(deg)"), LVCFMT_LEFT, 60, -1);
	m_listCtrlTrajJoint.InsertColumn(nCol++, _T("Joint2(deg)"), LVCFMT_LEFT, 60, -1);
	m_listCtrlTrajJoint.InsertColumn(nCol++, _T("Joint3(deg)"), LVCFMT_LEFT, 60, -1);
	m_listCtrlTrajJoint.InsertColumn(nCol++, _T("Joint4(deg)"), LVCFMT_LEFT, 60, -1);
	m_listCtrlTrajJoint.InsertColumn(nCol++, _T("Joint5(deg)"), LVCFMT_LEFT, 60, -1);
	m_listCtrlTrajJoint.InsertColumn(nCol++, _T("Joint6(deg)"), LVCFMT_LEFT, 60, -1);
	m_listCtrlTrajJoint.InsertColumn(nCol++, _T("Joint7(deg)"), LVCFMT_LEFT, 60, -1);
	m_listCtrlTrajJoint.InsertColumn(nCol++, _T("Joint8(deg)"), LVCFMT_LEFT, 60, -1);
	m_listCtrlTrajJoint.InsertColumn(nCol++, _T("Joint9(deg)"), LVCFMT_LEFT, 60, -1);
	m_listCtrlTrajJoint.InsertColumn(nCol++, _T("Joint10(deg)"), LVCFMT_LEFT, 60, -1);
	m_listCtrlTrajJoint.InsertColumn(nCol++, _T("Joint11(deg)"), LVCFMT_LEFT, 60, -1);
	m_listCtrlTrajJoint.InsertColumn(nCol++, _T("Joint12(deg)"), LVCFMT_LEFT, 60, -1);
	m_listCtrlTrajJoint.InsertColumn(nCol++, _T("Joint13(deg)"), LVCFMT_LEFT, 60, -1);
	m_listCtrlTrajJoint.InsertColumn(nCol++, _T("Joint14(deg)"), LVCFMT_LEFT, 60, -1);
	m_listCtrlTrajJoint.InsertColumn(nCol++, _T("Joint15(deg)"), LVCFMT_LEFT, 60, -1);
	m_listCtrlTrajJoint.InsertColumn(nCol++, _T("Joint16(deg)"), LVCFMT_LEFT, 60, -1);
	m_listCtrlTrajJoint.InsertColumn(nCol++, _T("DO"), LVCFMT_LEFT, 40, -1);
	m_listCtrlTrajJoint.InsertColumn(nCol++, _T("Duration(sec)"), LVCFMT_LEFT, 80, -1);

	UpdateTraj();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CTrajectoryDlg::SetTrajType(eTrajType type)
{
	switch (type)
	{
	case eTrajType_TASK:
		((CButton*)GetDlgItem(IDC_CHK_TRAJ_TASK))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_CHK_TRAJ_JOINT))->SetCheck(FALSE);
		//GetDlgItem(IDC_LIST_TRAJ_TASK)->EnableWindow(TRUE);
		//GetDlgItem(IDC_LIST_TRAJ_JOINT)->EnableWindow(FALSE);

		break;
	case eTrajType_JOINT:
		((CButton*)GetDlgItem(IDC_CHK_TRAJ_TASK))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_CHK_TRAJ_JOINT))->SetCheck(TRUE);
		//GetDlgItem(IDC_LIST_TRAJ_TASK)->EnableWindow(FALSE);
		//GetDlgItem(IDC_LIST_TRAJ_JOINT)->EnableWindow(TRUE);
		break;
	default:
		return;
	}
	m_trajType = type;
}

void CTrajectoryDlg::OnDeleteKeyPressed(CWnd* pWnd)
{
	if (pWnd == &m_listCtrlTrajJoint)
	{
		POSITION pos = m_listCtrlTrajJoint.GetFirstSelectedItemPosition();
		if (pos != NULL)
		{
			int selected = m_listCtrlTrajJoint.GetNextSelectedItem(pos);
			if (selected != -1)
			{
				m_listCtrlTrajJoint.DeleteItem(selected);
				DeleteTrajJoint(selected);
				if (selected < m_listCtrlTrajJoint.GetItemCount())
					m_listCtrlTrajJoint.SetItemState(selected, LVIS_SELECTED, LVIS_SELECTED);
				else if (selected > 0 && selected == m_listCtrlTrajJoint.GetItemCount())
					m_listCtrlTrajJoint.SetItemState(selected-1, LVIS_SELECTED, LVIS_SELECTED);
			}
		}
	}
	else if (pWnd == &m_listCtrlTrajTask)
	{
		POSITION pos = m_listCtrlTrajTask.GetFirstSelectedItemPosition();
		if (pos != NULL)
		{
			int selected = m_listCtrlTrajTask.GetNextSelectedItem(pos);
			if (selected != -1)
			{
				m_listCtrlTrajTask.DeleteItem(selected);
				DeleteTrajTask(selected);
				if (selected < m_listCtrlTrajTask.GetItemCount())
					m_listCtrlTrajTask.SetItemState(selected, LVIS_SELECTED, LVIS_SELECTED);
				else if (selected > 0 && selected == m_listCtrlTrajTask.GetItemCount())
					m_listCtrlTrajTask.SetItemState(selected-1, LVIS_SELECTED, LVIS_SELECTED);
			}
		}
	}
}

void CTrajectoryDlg::OnBnClickedBtnTrajAddTask()
{
	UpdateData(TRUE);

	m_strTaskXg.Trim();
	m_strTaskYg.Trim();
	m_strTaskZg.Trim();
	m_strTaskThg.Trim();
	m_strTaskX.Trim();
	m_strTaskY.Trim();
	m_strTaskZ.Trim();
	m_strTaskRoll.Trim();
	m_strTaskPitch.Trim();
	m_strTaskYaw.Trim();
	m_strTaskDO.Trim();
	m_strTaskDuration.Trim();
	if (m_strTaskXg.IsEmpty() || m_strTaskYg.IsEmpty() || m_strTaskZg.IsEmpty() || m_strTaskThg.IsEmpty() ||
		m_strTaskX.IsEmpty() || m_strTaskY.IsEmpty() || m_strTaskZ.IsEmpty() ||
		m_strTaskRoll.IsEmpty() || m_strTaskPitch.IsEmpty() || m_strTaskYaw.IsEmpty() ||
		m_strTaskDO.IsEmpty() ||
		m_strTaskDuration.IsEmpty())
	{
		::AfxMessageBox(_T("Not enough parameters."));
		return;
	}

	rPanelManipulatorTaskTarget_t traj;
	traj.xg = _tstof(m_strTaskXg) * 0.001;
	traj.yg = _tstof(m_strTaskYg) * 0.001;
	traj.zg = _tstof(m_strTaskZg) * 0.001;
	traj.thg = D2R(_tstof(m_strTaskThg));
	traj.x = _tstof(m_strTaskX) * 0.001;
	traj.y = _tstof(m_strTaskY) * 0.001;
	traj.z = _tstof(m_strTaskZ) * 0.001;
	traj.roll = D2R(_tstof(m_strTaskRoll));
	traj.pitch = D2R(_tstof(m_strTaskPitch));
	traj.yaw = D2R(_tstof(m_strTaskYaw));
	traj.DO = _tstoi(m_strTaskDO);
	traj.duration = _tstof(m_strTaskDuration);
	if (traj.duration < 1e-3)
	{
		::AfxMessageBox(_T("Duration must be greater than 0"));
		return;
	}

	m_listTrajTask.push_back(traj);

	int index = m_listCtrlTrajTask.GetItemCount();
	int index_sub = 0;
	m_listCtrlTrajTask.InsertItem(index, 0, 0);
	m_listCtrlTrajTask.SetItemText(index, index_sub++, m_strTaskX);
	m_listCtrlTrajTask.SetItemText(index, index_sub++, m_strTaskY);
	m_listCtrlTrajTask.SetItemText(index, index_sub++, m_strTaskZ);
	m_listCtrlTrajTask.SetItemText(index, index_sub++, m_strTaskRoll);
	m_listCtrlTrajTask.SetItemText(index, index_sub++, m_strTaskPitch);
	m_listCtrlTrajTask.SetItemText(index, index_sub++, m_strTaskYaw);
	m_listCtrlTrajTask.SetItemText(index, index_sub++, m_strTaskXg);
	m_listCtrlTrajTask.SetItemText(index, index_sub++, m_strTaskYg);
	m_listCtrlTrajTask.SetItemText(index, index_sub++, m_strTaskZg);
	m_listCtrlTrajTask.SetItemText(index, index_sub++, m_strTaskThg);
	m_listCtrlTrajTask.SetItemText(index, index_sub++, m_strTaskDO);
	m_listCtrlTrajTask.SetItemText(index, index_sub++, m_strTaskDuration);
}

void CTrajectoryDlg::OnBnClickedBtnTrajAddJoint()
{
	UpdateData(TRUE);

	m_strJoint1.Trim();
	m_strJoint2.Trim();
	m_strJoint3.Trim();
	m_strJoint4.Trim();
	m_strJoint5.Trim();
	m_strJoint6.Trim();
	m_strJoint7.Trim();
	m_strJoint8.Trim();
	m_strJoint9.Trim();
	m_strJoint10.Trim();
	m_strJoint11.Trim();
	m_strJoint12.Trim();
	m_strJoint13.Trim();
	m_strJoint14.Trim();
	m_strJoint15.Trim();
	m_strJoint16.Trim();
	m_strJointDO.Trim();
	m_strJointDuration.Trim();
	if (m_strJoint1.IsEmpty() || m_strJoint2.IsEmpty() || m_strJoint3.IsEmpty() ||
		m_strJoint4.IsEmpty() || m_strJoint5.IsEmpty() || m_strJoint6.IsEmpty() ||
		m_strJoint7.IsEmpty() || m_strJoint8.IsEmpty() || m_strJoint9.IsEmpty() || 
		m_strJoint10.IsEmpty() || m_strJoint11.IsEmpty() || m_strJoint12.IsEmpty() || 
		m_strJoint13.IsEmpty() || m_strJoint14.IsEmpty() || m_strJoint15.IsEmpty() || 
		m_strJoint16.IsEmpty() || 
		m_strJointDO.IsEmpty() ||
		m_strJointDuration.IsEmpty())
	{
		::AfxMessageBox(_T("Not enough parameters."));
		return;
	}

	rPanelManipulatorJointTarget_t traj;
	traj.joint_rad[0] = D2R(_tstof(m_strJoint1));
	traj.joint_rad[1] = D2R(_tstof(m_strJoint2));
	traj.joint_rad[2] = D2R(_tstof(m_strJoint3));
	traj.joint_rad[3] = D2R(_tstof(m_strJoint4));
	traj.joint_rad[4] = D2R(_tstof(m_strJoint5));
	traj.joint_rad[5] = D2R(_tstof(m_strJoint6));
	traj.joint_rad[6] = D2R(_tstof(m_strJoint7));
	traj.joint_rad[7] = D2R(_tstof(m_strJoint8));
	traj.joint_rad[8] = D2R(_tstof(m_strJoint9));
	traj.joint_rad[9] = D2R(_tstof(m_strJoint10));
	traj.joint_rad[10] = D2R(_tstof(m_strJoint11));
	traj.joint_rad[11] = D2R(_tstof(m_strJoint12));
	traj.joint_rad[12] = D2R(_tstof(m_strJoint13));
	traj.joint_rad[13] = D2R(_tstof(m_strJoint14));
	traj.joint_rad[14] = D2R(_tstof(m_strJoint15));
	traj.joint_rad[15] = D2R(_tstof(m_strJoint16));
	traj.DO = _tstoi(m_strJointDO);

	for (int i=0; i<16; i++)
	{
		if (ms_bEnableJointLimit && (traj.joint_rad[i] < m_jointParam.pos_lbound[i] || traj.joint_rad[i] > m_jointParam.pos_ubound[i]))
		{
			TCHAR tmp[256];
			_stprintf_s(tmp, 256, _T("Joint limit check failed for joint%d"), i+1);
			::AfxMessageBox(tmp);
			return;
		}
	}

	traj.duration = _tstof(m_strJointDuration);
	if (traj.duration < 1e-3)
	{
		::AfxMessageBox(_T("Duration must be greater than 0"));
		return;
	}

	m_listTrajJoint.push_back(traj);

	int index = m_listCtrlTrajJoint.GetItemCount();
	int index_sub = 0;
	
	_isAdd = true;
	
	m_listCtrlTrajJoint.InsertItem(index, 0, 0);
	m_listCtrlTrajJoint.SetItemText(index, index_sub++, m_strJoint1);
	m_listCtrlTrajJoint.SetItemText(index, index_sub++, m_strJoint2);
	m_listCtrlTrajJoint.SetItemText(index, index_sub++, m_strJoint3);
	m_listCtrlTrajJoint.SetItemText(index, index_sub++, m_strJoint4);
	m_listCtrlTrajJoint.SetItemText(index, index_sub++, m_strJoint5);
	m_listCtrlTrajJoint.SetItemText(index, index_sub++, m_strJoint6);
	m_listCtrlTrajJoint.SetItemText(index, index_sub++, m_strJoint7);
	m_listCtrlTrajJoint.SetItemText(index, index_sub++, m_strJoint8);
	m_listCtrlTrajJoint.SetItemText(index, index_sub++, m_strJoint9);
	m_listCtrlTrajJoint.SetItemText(index, index_sub++, m_strJoint10);
	m_listCtrlTrajJoint.SetItemText(index, index_sub++, m_strJoint11);
	m_listCtrlTrajJoint.SetItemText(index, index_sub++, m_strJoint12);
	m_listCtrlTrajJoint.SetItemText(index, index_sub++, m_strJoint13);
	m_listCtrlTrajJoint.SetItemText(index, index_sub++, m_strJoint14);
	m_listCtrlTrajJoint.SetItemText(index, index_sub++, m_strJoint15);
	m_listCtrlTrajJoint.SetItemText(index, index_sub++, m_strJoint16);
	m_listCtrlTrajJoint.SetItemText(index, index_sub++, m_strJointDO);
	m_listCtrlTrajJoint.SetItemText(index, index_sub++, m_strJointDuration);

	_isAdd = false;
}

void CTrajectoryDlg::DeleteTrajJoint(int index)
{
	list<rPanelManipulatorJointTarget_t>::iterator itr;
	itr = m_listTrajJoint.begin();
	while (itr != m_listTrajJoint.end() && index > 0)
	{
		itr++;
		index--;
	}
	if (itr != m_listTrajJoint.end())
		m_listTrajJoint.erase(itr);
}

void CTrajectoryDlg::DeleteTrajTask(int index)
{
	list<rPanelManipulatorTaskTarget_t>::iterator itr;
	itr = m_listTrajTask.begin();
	while (itr != m_listTrajTask.end() && index > 0)
	{
		itr++;
		index--;
	}
	if (itr != m_listTrajTask.end())
		m_listTrajTask.erase(itr);
}

void CTrajectoryDlg::DeleteAllTrajJoint()
{
	list<rPanelManipulatorJointTarget_t>::iterator itr;
	itr = m_listTrajJoint.begin();
	while (itr != m_listTrajJoint.end())
	{
		m_listTrajJoint.erase(itr);
		itr = m_listTrajJoint.begin();
	}
}

void CTrajectoryDlg::DeleteAllTrajTask()
{
	list<rPanelManipulatorTaskTarget_t>::iterator itr;
	itr = m_listTrajTask.begin();
	while (itr != m_listTrajTask.end())
	{
		m_listTrajTask.erase(itr);
		itr = m_listTrajTask.begin();
	}
}

void CTrajectoryDlg::UpdateTraj()
{
	m_listCtrlTrajJoint.DeleteAllItems();
	m_listCtrlTrajTask.DeleteAllItems();


	list<rPanelManipulatorJointTarget_t>::const_iterator joint = m_listTrajJoint.begin();
	while (joint != m_listTrajJoint.end())
	{
		int index = m_listCtrlTrajJoint.GetItemCount();
		int index_sub = 0;
		CString val;

		m_listCtrlTrajJoint.InsertItem(index, 0, 0);

		val.Format(_T("%.1f"), R2D((*joint).joint_rad[0]));
		m_listCtrlTrajJoint.SetItemText(index, index_sub++, val);

		val.Format(_T("%.1f"), R2D((*joint).joint_rad[1]));
		m_listCtrlTrajJoint.SetItemText(index, index_sub++, val);

		val.Format(_T("%.1f"), R2D((*joint).joint_rad[2]));
		m_listCtrlTrajJoint.SetItemText(index, index_sub++, val);

		val.Format(_T("%.1f"), R2D((*joint).joint_rad[3]));
		m_listCtrlTrajJoint.SetItemText(index, index_sub++, val);

		val.Format(_T("%.1f"), R2D((*joint).joint_rad[4]));
		m_listCtrlTrajJoint.SetItemText(index, index_sub++, val);

		val.Format(_T("%.1f"), R2D((*joint).joint_rad[5]));
		m_listCtrlTrajJoint.SetItemText(index, index_sub++, val);

		val.Format(_T("%.1f"), R2D((*joint).joint_rad[6]));
		m_listCtrlTrajJoint.SetItemText(index, index_sub++, val);

		val.Format(_T("%.1f"), R2D((*joint).joint_rad[7]));
		m_listCtrlTrajJoint.SetItemText(index, index_sub++, val);

		val.Format(_T("%.1f"), R2D((*joint).joint_rad[8]));
		m_listCtrlTrajJoint.SetItemText(index, index_sub++, val);

		val.Format(_T("%.1f"), R2D((*joint).joint_rad[9]));
		m_listCtrlTrajJoint.SetItemText(index, index_sub++, val);

		val.Format(_T("%.1f"), R2D((*joint).joint_rad[10]));
		m_listCtrlTrajJoint.SetItemText(index, index_sub++, val);

		val.Format(_T("%.1f"), R2D((*joint).joint_rad[11]));
		m_listCtrlTrajJoint.SetItemText(index, index_sub++, val);

		val.Format(_T("%.1f"), R2D((*joint).joint_rad[12]));
		m_listCtrlTrajJoint.SetItemText(index, index_sub++, val);

		val.Format(_T("%.1f"), R2D((*joint).joint_rad[13]));
		m_listCtrlTrajJoint.SetItemText(index, index_sub++, val);

		val.Format(_T("%.1f"), R2D((*joint).joint_rad[14]));
		m_listCtrlTrajJoint.SetItemText(index, index_sub++, val);

		val.Format(_T("%.1f"), R2D((*joint).joint_rad[15]));
		m_listCtrlTrajJoint.SetItemText(index, index_sub++, val);

		val.Format(_T("%d"), (*joint).DO);
		m_listCtrlTrajJoint.SetItemText(index, index_sub++, val);

		val.Format(_T("%.1f"), (*joint).duration);
		m_listCtrlTrajJoint.SetItemText(index, index_sub++, val);

		joint++;
	}
	
	list<rPanelManipulatorTaskTarget_t>::const_iterator task = m_listTrajTask.begin();
	while (task != m_listTrajTask.end())
	{
		int index = m_listCtrlTrajTask.GetItemCount();
		int index_sub = 0;
		CString val;

		m_listCtrlTrajTask.InsertItem(index, 0, 0);
		
		val.Format(_T("%d"), (int)((*task).x * 1000));
		m_listCtrlTrajTask.SetItemText(index, index_sub++, val);

		val.Format(_T("%d"), (int)((*task).y * 1000));
		m_listCtrlTrajTask.SetItemText(index, index_sub++, val);

		val.Format(_T("%d"), (int)((*task).z * 1000));
		m_listCtrlTrajTask.SetItemText(index, index_sub++, val);

		val.Format(_T("%.1f"), R2D((*task).roll));
		m_listCtrlTrajTask.SetItemText(index, index_sub++, val);

		val.Format(_T("%.1f"), R2D((*task).pitch));
		m_listCtrlTrajTask.SetItemText(index, index_sub++, val);

		val.Format(_T("%.1f"), R2D((*task).yaw));
		m_listCtrlTrajTask.SetItemText(index, index_sub++, val);

		val.Format(_T("%d"), (int)((*task).xg * 1000));
		m_listCtrlTrajTask.SetItemText(index, index_sub++, val);

		val.Format(_T("%d"), (int)((*task).yg * 1000));
		m_listCtrlTrajTask.SetItemText(index, index_sub++, val);

		val.Format(_T("%d"), (int)((*task).zg * 1000));
		m_listCtrlTrajTask.SetItemText(index, index_sub++, val);

		val.Format(_T("%.1f"), R2D((*task).thg));
		m_listCtrlTrajTask.SetItemText(index, index_sub++, val);

		val.Format(_T("%d"), (int)((*task).DO));
		m_listCtrlTrajTask.SetItemText(index, index_sub++, val);
		
		val.Format(_T("%.1f"), (*task).duration);
		m_listCtrlTrajTask.SetItemText(index, index_sub++, val);

		task++;	
	}
	UpdateData(FALSE);
}

void CTrajectoryDlg::OnBnClickedOk()
{
	_pSHM->traj.type = m_trajType;

	_pSHM->traj.num_task_traj = m_listTrajTask.size();
	list<rPanelManipulatorTaskTarget_t>::const_iterator task = m_listTrajTask.begin();
	for (unsigned int i=0; i<_pSHM->traj.num_task_traj && task!=m_listTrajTask.end(); i++, task++)
		_pSHM->traj.task_traj[i] = *task;

	_pSHM->traj.num_joint_traj = m_listTrajJoint.size();
	list<rPanelManipulatorJointTarget_t>::const_iterator joint = m_listTrajJoint.begin();
	for (unsigned int i=0; i<_pSHM->traj.num_joint_traj && joint!=m_listTrajJoint.end(); i++, joint++)
		_pSHM->traj.joint_traj[i] = *joint;
	//OnOK();
}

void CTrajectoryDlg::OnTrajectoryDialogFileNew()
{
	SetTrajType(eTrajType_TASK);
	DeleteAllTrajJoint();
	DeleteAllTrajTask();
	UpdateTraj();
	m_strTaskXg = _T("0");
	m_strTaskYg = _T("0");
	m_strTaskZg = _T("0");
	m_strTaskThg = _T("0");
	m_strTaskX = _T("0");
	m_strTaskY = _T("0");
	m_strTaskZ = _T("0");
	m_strTaskRoll = _T("0");
	m_strTaskPitch = _T("0");
	m_strTaskYaw = _T("0");
	m_strTaskDO = _T("0");
	m_strTaskDuration = _T("0");
	m_strJoint1 = _T("0");
	m_strJoint2 = _T("0");
	m_strJoint3 = _T("0");
	m_strJoint4 = _T("0");
	m_strJoint5 = _T("0");
	m_strJoint6 = _T("0");
	m_strJoint7 = _T("0");
	m_strJoint8 = _T("0");
	m_strJoint9 = _T("0");
	m_strJoint10 = _T("0");
	m_strJoint11 = _T("0");
	m_strJoint12 = _T("0");
	m_strJoint13 = _T("0");
	m_strJoint14 = _T("0");
	m_strJoint15 = _T("0");
	m_strJoint16 = _T("0");
	m_strJointDO = _T("0");
	m_strJointDuration = _T("0");
	UpdateData(FALSE);
}

void CTrajectoryDlg::OnTrajectoryDialogFileOpen()
{
	m_strTaskXg = _T("0");
	m_strTaskYg = _T("0");
	m_strTaskZg = _T("0");
	m_strTaskThg = _T("0");
	m_strTaskX = _T("0");	
	m_strTaskY = _T("0");
	m_strTaskZ = _T("0");
	m_strTaskRoll = _T("0");
	m_strTaskPitch = _T("0");
	m_strTaskYaw = _T("0");
	m_strTaskDO = _T("0");
	m_strTaskDuration = _T("0");
	m_strJoint1 = _T("0");
	m_strJoint2 = _T("0");
	m_strJoint3 = _T("0");
	m_strJoint4 = _T("0");
	m_strJoint5 = _T("0");
	m_strJoint6 = _T("0");
	m_strJoint7 = _T("0");
	m_strJoint8 = _T("0");
	m_strJoint9 = _T("0");
	m_strJoint10 = _T("0");
	m_strJoint11 = _T("0");
	m_strJoint12 = _T("0");
	m_strJoint13 = _T("0");
	m_strJoint14 = _T("0");
	m_strJoint15 = _T("0");
	m_strJoint16 = _T("0");
	m_strJointDO = _T("0");
	m_strJointDuration = _T("0");
	CFileDialog dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST, _T("Trajectory(*.rtrj)|*rtrj||"), NULL);

	dlg.m_ofn.lpstrTitle = _T("Open File");
	dlg.m_ofn.lStructSize = sizeof(OPENFILENAME);
	if (dlg.DoModal() == IDOK)
	{
		CString str = dlg.GetPathName();
		HANDLE hFile;
		DWORD dwRead;
		hFile = CreateFile(str, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (INVALID_HANDLE_VALUE != hFile)
		{
			DeleteAllTrajJoint();
			DeleteAllTrajTask();
			ReadFile(hFile, &m_trajType, sizeof(m_trajType), &dwRead, NULL);
			
			int count;
			ReadFile(hFile, &count, sizeof(count), &dwRead, NULL);
			rPanelManipulatorJointTarget_t joint;
			while (count > 0)
			{
				ReadFile(hFile, &joint, sizeof(rPanelManipulatorJointTarget_t), &dwRead, NULL);
				m_listTrajJoint.push_back(joint);
				count--;
			}

			ReadFile(hFile, &count, sizeof(count), &dwRead, NULL);
			rPanelManipulatorTaskTarget_t task;
			while (count > 0)
			{
				ReadFile(hFile, &task, sizeof(rPanelManipulatorTaskTarget_t), &dwRead, NULL);
				m_listTrajTask.push_back(task);
				count--;
			}

			SetTrajType(m_trajType);
			UpdateTraj();

			CloseHandle(hFile);
		}
	}
}

void CTrajectoryDlg::OnTrajectoryDialogFileSave()
{
}

void CTrajectoryDlg::OnTrajectoryDialogFileSaveas()
{
	CFileDialog dlg(FALSE, NULL, NULL, OFN_FILEMUSTEXIST, _T("Trajectory(*.rtrj)|*rtrj||"), NULL);

	dlg.m_ofn.lpstrTitle = _T("Save File");
	dlg.m_ofn.lStructSize = sizeof(OPENFILENAME);

	if (dlg.DoModal() == IDOK)
	{
		CString str = dlg.GetPathName();
		if (0 != _tcscmp(_T("rtrj"), dlg.GetFileExt()))
			str = str + _T(".rtrj");

		HANDLE hFile;
		DWORD dwWritten;
		hFile = CreateFile(str, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		WriteFile(hFile, &m_trajType, sizeof(eTrajType), &dwWritten, NULL);

		int count;
		count = m_listTrajJoint.size();
		WriteFile(hFile, &count, sizeof(int), &dwWritten, NULL);
		list<rPanelManipulatorJointTarget_t>::const_iterator joint = m_listTrajJoint.begin();
		while (joint != m_listTrajJoint.end())
		{
			WriteFile(hFile, &(*joint), sizeof(rPanelManipulatorJointTarget_t), &dwWritten, NULL);
			joint++;
		}

		count = m_listTrajTask.size();
		WriteFile(hFile, &count, sizeof(int), &dwWritten, NULL);
		list<rPanelManipulatorTaskTarget_t>::const_iterator task = m_listTrajTask.begin();
		while (task != m_listTrajTask.end())
		{
			WriteFile(hFile, &(*task), sizeof(rPanelManipulatorTaskTarget_t), &dwWritten, NULL);
			task++;
		}

		CloseHandle(hFile);
	}
}

void CTrajectoryDlg::OnBnClickedBtnTrajClearTask()
{
	DeleteAllTrajTask();
	UpdateTraj();
}

void CTrajectoryDlg::OnBnClickedBtnTrajDeleteTask()
{
	POSITION pos = m_listCtrlTrajTask.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		int selected = m_listCtrlTrajTask.GetNextSelectedItem(pos);
		if (selected != -1)
		{
			m_listCtrlTrajTask.DeleteItem(selected);
			DeleteTrajTask(selected);
			if (selected < m_listCtrlTrajTask.GetItemCount())
				m_listCtrlTrajTask.SetItemState(selected, LVIS_SELECTED, LVIS_SELECTED);
			else if (selected > 0 && selected == m_listCtrlTrajTask.GetItemCount())
				m_listCtrlTrajTask.SetItemState(selected-1, LVIS_SELECTED, LVIS_SELECTED);
		}
	}
}

void CTrajectoryDlg::OnBnClickedBtnTrajClearJoint()
{
	DeleteAllTrajJoint();
	UpdateTraj();
}

void CTrajectoryDlg::OnBnClickedBtnTrajDeleteJoint()
{
	POSITION pos = m_listCtrlTrajJoint.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		int selected = m_listCtrlTrajJoint.GetNextSelectedItem(pos);
		if (selected != -1)
		{
			m_listCtrlTrajJoint.DeleteItem(selected);
			DeleteTrajJoint(selected);
			if (selected < m_listCtrlTrajJoint.GetItemCount())
				m_listCtrlTrajJoint.SetItemState(selected, LVIS_SELECTED, LVIS_SELECTED);
			else if (selected > 0 && selected == m_listCtrlTrajJoint.GetItemCount())
				m_listCtrlTrajJoint.SetItemState(selected-1, LVIS_SELECTED, LVIS_SELECTED);
		}
	}
}

void CTrajectoryDlg::OnBnClickedBtnTrajApplyJoint()
{
	UpdateData(TRUE);

	m_strJoint1.Trim();
	m_strJoint2.Trim();
	m_strJoint3.Trim();
	m_strJoint4.Trim();
	m_strJoint5.Trim();
	m_strJoint6.Trim();
	m_strJoint7.Trim();
	m_strJoint8.Trim();
	m_strJoint9.Trim();
	m_strJoint10.Trim();
	m_strJoint11.Trim();
	m_strJoint12.Trim();
	m_strJoint13.Trim();
	m_strJoint14.Trim();
	m_strJoint15.Trim();
	m_strJoint16.Trim();
	m_strJointDO.Trim();
	m_strJointDuration.Trim();

	POSITION pos = m_listCtrlTrajJoint.GetFirstSelectedItemPosition();
	if(!pos)
		return;

	int index = m_listCtrlTrajJoint.GetNextSelectedItem(pos);
	int index_sub = 0;
	_isAdd = true;
	
	list<rPanelManipulatorJointTarget_t>::iterator itrPMJ_t;

	int cnt = 0;

	for(itrPMJ_t = m_listTrajJoint.begin() ; itrPMJ_t != m_listTrajJoint.end() ; itrPMJ_t++)
	{
		cnt++;
		if(cnt > index)
			break;
	}
	itrPMJ_t->joint_rad[0] = D2R(_tstof(m_strJoint1));
	itrPMJ_t->joint_rad[1] = D2R(_tstof(m_strJoint2));
	itrPMJ_t->joint_rad[2] = D2R(_tstof(m_strJoint3));
	itrPMJ_t->joint_rad[3] = D2R(_tstof(m_strJoint4));
	itrPMJ_t->joint_rad[4] = D2R(_tstof(m_strJoint5));
	itrPMJ_t->joint_rad[5] = D2R(_tstof(m_strJoint6));
	itrPMJ_t->joint_rad[6] = D2R(_tstof(m_strJoint7));
	itrPMJ_t->joint_rad[7] = D2R(_tstof(m_strJoint8));
	itrPMJ_t->joint_rad[8] = D2R(_tstof(m_strJoint9));
	itrPMJ_t->joint_rad[9] = D2R(_tstof(m_strJoint10));
	itrPMJ_t->joint_rad[10] = D2R(_tstof(m_strJoint11));
	itrPMJ_t->joint_rad[11] = D2R(_tstof(m_strJoint12));
	itrPMJ_t->joint_rad[12] = D2R(_tstof(m_strJoint13));
	itrPMJ_t->joint_rad[13] = D2R(_tstof(m_strJoint14));
	itrPMJ_t->joint_rad[14] = D2R(_tstof(m_strJoint15));
	itrPMJ_t->joint_rad[15] = D2R(_tstof(m_strJoint16));
	itrPMJ_t->DO = _tstoi(m_strJointDO);
	itrPMJ_t->duration = _tstof(m_strJointDuration);

	m_listCtrlTrajJoint.SetItemText(index, index_sub++, m_strJoint1);
	m_listCtrlTrajJoint.SetItemText(index, index_sub++, m_strJoint2);
	m_listCtrlTrajJoint.SetItemText(index, index_sub++, m_strJoint3);
	m_listCtrlTrajJoint.SetItemText(index, index_sub++, m_strJoint4);
	m_listCtrlTrajJoint.SetItemText(index, index_sub++, m_strJoint5);
	m_listCtrlTrajJoint.SetItemText(index, index_sub++, m_strJoint6);
	m_listCtrlTrajJoint.SetItemText(index, index_sub++, m_strJoint7);
	m_listCtrlTrajJoint.SetItemText(index, index_sub++, m_strJoint8);
	m_listCtrlTrajJoint.SetItemText(index, index_sub++, m_strJoint9);
	m_listCtrlTrajJoint.SetItemText(index, index_sub++, m_strJoint10);
	m_listCtrlTrajJoint.SetItemText(index, index_sub++, m_strJoint11);
	m_listCtrlTrajJoint.SetItemText(index, index_sub++, m_strJoint12);
	m_listCtrlTrajJoint.SetItemText(index, index_sub++, m_strJoint13);
	m_listCtrlTrajJoint.SetItemText(index, index_sub++, m_strJoint14);
	m_listCtrlTrajJoint.SetItemText(index, index_sub++, m_strJoint15);
	m_listCtrlTrajJoint.SetItemText(index, index_sub++, m_strJoint16);
	m_listCtrlTrajJoint.SetItemText(index, index_sub++, m_strJointDO);
	m_listCtrlTrajJoint.SetItemText(index, index_sub++, m_strJointDuration);

	_isAdd = false;
	UpdateData(FALSE);
}

void CTrajectoryDlg::OnBnClickedBtnTrajUpJoint()
{
	POSITION pos = m_listCtrlTrajJoint.GetFirstSelectedItemPosition();
	if ((int)pos == 1 || (int)pos == 0)
		return;

	list<rPanelManipulatorJointTarget_t>::iterator left = m_listTrajJoint.begin();
	int index = 0;
	rPanelManipulatorJointTarget_t tmp1, tmp2;
	while (index++ < (int)pos-2)
		left++;
	tmp1 = *left;
	tmp2 = *(++left);
	*left = tmp1;
	*(--left) = tmp2;

	UpdateTraj();
	m_listCtrlTrajJoint.SetItemState(index-1, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
	m_listCtrlTrajJoint.EnsureVisible(index-1, TRUE);
	m_listCtrlTrajJoint.SetFocus();
}

void CTrajectoryDlg::OnBnClickedBtnTrajDownJoint()
{
	POSITION pos = m_listCtrlTrajJoint.GetFirstSelectedItemPosition();
	int count = m_listTrajJoint.size();
	if ((int)pos == count || (int)pos == 0)
		return;

	list<rPanelManipulatorJointTarget_t>::iterator left = m_listTrajJoint.begin();
	int index = 0;
	rPanelManipulatorJointTarget_t tmp1, tmp2;
	
	while (index++ < (int)pos-1)
		left++;
	tmp1 = *left;
	tmp2 = *(++left);
	*left = tmp1;
	*(--left) = tmp2;
	
	UpdateTraj();
	m_listCtrlTrajJoint.SetItemState(index, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
	m_listCtrlTrajJoint.EnsureVisible(index, TRUE);
	m_listCtrlTrajJoint.SetFocus();
}


void CTrajectoryDlg::OnBnClickedBtnTrajApplyTask()
{
	UpdateData(TRUE);

	m_strTaskXg.Trim();
	m_strTaskYg.Trim();
	m_strTaskZg.Trim();
	m_strTaskThg.Trim();
	m_strTaskX.Trim();
	m_strTaskY.Trim();
	m_strTaskZ.Trim();
	m_strTaskRoll.Trim();
	m_strTaskPitch.Trim();
	m_strTaskYaw.Trim();
	m_strTaskDO.Trim();
	m_strTaskDuration.Trim();

	POSITION pos = m_listCtrlTrajTask.GetFirstSelectedItemPosition();
	if(!pos)
		return;

	int index = m_listCtrlTrajTask.GetNextSelectedItem(pos);
	int index_sub = 0;
	_isAdd = true;
	
	list<rPanelManipulatorTaskTarget_t>::iterator itrPMJ_t;

	int cnt = 0;

	for(itrPMJ_t = m_listTrajTask.begin() ; itrPMJ_t != m_listTrajTask.end() ; itrPMJ_t++)
	{
		cnt++;
		if(cnt > index)
			break;
	}
	itrPMJ_t->xg = _tstof(m_strTaskXg) * 0.001; 
	itrPMJ_t->yg = _tstof(m_strTaskYg) * 0.001;
	itrPMJ_t->zg = _tstof(m_strTaskZg) * 0.001;
	itrPMJ_t->thg = D2R(_tstof(m_strTaskThg));
	itrPMJ_t->x = _tstof(m_strTaskX) * 0.001; 
	itrPMJ_t->y = _tstof(m_strTaskY) * 0.001;
	itrPMJ_t->z = _tstof(m_strTaskZ) * 0.001;
	itrPMJ_t->roll = D2R(_tstof(m_strTaskRoll));
	itrPMJ_t->pitch = D2R(_tstof(m_strTaskPitch));
	itrPMJ_t->yaw = D2R(_tstof(m_strTaskYaw));
	itrPMJ_t->DO = _tstoi(m_strTaskDO);
	itrPMJ_t->duration = _tstof(m_strTaskDuration);

	m_listCtrlTrajTask.SetItemText(index, index_sub++, m_strTaskX);
	m_listCtrlTrajTask.SetItemText(index, index_sub++, m_strTaskY);
	m_listCtrlTrajTask.SetItemText(index, index_sub++, m_strTaskZ);
	m_listCtrlTrajTask.SetItemText(index, index_sub++, m_strTaskRoll);
	m_listCtrlTrajTask.SetItemText(index, index_sub++, m_strTaskPitch);
	m_listCtrlTrajTask.SetItemText(index, index_sub++, m_strTaskYaw);
	m_listCtrlTrajTask.SetItemText(index, index_sub++, m_strTaskXg);
	m_listCtrlTrajTask.SetItemText(index, index_sub++, m_strTaskYg);
	m_listCtrlTrajTask.SetItemText(index, index_sub++, m_strTaskZg);
	m_listCtrlTrajTask.SetItemText(index, index_sub++, m_strTaskThg);
	m_listCtrlTrajTask.SetItemText(index, index_sub++, m_strTaskDO);
	m_listCtrlTrajTask.SetItemText(index, index_sub++, m_strTaskDuration);

	_isAdd = false;
	UpdateData(FALSE);
}

void CTrajectoryDlg::OnLvnItemchangedListTrajTask(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	NM_LISTVIEW*pNMView=(NM_LISTVIEW*)pNMHDR;
	int index = pNMView->iItem;
	*pResult = 0;
	if ((pNMView->uNewState & (LVIS_FOCUSED|LVIS_SELECTED)) == 0 )
		return;

	for (int i = 0; i < 12; ++i) 
	{
		CString strValue = m_listCtrlTrajTask.GetItemText(index, i);
		*(_strTaskArray[i]) = strValue;
	}

	UpdateData(FALSE);
}

void CTrajectoryDlg::OnBnClickedBtnTrajUpTask()
{
	POSITION pos = m_listCtrlTrajTask.GetFirstSelectedItemPosition();
	if ((int)pos == 1 || (int)pos == 0)
		return;

	list<rPanelManipulatorTaskTarget_t>::iterator left = m_listTrajTask.begin();
	int index = 0;
	rPanelManipulatorTaskTarget_t tmp1, tmp2;
	while (index++ < (int)pos-2)
		left++;
	tmp1 = *left;
	tmp2 = *(++left);
	*left = tmp1;
	*(--left) = tmp2;

	UpdateTraj();
	m_listCtrlTrajTask.SetItemState(index-1, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
	m_listCtrlTrajTask.EnsureVisible(index-1, TRUE);
	m_listCtrlTrajTask.SetFocus();
}

void CTrajectoryDlg::OnBnClickedBtnTrajDownTask()
{
	POSITION pos = m_listCtrlTrajTask.GetFirstSelectedItemPosition();
	int count = m_listTrajTask.size();
	if ((int)pos == count || (int)pos == 0)
		return;

	list<rPanelManipulatorTaskTarget_t>::iterator left = m_listTrajTask.begin();
	int index = 0;
	rPanelManipulatorTaskTarget_t tmp1, tmp2;
	while (index++ < (int)pos-1)
		left++;
	tmp1 = *left;
	tmp2 = *(++left);
	*left = tmp1;
	*(--left) = tmp2;

	UpdateTraj();
	m_listCtrlTrajTask.SetItemState(index, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
	m_listCtrlTrajTask.EnsureVisible(index, TRUE);
	m_listCtrlTrajTask.SetFocus();
}

void CTrajectoryDlg::OnNMClickListTrajJoint(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	NM_LISTVIEW*pNMView=(NM_LISTVIEW*)pNMHDR;
	int index = pNMView->iItem;
	*pResult = 0;
	if (_isAdd) return;

	for (int i = 0; i < 10; ++i) 
	{
		CString strValue = m_listCtrlTrajJoint.GetItemText(index, i);
		*(_strJointArray[i]) = strValue;
	}

	m_listCtrlTrajJoint.ModifyStyle(0, LVS_SHOWSELALWAYS);
	UpdateData(FALSE);

	*pResult = 0;
}

void CTrajectoryDlg::OnBnClickedCheckLimitJoint()
{
	ms_bEnableJointLimit = m_chkEnableJointLimit.GetCheck();
}

void CTrajectoryDlg::OnBnClickedBtnTrajTaskGo()
{
	((CButton*)GetDlgItem(IDC_CHK_TRAJ_TASK))->SetCheck(BST_CHECKED);
	((CButton*)GetDlgItem(IDC_CHK_TRAJ_JOINT))->SetCheck(BST_UNCHECKED);
	SetTrajType(eTrajType_TASK);

	UpdateData(TRUE);

	m_strTaskDuration.Trim();
	m_strTaskX.Trim();
	m_strTaskY.Trim();
	m_strTaskZ.Trim();
	m_strTaskRoll.Trim();
	m_strTaskPitch.Trim();
	m_strTaskYaw.Trim();
	
	if (m_strTaskDuration.IsEmpty() ||
		m_strTaskX.IsEmpty() || m_strTaskY.IsEmpty() || m_strTaskZ.IsEmpty() ||
		m_strTaskRoll.IsEmpty() || m_strTaskPitch.IsEmpty() || m_strTaskYaw.IsEmpty()
		)
	{
		::AfxMessageBox(_T("Not enough parameters."));
		return;
	}

	if(_tstof(m_strTaskDuration) <= 0)
	{
		AfxMessageBox(_T("Duration must be bigger than 0"));
		return;
	}

	_pSHM->traj.num_task_traj			= 1;
	_pSHM->traj.type					= eTrajType_TASK;
	_pSHM->traj.task_traj[0].duration	= _tstof(m_strTaskDuration);
	_pSHM->traj.task_traj[0].x			= _tstof(m_strTaskX) * 0.001;
	_pSHM->traj.task_traj[0].y			= _tstof(m_strTaskY) * 0.001;
	_pSHM->traj.task_traj[0].z			= _tstof(m_strTaskZ) * 0.001;
	_pSHM->traj.task_traj[0].roll		= D2R(_tstof(m_strTaskRoll));
	_pSHM->traj.task_traj[0].pitch		= D2R(_tstof(m_strTaskPitch));
	_pSHM->traj.task_traj[0].yaw		= D2R(_tstof(m_strTaskYaw));
	
	_pSHM->cmd.command					= CMD_GO;
}

void CTrajectoryDlg::OnBnClickedBtnTrajTaskSetPosture()
{
	UpdateData(TRUE);

	m_strTaskX.Format(_T("%f"), _pSHM->state.endeffector_state[0].x);
	m_strTaskY.Format(_T("%f"), _pSHM->state.endeffector_state[0].y);
	m_strTaskZ.Format(_T("%f"), _pSHM->state.endeffector_state[0].z);
	m_strTaskRoll.Format(_T("%f"), _pSHM->state.endeffector_state[0].roll);
	m_strTaskPitch.Format(_T("%f"), _pSHM->state.endeffector_state[0].pitch);
	m_strTaskYaw.Format(_T("%f"), _pSHM->state.endeffector_state[0].yaw);
	
	UpdateData(FALSE);
}

void CTrajectoryDlg::OnBnClickedBtnTrajJointGo()
{
	((CButton*)GetDlgItem(IDC_CHK_TRAJ_TASK))->SetCheck(BST_UNCHECKED);
	((CButton*)GetDlgItem(IDC_CHK_TRAJ_JOINT))->SetCheck(BST_CHECKED);
	SetTrajType(eTrajType_JOINT);

	UpdateData(TRUE);

	m_strJointDuration.Trim();
	m_strJoint1.Trim();
	m_strJoint2.Trim();
	m_strJoint3.Trim();
	m_strJoint4.Trim();
	m_strJoint5.Trim();
	m_strJoint6.Trim();
	m_strJoint7.Trim();
	m_strJoint8.Trim();
	m_strJoint9.Trim();
	m_strJoint10.Trim();
	m_strJoint11.Trim();
	m_strJoint12.Trim();
	m_strJoint13.Trim();
	m_strJoint14.Trim();
	m_strJoint15.Trim();
	m_strJoint16.Trim();
	
	if(_tstof(m_strJointDuration) <= 0)
	{
		AfxMessageBox(_T("Duration must be bigger than 0"));
		return;
	}

	_pSHM->traj.num_joint_traj				= 1;
	_pSHM->traj.type						= eTrajType_JOINT;
	_pSHM->traj.joint_traj[0].duration		= _tstof(m_strJointDuration);
	_pSHM->traj.joint_traj[0].joint_rad[0]	= D2R(_tstof(m_strJoint1));
	_pSHM->traj.joint_traj[0].joint_rad[1]	= D2R(_tstof(m_strJoint2));
	_pSHM->traj.joint_traj[0].joint_rad[2]	= D2R(_tstof(m_strJoint3));
	_pSHM->traj.joint_traj[0].joint_rad[3]	= D2R(_tstof(m_strJoint4));
	_pSHM->traj.joint_traj[0].joint_rad[4]	= D2R(_tstof(m_strJoint5));
	_pSHM->traj.joint_traj[0].joint_rad[5]	= D2R(_tstof(m_strJoint6));
	_pSHM->traj.joint_traj[0].joint_rad[6]	= D2R(_tstof(m_strJoint7));
	_pSHM->traj.joint_traj[0].joint_rad[7]	= D2R(_tstof(m_strJoint8));
	_pSHM->traj.joint_traj[0].joint_rad[8]	= D2R(_tstof(m_strJoint9));
	_pSHM->traj.joint_traj[0].joint_rad[9]	= D2R(_tstof(m_strJoint10));
	_pSHM->traj.joint_traj[0].joint_rad[10]	= D2R(_tstof(m_strJoint11));
	_pSHM->traj.joint_traj[0].joint_rad[11]	= D2R(_tstof(m_strJoint12));
	_pSHM->traj.joint_traj[0].joint_rad[12]	= D2R(_tstof(m_strJoint13));
	_pSHM->traj.joint_traj[0].joint_rad[13]	= D2R(_tstof(m_strJoint14));
	_pSHM->traj.joint_traj[0].joint_rad[14]	= D2R(_tstof(m_strJoint15));
	_pSHM->traj.joint_traj[0].joint_rad[15]	= D2R(_tstof(m_strJoint16));

	_pSHM->cmd.command						= CMD_GO;
}

void CTrajectoryDlg::OnBnClickedBtnTrajJointSetPosture()
{
	UpdateData(TRUE);

	m_strJoint1.Format(_T("%f"), R2D(_pSHM->state.slave_state[0].position));
	m_strJoint2.Format(_T("%f"), R2D(_pSHM->state.slave_state[1].position));
	m_strJoint3.Format(_T("%f"), R2D(_pSHM->state.slave_state[2].position));
	m_strJoint4.Format(_T("%f"), R2D(_pSHM->state.slave_state[3].position));
	m_strJoint5.Format(_T("%f"), R2D(_pSHM->state.slave_state[4].position));
	m_strJoint6.Format(_T("%f"), R2D(_pSHM->state.slave_state[5].position));
	m_strJoint7.Format(_T("%f"), R2D(_pSHM->state.slave_state[6].position));
	m_strJoint8.Format(_T("%f"), R2D(_pSHM->state.slave_state[7].position));
	m_strJoint9.Format(_T("%f"), R2D(_pSHM->state.slave_state[8].position));
	m_strJoint10.Format(_T("%f"), R2D(_pSHM->state.slave_state[9].position));
	m_strJoint11.Format(_T("%f"), R2D(_pSHM->state.slave_state[10].position));
	m_strJoint12.Format(_T("%f"), R2D(_pSHM->state.slave_state[11].position));
	m_strJoint13.Format(_T("%f"), R2D(_pSHM->state.slave_state[12].position));
	m_strJoint14.Format(_T("%f"), R2D(_pSHM->state.slave_state[13].position));
	m_strJoint15.Format(_T("%f"), R2D(_pSHM->state.slave_state[14].position));
	m_strJoint16.Format(_T("%f"), R2D(_pSHM->state.slave_state[15].position));
	
	UpdateData(FALSE);
}

void CTrajectoryDlg::OnBnClickedBtnTrajRunTask()
{
	((CButton*)GetDlgItem(IDC_CHK_TRAJ_TASK))->SetCheck(BST_CHECKED);
	((CButton*)GetDlgItem(IDC_CHK_TRAJ_JOINT))->SetCheck(BST_UNCHECKED);
	SetTrajType(eTrajType_TASK);

	_pSHM->traj.num_task_traj = m_listTrajTask.size();
	list<rPanelManipulatorTaskTarget_t>::const_iterator task = m_listTrajTask.begin();
	for (unsigned int i=0; i<_pSHM->traj.num_task_traj && task!=m_listTrajTask.end(); i++, task++)
		_pSHM->traj.task_traj[i] = *task;

	_pSHM->cmd.command = CMD_GO;
}

void CTrajectoryDlg::OnBnClickedBtnTrajRunJoint()
{
	((CButton*)GetDlgItem(IDC_CHK_TRAJ_TASK))->SetCheck(BST_UNCHECKED);
	((CButton*)GetDlgItem(IDC_CHK_TRAJ_JOINT))->SetCheck(BST_CHECKED);
	SetTrajType(eTrajType_JOINT);


	_pSHM->traj.num_joint_traj = m_listTrajJoint.size();
	list<rPanelManipulatorJointTarget_t>::const_iterator joint = m_listTrajJoint.begin();
	for (unsigned int i=0; i<_pSHM->traj.num_joint_traj && joint!=m_listTrajJoint.end(); i++, joint++)
		_pSHM->traj.joint_traj[i] = *joint;

	_pSHM->cmd.command = CMD_GO;
}

void CTrajectoryDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	//ShowWindow(SW_HIDE);
	//CDialog::OnClose();
}

void CTrajectoryDlg::RefreshData()
{
	DeleteAllTrajTask();
	DeleteAllTrajJoint();
	
	SetTrajType(_pSHM->traj.type);
	for (unsigned int i=0; i<_pSHM->traj.num_task_traj; i++)
		m_listTrajTask.push_back(_pSHM->traj.task_traj[i]);
	
	m_listTrajJoint.clear();
	for (unsigned int i=0; i<_pSHM->traj.num_joint_traj; i++)
		m_listTrajJoint.push_back(_pSHM->traj.joint_traj[i]);
	
	m_jointParam = _pSHM->param;

	UpdateTraj();
}

void CTrajectoryDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	RefreshData();
}
