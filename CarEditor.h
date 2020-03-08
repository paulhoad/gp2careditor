// CarEditor.h : main header file for the CAREDITOR application
//

#ifndef _CAREDITOR_INCLUDED_H
#define _CAREDITOR_INCLUDED_H

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

class CCarTree;

#include "MainFrm.h"

extern void SetStatusText(LPCSTR text);



/////////////////////////////////////////////////////////////////////////////
// CCarEditorApp:
// See CarEditor.cpp for the implementation of this class
//

class CCarEditorApp : public CWinApp
{
public:
	CCarEditorApp();
	~CCarEditorApp();

	CString getGP2Location();
	void OnEditLocation();
	void OnGp2exe();

	CMultiDocTemplate* pDocTemplate;
	CMultiDocTemplate* pJamFileTemplate;

	void LoadImages();
	CImageList* pImageList;

	CImageList* getSharedImageList()
	{
		return pImageList;
	}

	void OnJamView(CDocument *pDoc);

	
	CCarTree *m_MainTree;

	UINT m_AnimationSpeed;

	BOOL useExternalJams;
	BOOL drawRCRJam;
	CMainFrame* pMainFrame;

	void SetMessageText(LPCSTR str)
	{
		if (pMainFrame)
		{
			pMainFrame->SetMessageText(str);
		}
	}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCarEditorApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCarEditorApp)
	afx_msg void OnAppAbout();
	afx_msg void OnEditgp2location();
	afx_msg void OnGp2Exe();
	afx_msg void OnFileimport();
	afx_msg void OnEditDistanceTrigger();
	afx_msg void OnSetAnimationDelay();
	afx_msg void OnInternalJam();
	afx_msg void OnUpdateInternalJam(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

#endif
