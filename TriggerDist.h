// TriggerDist.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTriggerDist dialog

class CTriggerDist : public CDialog
{
  // Construction
 public:
  CTriggerDist(CWnd* pParent = NULL);  // standard constructor

  // Dialog Data
  //{{AFX_DATA(CTriggerDist)
  enum
  {
    IDD = IDD_TRIGGERDISTANCE
  };
  int m_Distance;
  //}}AFX_DATA

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CTriggerDist)
 protected:
  virtual void
  DoDataExchange(CDataExchange* pDX);  // DDX/DDV support
                                       //}}AFX_VIRTUAL

  // Implementation
 protected:
  // Generated message map functions
  //{{AFX_MSG(CTriggerDist)
  afx_msg void
  OnDefaultVal();
  afx_msg void
  OnRcronly();
  afx_msg void
  OnShapedefault();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};
