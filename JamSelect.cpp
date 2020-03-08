// JamSelect.cpp : implementation file
//

#include "stdafx.h"
#include "CarEditor.h"
#include "JamSelect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CJamSelect dialog


CJamSelect::CJamSelect(CWnd* pParent /*=NULL*/)
	: CDialog(CJamSelect::IDD, pParent)
{
	//{{AFX_DATA_INIT(CJamSelect)
	m_JamFileName = _T("");
	//}}AFX_DATA_INIT
}


void CJamSelect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CJamSelect)
	DDX_Control(pDX, IDC_JAMNAME, m_Names);
	DDX_CBString(pDX, IDC_JAMNAME, m_JamFileName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CJamSelect, CDialog)
	//{{AFX_MSG_MAP(CJamSelect)
	ON_CBN_SELCHANGE(IDC_JAMNAME, OnSelchangeJamname)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CJamSelect message handlers

BOOL CJamSelect::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_Names.AddString("ferrari.jam");
	m_Names.AddString("benetton.jam");
	m_Names.AddString("footwork.jam");
	m_Names.AddString("simtek.jam");
	m_Names.AddString("jordan.jam");
	m_Names.AddString("mclaren.jam");
	m_Names.AddString("williams.jam");
	m_Names.AddString("tyrrell.jam");
	m_Names.AddString("sauber.jam");
	m_Names.AddString("larrous.jam");
	m_Names.AddString("pacific.jam");
	m_Names.AddString("lotus.jam");
	m_Names.AddString("ligier.jam");
	m_Names.AddString("minardi.jam");
	m_Names.SetCurSel(0);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CJamSelect::OnSelchangeJamname() 
{
	if (m_Names.GetCurSel() != LB_ERR) {
		m_Names.GetLBText(m_Names.GetCurSel(), m_JamFileName);
	}
}
