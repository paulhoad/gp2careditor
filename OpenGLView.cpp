// OpenGLView.cpp : implementation file
//

#include "stdafx.h"
#include "OpenGLView.h"
#include "RgbPal.h"
#include "Pallette.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#include <gl\glaux.h>
#include <gl\gl.h>
#include <gl\glu.h>

#include "BMP.h"

int winWidth = 512, winHeight = 256;

void
resize(void)
{
  // setProjection();
  // glViewport(0, 0, winWidth, winHeight);
}

/* Struct used to manage color ramps */
struct colorIndexState
{
  GLfloat amb[3];   /* ambient color / bottom of ramp */
  GLfloat diff[3];  /* diffuse color / middle of ramp */
  GLfloat spec[3];  /* specular color / top of ramp */
  GLfloat ratio;    /* ratio of diffuse to specular in ramp */
  GLint indexes[3]; /* where ramp was placed in palette */
};

/*
** Each entry in this array corresponds to a color ramp in the
** palette.  The indexes member of each struct is updated to
** reflect the placement of the color ramp in the palette.
*/
#define NUM_COLORS (sizeof(colors) / sizeof(colors[0]))
struct colorIndexState colors[] = {
    {
        {0.0F, 0.0F, 0.0F},
        {0.1F, 0.6F, 0.3F},
        {1.0F, 1.0F, 1.0F},
        0.75F,
        {0, 0, 0},
    },
    {
        {0.0F, 0.0F, 0.0F},
        {0.0F, 0.2F, 0.5F},
        {1.0F, 1.0F, 1.0F},
        0.75F,
        {0, 0, 0},
    },
    {
        {0.0F, 0.05F, 0.05F},
        {0.6F, 0.0F, 0.8F},
        {1.0F, 1.0F, 1.0F},
        0.75F,
        {0, 0, 0},
    },
};

/////////////////////////////////////////////////////////////////////////////
// COpenGLView

IMPLEMENT_DYNCREATE(COpenGLView, CScrollView)

COpenGLView::COpenGLView()
{
  m_LeftButtonDown = FALSE;
  scaler = 1.0;
  dist = 50.0;
  renderToDIB = FALSE;
  hBitmap = NULL;
  doubleBuffered = TRUE;
}

COpenGLView::~COpenGLView() {}

BEGIN_MESSAGE_MAP(COpenGLView, CScrollView)
//{{AFX_MSG_MAP(COpenGLView)
ON_WM_DESTROY()
ON_WM_CREATE()
ON_WM_LBUTTONUP()
ON_WM_LBUTTONDOWN()
ON_WM_MOUSEMOVE()
ON_WM_PAINT()
ON_WM_SIZE()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COpenGLView drawing

void
COpenGLView::OnInitialUpdate()
{
  CScrollView::OnInitialUpdate();

  CSize sizeTotal;
  // TODO: calculate the total size of this view
  sizeTotal.cx = sizeTotal.cy = 500;
  SetScrollSizes(MM_TEXT, sizeTotal);
}

void
COpenGLView::OnDraw(CDC *pDC)
{
  CDocument *pDoc = GetDocument();
  // TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// COpenGLView diagnostics

#ifdef _DEBUG
void
COpenGLView::AssertValid() const
{
  CScrollView::AssertValid();
}

void
COpenGLView::Dump(CDumpContext &dc) const
{
  CScrollView::Dump(dc);
}
#endif  //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// COpenGLView message handlers

void
COpenGLView::OnDestroy()
{
  CScrollView::OnDestroy();

  if (wglGetCurrentContext() != NULL) wglMakeCurrent(NULL, NULL);

  if (m_hGLContext != NULL) {
    wglDeleteContext(m_hGLContext);
    m_hGLContext = NULL;
  }
}

int
COpenGLView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (CScrollView::OnCreate(lpCreateStruct) == -1) return -1;

  HWND hWnd = GetSafeHwnd();
  // hDC = ::GetDC(hWnd);
  CDC *cdc = GetDC();
  hDC = cdc->m_hDC;

  m_hGLContext = NULL;
  m_GLPixelIndex = 0;

  m_xRotate = -70;
  m_yRotate = 0;
  m_zRotate = 140;

  m_size = (float)1.5;
  m_exp = 8;

  if (SetWindowPixelFormat(hDC) == FALSE) return 0;

  int nBPP = ::GetDeviceCaps(hDC, BITSPIXEL);

  // If we're running in a palette mode, generate the palette
  if (nBPP < 16) {
    CPalette pal;
    glEnable(GL_DITHER);
    pal.m_hObject = NULL;
    pal.CreatePalette((LOGPALETTE *)&rgb8palette);

    ::SelectPalette(hDC, (HPALETTE)pal, FALSE);
  }

  // hDCFrontBuffer = hDC;

  if (renderToDIB) {
    // hDC = CreateCompatibleDC(NULL);
    setupDIB(hDC);
  }

  setupPalette(hDC);
  // resize();

  if (CreateViewGLContext(hDC) == FALSE) return 0;

  COLORREF color = ::GetSysColor(COLOR_WINDOW);
  glClearColor((float)GetRValue(color) / 255.0f,
               (float)GetGValue(color) / 255.0f,
               (float)GetBValue(color) / 255.0f, (float)1.0);

  return 0;
}

void
COpenGLView::OnLButtonUp(UINT nFlags, CPoint point)
{
  m_LeftButtonDown = FALSE;
  m_ShiftDown = FALSE;

  CScrollView::OnLButtonUp(nFlags, point);
}

void
COpenGLView::OnLButtonDown(UINT nFlags, CPoint point)
{
  m_LeftButtonDown = TRUE;
  m_LeftDownPos = point;
  m_ShiftDown = (nFlags & MK_CONTROL) ? TRUE : FALSE;

  CScrollView::OnLButtonDown(nFlags, point);
}

void
COpenGLView::OnMouseMove(UINT nFlags, CPoint point)
{
  if (m_LeftButtonDown) {
    CSize rotate = m_LeftDownPos - point;
    m_LeftDownPos = point;

    m_xRotate -= rotate.cy;
    if (m_ShiftDown) {
      m_zRotate -= rotate.cx;
    }
    else {
      m_yRotate -= rotate.cx;
    }
    InvalidateRect(NULL, FALSE);
  }

  CScrollView::OnMouseMove(nFlags, point);
}

void
COpenGLView::OnPaint()
{
  if (IsIconic()) {
  }
  else {
    CPaintDC dc(this);  // device context for painting
    RenderScene(&dc);
    CScrollView::OnPaint();
  }
}

void
COpenGLView::OnSize(UINT nType, int cx, int cy)
{
  CScrollView::OnSize(nType, cx, cy);

  GLsizei width, height;
  GLdouble aspect;

  width = cx;
  height = cy;

  if (cy == 0)
    aspect = (GLdouble)width;
  else
    aspect = (GLdouble)width / (GLdouble)height;

  winWidth = width;
  winHeight = height;

  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45, aspect, 0.3, 100.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glDrawBuffer(GL_BACK);

  // glEnable(GL_LIGHTING);
  glEnable(GL_DEPTH_TEST);
}

BOOL
COpenGLView::CreateViewGLContext(HDC hdc)
{
  m_hGLContext = wglCreateContext(hdc);

  if (m_hGLContext == NULL) {
    AfxMessageBox("Cannot Make OpenGL Context");
    return FALSE;
  }

  if (wglMakeCurrent(hdc, m_hGLContext) == FALSE) return FALSE;

  return TRUE;
}

BOOL
COpenGLView::SetWindowPixelFormat(HDC hDC)
{
  PIXELFORMATDESCRIPTOR pixelDesc;

  pixelDesc.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  pixelDesc.nVersion = 1;

  pixelDesc.dwFlags = PFD_SUPPORT_OPENGL | PFD_STEREO_DONTCARE;

  BOOL colorIndexMode = FALSE;

  pixelDesc.cColorBits = GetDeviceCaps(hDC, BITSPIXEL);

  if (colorIndexMode) {
    pixelDesc.iPixelType = PFD_TYPE_COLORINDEX;
  }
  else {
    pixelDesc.iPixelType = PFD_TYPE_RGBA;
  }

  if (doubleBuffered) {
    pixelDesc.dwFlags |= PFD_DOUBLEBUFFER;
  }

  if (renderToDIB) {
    pixelDesc.dwFlags |= PFD_DRAW_TO_BITMAP;
  }
  else {
    pixelDesc.dwFlags |= PFD_DRAW_TO_WINDOW;
  }

  // pixelDesc.iPixelType = PFD_TYPE_RGBA;
  // pixelDesc.iPixelType = PFD_TYPE_COLORINDEX;
  // pixelDesc.cColorBits = 8;
  // pixelDesc.cDepthBits = 16;
  // pixelDesc.cStencilBits = 0;

  /*
  pixelDesc.cRedBits = 8;
  pixelDesc.cRedShift = 16;
  pixelDesc.cGreenBits = 8;
  pixelDesc.cGreenShift = 8;
  pixelDesc.cBlueBits = 8;
  pixelDesc.cBlueShift = 0;
  pixelDesc.cAlphaBits = 0;
  pixelDesc.cAlphaShift = 0;
  pixelDesc.cAccumBits = 64;
  pixelDesc.cAccumRedBits = 16;
  pixelDesc.cAccumGreenBits = 16;
  pixelDesc.cAccumBlueBits = 16;
  pixelDesc.cAccumAlphaBits = 0;

  pixelDesc.cStencilBits = 8;
  pixelDesc.cAuxBuffers = 0;
  pixelDesc.iLayerType = PFD_MAIN_PLANE;
  pixelDesc.bReserved = 0;
  pixelDesc.dwLayerMask = 0;
  pixelDesc.dwVisibleMask = 0;
  pixelDesc.dwDamageMask = 0;
  */

  m_GLPixelIndex = ChoosePixelFormat(hDC, &pixelDesc);
  if (m_GLPixelIndex == 0)  // Choose default
  {
    m_GLPixelIndex = 1;
    if (DescribePixelFormat(hDC, m_GLPixelIndex, sizeof(PIXELFORMATDESCRIPTOR),
                            &pixelDesc) == 0) {
      return FALSE;
    }
  }
  if (SetPixelFormat(hDC, m_GLPixelIndex, &pixelDesc) == FALSE) {
    return FALSE;
  }
  return TRUE;
}

void
COpenGLView::getLookPoint()
{
  glTranslated(0.0, 0.0, -dist);
  glRotated(m_xRotate, 1.0, 0.0, 0.0);
  glRotated(m_yRotate, 0.0, 1.0, 0.0);
  glRotated(m_zRotate, 0.0, 0.0, 1.0);

  /*
  gluLookAt(0.0f,0.0f,0.0f,
                         1.0f,0.0f,0.0f,
                         0.0f,0.0f,1.0f);
  */
}

void
COpenGLView::RenderScene(CPaintDC *dc)
{
  // prepare a semaphore
  static BOOL bBusy = FALSE;
  // use the semaphore to enter this critic section
  if (bBusy) return;
  bBusy = TRUE;

  GLfloat LightAmbient[] = {0.9f, 0.9f, 0.9f, 0.9f};
  GLfloat LightDiffuse[] = {0.7f, 0.7f, 0.7f, 0.7f};
  GLfloat LightSpecular[] = {0.5f, 0.5f, 0.5f, 0.1f};
  GLfloat LightPosition0[] = {0.0f, 0.0f, 1.0f, 0.0f};

  // GLfloat RedSurface[]   = { 1.0f, 0.0f, 0.0f, 1.0f};
  // GLfloat GreenSurface[] = { 0.0f, 1.0f, 0.0f, 1.0f};
  // GLfloat BlueSurface[]  = { 0.0f, 0.0f, 1.0f, 1.0f};
  // GLfloat WhiteSurface[]  = { 1.0f, 1.0f, 1.0f, 1.0f};

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // glClear(GL_COLOR_BUFFER_BIT);

  // LIGHT0 parameters
  // glLightfv(GL_LIGHT0,GL_AMBIENT,LightAmbient);
  // glLightfv(GL_LIGHT0,GL_DIFFUSE,LightDiffuse);
  // glLightfv(GL_LIGHT0,GL_SPECULAR,LightSpecular);
  // glLightfv(GL_LIGHT0,GL_POSITION,LightPosition0);
  // glEnable(GL_LIGHT0);
  // GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 0.1 };
  // GLfloat light_diffuse[] = { 0.1, 0.1, 0.1, 0.5 };
  // GLfloat light_specular[] = { 0.1, 0.0, 0.0, 0.1 };
  /*      light_position is NOT default value     */
  // GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.1 };

  // glLightfv (GL_LIGHT0, GL_AMBIENT, light_ambient);
  // glLightfv (GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  // glLightfv (GL_LIGHT0, GL_SPECULAR, light_specular);
  // glLightfv (GL_LIGHT0, GL_POSITION, light_position);

  // glEnable (GL_LIGHTING);
  // glEnable (GL_LIGHT0);
  // glDepthFunc(GL_LESS);
  // glEnable(GL_DEPTH_TEST);

  // GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
  // GLfloat mat_shininess[] = { 50.0 };
  // GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };

  // glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  // glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
  // glLightfv(GL_LIGHT0, GL_POSITION, light_position);

  // glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glDepthFunc(GL_LESS);
  glEnable(GL_DEPTH_TEST);

  glPushMatrix();

  // Position / translation
  // glTranslated(0.0,0.0,-dist);
  // glRotated(m_xRotate, 1.0, 0.0, 0.0);
  // glRotated(m_yRotate, 0.0, 1.0, 0.0);
  // glRotated(m_zRotate, 0.0, 0.0, 1.0);

  getLookPoint();

  // glEnable(GL_LINE_SMOOTH);
  // glEnable(GL_POLYGON_SMOOTH);

  // Material aspect
  GLfloat mat_ambient[] = {0.7f, 0.7f, 0.7f, 1.0f};
  GLfloat mat_ambient_color[] = {0.8f, 0.8f, 0.8f, 1.0f};
  GLfloat mat_diffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
  // GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
  GLfloat high_shininess[] = {50.0f};
  GLfloat mat_emission[] = {0.2f, 0.2f, 0.2f, 0.0f};

  // glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
  // glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
  // glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  // glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
  // glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);

  glPolygonMode(GL_FRONT, GL_FILL);
  glPolygonMode(GL_BACK, GL_FILL);

  float x = m_size;
  glShadeModel(GL_SMOOTH);
  // glEnable(GL_LIGHTING);

  OpenGLDrawScene(dc->m_ps.hdc);

  // SwapBuffers(dc->m_ps.hdc);

  glPopMatrix();

  bBusy = FALSE;
}

/*
void COpenGLView::OpenGLDrawScene(CPaintDC *dc)
{
         AfxMessageBox("Empty Base OpenGLDrawScene\n");
}
*/
void
COpenGLView::OpenGLDrawScene(HDC hDC)
{
  AfxMessageBox("Empty Base OpenGLDrawScene\n");
}

void
COpenGLView::OnOut()
{
  scaler *= 0.8;
  dist *= 1.2;
}

void
COpenGLView::OnIn()
{
  scaler *= 1.2;
  dist *= 0.8;
}

void
COpenGLView::OnZoomHome()
{
  scaler = 1.0;
  dist = 50.0;
}

void
COpenGLView::setupDIB(HDC hDC)
{
  BITMAPINFO *bmInfo;
  BITMAPINFOHEADER *bmHeader;
  UINT usage;
  VOID *base;
  int bmiSize;
  int bitsPerPixel;

  bmiSize = sizeof(*bmInfo);
  bitsPerPixel = GetDeviceCaps(hDC, BITSPIXEL);

  switch (bitsPerPixel) {
    case 8:
      /* bmiColors is 256 WORD palette indices */
      bmiSize += (256 * sizeof(WORD)) - sizeof(RGBQUAD);
      break;
    case 16:
      /* bmiColors is 3 WORD component masks */
      bmiSize += (3 * sizeof(DWORD)) - sizeof(RGBQUAD);
      break;
    case 24:
    case 32:
    default:
      /* bmiColors not used */
      break;
  }

  bmInfo = (BITMAPINFO *)calloc(1, bmiSize);
  bmHeader = &bmInfo->bmiHeader;

  bmHeader->biSize = sizeof(*bmHeader);
  bmHeader->biWidth = winWidth;
  bmHeader->biHeight = winHeight;
  bmHeader->biPlanes = 1; /* must be 1 */
  bmHeader->biBitCount = bitsPerPixel;
  bmHeader->biXPelsPerMeter = 0;
  bmHeader->biYPelsPerMeter = 0;
  bmHeader->biClrUsed = 0;      /* all are used */
  bmHeader->biClrImportant = 0; /* all are important */

  switch (bitsPerPixel) {
    case 8:
      bmHeader->biCompression = BI_RGB;
      bmHeader->biSizeImage = 0;
      usage = DIB_PAL_COLORS;
      /* bmiColors is 256 WORD palette indices */
      {
        WORD *palIndex = (WORD *)&bmInfo->bmiColors[0];
        int i;

        for (i = 0; i < 256; i++) {
          palIndex[i] = i;
        }
      }
      break;
    case 16:
      bmHeader->biCompression = BI_RGB;
      bmHeader->biSizeImage = 0;
      usage = DIB_RGB_COLORS;
      /* bmiColors is 3 WORD component masks */
      {
        DWORD *compMask = (DWORD *)&bmInfo->bmiColors[0];

        compMask[0] = 0xF800;
        compMask[1] = 0x07E0;
        compMask[2] = 0x001F;
      }
      break;
    case 24:
    case 32:
    default:
      bmHeader->biCompression = BI_RGB;
      bmHeader->biSizeImage = 0;
      usage = DIB_RGB_COLORS;
      /* bmiColors not used */
      break;
  }

  hBitmap = CreateDIBSection(hDC, bmInfo, usage, &base, NULL, 0);
  if (hBitmap == NULL) {
    (void)AfxMessageBox("Failed to create DIBSection.", MB_ICONERROR | MB_OK);
    exit(1);
  }

  hOldBitmap = (HBITMAP)SelectObject(hDC, hBitmap);

  SetPixel(hDC, 20, 10, RGB(255, 255, 0));
  SetPixel(hDC, 21, 10, RGB(255, 255, 0));

  free(bmInfo);
}

void
COpenGLView::setupPalette(HDC hDC)
{
  PIXELFORMATDESCRIPTOR pfd;
  LOGPALETTE *pPal;
  int pixelFormat = GetPixelFormat(hDC);
  int paletteSize;

  DescribePixelFormat(hDC, pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

  /*
  ** Determine if a palette is needed and if so what size.
  */
  if (pfd.dwFlags & PFD_NEED_PALETTE) {
    paletteSize = 1 << pfd.cColorBits;
  }
  else if (pfd.iPixelType == PFD_TYPE_COLORINDEX) {
    paletteSize = 4096;
  }
  else {
    return;
  }

  pPal = (LOGPALETTE *)malloc(sizeof(LOGPALETTE) +
                              paletteSize * sizeof(PALETTEENTRY));
  pPal->palVersion = 0x300;
  pPal->palNumEntries = paletteSize;

  if (pfd.iPixelType == PFD_TYPE_RGBA) {
    /*
    ** Fill the logical paletee with RGB color ramps
    */
    int redMask = (1 << pfd.cRedBits) - 1;
    int greenMask = (1 << pfd.cGreenBits) - 1;
    int blueMask = (1 << pfd.cBlueBits) - 1;
    int i;

    for (i = 0; i < paletteSize; ++i) {
      pPal->palPalEntry[i].peRed =
          (((i >> pfd.cRedShift) & redMask) * 255) / redMask;
      pPal->palPalEntry[i].peGreen =
          (((i >> pfd.cGreenShift) & greenMask) * 255) / greenMask;
      pPal->palPalEntry[i].peBlue =
          (((i >> pfd.cBlueShift) & blueMask) * 255) / blueMask;
      pPal->palPalEntry[i].peFlags = 0;
    }
  }
  else {
    /*
    ** Fill the logical palette with color ramps.
    **
    ** Set up the logical palette so that it can be realized
    ** into the system palette as an identity palette.
    **
    ** 1) The default static entries should be present and at the right
    **    location.  The easiest way to do this is to grab them from
    **    the current system palette.
    **
    ** 2) All non-static entries should be initialized to unique values.
    **    The easiest way to do this is to ensure that all of the non-static
    **    entries have the PC_NOCOLLAPSE flag bit set.
    */
    int numRamps = NUM_COLORS;
    int rampSize = (paletteSize - 20) / numRamps;
    int extra = (paletteSize - 20) - (numRamps * rampSize);
    int i, r;

    /*
    ** Initialize static entries by copying them from the
    ** current system palette.
    */
    GetSystemPaletteEntries(hDC, 0, paletteSize, &pPal->palPalEntry[0]);

    /*
    ** Fill in non-static entries with desired colors.
    */
    for (r = 0; r < numRamps; ++r) {
      int rampBase = r * rampSize + 10;
      PALETTEENTRY *pe = &pPal->palPalEntry[rampBase];
      int diffSize = (int)(rampSize * colors[r].ratio);
      int specSize = rampSize - diffSize;

      for (i = 0; i < rampSize; ++i) {
        GLfloat *c0, *c1;
        GLint a;

        if (i < diffSize) {
          c0 = colors[r].amb;
          c1 = colors[r].diff;
          a = (i * 255) / (diffSize - 1);
        }
        else {
          c0 = colors[r].diff;
          c1 = colors[r].spec;
          a = ((i - diffSize) * 255) / (specSize - 1);
        }

        pe[i].peRed = (BYTE)(a * (c1[0] - c0[0]) + 255 * c0[0]);
        pe[i].peGreen = (BYTE)(a * (c1[1] - c0[1]) + 255 * c0[1]);
        pe[i].peBlue = (BYTE)(a * (c1[2] - c0[2]) + 255 * c0[2]);
        pe[i].peFlags = PC_NOCOLLAPSE;
      }

      colors[r].indexes[0] = rampBase;
      colors[r].indexes[1] = rampBase + (diffSize - 1);
      colors[r].indexes[2] = rampBase + (rampSize - 1);
    }

    /*
    ** Initialize any remaining non-static entries.
    */
    for (i = 0; i < extra; ++i) {
      int index = numRamps * rampSize + 10 + i;
      PALETTEENTRY *pe = &pPal->palPalEntry[index];

      pe->peRed = (BYTE)0;
      pe->peGreen = (BYTE)0;
      pe->peBlue = (BYTE)0;
      pe->peFlags = PC_NOCOLLAPSE;
    }
  }

  hPalette = CreatePalette(pPal);
  free(pPal);

  if (hPalette) {
    SelectPalette(hDC, hPalette, FALSE);
    RealizePalette(hDC);
  }
}

void
COpenGLView::resizeDIB(HDC hDC)
{
  SelectObject(hDC, hOldBitmap);
  DeleteObject(hBitmap);
  setupDIB(hDC);
}

void
COpenGLView::SwapAnyBuffers(HDC hdc)
{
  if (renderToDIB) {
    // SwapBuffers(hdc);
    glFinish();
    // SetPixel(hDC,10,10,RGB(255,255,0));
    // BitBlt(hdc, 0, 0, winWidth, winHeight, hDC, 0, 0, SRCCOPY);

    double aspect = winWidth / winHeight;
    double nHeight = 100;
    double nWidth = aspect * nHeight;
    // StretchBlt(hdc, 0, 0, nWidth, nHeight, hDC, 0, 0,
    // winWidth,winHeight,SRCCOPY);
    if (hBitmap != NULL) {
      CBitmap *cbmp = CBitmap::FromHandle(hBitmap);
      BMP *image = new BMP();
      BITMAP bmp;
      cbmp->GetBitmap(&bmp);

      double aspect = bmp.bmWidth / bmp.bmHeight;
      double nHeight = 100;
      double nWidth = aspect * nHeight;

      // HDC hdc = ::GetDC(m_hWnd);
      // HDC memdc = CreateCompatibleDC(::GetDC(m_hWnd));
      // SelectObject(memdc,cbmp);

      // StretchBlt(hdc, 0, 0, nWidth,nHeight, hDC, 0, 0,
      // bmp.bmWidth,bmp.bmHeight,SRCCOPY); StretchBlt(hdc, nWidth, 0,
      // nWidth,nHeight, memdc, 0, 0, bmp.bmWidth,bmp.bmHeight,SRCCOPY);

      // image->Create(bmp.bmWidth,bmp.bmHeight,(unsigned char*)bmp.bmBits);
      // image->Save("testRCR.bmp");
      // renderToDIB = FALSE;
    }
    /*
    GdiFlush();
for(int j=0;j<nHeight;j++)
{
for(int i=0;i<nWidth;i++)
{
             COLORREF clr =  GetPixel(hdc,i,j);
             int xpos = GetRValue(clr);
             int ypos = GetGValue(clr);
             myRGB valX = palette[xpos];
             myRGB valY = palette[ypos];
             COLORREF clrx = RGB(valX.r,valX.g,valX.b);
             COLORREF clry = RGB(valY.r,valY.g,valY.b);

     SetPixel(hdc,nWidth+(i*2),j,clrx);
             SetPixel(hdc,nWidth+(i*2)+1,j,clry);
           }
     }
    */
    GdiFlush();
  }
  else {
    SwapBuffers(hdc);
  }
}
