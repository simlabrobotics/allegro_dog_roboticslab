
#pragma once

#include "rPanelManipulatorDef.h"
#include "rPanelManipulatorCmdUtil.h"

class CPropertiesToolBar : public CMFCToolBar
{
public:
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class CPropertiesWnd : public CDockablePane
{
// Construction
public:
	CPropertiesWnd();

	void AdjustLayout();

// Attributes
public:
	void SetVSDotNetLook(BOOL bSet)
	{
		m_wndPropList.SetVSDotNetLook(bSet);
		m_wndPropList.SetGroupNameFullWidth(bSet);
	}

protected:
	CFont m_fntPropList;
	CComboBox m_wndObjectCombo;
	CPropertiesToolBar m_wndToolBar;
	CMFCPropertyGridCtrl m_wndPropList;

	rPanelManipulatorData_t*	_data;
	UINT_PTR					_dataUpdateTimerID;
	CMFCPropertyGridProperty*	_pPropSystem;
	CMFCPropertyGridProperty*	_pPropCurTime;
	CMFCPropertyGridProperty*	_pPropJoint;
	CMFCPropertyGridProperty*	_pPropJointSub[JDOF];
	CMFCPropertyGridProperty*	_pPropJointSub_clr[JDOF];
	CMFCPropertyGridProperty*	_pPropJointSub_pos[JDOF];
	CMFCPropertyGridProperty*	_pPropJointSub_vel[JDOF];
	CMFCPropertyGridProperty*	_pPropJointSub_tau[JDOF];
	CMFCPropertyGridProperty*	_pPropJointSub_sta[JDOF];
	CMFCPropertyGridProperty*	_pPropJointSub_opm[JDOF];
	CMFCPropertyGridProperty*	_pPropJointSub_err[JDOF];

// Implementation
public:
	virtual ~CPropertiesWnd();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnExpandAllProperties();
	afx_msg void OnUpdateExpandAllProperties(CCmdUI* pCmdUI);
	afx_msg void OnSortProperties();
	afx_msg void OnUpdateSortProperties(CCmdUI* pCmdUI);
	afx_msg void OnProperties1();
	afx_msg void OnUpdateProperties1(CCmdUI* pCmdUI);
	afx_msg void OnProperties2();
	afx_msg void OnUpdateProperties2(CCmdUI* pCmdUI);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);

	DECLARE_MESSAGE_MAP()

	void InitPropList();
	void SetPropListFont();
	void UpdateProperties();
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
};

