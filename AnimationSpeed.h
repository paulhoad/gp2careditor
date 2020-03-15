// AnimationSpeed.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAnimationSpeed dialog

class CAnimationSpeed : public CDialog
{
  // Construction
 public:
  CAnimationSpeed(CWnd* pParent = NULL);  // standard constructor

  // Dialog Data
  //{{AFX_DATA(CAnimationSpeed)
  enum
  {
    IDD = IDD_DIALOG3
  };
  UINT m_Speed;
  //}}AFX_DATA

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CAnimationSpeed)
 protected:
  virtual void
  DoDataExchange(CDataExchange* pDX);  // DDX/DDV support
                                       //}}AFX_VIRTUAL

  // Implementation
 protected:
  // Generated message map functions
  //{{AFX_MSG(CAnimationSpeed)
  // NOTE: the ClassWizard will add member functions here
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};
