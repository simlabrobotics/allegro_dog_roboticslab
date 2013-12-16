
#include "stdafx.h"
#include "PropertiesWnd.h"
#include "Resource.h"
#include "MainFrm.h"
#include "rPanelAllegroDog.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar

CPropertiesWnd::CPropertiesWnd()
: _data(NULL)
, _dataUpdateTimerID(0)
{
}

CPropertiesWnd::~CPropertiesWnd()
{
}

BEGIN_MESSAGE_MAP(CPropertiesWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_EXPAND_ALL, OnExpandAllProperties)
	ON_UPDATE_COMMAND_UI(ID_EXPAND_ALL, OnUpdateExpandAllProperties)
	ON_COMMAND(ID_SORTPROPERTIES, OnSortProperties)
	ON_UPDATE_COMMAND_UI(ID_SORTPROPERTIES, OnUpdateSortProperties)
	ON_COMMAND(ID_PROPERTIES1, OnProperties1)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES1, OnUpdateProperties1)
	ON_COMMAND(ID_PROPERTIES2, OnProperties2)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES2, OnUpdateProperties2)
	ON_WM_SETFOCUS()
	ON_WM_SETTINGCHANGE()
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar message handlers

void CPropertiesWnd::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient,rectCombo;
	GetClientRect(rectClient);

	m_wndObjectCombo.GetWindowRect(&rectCombo);

	int cyCmb = rectCombo.Size().cy;
	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndObjectCombo.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), 200, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top + cyCmb, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndPropList.SetWindowPos(NULL, rectClient.left, rectClient.top + cyCmb + cyTlb, rectClient.Width(), rectClient.Height() -(cyCmb+cyTlb), SWP_NOACTIVATE | SWP_NOZORDER);
}

int CPropertiesWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create combo:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_BORDER | CBS_SORT | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!m_wndObjectCombo.Create(dwViewStyle, rectDummy, this, 1))
	{
		TRACE0("Failed to create Properties Combo \n");
		return -1;      // fail to create
	}

	m_wndObjectCombo.AddString(_T("Application"));
	m_wndObjectCombo.AddString(_T("Properties Window"));
	m_wndObjectCombo.SetFont(CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT)));
	m_wndObjectCombo.SetCurSel(0);

	if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2))
	{
		TRACE0("Failed to create Properties Grid \n");
		return -1;      // fail to create
	}

	_data = theApp.GetStateData();
	InitPropList();

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_PROPERTIES);
	m_wndToolBar.LoadToolBar(IDR_PROPERTIES, 0, 0, TRUE /* Is locked */);
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_PROPERTIES_HC : IDR_PROPERTIES, 0, 0, TRUE /* Locked */);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	m_wndToolBar.SetOwner(this);

	// All commands will be routed via this control , not via the parent frame:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	AdjustLayout();
	return 0;
}

void CPropertiesWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CPropertiesWnd::OnExpandAllProperties()
{
	m_wndPropList.ExpandAll();
}

void CPropertiesWnd::OnUpdateExpandAllProperties(CCmdUI* pCmdUI)
{
}

void CPropertiesWnd::OnSortProperties()
{
	m_wndPropList.SetAlphabeticMode(!m_wndPropList.IsAlphabeticMode());
}

void CPropertiesWnd::OnUpdateSortProperties(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndPropList.IsAlphabeticMode());
}

void CPropertiesWnd::OnProperties1()
{
	UpdateProperties();
}

void CPropertiesWnd::OnUpdateProperties1(CCmdUI* /*pCmdUI*/)
{
	// TODO: Add your command update UI handler code here
}

void CPropertiesWnd::OnProperties2()
{
	if (_dataUpdateTimerID)
	{
		KillTimer(_dataUpdateTimerID);
		_dataUpdateTimerID = 0;
	}
	else
	{
		_dataUpdateTimerID = _TIMER_ID_DATA_UPDATE_PROPERTY_WND;
		SetTimer(_dataUpdateTimerID, _TIMER_PERIOD_DATA_UPDATE_PROPERTY_WND, 0);
	}
}

void CPropertiesWnd::OnUpdateProperties2(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(_dataUpdateTimerID != 0);
}

void CPropertiesWnd::InitPropList()
{
	SetPropListFont();

	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();

	CString strJointName;
	CString strPropName;
	int nJointIndex;

	// System
	_pPropSystem = new CMFCPropertyGridProperty(_T("System"));
	m_wndPropList.AddProperty(_pPropSystem);

	_pPropCurTime = new CMFCPropertyGridProperty(_T("Current time"), (_variant_t) 0.0f, _T("Current time\r\n(second)"));
	_pPropCurTime->Enable(FALSE);
	_pPropSystem->AddSubItem(_pPropCurTime);

	// Joint
	_pPropJoint = new CMFCPropertyGridProperty(_T("Joint"));
	m_wndPropList.AddProperty(_pPropJoint);

	for (int i=0; i<JDOF; i++)
	{
		nJointIndex = i+1;

		strJointName.Format(_T("Joint %d"), nJointIndex);
		_pPropJointSub[i] = new CMFCPropertyGridProperty((LPCTSTR)strJointName, 0, FALSE);
		_pPropJoint->AddSubItem(_pPropJointSub[i]);

		strPropName.Format(_T("Graph %02d"), nJointIndex);
		_pPropJointSub_clr[i] = new CMFCPropertyGridColorProperty((LPCTSTR)strPropName, GraphLineColorArr[i], NULL, _T("Line color."));
		_pPropJointSub_clr[i]->Enable(FALSE);
		_pPropJointSub[i]->AddSubItem(_pPropJointSub_clr[i]);

		strPropName.Format(_T("Position %02d"), nJointIndex);
		_pPropJointSub_pos[i] = new CMFCPropertyGridProperty((LPCTSTR)strPropName, (_variant_t) 0.0f, _T("joint actual position.\r\n(degree)"));
		_pPropJointSub_pos[i]->Enable(FALSE);
		_pPropJointSub[i]->AddSubItem(_pPropJointSub_pos[i]);

		strPropName.Format(_T("Velocity %02d"), nJointIndex);
		_pPropJointSub_vel[i] = new CMFCPropertyGridProperty((LPCTSTR)strPropName, (_variant_t) 0.0f, _T("joint actual velocity.\r\n(degree/sec)"));
		_pPropJointSub_vel[i]->Enable(FALSE);
		_pPropJointSub[i]->AddSubItem(_pPropJointSub_vel[i]);

		strPropName.Format(_T("Torque %02d"), nJointIndex);
		_pPropJointSub_tau[i] = new CMFCPropertyGridProperty((LPCTSTR)strPropName, (_variant_t) 0.0f, _T("Commanded joint torque.\r\n(Nm)"));
		_pPropJointSub_tau[i]->Enable(FALSE);
		_pPropJointSub[i]->AddSubItem(_pPropJointSub_tau[i]);

		strPropName.Format(_T("Mode %02d"), nJointIndex);
		_pPropJointSub_opm[i] = new CMFCPropertyGridProperty((LPCTSTR)strPropName, (_variant_t) CString(szOpMode[eOpMode_POSITION]), _T("Current mode of operation."));
		_pPropJointSub_opm[i]->Enable(FALSE);
		_pPropJointSub[i]->AddSubItem(_pPropJointSub_opm[i]);

		strPropName.Format(_T("Status %02d"), nJointIndex);
		_pPropJointSub_sta[i] = new CMFCPropertyGridProperty((LPCTSTR)strPropName, (_variant_t) CString(szOpStatus[eOpStatus_START]), _T("Current operation status(CoE)."));
		_pPropJointSub_sta[i]->Enable(FALSE);
		_pPropJointSub[i]->AddSubItem(_pPropJointSub_sta[i]);

		strPropName.Format(_T("Error %02d"), nJointIndex);
		_pPropJointSub_err[i] = new CMFCPropertyGridProperty((LPCTSTR)strPropName, (_variant_t) CString(szErrCode[eErrCode_NONE]), _T("Error code."));
		_pPropJointSub_err[i]->Enable(FALSE);
		_pPropJointSub[i]->AddSubItem(_pPropJointSub_err[i]);
	}
}

void CPropertiesWnd::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	m_wndPropList.SetFocus();
}

void CPropertiesWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CDockablePane::OnSettingChange(uFlags, lpszSection);
	SetPropListFont();
}

void CPropertiesWnd::SetPropListFont()
{
	::DeleteObject(m_fntPropList.Detach());

	LOGFONT lf;
	afxGlobalData.fontRegular.GetLogFont(&lf);

	NONCLIENTMETRICS info;
	info.cbSize = sizeof(info);

	afxGlobalData.GetNonClientMetrics(info);

	lf.lfHeight = info.lfMenuFont.lfHeight;
	lf.lfWeight = info.lfMenuFont.lfWeight;
	lf.lfItalic = info.lfMenuFont.lfItalic;

	m_fntPropList.CreateFontIndirect(&lf);

	m_wndPropList.SetFont(&m_fntPropList);
}

void CPropertiesWnd::UpdateProperties()
{
	if (!GetSafeHwnd()) return;

	_pPropCurTime->SetValue((_variant_t) (float)_data->state.time);

	for (int i=0; i<JDOF; i++)
	{
		_pPropJointSub_sta[i]->SetValue((_variant_t) CString(szOpStatus[_data->state.slave_state[i].OP_status]));
		_pPropJointSub_opm[i]->SetValue((_variant_t) CString(szOpMode[_data->state.slave_state[i].OP_mode]));
		_pPropJointSub_err[i]->SetValue((_variant_t) CString(szErrCode[_data->state.slave_state[i].error_code]));
		_pPropJointSub_pos[i]->SetValue((_variant_t) (float)RTD(_data->state.slave_state[i].position));
		_pPropJointSub_vel[i]->SetValue((_variant_t) (float)RTD(_data->state.slave_state[i].velocity));
		_pPropJointSub_tau[i]->SetValue((_variant_t) (float)_data->state.slave_state[i].torque);
	}
}

void CPropertiesWnd::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case _TIMER_ID_DATA_UPDATE_PROPERTY_WND:
		UpdateProperties();
		break;
	}

	CDockablePane::OnTimer(nIDEvent);
}

void CPropertiesWnd::OnDestroy()
{
	CDockablePane::OnDestroy();

	KillTimer(9);
}
