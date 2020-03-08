// AnimationSpeed.cpp : implementation file
//

#include "stdafx.h"
#include "CarEditor.h"
#include "AnimationSpeed.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAnimationSpeed dialog


CAnimationSpeed::CAnimationSpeed(CWnd* pParent /*=NULL*/)
	: CDialog(CAnimationSpeed::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAnimationSpeed)
	m_Speed = 0;
	//}}AFX_DATA_INIT
}


void CAnimationSpeed::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAnimationSpeed)
	DDX_Text(pDX, IDC_EDIT1, m_Speed);
	DDV_MinMaxUInt(pDX, m_Speed, 0, 65535);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAnimationSpeed, CDialog)
	//{{AFX_MSG_MAP(CAnimationSpeed)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAnimationSpeed message handlers
