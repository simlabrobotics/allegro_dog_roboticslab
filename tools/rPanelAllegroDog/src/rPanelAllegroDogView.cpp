
// rPanelAllegroDogView.cpp : implementation of the CrPanelAllegroDogView class
//

#include "stdafx.h"
#include "rPanelAllegroDog.h"

#include "rPanelAllegroDogDoc.h"
#include "rPanelAllegroDogView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CrPanelAllegroDogView

IMPLEMENT_DYNCREATE(CrPanelAllegroDogView, CView)

BEGIN_MESSAGE_MAP(CrPanelAllegroDogView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CrPanelAllegroDogView::OnFilePrintPreview)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CrPanelAllegroDogView construction/destruction

CrPanelAllegroDogView::CrPanelAllegroDogView()
{
	// TODO: add construction code here

}

CrPanelAllegroDogView::~CrPanelAllegroDogView()
{
}

BOOL CrPanelAllegroDogView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CrPanelAllegroDogView drawing

void CrPanelAllegroDogView::OnDraw(CDC* /*pDC*/)
{
	CrPanelAllegroDogDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CrPanelAllegroDogView printing


void CrPanelAllegroDogView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL CrPanelAllegroDogView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CrPanelAllegroDogView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CrPanelAllegroDogView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CrPanelAllegroDogView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CrPanelAllegroDogView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// CrPanelAllegroDogView diagnostics

#ifdef _DEBUG
void CrPanelAllegroDogView::AssertValid() const
{
	CView::AssertValid();
}

void CrPanelAllegroDogView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CrPanelAllegroDogDoc* CrPanelAllegroDogView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CrPanelAllegroDogDoc)));
	return (CrPanelAllegroDogDoc*)m_pDocument;
}
#endif //_DEBUG


// CrPanelAllegroDogView message handlers


int CrPanelAllegroDogView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create tabs window:
	if (!m_wndTabs.Create(CMFCTabCtrl::STYLE_3D_ROUNDED_SCROLL, rectDummy, this, 1, CMFCBaseTabCtrl::LOCATION_TOP))
	{
		TRACE0("Failed to create main view tab window\n");
		return -1;      // fail to create
	}
	//m_wndTabs.SetImageList(IDB_TAB_ICONS, 16, RGB(255, 0, 255));
	m_wndTabs.EnableTabSwap(TRUE);

	if (!m_dlgControlPanel.Create(IDD_CONTROLPANELDLG, &m_wndTabs) ||
		!m_dlgJointParam.Create(IDD_JOINTPARAMDLG, &m_wndTabs) ||
		!m_dlgTrajectory.Create(IDD_TRAJECTORYDLG, &m_wndTabs) ||
		!m_dlgUnitTest.Create(IDD_UNITTESTDLG, &m_wndTabs))
	{
		TRACE0("Failed to create main views\n");
		return -1;      // fail to create
	}

	// Attach child windows to tab:
	CString strTabName;
	BOOL bNameValid;

	bNameValid = strTabName.LoadString(IDS_CONTROLPANEL_TAB);
	ASSERT(bNameValid);
	m_wndTabs.AddTab(&m_dlgControlPanel, strTabName, (UINT)0);
	bNameValid = strTabName.LoadString(IDS_UNITTEST_TAB);
	ASSERT(bNameValid);
	m_wndTabs.AddTab(&m_dlgUnitTest, strTabName, (UINT)1);
	bNameValid = strTabName.LoadString(IDS_TRAJECTORY_TAB);
	ASSERT(bNameValid);
	m_wndTabs.AddTab(&m_dlgTrajectory, strTabName, (UINT)2);
	bNameValid = strTabName.LoadString(IDS_JOINTPARAM_TAB);
	ASSERT(bNameValid);
	m_wndTabs.AddTab(&m_dlgJointParam, strTabName, (UINT)3);

	return 0;
}

void CrPanelAllegroDogView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	if (m_wndTabs.GetSafeHwnd())
		m_wndTabs.SetWindowPos(NULL, -1, 30, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
}

void CrPanelAllegroDogView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	// TODO: Add your specialized code here and/or call the base class
}
