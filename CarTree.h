// CarTree.h : header file
#ifndef _INCLUDED_CCARTREE_H_
#define _INCLUDED_CCARTREE_H_
//
#include "sizecbar.h"

/////////////////////////////////////////////////////////////////////////////
// CCarTree window

#define TO_ID(x) (x - IDB_CAR)

class Car;
class Helmet;
class GPOObject;

class CCarTree : public CSizingControlBar
{
  // Construction
 public:
  CCarTree();

  HTREEITEM
  insertInfoNode(HTREEITEM parent, LPSTR title, int bitmap);

  void
  InsertGPOObject(CDocument *doc, GPOObject *object);

  // Attributes
 public:
  // Operations
 public:
  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CCarTree)
  //}}AFX_VIRTUAL

  CTreeCtrl m_CarTree;

  // Implementation
 public:
  virtual ~CCarTree();

  void
  loadTree();
  void
  Reposition(CRect rc);
  void
  LoadImages();

  CImageList *pImageList;

  void
  InsertCar(CDocument *pDoc, Car *car);
  void
  InsertHelmet(CDocument *pDoc, Helmet *helmet);

  void
  OnDblclkCartree(NMHDR *pNMHDR, LRESULT *pResult);
  void
  OnclkCartree(NMHDR *pNMHDR, LRESULT *pResult);

  // Generated message map functions
 protected:
  //{{AFX_MSG(CCarTree)
  afx_msg void
  OnSize(UINT nType, int cx, int cy);
  afx_msg int
  OnCreate(LPCREATESTRUCT lpCreateStruct);
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif