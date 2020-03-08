
#include "stdafx.h"
#include "Jam.h"

#include "Pallette.h"

JAM::~JAM()
{
	int size = getNumberOfImages();		
    delete[] entry;

	if (jammemory) free(jammemory);
}

JAMEntry::~JAMEntry()
{
	   //if (info) free(info);
	   if (jamBmp) 
	   {
		   delete (jamBmp);
		   jamBmp=NULL;
	   }	 
}

unsigned char *JAM::getImagePtr(int img)
{
	    int offset=0;
	    int imageptr = offset;
		unsigned char *ptr = jamimagememory;
		ptr += imageptr;
		entry[img].img_ptr = ptr;
		return ptr;
}

BOOL JAM::LoadBMPImage( CBitmap& bitmap, int m_Width,int m_Height)
{		
	    int size = getNumberOfImages();

		int width  = getMaxX();
		int height = getMinX();

		for(int i=0;i<size;i++)
		{
			LoadSingleBMPImage(bitmap,0,0,i);
		}
		return TRUE;
}


BOOL JAM::DrawAllJams(CDC *pDC,double currentScale, BOOL drawRcr, BOOL drawOutlines)
{
	  if (!valid)
	  {
			OpenToRead(FALSE);
      }

	  if (valid)
	  {
	  
	  //LoadSingleBMPImage(bmp,256,getMaxY(),0);

	  for(int i=0;i<getNumberOfImages();i++)
	  //for(int i=0;i<7;i++)
      {
	  CBitmap bmp, *poldbmp;
	  LoadSingleBMPImage(bmp,getImageWidth(i), getImageHeight(i),i);

	  CDC memdc;
	   
      // Create a compatible memory DC
      memdc.CreateCompatibleDC( pDC );
 
	  // Select the bitmap into the DC
      poldbmp = memdc.SelectObject(&bmp);

	  int nWidth  = getImageWidth(i);
	  int nHeight = getImageHeight(i);

	  if (currentScale!=1.0)
	  {
		int dwidth  = (int)(nWidth * currentScale);
		int dheight = (int)(nHeight * currentScale);
		int dx = (int)(getImageX(i)*currentScale);
		int dy = (int)(getImageY(i)*currentScale);
		if (drawRcr)
		{
			dx*=2;
			dy*=2;
		}
		pDC->StretchBlt( dx,dy, dwidth,dheight, &memdc, 0,0, nWidth, nHeight,SRCCOPY );
	  }
	  else
	  {
		int x = (int)(getImageX(i)*currentScale);
		int y = (int)(getImageY(i)*currentScale);
	    pDC->BitBlt( x,y, 2*nWidth, nHeight, &memdc, 0,0, SRCCOPY );
	  }
	 
      memdc.SelectObject( &poldbmp );
	  }

	  CPen *yellowPen = new CPen(PS_SOLID,1,RGB(0,0,0));
	  pDC->SelectObject(yellowPen);
	  //pDC->SetBkMode(TRANSPARENT);
	  pDC->SetTextColor(RGB(0,0,0));
	  if (drawOutlines)
	  {
		 for(int i=0;i<getNumberOfImages();i++)
         {
			int x = (int)(currentScale*getImageX(i));
			int y = (int)(currentScale*getImageY(i));
			if (drawRcr)
		    {
			  /*
			  if (i>0)
			  {
				  int py = (int)(currentScale*getImageY(i-1));
				  if (y-py == 1)
				  {
					x=256+x;
					y = py;
				  }
			  }
			  y=y/2;
			  */
			  x = 0;
			  y = i*30;
		    }
			int w = (int)(currentScale*getImageWidth(i));
			int h = (int)(currentScale*getImageHeight(i));
			if (drawRcr)
		    {
			  w*=2;
		    }

		    pDC->MoveTo(x,y); 
			pDC->LineTo(x+w,y); 
			pDC->LineTo(x+w,y+h); 
			pDC->LineTo(x,y+h); 
			pDC->LineTo(x,y); 
			char label[10];
			int id = getImageId(i);
			
			 wsprintf(label,"#%d(%x)",id,id);
			 pDC->TextOut((x+(x+w))/2-10,(y+(y+h))/2-10,label);
		 }
	  }
	  delete yellowPen;

	  }
	  else
	  {
		  AfxMessageBox("Trouble Loading JAM File perhaps I don't know GP2 Location or Jam Does not exist/corrupt");
	  }

	  return TRUE;
}

unsigned char * JAMEntry::AdjustImage(int image_total_size,unsigned char *imgo,BOOL rcr)
{
	if (imageAdjusted) return jamData;

	unsigned char *img     = img_ptr;
	
	//int m_Width     = (rcr) ? 2*info->width : info->width;
	int m_Width = (rcr) ? 1024 : info->width;
	int m_Height = (rcr) ? image_total_size/2 : info->height;
	//int m_Height    = info->height;
	int m_X     = (rcr) ? 0 : info->x;
	int m_Y     = (rcr) ? 0 : info->y;
	int m_ImagePal  = (rcr) ? 0 : info->palette_size_div4;

	jamData = (unsigned char*)malloc(m_Width*m_Height);
	unsigned char * palptr = palette1;
	memset(jamData,0,(m_Width*m_Height));

	TRACE("%x %d %d\n",img_ptr,m_Width,m_Height);

	unsigned char localPal[256];

	if (m_ImagePal > 0)
	{	 
	 for(int i=0;i<m_ImagePal;i++)
     {
		   unsigned char idx = palptr[i];
		   localPal[i] = idx;
	 }
	}
	else
	{
	 for(int i=0;i<256;i++)
     {
		  localPal[i] = i;
	 }
	}
	 
	 int ox = m_X;
	 int oy = m_Y;
	 int step = (rcr) ? 2 : 1;
	 for(int i=0;i<m_Height;i++)
	 {
	  for(int j=0;j<(m_Width);j++)
	  {
		 int idx=0;
		 
		 if (rcr) idx = ox+j+((oy+i)*512);
		 else idx = ox+j+((oy+i)*256);

		 int idxD = (i*m_Width)+j;
		 int da = img[idx];
		 int palcol = localPal[da];
		 
		 
		 jamData[idxD] = palcol;
		 //jamData[idxD] = da;
	  }
	 }

	imageAdjusted = TRUE;
	return jamData;
}

BOOL JAM::LoadSingleBMPImage( CBitmap& bitmap, int m_Width,int m_Height,int index)
{		
	    if (entry[index].jamBmp==NULL) 
		{
			entry[index].jamBmp = new BMP();
		}

		if (rcr) m_Width*=2;
	   		
		if (!valid)
		{
			OpenToRead(FALSE);
		}
		unsigned char *img     = getImagePtr(index);
		unsigned char *palptr  = getPalletePtr(index);
		int m_ImagePal         = getPalleteSize(index);

		RGBQUAD *pal = (RGBQUAD *)malloc(256*sizeof(RGBQUAD));

		int palleteSize = sizeof(palette)/sizeof(palette[0]);

		for(int i=0;i<palleteSize;i++)
		{
			pal[i].rgbRed   = palette[i+1].r;
			pal[i].rgbGreen = palette[i+1].g;
			pal[i].rgbBlue  = palette[i+1].b;
			pal[i].rgbReserved = 0;
		}

		unsigned char *newJamData = entry[index].AdjustImage(header->image_total_size,img,rcr);

		/*
		int count=0;
		int ypos=0;
		for( i=0;i<256;i++)
		{			
			newJamData[((ypos*256)+count)] = i;
			count++;
			if (count>15)
			{
				count = 0;
				ypos++;
			}
		}
		*/

		entry[index].jamBmp->SetPalette(pal);

		//TRACE("w h %d %d\n",m_Width,m_Height);

		entry[index].jamBmp->Create(m_Width,m_Height,newJamData);

		entry[index].jamBmp->CreateDIB(bitmap);
	    
        return TRUE;
}

void
JAM::UnJam(
        void    *ptr,
        unsigned int            len
) 
{
        unsigned long   x;
        unsigned int            n;
        unsigned char   *pc;
        unsigned long   *pl;

        x = 0xb082f164UL;
        n = len / 4;
        x |= 1;
        pl = (unsigned long *) ptr;

        while (n--) {
                *pl ^= x;
                ++pl;
                x = x + (x * 4);
        }

        n = len & (4 - 1);
        if (n != 0) {
                pc = (unsigned char *) pl;
                *pc ^= x & 0xff;
                ++pc;
                x >>= 8;
        }
}

int JAM::GetPoint(int x,int y)
{
	unsigned char *img     = getImagePtr(0);

	img+= (y*256);
	img+= x;

	return *img;
}

BOOL JAM::DrawSingleJams(CDC *pDC,double currentScale,CBitmap &currentJam,BOOL rcr)
{
	  if (!valid)
	  {
			OpenToRead(FALSE);
      }

	  if (valid)
	  {
	  CBitmap bmp, *poldbmp,*decaloldbmp;
	  int nWidth,nHeight;

	  if (rcr)
	  {
	   nWidth  = 512;
	   nHeight = header->image_total_size/2;
	  }
	  else
	  {
	   nWidth  = 256;
	   nHeight = header->image_total_size;
	  }

	  LoadSingleBMPImage(bmp,nWidth, nHeight,0);

	  CDC memdc;
	   
      // Create a compatible memory DC
      memdc.CreateCompatibleDC( pDC );
 
	  // Select the bitmap into the DC
      poldbmp = memdc.SelectObject(&bmp);

	  

	  if (currentScale!=1.0)
	  {
		int dwidth  = (int)(nWidth * currentScale);
		int dheight = (int)(nHeight * currentScale);
		int dx = 0;
		int dy = 0;
		pDC->StretchBlt( dx,dy, dwidth,dheight, &memdc, 0,0, nWidth, nHeight,SRCCOPY );
	  }
	  else
	  {
		int x = 0;
		int y = 0;
		pDC->BitBlt( x,y, nWidth, nHeight, &memdc, 0,0, SRCCOPY );
	  }
	  memdc.SelectObject( &poldbmp );

	  CPen *yellowPen = new CPen(PS_SOLID,1,RGB(255,255,0));
	  pDC->SelectObject(yellowPen);
	  pDC->SetBkMode(TRANSPARENT);
	  pDC->SetTextColor(RGB(255,255,0));
	  if (TRUE)
	  {
		 for(int i=0;i<getNumberOfImages();i++)
         {
			int x = (int)(currentScale*getImageX(i));
			int y = (int)(currentScale*getImageY(i));
			int w = (int)(currentScale*getImageWidth(i));
			int h = (int)(currentScale*getImageHeight(i));

			if (y%2)
			{
				x+=256;
				y=y-1;
			}

			w=w*2;
			y=y/2;
			

		    pDC->MoveTo(x,y); 
			pDC->LineTo(x+w,y); 
			pDC->LineTo(x+w,y+h); 
			pDC->LineTo(x,y+h); 
			pDC->LineTo(x,y); 
			char label[10];
			int id = getImageId(i);
			
			wsprintf(label,"#%d(%x)",id,id);
			pDC->TextOut((x+(x+w))/2-10,(y+(y+h))/2-10,label);
		 }
	  }
	  delete yellowPen;

	  CBitmap img;
	  
	  img.CreateCompatibleBitmap(pDC,(nWidth/2),nHeight);
	  poldbmp = memdc.SelectObject(&img);

	  if (rcr)
	  {
	  CDC memdcDecal;
      memdcDecal.CreateCompatibleDC( pDC );
      decaloldbmp = memdcDecal.SelectObject(&currentJam);

	  unsigned char *ptr = getImagePtr(0);

	  int SmallWidth = nWidth/2;
	  for(int j=0;j<nHeight;j++)
	  {
	   for(int i=0;i<SmallWidth;i++)
	   {
		   int valX = *ptr;
		   ptr++;
		   int valY = *ptr;
		   ptr++;
		   memdc.SetPixel(i,j,memdcDecal.GetPixel(valX,valY));
	   }
	  }

	  pDC->BitBlt( 0,nHeight, SmallWidth, nHeight, &memdc, 0,0, SRCCOPY );
	 
      memdc.SelectObject( &poldbmp );
      memdcDecal.SelectObject( &decaloldbmp );
	  }
	  }

	  return TRUE;
}