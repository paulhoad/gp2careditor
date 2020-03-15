// CarEditorDoc.h : interface of the CCarEditorDoc class
//
/////////////////////////////////////////////////////////////////////////////
#include "car.h"

class CCarEditorDoc : public CDocument
{
 protected:  // create from serialization only
  CCarEditorDoc();
  DECLARE_DYNCREATE(CCarEditorDoc)

  // Attributes
 public:
  Car *car;
  Helmet *helmet;
  GPOObject *gpoobject;
  int selectedScale;

  void
  DeleteContents();

  JAM *currentJam;

  CBitmap currentJamBitmap;

  JAM *
  getCurrentJam()
  {
    return currentJam;
  }

  void
  setCurrentJam(JAM *jam)
  {
    currentJam = jam;
  }

  GPOObject *
  ImportGPOObject(CString filename);

  // Operations
 public:
  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CCarEditorDoc)
 public:
  virtual BOOL
  OnNewDocument();
  virtual void
  Serialize(CArchive &ar);
  virtual BOOL
  OnOpenDocument(LPCTSTR lpszPathName);
  //}}AFX_VIRTUAL

  // Implementation
 public:
  virtual ~CCarEditorDoc();
#ifdef _DEBUG
  virtual void
  AssertValid() const;
  virtual void
  Dump(CDumpContext &dc) const;
#endif

 protected:
  // Generated message map functions
 protected:
  //{{AFX_MSG(CCarEditorDoc)
  afx_msg void
  OnFileimport();
  afx_msg void
  OnFileSaveAs();
  afx_msg void
  OnFileexport();
  afx_msg void
  OnHighNose();
  afx_msg void
  OnUpdateHighNose(CCmdUI *pCmdUI);
  afx_msg void
  OnCopyCar1ToCar2();
  afx_msg void
  OnCarone();
  afx_msg void
  OnUpdateCarone(CCmdUI *pCmdUI);
  afx_msg void
  OnCartwo();
  afx_msg void
  OnUpdateCartwo(CCmdUI *pCmdUI);
  afx_msg void
  OnFileSave();
  afx_msg void
  OnJamView();
  afx_msg void
  OnFileImporthelmet();
  afx_msg void
  OnSaveHelmet();
  afx_msg void
  OnFileImportgpoobject();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
