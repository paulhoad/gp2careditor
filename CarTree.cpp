// CarTree.cpp : implementation file
//

#include "stdafx.h"
#include "CarEditor.h"
#include "CarTree.h"
#include "Car.h"
#include "Observer.h"
#include "CarEditorDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define PROFILE(x, y, b) \
  AfxGetApp()->GetProfileInt(CString(#y), CString(#x), b);
#define WR_PROFILE(x, y, b) AfxGetApp()->WriteProfileInt(CString(#y), #x, b);

/////////////////////////////////////////////////////////////////////////////
// CCarTree

CCarTree::CCarTree() {}

CCarTree::~CCarTree() {}

void
CCarTree::loadTree()
{
  TV_INSERTSTRUCT tvstruct;

  tvstruct.hParent = NULL;
  tvstruct.hInsertAfter = TVI_FIRST;
  tvstruct.item.iImage = TO_ID(IDB_CAR);
  tvstruct.item.iSelectedImage = TO_ID(IDB_CAR);
  tvstruct.item.pszText = _T("All Cars");
  tvstruct.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT;
  HTREEITEM hTreeRoot = m_CarTree.InsertItem(&tvstruct);

  m_CarTree.Expand(m_CarTree.GetRootItem(), TVE_EXPAND);
}

BEGIN_MESSAGE_MAP(CCarTree, CSizingControlBar)
//{{AFX_MSG_MAP(CCarTree)
ON_WM_SIZE()
ON_WM_CREATE()
ON_NOTIFY(NM_DBLCLK, IDC_CARTREE, OnDblclkCartree)
ON_NOTIFY(TVN_SELCHANGED, IDC_CARTREE, OnclkCartree)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCarTree message handlers

void
CCarTree::OnSize(UINT nType, int cx, int cy)
{
  CSizingControlBar::OnSize(nType, cx, cy);
  CRect rc;
  GetClientRect(rc);

  Reposition(rc);
}

void
CCarTree::Reposition(CRect rc)
{
  int border = 4;

  int bottom = (IsHorz() || IsFloating()) ? rc.Height() - (2 * border)
                                          : rc.Height() - (2 * border) - 4;

  m_CarTree.SetWindowPos(NULL, rc.left + border, rc.top + border,
                         rc.Width() - (2 * border), bottom,
                         SWP_NOACTIVATE | SWP_NOZORDER);
}

int
CCarTree::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (CSizingControlBar::OnCreate(lpCreateStruct) == -1) return -1;

  CRect rc;

  GetClientRect(rc);
  rc.left = PROFILE(TreeLeft, Preferences, 10);
  rc.right = PROFILE(TreeRight, Preferences, 100);
  rc.top = PROFILE(TreeTop, Preferences, 10);
  rc.bottom = PROFILE(TreeBottom, Preferences, 110);

  int cx = rc.right;
  int cy = rc.right;

  /*
  if (!m_CarTree.CreateEx(WS_EX_CLIENTEDGE,"SysTreeView32","",
          TVS_HASLINES  | TVS_HASBUTTONS | TVS_SHOWSELALWAYS |
 TVS_DISABLEDRAGDROP | WS_VISIBLE | WS_CHILD | WS_BORDER ,
          0,0,cx,cy,GetSafeHwnd(),(HMENU)IDC_CARTREE))
 return -1;*/

  CRect r2;
  r2.left = 0;
  r2.top = 0;
  r2.right = cx;
  r2.bottom = cy;

  if (!m_CarTree.CreateEx(WS_EX_CLIENTEDGE,
                          TVS_HASLINES | TVS_HASBUTTONS | TVS_SHOWSELALWAYS |
                              TVS_DISABLEDRAGDROP | WS_VISIBLE | WS_CHILD |
                              WS_BORDER,
                          r2, this, (UINT)IDC_CARTREE))
    return -1;

  Reposition(rc);

  LoadImages();

  return 0;
}

void
CCarTree::LoadImages()
{
  pImageList = (((CCarEditorApp *)AfxGetApp())->getSharedImageList());
  m_CarTree.SetImageList(pImageList, TVSIL_NORMAL);

  // ImagesNotLoaded = FALSE;

  loadTree();
}

void
CCarTree::InsertCar(CDocument *doc, Car *car)
{
  m_CarTree.SetRedraw(FALSE);
  car->pDoc = doc;
  car->LoadTree(this, doc);
  m_CarTree.SetRedraw(TRUE);
}

void
CCarTree::InsertHelmet(CDocument *doc, Helmet *helmet)
{
  helmet->pDoc = doc;
  if (helmet != NULL) helmet->LoadTree(this);
}

void
CCarTree::InsertGPOObject(CDocument *doc, GPOObject *object)
{
  object->pDoc = doc;
  if (object != NULL) object->LoadTree(this);
}

HTREEITEM
CCarTree::insertInfoNode(HTREEITEM parent, LPSTR title, int bitmap)
{
  if (m_CarTree) {
    TV_INSERTSTRUCT tvstruct;

    tvstruct.hParent = parent;
    tvstruct.hInsertAfter = TVI_LAST;
    tvstruct.item.iImage = bitmap;
    tvstruct.item.iSelectedImage = bitmap;
    tvstruct.item.pszText = _T((char *)title);
    tvstruct.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT;
    HTREEITEM hThisItem = m_CarTree.InsertItem(&tvstruct);
    m_CarTree.Expand(m_CarTree.GetRootItem(), TVE_EXPAND);
    return hThisItem;
  }
  else {
    char msg[256];

    wsprintf(msg, "m_TrackTree is NULL for adding in %s", title);
    AfxMessageBox(msg);
  }
  return 0;
}

void
CCarTree::OnDblclkCartree(NMHDR *pNMHDR, LRESULT *pResult)
{
  if (m_CarTree) {
    HTREEITEM TreeNode = m_CarTree.GetSelectedItem();

    DWORD_PTR data = m_CarTree.GetItemData(TreeNode);

    Observer *dlg = (Observer *)data;
    if (dlg != NULL) {
      dlg->Open(this);
      if (dlg->getObject()) {
        dlg->getObject()->PatchData();
        dlg->getObject()->updateData(FALSE);
        if (dlg->getObject()->pDoc != NULL) {
          dlg->getObject()->pDoc->UpdateAllViews(NULL);
        }
      }
    }
  }
  else {
    AfxMessageBox("Tree NULL", MB_OK);
  }

  *pResult = 0;
}

void
CCarTree::OnclkCartree(NMHDR *pNMHDR, LRESULT *pResult)
{
  if (m_CarTree) {
    HTREEITEM TreeNode = m_CarTree.GetSelectedItem();

    if (TreeNode) {
      DWORD_PTR data = m_CarTree.GetItemData(TreeNode);

      Observer *dlg = (Observer *)data;
      if (dlg != NULL) {
        if (dlg->type == t_JamFile) {
          JAM *jam = ((JamFile *)dlg)->JamFileStructure;
          ((CCarEditorDoc *)dlg->getDocument())->setCurrentJam(jam);
        }
        else if (dlg->getObject()) {
          dlg->getObject()->clearSelections();
          dlg->Select(this);
          if (dlg->getObject()->pDoc != NULL) {
            dlg->getObject()->pDoc->UpdateAllViews(NULL);
          }
        }
      }
    }
  }
  else {
    AfxMessageBox("Tree NULL", MB_OK);
  }

  *pResult = 0;
}
