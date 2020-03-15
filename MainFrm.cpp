// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "CarEditor.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define PROFILE(x, y, b) \
  AfxGetApp()->GetProfileInt(CString(#y), CString(#x), b);
#define WR_PROFILE(x, y, b) AfxGetApp()->WriteProfileInt(CString(#y), #x, b);

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
//{{AFX_MSG_MAP(CMainFrame)
ON_WM_CREATE()
ON_WM_SIZE()
//}}AFX_MSG_MAP
// Global help commands
ON_COMMAND(ID_HELP_FINDER, CMDIFrameWnd::OnHelpFinder)
ON_COMMAND(ID_HELP, CMDIFrameWnd::OnHelp)
ON_COMMAND(ID_CONTEXT_HELP, CMDIFrameWnd::OnContextHelp)
ON_COMMAND(ID_DEFAULT_HELP, CMDIFrameWnd::OnHelpFinder)
END_MESSAGE_MAP()

static UINT indicators[] = {
    ID_SEPARATOR,  // status line indicator
    ID_INDICATOR_CAPS,
    ID_INDICATOR_NUM,
    ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
  // TODO: add member initialization code here
}

CMainFrame::~CMainFrame() {}

int
CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1) return -1;

  if (!m_wndToolBar.Create(this) || !m_wndToolBar.LoadToolBar(IDR_MAINFRAME)) {
    TRACE0("Failed to create toolbar\n");
    return -1;  // fail to create
  }

  if (!m_wndShowBar.Create(this) ||
      !m_wndShowBar.LoadToolBar(IDR_SHOWTOOLBAR)) {
    TRACE0("Failed to create toolbar\n");
    return -1;  // fail to create
  }

  if (!m_wndStatusBar.Create(this) ||
      !m_wndStatusBar.SetIndicators(indicators,
                                    sizeof(indicators) / sizeof(UINT))) {
    TRACE0("Failed to create status bar\n");
    return -1;  // fail to create
  }

  // TODO: Remove this if you don't want tool tips or a resizeable toolbar
  m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() | CBRS_TOOLTIPS |
                           CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

  // TODO: Delete these three lines if you don't want the toolbar to
  //  be dockable
  m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
  EnableDocking(CBRS_ALIGN_ANY);
  DockControlBar(&m_wndToolBar);

  // TODO: Remove this if you don't want tool tips or a resizeable toolbar
  m_wndShowBar.SetBarStyle(m_wndShowBar.GetBarStyle() | CBRS_TOOLTIPS |
                           CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

  // TODO: Delete these three lines if you don't want the toolbar to
  //  be dockable
  m_wndShowBar.EnableDocking(CBRS_ALIGN_ANY);
  EnableDocking(CBRS_ALIGN_ANY);
  DockControlBar(&m_wndShowBar);

  int TreeHeight = PROFILE(TreeHeight, Preferences, 10);
  int TreeWidth = PROFILE(TreeWidth, Preferences, 200);

  if (!m_wndDlgBar.Create(this, CSize(TreeWidth, TreeHeight),
                          AFX_IDW_CONTROLBAR_FIRST + 32,
                          WS_CHILD | WS_VISIBLE | CBRS_LEFT)) {
    AfxMessageBox("Failed to create DlgBar", MB_OK);
    return -1;  // fail to create
  }
  m_wndDlgBar.EnableDocking(CBRS_ALIGN_LEFT | CBRS_ALIGN_RIGHT);
  DockControlBar(&m_wndDlgBar);

  // theApp.theMainTree = &m_wndDlgBar;
  ((CCarEditorApp*)AfxGetApp())->m_MainTree = &m_wndDlgBar;

  return 0;
}

BOOL
CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
  // TODO: Modify the Window class or styles here by modifying
  //  the CREATESTRUCT cs

  return CMDIFrameWnd::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void
CMainFrame::AssertValid() const
{
  CMDIFrameWnd::AssertValid();
}

void
CMainFrame::Dump(CDumpContext& dc) const
{
  CMDIFrameWnd::Dump(dc);
}

#endif  //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void
CMainFrame::OnSize(UINT nType, int cx, int cy)
{
  CMDIFrameWnd::OnSize(nType, cx, cy);

  RecalcLayout(TRUE);
}

void
CMainFrame::RecalcLayout(BOOL bNotify)
{
  CMDIFrameWnd::RecalcLayout(bNotify);
  CMDIFrameWnd::RecalcLayout(bNotify);
}
