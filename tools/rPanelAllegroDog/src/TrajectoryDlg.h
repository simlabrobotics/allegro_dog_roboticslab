#pragma once

#include "afxcmn.h"
#include "afxwin.h"
#include "ScrollDlg.h"
#include "ComboListCtrl.h"

#include <list>
using namespace std;


class CListCtrlKeyDown : public CListCtrl
{
public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

// CTrajectoryDlg dialog

class CTrajectoryDlg : public CScrollDlg
{
	DECLARE_DYNAMIC(CTrajectoryDlg)

public:
	CTrajectoryDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTrajectoryDlg();

	void SetTrajType(eTrajType type);
	void DeleteTrajJoint(int index);
	void DeleteTrajTask(int index);
	void DeleteAllTrajJoint();
	void DeleteAllTrajTask();
	void UpdateTraj();
	void OnDeleteKeyPressed(CWnd* pWnd);

// Dialog Data
	enum { IDD = IDD_TRAJECTORYDLG };

	eTrajType m_trajType;

	static BOOL ms_bEnableJointLimit;

private:
	bool _isAdd;
	CString* _strJointArray[18];
	CString* _strTaskArray[12];
	rPanelManipulatorData_t	*_pSHM;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedChkTrajTask();
	afx_msg void OnBnClickedChkTrajJoint();
	virtual BOOL OnInitDialog();
	CComboListCtrl m_listCtrlTrajTask;
	CComboListCtrl m_listCtrlTrajJoint;
	afx_msg void OnBnClickedBtnTrajAddTask();
	afx_msg void OnBnClickedBtnTrajAddJoint();
	CString m_strTaskXg;
	CString m_strTaskYg;
	CString m_strTaskZg;
	CString m_strTaskThg;
	CString m_strTaskX;
	CString m_strTaskY;
	CString m_strTaskZ;
	CString m_strTaskRoll;
	CString m_strTaskPitch;
	CString m_strTaskYaw;
	CString m_strTaskDO;
	CString m_strTaskDuration;
	CString m_strJoint1;
	CString m_strJoint2;
	CString m_strJoint3;
	CString m_strJoint4;
	CString m_strJoint5;
	CString m_strJoint6;
	CString m_strJoint7;
	CString m_strJoint8;
	CString m_strJoint9;
	CString m_strJoint10;
	CString m_strJoint11;
	CString m_strJoint12;
	CString m_strJoint13;
	CString m_strJoint14;
	CString m_strJoint15;
	CString m_strJoint16;
	CString m_strJointDO;
	CString m_strJointDuration;
	list<rPanelManipulatorTaskTarget_t> m_listTrajTask;
	list<rPanelManipulatorJointTarget_t> m_listTrajJoint;
	rPanelManipulatorParam_t m_jointParam;
	afx_msg void OnBnClickedOk();
	afx_msg void OnTrajectoryDialogFileNew();
	afx_msg void OnTrajectoryDialogFileOpen();
	afx_msg void OnTrajectoryDialogFileSave();
	afx_msg void OnTrajectoryDialogFileSaveas();
	afx_msg void OnBnClickedBtnTrajClearTask();
	afx_msg void OnBnClickedBtnTrajDeleteTask();
	afx_msg void OnBnClickedBtnTrajClearJoint();
	afx_msg void OnBnClickedBtnTrajDeleteJoint();
	afx_msg void OnBnClickedBtnTrajApplyJoint();
	afx_msg void OnBnClickedBtnTrajUpJoint();
	afx_msg void OnBnClickedBtnTrajDownJoint();
	afx_msg void OnBnClickedBtnTrajApplyTask();
	afx_msg void OnLvnItemchangedListTrajTask(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnTrajUpTask();
	afx_msg void OnBnClickedBtnTrajDownTask();
	afx_msg void OnNMClickListTrajJoint(NMHDR *pNMHDR, LRESULT *pResult);
	CButton m_chkEnableJointLimit;
	afx_msg void OnBnClickedCheckLimitJoint();
	afx_msg void OnBnClickedBtnTrajTaskGo();
	afx_msg void OnBnClickedBtnTrajTaskSetPosture();
	afx_msg void OnBnClickedBtnTrajJointGo();
	afx_msg void OnBnClickedBtnTrajJointSetPosture();
	afx_msg void OnBnClickedBtnTrajRunTask();
	afx_msg void OnBnClickedBtnTrajRunJoint();
	afx_msg void OnClose();
	void RefreshData();
	afx_msg void OnBnClickedCancel();
};
