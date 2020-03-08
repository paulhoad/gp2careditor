// CarEditorView.h : interface of the CCarEditorView class
//
/////////////////////////////////////////////////////////////////////////////
#include "OpenGLView.h"

enum t_CameraView { t_Air,t_Cam1,t_Cam2,t_Cam3,t_Cam4, t_Cam5, t_Cam6, t_Cam7, t_Cam8, t_Cam9 };

class CCarEditorView : public COpenGLView
{
protected: // create from serialization only
	CCarEditorView();
	DECLARE_DYNCREATE(CCarEditorView)

// Attributes
public:
	CCarEditorDoc* GetDocument();

	BOOL showFilled;
	BOOL showPoints;
	BOOL showHelmet;
	BOOL showTyres;
	BOOL showAxis;
	BOOL showCameras;
	BOOL showTexture;
	BOOL showShadowBox;
	BOOL showLines;
	BOOL showCulling;
	BOOL showNormals;
	BOOL showSingleColor;
	BOOL showLighting;
	BOOL showCar;
	BOOL showPointNumbers;
	BOOL showVertexNumbers;
	BOOL showPolygonNumbers;
	BOOL showCameraNumbers;
	BOOL showNoseDetail;
	BOOL showObject;

	BOOL Animation;

	t_CameraView currentView;

	CString DefaultJamFileName;
	CBitmap tyreface;
	CBitmap tyre;
	CBitmap helmets;
	CBitmap shadow;

	void UpdateJamFile();

	void getLookPoint();

	void TextureMap(CBitmap &cbmp);
	void DrawHelmetModel();
	void DrawObjectModel();

	void ReReadJams();


// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCarEditorView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCarEditorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	void OpenGLDrawScene(HDC hdc);

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CCarEditorView)
	afx_msg void OnFillobjects();
	afx_msg void OnUpdateFillobjects(CCmdUI* pCmdUI);
	afx_msg void OnShowPoints();
	afx_msg void OnUpdateShowPoints(CCmdUI* pCmdUI);
	afx_msg void OnShowtyres();
	afx_msg void OnUpdateShowtyres(CCmdUI* pCmdUI);
	afx_msg void OnHelmet();
	afx_msg void OnDrawaxis();
	afx_msg void OnUpdateDrawaxis(CCmdUI* pCmdUI);
	afx_msg void OnShowcameras();
	afx_msg void OnUpdateShowcameras(CCmdUI* pCmdUI);
	afx_msg void OnTexturemaps();
	afx_msg void OnUpdateTexturemaps(CCmdUI* pCmdUI);
	afx_msg void OnUpdateHelmet(CCmdUI* pCmdUI);
	afx_msg void OnShowShadow();
	afx_msg void OnUpdateShowShadow(CCmdUI* pCmdUI);
	afx_msg void OnShowLines();
	afx_msg void OnUpdateShowLines(CCmdUI* pCmdUI);
	afx_msg void OnShowCulling();
	afx_msg void OnUpdateShowCulling(CCmdUI* pCmdUI);
	afx_msg void OnShowNormals();
	afx_msg void OnUpdateShowNormals(CCmdUI* pCmdUI);
	afx_msg void OnShowSingleColor();
	afx_msg void OnUpdateShowSingleColor(CCmdUI* pCmdUI);
	afx_msg void OnLighting();
	afx_msg void OnUpdateLighting(CCmdUI* pCmdUI);
	afx_msg void OnShowCar();
	afx_msg void OnUpdateShowCar(CCmdUI* pCmdUI);
	afx_msg void OnShowPolygonNumbers();
	afx_msg void OnUpdateShowPolygonNumbers(CCmdUI* pCmdUI);
	afx_msg void OnShowPointnumbers();
	afx_msg void OnUpdateShowPointnumbers(CCmdUI* pCmdUI);
	afx_msg void OnShowVertexNumbers();
	afx_msg void OnUpdateShowVertexNumbers(CCmdUI* pCmdUI);
	afx_msg void OnShowCameraNumbers();
	afx_msg void OnUpdateShowCameraNumbers(CCmdUI* pCmdUI);
	afx_msg void OnJamSet();
	afx_msg void OnCameraViewfromcamera1();
	afx_msg void OnCameraViewfromcamera2();
	afx_msg void OnCameraViewfromcamera3();
	afx_msg void OnCameraViewfromcamera4();
	afx_msg void OnCameraViewfromcamera5();
	afx_msg void OnCameraViewfromcamera6();
	afx_msg void OnCameraViewfromcamera7();
	afx_msg void OnCameraViewfromcamera8();
	afx_msg void OnCameraViewfromcamera9();
	afx_msg void OnUpdateCameraViewfromcamera1(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCameraViewfromcamera2(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCameraViewfromcamera3(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCameraViewfromcamera4(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCameraViewfromcamera5(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCameraViewfromcamera6(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCameraViewfromcamera7(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCameraViewfromcamera8(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCameraViewfromcamera9(CCmdUI* pCmdUI);
	afx_msg void OnNormalView();
	afx_msg void OnUpdateNormalView(CCmdUI* pCmdUI);
	afx_msg void OnCameraViewrightside();
	afx_msg void OnCameraViewleftside();
	afx_msg void OnCameraViewfront();
	afx_msg void OnCameraViewtop();
	afx_msg void OnCameraViewback();
	afx_msg void OnCameraViewbottom();
	afx_msg void OnCameraViewfrontleft();
	afx_msg void OnCameraViewfrontright();
	afx_msg void OnCameraViewbackleft();
	afx_msg void OnCameraViewbackright();
	afx_msg void OnStartAnimation();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnUpdateStartAnimation(CCmdUI* pCmdUI);
	afx_msg void OnZoomOut();
	afx_msg void OnZoomIn();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnShowNoseCone();
	afx_msg void OnUpdateShowNoseCone(CCmdUI* pCmdUI);
	afx_msg void OnJamExportrcrjam();
	afx_msg void OnShowObject();
	afx_msg void OnUpdateShowObject(CCmdUI* pCmdUI);
	afx_msg void OnJamDrawrcrjam();
	afx_msg void OnUpdateJamDrawrcrjam(CCmdUI* pCmdUI);
	afx_msg void OnJamProducercrimages();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
