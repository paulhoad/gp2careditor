// JamView.h : header file
//
#include "Jam.h"

/////////////////////////////////////////////////////////////////////////////
// CJamView view

class CJamView : public CScrollView
{
 protected:
  CJamView();  // protected constructor used by dynamic creation
  DECLARE_DYNCREATE(CJamView)

  // Attributes
 public:
  // Operations
 public:
  JAM* myjam;

  double currentScale;
  BOOL rcrJam;
  BOOL showNumbers;

  void
  Resize();

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CJamView)
 protected:
  virtual void
  OnDraw(CDC* pDC);  // overridden to draw this view
  virtual void
  OnInitialUpdate();  // first time after construct
                      //}}AFX_VIRTUAL

  // Implementation
 protected:
  virtual ~CJamView();
#ifdef _DEBUG
  virtual void
  AssertValid() const;
  virtual void
  Dump(CDumpContext& dc) const;
#endif

  // Generated message map functions
  //{{AFX_MSG(CJamView)
  afx_msg int
  OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void
  OnShowJamnumbers();
  afx_msg void
  OnUpdateShowJamnumbers(CCmdUI* pCmdUI);
  afx_msg void
  OnMouseMove(UINT nFlags, CPoint point);
  afx_msg void
  OnZoomIn();
  afx_msg void
  OnZoomOut();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
