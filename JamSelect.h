// JamSelect.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CJamSelect dialog

class CJamSelect : public CDialog
{
// Construction
public:
	CJamSelect(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CJamSelect)
	enum { IDD = IDD_SELECTJAM };
	CComboBox	m_Names;
	CString	m_JamFileName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJamSelect)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CJamSelect)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeJamname();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
