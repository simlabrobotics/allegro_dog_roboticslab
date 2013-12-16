
#include "stdafx.h"

#include "OutputWnd.h"
#include "Resource.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COutputBar

COutputWnd::COutputWnd()
{
}

COutputWnd::~COutputWnd()
{
}

BEGIN_MESSAGE_MAP(COutputWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

void COutputWnd::WriteLog(eLogTarget etarget, eLogType etype, bool bringFrontWnd, bool mb, LPCTSTR lpszLog)
{
	CWnd* target = NULL;
	switch (etarget)
	{
	case eLOG_TARGET_OUTPUT:
		target = &m_wndOutputGeneral;
		break;
	case eLOG_TARGET_DEBUG:
		target = &m_wndOutputDebug;
		break;
	case eLOG_TARGET_PARAM:
		target = &m_wndOutputFind;
		break;
	default:
		return;
	}

	// prepare log string.
	CString log = lpszLog;

	// append prefix if needed.
	switch (etype)
	{
	case eLOG_WARN:
		log = _T("[WARN] ") + log;
		break;
	case eLOG_ERROR:
		log = _T("[ERROR] ") + log;
		break;
	}

	// attach time & date
	time_t timer;
	TCHAR szTime[25];
	struct tm tm_info;
	time(&timer);
	localtime_s(&tm_info, &timer);
	_tcsftime(szTime, 25, _T("[%Y/%m/%d %H:%M:%S] "), &tm_info);
	log = szTime + log;

	// show log string.
	switch (etarget)
	{
	case eLOG_TARGET_OUTPUT:
	case eLOG_TARGET_DEBUG:
	case eLOG_TARGET_PARAM:
		((COutputList*)target)->AddString(log);
		break;
	}
	target->Invalidate();
	target->UpdateWindow();
	

	// set the selected log window as active(front) window.
	if (bringFrontWnd)
		m_wndTabs.SetActiveTab(m_wndTabs.GetTabFromHwnd(target->GetSafeHwnd()));

	// message box
	if (mb)
		AfxMessageBox(log);

	// scroll down to the bottom
	((COutputList*)target)->SetSel(((COutputList*)target)->GetCount()-1, 1);
	((COutputList*)target)->SetTopIndex(((COutputList*)target)->GetCount()-1);
	((COutputList*)target)->SetScrollPos(SB_VERT, ((COutputList*)target)->GetCount()-1);
}

void COutputWnd::ClearLog(eLogTarget etarget)
{
	COutputList* target = NULL;
	switch (etarget)
	{
	case eLOG_TARGET_OUTPUT:
		target = &m_wndOutputGeneral;
		break;
	case eLOG_TARGET_DEBUG:
		target = &m_wndOutputDebug;
		break;
	case eLOG_TARGET_PARAM:
		target = &m_wndOutputFind;
		break;
	default:
		return;
	}

	int nItems;
	while ((nItems = target->GetCount()) > 0)
	{
		target->DeleteString((UINT)(nItems-1));
	}
}

int COutputWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_Font.CreateStockObject(DEFAULT_GUI_FONT);

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create tabs window:
	if (!m_wndTabs.Create(CMFCTabCtrl::STYLE_FLAT, rectDummy, this, 1))
	{
		TRACE0("Failed to create output tab window\n");
		return -1;      // fail to create
	}

	// Create output panes:
	const DWORD dwStyle = LBS_NOINTEGRALHEIGHT | WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL;

	if (!m_wndOutputGeneral.Create(dwStyle, rectDummy, &m_wndTabs, 2) ||
		!m_wndOutputDebug.Create(dwStyle, rectDummy, &m_wndTabs, 3) ||
		!m_wndOutputFind.Create(dwStyle, rectDummy, &m_wndTabs, 4))
	{
		TRACE0("Failed to create output windows\n");
		return -1;      // fail to create
	}

	m_wndOutputGeneral.SetFont(&m_Font);
	m_wndOutputDebug.SetFont(&m_Font);
	m_wndOutputFind.SetFont(&m_Font);

	CString strTabName;
	BOOL bNameValid;

	// Attach list windows to tab:
	bNameValid = strTabName.LoadString(IDS_BUILD_TAB);
	ASSERT(bNameValid);
	m_wndTabs.AddTab(&m_wndOutputGeneral, strTabName, (UINT)0);
	/*bNameValid = strTabName.LoadString(IDS_DEBUG_TAB);
	ASSERT(bNameValid);
	m_wndTabs.AddTab(&m_wndOutputDebug, strTabName, (UINT)1);
	bNameValid = strTabName.LoadString(IDS_FIND_TAB);
	ASSERT(bNameValid);
	m_wndTabs.AddTab(&m_wndOutputFind, strTabName, (UINT)2);*/

	return 0;
}

void COutputWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// Tab control should cover the whole client area:
	m_wndTabs.SetWindowPos (NULL, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
}

void COutputWnd::AdjustHorzScroll(CListBox& wndListBox)
{
	CClientDC dc(this);
	CFont* pOldFont = dc.SelectObject(&m_Font);

	int cxExtentMax = 0;

	for (int i = 0; i < wndListBox.GetCount(); i ++)
	{
		CString strItem;
		wndListBox.GetText(i, strItem);

		cxExtentMax = max(cxExtentMax, dc.GetTextExtent(strItem).cx);
	}

	wndListBox.SetHorizontalExtent(cxExtentMax);
	dc.SelectObject(pOldFont);
}

/////////////////////////////////////////////////////////////////////////////
// COutputList1

COutputList::COutputList()
{
}

COutputList::~COutputList()
{
}

BEGIN_MESSAGE_MAP(COutputList, CListBox)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_COMMAND(ID_VIEW_OUTPUTWND, OnViewOutput)
	ON_WM_WINDOWPOSCHANGING()
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// COutputList message handlers

void COutputList::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	CMenu menu;
	menu.LoadMenu(IDR_OUTPUT_POPUP);

	CMenu* pSumMenu = menu.GetSubMenu(0);

	if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))
	{
		CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

		if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
			return;

		((CMDIFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
		UpdateDialogControls(this, FALSE);
	}

	SetFocus();
}

void COutputList::OnEditCopy()
{
	MessageBox(_T("Copy output"));
}

void COutputList::OnEditClear()
{
	MessageBox(_T("Clear output"));
}

void COutputList::OnViewOutput()
{
	CDockablePane* pParentBar = DYNAMIC_DOWNCAST(CDockablePane, GetOwner());
	CMDIFrameWndEx* pMainFrame = DYNAMIC_DOWNCAST(CMDIFrameWndEx, GetTopLevelFrame());

	if (pMainFrame != NULL && pParentBar != NULL)
	{
		pMainFrame->SetFocus();
		pMainFrame->ShowPane(pParentBar, FALSE, FALSE, FALSE);
		pMainFrame->RecalcLayout();

	}
}


