// DataValue.cpp : implementation file
//

#include "stdafx.h"
#include "CarEditor.h"
#include "DataValue.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDataValue dialog


CDataValue::CDataValue(CWnd* pParent /*=NULL*/)
	: CDialog(CDataValue::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDataValue)
	m_DataVal = 0;
	//}}AFX_DATA_INIT
}


void CDataValue::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDataValue)
	DDX_Text(pDX, IDC_EDIT1, m_DataVal);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDataValue, CDialog)
	//{{AFX_MSG_MAP(CDataValue)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataValue message handlers
