// TriggerDist.cpp : implementation file
//

#include "stdafx.h"
#include "CarEditor.h"
#include "TriggerDist.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTriggerDist dialog

CTriggerDist::CTriggerDist(CWnd* pParent /*=NULL*/)
    : CDialog(CTriggerDist::IDD, pParent)
{
  //{{AFX_DATA_INIT(CTriggerDist)
  m_Distance = 0;
  //}}AFX_DATA_INIT
}

void
CTriggerDist::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CTriggerDist)
  DDX_Text(pDX, IDC_EDIT1, m_Distance);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTriggerDist, CDialog)
//{{AFX_MSG_MAP(CTriggerDist)
ON_BN_CLICKED(IDC_DEFAULT_VAL, OnDefaultVal)
ON_BN_CLICKED(IDC_RCRONLY, OnRcronly)
ON_BN_CLICKED(IDC_SHAPEDEFAULT, OnShapedefault)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTriggerDist message handlers

void
CTriggerDist::OnDefaultVal()
{
  m_Distance = 3200;
  UpdateData(FALSE);
}

void
CTriggerDist::OnRcronly()
{
  m_Distance = 0;
  UpdateData(FALSE);
}

void
CTriggerDist::OnShapedefault()
{
  m_Distance = 10000;
  UpdateData(FALSE);
}
