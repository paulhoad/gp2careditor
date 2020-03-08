// CamEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCamEdit dialog

class CCamEdit : public CDialog
{
// Construction
public:
	CCamEdit(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCamEdit)
	enum { IDD = IDD_CAMEDIT };
	int		m_X;
	int		m_Y;
	int		m_Z;
	int		m_Dir;
	int		m_Pitch;
	int		m_Roll;
	int		m_Detail1;
	int		m_Detail2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCamEdit)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCamEdit)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
