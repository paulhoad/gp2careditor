// Point3DDataValue.cpp : implementation file
//

#include "stdafx.h"
#include "CarEditor.h"
#include "Point3DDataValue.h"
#include "Car.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPoint3DDataValue dialog

CPoint3DDataValue::CPoint3DDataValue(Object3D* obj, CWnd* pParent /*=NULL*/)
    : CDialog(CPoint3DDataValue::IDD, pParent), obj(obj)
{
  //{{AFX_DATA_INIT(CPoint3DDataValue)
  m_X = 0;
  m_Y = 0;
  m_Z = 0;
  m_OY = 0;
  m_OX = 0;
  m_XScale = _T("");
  m_YScale = _T("");
  m_XSign = FALSE;
  m_YSign = FALSE;
  //}}AFX_DATA_INIT
}

void
CPoint3DDataValue::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CPoint3DDataValue)
  DDX_Control(pDX, IDC_COMBO4, m_ScaleYCombo);
  DDX_Control(pDX, IDC_COMBO3, m_ScaleXCombo);
  DDX_Text(pDX, IDC_EDIT1, m_X);
  DDX_Text(pDX, IDC_EDIT2, m_Y);
  DDX_Text(pDX, IDC_EDIT3, m_Z);
  DDX_Text(pDX, IDC_EDIT5, m_OY);
  DDX_Text(pDX, IDC_EDIT4, m_OX);
  DDX_Text(pDX, IDC_XSCALE, m_XScale);
  DDX_Text(pDX, IDC_YSCALE, m_YScale);
  DDX_Check(pDX, IDC_X_SIGN, m_XSign);
  DDX_Check(pDX, IDC_Y_SIGN, m_YSign);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPoint3DDataValue, CDialog)
//{{AFX_MSG_MAP(CPoint3DDataValue)
ON_BN_CLICKED(IDC_X_SIGN, OnXSign)
ON_BN_CLICKED(IDC_Y_SIGN, OnYSign)
ON_EN_CHANGE(IDC_EDIT4, OnChangeEdit4)
ON_EN_CHANGE(IDC_EDIT5, OnChangeEdit5)
ON_CBN_SELCHANGE(IDC_COMBO3, OnSelchangeCombo3)
ON_CBN_SELCHANGE(IDC_COMBO4, OnSelchangeCombo4)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPoint3DDataValue message handlers

BOOL
CPoint3DDataValue::OnInitDialog()
{
  CDialog::OnInitDialog();

  // TODO: Add extra initialization here

  for (int i = 0; i < 32; i++) {
    char msg[256];
    wsprintf(msg, "%d", obj->scaleData[i].val);
    m_ScaleYCombo.AddString(msg);
    m_ScaleXCombo.AddString(msg);
  }
  for (int i = 0; i < obj->numPoints; i++) {
    char msg[256];
    wsprintf(msg, "p[%d]", i);
    m_ScaleXCombo.AddString(msg);
  }

  /*
  for(i=0;i<32;i++)
  {
     char msg[256];
     wsprintf(msg,"-%d",obj->scaleData[i].val);
     m_ScaleYCombo.AddString(msg);
     m_ScaleXCombo.AddString(msg);
  }
  */

  if (m_OX < 128) {
    int idx = m_OX / 4;
    m_ScaleXCombo.SetCurSel(idx - 1);
  }
  else if (m_OX < 0xFF) {
    int idx = (m_OX - 128) / 4;
    m_ScaleXCombo.SetCurSel(idx - 1);
  }

  if (m_OY < 128) {
    int idx = m_OY / 4;
    m_ScaleYCombo.SetCurSel(idx - 1);
  }
  else if (m_OY < 0xFF) {
    int idx = (m_OY - 128) / 4;
    m_ScaleYCombo.SetCurSel(idx - 1);
  }

  OnChangeEdit4();
  OnChangeEdit5();

  return TRUE;  // return TRUE unless you set the focus to a control
                // EXCEPTION: OCX Property Pages should return FALSE
}

void
CPoint3DDataValue::OnXSign()
{
  // TODO: Add your control notification handler code here
  if (m_XSign && m_OX >= 132 && m_OX < 0xFF) {
    m_OX = m_OX - 128;
    m_XSign = FALSE;
  }
  else if (!m_XSign && m_OX < 132) {
    m_OX += 128;
    m_XSign = TRUE;
  }
  else {
    AfxMessageBox("Cannot Change sign of predefined point", MB_OK);
  }
  UpdateXY();
  UpdateData(FALSE);
}

void
CPoint3DDataValue::OnYSign()
{
  if (m_YSign && m_OY >= 132 && m_OY < 0xFF) {
    m_OY = m_OY - 128;
    m_YSign = FALSE;
  }
  else if (!m_YSign && m_OY < 132) {
    m_OY += 128;
    m_YSign = TRUE;
  }
  else {
    AfxMessageBox("Cannot Change sign of predefined point", MB_OK);
  }
  UpdateXY();
  UpdateData(FALSE);
}

void
CPoint3DDataValue::OnChangeEdit4()
{
  if (m_OX >= 132 && m_OX < 0xFF)
    m_XSign = TRUE;
  else
    m_XSign = FALSE;
  UpdateXY();
  UpdateData(FALSE);
}

void
CPoint3DDataValue::OnChangeEdit5()
{
  if (m_OY >= 132 && m_OY < 0xFF)
    m_YSign = TRUE;
  else
    m_YSign = FALSE;
  UpdateXY();
  UpdateData(FALSE);
}

void
CPoint3DDataValue::OnSelchangeCombo3()
{
  // TODO: Add your control notification handler code here
  int idx = m_ScaleXCombo.GetCurSel();

  if (idx < 32) {
    if (m_XSign) {
      m_OX = 128 + ((idx + 1) * 4);
    }
    else {
      m_OX = (idx + 1) * 4;
    }
  }
  else {
    m_OX = 0x8001 + (idx - 32);
  }
  UpdateXY();
  UpdateData(FALSE);
}

void
CPoint3DDataValue::OnSelchangeCombo4()
{
  int idx = m_ScaleYCombo.GetCurSel();

  if (m_YSign) {
    m_OY = 128 + ((idx + 1) * 4);
  }
  else {
    m_OY = (idx + 1) * 4;
  }

  UpdateXY();
  UpdateData(FALSE);
}

void
CPoint3DDataValue::UpdateXY()
{
  if (m_OX < 257) {
    int idx = m_ScaleXCombo.GetCurSel();
    int scaleval = obj->scaleData[idx].val;

    if (m_XSign) scaleval = -scaleval;
    m_X = scaleval;

    idx = m_ScaleYCombo.GetCurSel();
    scaleval = obj->scaleData[idx].val;
    if (m_YSign) scaleval = -scaleval;
    m_Y = scaleval;
  }
  else {
    int idx = m_OX - 0x8001;
    m_X = obj->pointData[idx].x;
    m_Y = obj->pointData[idx].y;
  }
}
