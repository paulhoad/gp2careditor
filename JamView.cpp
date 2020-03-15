// JamView.cpp : implementation file
//

#include "stdafx.h"
#include "careditor.h"
#include "JamView.h"
#include "CarEditorDoc.h"
#include "JAM.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CJamView

IMPLEMENT_DYNCREATE(CJamView, CScrollView)

CJamView::CJamView()
{
  currentScale = 1.0;
  rcrJam = TRUE;
  showNumbers = FALSE;
}

CJamView::~CJamView() {}

BEGIN_MESSAGE_MAP(CJamView, CScrollView)
//{{AFX_MSG_MAP(CJamView)
ON_WM_CREATE()
ON_COMMAND(ID_SHOW_JAMNUMBERS, OnShowJamnumbers)
ON_UPDATE_COMMAND_UI(ID_SHOW_JAMNUMBERS, OnUpdateShowJamnumbers)
ON_WM_MOUSEMOVE()
ON_COMMAND(ID_ZOOM_IN, OnZoomIn)
ON_COMMAND(ID_ZOOM_OUT, OnZoomOut)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CJamView drawing

void
CJamView::OnInitialUpdate()
{
  CScrollView::OnInitialUpdate();

  CSize sizeTotal;
  // TODO: calculate the total size of this view
  sizeTotal.cx = 512;
  sizeTotal.cy = 2000;
  SetScrollSizes(MM_TEXT, sizeTotal);

  CCarEditorDoc* pDoc = (CCarEditorDoc*)GetDocument();
  myjam = pDoc->getCurrentJam();
  if (myjam == NULL) {
    AfxMessageBox("You must select a JAM file from the tree first!", MB_OK);
  }
}

void
CJamView::OnDraw(CDC* pDC)
{
  CCarEditorDoc* pDoc = (CCarEditorDoc*)GetDocument();

  if (myjam) {
    //|myjam->DrawAllJams(pDC,currentScale,rcrJam,showNumbers);
    if (myjam->rcr)
      myjam->DrawSingleJams(pDC, currentScale, pDoc->currentJamBitmap,
                            myjam->rcr);
    else
      myjam->DrawAllJams(pDC, currentScale, rcrJam, showNumbers);
  }
}

/////////////////////////////////////////////////////////////////////////////
// CJamView diagnostics

#ifdef _DEBUG
void
CJamView::AssertValid() const
{
  CScrollView::AssertValid();
}

void
CJamView::Dump(CDumpContext& dc) const
{
  CScrollView::Dump(dc);
}
#endif  //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CJamView message handlers

int
CJamView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (CScrollView::OnCreate(lpCreateStruct) == -1) return -1;

  // TODO: Add your specialized creation code here

  return 0;
}

void
CJamView::Resize()
{
  CSize sizeTotal;
  sizeTotal.cx = sizeTotal.cy = (int)(currentScale * 256);
  SetScrollSizes(MM_TEXT, sizeTotal);
}

void
CJamView::OnShowJamnumbers()
{
  showNumbers = !showNumbers;
  InvalidateRect(NULL, FALSE);
}

void
CJamView::OnUpdateShowJamnumbers(CCmdUI* pCmdUI)
{
  pCmdUI->SetCheck(showNumbers);
}

void
CJamView::OnMouseMove(UINT nFlags, CPoint point)
{
  // TODO: Add your message handler code here and/or call default
  CDC* dc = GetDC();

  COLORREF clr = dc->GetPixel(point.x, point.y);

  if (myjam) {
    int idx = myjam->GetPoint(point.x, point.y);

    char msg[256];
    wsprintf(msg, "[%d] = RGB(%d,%d,%d)", idx, GetRValue(clr), GetGValue(clr),
             GetBValue(clr));

    SetStatusText(msg);
  }

  CScrollView::OnMouseMove(nFlags, point);
}

void
CJamView::OnZoomIn()
{
  currentScale *= 1.1;
  InvalidateRect(NULL, FALSE);
}

void
CJamView::OnZoomOut()
{
  currentScale *= 0.9;
  InvalidateRect(NULL, FALSE);
}
