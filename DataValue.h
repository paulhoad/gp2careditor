// DataValue.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDataValue dialog

class CDataValue : public CDialog
{
  // Construction
 public:
  CDataValue(CWnd* pParent = NULL);  // standard constructor

  // Dialog Data
  //{{AFX_DATA(CDataValue)
  enum
  {
    IDD = IDD_DATAVAL
  };
  int m_DataVal;
  //}}AFX_DATA

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CDataValue)
 protected:
  virtual void
  DoDataExchange(CDataExchange* pDX);  // DDX/DDV support
                                       //}}AFX_VIRTUAL

  // Implementation
 protected:
  // Generated message map functions
  //{{AFX_MSG(CDataValue)
  // NOTE: the ClassWizard will add member functions here
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};
