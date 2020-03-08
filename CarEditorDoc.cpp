// CarEditorDoc.cpp : implementation of the CCarEditorDoc class
//

#include "stdafx.h"
#include "CarEditor.h"

#include "CarEditorDoc.h"
#include "Car.h"
#include "CarTree.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCarEditorDoc

IMPLEMENT_DYNCREATE(CCarEditorDoc, CDocument)

BEGIN_MESSAGE_MAP(CCarEditorDoc, CDocument)
	//{{AFX_MSG_MAP(CCarEditorDoc)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_COMMAND(ID_FILEEXPORT, OnFileexport)
	ON_COMMAND(ID_HIGH_NOSE, OnHighNose)
	ON_UPDATE_COMMAND_UI(ID_HIGH_NOSE, OnUpdateHighNose)
	ON_COMMAND(ID_COPY_CAR1_TO_CAR2, OnCopyCar1ToCar2)
	ON_COMMAND(ID_CARONE, OnCarone)
	ON_UPDATE_COMMAND_UI(ID_CARONE, OnUpdateCarone)
	ON_COMMAND(ID_CARTWO, OnCartwo)
	ON_UPDATE_COMMAND_UI(ID_CARTWO, OnUpdateCartwo)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_JAM_VIEW, OnJamView)
	ON_COMMAND(ID_FILE_IMPORTHELMET, OnFileImporthelmet)
	ON_COMMAND(ID_SAVE_HELMET, OnSaveHelmet)
	ON_COMMAND(ID_FILE_IMPORTGPOOBJECT, OnFileImportgpoobject)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCarEditorDoc construction/destruction

CCarEditorDoc::CCarEditorDoc()
{
	car = NULL;
	helmet = NULL;
	gpoobject = NULL;
	selectedScale = -1;
	currentJam = NULL;
}

CCarEditorDoc::~CCarEditorDoc()
{
	if (car) delete car;
}

BOOL CCarEditorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	char buffer[1024];

	CString gp2Location = ((CCarEditorApp*)AfxGetApp())->getGP2Location();

	wsprintf(buffer,"%s\\gp2.exe",gp2Location);

	car = ImportCar(buffer);
	helmet = ImportHelmet(buffer);
	ImportCameras(car,buffer);
	OnFileimport();

	if (car == NULL)
	{
	  char msg[1024];

	  wsprintf(msg,"Bad Exe Path %s",buffer);
	  AfxMessageBox(msg,MB_OK);
	  return FALSE;
	}

	if (!car->isValid())
	{
		delete car;
		car = NULL;
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CCarEditorDoc serialization

void CCarEditorDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CCarEditorDoc diagnostics

#ifdef _DEBUG
void CCarEditorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCarEditorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCarEditorDoc commands

BOOL CCarEditorDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: Add your specialized creation code here
	car = LoadCar(lpszPathName);
	//OnFileimport();
	((CCarEditorApp*)AfxGetApp())->m_MainTree->InsertCar(this,car);
	
	return TRUE;
}

void CCarEditorDoc::OnFileimport() 
{
	if (car)((CCarEditorApp*)AfxGetApp())->m_MainTree->InsertCar(this,car);
	if (helmet) ((CCarEditorApp*)AfxGetApp())->m_MainTree->InsertHelmet(this,helmet);
}

char lpszFileFilter[] = "F1GP2 Car Files (*.shp)|*.shp|All Files (*.*)|*.*|";

void CCarEditorDoc::OnFileSaveAs() 
{
	if (car)
	{
	 char fileName[256];

	 strcpy(fileName,"unnamed.dat");
	 CFileDialog *fdlg = new CFileDialog(FALSE,"dat",(LPCSTR)&fileName,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,lpszFileFilter);

	 INT_PTR result = fdlg->DoModal();

	 if (result==IDOK)
	 {
	 CString filename = fdlg->GetFileName();
     
	 car->write(filename);
	 }
	}
}




void CCarEditorDoc::OnFileexport() 
{
	char buffer[1024];

	CString gp2Location = ((CCarEditorApp*)AfxGetApp())->getGP2Location();

	wsprintf(buffer,"%s\\gp2.exe",gp2Location);

	ExportCar(buffer,car);
	ExportCameras(buffer,car);
	if (helmet) ExportHelmet(buffer,helmet);
}

void CCarEditorDoc::OnHighNose() 
{
	if (car!=NULL)
	{
	 car->HiNose = !car->HiNose;
	 car->updateData(FALSE);
	 UpdateAllViews(NULL);
	}
}

void CCarEditorDoc::DeleteContents()
{
	if (car!=NULL && car->carNode)
	{
	  ((CCarEditorApp*)AfxGetApp())->m_MainTree->m_CarTree.DeleteItem(car->carNode);
	}
	if (helmet!=NULL && helmet->carNode)
	{
		((CCarEditorApp*)AfxGetApp())->m_MainTree->m_CarTree.DeleteItem(helmet->carNode);
	}
}

void CCarEditorDoc::OnUpdateHighNose(CCmdUI* pCmdUI) 
{
	
	if (car!=NULL)
		pCmdUI->SetCheck(car->HiNose);
}

void CCarEditorDoc::OnCopyCar1ToCar2() 
{
	int result = AfxMessageBox("Copying Car 1 to Car 2, Are you sure?",MB_YESNO);

	if (result == IDYES)
	{
		for(int i=0;i<32;i++)
		{
			car->scaleData[i+31].val = car->scaleData[i].val;
			car->scaleData[i+31].setChanged();
		}
		car->PatchData();
		car->updateData(FALSE);
	}
}

void CCarEditorDoc::OnCarone() 
{
	if (car) 
	{
		car->HiNose = FALSE;
	    car->updateData(FALSE);
	}
	UpdateAllViews(NULL);
}

void CCarEditorDoc::OnUpdateCarone(CCmdUI* pCmdUI) 
{
	if (car!=NULL)
		pCmdUI->SetCheck(!car->HiNose);
}

void CCarEditorDoc::OnCartwo() 
{
	if (car) 
	{
		car->HiNose = TRUE;
	    car->updateData(FALSE);
	}
	UpdateAllViews(NULL);
}

void CCarEditorDoc::OnUpdateCartwo(CCmdUI* pCmdUI) 
{
	if (car!=NULL)
		pCmdUI->SetCheck(car->HiNose);

}

void CCarEditorDoc::OnFileSave() 
{
	OnFileSaveAs();	
}

void CCarEditorDoc::OnJamView() 
{
	((CCarEditorApp*)AfxGetApp())->OnJamView(this);
}

char lpszFileHelmetFilter[] = "F1GP2 Helmet Files (*.hlt)|*.hlt|All Files (*.*)|*.*|";


void CCarEditorDoc::OnFileImporthelmet() 
{
	char fileName[256];

	 strcpy(fileName,"unnamed.hlt");
	 CFileDialog *fdlg = new CFileDialog(TRUE,"hlt",(LPCSTR)&fileName,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,lpszFileHelmetFilter);

	 INT_PTR result = fdlg->DoModal();

	 if (result==IDOK)
	 {
	 CString filename = fdlg->GetFileName();

	 helmet = LoadHelmet(filename);
	 }
     
	if (helmet) ((CCarEditorApp*)AfxGetApp())->m_MainTree->InsertHelmet(this,helmet);
}

void CCarEditorDoc::OnSaveHelmet() 
{
	if (helmet)
	{
	 char fileName[256];

	 strcpy(fileName,"unnamed.hlt");
	 CFileDialog *fdlg = new CFileDialog(FALSE,"hlt",(LPCSTR)&fileName,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,lpszFileHelmetFilter);

	 INT_PTR result = fdlg->DoModal();

	 if (result==IDOK)
	 {
	 CString filename = fdlg->GetFileName();
     
	 helmet->write(filename);
	 }
	}
}


char lpszFileGPOFilter[] = "F1GP2 Object Files (*.gpo)|*.gpo|All Files (*.*)|*.*|";


void CCarEditorDoc::OnFileImportgpoobject() 
{
	 char fileName[256];

	 strcpy(fileName,"*.gpo");
	 CFileDialog *fdlg = new CFileDialog(TRUE,"gpo",(LPCSTR)&fileName,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,lpszFileGPOFilter);

	 INT_PTR result = fdlg->DoModal();

	 if (result==IDOK)
	 {
	 CString filename = fdlg->GetFileName();

	 gpoobject = ImportGPOObject(filename);
	 gpoobject->parse(0);
	 if (gpoobject) ((CCarEditorApp*)AfxGetApp())->m_MainTree->InsertGPOObject(this,gpoobject);
	 }
}

static char * getObjectline(FILE *fp)
{
	static char buffer[512];

	fscanf(fp,"%[^\n]\n",buffer);
	return buffer;
}


GPOObject * CCarEditorDoc::ImportGPOObject(CString filename)
{
	  FILE *fp = fopen(filename,"r");

	  if (fp==NULL)
	  {
		  AfxMessageBox("Failed to open import file");
		  return FALSE;
	  }

	  GPOObject *obj = new GPOObject();


	  // get number of datavalues
      int listsize=0;

	  while(!feof(fp))
	  {
		  char *line = getObjectline(fp);

		  if (line[0] == '#') continue;
		  if (line[0] == '/' && line[1] == '/') continue;
		  
		  sscanf(line,"%d",&listsize);break;
	  }

	  int objectLength = 0;
	  int count=0;

	  for(int i=0;i<listsize;i++)
	  {
		 char *line = getObjectline(fp);

		 // don't gobble lines
		  if (line[0] == '#') 
		  {
			  i--;
			  continue;
		  }
		  if (line[0] == '/' && line[1] == '/')
		  {
			  i--;
			  continue;
		  }

		  int offset;
		  int size;
		  int value;

		  sscanf(line,"%d %d 0x%x",&offset,&size,&value);
		  objectLength+=size;
		  switch(size)
		  {
            case 2:
			   obj->data[count++] = LOBYTE(value);
			   obj->data[count++] = HIBYTE(value);
			   break;
			case 4:
			   obj->data[count++] = LOBYTE(LOWORD(value));
			   obj->data[count++] = HIBYTE(LOWORD(value));
			   obj->data[count++] = LOBYTE(HIWORD(value));
			   obj->data[count++] = HIBYTE(HIWORD(value));
			   break;
		  }
	  }

	  fclose(fp);
	  return obj;
}
