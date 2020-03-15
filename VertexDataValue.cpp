// VertexDataValue.cpp : implementation file
//

#include "stdafx.h"
#include "CarEditor.h"
#include "VertexDataValue.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVertexDataValue dialog

CVertexDataValue::CVertexDataValue(CWnd* pParent /*=NULL*/)
    : CDialog(CVertexDataValue::IDD, pParent)
{
  //{{AFX_DATA_INIT(CVertexDataValue)
  m_From = 0;
  m_To = 0;
  //}}AFX_DATA_INIT
}

void
CVertexDataValue::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CVertexDataValue)
  DDX_Text(pDX, IDC_EDIT1, m_From);
  DDV_MinMaxInt(pDX, m_From, 0, 255);
  DDX_Text(pDX, IDC_EDIT2, m_To);
  DDV_MinMaxInt(pDX, m_To, 0, 255);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CVertexDataValue, CDialog)
//{{AFX_MSG_MAP(CVertexDataValue)
// NOTE: the ClassWizard will add message map macros here
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVertexDataValue message handlers
