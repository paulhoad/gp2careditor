// CamEdit.cpp : implementation file
//

#include "stdafx.h"
#include "CarEditor.h"
#include "CamEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCamEdit dialog


CCamEdit::CCamEdit(CWnd* pParent /*=NULL*/)
	: CDialog(CCamEdit::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCamEdit)
	m_X = 0;
	m_Y = 0;
	m_Z = 0;
	m_Dir = 0;
	m_Pitch = 0;
	m_Roll = 0;
	m_Detail1 = 0;
	m_Detail2 = 0;
	//}}AFX_DATA_INIT
}


void CCamEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCamEdit)
	DDX_Text(pDX, IDC_EDIT1, m_X);
	DDX_Text(pDX, IDC_EDIT2, m_Y);
	DDX_Text(pDX, IDC_EDIT3, m_Z);
	DDX_Text(pDX, IDC_EDIT4, m_Dir);
	DDX_Text(pDX, IDC_EDIT5, m_Pitch);
	DDX_Text(pDX, IDC_EDIT6, m_Roll);
	DDX_Text(pDX, IDC_EDIT7, m_Detail1);
	DDX_Text(pDX, IDC_EDIT8, m_Detail2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCamEdit, CDialog)
	//{{AFX_MSG_MAP(CCamEdit)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCamEdit message handlers
