// CarEditor.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "CarEditor.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "CarEditorDoc.h"
#include "CarEditorView.h"
#include "TriggerDist.h"
#include "JamView.h"

#include "AnimationSpeed.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "Shlobj.h"

void
SetStatusText(LPCSTR text)
{
  ((CCarEditorApp*)AfxGetApp())->SetMessageText(text);
}

/////////////////////////////////////////////////////////////////////////////
// CCarEditorApp

BEGIN_MESSAGE_MAP(CCarEditorApp, CWinApp)
//{{AFX_MSG_MAP(CCarEditorApp)
ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
ON_COMMAND(ID_EDIT_EDITGP2LOCATION, OnEditgp2location)
ON_COMMAND(ID_GP2_EXE, OnGp2Exe)
ON_COMMAND(ID_FILEIMPORT, OnFileimport)
ON_COMMAND(ID_EDIT_DISTANCE_TRIGGER, OnEditDistanceTrigger)
ON_COMMAND(ID_SET_ANIMATION_DELAY, OnSetAnimationDelay)
ON_COMMAND(ID_INTERNAL_JAM, OnInternalJam)
ON_UPDATE_COMMAND_UI(ID_INTERNAL_JAM, OnUpdateInternalJam)
//}}AFX_MSG_MAP
// Standard file based document commands
ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
// Standard print setup command
ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCarEditorApp construction

CCarEditorApp::CCarEditorApp()
{
  // TODO: add construction code here,
  // Place all significant initialization in InitInstance
  m_MainTree = NULL;
  m_AnimationSpeed = 15;
  useExternalJams = TRUE;
}

CCarEditorApp::~CCarEditorApp()
{
  if (pImageList != NULL) delete pImageList;
}
/////////////////////////////////////////////////////////////////////////////
// The one and only CCarEditorApp object

CCarEditorApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CCarEditorApp initialization

BOOL
CCarEditorApp::InitInstance()
{
  // Standard initialization
  // If you are not using these features and wish to reduce the size
  //  of your final executable, you should remove from the following
  //  the specific initialization routines you do not need.

#ifdef _AFXDLL
  Enable3dControls();  // Call this when using MFC in a shared DLL
#else
  // Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

  SetRegistryKey("GP2CarEditorProject\\CarEditor");

  LoadStdProfileSettings();  // Load standard INI file options (including MRU)

  // Register the application's document templates.  Document templates
  //  serve as the connection between documents, frame windows and views.

  LoadImages();

  pDocTemplate = new CMultiDocTemplate(
      IDR_CAREDITYPE, RUNTIME_CLASS(CCarEditorDoc),
      RUNTIME_CLASS(CChildFrame),  // custom MDI child frame
      RUNTIME_CLASS(CCarEditorView));
  AddDocTemplate(pDocTemplate);

  pDocTemplate = new CMultiDocTemplate(
      IDR_CAREDITYPE_OLD, RUNTIME_CLASS(CCarEditorDoc),
      RUNTIME_CLASS(CChildFrame),  // custom MDI child frame
      RUNTIME_CLASS(CCarEditorView));
  AddDocTemplate(pDocTemplate);

  // pJamFileTemplate = new CMultiDocTemplate(IDR_CAREDITYPE,
  //		RUNTIME_CLASS(CCarEditorDoc),
  //		RUNTIME_CLASS(CChildFrame),
  //		RUNTIME_CLASS(CJamView));
  // AddDocTemplate(pJamFileTemplate);

  OnAppAbout();

  EnableShellOpen();
  RegisterShellFileTypes(TRUE);

  // create main MDI Frame window
  pMainFrame = new CMainFrame;
  if (!pMainFrame->LoadFrame(IDR_MAINFRAME)) return FALSE;
  m_pMainWnd = pMainFrame;

  // Parse command line for standard shell commands, DDE, file open
  CCommandLineInfo cmdInfo;
  ParseCommandLine(cmdInfo);

  // Dispatch commands specified on the command line
  if (cmdInfo.m_nShellCommand != CCommandLineInfo::FileNew) {
    if (!ProcessShellCommand(cmdInfo)) return FALSE;
  }

  // The main window has been initialized, so show and update it.
  pMainFrame->ShowWindow(m_nCmdShow);
  pMainFrame->UpdateWindow();

  return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
 public:
  CAboutDlg();

  // Dialog Data
  //{{AFX_DATA(CAboutDlg)
  enum
  {
    IDD = IDD_ABOUTBOX
  };
  //}}AFX_DATA

  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CAboutDlg)
 protected:
  virtual void
  DoDataExchange(CDataExchange* pDX);  // DDX/DDV support
                                       //}}AFX_VIRTUAL

  // Implementation
 protected:
  //{{AFX_MSG(CAboutDlg)
  // No message handlers
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
  //{{AFX_DATA_INIT(CAboutDlg)
  //}}AFX_DATA_INIT
}

void
CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CAboutDlg)
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
//{{AFX_MSG_MAP(CAboutDlg)
// No message handlers
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void
CCarEditorApp::OnAppAbout()
{
  CAboutDlg aboutDlg;
  aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CCarEditorApp commands

void
CCarEditorApp::OnEditLocation()
{
  char fileName[256];

  strcpy(fileName, "*.*");

  BROWSEINFO browser;
  char buffer[MAX_PATH];

  browser.hwndOwner = NULL;
  browser.pszDisplayName = buffer;
  browser.lpszTitle = "Select GP2 Location";
  browser.lpfn = NULL;
  browser.pidlRoot = NULL;
  browser.ulFlags = BIF_RETURNONLYFSDIRS;

  LPITEMIDLIST files;

  files = SHBrowseForFolder(&browser);

  if (files != NULL) {
    char filename[256];

    if (SHGetPathFromIDList(files, filename)) {
      CString strSection = "Preferences";
      CString strStringItem = "GP2Location";
      CWinApp* pApp = AfxGetApp();

      pApp->WriteProfileString(strSection, strStringItem, CString(filename));
    }
    else {
      AfxMessageBox("Failed to Locate GP2 directory");
    }
  }
  else {
    AfxMessageBox("Failed to find GP2 directory");
  }
}

void
CCarEditorApp::OnGp2exe()
{
  OnGp2Exe();
}

CString
CCarEditorApp::getGP2Location()
{
  CString strSection = "Preferences";
  CString strStringItem = "GP2Location";
  CWinApp* pApp = AfxGetApp();

  CString circuitsDir =
      pApp->GetProfileString(strSection, strStringItem, "c:\\gp2\\");
  return circuitsDir;
}

void
CCarEditorApp::OnEditgp2location()
{
  OnEditLocation();
}

void
CCarEditorApp::OnGp2Exe()
{
  static char buffer[512];
  char workingDir[512];
  CString gp2Location = ((CCarEditorApp*)AfxGetApp())->getGP2Location();

  GetCurrentDirectory(512, workingDir);

  SetCurrentDirectory(gp2Location);
  wsprintf(buffer, "%s\\gp2.exe", gp2Location);

  WinExec(buffer, SW_SHOW);
  SetCurrentDirectory(workingDir);
}

void
CCarEditorApp::OnFileimport()
{
  CDocument* pDoc = pDocTemplate->CreateNewDocument();
  pDoc->OnNewDocument();
  CFrameWnd* pNewFrame = pDocTemplate->CreateNewFrame(pDoc, NULL);
  pDocTemplate->InitialUpdateFrame(pNewFrame, pDoc);
}

#define IDB_MIN IDB_CAR

void
CCarEditorApp::LoadImages()
{
  if (pImageList != NULL) delete pImageList;

  pImageList = new CImageList();

  int size = (IDB_MAX - IDB_MIN);
  pImageList->Create(16, 15, TRUE, size, 2);

  CBitmap bitmap;
  for (UINT iCnt = IDB_MIN; iCnt <= IDB_MAX; iCnt++) {
    bitmap.LoadBitmap(iCnt);
    pImageList->Add(&bitmap, (COLORREF)0xFFFFFF);
    bitmap.DeleteObject();
  }
}

void
CCarEditorApp::OnEditDistanceTrigger()
{
  char buffer[1024];

  CString gp2Location = ((CCarEditorApp*)AfxGetApp())->getGP2Location();

  wsprintf(buffer, "%s\\gp2.exe", gp2Location);

  FILE* fp = fopen(buffer, "rb+w");

  if (fp == NULL) {
    AfxMessageBox("Cannot find GP2.EXE", MB_OK);
    return;
  }

  long distance[1];

  if (!fseek(fp, 0x527f98, 0)) {
    fread(distance, 1, sizeof(long), fp);
  }

  CTriggerDist* dlg = new CTriggerDist();

  dlg->m_Distance = distance[0];

  INT_PTR result = dlg->DoModal();

  if (result == IDOK) {
    distance[0] = dlg->m_Distance;
    if (!fseek(fp, 0x527f98, 0)) {
      fwrite(distance, 1, sizeof(long), fp);
    }
  }

  fclose(fp);
}

void
CCarEditorApp::OnJamView(CDocument* pDoc)
{
  CFrameWnd* pNewFrame = pJamFileTemplate->CreateNewFrame(pDoc, NULL);
  pJamFileTemplate->InitialUpdateFrame(pNewFrame, pDoc);
}

void
CCarEditorApp::OnSetAnimationDelay()
{
  CAnimationSpeed* dlg = new CAnimationSpeed();

  dlg->m_Speed = m_AnimationSpeed;

  INT_PTR result = dlg->DoModal();

  if (result == IDOK) {
    m_AnimationSpeed = dlg->m_Speed;
  }
}

void
CCarEditorApp::OnInternalJam()
{
  useExternalJams = !useExternalJams;
}

void
CCarEditorApp::OnUpdateInternalJam(CCmdUI* pCmdUI)
{
  pCmdUI->SetCheck(useExternalJams);
}
