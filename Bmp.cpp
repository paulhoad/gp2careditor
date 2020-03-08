
#include "stdafx.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bmp.h"


BMP::BMP(
	void
) {
	memset(&m_Hdr, 0, sizeof(m_Hdr));
	memset(&bmInfo.bmiHeader, 0, sizeof(bmInfo.bmiHeader));
	memset(m_Palette, 0, sizeof(m_Palette));
	m_pImage		= NULL;
	m_BmpImageSize	= 0;
}

BMP::~BMP(
	void
) {
	if (m_pImage) {
		delete[] m_pImage;
		m_pImage = NULL;
	}
}

BOOL
BMP::Load(
	const char *filename
) {
	FILE	*fp;
	BOOL	ok = TRUE;

	memset(&m_Hdr, 0, sizeof(m_Hdr));
	memset(&bmInfo.bmiHeader, 0, sizeof(bmInfo.bmiHeader));
	memset(m_Palette, 0, sizeof(m_Palette));
	if (m_pImage) {
		delete m_pImage;
		m_pImage = NULL;
	}

	if ((fp = fopen(filename, "rb")) == 0) {
		printf("Unable to open file '%s' for loading.\n", filename);
		return FALSE;
	}

	if (ok && fread(&m_Hdr, sizeof(m_Hdr), 1, fp) != 1) {
		printf("Read #1 failed from file '%s'\n", filename);
		ok = FALSE;
	}

	if (ok && !IsBmp()) {
		printf("The file '%s' is not a bitmap file.\n", filename);
		ok = FALSE;
	}

	if (ok && fread(&bmInfo.bmiHeader, sizeof(bmInfo.bmiHeader), 1, fp) != 1) {
		printf("Read #2 failed from file '%s'\n", filename);
		ok = FALSE;
	}

	/*
	** Calculate some of the fields that may not be filled in!
	*/
	if (ok && bmInfo.bmiHeader.biSizeImage == 0) {
		long size;

		size = (bmInfo.bmiHeader.biWidth * bmInfo.bmiHeader.biBitCount) / 8;
		if (size % 4) {
			size += 4 - (size % 4);
		}
		size *= bmInfo.bmiHeader.biHeight;
		bmInfo.bmiHeader.biSizeImage = (unsigned long) size;
	}

	if (ok && !IsBmpVariantOk()) {
		printf(	"The bitmap file '%s' is stored in the wrong bitmap sub-format.\n"
				"Make sure you save the bitmap with the following options:\n"
				"\n"
				"     256 colours.\n"
				"     Windows RGB encoded (not RLE or OS/2 encoded)\n"
				"\n",
				filename
			);
		ok = FALSE;
	}
	if (ok && fread(m_Palette, sizeof(m_Palette), 1, fp) != 1) {
		printf("Read #3 failed from file '%s'\n", filename);
		ok = FALSE;
	}

	if (ok) {
		m_pImage = new unsigned char[(unsigned int) bmInfo.bmiHeader.biSizeImage];
		if (m_pImage == NULL) {
			printf("Out of memory trying to allocate a buffer for '%s'.\n", filename);
			ok = FALSE;
		}
		else {
			memset(m_pImage, 0, (unsigned int) bmInfo.bmiHeader.biSizeImage);
		}
	}

	/*lint -esym(668,fread) Ignore possible null pointer*/
	if (ok && fread(m_pImage, (unsigned int) bmInfo.bmiHeader.biSizeImage, 1, fp) != 1) {
		printf("Read #4 failed from file '%s'\n", filename);
		ok = FALSE;
	}
	/*lint +esym(668,m_pImage) */

	fclose(fp);
	return ok;
}

BOOL
BMP::Create(
	unsigned short	width,
	unsigned short 	height,
	unsigned char	*pImage
) {
	unsigned long	iw;

	iw = width;

	if ((iw & 0x03) != 0) {
		iw += 4;
		iw &= ~0x03;
	}


	m_BmpImageSize	= iw * height;

	m_Hdr.bfType		= 0x4d42;
	m_Hdr.bfSize		= sizeof(m_Hdr) + sizeof(bmInfo.bmiHeader) + sizeof(m_Palette) + m_BmpImageSize;
	m_Hdr.bfReserved1	= 0;
	m_Hdr.bfReserved2	= 0;
	m_Hdr.bfOffBits	= sizeof(m_Hdr) + sizeof(bmInfo.bmiHeader) + sizeof(m_Palette);

	bmInfo.bmiHeader.biSize            = sizeof(bmInfo.bmiHeader);
	bmInfo.bmiHeader.biWidth           = (long) width;
	bmInfo.bmiHeader.biHeight          = (long) height;
	bmInfo.bmiHeader.biPlanes          = 1;
	bmInfo.bmiHeader.biBitCount        = 8;
	bmInfo.bmiHeader.biCompression     = BI_RGB;
	bmInfo.bmiHeader.biSizeImage       = m_BmpImageSize;
	bmInfo.bmiHeader.biXPelsPerMeter   = 0;
	bmInfo.bmiHeader.biYPelsPerMeter   = 0;
	bmInfo.bmiHeader.biClrUsed         = 256;
	bmInfo.bmiHeader.biClrImportant    = 256;

	m_pImage = new unsigned char[(unsigned int) m_BmpImageSize];
	if (m_pImage == NULL) {
		printf("Out of memory trying to allocate a buffer for BMP file.\n");
		return FALSE;
	}
	memset(m_pImage, 0, (unsigned int) m_BmpImageSize);

	if (pImage) {
		unsigned int	y;

		for (y = 0; y < height; y++) {
			memcpy(&m_pImage[y * iw], &pImage[y * width], width);
		}
	}

	InvertImage();

	return TRUE;
}

void BMP::CreateDIB(CBitmap &bitmap)
{
        CClientDC dc(NULL);

		int paletteSize = 256;
		LOGPALETTE *pPal = (LOGPALETTE*)
        malloc(sizeof(LOGPALETTE) + paletteSize * sizeof(PALETTEENTRY));
        pPal->palVersion = 0x300;
        pPal->palNumEntries = paletteSize;

		for (int i=0; i<paletteSize; i++) 
		{
		  pPal->palPalEntry[i].peRed = m_Palette[i].rgbRed;
		  pPal->palPalEntry[i].peGreen = m_Palette[i].rgbGreen;
		  pPal->palPalEntry[i].peBlue = m_Palette[i].rgbBlue;
		  pPal->palPalEntry[i].peFlags = 0;
		}

		//pPal->palPalEntry = m_Palette;


		//HPALETTE hPalette = CreatePalette(pPal);
		//::SelectPalette(dc.m_hDC, hPalette, FALSE);
        //::RealizePalette(dc.m_hDC);
        
        HBITMAP hBmp = CreateDIBitmap( dc.m_hDC,                // handle to device context 
                                &bmInfo.bmiHeader,     // pointer to bitmap size and format data 
                                CBM_INIT,       // initialization flag 
                                m_pImage,      // pointer to initialization data 
                                &bmInfo,        // pointer to bitmap color-format data 
                                DIB_RGB_COLORS);                // color-data usage 
        bitmap.Attach( hBmp );
}

BOOL
BMP::Save(
	const char *filename
) {
	FILE	*fp;
	BOOL	ok;

	if ((fp = fopen(filename, "wb")) == 0) {
		printf("Unable to open file '%s' for saving.\n", filename);
		return FALSE;
	}

	ok = TRUE;
	if (fwrite(&m_Hdr, sizeof(m_Hdr), 1, fp) != 1) {
		ok = FALSE;
	}

	if (ok && fwrite(&bmInfo.bmiHeader, sizeof(bmInfo.bmiHeader), 1, fp) != 1) {
		ok = FALSE;
	}

	if (ok && fwrite(m_Palette, sizeof(m_Palette), 1, fp) != 1) {
		ok = FALSE;
	}

	InvertImage();
	if (ok && fwrite(m_pImage, (unsigned int) m_BmpImageSize, 1, fp) != 1) {
		ok = FALSE;
	}
	InvertImage();

	fclose(fp);
	if (!ok) {
		printf("Write to '%s' failed, disk full?\n", filename);
		return FALSE;
	}
	return TRUE;
}

void
BMP::SetPalette(
	RGBQUAD *pPalette
) {
	memcpy(m_Palette, pPalette, sizeof(m_Palette));
}

BOOL
BMP::IsBmp(
	void
) {
	return (m_Hdr.bfType == 0x4d42);
}

BOOL
BMP::IsBmpVariantOk(
	void
) {
	return (	IsBmp() 					    	&&
			bmInfo.bmiHeader.biPlanes 		== 1	    	&&
			bmInfo.bmiHeader.biBitCount 	== 8	    	&&
			bmInfo.bmiHeader.biCompression 	== BI_RGB
		);
}

void
BMP::InvertImage(
	void
) {
	unsigned char	*pTmpBuff	= NULL;
	unsigned short	iw;
	unsigned short	i;
	unsigned short	n;

	iw = (unsigned short) bmInfo.bmiHeader.biWidth;

	if ((iw & 0x03) != 0) {
		iw += 4;
		iw &= ~0x03;
	}

	pTmpBuff = new unsigned char[(unsigned int) iw];
	if (pTmpBuff == NULL) {
		printf("Out of memory trying to allocate invert buffer.\n");
		exit(1);
	}

	for (i = 0, n = (unsigned short) bmInfo.bmiHeader.biHeight - 1; i < (unsigned short) bmInfo.bmiHeader.biHeight / 2; i++, n--) {
		memcpy(pTmpBuff, 			&m_pImage[i * iw], 		iw);
		memcpy(&m_pImage[i * iw], 	&m_pImage[n * iw],		iw);
		memcpy(&m_pImage[n * iw], 	pTmpBuff,				iw);
	}

	delete[] pTmpBuff;
}

/*---------------------------------------------------------------------------
** End of File
*/
