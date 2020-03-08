// VertexDataValue.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CVertexDataValue dialog

class CVertexDataValue : public CDialog
{
// Construction
public:
	CVertexDataValue(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CVertexDataValue)
	enum { IDD = IDD_VERTEXVALUE };
	int		m_From;
	int		m_To;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVertexDataValue)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CVertexDataValue)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
