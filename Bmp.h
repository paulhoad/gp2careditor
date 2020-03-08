/*
** File   : bmp.h
** Author : tk
** Date   : 04/08/96
**
** $Header:   F:/TK/GP2/GP2JAM/VCS/BMP.H__   1.2   29 Sep 1996 16:38:48   tk  $
**
** Bitmap class.
*/

#ifndef   bmp_h
#define   bmp_h

/*---------------------------------------------------------------------------
** Includes
*/

/*---------------------------------------------------------------------------
** Defines and Macros
*/

/* constants for the biCompression field */
#define BI_RGB      		0L
#define BI_RLE8     		1L
#define BI_RLE4     		2L

/*---------------------------------------------------------------------------
** Typedefs
*/

#pragma pack(1)

/*
typedef struct tagBITMAPFILEHEADER
{
    WORD    bfType;
    DWORD   bfSize;
    WORD    bfReserved1;
    WORD    bfReserved2;
    DWORD   bfOffBits;
} BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER
{
    DWORD   biSize;
    LONG    biWidth;
    LONG    biHeight;
    WORD    biPlanes;
    WORD    biBitCount;
    DWORD   biCompression;
    DWORD   biSizeImage;
    LONG    biXPelsPerMeter;
    LONG    biYPelsPerMeter;
    DWORD   biClrUsed;
    DWORD   biClrImportant;
} BITMAPINFOHEADER;
*/

class BMP {
public:
	BMP();
	~BMP();

	BOOL		Load(const char *filename);
	BOOL		Create(unsigned short width, unsigned short height, unsigned char *pImage = NULL);
	BOOL		Save(const char *filename);
	void		SetPalette(RGBQUAD *pPalette);

	void        CreateDIB(CBitmap &bitmap);

	BOOL		IsBmp();
	BOOL		IsBmpVariantOk();

	long		ImageWidth()			const { return bmInfo.bmiHeader.biWidth; }
	long		ImageHeight()			const { return bmInfo.bmiHeader.biHeight; }

	unsigned char	*Image()				const { return m_pImage; }
	RGBQUAD	*Palette()			{ return m_Palette; }
	void		InvertImage();

private:
	BITMAPFILEHEADER	m_Hdr;
	BITMAPINFO          bmInfo;
	//BITMAPINFOHEADER	m_InfoHdr;
	RGBQUAD			m_Palette[256];
	unsigned char			*m_pImage;
	unsigned long			m_BmpImageSize;
};

/*---------------------------------------------------------------------------
** Data
*/

/*---------------------------------------------------------------------------
** Functions
*/


#endif    /* bmp_h */
/*---------------------------------------------------------------------------
** End of File
*/
